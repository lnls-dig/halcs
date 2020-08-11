/*
 * Copyright (C) 2020 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "halcs_server.h"
/* Private headers */
#include "sm_io_fmc100m_4ch_codes.h"
#include "sm_io_fmc100m_4ch_defaults.h"
#include "sm_io_fmc100m_4ch_exports.h"
#include "sm_io_fmc100m_4ch_core.h"
#include "sm_io_fmc100m_4ch_exp.h"
#include "hw/wb_fmc100m_4ch_regs.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...)  \
    ASSERT_HAL_TEST(test_boolean, SM_IO, "[sm_io:fmc100m_4ch_exp]",             \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)                   \
    ASSERT_HAL_ALLOC(ptr, SM_IO, "[sm_io:fmc100m_4ch_exp]",                     \
            smio_err_str(SMIO_ERR_ALLOC),                                       \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                                \
    CHECK_HAL_ERR(err, SM_IO, "[sm_io:fmc100m_4ch_exp]",                        \
            smio_err_str (err_type))

/************************************************************/
/*********** Specific FMC_100M_4CH Operations *************/
/************************************************************/

#define HALCS_FMC_100M_4CH_SI571_OE_MIN             0 /* SI571 Output disable */
#define HALCS_FMC_100M_4CH_SI571_OE_MAX             1 /* SI571 Output enable */

RW_PARAM_FUNC(fmc100m_4ch, si571_oe) {
    SET_GET_PARAM(fmc100m_4ch, 0x0, FMC_ADC_100MS_CSR,
            CTL, FMC_CLK_OE, SINGLE_BIT_PARAM,
            HALCS_FMC_100M_4CH_SI571_OE_MIN, HALCS_FMC_100M_4CH_SI571_OE_MAX, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

#define HALCS_FMC_100M_4CH_ACQ_LED_MIN             0 /* ACQ_LED disabled */
#define HALCS_FMC_100M_4CH_ACQ_LED_MAX             1 /* ACQ_LED enabled */

RW_PARAM_FUNC(fmc100m_4ch, acq_led) {
    SET_GET_PARAM(fmc100m_4ch, 0x0, FMC_ADC_100MS_CSR, CTL, ACQ_LED,
            SINGLE_BIT_PARAM, HALCS_FMC_100M_4CH_ACQ_LED_MIN, HALCS_FMC_100M_4CH_ACQ_LED_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FMC_ADC_100MS_CSR_STA_MASK     ((1ULL<<32)-1)
#define FMC_ADC_100MS_CSR_STA_W(val)   (val)
#define FMC_ADC_100MS_CSR_STA_R(val)   (val)

RW_PARAM_FUNC(fmc100m_4ch, status) {
    SET_GET_PARAM(fmc100m_4ch, 0x0, FMC_ADC_100MS_CSR, STA, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

/* This must match the FPGA maximum number of channels */
#define FMC_ADC_100MS_NUM_CHAN                          4
#define FMC_ADC_100MS_CHAN_OFFSET                       0x040 /* 6 32-bit registers + padding (10 words * 4 bytes) */

/*****************************************************************/
/********************* ADC Channel operations ********************/
/*****************************************************************/

#define FMC_ADC_100MS_CSR_CH1_STA_MASK     ((1ULL<<32)-1)
#define FMC_ADC_100MS_CSR_CH1_STA_W(val)   (val)
#define FMC_ADC_100MS_CSR_CH1_STA_R(val)   (val)

RW_PARAM_FUNC(fmc100m_4ch, adc_status) {
    SET_GET_PARAM_CHANNEL(fmc100m_4ch, 0x0, FMC_ADC_100MS_CSR,
            CH1_STA, /* No field */, FMC_ADC_100MS_CHAN_OFFSET, FMC_ADC_100MS_NUM_CHAN, 
            MULT_BIT_PARAM, /* No minimum check */, /* No maximum check */, 
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FMC_ADC_100MS_CSR_CH1_CALIB_GAIN_W(val)   WBGEN2_GEN_WRITE(val, 0, 16)
#define FMC_ADC_100MS_CSR_CH1_CALIB_GAIN_R(val)   WBGEN2_GEN_READ(val, 0, 16)

RW_PARAM_FUNC(fmc100m_4ch, adc_calib_gain) {
    SET_GET_PARAM_CHANNEL(fmc100m_4ch, 0x0, FMC_ADC_100MS_CSR, 
            CH1_CALIB, GAIN, FMC_ADC_100MS_CHAN_OFFSET, FMC_ADC_100MS_NUM_CHAN, 
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FMC_ADC_100MS_CSR_CH1_CALIB_OFFSET_W(val)   WBGEN2_GEN_WRITE(val, 16, 16)
#define FMC_ADC_100MS_CSR_CH1_CALIB_OFFSET_R(val)   WBGEN2_GEN_READ(val, 16, 16)

RW_PARAM_FUNC(fmc100m_4ch, adc_calib_offset) {
    SET_GET_PARAM_CHANNEL(fmc100m_4ch, 0x0, FMC_ADC_100MS_CSR, 
            CH1_CALIB, OFFSET, FMC_ADC_100MS_CHAN_OFFSET, FMC_ADC_100MS_NUM_CHAN, 
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FMC_ADC_100MS_CSR_CH1_SAT_VAL_W(val)   WBGEN2_GEN_WRITE(val, 0, 16)
#define FMC_ADC_100MS_CSR_CH1_SAT_VAL_R(val)   WBGEN2_GEN_READ(val, 0, 16) 

RW_PARAM_FUNC(fmc100m_4ch, adc_sat) {
    SET_GET_PARAM_CHANNEL(fmc100m_4ch, 0x0, FMC_ADC_100MS_CSR,
            CH1_SAT, VAL, FMC_ADC_100MS_CHAN_OFFSET, FMC_ADC_100MS_NUM_CHAN, 
            MULT_BIT_PARAM, /* No minimum check */, /* No maximum check */, 
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FMC_ADC_100MS_CSR_CH1_CTL_SSR_W(val)   WBGEN2_GEN_WRITE(val, 0, 7)
#define FMC_ADC_100MS_CSR_CH1_CTL_SSR_R(val)   WBGEN2_GEN_READ(val, 0, 7) 

RW_PARAM_FUNC(fmc100m_4ch, adc_ssr) {
    SET_GET_PARAM_CHANNEL(fmc100m_4ch, 0x0, FMC_ADC_100MS_CSR,
            CH1_CTL, SSR, FMC_ADC_100MS_CHAN_OFFSET, FMC_ADC_100MS_NUM_CHAN, 
            MULT_BIT_PARAM, /* No minimum check */, /* No maximum check */, 
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

/*****************************************************************/
/************************* General operations ********************/
/*****************************************************************/

#define FMC_ADC_100MS_CSR_FS_FREQ_MASK     ((1ULL<<32)-1)
#define FMC_ADC_100MS_CSR_FS_FREQ_W(val)   (val)
#define FMC_ADC_100MS_CSR_FS_FREQ_R(val)   (val)

RW_PARAM_FUNC(fmc100m_4ch, fs_freq) {
    SET_GET_PARAM(fmc100m_4ch, 0x0, FMC_ADC_100MS_CSR, FS_FREQ, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

/* Macros to avoid repetition of the function body Si57X */
typedef smch_err_e (*smch_si57x_func_fp) (smch_si57x_t *self, double *param);

#define FMC_100M_4CH_SI571_FUNC_NAME(func_name)                               \
    _fmc100m_4ch_si571_ ## func_name

#define FMC_100M_4CH_SI571_FUNC_NAME_HEADER(func_name)                        \
    static int FMC_100M_4CH_SI571_FUNC_NAME(func_name) (void *owner, void *args, void *ret)

#define FMC_100M_4CH_SI571_FUNC_BODY(owner, args, ret, read_func, write_func, \
        error_msg)                                                              \
    do {                                                                        \
        UNUSED(ret);                                                             \
        assert (owner);                                                         \
        assert (args);                                                          \
                                                                                \
        int err = -FMC_100M_4CH_OK;                                           \
        SMIO_OWNER_TYPE *self = SMIO_EXP_OWNER(owner);                          \
        smio_fmc100m_4ch_t *fmc100m4ch = smio_get_handler (self);               \
        ASSERT_TEST(fmc100m4ch != NULL, "Could not get SMIO FMC 100M 4CH handler", \
                err_get_fmc100m4ch_handler, -FMC_100M_4CH_ERR);                  \
        smch_si57x_t *smch_si57x = SMIO_SI57X_HANDLER(fmc100m4ch);                 \
        uint32_t rw = *(uint32_t *) EXP_MSG_ZMQ_FIRST_ARG(args);                \
        double param = *(double *) EXP_MSG_ZMQ_NEXT_ARG(args);                  \
                                                                                \
        smch_err_e serr = SMCH_SUCCESS;                                         \
        /* Call specific function */                                            \
        if (rw) {                                                               \
            WHEN(ISEMPTY(read_func))(                                           \
                UNUSED(ret);                                                     \
                DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fmc100m_4ch_exp] " \
                        "SI57x read function not implemented\n");               \
                err = -FMC_100M_4CH_UNINPL;                                   \
                return err;                                                     \
            )                                                                   \
            WHENNOT(ISEMPTY(read_func))(                                        \
                double value = 0;                                               \
                serr = ((smch_si57x_func_fp) read_func) (smch_si57x,            \
                        &value);                                                \
                if (serr != SMCH_SUCCESS) {                                     \
                    err = -FMC_100M_4CH_ERR;                                  \
                }                                                               \
                else {                                                          \
                    *((double *) ret) = value;                                  \
                    err = sizeof (value);                                       \
                    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fmc100m_4ch_exp] " \
                            "SI57x function read value = 0x%f\n", value);       \
                }                                                               \
            )                                                                   \
        }                                                                       \
        else {                                                                  \
            WHEN(ISEMPTY(write_func))(                                          \
                DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fmc100m_4ch_exp] " \
                        "SI57x write function not implemented\n");              \
                err = -FMC_100M_4CH_UNINPL;                                   \
                return err;                                                     \
            )                                                                   \
            WHENNOT(ISEMPTY(write_func))(                                       \
                serr = ((smch_si57x_func_fp) write_func) (smch_si57x,           \
                        &param);                                                \
                if (serr != SMCH_SUCCESS) {                                     \
                    err = -FMC_100M_4CH_ERR;                                  \
                }                                                               \
                else {                                                          \
                    err = -FMC_100M_4CH_OK;                                   \
                }                                                               \
            )                                                                   \
        }                                                                       \
                                                                                \
err_get_fmc100m4ch_handler:                                                        \
        return err;                                                             \
                                                                                \
    } while(0)

FMC_100M_4CH_SI571_FUNC_NAME_HEADER(freq)
{
    FMC_100M_4CH_SI571_FUNC_BODY(owner, args, ret, smch_si57x_get_freq,
            smch_si57x_set_freq, "Could not set SI571 frequency");
}

static smch_err_e smch_si57x_get_defaults_compat (smch_si57x_t *self, double *param)
{
    return smch_si57x_get_defaults (self, *param);
}

FMC_100M_4CH_SI571_FUNC_NAME_HEADER(get_defaults)
{
    FMC_100M_4CH_SI571_FUNC_BODY(owner, args, ret, /* No read func*/,
            smch_si57x_get_defaults_compat, "Could not restart SI571 to its defaults");
}

static smch_err_e smch_si57x_get_fstartup_compat (smch_si57x_t *self, double *param)
{
    smch_err_e err = SMCH_SUCCESS;
    double fstartup = smch_si57x_get_fstartup (self);
    *param = fstartup;
    return err;
}

static smch_err_e smch_si57x_set_fstartup_compat (smch_si57x_t *self, double *param)
{
    return smch_si57x_set_fstartup (self, *param);
}

FMC_100M_4CH_SI571_FUNC_NAME_HEADER(fstartup)
{
    FMC_100M_4CH_SI571_FUNC_BODY(owner, args, ret, smch_si57x_get_fstartup_compat,
            smch_si57x_set_fstartup_compat, "Could not set/get SI571 startup frequency");
}

/******************************** Chips Export functions *************************/

/* Macros to avoid repetition of the function body LTC2174 */
typedef smch_err_e (*smch_ltc2174_func_fp) (smch_ltc2174_t *self, uint32_t *param);

#define FMC_100M_4CH_LTC2174_FUNC_NAME(func_name)                                 \
    _fmc100m_4ch_ltc2174_ ## func_name

#define FMC_100M_4CH_LTC2174_FUNC_NAME_HEADER(func_name)                          \
    static int FMC_100M_4CH_LTC2174_FUNC_NAME(func_name) (void *owner, void *args, void *ret)

#define FMC_100M_4CH_LTC2174_FUNC_BODY(owner, args, ret, read_func, write_func,   \
            error_msg)                                                            \
    do {                                                                          \
        assert (owner);                                                           \
        assert (args);                                                            \
                                                                                  \
        int err = -FMC_100M_4CH_OK;                                               \
        SMIO_OWNER_TYPE *self = SMIO_EXP_OWNER(owner);                            \
        smio_fmc100m_4ch_t *fmc100m4ch = smio_get_handler (self);                 \
        ASSERT_TEST(fmc100m4ch != NULL, "Could not get SMIO FMC 100M 4CH handler", \
                err_get_fmc100m4ch_handler, -FMC_100M_4CH_ERR);                   \
        smch_ltc2174_t *smch_ltc2174 = SMIO_LTC2174_HANDLER(fmc100m4ch);          \
        uint32_t rw = *(uint32_t *) EXP_MSG_ZMQ_FIRST_ARG(args);                  \
        uint32_t param = *(uint32_t *) EXP_MSG_ZMQ_NEXT_ARG(args);                \
                                                                                  \
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fmc100m_4ch_exp] Calling "  \
                "LTC2174 function\n");                                            \
                                                                                  \
        smch_err_e serr = SMCH_SUCCESS;                                           \
        /* Call specific function */                                              \
        if (rw) {                                                                 \
            WHEN(ISEMPTY(read_func))(                                             \
                UNUSED(ret);                                                      \
                DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fmc100m_4ch_exp] "  \
                        "LTC2174 read function not implemented\n");               \
                err = -FMC_100M_4CH_UNINPL;                                       \
                return err;                                                       \
            )                                                                     \
            WHENNOT(ISEMPTY(read_func))(                                          \
                uint32_t value = 0;                                               \
                serr = ((smch_ltc2174_func_fp) read_func) (smch_ltc2174,          \
                        &value);                                                  \
                if (serr != SMCH_SUCCESS) {                                       \
                    err = -FMC_100M_4CH_ERR;                                      \
                }                                                                 \
                else {                                                            \
                    *((uint32_t *) ret) = value;                                  \
                    err = sizeof (value);                                         \
                    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fmc100m_4ch_exp] " \
                            "LTC2174 function read value = 0x%08X\n", value);     \
                }                                                                 \
            )                                                                     \
        }                                                                         \
        else {                                                                    \
            WHEN(ISEMPTY(write_func))(                                            \
                DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fmc100m_4ch_exp] "  \
                        "LTC2174 write function not implemented\n");              \
                err = -FMC_100M_4CH_UNINPL;                                       \
                return err;                                                       \
            )                                                                     \
            WHENNOT(ISEMPTY(write_func))(                                         \
                serr = ((smch_ltc2174_func_fp) write_func) (smch_ltc2174,         \
                        &param);                                                  \
                if (serr != SMCH_SUCCESS) {                                       \
                    err = -FMC_100M_4CH_ERR;                                      \
                }                                                                 \
                else {                                                            \
                    err = -FMC_100M_4CH_OK;                                       \
                }                                                                 \
            )                                                                     \
        }                                                                         \
                                                                                  \
err_get_fmc100m4ch_handler:                                                       \
        return err;                                                               \
    } while(0)

FMC_100M_4CH_LTC2174_FUNC_NAME_HEADER(test_patt)
{
    FMC_100M_4CH_LTC2174_FUNC_BODY(owner, args, ret, smch_ltc2174_get_test_patt,
            smch_ltc2174_set_test_patt, "Could not set/get LTC2174 test pattern");
}

FMC_100M_4CH_LTC2174_FUNC_NAME_HEADER(test_patt_data)
{
    FMC_100M_4CH_LTC2174_FUNC_BODY(owner, args, ret, smch_ltc2174_get_test_patt_data,
            smch_ltc2174_set_test_patt_data, "Could not set/get LTC2174 test pattern data");
}

static smch_err_e smch_ltc2174_reset_compat (smch_ltc2174_t *self, uint32_t *param)
{
    UNUSED(param);
    return smch_ltc2174_reset (self);
}

FMC_100M_4CH_LTC2174_FUNC_NAME_HEADER(reset)
{
    FMC_100M_4CH_LTC2174_FUNC_BODY(owner, args, ret, /* No read function */,
            smch_ltc2174_reset_compat, "Could not set/get LTC2174 defaults");
}

FMC_100M_4CH_LTC2174_FUNC_NAME_HEADER(twos_compl)
{
    FMC_100M_4CH_LTC2174_FUNC_BODY(owner, args, ret, smch_ltc2174_get_twos_compl,
            smch_ltc2174_set_twos_compl, "Could not set/get LTC2174 twos complement data");
}

/* Exported function pointers */
const disp_table_func_fp fmc100m_4ch_exp_fp [] = {
    RW_PARAM_FUNC_NAME(fmc100m_4ch, si571_oe),
    RW_PARAM_FUNC_NAME(fmc100m_4ch, acq_led),
    RW_PARAM_FUNC_NAME(fmc100m_4ch, status),
    RW_PARAM_FUNC_NAME(fmc100m_4ch, adc_status),
    RW_PARAM_FUNC_NAME(fmc100m_4ch, adc_calib_gain),
    RW_PARAM_FUNC_NAME(fmc100m_4ch, adc_calib_offset),
    RW_PARAM_FUNC_NAME(fmc100m_4ch, adc_sat),
    RW_PARAM_FUNC_NAME(fmc100m_4ch, adc_ssr),
    RW_PARAM_FUNC_NAME(fmc100m_4ch, fs_freq),
    FMC_100M_4CH_SI571_FUNC_NAME(freq),
    FMC_100M_4CH_SI571_FUNC_NAME(get_defaults),
    FMC_100M_4CH_SI571_FUNC_NAME(fstartup),
    FMC_100M_4CH_LTC2174_FUNC_NAME(test_patt),
    FMC_100M_4CH_LTC2174_FUNC_NAME(test_patt_data),
    FMC_100M_4CH_LTC2174_FUNC_NAME(reset),
    FMC_100M_4CH_LTC2174_FUNC_NAME(twos_compl),
    NULL
};

/************************************************************/
/***************** Export methods functions *****************/
/************************************************************/

static smio_err_e _fmc100m_4ch_do_op (void *owner, void *msg);

/* Attach an instance of sm_io to dev_io function pointer */
smio_err_e fmc100m_4ch_attach (smio_t *self, void *args)
{
    UNUSED(self);
    UNUSED(args);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Deattach an instance of sm_io to dev_io function pointer */
smio_err_e fmc100m_4ch_deattach (smio_t *self)
{
    UNUSED(self);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Export (register) sm_io to handle operations function pointer */
smio_err_e fmc100m_4ch_export_ops (smio_t *self,
        const disp_op_t** smio_exp_ops)
{
    UNUSED(self);
    UNUSED(smio_exp_ops);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Unexport (unregister) sm_io to handle operations function pointer */
smio_err_e fmc100m_4ch_unexport_ops (smio_t *self)
{
    UNUSED(self);
    return SMIO_ERR_FUNC_NOT_IMPL;
}


/* Generic wrapper for receiving opcodes and arguments to specific funtions function pointer */
/* FIXME: Code repetition! _devio_do_smio_op () function does almost the same!!! */
smio_err_e _fmc100m_4ch_do_op (void *owner, void *msg)
{
    UNUSED(owner);
    UNUSED(msg);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

smio_err_e fmc100m_4ch_do_op (void *self, void *msg)
{
    return _fmc100m_4ch_do_op (self, msg);
}

const smio_ops_t fmc100m_4ch_ops = {
    .attach             = &fmc100m_4ch_attach,          /* Attach sm_io instance to dev_io */
    .deattach           = &fmc100m_4ch_deattach,        /* Deattach sm_io instance to dev_io */
    .export_ops         = &fmc100m_4ch_export_ops,      /* Export sm_io operations to dev_io */
    .unexport_ops       = &fmc100m_4ch_unexport_ops,    /* Unexport sm_io operations to dev_io */
    .do_op              = &fmc100m_4ch_do_op            /* Generic wrapper for handling specific operations */
};

/************************************************************/
/****************** Bootstrap Operations ********************/
/************************************************************/

smio_err_e fmc100m_4ch_init (smio_t * self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fmc100m_4ch_exp] Initializing fmc100m_4ch\n");

    smio_err_e err = SMIO_SUCCESS;

    err = smio_set_id (self, FMC_100M_4CH_SDB_DEVID);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO id", err_set_id);
    err = smio_set_name (self, FMC_100M_4CH_SDB_NAME);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO name", err_set_name);

    /* Set SMIO ops pointers */
    err = smio_set_ops (self, &fmc100m_4ch_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO operations",
            err_smio_set_ops);
    err = smio_set_thsafe_client_ops (self, &smio_thsafe_client_zmq_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO thsafe operations",
            err_smio_set_thsafe_ops);

    /* Fill the disp_op_t description structure with the callbacks. */

    /* disp_op_t structure is const and all of the functions performing on it
     * obviously receives a const argument, but here (and only on the SMIO
     * initialization) we need to make an exception if we want to keep the
     * functions' description and the function pointers separate */
    err = smio_init_exp_ops (self, (disp_op_t **) fmc100m_4ch_exp_ops,
            fmc100m_4ch_exp_fp);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not fill SMIO "
            "function descriptors with the callbacks", err_fill_desc);

    err = smio_set_exp_ops (self, fmc100m_4ch_exp_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO exported operations",
            err_smio_set_exp_ops);

    /* Initialize specific structure */
    smio_fmc100m_4ch_t *smio_handler = smio_fmc100m_4ch_new (self);
    ASSERT_ALLOC(smio_handler, err_smio_handler_alloc, SMIO_ERR_ALLOC);
    err = smio_set_handler (self, smio_handler);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO handler",
            err_smio_set_handler);

    return err;

err_smio_set_handler:
    smio_fmc100m_4ch_destroy (&smio_handler);
err_smio_handler_alloc:
    smio_set_exp_ops (self, NULL);
err_smio_set_exp_ops:
err_fill_desc:
    smio_set_thsafe_client_ops (self, NULL);
err_smio_set_thsafe_ops:
    smio_set_ops (self, NULL);
err_smio_set_ops:
err_set_name:
err_set_id:
    return err;
}

/* Destroy sm_io instance of fmc100m_4ch */
smio_err_e fmc100m_4ch_shutdown (smio_t *self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fmc100m_4ch_exp] Shutting down fmc100m_4ch\n");

    smio_err_e err = SMIO_SUCCESS;
    smio_fmc100m_4ch_t *fmc100m4ch = smio_get_handler (self);
    ASSERT_TEST(fmc100m4ch != NULL, "Could not get FMC 100M 4CH handler",
            err_fmc100m4ch_handler, SMIO_ERR_ALLOC /* FIXME: improve return code */);

    /* Destroy SMIO instance */
    smio_fmc100m_4ch_destroy (&fmc100m4ch);
    /* Nullify operation pointers */
    smio_set_exp_ops (self, NULL);
    smio_set_thsafe_client_ops (self, NULL);
    smio_set_ops (self, NULL);

err_fmc100m4ch_handler:
    return err;
}

const smio_bootstrap_ops_t fmc100m_4ch_bootstrap_ops = {
    .init            = &fmc100m_4ch_init,
    .shutdown        = &fmc100m_4ch_shutdown,
    .config_defaults = &fmc100m_4ch_config_defaults
};

SMIO_MOD_DECLARE(FMC_100M_4CH_SDB_DEVID, FMC_100M_4CH_SDB_NAME, fmc100m_4ch_bootstrap_ops)
