/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "halcs_client.h"
/* Private headers */
#include "errhand.h"
#include "halcs_client_rw_param_codes.h"
#include "halcs_client_codes.h"
#include "halcs_client_revision.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, LIB_CLIENT, "[libclient]",\
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_ALLOC(ptr, LIB_CLIENT, "[libclient]",        \
            halcs_client_err_str(HALCS_CLIENT_ERR_ALLOC),       \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                            \
    CHECK_HAL_ERR(err, LIB_CLIENT, "[libclient]",           \
            halcs_client_err_str (err_type))

#define HALCSCLIENT_DFLT_LOG_MODE             "w"
#define HALCSCLIENT_MLM_CONNECT_TIMEOUT       1000        /* in ms */
#define HALCSCLIENT_DFLT_TIMEOUT              1000        /* in ms */

/* Our structure */
struct _halcs_client_t {
    zuuid_t * uuid;                             /* Client UUID */
    mlm_client_t *mlm_client;                   /* Malamute client instance */
    int timeout;                                /* Timeout in msec for send/recv */
    zpoller_t *poller;                          /* Poller for receiving messages */
};

static halcs_client_t *_halcs_client_new (char *broker_endp, int verbose,
        const char *log_file_name, const char *log_mode, int timeout);
static halcs_client_err_e _func_polling (halcs_client_t *self, char *name,
        char *service, uint32_t *input, uint32_t *output, int timeout);


/********************************************************/
/************************ Our API ***********************/
/********************************************************/
halcs_client_t *halcs_client_new (char *broker_endp, int verbose,
        const char *log_file_name)
{
    return _halcs_client_new (broker_endp, verbose, log_file_name,
            HALCSCLIENT_DFLT_LOG_MODE, HALCSCLIENT_DFLT_TIMEOUT);
}

halcs_client_t *halcs_client_new_log_mode (char *broker_endp, int verbose,
        const char *log_file_name, const char *log_mode)
{
    return _halcs_client_new (broker_endp, verbose, log_file_name,
            log_mode, HALCSCLIENT_DFLT_TIMEOUT);
}

halcs_client_t *halcs_client_new_log_mode_time (char *broker_endp, int verbose,
        const char *log_file_name, const char *log_mode, int timeout)
{
    return _halcs_client_new (broker_endp, verbose, log_file_name,
            log_mode, timeout);
}

halcs_client_t *halcs_client_new_time (char *broker_endp, int verbose,
        const char *log_file_name, int timeout)
{
    return _halcs_client_new (broker_endp, verbose, log_file_name,
            HALCSCLIENT_DFLT_LOG_MODE, timeout);
}

void halcs_client_destroy (halcs_client_t **self_p)
{
    assert (self_p);

    if (*self_p) {
        halcs_client_t *self = *self_p;

        zpoller_destroy (&self->poller);
        mlm_client_destroy (&self->mlm_client);
        zuuid_destroy (&self->uuid);
        free (self);
        *self_p = NULL;

        errhand_log_destroy ();
    }
}

/*************************** Acessor methods *****************************/

zpoller_t *halcs_client_get_poller (halcs_client_t *self)
{
    return self->poller;
}

halcs_client_err_e halcs_client_set_timeout (halcs_client_t *self, int timeout)
{
    halcs_client_err_e err = HALCS_CLIENT_SUCCESS;
    self->timeout = timeout;
    return err;
}

uint32_t halcs_client_get_timeout (halcs_client_t *self)
{
    return self->timeout;
}

/**************** Static LIB Client Functions ****************/
static halcs_client_t *_halcs_client_new (char *broker_endp, int verbose,
        const char *log_file_name, const char *log_mode, int timeout)
{
    UNUSED(verbose);

    assert (broker_endp);

    /* Set logfile available for all dev_mngr and dev_io instances.
     * We accept NULL as a parameter, meaning to suppress all messages */
    errhand_log_new (log_file_name, log_mode);

    DBE_DEBUG (DBG_LIB_CLIENT | DBG_LVL_INFO, "[libclient] Spawing LIBHALCSCLIENT"
            " with broker address %s, with logfile on %s ...\n", broker_endp,
            (log_file_name == NULL) ? "NULL" : log_file_name);

    /* Print Software info */
    DBE_DEBUG (DBG_LIB_CLIENT | DBG_LVL_INFO, "[libclient] HALCS Client version %s,"
            " Build by: %s, %s\n",
            revision_get_build_version (),
            revision_get_build_user_name (),
            revision_get_build_date ());

    halcs_client_t *self = zmalloc (sizeof *self);
    ASSERT_ALLOC(self, err_self_alloc);

    /* Generate UUID to work with MLM broker */
    self->uuid = zuuid_new ();
    ASSERT_ALLOC(self->uuid, err_uuid_alloc);

    self->mlm_client = mlm_client_new ();
    ASSERT_TEST(self->mlm_client!=NULL, "Could not create MLM client",
            err_mlm_client);

    /* Connect to broker with current UUID address in canonical form */
    int rc = mlm_client_connect (self->mlm_client, broker_endp,
            HALCSCLIENT_MLM_CONNECT_TIMEOUT, zuuid_str_canonical (self->uuid));
    ASSERT_TEST(rc >= 0, "Could not connect MLM client to broker", err_mlm_connect);

    /* Get MLM socket for use with poller */
    zsock_t *msgpipe = mlm_client_msgpipe (self->mlm_client);
    ASSERT_TEST (msgpipe != NULL, "Invalid MLM client socket reference",
            err_mlm_inv_client_socket);
    /* Initialize poller */
    self->poller = zpoller_new (msgpipe, NULL);
    ASSERT_TEST (self->poller != NULL, "Could not Initialize poller",
            err_init_poller);

    /* Initialize timeout */
    self->timeout = timeout;

    return self;

err_init_poller:
err_mlm_inv_client_socket:
err_mlm_connect:
    mlm_client_destroy (&self->mlm_client);
err_mlm_client:
    zuuid_destroy (&self->uuid);
err_uuid_alloc:
    free (self);
err_self_alloc:
    return NULL;
}

/**************** General Function to call the others *********/

halcs_client_err_e halcs_func_exec_size (halcs_client_t *self, const disp_op_t *func,
        char *service, uint32_t *input, uint32_t *output, uint32_t output_size)
{
    halcs_client_err_e err = HALCS_CLIENT_SUCCESS;
    uint8_t *input8 = (uint8_t *) input;
    uint8_t *output8 = (uint8_t *) output;

    /* Check input arguments */
    ASSERT_TEST(self != NULL, "Bpm_client is NULL", err_null_exp,
            HALCS_CLIENT_ERR_INV_FUNCTION);
    ASSERT_TEST(func != NULL, "Function structure is NULL", err_null_exp,
            HALCS_CLIENT_ERR_INV_FUNCTION);
    ASSERT_TEST(!(func->args[0] != DISP_ARG_END && input8 == NULL),
            "Invalid input arguments!", err_inv_param, HALCS_CLIENT_ERR_INV_PARAM);
    ASSERT_TEST(!(func->retval != DISP_ARG_END && output8 == NULL),
            "Invalid output arguments!", err_inv_param, HALCS_CLIENT_ERR_INV_PARAM);

    /* Create the message */
    zmsg_t *msg = zmsg_new ();
    ASSERT_ALLOC(msg, err_msg_alloc, HALCS_CLIENT_ERR_ALLOC);

    /* Add the frame containing the opcode for the function desired (always first) */
    zmsg_addmem (msg, &func->opcode, sizeof (func->opcode));

    /* Add the arguments in their respective frames in the message */
    for (int i = 0; func->args[i] != DISP_ARG_END; ++i) {
        /* Get the size of the message being sent */
        uint32_t in_size = DISP_GET_ASIZE(func->args[i]);
        /* Create a frame to compose the message */
        zmsg_addmem (msg, input8, in_size);
        /* Moves along the pointer */
        input8 += in_size;
    }

    mlm_client_sendto (self->mlm_client, service, NULL, NULL, 0, &msg);

    /* Receive report */
    zmsg_t *report = param_client_recv_timeout (self);
    ASSERT_TEST(report != NULL, "Report received is NULL", err_msg);

    /* Message is:
     * frame 0: Error code
     * frame 1: Number of bytes received
     * frame 2+: Data received      */

    /* Handling malformed messages */
    size_t msg_size = zmsg_size (report);
    ASSERT_TEST(msg_size == MSG_ERR_CODE_SIZE || msg_size == MSG_FULL_SIZE,
            "Unexpected message received", err_msg_size);

    /* Get message contents */
    zframe_t *err_code = zmsg_pop(report);
    ASSERT_TEST(err_code != NULL, "Could not receive error code", err_msg_code);
    err = *(uint32_t *)zframe_data(err_code);

    zframe_t *data_size_frm = NULL;
    zframe_t *data_frm = NULL;
    if (msg_size == MSG_FULL_SIZE)
    {
        data_size_frm = zmsg_pop (report);
        ASSERT_TEST(data_size_frm != NULL, "Could not receive data size", err_null_data_size);
        data_frm = zmsg_pop (report);
        ASSERT_TEST(data_frm != NULL, "Could not receive data", err_null_data);

        ASSERT_TEST(zframe_size (data_size_frm) == RW_REPLY_SIZE,
                "Wrong <number of payload bytes> parameter size", err_msg_fmt);

        /* Size in the second frame must match the frame size of the third */
        RW_REPLY_TYPE data_size = *(RW_REPLY_TYPE *) zframe_data(data_size_frm);
        ASSERT_TEST(data_size == zframe_size (data_frm),
                "<payload> parameter size does not match size in <number of payload bytes> parameter",
                err_msg_fmt);

        uint32_t *data_out = (uint32_t *)zframe_data(data_frm);
        /* Copy only the user specified ammount of bytes if this is less than the message size */
        uint32_t output8_size = (output_size > data_size)? data_size : output_size;
        memcpy (output8, data_out, output8_size);
    }

err_msg_fmt:
    zframe_destroy (&data_frm);
err_null_data:
    zframe_destroy (&data_size_frm);
err_null_data_size:
    zframe_destroy (&err_code);
err_msg_code:
    zmsg_destroy (&report);
err_msg_size:
err_msg:
    zmsg_destroy (&msg);
err_msg_alloc:
err_null_exp:
err_inv_param:
    return err;
}

halcs_client_err_e halcs_func_exec (halcs_client_t *self, const disp_op_t *func,
        char *service, uint32_t *input, uint32_t *output)
{
    /* Use the specified size in disp_op_t structure */
    uint32_t output_size = DISP_GET_ASIZE(func->retval);
    return halcs_func_exec_size (self, func, service, input, output, output_size);
}

const disp_op_t *halcs_func_translate (char *name)
{
    assert (name);

    /* Search the function table for a match in the 'name' field */
    for (int i=0; smio_exp_ops[i] != NULL; i++) {
        for (int j=0; smio_exp_ops[i][j] != NULL; j++) {
            if (streq(name, smio_exp_ops[i][j]->name)) {
                return smio_exp_ops[i][j];
            }
        }
    }

    return 0;
}

halcs_client_err_e halcs_func_trans_exec (halcs_client_t *self, char *name, char *service, uint32_t *input, uint32_t *output)
{
    const disp_op_t *func = halcs_func_translate(name);
    halcs_client_err_e err = halcs_func_exec (self, func, service, input, output);
    return err;
}

/********************** Accessor Methods **********************/

mlm_client_t *halcs_get_mlm_client (halcs_client_t *self)
{
    assert (self);
    return self->mlm_client;
}

/**************** FMC ADC COMMON SMIO Functions ****************/

PARAM_FUNC_CLIENT_WRITE(fmc_leds)
{
    return param_client_write (self, service, FMC_ADC_COMMON_OPCODE_LEDS,
            fmc_leds);
}

PARAM_FUNC_CLIENT_READ(fmc_leds)
{
    return param_client_read (self, service, FMC_ADC_COMMON_OPCODE_LEDS,
            fmc_leds);
}

PARAM_FUNC_CLIENT_WRITE(adc_test_data_en)
{
    return param_client_write (self, service, FMC_ADC_COMMON_OPCODE_TEST_DATA_EN, adc_test_data_en);
}

PARAM_FUNC_CLIENT_READ(adc_test_data_en)
{
     return param_client_read (self, service, FMC_ADC_COMMON_OPCODE_TEST_DATA_EN, adc_test_data_en);
}

PARAM_FUNC_CLIENT_WRITE(trig_dir)
{
    return param_client_write (self, service, FMC_ADC_COMMON_OPCODE_TRIG_DIR, trig_dir);
}

PARAM_FUNC_CLIENT_READ(trig_dir)
{
    return param_client_read (self, service, FMC_ADC_COMMON_OPCODE_TRIG_DIR, trig_dir);
}

PARAM_FUNC_CLIENT_WRITE(trig_term)
{
    return param_client_write (self, service, FMC_ADC_COMMON_OPCODE_TRIG_TERM, trig_term);
}

PARAM_FUNC_CLIENT_READ(trig_term)
{
    return param_client_read (self, service, FMC_ADC_COMMON_OPCODE_TRIG_TERM, trig_term);
}

PARAM_FUNC_CLIENT_WRITE(trig_val)
{
    return param_client_write (self, service, FMC_ADC_COMMON_OPCODE_TRIG_VAL, trig_val);
}

PARAM_FUNC_CLIENT_READ(trig_val)
{
    return param_client_read (self, service, FMC_ADC_COMMON_OPCODE_TRIG_VAL, trig_val);
}

/**************** FMC ACTIVE CLOCK SMIO Functions ****************/

/* FUNCTION pin functions */
PARAM_FUNC_CLIENT_WRITE(fmc_pll_function)
{
    return param_client_write (self, service, FMC_ACTIVE_CLK_OPCODE_PLL_FUNCTION,
            fmc_pll_function);
}

PARAM_FUNC_CLIENT_READ(fmc_pll_function)
{
    return param_client_read (self, service, FMC_ACTIVE_CLK_OPCODE_PLL_FUNCTION,
            fmc_pll_function);
}

/* STATUS pin functions */
PARAM_FUNC_CLIENT_WRITE(fmc_pll_status)
{
    return param_client_write (self, service, FMC_ACTIVE_CLK_OPCODE_PLL_STATUS,
            fmc_pll_status);
}

PARAM_FUNC_CLIENT_READ(fmc_pll_status)
{
    return param_client_read (self, service, FMC_ACTIVE_CLK_OPCODE_PLL_STATUS,
            fmc_pll_status);
}

/* CLK_SEL pin functions */
PARAM_FUNC_CLIENT_WRITE(fmc_clk_sel)
{
    return param_client_write (self, service, FMC_ACTIVE_CLK_OPCODE_CLK_SEL,
            fmc_clk_sel);
}

PARAM_FUNC_CLIENT_READ(fmc_clk_sel)
{
    return param_client_read (self, service, FMC_ACTIVE_CLK_OPCODE_CLK_SEL,
            fmc_clk_sel);
}

/* RESET ISLA250P functions */
PARAM_FUNC_CLIENT_WRITE(rst_isla216p)
{
    return param_client_write (self, service, FMC_ACTIVE_CLK_OPCODE_RST_ISLA216P,
            rst_isla216p);
}

PARAM_FUNC_CLIENT_WRITE(si571_oe)
{
    return param_client_write (self, service, FMC_ACTIVE_CLK_OPCODE_SI571_OE, si571_oe);
}

PARAM_FUNC_CLIENT_READ(si571_oe)
{
     return param_client_read (self, service, FMC_ACTIVE_CLK_OPCODE_SI571_OE, si571_oe);
}

PARAM_FUNC_CLIENT_WRITE(ad9510_defaults)
{
    return param_client_write (self, service, FMC_ACTIVE_CLK_OPCODE_AD9510_CFG_DEFAULTS,
            ad9510_defaults);
}

PARAM_FUNC_CLIENT_READ(ad9510_defaults)
{
    return param_client_read (self, service, FMC_ACTIVE_CLK_OPCODE_AD9510_CFG_DEFAULTS,
            ad9510_defaults);
}

/* AD9510 PLL A divider */
PARAM_FUNC_CLIENT_WRITE(ad9510_pll_a_div)
{
    return param_client_write (self, service, FMC_ACTIVE_CLK_OPCODE_AD9510_PLL_A_DIV,
            ad9510_pll_a_div);
}

PARAM_FUNC_CLIENT_READ(ad9510_pll_a_div)
{
    return param_client_read (self, service, FMC_ACTIVE_CLK_OPCODE_AD9510_PLL_A_DIV,
            ad9510_pll_a_div);
}

/* AD9510 PLL B divider */
PARAM_FUNC_CLIENT_WRITE(ad9510_pll_b_div)
{
    return param_client_write (self, service, FMC_ACTIVE_CLK_OPCODE_AD9510_PLL_B_DIV,
            ad9510_pll_b_div);
}

PARAM_FUNC_CLIENT_READ(ad9510_pll_b_div)
{
    return param_client_read (self, service, FMC_ACTIVE_CLK_OPCODE_AD9510_PLL_B_DIV,
            ad9510_pll_b_div);
}

/* AD9510 PLL Prescaler */
PARAM_FUNC_CLIENT_WRITE(ad9510_pll_prescaler)
{
    return param_client_write (self, service, FMC_ACTIVE_CLK_OPCODE_AD9510_PLL_PRESCALER,
            ad9510_pll_prescaler);
}

PARAM_FUNC_CLIENT_READ(ad9510_pll_prescaler)
{
    return param_client_read (self, service, FMC_ACTIVE_CLK_OPCODE_AD9510_PLL_PRESCALER,
            ad9510_pll_prescaler);
}

/* AD9510 R divider */
PARAM_FUNC_CLIENT_WRITE(ad9510_r_div)
{
    return param_client_write (self, service, FMC_ACTIVE_CLK_OPCODE_AD9510_R_DIV,
            ad9510_r_div);
}

PARAM_FUNC_CLIENT_READ(ad9510_r_div)
{
    return param_client_read (self, service, FMC_ACTIVE_CLK_OPCODE_AD9510_R_DIV,
            ad9510_r_div);
}

/* AD9510 PLL Power Down */
PARAM_FUNC_CLIENT_WRITE(ad9510_pll_pdown)
{
    return param_client_write (self, service, FMC_ACTIVE_CLK_OPCODE_AD9510_PLL_PDOWN,
            ad9510_pll_pdown);
}

PARAM_FUNC_CLIENT_READ(ad9510_pll_pdown)
{
    return param_client_read (self, service, FMC_ACTIVE_CLK_OPCODE_AD9510_PLL_PDOWN,
            ad9510_pll_pdown);
}

/* AD9510 Mux Status */
PARAM_FUNC_CLIENT_WRITE(ad9510_mux_status)
{
    return param_client_write (self, service, FMC_ACTIVE_CLK_OPCODE_AD9510_MUX_STATUS,
            ad9510_mux_status);
}

PARAM_FUNC_CLIENT_READ(ad9510_mux_status)
{
    return param_client_read (self, service, FMC_ACTIVE_CLK_OPCODE_AD9510_MUX_STATUS,
            ad9510_mux_status);
}

/* AD9510 CP current */
PARAM_FUNC_CLIENT_WRITE(ad9510_cp_current)
{
    return param_client_write (self, service, FMC_ACTIVE_CLK_OPCODE_AD9510_CP_CURRENT,
            ad9510_cp_current);
}

PARAM_FUNC_CLIENT_READ(ad9510_cp_current)
{
    return param_client_read (self, service, FMC_ACTIVE_CLK_OPCODE_AD9510_CP_CURRENT,
            ad9510_cp_current);
}

/* AD9510 Outputs */
PARAM_FUNC_CLIENT_WRITE(ad9510_outputs)
{
    return param_client_write (self, service, FMC_ACTIVE_CLK_OPCODE_AD9510_OUTPUTS,
            ad9510_outputs);
}

PARAM_FUNC_CLIENT_READ(ad9510_outputs)
{
    return param_client_read (self, service, FMC_ACTIVE_CLK_OPCODE_AD9510_OUTPUTS,
            ad9510_outputs);
}

/* AD9510 PLL CLK Selection */
PARAM_FUNC_CLIENT_WRITE(ad9510_pll_clk_sel)
{
    return param_client_write (self, service, FMC_ACTIVE_CLK_OPCODE_AD9510_PLL_CLK_SEL,
            ad9510_pll_clk_sel);
}

PARAM_FUNC_CLIENT_READ(ad9510_pll_clk_sel)
{
    return param_client_read (self, service, FMC_ACTIVE_CLK_OPCODE_AD9510_PLL_CLK_SEL,
            ad9510_pll_clk_sel);
}

/* SI571 Set frequency */
PARAM_FUNC_CLIENT_WRITE_DOUBLE(si571_freq)
{
    return param_client_write_double (self, service, FMC_ACTIVE_CLK_OPCODE_SI571_FREQ,
            si571_freq);
}

PARAM_FUNC_CLIENT_READ_DOUBLE(si571_freq)
{
    return param_client_read_double (self, service, FMC_ACTIVE_CLK_OPCODE_SI571_FREQ,
            si571_freq);
}

/* SI571 Get defaults */
PARAM_FUNC_CLIENT_WRITE_DOUBLE(si571_defaults)
{
    return param_client_write_double (self, service, FMC_ACTIVE_CLK_OPCODE_SI571_GET_DEFAULTS,
            si571_defaults);
}

/**************** FMC 130M SMIO Functions ****************/

/* ADC LTC2208 RAND */
PARAM_FUNC_CLIENT_WRITE(adc_rand)
{
    return param_client_write (self, service, FMC130M_4CH_OPCODE_ADC_RAND, adc_rand);
}

PARAM_FUNC_CLIENT_READ(adc_rand)
{
     return param_client_read (self, service, FMC130M_4CH_OPCODE_ADC_RAND, adc_rand);
}

/* ADC LTC2208 DITH */
PARAM_FUNC_CLIENT_WRITE(adc_dith)
{
    return param_client_write (self, service, FMC130M_4CH_OPCODE_ADC_DITH, adc_dith);
}

PARAM_FUNC_CLIENT_READ(adc_dith)
{
     return param_client_read (self, service, FMC130M_4CH_OPCODE_ADC_DITH, adc_dith);
}

/* ADC LTC2208 SHDN */
PARAM_FUNC_CLIENT_WRITE(adc_shdn)
{
    return param_client_write (self, service, FMC130M_4CH_OPCODE_ADC_SHDN, adc_shdn);
}

PARAM_FUNC_CLIENT_READ(adc_shdn)
{
     return param_client_read (self, service, FMC130M_4CH_OPCODE_ADC_SHDN, adc_shdn);
}

/* ADC LTC2208 PGA */
PARAM_FUNC_CLIENT_WRITE(adc_pga)
{
    return param_client_write (self, service, FMC130M_4CH_OPCODE_ADC_PGA, adc_pga);
}

PARAM_FUNC_CLIENT_READ(adc_pga)
{
     return param_client_read (self, service, FMC130M_4CH_OPCODE_ADC_PGA, adc_pga);
}

/* RAW ADC data 0 value */
PARAM_FUNC_CLIENT_WRITE(adc_data0)
{
    return param_client_write (self, service, FMC130M_4CH_OPCODE_ADC_DATA0, adc_data0);
}

PARAM_FUNC_CLIENT_READ(adc_data0)
{
     return param_client_read (self, service, FMC130M_4CH_OPCODE_ADC_DATA0, adc_data0);
}

/* RAW ADC data 1 value */
PARAM_FUNC_CLIENT_WRITE(adc_data1)
{
    return param_client_write (self, service, FMC130M_4CH_OPCODE_ADC_DATA1, adc_data1);
}

PARAM_FUNC_CLIENT_READ(adc_data1)
{
     return param_client_read (self, service, FMC130M_4CH_OPCODE_ADC_DATA1, adc_data1);
}

/* RAW ADC data 2 value */
PARAM_FUNC_CLIENT_WRITE(adc_data2)
{
    return param_client_write (self, service, FMC130M_4CH_OPCODE_ADC_DATA2, adc_data2);
}

PARAM_FUNC_CLIENT_READ(adc_data2)
{
     return param_client_read (self, service, FMC130M_4CH_OPCODE_ADC_DATA2, adc_data2);
}

/* RAW ADC data 3 value */
PARAM_FUNC_CLIENT_WRITE(adc_data3)
{
    return param_client_write (self, service, FMC130M_4CH_OPCODE_ADC_DATA3, adc_data3);
}

PARAM_FUNC_CLIENT_READ(adc_data3)
{
     return param_client_read (self, service, FMC130M_4CH_OPCODE_ADC_DATA3, adc_data3);
}

/****************** FMC130M Delay Value Functions ****************/

/* ADC delay value 0 */
PARAM_FUNC_CLIENT_WRITE(adc_dly_val0)
{
    return param_client_write (self, service, FMC130M_4CH_OPCODE_ADC_DLY_VAL0, adc_dly_val0);
}

PARAM_FUNC_CLIENT_READ(adc_dly_val0)
{
     return param_client_read (self, service, FMC130M_4CH_OPCODE_ADC_DLY_VAL0, adc_dly_val0);
}

/* ADC delay value 1 */
PARAM_FUNC_CLIENT_WRITE(adc_dly_val1)
{
    return param_client_write (self, service, FMC130M_4CH_OPCODE_ADC_DLY_VAL1, adc_dly_val1);
}

PARAM_FUNC_CLIENT_READ(adc_dly_val1)
{
     return param_client_read (self, service, FMC130M_4CH_OPCODE_ADC_DLY_VAL1, adc_dly_val1);
}

/* ADC delay value 2 */
PARAM_FUNC_CLIENT_WRITE(adc_dly_val2)
{
    return param_client_write (self, service, FMC130M_4CH_OPCODE_ADC_DLY_VAL2, adc_dly_val2);
}

PARAM_FUNC_CLIENT_READ(adc_dly_val2)
{
     return param_client_read (self, service, FMC130M_4CH_OPCODE_ADC_DLY_VAL2, adc_dly_val2);
}

/* ADC delay value 3 */
PARAM_FUNC_CLIENT_WRITE(adc_dly_val3)
{
    return param_client_write (self, service, FMC130M_4CH_OPCODE_ADC_DLY_VAL3, adc_dly_val3);
}

PARAM_FUNC_CLIENT_READ(adc_dly_val3)
{
     return param_client_read (self, service, FMC130M_4CH_OPCODE_ADC_DLY_VAL3, adc_dly_val3);
}

/****************** FMC130M Delay Line selection Functions ****************/

/* ADC line value 0 */
PARAM_FUNC_CLIENT_WRITE(adc_dly_line0)
{
    return param_client_write (self, service, FMC130M_4CH_OPCODE_ADC_DLY_LINE0, adc_dly_line0);
}

PARAM_FUNC_CLIENT_READ(adc_dly_line0)
{
     return param_client_read (self, service, FMC130M_4CH_OPCODE_ADC_DLY_LINE0, adc_dly_line0);
}

/* ADC line value 1 */
PARAM_FUNC_CLIENT_WRITE(adc_dly_line1)
{
    return param_client_write (self, service, FMC130M_4CH_OPCODE_ADC_DLY_LINE1, adc_dly_line1);
}

PARAM_FUNC_CLIENT_READ(adc_dly_line1)
{
     return param_client_read (self, service, FMC130M_4CH_OPCODE_ADC_DLY_LINE1, adc_dly_line1);
}

/* ADC line value 2 */
PARAM_FUNC_CLIENT_WRITE(adc_dly_line2)
{
    return param_client_write (self, service, FMC130M_4CH_OPCODE_ADC_DLY_LINE2, adc_dly_line2);
}

PARAM_FUNC_CLIENT_READ(adc_dly_line2)
{
     return param_client_read (self, service, FMC130M_4CH_OPCODE_ADC_DLY_LINE2, adc_dly_line2);
}

/* ADC line value 3 */
PARAM_FUNC_CLIENT_WRITE(adc_dly_line3)
{
    return param_client_write (self, service, FMC130M_4CH_OPCODE_ADC_DLY_LINE3, adc_dly_line3);
}

PARAM_FUNC_CLIENT_READ(adc_dly_line3)
{
     return param_client_read (self, service, FMC130M_4CH_OPCODE_ADC_DLY_LINE3, adc_dly_line3);
}

/****************** FMC130M Delay update Functions ****************/

/* ADC Update channel 0 */
PARAM_FUNC_CLIENT_WRITE(adc_dly_updt0)
{
    return param_client_write (self, service, FMC130M_4CH_OPCODE_ADC_DLY_UPDT0, adc_dly_updt0);
}

PARAM_FUNC_CLIENT_READ(adc_dly_updt0)
{
     return param_client_read (self, service, FMC130M_4CH_OPCODE_ADC_DLY_UPDT0, adc_dly_updt0);
}

/* ADC Update channel 1 */
PARAM_FUNC_CLIENT_WRITE(adc_dly_updt1)
{
    return param_client_write (self, service, FMC130M_4CH_OPCODE_ADC_DLY_UPDT1, adc_dly_updt1);
}

PARAM_FUNC_CLIENT_READ(adc_dly_updt1)
{
     return param_client_read (self, service, FMC130M_4CH_OPCODE_ADC_DLY_UPDT1, adc_dly_updt1);
}

/* ADC Update channel 2 */
PARAM_FUNC_CLIENT_WRITE(adc_dly_updt2)
{
    return param_client_write (self, service, FMC130M_4CH_OPCODE_ADC_DLY_UPDT2, adc_dly_updt2);
}

PARAM_FUNC_CLIENT_READ(adc_dly_updt2)
{
     return param_client_read (self, service, FMC130M_4CH_OPCODE_ADC_DLY_UPDT2, adc_dly_updt2);
}

/* ADC Update channel 3 */
PARAM_FUNC_CLIENT_WRITE(adc_dly_updt3)
{
    return param_client_write (self, service, FMC130M_4CH_OPCODE_ADC_DLY_UPDT3, adc_dly_updt3);
}

PARAM_FUNC_CLIENT_READ(adc_dly_updt3)
{
     return param_client_read (self, service, FMC130M_4CH_OPCODE_ADC_DLY_UPDT3, adc_dly_updt3);
}

/****************** FMC130M set delay functions ****************/

/* ADC set delay channel 0 */
PARAM_FUNC_CLIENT_WRITE2(adc_dly0, type, val)
{
    return param_client_write_raw (self, service, FMC130M_4CH_OPCODE_ADC_DLY0,
            type, val);
}

/* ADC set delay channel 1 */
PARAM_FUNC_CLIENT_WRITE2(adc_dly1, type, val)
{
    return param_client_write_raw (self, service, FMC130M_4CH_OPCODE_ADC_DLY1,
            type, val);
}

/* ADC set delay channel 2 */
PARAM_FUNC_CLIENT_WRITE2(adc_dly2, type, val)
{
    return param_client_write_raw (self, service, FMC130M_4CH_OPCODE_ADC_DLY2,
            type, val);
}
/* ADC set delay channel 3 */
PARAM_FUNC_CLIENT_WRITE2(adc_dly3, type, val)
{
    return param_client_write_raw (self, service, FMC130M_4CH_OPCODE_ADC_DLY3,
            type, val);
}

/****************** FMC130M EEPROM functions ****************/

/* EEPROM read/write */
PARAM_FUNC_CLIENT_WRITE2(24aa64_data, addr, data)
{
    return param_client_write2 (self, service, FMC130M_4CH_OPCODE_24AA64_DATA, addr, data);
}

PARAM_FUNC_CLIENT_WRITE_READ(24aa64_data, addr, data)
{
    return param_client_write_read (self, service, FMC130M_4CH_OPCODE_24AA64_DATA, addr, data);
}

/****************** FMC_ACTIVE_CLK functions ****************/

/* AD9510 read/write */
PARAM_FUNC_CLIENT_WRITE2(ad9510_data, addr, data)
{
    return param_client_write2 (self, service, FMC_ACTIVE_CLK_OPCODE_AD9510_DATA, addr, data);
}

PARAM_FUNC_CLIENT_WRITE_READ(ad9510_data, addr, data)
{
    return param_client_write_read (self, service, FMC_ACTIVE_CLK_OPCODE_AD9510_DATA, addr, data);
}

/*************************** FMC250M Chips Functions *************************/

/* ISLA216P RST ADCs */
PARAM_FUNC_CLIENT_WRITE(rst_adcs)
{
    return param_client_write (self, service, FMC250M_4CH_OPCODE_RST_ADCS,
            rst_adcs);
}

PARAM_FUNC_CLIENT_READ(rst_adcs)
{
    return param_client_read (self, service, FMC250M_4CH_OPCODE_RST_ADCS,
            rst_adcs);
}

/* ISLA216P RST ADCs */
PARAM_FUNC_CLIENT_WRITE(rst_div_adcs)
{
    return param_client_write (self, service, FMC250M_4CH_OPCODE_RST_DIV_ADCS,
            rst_div_adcs);
}

PARAM_FUNC_CLIENT_READ(rst_div_adcs)
{
    return param_client_read (self, service, FMC250M_4CH_OPCODE_RST_DIV_ADCS,
            rst_div_adcs);
}

/* ISLA216P Sleep ADCs */
PARAM_FUNC_CLIENT_WRITE(sleep_adcs)
{
    return param_client_write (self, service, FMC250M_4CH_OPCODE_SLEEP_ADCS,
            sleep_adcs);
}

PARAM_FUNC_CLIENT_READ(sleep_adcs)
{
    return param_client_read (self, service, FMC250M_4CH_OPCODE_SLEEP_ADCS,
            sleep_adcs);
}

/* ISLA216P Test modes */
PARAM_FUNC_CLIENT_WRITE2(test_mode_adc, chan, mode)
{
    return param_client_write2 (self, service, FMC250M_4CH_OPCODE_TESTMODE,
            chan, mode);
}

/* ISLA216P reset modes */
PARAM_FUNC_CLIENT_WRITE2(rst_modes_adc, chan, mode)
{
    return param_client_write2 (self, service, FMC250M_4CH_OPCODE_RST,
            chan, mode);
}

/* ISLA216P portconfig */
PARAM_FUNC_CLIENT_WRITE2(portconfig_adc, chan, portconfig)
{
    return param_client_write2 (self, service, FMC250M_4CH_OPCODE_PORTCONFIG,
            chan, portconfig);
}

/* ISLA216P write register */
PARAM_FUNC_CLIENT_WRITE3(reg_adc, chan, addr, val)
{
    return param_client_write3 (self, service, FMC250M_4CH_OPCODE_REG,
            chan, addr, val);
}

PARAM_FUNC_CLIENT_WRITE2_READ(reg_adc, chan, addr, val)
{
    return param_client_write2_read (self, service, FMC250M_4CH_OPCODE_REG,
            chan, addr, val);
}

/* ISLA216P temperature */
PARAM_FUNC_CLIENT_WRITE_READ(temp_adc, chan, temp)
{
    return param_client_write_read (self, service, FMC250M_4CH_OPCODE_TEMP,
            chan, temp);
}

/* ISLA216P calibration status */
PARAM_FUNC_CLIENT_WRITE_READ(cal_status_adc, chan, cal_status)
{
    return param_client_write_read (self, service, FMC250M_4CH_OPCODE_CAL_STATUS,
            chan, cal_status);
}

/****************** FMC250M Delay Value Functions ****************/

/* ADC delay value 0 */
PARAM_FUNC_CLIENT_WRITE(adc250_data_dly0)
{
    return param_client_write (self, service, FMC250M_4CH_OPCODE_ADC_DATA_DLY0, adc250_data_dly0);
}

PARAM_FUNC_CLIENT_READ(adc250_data_dly0)
{
     return param_client_read (self, service, FMC250M_4CH_OPCODE_ADC_DATA_DLY0, adc250_data_dly0);
}

/* ADC delay value 1 */
PARAM_FUNC_CLIENT_WRITE(adc250_data_dly1)
{
    return param_client_write (self, service, FMC250M_4CH_OPCODE_ADC_DATA_DLY1, adc250_data_dly1);
}

PARAM_FUNC_CLIENT_READ(adc250_data_dly1)
{
     return param_client_read (self, service, FMC250M_4CH_OPCODE_ADC_DATA_DLY1, adc250_data_dly1);
}

/* ADC delay value 2 */
PARAM_FUNC_CLIENT_WRITE(adc250_data_dly2)
{
    return param_client_write (self, service, FMC250M_4CH_OPCODE_ADC_DATA_DLY2, adc250_data_dly2);
}

PARAM_FUNC_CLIENT_READ(adc250_data_dly2)
{
     return param_client_read (self, service, FMC250M_4CH_OPCODE_ADC_DATA_DLY2, adc250_data_dly2);
}

/* ADC delay value 3 */
PARAM_FUNC_CLIENT_WRITE(adc250_data_dly3)
{
    return param_client_write (self, service, FMC250M_4CH_OPCODE_ADC_DATA_DLY3, adc250_data_dly3);
}

PARAM_FUNC_CLIENT_READ(adc250_data_dly3)
{
     return param_client_read (self, service, FMC250M_4CH_OPCODE_ADC_DATA_DLY3, adc250_data_dly3);
}

/* ADC delay value 0 */
PARAM_FUNC_CLIENT_WRITE(adc250_clk_dly0)
{
    return param_client_write (self, service, FMC250M_4CH_OPCODE_ADC_CLK_DLY0, adc250_clk_dly0);
}

PARAM_FUNC_CLIENT_READ(adc250_clk_dly0)
{
     return param_client_read (self, service, FMC250M_4CH_OPCODE_ADC_CLK_DLY0, adc250_clk_dly0);
}

/* ADC delay value 1 */
PARAM_FUNC_CLIENT_WRITE(adc250_clk_dly1)
{
    return param_client_write (self, service, FMC250M_4CH_OPCODE_ADC_CLK_DLY1, adc250_clk_dly1);
}

PARAM_FUNC_CLIENT_READ(adc250_clk_dly1)
{
     return param_client_read (self, service, FMC250M_4CH_OPCODE_ADC_CLK_DLY1, adc250_clk_dly1);
}

/* ADC delay value 2 */
PARAM_FUNC_CLIENT_WRITE(adc250_clk_dly2)
{
    return param_client_write (self, service, FMC250M_4CH_OPCODE_ADC_CLK_DLY2, adc250_clk_dly2);
}

PARAM_FUNC_CLIENT_READ(adc250_clk_dly2)
{
     return param_client_read (self, service, FMC250M_4CH_OPCODE_ADC_CLK_DLY2, adc250_clk_dly2);
}

/* ADC delay value 3 */
PARAM_FUNC_CLIENT_WRITE(adc250_clk_dly3)
{
    return param_client_write (self, service, FMC250M_4CH_OPCODE_ADC_CLK_DLY3, adc250_clk_dly3);
}

PARAM_FUNC_CLIENT_READ(adc250_clk_dly3)
{
     return param_client_read (self, service, FMC250M_4CH_OPCODE_ADC_CLK_DLY3, adc250_clk_dly3);
}

/****************** FMC250M Delay Line selection Functions ****************/

/* ADC line value 0 */
PARAM_FUNC_CLIENT_WRITE(adc250_dly_line0)
{
    return param_client_write (self, service, FMC250M_4CH_OPCODE_ADC_DLY_LINE0, adc250_dly_line0);
}

PARAM_FUNC_CLIENT_READ(adc250_dly_line0)
{
     return param_client_read (self, service, FMC250M_4CH_OPCODE_ADC_DLY_LINE0, adc250_dly_line0);
}

/* ADC line value 1 */
PARAM_FUNC_CLIENT_WRITE(adc250_dly_line1)
{
    return param_client_write (self, service, FMC250M_4CH_OPCODE_ADC_DLY_LINE1, adc250_dly_line1);
}

PARAM_FUNC_CLIENT_READ(adc250_dly_line1)
{
     return param_client_read (self, service, FMC250M_4CH_OPCODE_ADC_DLY_LINE1, adc250_dly_line1);
}

/* ADC line value 2 */
PARAM_FUNC_CLIENT_WRITE(adc250_dly_line2)
{
    return param_client_write (self, service, FMC250M_4CH_OPCODE_ADC_DLY_LINE2, adc250_dly_line2);
}

PARAM_FUNC_CLIENT_READ(adc250_dly_line2)
{
     return param_client_read (self, service, FMC250M_4CH_OPCODE_ADC_DLY_LINE2, adc250_dly_line2);
}

/* ADC line value 3 */
PARAM_FUNC_CLIENT_WRITE(adc250_dly_line3)
{
    return param_client_write (self, service, FMC250M_4CH_OPCODE_ADC_DLY_LINE3, adc250_dly_line3);
}

PARAM_FUNC_CLIENT_READ(adc250_dly_line3)
{
     return param_client_read (self, service, FMC250M_4CH_OPCODE_ADC_DLY_LINE3, adc250_dly_line3);
}

/****************** FMC250M Delay update Functions ****************/

/* ADC Update channel 0 */
PARAM_FUNC_CLIENT_WRITE(adc250_dly_updt0)
{
    return param_client_write (self, service, FMC250M_4CH_OPCODE_ADC_DLY_UPDT0, adc250_dly_updt0);
}

PARAM_FUNC_CLIENT_READ(adc250_dly_updt0)
{
     return param_client_read (self, service, FMC250M_4CH_OPCODE_ADC_DLY_UPDT0, adc250_dly_updt0);
}

/* ADC Update channel 1 */
PARAM_FUNC_CLIENT_WRITE(adc250_dly_updt1)
{
    return param_client_write (self, service, FMC250M_4CH_OPCODE_ADC_DLY_UPDT1, adc250_dly_updt1);
}

PARAM_FUNC_CLIENT_READ(adc250_dly_updt1)
{
     return param_client_read (self, service, FMC250M_4CH_OPCODE_ADC_DLY_UPDT1, adc250_dly_updt1);
}

/* ADC Update channel 2 */
PARAM_FUNC_CLIENT_WRITE(adc250_dly_updt2)
{
    return param_client_write (self, service, FMC250M_4CH_OPCODE_ADC_DLY_UPDT2, adc250_dly_updt2);
}

PARAM_FUNC_CLIENT_READ(adc250_dly_updt2)
{
     return param_client_read (self, service, FMC250M_4CH_OPCODE_ADC_DLY_UPDT2, adc250_dly_updt2);
}

/* ADC Update channel 3 */
PARAM_FUNC_CLIENT_WRITE(adc250_dly_updt3)
{
    return param_client_write (self, service, FMC250M_4CH_OPCODE_ADC_DLY_UPDT3, adc250_dly_updt3);
}

PARAM_FUNC_CLIENT_READ(adc250_dly_updt3)
{
     return param_client_read (self, service, FMC250M_4CH_OPCODE_ADC_DLY_UPDT3, adc250_dly_updt3);
}

/****************** FMC250M set delay functions ****************/

/* ADC set delay channel 0 */
PARAM_FUNC_CLIENT_WRITE2(adc250_dly0, type, val)
{
    return param_client_write_raw (self, service, FMC250M_4CH_OPCODE_ADC_DLY0,
            type, val);
}

/* ADC set delay channel 1 */
PARAM_FUNC_CLIENT_WRITE2(adc250_dly1, type, val)
{
    return param_client_write_raw (self, service, FMC250M_4CH_OPCODE_ADC_DLY1,
            type, val);
}

/* ADC set delay channel 2 */
PARAM_FUNC_CLIENT_WRITE2(adc250_dly2, type, val)
{
    return param_client_write_raw (self, service, FMC250M_4CH_OPCODE_ADC_DLY2,
            type, val);
}
/* ADC set delay channel 3 */
PARAM_FUNC_CLIENT_WRITE2(adc250_dly3, type, val)
{
    return param_client_write_raw (self, service, FMC250M_4CH_OPCODE_ADC_DLY3,
            type, val);
}

/**************** FMCPICO1M_4CH SMIO Functions ****************/

/* Led1 functions */
PARAM_FUNC_CLIENT_WRITE(fmcpico_led1)
{
    return param_client_write (self, service, FMCPICO1M_4CH_OPCODE_SET_GET_LED1, fmcpico_led1);
}

PARAM_FUNC_CLIENT_READ(fmcpico_led1)
{
    return param_client_read (self, service, FMCPICO1M_4CH_OPCODE_SET_GET_LED1, fmcpico_led1);
}

/* Led2 functions */
PARAM_FUNC_CLIENT_WRITE(fmcpico_led2)
{
    return param_client_write (self, service, FMCPICO1M_4CH_OPCODE_SET_GET_LED2, fmcpico_led2);
}

PARAM_FUNC_CLIENT_READ(fmcpico_led2)
{
    return param_client_read (self, service, FMCPICO1M_4CH_OPCODE_SET_GET_LED2, fmcpico_led2);
}

/* Rng_R0 functions */
PARAM_FUNC_CLIENT_WRITE(fmcpico_rng_r0)
{
    return param_client_write (self, service, FMCPICO1M_4CH_OPCODE_SET_GET_RNG_R0, fmcpico_rng_r0);
}

PARAM_FUNC_CLIENT_READ(fmcpico_rng_r0)
{
    return param_client_read (self, service, FMCPICO1M_4CH_OPCODE_SET_GET_RNG_R0, fmcpico_rng_r0);
}

/* Rng_R1 functions */
PARAM_FUNC_CLIENT_WRITE(fmcpico_rng_r1)
{
    return param_client_write (self, service, FMCPICO1M_4CH_OPCODE_SET_GET_RNG_R1, fmcpico_rng_r1);
}

PARAM_FUNC_CLIENT_READ(fmcpico_rng_r1)
{
    return param_client_read (self, service, FMCPICO1M_4CH_OPCODE_SET_GET_RNG_R1, fmcpico_rng_r1);
}

/* Rng_R2 functions */
PARAM_FUNC_CLIENT_WRITE(fmcpico_rng_r2)
{
    return param_client_write (self, service, FMCPICO1M_4CH_OPCODE_SET_GET_RNG_R2, fmcpico_rng_r2);
}

PARAM_FUNC_CLIENT_READ(fmcpico_rng_r2)
{
    return param_client_read (self, service, FMCPICO1M_4CH_OPCODE_SET_GET_RNG_R2, fmcpico_rng_r2);
}

/* Rng_R3 functions */
PARAM_FUNC_CLIENT_WRITE(fmcpico_rng_r3)
{
    return param_client_write (self, service, FMCPICO1M_4CH_OPCODE_SET_GET_RNG_R3, fmcpico_rng_r3);
}

PARAM_FUNC_CLIENT_READ(fmcpico_rng_r3)
{
    return param_client_read (self, service, FMCPICO1M_4CH_OPCODE_SET_GET_RNG_R3, fmcpico_rng_r3);
}

/* AdcData0 functions */
PARAM_FUNC_CLIENT_WRITE(fmcpico_adc_data0)
{
    return param_client_write (self, service, FMCPICO1M_4CH_OPCODE_SET_GET_ADC_DATA0, fmcpico_adc_data0);
}

PARAM_FUNC_CLIENT_READ(fmcpico_adc_data0)
{
    return param_client_read (self, service, FMCPICO1M_4CH_OPCODE_SET_GET_ADC_DATA0, fmcpico_adc_data0);
}

/* AdcData1 functions */
PARAM_FUNC_CLIENT_WRITE(fmcpico_adc_data1)
{
    return param_client_write (self, service, FMCPICO1M_4CH_OPCODE_SET_GET_ADC_DATA1, fmcpico_adc_data1);
}

PARAM_FUNC_CLIENT_READ(fmcpico_adc_data1)
{
    return param_client_read (self, service, FMCPICO1M_4CH_OPCODE_SET_GET_ADC_DATA1, fmcpico_adc_data1);
}

/* AdcData2 functions */
PARAM_FUNC_CLIENT_WRITE(fmcpico_adc_data2)
{
    return param_client_write (self, service, FMCPICO1M_4CH_OPCODE_SET_GET_ADC_DATA2, fmcpico_adc_data2);
}

PARAM_FUNC_CLIENT_READ(fmcpico_adc_data2)
{
    return param_client_read (self, service, FMCPICO1M_4CH_OPCODE_SET_GET_ADC_DATA2, fmcpico_adc_data2);
}

/* AdcData3 functions */
PARAM_FUNC_CLIENT_WRITE(fmcpico_adc_data3)
{
    return param_client_write (self, service, FMCPICO1M_4CH_OPCODE_SET_GET_ADC_DATA3, fmcpico_adc_data3);
}

PARAM_FUNC_CLIENT_READ(fmcpico_adc_data3)
{
    return param_client_read (self, service, FMCPICO1M_4CH_OPCODE_SET_GET_ADC_DATA3, fmcpico_adc_data3);
}

/**************** DSP SMIO Functions ****************/

/* Kx functions */
PARAM_FUNC_CLIENT_WRITE(kx)
{
    return param_client_write (self, service, DSP_OPCODE_SET_GET_KX, kx);
}

PARAM_FUNC_CLIENT_READ(kx)
{
    return param_client_read (self, service, DSP_OPCODE_SET_GET_KX, kx);
}

/* Ky functions */
PARAM_FUNC_CLIENT_WRITE(ky)
{
    return param_client_write (self, service, DSP_OPCODE_SET_GET_KY, ky);
}

PARAM_FUNC_CLIENT_READ(ky)
{
    return param_client_read (self, service, DSP_OPCODE_SET_GET_KY, ky);
}

/* Ksum functions */
PARAM_FUNC_CLIENT_WRITE(ksum)
{
    return param_client_write (self, service, DSP_OPCODE_SET_GET_KSUM, ksum);
}

PARAM_FUNC_CLIENT_READ(ksum)
{
    return param_client_read (self, service, DSP_OPCODE_SET_GET_KSUM, ksum);
}

/* Delta/Sigma TBT threshold calculation functions */
PARAM_FUNC_CLIENT_WRITE(ds_tbt_thres)
{
    return param_client_write (self, service, DSP_OPCODE_SET_GET_DS_TBT_THRES, ds_tbt_thres);
}

PARAM_FUNC_CLIENT_READ(ds_tbt_thres)
{
    return param_client_read (self, service, DSP_OPCODE_SET_GET_DS_TBT_THRES, ds_tbt_thres);
}

/* Delta/Sigma FOFB threshold calculation functions */
PARAM_FUNC_CLIENT_WRITE(ds_fofb_thres)
{
    return param_client_write (self, service, DSP_OPCODE_SET_GET_DS_FOFB_THRES, ds_fofb_thres);
}

PARAM_FUNC_CLIENT_READ(ds_fofb_thres)
{
     return param_client_read (self, service, DSP_OPCODE_SET_GET_DS_FOFB_THRES, ds_fofb_thres);
}

/* Delta/Sigma MONIT. threshold calculation functions */
PARAM_FUNC_CLIENT_WRITE(ds_monit_thres)
{
    return param_client_write (self, service, DSP_OPCODE_SET_GET_DS_MONIT_THRES, ds_monit_thres);
}

PARAM_FUNC_CLIENT_READ(ds_monit_thres)
{
     return param_client_read (self, service, DSP_OPCODE_SET_GET_DS_MONIT_THRES, ds_monit_thres);
}

/* Test Data functions */
PARAM_FUNC_CLIENT_WRITE(dsp_cfg_test_data)
{
    return param_client_write (self, service, DSP_OPCODE_SET_GET_CFG_TEST_DATA, dsp_cfg_test_data);
}

PARAM_FUNC_CLIENT_READ(dsp_cfg_test_data)
{
     return param_client_read (self, service, DSP_OPCODE_SET_GET_CFG_TEST_DATA, dsp_cfg_test_data);
}

/* Monitoring Amplitude channel 0 value */
PARAM_FUNC_CLIENT_WRITE(monit_amp_ch0)
{
    return param_client_write (self, service, DSP_OPCODE_SET_GET_MONIT_AMP_CH0, monit_amp_ch0);
}

PARAM_FUNC_CLIENT_READ(monit_amp_ch0)
{
     return param_client_read (self, service, DSP_OPCODE_SET_GET_MONIT_AMP_CH0, monit_amp_ch0);
}

/* Monitoring Amplitude channel 1 value */
PARAM_FUNC_CLIENT_WRITE(monit_amp_ch1)
{
    return param_client_write (self, service, DSP_OPCODE_SET_GET_MONIT_AMP_CH1, monit_amp_ch1);
}

PARAM_FUNC_CLIENT_READ(monit_amp_ch1)
{
     return param_client_read (self, service, DSP_OPCODE_SET_GET_MONIT_AMP_CH1, monit_amp_ch1);
}

/* Monitoring Amplitude channel 2 value */
PARAM_FUNC_CLIENT_WRITE(monit_amp_ch2)
{
    return param_client_write (self, service, DSP_OPCODE_SET_GET_MONIT_AMP_CH2, monit_amp_ch2);
}

PARAM_FUNC_CLIENT_READ(monit_amp_ch2)
{
     return param_client_read (self, service, DSP_OPCODE_SET_GET_MONIT_AMP_CH2, monit_amp_ch2);
}

/* Monitoring Amplitude channel 3 value */
PARAM_FUNC_CLIENT_WRITE(monit_amp_ch3)
{
    return param_client_write (self, service, DSP_OPCODE_SET_GET_MONIT_AMP_CH3, monit_amp_ch3);
}

PARAM_FUNC_CLIENT_READ(monit_amp_ch3)
{
     return param_client_read (self, service, DSP_OPCODE_SET_GET_MONIT_AMP_CH3, monit_amp_ch3);
}

/* Monitoring Position X value */
PARAM_FUNC_CLIENT_WRITE(monit_pos_x)
{
    return param_client_write (self, service, DSP_OPCODE_SET_GET_MONIT_POS_X, monit_pos_x);
}

PARAM_FUNC_CLIENT_READ(monit_pos_x)
{
     return param_client_read (self, service, DSP_OPCODE_SET_GET_MONIT_POS_X, monit_pos_x);
}

/* Monitoring Position Y value */
PARAM_FUNC_CLIENT_WRITE(monit_pos_y)
{
    return param_client_write (self, service, DSP_OPCODE_SET_GET_MONIT_POS_Y, monit_pos_y);
}

PARAM_FUNC_CLIENT_READ(monit_pos_y)
{
     return param_client_read (self, service, DSP_OPCODE_SET_GET_MONIT_POS_Y, monit_pos_y);
}

/* Monitoring Position Q value */
PARAM_FUNC_CLIENT_WRITE(monit_pos_q)
{
    return param_client_write (self, service, DSP_OPCODE_SET_GET_MONIT_POS_Q, monit_pos_q);
}

PARAM_FUNC_CLIENT_READ(monit_pos_q)
{
     return param_client_read (self, service, DSP_OPCODE_SET_GET_MONIT_POS_Q, monit_pos_q);
}

/* Monitoring Position Sum value */
PARAM_FUNC_CLIENT_WRITE(monit_pos_sum)
{
    return param_client_write (self, service, DSP_OPCODE_SET_GET_MONIT_POS_SUM, monit_pos_sum);
}

PARAM_FUNC_CLIENT_READ(monit_pos_sum)
{
     return param_client_read (self, service, DSP_OPCODE_SET_GET_MONIT_POS_SUM, monit_pos_sum);
}

/* Monitoring Update value */
PARAM_FUNC_CLIENT_WRITE(monit_updt)
{
    return param_client_write (self, service, DSP_OPCODE_SET_GET_MONIT_UPDT, monit_updt);
}

PARAM_FUNC_CLIENT_READ(monit_updt)
{
    return param_client_read (self, service, DSP_OPCODE_SET_GET_MONIT_UPDT, monit_updt);
}

/**************** Swap SMIO Functions ****************/

/* Switching functions */
PARAM_FUNC_CLIENT_WRITE(sw)
{
    return param_client_write (self, service, SWAP_OPCODE_SET_GET_SW, sw);
}

PARAM_FUNC_CLIENT_READ(sw)
{
    return param_client_read (self, service, SWAP_OPCODE_SET_GET_SW, sw);
}

/* Switching Clock division functions */
PARAM_FUNC_CLIENT_WRITE(div_clk)
{
    return param_client_write (self, service, SWAP_OPCODE_SET_GET_DIV_CLK, div_clk);
}

PARAM_FUNC_CLIENT_READ(div_clk)
{
    return param_client_read (self, service, SWAP_OPCODE_SET_GET_DIV_CLK, div_clk);
}

/* Switching delay functions */
PARAM_FUNC_CLIENT_WRITE(sw_dly)
{
    return param_client_write (self, service, SWAP_OPCODE_SET_GET_SW_DLY, sw_dly);
}

PARAM_FUNC_CLIENT_READ(sw_dly)
{
    return param_client_read (self, service, SWAP_OPCODE_SET_GET_SW_DLY, sw_dly);
}

/**************** RFFE SMIO Functions ****************/

/* RFFE get/set attenuator */
PARAM_FUNC_CLIENT_WRITE_DOUBLE(rffe_att)
{
    return param_client_write_double (self, service, RFFE_OPCODE_SET_GET_ATT,
            rffe_att);
}

PARAM_FUNC_CLIENT_READ_DOUBLE(rffe_att)
{
    return param_client_read_double (self, service, RFFE_OPCODE_SET_GET_ATT,
            rffe_att);
}

/* RFFE get temperatures */
PARAM_FUNC_CLIENT_READ_DOUBLE(rffe_temp_ac)
{
    return param_client_read_double (self, service, RFFE_OPCODE_SET_GET_TEMP_AC,
            rffe_temp_ac);
}

PARAM_FUNC_CLIENT_READ_DOUBLE(rffe_temp_bd)
{
    return param_client_read_double (self, service, RFFE_OPCODE_SET_GET_TEMP_BD,
            rffe_temp_bd);
}

/* RFFE get/set set points */
PARAM_FUNC_CLIENT_WRITE_DOUBLE(rffe_set_point_ac)
{
    return param_client_write_double (self, service, RFFE_OPCODE_SET_GET_SET_POINT_AC,
            rffe_set_point_ac);
}

PARAM_FUNC_CLIENT_READ_DOUBLE(rffe_set_point_ac)
{
    return param_client_read_double (self, service, RFFE_OPCODE_SET_GET_SET_POINT_AC,
            rffe_set_point_ac);
}

PARAM_FUNC_CLIENT_WRITE_DOUBLE(rffe_set_point_bd)
{
    return param_client_write_double (self, service, RFFE_OPCODE_SET_GET_SET_POINT_BD,
            rffe_set_point_bd);
}

PARAM_FUNC_CLIENT_READ_DOUBLE(rffe_set_point_bd)
{
    return param_client_read_double (self, service, RFFE_OPCODE_SET_GET_SET_POINT_BD,
            rffe_set_point_bd);
}

/* RFFE get/set temperature control */
PARAM_FUNC_CLIENT_WRITE_BYTE(rffe_temp_control)
{
    return param_client_write_byte (self, service, RFFE_OPCODE_SET_GET_TEMP_CONTROL,
            rffe_temp_control);
}

PARAM_FUNC_CLIENT_READ_BYTE(rffe_temp_control)
{
    return param_client_read_byte (self, service, RFFE_OPCODE_SET_GET_TEMP_CONTROL,
            rffe_temp_control);
}

/* RFFE outputs */
PARAM_FUNC_CLIENT_WRITE_DOUBLE(rffe_heater_ac)
{
    return param_client_write_double (self, service, RFFE_OPCODE_SET_GET_HEATER_AC,
            rffe_heater_ac);
}

PARAM_FUNC_CLIENT_READ_DOUBLE(rffe_heater_ac)
{
    return param_client_read_double (self, service, RFFE_OPCODE_SET_GET_HEATER_AC,
            rffe_heater_ac);
}

PARAM_FUNC_CLIENT_WRITE_DOUBLE(rffe_heater_bd)
{
    return param_client_write_double (self, service, RFFE_OPCODE_SET_GET_HEATER_BD,
            rffe_heater_bd);
}

PARAM_FUNC_CLIENT_READ_DOUBLE(rffe_heater_bd)
{
    return param_client_read_double (self, service, RFFE_OPCODE_SET_GET_HEATER_BD,
            rffe_heater_bd);
}

/* RFFE get/set reset */
PARAM_FUNC_CLIENT_WRITE_BYTE(rffe_reset)
{
    return param_client_write_byte (self, service, RFFE_OPCODE_SET_GET_RESET,
            rffe_reset);
}

PARAM_FUNC_CLIENT_READ_BYTE(rffe_reset)
{
    return param_client_read_byte (self, service, RFFE_OPCODE_SET_GET_RESET,
            rffe_reset);
}

/* RFFE get/set reprogram */
PARAM_FUNC_CLIENT_WRITE_BYTE(rffe_reprog)
{
    return param_client_write_byte (self, service, RFFE_OPCODE_SET_GET_REPROG,
            rffe_reprog);
}

PARAM_FUNC_CLIENT_READ_BYTE(rffe_reprog)
{
    return param_client_read_byte (self, service, RFFE_OPCODE_SET_GET_REPROG,
            rffe_reprog);
}

/* RFFE set/get data */
halcs_client_err_e halcs_set_rffe_data (halcs_client_t *self, char *service,
        struct _smio_rffe_data_block_t *rffe_data_block)
{
    uint32_t rw = WRITE_MODE;
    return param_client_write_gen (self, service, RFFE_OPCODE_SET_GET_DATA,
            rw, rffe_data_block, sizeof (*rffe_data_block), NULL, 0, NULL, 0);
}

halcs_client_err_e halcs_get_rffe_data (halcs_client_t *self, char *service,
        struct _smio_rffe_data_block_t *rffe_data_block)
{
    uint32_t rw = READ_MODE;
    return param_client_read_gen (self, service, RFFE_OPCODE_SET_GET_DATA,
            rw, rffe_data_block, sizeof (*rffe_data_block), NULL, 0, NULL, 0,
            rffe_data_block, sizeof (*rffe_data_block));
}

/* RFFE get version */
halcs_client_err_e halcs_get_rffe_version (halcs_client_t *self, char *service,
        struct _smio_rffe_version_t *rffe_version)
{
    uint32_t rw = READ_MODE;
    return param_client_read_gen (self, service, RFFE_OPCODE_SET_GET_VERSION,
            rw, rffe_version, sizeof (*rffe_version), NULL, 0, NULL, 0,
            rffe_version, sizeof (*rffe_version));
}

/* RFFE PID parameters */
PARAM_FUNC_CLIENT_WRITE_DOUBLE(rffe_pid_ac_kp)
{
    return param_client_write_double (self, service, RFFE_OPCODE_SET_GET_PID_AC_KP,
            rffe_pid_ac_kp);
}

PARAM_FUNC_CLIENT_READ_DOUBLE(rffe_pid_ac_kp)
{
    return param_client_read_double (self, service, RFFE_OPCODE_SET_GET_PID_AC_KP,
            rffe_pid_ac_kp);
}

PARAM_FUNC_CLIENT_WRITE_DOUBLE(rffe_pid_ac_ti)
{
    return param_client_write_double (self, service, RFFE_OPCODE_SET_GET_PID_AC_TI,
            rffe_pid_ac_ti);
}

PARAM_FUNC_CLIENT_READ_DOUBLE(rffe_pid_ac_ti)
{
    return param_client_read_double (self, service, RFFE_OPCODE_SET_GET_PID_AC_TI,
            rffe_pid_ac_ti);
}

PARAM_FUNC_CLIENT_WRITE_DOUBLE(rffe_pid_ac_td)
{
    return param_client_write_double (self, service, RFFE_OPCODE_SET_GET_PID_AC_TD,
            rffe_pid_ac_td);
}

PARAM_FUNC_CLIENT_READ_DOUBLE(rffe_pid_ac_td)
{
    return param_client_read_double (self, service, RFFE_OPCODE_SET_GET_PID_AC_TD,
            rffe_pid_ac_td);
}

PARAM_FUNC_CLIENT_WRITE_DOUBLE(rffe_pid_bd_kp)
{
    return param_client_write_double (self, service, RFFE_OPCODE_SET_GET_PID_BD_KP,
            rffe_pid_bd_kp);
}

PARAM_FUNC_CLIENT_READ_DOUBLE(rffe_pid_bd_kp)
{
    return param_client_read_double (self, service, RFFE_OPCODE_SET_GET_PID_BD_KP,
            rffe_pid_bd_kp);
}

PARAM_FUNC_CLIENT_WRITE_DOUBLE(rffe_pid_bd_ti)
{
    return param_client_write_double (self, service, RFFE_OPCODE_SET_GET_PID_BD_TI,
            rffe_pid_bd_ti);
}

PARAM_FUNC_CLIENT_READ_DOUBLE(rffe_pid_bd_ti)
{
    return param_client_read_double (self, service, RFFE_OPCODE_SET_GET_PID_BD_TI,
            rffe_pid_bd_ti);
}

PARAM_FUNC_CLIENT_WRITE_DOUBLE(rffe_pid_bd_td)
{
    return param_client_write_double (self, service, RFFE_OPCODE_SET_GET_PID_BD_TD,
            rffe_pid_bd_td);
}

PARAM_FUNC_CLIENT_READ_DOUBLE(rffe_pid_bd_td)
{
    return param_client_read_double (self, service, RFFE_OPCODE_SET_GET_PID_BD_TD,
            rffe_pid_bd_td);
}

/********************** AFC Diagnostics Functions ********************/

/* AFC card slot */
PARAM_FUNC_CLIENT_WRITE(afc_diag_card_slot)
{
    return param_client_write (self, service, AFC_DIAG_OPCODE_SET_GET_CARD_SLOT,
            afc_diag_card_slot);
}

PARAM_FUNC_CLIENT_READ(afc_diag_card_slot)
{
    return param_client_read (self, service, AFC_DIAG_OPCODE_SET_GET_CARD_SLOT,
            afc_diag_card_slot);
}

/* AFC IPMI address */
PARAM_FUNC_CLIENT_WRITE(afc_diag_ipmi_addr)
{
    return param_client_write (self, service, AFC_DIAG_OPCODE_SET_GET_IPMI_ADDR,
            afc_diag_ipmi_addr);
}

PARAM_FUNC_CLIENT_READ(afc_diag_ipmi_addr)
{
    return param_client_read (self, service, AFC_DIAG_OPCODE_SET_GET_IPMI_ADDR,
            afc_diag_ipmi_addr);
}

/* Build Revision */
halcs_client_err_e halcs_get_afc_diag_build_revision (halcs_client_t *self, char *service,
        struct _smio_afc_diag_revision_data_t *revision_data)
{
    uint32_t rw = READ_MODE;
    return param_client_read_gen (self, service, AFC_DIAG_OPCODE_GET_BUILD_REVISION,
            rw, revision_data, sizeof (*revision_data), NULL, 0, NULL, 0,
            revision_data, sizeof (*revision_data));
}

/* Build Date */
halcs_client_err_e halcs_get_afc_diag_build_date (halcs_client_t *self, char *service,
        struct _smio_afc_diag_revision_data_t *revision_data)
{
    uint32_t rw = READ_MODE;
    return param_client_read_gen (self, service, AFC_DIAG_OPCODE_GET_BUILD_DATE,
            rw, revision_data, sizeof (*revision_data), NULL, 0, NULL, 0,
            revision_data, sizeof (*revision_data));
}

/* Build User Name */
halcs_client_err_e halcs_get_afc_diag_build_user_name (halcs_client_t *self, char *service,
        struct _smio_afc_diag_revision_data_t *revision_data)
{
    uint32_t rw = READ_MODE;
    return param_client_read_gen (self, service, AFC_DIAG_OPCODE_GET_BUILD_USER_NAME,
            rw, revision_data, sizeof (*revision_data), NULL, 0, NULL, 0,
            revision_data, sizeof (*revision_data));
}

/* Build User Email */
halcs_client_err_e halcs_get_afc_diag_build_user_email (halcs_client_t *self, char *service,
        struct _smio_afc_diag_revision_data_t *revision_data)
{
    uint32_t rw = READ_MODE;
    return param_client_read_gen (self, service, AFC_DIAG_OPCODE_GET_BUILD_USER_EMAIL,
            rw, revision_data, sizeof (*revision_data), NULL, 0, NULL, 0,
            revision_data, sizeof (*revision_data));
}

/********************** Trigger Interface Functions ********************/

/* Trigger direction */
PARAM_FUNC_CLIENT_WRITE2(trigger_dir, chan, dir)
{
    return param_client_write2 (self, service, TRIGGER_IFACE_OPCODE_DIR,
            chan, dir);
}

PARAM_FUNC_CLIENT_WRITE_READ(trigger_dir, chan, dir)
{
    return param_client_write_read (self, service, TRIGGER_IFACE_OPCODE_DIR,
            chan, dir);
}

/* Trigger direction */
PARAM_FUNC_CLIENT_WRITE2(trigger_dir_pol, chan, dir_pol)
{
    return param_client_write2 (self, service, TRIGGER_IFACE_OPCODE_DIR_POL,
            chan, dir_pol);
}

PARAM_FUNC_CLIENT_WRITE_READ(trigger_dir_pol, chan, dir_pol)
{
    return param_client_write_read (self, service, TRIGGER_IFACE_OPCODE_DIR_POL,
                chan, dir_pol);
}

/* Trigger receive counter reset */
PARAM_FUNC_CLIENT_WRITE2(trigger_rcv_count_rst, chan, rcv_count_rst)
{
    return param_client_write2 (self, service, TRIGGER_IFACE_OPCODE_RCV_COUNT_RST,
            chan, rcv_count_rst);
}

PARAM_FUNC_CLIENT_WRITE_READ(trigger_rcv_count_rst, chan, rcv_count_rst)
{
    return param_client_write_read (self, service, TRIGGER_IFACE_OPCODE_RCV_COUNT_RST,
            chan, rcv_count_rst);
}

/* Trigger transmit counter reset */
PARAM_FUNC_CLIENT_WRITE2(trigger_transm_count_rst, chan, transm_count_rst)
{
    return param_client_write2 (self, service, TRIGGER_IFACE_OPCODE_TRANSM_COUNT_RST,
            chan, transm_count_rst);
}

PARAM_FUNC_CLIENT_WRITE_READ(trigger_transm_count_rst, chan, transm_count_rst)
{
    return param_client_write_read (self, service, TRIGGER_IFACE_OPCODE_TRANSM_COUNT_RST,
            chan, transm_count_rst);
}

/* Trigger receive length debounce */
PARAM_FUNC_CLIENT_WRITE2(trigger_rcv_len, chan, rcv_len)
{
    return param_client_write2 (self, service, TRIGGER_IFACE_OPCODE_RCV_LEN,
            chan, rcv_len);
}

PARAM_FUNC_CLIENT_WRITE_READ(trigger_rcv_len, chan, rcv_len)
{
    return param_client_write_read (self, service, TRIGGER_IFACE_OPCODE_RCV_LEN,
            chan, rcv_len);
}

/* Trigger transmit length debounce */
PARAM_FUNC_CLIENT_WRITE2(trigger_transm_len, chan, transm_len)
{
    return param_client_write2 (self, service, TRIGGER_IFACE_OPCODE_TRANSM_LEN,
            chan, transm_len);
}

PARAM_FUNC_CLIENT_WRITE_READ(trigger_transm_len, chan, transm_len)
{
    return param_client_write_read (self, service, TRIGGER_IFACE_OPCODE_TRANSM_LEN,
            chan, transm_len);
}

/* Trigger count_receive */
PARAM_FUNC_CLIENT_WRITE2(trigger_count_rcv, chan, count_rcv)
{
    return param_client_write2 (self, service, TRIGGER_IFACE_OPCODE_COUNT_RCV,
            chan, count_rcv);
}

PARAM_FUNC_CLIENT_WRITE_READ(trigger_count_rcv, chan, count_rcv)
{
    return param_client_write_read (self, service, TRIGGER_IFACE_OPCODE_COUNT_RCV,
            chan, count_rcv);
}

/* Trigger count transmit */
PARAM_FUNC_CLIENT_WRITE2(trigger_count_transm, chan, count_transm)
{
    return param_client_write2 (self, service, TRIGGER_IFACE_OPCODE_COUNT_TRANSM,
            chan, count_transm);
}

PARAM_FUNC_CLIENT_WRITE_READ(trigger_count_transm, chan, count_transm)
{
    return param_client_write_read (self, service, TRIGGER_IFACE_OPCODE_COUNT_TRANSM,
            chan, count_transm);
}

/********************** Trigger Mux Functions ********************/

/* Trigger receive source */
PARAM_FUNC_CLIENT_WRITE2(trigger_rcv_src, chan, rcv_src)
{
    return param_client_write2 (self, service, TRIGGER_MUX_OPCODE_RCV_SRC,
            chan, rcv_src);
}

PARAM_FUNC_CLIENT_WRITE_READ(trigger_rcv_src, chan, rcv_src)
{
    return param_client_write_read (self, service, TRIGGER_MUX_OPCODE_RCV_SRC,
            chan, rcv_src);
}

/* Trigger receive in selection */
PARAM_FUNC_CLIENT_WRITE2(trigger_rcv_in_sel, chan, rcv_in_sel)
{
    return param_client_write2 (self, service, TRIGGER_MUX_OPCODE_RCV_IN_SEL,
            chan, rcv_in_sel);
}

PARAM_FUNC_CLIENT_WRITE_READ(trigger_rcv_in_sel, chan, rcv_in_sel)
{
    return param_client_write_read (self, service, TRIGGER_MUX_OPCODE_RCV_IN_SEL,
            chan, rcv_in_sel);
}

/* Trigger transmit source */
PARAM_FUNC_CLIENT_WRITE2(trigger_transm_src, chan, transm_src)
{
    return param_client_write2 (self, service, TRIGGER_MUX_OPCODE_TRANSM_SRC,
            chan, transm_src);
}

PARAM_FUNC_CLIENT_WRITE_READ(trigger_transm_src, chan, transm_src)
{
    return param_client_write_read (self, service, TRIGGER_MUX_OPCODE_TRANSM_SRC,
            chan, transm_src);
}

/* Trigger transmit selection */
PARAM_FUNC_CLIENT_WRITE2(trigger_transm_out_sel, chan, transm_out_sel)
{
    return param_client_write2 (self, service, TRIGGER_MUX_OPCODE_TRANSM_OUT_SEL,
            chan, transm_out_sel);
}

PARAM_FUNC_CLIENT_WRITE_READ(trigger_transm_out_sel, chan, transm_out_sel)
{
    return param_client_write_read (self, service, TRIGGER_MUX_OPCODE_TRANSM_OUT_SEL,
            chan, transm_out_sel);
}

/**************** Helper Function ****************/

halcs_client_err_e func_polling (halcs_client_t *self, char *name, char *service,
        uint32_t *input, uint32_t *output, int timeout)
{
    return _func_polling (self, name, service, input, output, timeout);
}

/* Polling Function */
static halcs_client_err_e _func_polling (halcs_client_t *self, char *name,
        char *service, uint32_t *input, uint32_t *output, int timeout)
{
    assert (self);
    assert (name);
    assert (service);

    /* timeout < 0 means "infinite" wait */
    if (timeout < 0) {
        /* FIXME! Very unportable way! We assume that time_t is at least of
         * size INT */
        timeout = INT_MAX;
    }

    halcs_client_err_e err = HALCS_CLIENT_SUCCESS;
    time_t start = time (NULL);
    while ((time(NULL) - start)*1000 < timeout) {
        if (zsys_interrupted) {
            err = HALCS_CLIENT_INT;
            goto halcs_zsys_interrupted;
        }

        const disp_op_t* func = halcs_func_translate(name);
        err = halcs_func_exec (self, func, service, input, output);

        if (err == HALCS_CLIENT_SUCCESS) {
            DBE_DEBUG (DBG_LIB_CLIENT | DBG_LVL_TRACE, "[libclient] "
                    "func_polling: finished waiting\n");
            goto exit;
        }

        usleep (MSECS*MIN_WAIT_TIME);
    }

    DBE_DEBUG (DBG_LIB_CLIENT | DBG_LVL_TRACE, "[libclient] "
            "func_polling: number of tries was exceeded\n");
    /* timeout occured */
    err = HALCS_CLIENT_ERR_TIMEOUT;

halcs_zsys_interrupted:
exit:
    return err;
}

/**************** AFC Timing SMIO Functions ****************/

/* Fiber link status function */
PARAM_FUNC_CLIENT_READ_MOD(afc_timing, link_status)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_LINK_STATUS, link_status);
}

/* RX enabled function */
PARAM_FUNC_CLIENT_READ_MOD(afc_timing, rxen_status)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_RXEN_STATUS, rxen_status);
}

/* Reference Clock output locked */
PARAM_FUNC_CLIENT_READ_MOD(afc_timing, ref_clk_locked)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_REF_CLK_LOCKED, ref_clk_locked);
}

/* Event Code Channel 0 functions */
PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, evt_code_0)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_EVT_IN0, evt_code_0);
}

PARAM_FUNC_CLIENT_READ_MOD(afc_timing, evt_code_0)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_EVT_IN0, evt_code_0);
}

/* Event Code Channel 1 functions */
PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, evt_code_1)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_EVT_IN1, evt_code_1);
}

PARAM_FUNC_CLIENT_READ_MOD(afc_timing, evt_code_1)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_EVT_IN1, evt_code_1);
}

/* Event Code Channel 2 functions */
PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, evt_code_2)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_EVT_IN2, evt_code_2);
}

PARAM_FUNC_CLIENT_READ_MOD(afc_timing, evt_code_2)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_EVT_IN2, evt_code_2);
}

/* Event Code Channel 3 functions */
PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, evt_code_3)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_EVT_IN3, evt_code_3);
}

PARAM_FUNC_CLIENT_READ_MOD(afc_timing, evt_code_3)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_EVT_IN3, evt_code_3);
}

/* Event Code Channel 4 functions */
PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, evt_code_4)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_EVT_IN4, evt_code_4);
}

PARAM_FUNC_CLIENT_READ_MOD(afc_timing, evt_code_4)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_EVT_IN4, evt_code_4);
}

/* Event Code Channel 5 functions */
PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, evt_code_5)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_EVT_IN5, evt_code_5);
}

PARAM_FUNC_CLIENT_READ_MOD(afc_timing, evt_code_5)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_EVT_IN5, evt_code_5);
}

/* Event Code Channel 6 functions */
PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, evt_code_6)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_EVT_IN6, evt_code_6);
}

PARAM_FUNC_CLIENT_READ_MOD(afc_timing, evt_code_6)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_EVT_IN6, evt_code_6);
}

/* Event Delay Channel 0 functions */
PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, evt_delay_0)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_EVT_DLY0, evt_delay_0);
}

PARAM_FUNC_CLIENT_READ_MOD(afc_timing, evt_delay_0)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_EVT_DLY0, evt_delay_0);
}

/* Event Delay Channel 1 functions */
PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, evt_delay_1)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_EVT_DLY1, evt_delay_1);
}

PARAM_FUNC_CLIENT_READ_MOD(afc_timing, evt_delay_1)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_EVT_DLY1, evt_delay_1);
}

/* Event Delay Channel 2 functions */
PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, evt_delay_2)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_EVT_DLY2, evt_delay_2);
}

PARAM_FUNC_CLIENT_READ_MOD(afc_timing, evt_delay_2)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_EVT_DLY2, evt_delay_2);
}

/* Event Delay Channel 3 functions */
PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, evt_delay_3)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_EVT_DLY3, evt_delay_3);
}

PARAM_FUNC_CLIENT_READ_MOD(afc_timing, evt_delay_3)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_EVT_DLY3, evt_delay_3);
}

/* Event Delay Channel 4 functions */
PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, evt_delay_4)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_EVT_DLY4, evt_delay_4);
}

PARAM_FUNC_CLIENT_READ_MOD(afc_timing, evt_delay_4)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_EVT_DLY4, evt_delay_4);
}

/* Event Delay Channel 5 functions */
PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, evt_delay_5)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_EVT_DLY5, evt_delay_5);
}

PARAM_FUNC_CLIENT_READ_MOD(afc_timing, evt_delay_5)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_EVT_DLY5, evt_delay_5);
}

/* Event Delay Channel 6 functions */
PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, evt_delay_6)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_EVT_DLY6, evt_delay_6);
}

PARAM_FUNC_CLIENT_READ_MOD(afc_timing, evt_delay_6)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_EVT_DLY6, evt_delay_6);
}

/* Event Width Channel 0 functions */
PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, evt_width_0)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_EVT_WDT0, evt_width_0);
}

PARAM_FUNC_CLIENT_READ_MOD(afc_timing, evt_width_0)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_EVT_WDT0, evt_width_0);
}

/* Event Width Channel 1 functions */
PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, evt_width_1)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_EVT_WDT1, evt_width_1);
}

PARAM_FUNC_CLIENT_READ_MOD(afc_timing, evt_width_1)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_EVT_WDT1, evt_width_1);
}

/* Event Width Channel 2 functions */
PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, evt_width_2)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_EVT_WDT2, evt_width_2);
}

PARAM_FUNC_CLIENT_READ_MOD(afc_timing, evt_width_2)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_EVT_WDT2, evt_width_2);
}

/* Event Width Channel 3 functions */
PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, evt_width_3)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_EVT_WDT3, evt_width_3);
}

PARAM_FUNC_CLIENT_READ_MOD(afc_timing, evt_width_3)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_EVT_WDT3, evt_width_3);
}

/* Event Width Channel 4 functions */
PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, evt_width_4)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_EVT_WDT4, evt_width_4);
}

PARAM_FUNC_CLIENT_READ_MOD(afc_timing, evt_width_4)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_EVT_WDT4, evt_width_4);
}

/* Event Width Channel 5 functions */
PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, evt_width_5)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_EVT_WDT5, evt_width_5);
}

PARAM_FUNC_CLIENT_READ_MOD(afc_timing, evt_width_5)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_EVT_WDT5, evt_width_5);
}

/* Event Width Channel 6 functions */
PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, evt_width_6)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_EVT_WDT6, evt_width_6);
}

PARAM_FUNC_CLIENT_READ_MOD(afc_timing, evt_width_6)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_EVT_WDT6, evt_width_6);
}

/* Proportional Gain of Frequency feedback functions */
PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, freq_kp)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_FREQ_KP, freq_kp);
}

PARAM_FUNC_CLIENT_READ_MOD(afc_timing, freq_kp)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_FREQ_KP, freq_kp);
}

/* Integral Gain of Frequency feedback functions */
PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, freq_ki)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_FREQ_KI, freq_ki);
}

PARAM_FUNC_CLIENT_READ_MOD(afc_timing, freq_ki)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_FREQ_KI, freq_ki);
}

/* Proportional Gain of Phase feedback functions */
PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, phase_kp)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_PHASE_KP, phase_kp);
}

PARAM_FUNC_CLIENT_READ_MOD(afc_timing, phase_kp)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_PHASE_KP, phase_kp);
}

/* Integral Gain of Phase feedback functions */
PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, phase_ki)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_PHASE_KI, phase_ki);
}

PARAM_FUNC_CLIENT_READ_MOD(afc_timing, phase_ki)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_PHASE_KI, phase_ki);
}

/* Phase Setpoint functions */
PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, phase_set)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_PHASE_SET, phase_set);
}

PARAM_FUNC_CLIENT_READ_MOD(afc_timing, phase_set)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_PHASE_SET, phase_set);
}

/* Average Exponent functions */
PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, avg_exponent)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_AVG_EXPONENT, avg_exponent);
}

PARAM_FUNC_CLIENT_READ_MOD(afc_timing, avg_exponent)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_AVG_EXPONENT, avg_exponent);
}

/* RTM Si570 rfreq[37-19] functions */
PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, rtm_rfreq_hi)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_RTM_RFREQ_HI, rtm_rfreq_hi);
}

PARAM_FUNC_CLIENT_READ_MOD(afc_timing, rtm_rfreq_hi)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_RTM_RFREQ_HI, rtm_rfreq_hi);
}

/* RTM Si570 rfreq[18-0] functions */
PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, rtm_rfreq_lo)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_RTM_RFREQ_LO, rtm_rfreq_lo);
}

PARAM_FUNC_CLIENT_READ_MOD(afc_timing, rtm_rfreq_lo)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_RTM_RFREQ_LO, rtm_rfreq_lo);
}

/* RTM Si570 n1 functions */
PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, rtm_n1)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_RTM_N1, rtm_n1);
}

PARAM_FUNC_CLIENT_READ_MOD(afc_timing, rtm_n1)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_RTM_N1, rtm_n1);
}

/* RTM Si570 hs_div functions */
PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, rtm_hs_div)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_RTM_HS_DIV, rtm_hs_div);
}

PARAM_FUNC_CLIENT_READ_MOD(afc_timing, rtm_hs_div)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_RTM_HS_DIV, rtm_hs_div);
}

/* AFC Si570 rfreq[37-19] functions */
PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, afc_rfreq_hi)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_AFC_RFREQ_HI, afc_rfreq_hi);
}

PARAM_FUNC_CLIENT_READ_MOD(afc_timing, afc_rfreq_hi)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_AFC_RFREQ_HI, afc_rfreq_hi);
}

/* AFC Si570 rfreq[18-0] functions */
PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, afc_rfreq_lo)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_AFC_RFREQ_LO, afc_rfreq_lo);
}

PARAM_FUNC_CLIENT_READ_MOD(afc_timing, afc_rfreq_lo)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_AFC_RFREQ_LO, afc_rfreq_lo);
}

/* AFC Si570 n1 functions */
PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, afc_n1)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_AFC_N1, afc_n1);
}

PARAM_FUNC_CLIENT_READ_MOD(afc_timing, afc_n1)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_AFC_N1, afc_n1);
}

/* AFC Si570 hs_div functions */
PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, afc_hs_div)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_AFC_HS_DIV, afc_hs_div);
}

PARAM_FUNC_CLIENT_READ_MOD(afc_timing, afc_hs_div)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_AFC_HS_DIV, afc_hs_div);
}

