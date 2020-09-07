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
#define HALCSCLIENT_MLM_CONNECT_TIMEOUT       5000        /* in ms */
#define HALCSCLIENT_DFLT_TIMEOUT              5000        /* in ms */

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

    /* No CZMQ logs by default */
    zsys_set_logstream (NULL);

    DBE_DEBUG (DBG_LIB_CLIENT | DBG_LVL_TRACE, "[libclient] Spawing LIBHALCSCLIENT"
            " with broker address %s, with logfile on %s ...\n", broker_endp,
            (log_file_name == NULL) ? "NULL" : log_file_name);

    /* Print Software info */
    DBE_DEBUG (DBG_LIB_CLIENT | DBG_LVL_TRACE, "[libclient] HALCS Client version %s,"
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
    uint32_t module_err = 0;
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
    zmsg_t *report = param_client_recv_timeout (self, service);
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

    module_err = *(uint32_t *)zframe_data(err_code);
    ASSERT_TEST(module_err < (HALCS_CLIENT_ERR_END - HALCS_CLIENT_ERR_MODULE - 1),
            "Invalid return code", err_inv_ret_code, HALCS_CLIENT_ERR_INV_RETCODE);
    if (module_err > 0) {
        err = HALCS_CLIENT_ERR_MODULE + module_err;
    }

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
err_inv_ret_code:
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

PARAM_FUNC_CLIENT_WRITE(adc_mmcm_rst)
{
    return param_client_write (self, service, FMC_ADC_COMMON_OPCODE_MMCM_RST, adc_mmcm_rst);
}

PARAM_FUNC_CLIENT_READ(adc_mmcm_rst)
{
     return param_client_read (self, service, FMC_ADC_COMMON_OPCODE_MMCM_RST, adc_mmcm_rst);
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

PARAM_FUNC_CLIENT_WRITE(rst_swap)
{
    return param_client_write (self, service, FMC_ACTIVE_CLK_OPCODE_RST_SWAP,
            rst_swap);
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

PARAM_FUNC_CLIENT_WRITE_DOUBLE(si571_fstartup)
{
    return param_client_write_double (self, service, FMC_ACTIVE_CLK_OPCODE_SI571_FSTARTUP,
            si571_fstartup);
}

PARAM_FUNC_CLIENT_READ_DOUBLE(si571_fstartup)
{
    return param_client_read_double (self, service, FMC_ACTIVE_CLK_OPCODE_SI571_FSTARTUP,
            si571_fstartup);
}

/**************** FMC 100M SMIO Functions ****************/

PARAM_FUNC_CLIENT_READ(adc100_status)
{
     return param_client_read (self, service, FMC_100M_4CH_OPCODE_STATUS, adc100_status);
}

PARAM_FUNC_CLIENT_WRITE(adc100_acq_led)
{
    return param_client_write (self, service, FMC_100M_4CH_OPCODE_ACQ_LED, adc100_acq_led);
}

PARAM_FUNC_CLIENT_READ(adc100_acq_led)
{
     return param_client_read (self, service, FMC_100M_4CH_OPCODE_ACQ_LED, adc100_acq_led);
}

PARAM_FUNC_CLIENT_WRITE_READ(adc100_adc_status, chan, status)
{
     return param_client_write_read (self, service, FMC_100M_4CH_OPCODE_ADC_STATUS,
             chan, status);
}

PARAM_FUNC_CLIENT_WRITE2(adc100_adc_calib_gain, chan, gain)
{
    return param_client_write2 (self, service, FMC_100M_4CH_OPCODE_ADC_CALIB_GAIN,
            chan, gain);
}

PARAM_FUNC_CLIENT_WRITE_READ(adc100_adc_calib_gain, chan, gain)
{
     return param_client_write_read (self, service, FMC_100M_4CH_OPCODE_ADC_CALIB_GAIN,
             chan, gain);
}

PARAM_FUNC_CLIENT_WRITE2(adc100_adc_calib_offset, chan, offset)
{
    return param_client_write2 (self, service, FMC_100M_4CH_OPCODE_ADC_CALIB_OFFSET,
            chan, offset);
}

PARAM_FUNC_CLIENT_WRITE_READ(adc100_adc_calib_offset, chan, offset)
{
     return param_client_write_read (self, service, FMC_100M_4CH_OPCODE_ADC_CALIB_OFFSET,
             chan, offset);
}

PARAM_FUNC_CLIENT_WRITE2(adc100_adc_sat, chan, sat)
{
    return param_client_write2 (self, service, FMC_100M_4CH_OPCODE_ADC_SAT,
            chan, sat);
}

PARAM_FUNC_CLIENT_WRITE_READ(adc100_adc_sat, chan, sat)
{
     return param_client_write_read (self, service, FMC_100M_4CH_OPCODE_ADC_SAT,
             chan, sat);
}

PARAM_FUNC_CLIENT_WRITE2(adc100_adc_ssr, chan, sat)
{
    return param_client_write2 (self, service, FMC_100M_4CH_OPCODE_ADC_SSR,
            chan, sat);
}

PARAM_FUNC_CLIENT_WRITE_READ(adc100_adc_ssr, chan, sat)
{
     return param_client_write_read (self, service, FMC_100M_4CH_OPCODE_ADC_SSR,
             chan, sat);
}

PARAM_FUNC_CLIENT_READ(adc100_fs_freq)
{
     return param_client_read (self, service, FMC_100M_4CH_OPCODE_FS_FREQ, adc100_fs_freq);
}

/* LTC2174 Test Pattern */
PARAM_FUNC_CLIENT_WRITE(adc100_ltc2174_test_patt)
{
    return param_client_write (self, service, FMC_100M_4CH_OPCODE_LTC2174_TEST_PATT,
            adc100_ltc2174_test_patt);
}

PARAM_FUNC_CLIENT_READ(adc100_ltc2174_test_patt)
{
    return param_client_read (self, service, FMC_100M_4CH_OPCODE_LTC2174_TEST_PATT,
            adc100_ltc2174_test_patt);
}

/* LTC2174 Test Pattern Data */
PARAM_FUNC_CLIENT_WRITE(adc100_ltc2174_test_patt_data)
{
    return param_client_write (self, service, FMC_100M_4CH_OPCODE_LTC2174_TEST_PATT_DATA,
            adc100_ltc2174_test_patt_data);
}

PARAM_FUNC_CLIENT_READ(adc100_ltc2174_test_patt_data)
{
    return param_client_read (self, service, FMC_100M_4CH_OPCODE_LTC2174_TEST_PATT_DATA,
            adc100_ltc2174_test_patt_data);
}

PARAM_FUNC_CLIENT_WRITE(adc100_ltc2174_reset)
{
    return param_client_write (self, service, FMC_100M_4CH_OPCODE_LTC2174_RESET,
            adc100_ltc2174_reset);
}

/* LTC2174 Test Pattern Data */
PARAM_FUNC_CLIENT_WRITE(adc100_ltc2174_twos_compl)
{
    return param_client_write (self, service, FMC_100M_4CH_OPCODE_LTC2174_TWOS_COMPL,
            adc100_ltc2174_twos_compl);
}

PARAM_FUNC_CLIENT_READ(adc100_ltc2174_twos_compl)
{
    return param_client_read (self, service, FMC_100M_4CH_OPCODE_LTC2174_TWOS_COMPL,
            adc100_ltc2174_twos_compl);
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

/* Monitoring Update value */
PARAM_FUNC_CLIENT_WRITE(monit_poll_time)
{
    return param_client_write (self, service, DSP_OPCODE_SET_GET_MONIT_POLL_TIME, monit_poll_time);
}

PARAM_FUNC_CLIENT_READ(monit_poll_time)
{
    return param_client_read (self, service, DSP_OPCODE_SET_GET_MONIT_POLL_TIME, monit_poll_time);
}

halcs_client_err_e halcs_set_monit_subscription (halcs_client_t *self, const char *stream,
        const char *pattern)
{
    assert (self);
    assert (stream);
    assert (pattern);
    halcs_client_err_e err = HALCS_CLIENT_SUCCESS;

    int rc = mlm_client_set_consumer (self->mlm_client, stream, pattern);
    if (rc != 0) {
        err = HALCS_CLIENT_ERR_TIMEOUT;
    }

    return err;
}

halcs_client_err_e halcs_remove_monit_subscription (halcs_client_t *self, const char *stream)
{
    assert (self);
    assert (stream);
    halcs_client_err_e err = HALCS_CLIENT_SUCCESS;

    int rc = mlm_client_remove_consumer (self->mlm_client, stream);
    if (rc != 0) {
        err = HALCS_CLIENT_ERR_TIMEOUT;
    }

    return err;
}

halcs_client_err_e halcs_get_monit_stream (halcs_client_t *self,
        const char *subject, struct _smio_dsp_monit_data_t *monit_data)
{
    assert (self);
    assert(monit_data);
    halcs_client_err_e err = HALCS_CLIENT_SUCCESS;

    zmsg_t *msg = mlm_client_recv (self->mlm_client);
    if (msg == NULL) { /* interrupted */
        err = HALCS_CLIENT_ERR_MSG;
        goto err_null_msg;
    }

    const char *mlm_subject = mlm_client_subject (self->mlm_client);
    if (!streq (mlm_subject, subject)) {
        DBE_DEBUG (DBG_LIB_CLIENT | DBG_LVL_FATAL, "[libclient] "
                "halcs_get_monit_stream: Unexpected subject %s, waiting for %s\n",
                 mlm_subject, subject);
        err = HALCS_CLIENT_ERR_MSG;
        goto err_unexpected_subject;
    }

    zframe_t *frame = zmsg_pop (msg);
    ASSERT_ALLOC(frame, err_msg_alloc, HALCS_CLIENT_ERR_ALLOC);
    ASSERT_TEST(zframe_size (frame) == sizeof (*monit_data),
            "Unexpected message received", err_msg_size);

    uint32_t *data_out = (uint32_t *) zframe_data (frame);
    memcpy (monit_data, data_out, sizeof (*monit_data));

err_msg_size:
    zframe_destroy (&frame);
err_msg_alloc:
err_unexpected_subject:
    zmsg_destroy (&msg);
err_null_msg:
    return err;
}

/* Monitoring 1 Amplitude channel 0 value */
PARAM_FUNC_CLIENT_WRITE(monit1_amp_ch0)
{
    return param_client_write (self, service, DSP_OPCODE_SET_GET_MONIT1_AMP_CH0, monit1_amp_ch0);
}

PARAM_FUNC_CLIENT_READ(monit1_amp_ch0)
{
     return param_client_read (self, service, DSP_OPCODE_SET_GET_MONIT1_AMP_CH0, monit1_amp_ch0);
}

/* Monitoring 1 Amplitude channel 1 value */
PARAM_FUNC_CLIENT_WRITE(monit1_amp_ch1)
{
    return param_client_write (self, service, DSP_OPCODE_SET_GET_MONIT1_AMP_CH1, monit1_amp_ch1);
}

PARAM_FUNC_CLIENT_READ(monit1_amp_ch1)
{
     return param_client_read (self, service, DSP_OPCODE_SET_GET_MONIT1_AMP_CH1, monit1_amp_ch1);
}

/* Monitoring 1 Amplitude channel 2 value */
PARAM_FUNC_CLIENT_WRITE(monit1_amp_ch2)
{
    return param_client_write (self, service, DSP_OPCODE_SET_GET_MONIT1_AMP_CH2, monit1_amp_ch2);
}

PARAM_FUNC_CLIENT_READ(monit1_amp_ch2)
{
     return param_client_read (self, service, DSP_OPCODE_SET_GET_MONIT1_AMP_CH2, monit1_amp_ch2);
}

/* Monitoring 1 Amplitude channel 3 value */
PARAM_FUNC_CLIENT_WRITE(monit1_amp_ch3)
{
    return param_client_write (self, service, DSP_OPCODE_SET_GET_MONIT1_AMP_CH3, monit1_amp_ch3);
}

PARAM_FUNC_CLIENT_READ(monit1_amp_ch3)
{
     return param_client_read (self, service, DSP_OPCODE_SET_GET_MONIT1_AMP_CH3, monit1_amp_ch3);
}

/* Monitoring 1 Position X value */
PARAM_FUNC_CLIENT_WRITE(monit1_pos_x)
{
    return param_client_write (self, service, DSP_OPCODE_SET_GET_MONIT1_POS_X, monit1_pos_x);
}

PARAM_FUNC_CLIENT_READ(monit1_pos_x)
{
     return param_client_read (self, service, DSP_OPCODE_SET_GET_MONIT1_POS_X, monit1_pos_x);
}

/* Monitoring 1 Position Y value */
PARAM_FUNC_CLIENT_WRITE(monit1_pos_y)
{
    return param_client_write (self, service, DSP_OPCODE_SET_GET_MONIT1_POS_Y, monit1_pos_y);
}

PARAM_FUNC_CLIENT_READ(monit1_pos_y)
{
     return param_client_read (self, service, DSP_OPCODE_SET_GET_MONIT1_POS_Y, monit1_pos_y);
}

/* Monitoring 1 Position Q value */
PARAM_FUNC_CLIENT_WRITE(monit1_pos_q)
{
    return param_client_write (self, service, DSP_OPCODE_SET_GET_MONIT1_POS_Q, monit1_pos_q);
}

PARAM_FUNC_CLIENT_READ(monit1_pos_q)
{
     return param_client_read (self, service, DSP_OPCODE_SET_GET_MONIT1_POS_Q, monit1_pos_q);
}

/* Monitoring 1 Position Sum value */
PARAM_FUNC_CLIENT_WRITE(monit1_pos_sum)
{
    return param_client_write (self, service, DSP_OPCODE_SET_GET_MONIT1_POS_SUM, monit1_pos_sum);
}

PARAM_FUNC_CLIENT_READ(monit1_pos_sum)
{
     return param_client_read (self, service, DSP_OPCODE_SET_GET_MONIT1_POS_SUM, monit1_pos_sum);
}

/* Monitoring 1 Update value */
PARAM_FUNC_CLIENT_WRITE(monit1_updt)
{
    return param_client_write (self, service, DSP_OPCODE_SET_GET_MONIT1_UPDT, monit1_updt);
}

PARAM_FUNC_CLIENT_READ(monit1_updt)
{
    return param_client_read (self, service, DSP_OPCODE_SET_GET_MONIT1_UPDT, monit1_updt);
}

/* Monitoring 1 AMP/POS funcion */
halcs_client_err_e halcs_get_monit1_amp_pos (halcs_client_t *self, char *service,
        struct _smio_dsp_data_t *dsp_data)
{
    assert (self);
    assert (service);

    /* Use the direct function table pointer to speed up the process */
    const disp_op_t* func = &dsp_set_get_monit1_amp_pos_exp;
    halcs_client_err_e err = halcs_func_exec(self, func, service,
            NULL, (uint32_t *)dsp_data);
    ASSERT_TEST(err == HALCS_CLIENT_SUCCESS, "Could not get Monit. AMP/POS",
            err_get_amp_pos_data);

    return err;

err_get_amp_pos_data:
    return err;
}

/* Switching Tag Enable */
PARAM_FUNC_CLIENT_WRITE(sw_tag_en)
{
    return param_client_write (self, service, DSP_OPCODE_SET_GET_SW_TAG_EN, sw_tag_en);
}

PARAM_FUNC_CLIENT_READ(sw_tag_en)
{
     return param_client_read (self, service, DSP_OPCODE_SET_GET_SW_TAG_EN, sw_tag_en);
}

/* Switching Data Mask Enable */
PARAM_FUNC_CLIENT_WRITE(sw_data_mask_en)
{
    return param_client_write (self, service, DSP_OPCODE_SET_GET_SW_DATA_MASK_EN, sw_data_mask_en);
}

PARAM_FUNC_CLIENT_READ(sw_data_mask_en)
{
    return param_client_read (self, service, DSP_OPCODE_SET_GET_SW_DATA_MASK_EN, sw_data_mask_en);
}

/* Switching Data Mask Samples */
PARAM_FUNC_CLIENT_WRITE(sw_data_mask_samples)
{
    return param_client_write (self, service, DSP_OPCODE_SET_GET_SW_DATA_MASK_SAMPLES, sw_data_mask_samples);
}

PARAM_FUNC_CLIENT_READ(sw_data_mask_samples)
{
    return param_client_read (self, service, DSP_OPCODE_SET_GET_SW_DATA_MASK_SAMPLES, sw_data_mask_samples);
}

/* TBT Tag Enable */
PARAM_FUNC_CLIENT_WRITE(tbt_tag_en)
{
    return param_client_write (self, service, DSP_OPCODE_SET_GET_TBT_TAG_EN, tbt_tag_en);
}

PARAM_FUNC_CLIENT_READ(tbt_tag_en)
{
     return param_client_read (self, service, DSP_OPCODE_SET_GET_TBT_TAG_EN, tbt_tag_en);
}

/* TBT Tag Delay */
PARAM_FUNC_CLIENT_WRITE(tbt_tag_dly)
{
    return param_client_write (self, service, DSP_OPCODE_SET_GET_TBT_TAG_DLY, tbt_tag_dly);
}

PARAM_FUNC_CLIENT_READ(tbt_tag_dly)
{
     return param_client_read (self, service, DSP_OPCODE_SET_GET_TBT_TAG_DLY, tbt_tag_dly);
}

/* TBT Data Mask Enable */
PARAM_FUNC_CLIENT_WRITE(tbt_data_mask_en)
{
    return param_client_write (self, service, DSP_OPCODE_SET_GET_TBT_DATA_MASK_EN, tbt_data_mask_en);
}

PARAM_FUNC_CLIENT_READ(tbt_data_mask_en)
{
    return param_client_read (self, service, DSP_OPCODE_SET_GET_TBT_DATA_MASK_EN, tbt_data_mask_en);
}

/* TBT Data Mask Samples Begginning */
PARAM_FUNC_CLIENT_WRITE(tbt_data_mask_samples_beg)
{
    return param_client_write (self, service, DSP_OPCODE_SET_GET_TBT_DATA_MASK_SAMPLES_BEG, tbt_data_mask_samples_beg);
}

PARAM_FUNC_CLIENT_READ(tbt_data_mask_samples_beg)
{
    return param_client_read (self, service, DSP_OPCODE_SET_GET_TBT_DATA_MASK_SAMPLES_BEG, tbt_data_mask_samples_beg);
}

/* TBT Data Mask Samples Ending */
PARAM_FUNC_CLIENT_WRITE(tbt_data_mask_samples_end)
{
    return param_client_write (self, service, DSP_OPCODE_SET_GET_TBT_DATA_MASK_SAMPLES_END, tbt_data_mask_samples_end);
}

PARAM_FUNC_CLIENT_READ(tbt_data_mask_samples_end)
{
    return param_client_read (self, service, DSP_OPCODE_SET_GET_TBT_DATA_MASK_SAMPLES_END, tbt_data_mask_samples_end);
}

/* SW Tag Desync Counter Reset */
PARAM_FUNC_CLIENT_WRITE(sw_tag_desync_cnt_rst)
{
    return param_client_write (self, service, DSP_OPCODE_SET_GET_SW_TAG_DESYNC_CNT_RST, sw_tag_desync_cnt_rst);
}

PARAM_FUNC_CLIENT_READ(sw_tag_desync_cnt_rst)
{
    return param_client_read (self, service, DSP_OPCODE_SET_GET_SW_TAG_DESYNC_CNT_RST, sw_tag_desync_cnt_rst);
}

/* SW Tag Desync Counter */
PARAM_FUNC_CLIENT_READ(sw_tag_desync_cnt)
{
    return param_client_read (self, service, DSP_OPCODE_SET_GET_SW_TAG_DESYNC_CNT, sw_tag_desync_cnt);
}

/* TBT Tag Desync Counter Reset */
PARAM_FUNC_CLIENT_WRITE(tbt_tag_desync_cnt_rst)
{
    return param_client_write (self, service, DSP_OPCODE_SET_GET_TBT_TAG_DESYNC_CNT_RST, tbt_tag_desync_cnt_rst);
}

PARAM_FUNC_CLIENT_READ(tbt_tag_desync_cnt_rst)
{
    return param_client_read (self, service, DSP_OPCODE_SET_GET_TBT_TAG_DESYNC_CNT_RST, tbt_tag_desync_cnt_rst);
}

/* TBT Tag Desync Counter */
PARAM_FUNC_CLIENT_READ(tbt_tag_desync_cnt)
{
    return param_client_read (self, service, DSP_OPCODE_SET_GET_TBT_TAG_DESYNC_CNT, tbt_tag_desync_cnt);
}

/* MONIT1 Tag Enable */
PARAM_FUNC_CLIENT_WRITE(monit1_tag_en)
{
    return param_client_write (self, service, DSP_OPCODE_SET_GET_MONIT1_TAG_EN, monit1_tag_en);
}

PARAM_FUNC_CLIENT_READ(monit1_tag_en)
{
     return param_client_read (self, service, DSP_OPCODE_SET_GET_MONIT1_TAG_EN, monit1_tag_en);
}

/* MONIT1 Tag Delay */
PARAM_FUNC_CLIENT_WRITE(monit1_tag_dly)
{
    return param_client_write (self, service, DSP_OPCODE_SET_GET_MONIT1_TAG_DLY, monit1_tag_dly);
}

PARAM_FUNC_CLIENT_READ(monit1_tag_dly)
{
     return param_client_read (self, service, DSP_OPCODE_SET_GET_MONIT1_TAG_DLY, monit1_tag_dly);
}

/* MONIT1 Data Mask Enable */
PARAM_FUNC_CLIENT_WRITE(monit1_data_mask_en)
{
    return param_client_write (self, service, DSP_OPCODE_SET_GET_MONIT1_DATA_MASK_EN, monit1_data_mask_en);
}

PARAM_FUNC_CLIENT_READ(monit1_data_mask_en)
{
    return param_client_read (self, service, DSP_OPCODE_SET_GET_MONIT1_DATA_MASK_EN, monit1_data_mask_en);
}

/* MONIT1 Data Mask Samples Begginning */
PARAM_FUNC_CLIENT_WRITE(monit1_data_mask_samples_beg)
{
    return param_client_write (self, service, DSP_OPCODE_SET_GET_MONIT1_DATA_MASK_SAMPLES_BEG, monit1_data_mask_samples_beg);
}

PARAM_FUNC_CLIENT_READ(monit1_data_mask_samples_beg)
{
    return param_client_read (self, service, DSP_OPCODE_SET_GET_MONIT1_DATA_MASK_SAMPLES_BEG, monit1_data_mask_samples_beg);
}

/* MONIT1 Data Mask Samples Ending */
PARAM_FUNC_CLIENT_WRITE(monit1_data_mask_samples_end)
{
    return param_client_write (self, service, DSP_OPCODE_SET_GET_MONIT1_DATA_MASK_SAMPLES_END, monit1_data_mask_samples_end);
}

PARAM_FUNC_CLIENT_READ(monit1_data_mask_samples_end)
{
    return param_client_read (self, service, DSP_OPCODE_SET_GET_MONIT1_DATA_MASK_SAMPLES_END, monit1_data_mask_samples_end);
}

/* MONIT1 Tag Desync Counter Reset */
PARAM_FUNC_CLIENT_WRITE(monit1_tag_desync_cnt_rst)
{
    return param_client_write (self, service, DSP_OPCODE_SET_GET_MONIT1_TAG_DESYNC_CNT_RST, monit1_tag_desync_cnt_rst);
}

PARAM_FUNC_CLIENT_READ(monit1_tag_desync_cnt_rst)
{
    return param_client_read (self, service, DSP_OPCODE_SET_GET_MONIT1_TAG_DESYNC_CNT_RST, monit1_tag_desync_cnt_rst);
}

/* MONIT1 Tag Desync Counter */
PARAM_FUNC_CLIENT_READ(monit1_tag_desync_cnt)
{
    return param_client_read (self, service, DSP_OPCODE_SET_GET_MONIT1_TAG_DESYNC_CNT, monit1_tag_desync_cnt);
}

/* MONIT Tag Enable */
PARAM_FUNC_CLIENT_WRITE(monit_tag_en)
{
    return param_client_write (self, service, DSP_OPCODE_SET_GET_MONIT_TAG_EN, monit_tag_en);
}

PARAM_FUNC_CLIENT_READ(monit_tag_en)
{
     return param_client_read (self, service, DSP_OPCODE_SET_GET_MONIT_TAG_EN, monit_tag_en);
}

/* MONIT Tag Delay */
PARAM_FUNC_CLIENT_WRITE(monit_tag_dly)
{
    return param_client_write (self, service, DSP_OPCODE_SET_GET_MONIT_TAG_DLY, monit_tag_dly);
}

PARAM_FUNC_CLIENT_READ(monit_tag_dly)
{
     return param_client_read (self, service, DSP_OPCODE_SET_GET_MONIT_TAG_DLY, monit_tag_dly);
}

/* MONIT Data Mask Enable */
PARAM_FUNC_CLIENT_WRITE(monit_data_mask_en)
{
    return param_client_write (self, service, DSP_OPCODE_SET_GET_MONIT_DATA_MASK_EN, monit_data_mask_en);
}

PARAM_FUNC_CLIENT_READ(monit_data_mask_en)
{
    return param_client_read (self, service, DSP_OPCODE_SET_GET_MONIT_DATA_MASK_EN, monit_data_mask_en);
}

/* MONIT Data Mask Samples Begginning */
PARAM_FUNC_CLIENT_WRITE(monit_data_mask_samples_beg)
{
    return param_client_write (self, service, DSP_OPCODE_SET_GET_MONIT_DATA_MASK_SAMPLES_BEG, monit_data_mask_samples_beg);
}

PARAM_FUNC_CLIENT_READ(monit_data_mask_samples_beg)
{
    return param_client_read (self, service, DSP_OPCODE_SET_GET_MONIT_DATA_MASK_SAMPLES_BEG, monit_data_mask_samples_beg);
}

/* MONIT Data Mask Samples Ending */
PARAM_FUNC_CLIENT_WRITE(monit_data_mask_samples_end)
{
    return param_client_write (self, service, DSP_OPCODE_SET_GET_MONIT_DATA_MASK_SAMPLES_END, monit_data_mask_samples_end);
}

PARAM_FUNC_CLIENT_READ(monit_data_mask_samples_end)
{
    return param_client_read (self, service, DSP_OPCODE_SET_GET_MONIT_DATA_MASK_SAMPLES_END, monit_data_mask_samples_end);
}

/* MONIT Tag Desync Counter Reset */
PARAM_FUNC_CLIENT_WRITE(monit_tag_desync_cnt_rst)
{
    return param_client_write (self, service, DSP_OPCODE_SET_GET_MONIT_TAG_DESYNC_CNT_RST, monit_tag_desync_cnt_rst);
}

PARAM_FUNC_CLIENT_READ(monit_tag_desync_cnt_rst)
{
    return param_client_read (self, service, DSP_OPCODE_SET_GET_MONIT_TAG_DESYNC_CNT_RST, monit_tag_desync_cnt_rst);
}

/* MONIT Tag Desync Counter */
PARAM_FUNC_CLIENT_READ(monit_tag_desync_cnt)
{
    return param_client_read (self, service, DSP_OPCODE_SET_GET_MONIT_TAG_DESYNC_CNT, monit_tag_desync_cnt);
}

/* Offset X value */
PARAM_FUNC_CLIENT_WRITE_SIGNED(offset_x)
{
    return param_client_write_signed (self, service, DSP_OPCODE_SET_GET_OFFSET_X, offset_x);
}

PARAM_FUNC_CLIENT_READ_SIGNED(offset_x)
{
     return param_client_read_signed (self, service, DSP_OPCODE_SET_GET_OFFSET_X, offset_x);
}

/* Offset Y value */
PARAM_FUNC_CLIENT_WRITE_SIGNED(offset_y)
{
    return param_client_write_signed (self, service, DSP_OPCODE_SET_GET_OFFSET_Y, offset_y);
}

PARAM_FUNC_CLIENT_READ_SIGNED(offset_y)
{
     return param_client_read_signed (self, service, DSP_OPCODE_SET_GET_OFFSET_Y, offset_y);
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
    uint32_t dummy = 0;
    return param_client_read_gen (self, service, AFC_DIAG_OPCODE_GET_BUILD_REVISION,
            rw, &dummy, sizeof (dummy), NULL, 0, NULL, 0,
            revision_data, sizeof (*revision_data));
}

/* Build Date */
halcs_client_err_e halcs_get_afc_diag_build_date (halcs_client_t *self, char *service,
        struct _smio_afc_diag_revision_data_t *revision_data)
{
    uint32_t rw = READ_MODE;
    uint32_t dummy = 0;
    return param_client_read_gen (self, service, AFC_DIAG_OPCODE_GET_BUILD_DATE,
            rw, &dummy, sizeof (dummy), NULL, 0, NULL, 0,
            revision_data, sizeof (*revision_data));
}

/* Build User Name */
halcs_client_err_e halcs_get_afc_diag_build_user_name (halcs_client_t *self, char *service,
        struct _smio_afc_diag_revision_data_t *revision_data)
{
    uint32_t rw = READ_MODE;
    uint32_t dummy = 0;
    return param_client_read_gen (self, service, AFC_DIAG_OPCODE_GET_BUILD_USER_NAME,
            rw, &dummy, sizeof (dummy), NULL, 0, NULL, 0,
            revision_data, sizeof (*revision_data));
}

/* Build User Email */
halcs_client_err_e halcs_get_afc_diag_build_user_email (halcs_client_t *self, char *service,
        struct _smio_afc_diag_revision_data_t *revision_data)
{
    uint32_t rw = READ_MODE;
    uint32_t dummy = 0;
    return param_client_read_gen (self, service, AFC_DIAG_OPCODE_GET_BUILD_USER_EMAIL,
            rw, &dummy, sizeof (dummy), NULL, 0, NULL, 0,
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

PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, link_status)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_LINK_STATUS, link_status);
}
PARAM_FUNC_CLIENT_READ_MOD(afc_timing, link_status)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_LINK_STATUS, link_status);
}

PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, rxen_status)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_RXEN_STATUS, rxen_status);
}
PARAM_FUNC_CLIENT_READ_MOD(afc_timing, rxen_status)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_RXEN_STATUS, rxen_status);
}

PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, ref_clk_locked)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_REF_CLK_LOCKED, ref_clk_locked);
}
PARAM_FUNC_CLIENT_READ_MOD(afc_timing, ref_clk_locked)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_REF_CLK_LOCKED, ref_clk_locked);
}

PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, evren)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_EVREN, evren);
}
PARAM_FUNC_CLIENT_READ_MOD(afc_timing, evren)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_EVREN, evren);
}

PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, alive)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_ALIVE, alive);
}
PARAM_FUNC_CLIENT_READ_MOD(afc_timing, alive)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_ALIVE, alive);
}

// AMC channels
PARAM_FUNC_CLIENT_WRITE2(afc_timing_amc_en, chan, amc_en)
{
    return param_client_write2 (self, service, AFC_TIMING_OPCODE_SET_GET_AMC_EN, chan, amc_en);
}
PARAM_FUNC_CLIENT_WRITE_READ(afc_timing_amc_en, chan, amc_en)
{
    return param_client_write_read (self, service, AFC_TIMING_OPCODE_SET_GET_AMC_EN, chan, amc_en);
}

PARAM_FUNC_CLIENT_WRITE2(afc_timing_amc_pol, chan, amc_pol)
{
    return param_client_write2 (self, service, AFC_TIMING_OPCODE_SET_GET_AMC_POL, chan, amc_pol);
}
PARAM_FUNC_CLIENT_WRITE_READ(afc_timing_amc_pol, chan, amc_pol)
{
    return param_client_write_read (self, service, AFC_TIMING_OPCODE_SET_GET_AMC_POL, chan, amc_pol);
}

PARAM_FUNC_CLIENT_WRITE2(afc_timing_amc_log, chan, amc_log)
{
    return param_client_write2 (self, service, AFC_TIMING_OPCODE_SET_GET_AMC_LOG, chan, amc_log);
}
PARAM_FUNC_CLIENT_WRITE_READ(afc_timing_amc_log, chan, amc_log)
{
    return param_client_write_read (self, service, AFC_TIMING_OPCODE_SET_GET_AMC_LOG, chan, amc_log);
}

PARAM_FUNC_CLIENT_WRITE2(afc_timing_amc_itl, chan, amc_itl)
{
    return param_client_write2 (self, service, AFC_TIMING_OPCODE_SET_GET_AMC_ITL, chan, amc_itl);
}
PARAM_FUNC_CLIENT_WRITE_READ(afc_timing_amc_itl, chan, amc_itl)
{
    return param_client_write_read (self, service, AFC_TIMING_OPCODE_SET_GET_AMC_ITL, chan, amc_itl);
}

PARAM_FUNC_CLIENT_WRITE2(afc_timing_amc_src, chan, amc_src)
{
    return param_client_write2 (self, service, AFC_TIMING_OPCODE_SET_GET_AMC_SRC, chan, amc_src);
}
PARAM_FUNC_CLIENT_WRITE_READ(afc_timing_amc_src, chan, amc_src)
{
    return param_client_write_read (self, service, AFC_TIMING_OPCODE_SET_GET_AMC_SRC, chan, amc_src);
}

PARAM_FUNC_CLIENT_WRITE2(afc_timing_amc_dir, chan, amc_dir)
{
    return param_client_write2 (self, service, AFC_TIMING_OPCODE_SET_GET_AMC_DIR, chan, amc_dir);
}
PARAM_FUNC_CLIENT_WRITE_READ(afc_timing_amc_dir, chan, amc_dir)
{
    return param_client_write_read (self, service, AFC_TIMING_OPCODE_SET_GET_AMC_DIR, chan, amc_dir);
}

PARAM_FUNC_CLIENT_WRITE2(afc_timing_amc_count_rst, chan, amc_count_rst)
{
    return param_client_write2 (self, service, AFC_TIMING_OPCODE_SET_GET_AMC_COUNT_RST, chan, amc_count_rst);
}
PARAM_FUNC_CLIENT_WRITE_READ(afc_timing_amc_count_rst, chan, amc_count_rst)
{
    return param_client_write_read (self, service, AFC_TIMING_OPCODE_SET_GET_AMC_COUNT_RST, chan, amc_count_rst);
}

PARAM_FUNC_CLIENT_WRITE2(afc_timing_amc_pulses, chan, amc_pulses)
{
    return param_client_write2 (self, service, AFC_TIMING_OPCODE_SET_GET_AMC_PULSES, chan, amc_pulses);
}
PARAM_FUNC_CLIENT_WRITE_READ(afc_timing_amc_pulses, chan, amc_pulses)
{
    return param_client_write_read (self, service, AFC_TIMING_OPCODE_SET_GET_AMC_PULSES, chan, amc_pulses);
}

PARAM_FUNC_CLIENT_WRITE2(afc_timing_amc_count, chan, amc_count)
{
    return param_client_write2 (self, service, AFC_TIMING_OPCODE_SET_GET_AMC_COUNT, chan, amc_count);
}
PARAM_FUNC_CLIENT_WRITE_READ(afc_timing_amc_count, chan, amc_count)
{
    return param_client_write_read (self, service, AFC_TIMING_OPCODE_SET_GET_AMC_COUNT, chan, amc_count);
}

PARAM_FUNC_CLIENT_WRITE2(afc_timing_amc_evt, chan, amc_evt)
{
    return param_client_write2 (self, service, AFC_TIMING_OPCODE_SET_GET_AMC_EVT, chan, amc_evt);
}
PARAM_FUNC_CLIENT_WRITE_READ(afc_timing_amc_evt, chan, amc_evt)
{
    return param_client_write_read (self, service, AFC_TIMING_OPCODE_SET_GET_AMC_EVT, chan, amc_evt);
}

PARAM_FUNC_CLIENT_WRITE2(afc_timing_amc_dly, chan, amc_dly)
{
    return param_client_write2 (self, service, AFC_TIMING_OPCODE_SET_GET_AMC_DLY, chan, amc_dly);
}
PARAM_FUNC_CLIENT_WRITE_READ(afc_timing_amc_dly, chan, amc_dly)
{
    return param_client_write_read (self, service, AFC_TIMING_OPCODE_SET_GET_AMC_DLY, chan, amc_dly);
}

PARAM_FUNC_CLIENT_WRITE2(afc_timing_amc_wdt, chan, amc_wdt)
{
    return param_client_write2 (self, service, AFC_TIMING_OPCODE_SET_GET_AMC_WDT, chan, amc_wdt);
}
PARAM_FUNC_CLIENT_WRITE_READ(afc_timing_amc_wdt, chan, amc_wdt)
{
    return param_client_write_read (self, service, AFC_TIMING_OPCODE_SET_GET_AMC_WDT, chan, amc_wdt);
}

// FMC 1 channels
PARAM_FUNC_CLIENT_WRITE2(afc_timing_fmc1_en, chan, fmc1_en)
{
    return param_client_write2 (self, service, AFC_TIMING_OPCODE_SET_GET_FMC1_EN, chan, fmc1_en);
}
PARAM_FUNC_CLIENT_WRITE_READ(afc_timing_fmc1_en, chan, fmc1_en)
{
    return param_client_write_read (self, service, AFC_TIMING_OPCODE_SET_GET_FMC1_EN, chan, fmc1_en);
}

PARAM_FUNC_CLIENT_WRITE2(afc_timing_fmc1_pol, chan, fmc1_pol)
{
    return param_client_write2 (self, service, AFC_TIMING_OPCODE_SET_GET_FMC1_POL, chan, fmc1_pol);
}
PARAM_FUNC_CLIENT_WRITE_READ(afc_timing_fmc1_pol, chan, fmc1_pol)
{
    return param_client_write_read (self, service, AFC_TIMING_OPCODE_SET_GET_FMC1_POL, chan, fmc1_pol);
}

PARAM_FUNC_CLIENT_WRITE2(afc_timing_fmc1_log, chan, fmc1_log)
{
    return param_client_write2 (self, service, AFC_TIMING_OPCODE_SET_GET_FMC1_LOG, chan, fmc1_log);
}
PARAM_FUNC_CLIENT_WRITE_READ(afc_timing_fmc1_log, chan, fmc1_log)
{
    return param_client_write_read (self, service, AFC_TIMING_OPCODE_SET_GET_FMC1_LOG, chan, fmc1_log);
}

PARAM_FUNC_CLIENT_WRITE2(afc_timing_fmc1_itl, chan, fmc1_itl)
{
    return param_client_write2 (self, service, AFC_TIMING_OPCODE_SET_GET_FMC1_ITL, chan, fmc1_itl);
}
PARAM_FUNC_CLIENT_WRITE_READ(afc_timing_fmc1_itl, chan, fmc1_itl)
{
    return param_client_write_read (self, service, AFC_TIMING_OPCODE_SET_GET_FMC1_ITL, chan, fmc1_itl);
}

PARAM_FUNC_CLIENT_WRITE2(afc_timing_fmc1_src, chan, fmc1_src)
{
    return param_client_write2 (self, service, AFC_TIMING_OPCODE_SET_GET_FMC1_SRC, chan, fmc1_src);
}
PARAM_FUNC_CLIENT_WRITE_READ(afc_timing_fmc1_src, chan, fmc1_src)
{
    return param_client_write_read (self, service, AFC_TIMING_OPCODE_SET_GET_FMC1_SRC, chan, fmc1_src);
}

PARAM_FUNC_CLIENT_WRITE2(afc_timing_fmc1_dir, chan, fmc1_dir)
{
    return param_client_write2 (self, service, AFC_TIMING_OPCODE_SET_GET_FMC1_DIR, chan, fmc1_dir);
}
PARAM_FUNC_CLIENT_WRITE_READ(afc_timing_fmc1_dir, chan, fmc1_dir)
{
    return param_client_write_read (self, service, AFC_TIMING_OPCODE_SET_GET_FMC1_DIR, chan, fmc1_dir);
}

PARAM_FUNC_CLIENT_WRITE2(afc_timing_fmc1_count_rst, chan, fmc1_count_rst)
{
    return param_client_write2 (self, service, AFC_TIMING_OPCODE_SET_GET_FMC1_COUNT_RST, chan, fmc1_count_rst);
}
PARAM_FUNC_CLIENT_WRITE_READ(afc_timing_fmc1_count_rst, chan, fmc1_count_rst)
{
    return param_client_write_read (self, service, AFC_TIMING_OPCODE_SET_GET_FMC1_COUNT_RST, chan, fmc1_count_rst);
}

PARAM_FUNC_CLIENT_WRITE2(afc_timing_fmc1_pulses, chan, fmc1_pulses)
{
    return param_client_write2 (self, service, AFC_TIMING_OPCODE_SET_GET_FMC1_PULSES, chan, fmc1_pulses);
}
PARAM_FUNC_CLIENT_WRITE_READ(afc_timing_fmc1_pulses, chan, fmc1_pulses)
{
    return param_client_write_read (self, service, AFC_TIMING_OPCODE_SET_GET_FMC1_PULSES, chan, fmc1_pulses);
}

PARAM_FUNC_CLIENT_WRITE2(afc_timing_fmc1_count, chan, fmc1_count)
{
    return param_client_write2 (self, service, AFC_TIMING_OPCODE_SET_GET_FMC1_COUNT, chan, fmc1_count);
}
PARAM_FUNC_CLIENT_WRITE_READ(afc_timing_fmc1_count, chan, fmc1_count)
{
    return param_client_write_read (self, service, AFC_TIMING_OPCODE_SET_GET_FMC1_COUNT, chan, fmc1_count);
}

PARAM_FUNC_CLIENT_WRITE2(afc_timing_fmc1_evt, chan, fmc1_evt)
{
    return param_client_write2 (self, service, AFC_TIMING_OPCODE_SET_GET_FMC1_EVT, chan, fmc1_evt);
}
PARAM_FUNC_CLIENT_WRITE_READ(afc_timing_fmc1_evt, chan, fmc1_evt)
{
    return param_client_write_read (self, service, AFC_TIMING_OPCODE_SET_GET_FMC1_EVT, chan, fmc1_evt);
}

PARAM_FUNC_CLIENT_WRITE2(afc_timing_fmc1_dly, chan, fmc1_dly)
{
    return param_client_write2 (self, service, AFC_TIMING_OPCODE_SET_GET_FMC1_DLY, chan, fmc1_dly);
}
PARAM_FUNC_CLIENT_WRITE_READ(afc_timing_fmc1_dly, chan, fmc1_dly)
{
    return param_client_write_read (self, service, AFC_TIMING_OPCODE_SET_GET_FMC1_DLY, chan, fmc1_dly);
}

PARAM_FUNC_CLIENT_WRITE2(afc_timing_fmc1_wdt, chan, fmc1_wdt)
{
    return param_client_write2 (self, service, AFC_TIMING_OPCODE_SET_GET_FMC1_WDT, chan, fmc1_wdt);
}
PARAM_FUNC_CLIENT_WRITE_READ(afc_timing_fmc1_wdt, chan, fmc1_wdt)
{
    return param_client_write_read (self, service, AFC_TIMING_OPCODE_SET_GET_FMC1_WDT, chan, fmc1_wdt);
}

// FMC 2 channels
PARAM_FUNC_CLIENT_WRITE2(afc_timing_fmc2_en, chan, fmc2_en)
{
    return param_client_write2 (self, service, AFC_TIMING_OPCODE_SET_GET_FMC2_EN, chan, fmc2_en);
}
PARAM_FUNC_CLIENT_WRITE_READ(afc_timing_fmc2_en, chan, fmc2_en)
{
    return param_client_write_read (self, service, AFC_TIMING_OPCODE_SET_GET_FMC2_EN, chan, fmc2_en);
}

PARAM_FUNC_CLIENT_WRITE2(afc_timing_fmc2_pol, chan, fmc2_pol)
{
    return param_client_write2 (self, service, AFC_TIMING_OPCODE_SET_GET_FMC2_POL, chan, fmc2_pol);
}
PARAM_FUNC_CLIENT_WRITE_READ(afc_timing_fmc2_pol, chan, fmc2_pol)
{
    return param_client_write_read (self, service, AFC_TIMING_OPCODE_SET_GET_FMC2_POL, chan, fmc2_pol);
}

PARAM_FUNC_CLIENT_WRITE2(afc_timing_fmc2_log, chan, fmc2_log)
{
    return param_client_write2 (self, service, AFC_TIMING_OPCODE_SET_GET_FMC2_LOG, chan, fmc2_log);
}
PARAM_FUNC_CLIENT_WRITE_READ(afc_timing_fmc2_log, chan, fmc2_log)
{
    return param_client_write_read (self, service, AFC_TIMING_OPCODE_SET_GET_FMC2_LOG, chan, fmc2_log);
}

PARAM_FUNC_CLIENT_WRITE2(afc_timing_fmc2_itl, chan, fmc2_itl)
{
    return param_client_write2 (self, service, AFC_TIMING_OPCODE_SET_GET_FMC2_ITL, chan, fmc2_itl);
}
PARAM_FUNC_CLIENT_WRITE_READ(afc_timing_fmc2_itl, chan, fmc2_itl)
{
    return param_client_write_read (self, service, AFC_TIMING_OPCODE_SET_GET_FMC2_ITL, chan, fmc2_itl);
}

PARAM_FUNC_CLIENT_WRITE2(afc_timing_fmc2_src, chan, fmc2_src)
{
    return param_client_write2 (self, service, AFC_TIMING_OPCODE_SET_GET_FMC2_SRC, chan, fmc2_src);
}
PARAM_FUNC_CLIENT_WRITE_READ(afc_timing_fmc2_src, chan, fmc2_src)
{
    return param_client_write_read (self, service, AFC_TIMING_OPCODE_SET_GET_FMC2_SRC, chan, fmc2_src);
}

PARAM_FUNC_CLIENT_WRITE2(afc_timing_fmc2_dir, chan, fmc2_dir)
{
    return param_client_write2 (self, service, AFC_TIMING_OPCODE_SET_GET_FMC2_DIR, chan, fmc2_dir);
}
PARAM_FUNC_CLIENT_WRITE_READ(afc_timing_fmc2_dir, chan, fmc2_dir)
{
    return param_client_write_read (self, service, AFC_TIMING_OPCODE_SET_GET_FMC2_DIR, chan, fmc2_dir);
}

PARAM_FUNC_CLIENT_WRITE2(afc_timing_fmc2_count_rst, chan, fmc2_count_rst)
{
    return param_client_write2 (self, service, AFC_TIMING_OPCODE_SET_GET_FMC2_COUNT_RST, chan, fmc2_count_rst);
}
PARAM_FUNC_CLIENT_WRITE_READ(afc_timing_fmc2_count_rst, chan, fmc2_count_rst)
{
    return param_client_write_read (self, service, AFC_TIMING_OPCODE_SET_GET_FMC2_COUNT_RST, chan, fmc2_count_rst);
}

PARAM_FUNC_CLIENT_WRITE2(afc_timing_fmc2_pulses, chan, fmc2_pulses)
{
    return param_client_write2 (self, service, AFC_TIMING_OPCODE_SET_GET_FMC2_PULSES, chan, fmc2_pulses);
}
PARAM_FUNC_CLIENT_WRITE_READ(afc_timing_fmc2_pulses, chan, fmc2_pulses)
{
    return param_client_write_read (self, service, AFC_TIMING_OPCODE_SET_GET_FMC2_PULSES, chan, fmc2_pulses);
}

PARAM_FUNC_CLIENT_WRITE2(afc_timing_fmc2_count, chan, fmc2_count)
{
    return param_client_write2 (self, service, AFC_TIMING_OPCODE_SET_GET_FMC2_COUNT, chan, fmc2_count);
}
PARAM_FUNC_CLIENT_WRITE_READ(afc_timing_fmc2_count, chan, fmc2_count)
{
    return param_client_write_read (self, service, AFC_TIMING_OPCODE_SET_GET_FMC2_COUNT, chan, fmc2_count);
}

PARAM_FUNC_CLIENT_WRITE2(afc_timing_fmc2_evt, chan, fmc2_evt)
{
    return param_client_write2 (self, service, AFC_TIMING_OPCODE_SET_GET_FMC2_EVT, chan, fmc2_evt);
}
PARAM_FUNC_CLIENT_WRITE_READ(afc_timing_fmc2_evt, chan, fmc2_evt)
{
    return param_client_write_read (self, service, AFC_TIMING_OPCODE_SET_GET_FMC2_EVT, chan, fmc2_evt);
}

PARAM_FUNC_CLIENT_WRITE2(afc_timing_fmc2_dly, chan, fmc2_dly)
{
    return param_client_write2 (self, service, AFC_TIMING_OPCODE_SET_GET_FMC2_DLY, chan, fmc2_dly);
}
PARAM_FUNC_CLIENT_WRITE_READ(afc_timing_fmc2_dly, chan, fmc2_dly)
{
    return param_client_write_read (self, service, AFC_TIMING_OPCODE_SET_GET_FMC2_DLY, chan, fmc2_dly);
}

PARAM_FUNC_CLIENT_WRITE2(afc_timing_fmc2_wdt, chan, fmc2_wdt)
{
    return param_client_write2 (self, service, AFC_TIMING_OPCODE_SET_GET_FMC2_WDT, chan, fmc2_wdt);
}
PARAM_FUNC_CLIENT_WRITE_READ(afc_timing_fmc2_wdt, chan, fmc2_wdt)
{
    return param_client_write_read (self, service, AFC_TIMING_OPCODE_SET_GET_FMC2_WDT, chan, fmc2_wdt);
}

PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, rtm_freq_kp)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_RTM_FREQ_KP, rtm_freq_kp);
}
PARAM_FUNC_CLIENT_READ_MOD(afc_timing, rtm_freq_kp)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_RTM_FREQ_KP, rtm_freq_kp);
}

PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, rtm_freq_ki)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_RTM_FREQ_KI, rtm_freq_ki);
}
PARAM_FUNC_CLIENT_READ_MOD(afc_timing, rtm_freq_ki)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_RTM_FREQ_KI, rtm_freq_ki);
}

PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, rtm_phase_kp)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_RTM_PHASE_KP, rtm_phase_kp);
}
PARAM_FUNC_CLIENT_READ_MOD(afc_timing, rtm_phase_kp)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_RTM_PHASE_KP, rtm_phase_kp);
}

PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, rtm_phase_ki)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_RTM_PHASE_KI, rtm_phase_ki);
}
PARAM_FUNC_CLIENT_READ_MOD(afc_timing, rtm_phase_ki)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_RTM_PHASE_KI, rtm_phase_ki);
}

PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, rtm_phase_navg)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_RTM_PHASE_NAVG, rtm_phase_navg);
}
PARAM_FUNC_CLIENT_READ_MOD(afc_timing, rtm_phase_navg)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_RTM_PHASE_NAVG, rtm_phase_navg);
}

PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, rtm_phase_div_exp)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_RTM_PHASE_DIV_EXP, rtm_phase_div_exp);
}
PARAM_FUNC_CLIENT_READ_MOD(afc_timing, rtm_phase_div_exp)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_RTM_PHASE_DIV_EXP, rtm_phase_div_exp);
}

PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, rtm_rfreq_hi)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_RTM_RFREQ_HI, rtm_rfreq_hi);
}
PARAM_FUNC_CLIENT_READ_MOD(afc_timing, rtm_rfreq_hi)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_RTM_RFREQ_HI, rtm_rfreq_hi);
}

PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, rtm_rfreq_lo)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_RTM_RFREQ_LO, rtm_rfreq_lo);
}
PARAM_FUNC_CLIENT_READ_MOD(afc_timing, rtm_rfreq_lo)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_RTM_RFREQ_LO, rtm_rfreq_lo);
}

PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, rtm_n1)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_RTM_N1, rtm_n1);
}
PARAM_FUNC_CLIENT_READ_MOD(afc_timing, rtm_n1)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_RTM_N1, rtm_n1);
}

PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, rtm_hs_div)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_RTM_HS_DIV, rtm_hs_div);
}
PARAM_FUNC_CLIENT_READ_MOD(afc_timing, rtm_hs_div)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_RTM_HS_DIV, rtm_hs_div);
}

PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, afc_freq_kp)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_AFC_FREQ_KP, afc_freq_kp);
}
PARAM_FUNC_CLIENT_READ_MOD(afc_timing, afc_freq_kp)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_AFC_FREQ_KP, afc_freq_kp);
}

PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, afc_freq_ki)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_AFC_FREQ_KI, afc_freq_ki);
}
PARAM_FUNC_CLIENT_READ_MOD(afc_timing, afc_freq_ki)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_AFC_FREQ_KI, afc_freq_ki);
}

PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, afc_phase_kp)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_AFC_PHASE_KP, afc_phase_kp);
}
PARAM_FUNC_CLIENT_READ_MOD(afc_timing, afc_phase_kp)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_AFC_PHASE_KP, afc_phase_kp);
}

PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, afc_phase_ki)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_AFC_PHASE_KI, afc_phase_ki);
}
PARAM_FUNC_CLIENT_READ_MOD(afc_timing, afc_phase_ki)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_AFC_PHASE_KI, afc_phase_ki);
}

PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, afc_phase_navg)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_AFC_PHASE_NAVG, afc_phase_navg);
}
PARAM_FUNC_CLIENT_READ_MOD(afc_timing, afc_phase_navg)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_AFC_PHASE_NAVG, afc_phase_navg);
}

PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, afc_phase_div_exp)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_AFC_PHASE_DIV_EXP, afc_phase_div_exp);
}
PARAM_FUNC_CLIENT_READ_MOD(afc_timing, afc_phase_div_exp)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_AFC_PHASE_DIV_EXP, afc_phase_div_exp);
}

PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, afc_rfreq_hi)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_AFC_RFREQ_HI, afc_rfreq_hi);
}
PARAM_FUNC_CLIENT_READ_MOD(afc_timing, afc_rfreq_hi)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_AFC_RFREQ_HI, afc_rfreq_hi);
}

PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, afc_rfreq_lo)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_AFC_RFREQ_LO, afc_rfreq_lo);
}
PARAM_FUNC_CLIENT_READ_MOD(afc_timing, afc_rfreq_lo)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_AFC_RFREQ_LO, afc_rfreq_lo);
}

PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, afc_n1)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_AFC_N1, afc_n1);
}
PARAM_FUNC_CLIENT_READ_MOD(afc_timing, afc_n1)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_AFC_N1, afc_n1);
}

PARAM_FUNC_CLIENT_WRITE_MOD(afc_timing, afc_hs_div)
{
    return param_client_write (self, service, AFC_TIMING_OPCODE_SET_GET_AFC_HS_DIV, afc_hs_div);
}
PARAM_FUNC_CLIENT_READ_MOD(afc_timing, afc_hs_div)
{
    return param_client_read (self, service, AFC_TIMING_OPCODE_SET_GET_AFC_HS_DIV, afc_hs_div);
}

/**************** INIT SMIO Functions ****************/

/* Init function */
PARAM_FUNC_CLIENT_READ(init_check)
{
    return param_client_read (self, service, INIT_OPCODE_SET_GET_CHECK, init_check);
}

/**************** TIM RCV SMIO Functions ****************/

/* DMTD Number of averages function */
PARAM_FUNC_CLIENT_WRITE(phase_meas_navg)
{
    return param_client_write (self, service, TIM_RCV_OPCODE_PHASE_MEAS_NAVG, phase_meas_navg);
}

PARAM_FUNC_CLIENT_READ(phase_meas_navg)
{
    return param_client_read (self, service, TIM_RCV_OPCODE_PHASE_MEAS_NAVG, phase_meas_navg);
}
/* DMTD A deglitcher threshold function */
PARAM_FUNC_CLIENT_WRITE(dmtd_a_deglitcher_thres)
{
    return param_client_write (self, service, TIM_RCV_OPCODE_DMTD_A_DEGLITCHER_THRES, dmtd_a_deglitcher_thres);
}

PARAM_FUNC_CLIENT_READ(dmtd_a_deglitcher_thres)
{
    return param_client_read (self, service, TIM_RCV_OPCODE_DMTD_A_DEGLITCHER_THRES, dmtd_a_deglitcher_thres);
}

/* DMTD B deglitcher threshold function */
PARAM_FUNC_CLIENT_WRITE(dmtd_b_deglitcher_thres)
{
    return param_client_write (self, service, TIM_RCV_OPCODE_DMTD_B_DEGLITCHER_THRES, dmtd_b_deglitcher_thres);
}

PARAM_FUNC_CLIENT_READ(dmtd_b_deglitcher_thres)
{
    return param_client_read (self, service, TIM_RCV_OPCODE_DMTD_B_DEGLITCHER_THRES, dmtd_b_deglitcher_thres);
}

/* DMTD Phase measurement function */
PARAM_FUNC_CLIENT_WRITE(phase_meas)
{
    return param_client_write (self, service, TIM_RCV_OPCODE_PHASE_MEAS, phase_meas);
}

PARAM_FUNC_CLIENT_READ(phase_meas)
{
    return param_client_read (self, service, TIM_RCV_OPCODE_PHASE_MEAS, phase_meas);
}

/* DMTD A frequency measurement */
PARAM_FUNC_CLIENT_WRITE(dmtd_a_freq)
{
    return param_client_write (self, service, TIM_RCV_OPCODE_DMTD_A_FREQ, dmtd_a_freq);
}

PARAM_FUNC_CLIENT_READ(dmtd_a_freq)
{
    return param_client_read (self, service, TIM_RCV_OPCODE_DMTD_A_FREQ, dmtd_a_freq);
}

/* DMTD A frequency valid */
PARAM_FUNC_CLIENT_WRITE(dmtd_a_valid)
{
    return param_client_write (self, service, TIM_RCV_OPCODE_DMTD_A_VALID, dmtd_a_valid);
}

PARAM_FUNC_CLIENT_READ(dmtd_a_valid)
{
    return param_client_read (self, service, TIM_RCV_OPCODE_DMTD_A_VALID, dmtd_a_valid);
}

/* DMTD B frequency measurement */
PARAM_FUNC_CLIENT_WRITE(dmtd_b_freq)
{
    return param_client_write (self, service, TIM_RCV_OPCODE_DMTD_B_FREQ, dmtd_b_freq);
}

PARAM_FUNC_CLIENT_READ(dmtd_b_freq)
{
    return param_client_read (self, service, TIM_RCV_OPCODE_DMTD_B_FREQ, dmtd_b_freq);
}

/* DMTD B frequency valid */
PARAM_FUNC_CLIENT_WRITE(dmtd_b_valid)
{
    return param_client_write (self, service, TIM_RCV_OPCODE_DMTD_B_VALID, dmtd_b_valid);
}

PARAM_FUNC_CLIENT_READ(dmtd_b_valid)
{
    return param_client_read (self, service, TIM_RCV_OPCODE_DMTD_B_VALID, dmtd_b_valid);
}

/**************** ORBIT INTLK SMIO Functions ****************/

/* Orbit interlock enable */
PARAM_FUNC_CLIENT_WRITE(orbit_intlk_en)
{
    return param_client_write (self, service, ORBIT_INTLK_OPCODE_SET_GET_EN, orbit_intlk_en);
}

PARAM_FUNC_CLIENT_READ(orbit_intlk_en)
{
    return param_client_read (self, service, ORBIT_INTLK_OPCODE_SET_GET_EN, orbit_intlk_en);
}

/* Orbit interlock clear */
PARAM_FUNC_CLIENT_WRITE(orbit_intlk_clr)
{
    return param_client_write (self, service, ORBIT_INTLK_OPCODE_SET_GET_CLR, orbit_intlk_clr);
}

PARAM_FUNC_CLIENT_READ(orbit_intlk_clr)
{
    return param_client_read (self, service, ORBIT_INTLK_OPCODE_SET_GET_CLR, orbit_intlk_clr);
}

/* Orbit interlock minimum SUM to trigger interlock enable */
PARAM_FUNC_CLIENT_WRITE(orbit_intlk_min_sum_en)
{
    return param_client_write (self, service, ORBIT_INTLK_OPCODE_SET_GET_MIN_SUM_EN, orbit_intlk_min_sum_en);
}

PARAM_FUNC_CLIENT_READ(orbit_intlk_min_sum_en)
{
    return param_client_read (self, service, ORBIT_INTLK_OPCODE_SET_GET_MIN_SUM_EN, orbit_intlk_min_sum_en);
}

/* Orbit interlock translation enable */
PARAM_FUNC_CLIENT_WRITE(orbit_intlk_trans_en)
{
    return param_client_write (self, service, ORBIT_INTLK_OPCODE_SET_GET_TRANS_EN, orbit_intlk_trans_en);
}

PARAM_FUNC_CLIENT_READ(orbit_intlk_trans_en)
{
    return param_client_read (self, service, ORBIT_INTLK_OPCODE_SET_GET_TRANS_EN, orbit_intlk_trans_en);
}

/* Orbit interlock translation clear */
PARAM_FUNC_CLIENT_WRITE(orbit_intlk_trans_clr)
{
    return param_client_write (self, service, ORBIT_INTLK_OPCODE_SET_GET_TRANS_CLR, orbit_intlk_trans_clr);
}

PARAM_FUNC_CLIENT_READ(orbit_intlk_trans_clr)
{
    return param_client_read (self, service, ORBIT_INTLK_OPCODE_SET_GET_TRANS_CLR, orbit_intlk_trans_clr);
}

/* Orbit interlock angular enable */
PARAM_FUNC_CLIENT_WRITE(orbit_intlk_ang_en)
{
    return param_client_write (self, service, ORBIT_INTLK_OPCODE_SET_GET_ANG_EN, orbit_intlk_ang_en);
}

PARAM_FUNC_CLIENT_READ(orbit_intlk_ang_en)
{
    return param_client_read (self, service, ORBIT_INTLK_OPCODE_SET_GET_ANG_EN, orbit_intlk_ang_en);
}

/* Orbit interlock angular clear */
PARAM_FUNC_CLIENT_WRITE(orbit_intlk_ang_clr)
{
    return param_client_write (self, service, ORBIT_INTLK_OPCODE_SET_GET_ANG_CLR, orbit_intlk_ang_clr);
}

PARAM_FUNC_CLIENT_READ(orbit_intlk_ang_clr)
{
    return param_client_read (self, service, ORBIT_INTLK_OPCODE_SET_GET_ANG_CLR, orbit_intlk_ang_clr);
}

/* Orbit interlock translation trip X */
PARAM_FUNC_CLIENT_READ(orbit_intlk_trans_bigger_x)
{
    return param_client_read (self, service, ORBIT_INTLK_OPCODE_SET_GET_TRANS_BIGGER_X, orbit_intlk_trans_bigger_x);
}

/* Orbit interlock translation trip Y */
PARAM_FUNC_CLIENT_READ(orbit_intlk_trans_bigger_y)
{
    return param_client_read (self, service, ORBIT_INTLK_OPCODE_SET_GET_TRANS_BIGGER_Y, orbit_intlk_trans_bigger_y);
}

/* Orbit interlock translation trip X latched */
PARAM_FUNC_CLIENT_READ(orbit_intlk_trans_bigger_ltc_x)
{
    return param_client_read (self, service, ORBIT_INTLK_OPCODE_SET_GET_TRANS_BIGGER_LTC_X, orbit_intlk_trans_bigger_ltc_x);
}

/* Orbit interlock translation trip Y latched */
PARAM_FUNC_CLIENT_READ(orbit_intlk_trans_bigger_ltc_y)
{
    return param_client_read (self, service, ORBIT_INTLK_OPCODE_SET_GET_TRANS_BIGGER_LTC_Y, orbit_intlk_trans_bigger_ltc_y);
}

/* Orbit interlock translation any trip */
PARAM_FUNC_CLIENT_READ(orbit_intlk_trans_bigger_any)
{
    return param_client_read (self, service, ORBIT_INTLK_OPCODE_SET_GET_TRANS_BIGGER_ANY, orbit_intlk_trans_bigger_any);
}

/* Orbit interlock translation any trip masked with enabled */
PARAM_FUNC_CLIENT_READ(orbit_intlk_trans_bigger)
{
    return param_client_read (self, service, ORBIT_INTLK_OPCODE_SET_GET_TRANS_BIGGER, orbit_intlk_trans_bigger);
}

/* Orbit interlock translation any trip masked with enabled latched */
PARAM_FUNC_CLIENT_READ(orbit_intlk_trans_bigger_ltc)
{
    return param_client_read (self, service, ORBIT_INTLK_OPCODE_SET_GET_TRANS_BIGGER_LTC, orbit_intlk_trans_bigger_ltc);
}

/* Orbit interlock angular trip X */
PARAM_FUNC_CLIENT_READ(orbit_intlk_ang_bigger_x)
{
    return param_client_read (self, service, ORBIT_INTLK_OPCODE_SET_GET_ANG_BIGGER_X, orbit_intlk_ang_bigger_x);
}

/* Orbit interlock angular trip Y */
PARAM_FUNC_CLIENT_READ(orbit_intlk_ang_bigger_y)
{
    return param_client_read (self, service, ORBIT_INTLK_OPCODE_SET_GET_ANG_BIGGER_Y, orbit_intlk_ang_bigger_y);
}

/* Orbit interlock angular trip X latched */
PARAM_FUNC_CLIENT_READ(orbit_intlk_ang_bigger_ltc_x)
{
    return param_client_read (self, service, ORBIT_INTLK_OPCODE_SET_GET_ANG_BIGGER_LTC_X, orbit_intlk_ang_bigger_ltc_x);
}

/* Orbit interlock angular trip Y latched */
PARAM_FUNC_CLIENT_READ(orbit_intlk_ang_bigger_ltc_y)
{
    return param_client_read (self, service, ORBIT_INTLK_OPCODE_SET_GET_ANG_BIGGER_LTC_Y, orbit_intlk_ang_bigger_ltc_y);
}

/* Orbit interlock angular any trip */
PARAM_FUNC_CLIENT_READ(orbit_intlk_ang_bigger_any)
{
    return param_client_read (self, service, ORBIT_INTLK_OPCODE_SET_GET_ANG_BIGGER_ANY, orbit_intlk_ang_bigger_any);
}

/* Orbit interlock angular any trip masked with enabled */
PARAM_FUNC_CLIENT_READ(orbit_intlk_ang_bigger)
{
    return param_client_read (self, service, ORBIT_INTLK_OPCODE_SET_GET_ANG_BIGGER, orbit_intlk_ang_bigger);
}

/* Orbit interlock angular any trip masked with enabled latched */
PARAM_FUNC_CLIENT_READ(orbit_intlk_ang_bigger_ltc)
{
    return param_client_read (self, service, ORBIT_INTLK_OPCODE_SET_GET_ANG_BIGGER_LTC, orbit_intlk_ang_bigger_ltc);
}

/* Orbit interlock trip */
PARAM_FUNC_CLIENT_READ(orbit_intlk)
{
    return param_client_read (self, service, ORBIT_INTLK_OPCODE_SET_GET_INTLK, orbit_intlk);
}

/* Orbit interlock trip latched */
PARAM_FUNC_CLIENT_READ(orbit_intlk_ltc)
{
    return param_client_read (self, service, ORBIT_INTLK_OPCODE_SET_GET_INTLK_LTC, orbit_intlk_ltc);
}

/* Orbit interlock minium sum */
PARAM_FUNC_CLIENT_WRITE(orbit_intlk_min_sum)
{
    return param_client_write (self, service, ORBIT_INTLK_OPCODE_SET_GET_MIN_SUM, orbit_intlk_min_sum);
}

PARAM_FUNC_CLIENT_READ(orbit_intlk_min_sum)
{
    return param_client_read (self, service, ORBIT_INTLK_OPCODE_SET_GET_MIN_SUM, orbit_intlk_min_sum);
}

/* Orbit interlock translation max X */
PARAM_FUNC_CLIENT_WRITE_SIGNED(orbit_intlk_trans_max_x)
{
    return param_client_write_signed (self, service, ORBIT_INTLK_OPCODE_SET_GET_TRANS_MAX_X, orbit_intlk_trans_max_x);
}

PARAM_FUNC_CLIENT_READ_SIGNED(orbit_intlk_trans_max_x)
{
    return param_client_read_signed (self, service, ORBIT_INTLK_OPCODE_SET_GET_TRANS_MAX_X, orbit_intlk_trans_max_x);
}

/* Orbit interlock translation max Y */
PARAM_FUNC_CLIENT_WRITE_SIGNED(orbit_intlk_trans_max_y)
{
    return param_client_write_signed (self, service, ORBIT_INTLK_OPCODE_SET_GET_TRANS_MAX_Y, orbit_intlk_trans_max_y);
}

PARAM_FUNC_CLIENT_READ_SIGNED(orbit_intlk_trans_max_y)
{
    return param_client_read_signed (self, service, ORBIT_INTLK_OPCODE_SET_GET_TRANS_MAX_Y, orbit_intlk_trans_max_y);
}

/* Orbit interlock angular max X */
PARAM_FUNC_CLIENT_WRITE_SIGNED(orbit_intlk_ang_max_x)
{
    return param_client_write_signed (self, service, ORBIT_INTLK_OPCODE_SET_GET_ANG_MAX_X, orbit_intlk_ang_max_x);
}

PARAM_FUNC_CLIENT_READ_SIGNED(orbit_intlk_ang_max_x)
{
    return param_client_read_signed (self, service, ORBIT_INTLK_OPCODE_SET_GET_ANG_MAX_X, orbit_intlk_ang_max_x);
}

/* Orbit interlock angular max Y */
PARAM_FUNC_CLIENT_WRITE_SIGNED(orbit_intlk_ang_max_y)
{
    return param_client_write_signed (self, service, ORBIT_INTLK_OPCODE_SET_GET_ANG_MAX_Y, orbit_intlk_ang_max_y);
}

PARAM_FUNC_CLIENT_READ_SIGNED(orbit_intlk_ang_max_y)
{
    return param_client_read_signed (self, service, ORBIT_INTLK_OPCODE_SET_GET_ANG_MAX_Y, orbit_intlk_ang_max_y);
}

/* Orbit interlock translation trip X */
PARAM_FUNC_CLIENT_READ(orbit_intlk_trans_smaller_x)
{
    return param_client_read (self, service, ORBIT_INTLK_OPCODE_SET_GET_TRANS_SMALLER_X, orbit_intlk_trans_smaller_x);
}

/* Orbit interlock translation trip Y */
PARAM_FUNC_CLIENT_READ(orbit_intlk_trans_smaller_y)
{
    return param_client_read (self, service, ORBIT_INTLK_OPCODE_SET_GET_TRANS_SMALLER_Y, orbit_intlk_trans_smaller_y);
}

/* Orbit interlock translation trip X latched */
PARAM_FUNC_CLIENT_READ(orbit_intlk_trans_smaller_ltc_x)
{
    return param_client_read (self, service, ORBIT_INTLK_OPCODE_SET_GET_TRANS_SMALLER_LTC_X, orbit_intlk_trans_smaller_ltc_x);
}

/* Orbit interlock translation trip Y latched */
PARAM_FUNC_CLIENT_READ(orbit_intlk_trans_smaller_ltc_y)
{
    return param_client_read (self, service, ORBIT_INTLK_OPCODE_SET_GET_TRANS_SMALLER_LTC_Y, orbit_intlk_trans_smaller_ltc_y);
}

/* Orbit interlock translation any trip */
PARAM_FUNC_CLIENT_READ(orbit_intlk_trans_smaller_any)
{
    return param_client_read (self, service, ORBIT_INTLK_OPCODE_SET_GET_TRANS_SMALLER_ANY, orbit_intlk_trans_smaller_any);
}

/* Orbit interlock translation any trip masked with enabled */
PARAM_FUNC_CLIENT_READ(orbit_intlk_trans_smaller)
{
    return param_client_read (self, service, ORBIT_INTLK_OPCODE_SET_GET_TRANS_SMALLER, orbit_intlk_trans_smaller);
}

/* Orbit interlock translation any trip masked with enabled latched */
PARAM_FUNC_CLIENT_READ(orbit_intlk_trans_smaller_ltc)
{
    return param_client_read (self, service, ORBIT_INTLK_OPCODE_SET_GET_TRANS_SMALLER_LTC, orbit_intlk_trans_smaller_ltc);
}

/* Orbit interlock angular trip X */
PARAM_FUNC_CLIENT_READ(orbit_intlk_ang_smaller_x)
{
    return param_client_read (self, service, ORBIT_INTLK_OPCODE_SET_GET_ANG_SMALLER_X, orbit_intlk_ang_smaller_x);
}

/* Orbit interlock angular trip Y */
PARAM_FUNC_CLIENT_READ(orbit_intlk_ang_smaller_y)
{
    return param_client_read (self, service, ORBIT_INTLK_OPCODE_SET_GET_ANG_SMALLER_Y, orbit_intlk_ang_smaller_y);
}

/* Orbit interlock angular trip X latched */
PARAM_FUNC_CLIENT_READ(orbit_intlk_ang_smaller_ltc_x)
{
    return param_client_read (self, service, ORBIT_INTLK_OPCODE_SET_GET_ANG_SMALLER_LTC_X, orbit_intlk_ang_smaller_ltc_x);
}

/* Orbit interlock angular trip Y latched */
PARAM_FUNC_CLIENT_READ(orbit_intlk_ang_smaller_ltc_y)
{
    return param_client_read (self, service, ORBIT_INTLK_OPCODE_SET_GET_ANG_SMALLER_LTC_Y, orbit_intlk_ang_smaller_ltc_y);
}

/* Orbit interlock angular any trip */
PARAM_FUNC_CLIENT_READ(orbit_intlk_ang_smaller_any)
{
    return param_client_read (self, service, ORBIT_INTLK_OPCODE_SET_GET_ANG_SMALLER_ANY, orbit_intlk_ang_smaller_any);
}

/* Orbit interlock angular any trip masked with enabled */
PARAM_FUNC_CLIENT_READ(orbit_intlk_ang_smaller)
{
    return param_client_read (self, service, ORBIT_INTLK_OPCODE_SET_GET_ANG_SMALLER, orbit_intlk_ang_smaller);
}

/* Orbit interlock angular any trip masked with enabled latched */
PARAM_FUNC_CLIENT_READ(orbit_intlk_ang_smaller_ltc)
{
    return param_client_read (self, service, ORBIT_INTLK_OPCODE_SET_GET_ANG_SMALLER_LTC, orbit_intlk_ang_smaller_ltc);
}

/* Orbit interlock translation min X */
PARAM_FUNC_CLIENT_WRITE_SIGNED(orbit_intlk_trans_min_x)
{
    return param_client_write_signed (self, service, ORBIT_INTLK_OPCODE_SET_GET_TRANS_MIN_X, orbit_intlk_trans_min_x);
}

PARAM_FUNC_CLIENT_READ_SIGNED(orbit_intlk_trans_min_x)
{
    return param_client_read_signed (self, service, ORBIT_INTLK_OPCODE_SET_GET_TRANS_MIN_X, orbit_intlk_trans_min_x);
}

/* Orbit interlock translation min Y */
PARAM_FUNC_CLIENT_WRITE_SIGNED(orbit_intlk_trans_min_y)
{
    return param_client_write_signed (self, service, ORBIT_INTLK_OPCODE_SET_GET_TRANS_MIN_Y, orbit_intlk_trans_min_y);
}

PARAM_FUNC_CLIENT_READ_SIGNED(orbit_intlk_trans_min_y)
{
    return param_client_read_signed (self, service, ORBIT_INTLK_OPCODE_SET_GET_TRANS_MIN_Y, orbit_intlk_trans_min_y);
}

/* Orbit interlock angular min X */
PARAM_FUNC_CLIENT_WRITE_SIGNED(orbit_intlk_ang_min_x)
{
    return param_client_write_signed (self, service, ORBIT_INTLK_OPCODE_SET_GET_ANG_MIN_X, orbit_intlk_ang_min_x);
}

PARAM_FUNC_CLIENT_READ_SIGNED(orbit_intlk_ang_min_x)
{
    return param_client_read_signed (self, service, ORBIT_INTLK_OPCODE_SET_GET_ANG_MIN_X, orbit_intlk_ang_min_x);
}

/* Orbit interlock angular min Y */
PARAM_FUNC_CLIENT_WRITE_SIGNED(orbit_intlk_ang_min_y)
{
    return param_client_write_signed (self, service, ORBIT_INTLK_OPCODE_SET_GET_ANG_MIN_Y, orbit_intlk_ang_min_y);
}

PARAM_FUNC_CLIENT_READ_SIGNED(orbit_intlk_ang_min_y)
{
    return param_client_read_signed (self, service, ORBIT_INTLK_OPCODE_SET_GET_ANG_MIN_Y, orbit_intlk_ang_min_y);
}
