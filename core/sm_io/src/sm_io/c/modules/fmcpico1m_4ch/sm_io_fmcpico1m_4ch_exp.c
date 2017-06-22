/*
 * Copyright (C) 2017 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "halcs_server.h"
/* Private headers */
#include "sm_io_fmcpico1m_4ch_codes.h"
#include "sm_io_fmcpico1m_4ch_defaults.h"
#include "sm_io_fmcpico1m_4ch_exports.h"
#include "sm_io_fmcpico1m_4ch_core.h"
#include "sm_io_fmcpico1m_4ch_exp.h"
#include "hw/wb_fmcpico1m_4ch_regs.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, SM_IO, "[sm_io:fmcpico1m_4ch_exp]", \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_ALLOC(ptr, SM_IO, "[sm_io:fmcpico1m_4ch_exp]",         \
            smio_err_str(SMIO_ERR_ALLOC),                   \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                            \
    CHECK_HAL_ERR(err, SM_IO, "[sm_io:fmcpico1m_4ch_exp]",            \
            smio_err_str (err_type))

/************************************************************/
/*****************  Specific FMCPICO1M_4CH Operations *****************/
/************************************************************/

#define FMC_CTL_LED1_MIN                            0
#define FMC_CTL_LED1_MAX                            1
RW_PARAM_FUNC(fmcpico1m_4ch, ctl_led1) {
    SET_GET_PARAM(fmcpico1m_4ch, 0x0, WB_FMCPICO1M_4CH_CSR, FMC_CTL, LED1,
            SINLE_BIT_PARAM, FMC_CTL_LED1_MIN, FMC_CTL_LED1_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FMC_CTL_LED2_MIN                            0
#define FMC_CTL_LED2_MAX                            1
RW_PARAM_FUNC(fmcpico1m_4ch, ctl_led2) {
    SET_GET_PARAM(fmcpico1m_4ch, 0x0, WB_FMCPICO1M_4CH_CSR, FMC_CTL, LED2,
            SINLE_BIT_PARAM, FMC_CTL_LED2_MIN, FMC_CTL_LED2_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FMC_RNG_CTL_R0_MIN                          0
#define FMC_RNG_CTL_R0_MAX                          1
RW_PARAM_FUNC(fmcpico1m_4ch, rng_r0) {
    SET_GET_PARAM(fmcpico1m_4ch, 0x0, WB_FMCPICO1M_4CH_CSR, RNG_CTL, R0,
            SINLE_BIT_PARAM, FMC_RNG_CTL_R0_MIN, FMC_RNG_CTL_R0_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FMC_RNG_CTL_R1_MIN                          0
#define FMC_RNG_CTL_R1_MAX                          1
RW_PARAM_FUNC(fmcpico1m_4ch, rng_r1) {
    SET_GET_PARAM(fmcpico1m_4ch, 0x0, WB_FMCPICO1M_4CH_CSR, RNG_CTL, R1,
            SINLE_BIT_PARAM, FMC_RNG_CTL_R1_MIN, FMC_RNG_CTL_R1_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FMC_RNG_CTL_R2_MIN                          0
#define FMC_RNG_CTL_R2_MAX                          1
RW_PARAM_FUNC(fmcpico1m_4ch, rng_r2) {
    SET_GET_PARAM(fmcpico1m_4ch, 0x0, WB_FMCPICO1M_4CH_CSR, RNG_CTL, R2,
            SINLE_BIT_PARAM, FMC_RNG_CTL_R2_MIN, FMC_RNG_CTL_R2_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FMC_RNG_CTL_R3_MIN                          0
#define FMC_RNG_CTL_R3_MAX                          1
RW_PARAM_FUNC(fmcpico1m_4ch, rng_r3) {
    SET_GET_PARAM(fmcpico1m_4ch, 0x0, WB_FMCPICO1M_4CH_CSR, RNG_CTL, R3,
            SINLE_BIT_PARAM, FMC_RNG_CTL_R3_MIN, FMC_RNG_CTL_R3_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define WB_FMCPICO1M_4CH_CSR_DATA0_R(val)           (val)
#define WB_FMCPICO1M_4CH_CSR_DATA0_W(val)           (val)
#define WB_FMCPICO1M_4CH_CSR_DATA0_MASK             ((1ULL<<32)-1)
RW_PARAM_FUNC(fmcpico1m_4ch, adc_data0) {
    SET_GET_PARAM(fmcpico1m_4ch, 0x0, WB_FMCPICO1M_4CH_CSR, DATA0, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define WB_FMCPICO1M_4CH_CSR_DATA1_R(val)           (val)
#define WB_FMCPICO1M_4CH_CSR_DATA1_W(val)           (val)
#define WB_FMCPICO1M_4CH_CSR_DATA1_MASK             ((1ULL<<32)-1)
RW_PARAM_FUNC(fmcpico1m_4ch, adc_data1) {
    SET_GET_PARAM(fmcpico1m_4ch, 0x0, WB_FMCPICO1M_4CH_CSR, DATA1, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define WB_FMCPICO1M_4CH_CSR_DATA2_R(val)           (val)
#define WB_FMCPICO1M_4CH_CSR_DATA2_W(val)           (val)
#define WB_FMCPICO1M_4CH_CSR_DATA2_MASK             ((1ULL<<32)-1)
RW_PARAM_FUNC(fmcpico1m_4ch, adc_data2) {
    SET_GET_PARAM(fmcpico1m_4ch, 0x0, WB_FMCPICO1M_4CH_CSR, DATA2, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define WB_FMCPICO1M_4CH_CSR_DATA3_R(val)           (val)
#define WB_FMCPICO1M_4CH_CSR_DATA3_W(val)           (val)
#define WB_FMCPICO1M_4CH_CSR_DATA3_MASK             ((1ULL<<32)-1)
RW_PARAM_FUNC(fmcpico1m_4ch, adc_data3) {
    SET_GET_PARAM(fmcpico1m_4ch, 0x0, WB_FMCPICO1M_4CH_CSR, DATA3, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

/* Exported function pointers */
const disp_table_func_fp fmcpico1m_4ch_exp_fp [] = {
    RW_PARAM_FUNC_NAME(fmcpico1m_4ch, ctl_led1),
    RW_PARAM_FUNC_NAME(fmcpico1m_4ch, ctl_led2),
    RW_PARAM_FUNC_NAME(fmcpico1m_4ch, rng_r0),
    RW_PARAM_FUNC_NAME(fmcpico1m_4ch, rng_r1),
    RW_PARAM_FUNC_NAME(fmcpico1m_4ch, rng_r2),
    RW_PARAM_FUNC_NAME(fmcpico1m_4ch, rng_r3),
    RW_PARAM_FUNC_NAME(fmcpico1m_4ch, adc_data0),
    RW_PARAM_FUNC_NAME(fmcpico1m_4ch, adc_data1),
    RW_PARAM_FUNC_NAME(fmcpico1m_4ch, adc_data2),
    RW_PARAM_FUNC_NAME(fmcpico1m_4ch, adc_data3),
    NULL
};

/************************************************************/
/***************** Export methods functions *****************/
/************************************************************/

static smio_err_e _fmcpico1m_4ch_do_op (void *owner, void *msg);

/* Attach an instance of sm_io to dev_io function pointer */
smio_err_e fmcpico1m_4ch_attach (smio_t *self, void *args)
{
    UNUSED(self);
    UNUSED(args);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Deattach an instance of sm_io to dev_io function pointer */
smio_err_e fmcpico1m_4ch_deattach (smio_t *self)
{
    UNUSED(self);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Export (register) sm_io to handle operations function pointer */
smio_err_e fmcpico1m_4ch_export_ops (smio_t *self,
        const disp_op_t **smio_exp_ops)
{
    UNUSED(self);
    UNUSED(smio_exp_ops);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Unexport (unregister) sm_io to handle operations function pointer */
smio_err_e fmcpico1m_4ch_unexport_ops (smio_t *self)
{
    UNUSED(self);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Generic wrapper for receiving opcodes and arguments to specific funtions function pointer */
/* FIXME: Code repetition! _devio_do_smio_op () function does almost the same!!! */
smio_err_e _fmcpico1m_4ch_do_op (void *owner, void *msg)
{
    UNUSED(owner);
    UNUSED(msg);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

smio_err_e fmcpico1m_4ch_do_op (void *self, void *msg)
{
    return _fmcpico1m_4ch_do_op (self, msg);
}

const smio_ops_t fmcpico1m_4ch_ops = {
    .attach             = &fmcpico1m_4ch_attach,          /* Attach sm_io instance to dev_io */
    .deattach           = &fmcpico1m_4ch_deattach,        /* Deattach sm_io instance to dev_io */
    .export_ops         = &fmcpico1m_4ch_export_ops,      /* Export sm_io operations to dev_io */
    .unexport_ops       = &fmcpico1m_4ch_unexport_ops,    /* Unexport sm_io operations to dev_io */
    .do_op              = &fmcpico1m_4ch_do_op            /* Generic wrapper for handling specific operations */
};

/************************************************************/
/****************** Bootstrap Operations ********************/
/************************************************************/

smio_err_e fmcpico1m_4ch_init (smio_t * self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fmcpico1m_4ch_exp] Initializing fmcpico1m_4ch\n");

    smio_err_e err = SMIO_SUCCESS;

    err = smio_set_id (self, FMCPICO1M_4CH_SDB_DEVID);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO id", err_set_id);
    err = smio_set_name (self, FMCPICO1M_4CH_SDB_NAME);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO name", err_set_name);

    /* Set SMIO ops pointers */
    err = smio_set_ops (self, &fmcpico1m_4ch_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO operations",
            err_smio_set_ops);
    err = smio_set_thsafe_client_ops (self, &smio_thsafe_client_zmq_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO thsafe operations",
            err_smio_set_thsafe_ops);

    /* disp_op_t structure is const and all of the functions performing on it
     * obviously receives a const argument, but here (and only on the SMIO
     * initialization) we need to make an exception if we want to keep the
     * functions' description and the function pointers separate */
    err = smio_init_exp_ops (self, (disp_op_t **) fmcpico1m_4ch_exp_ops, fmcpico1m_4ch_exp_fp);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not fill SMIO "
            "function descriptors with the callbacks", err_fill_desc);

    err = smio_set_exp_ops (self, fmcpico1m_4ch_exp_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO exported operations",
            err_smio_set_exp_ops);

    /* Initialize specific structure */
    smio_fmcpico1m_4ch_t *smio_handler = smio_fmcpico1m_4ch_new (self);
    ASSERT_ALLOC(smio_handler, err_smio_handler_alloc, SMIO_ERR_ALLOC);
    err = smio_set_handler (self, smio_handler);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO handler",
            err_smio_set_handler);

    return err;

err_smio_set_handler:
    smio_fmcpico1m_4ch_destroy (&smio_handler);
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

/* Destroy sm_io instance of fmcpico1m_4ch */
smio_err_e fmcpico1m_4ch_shutdown (smio_t *self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fmcpico1m_4ch_exp] Shutting down fmcpico1m_4ch\n");

    smio_err_e err = SMIO_SUCCESS;
    smio_fmcpico1m_4ch_t *fmcpico1m_4ch = smio_get_handler (self);
    ASSERT_TEST(fmcpico1m_4ch != NULL, "Could not get FMCPICO1M_4CH handler",
            err_fmcpico1m_4ch_handler, SMIO_ERR_ALLOC /* FIXME: improve return code */);

    /* Destroy SMIO instance */
    smio_fmcpico1m_4ch_destroy (&fmcpico1m_4ch);
    /* Nullify operation pointers */
    smio_set_exp_ops (self, NULL);
    smio_set_thsafe_client_ops (self, NULL);
    smio_set_ops (self, NULL);

err_fmcpico1m_4ch_handler:
    return err;
}

const smio_bootstrap_ops_t fmcpico1m_4ch_bootstrap_ops = {
    .init            = &fmcpico1m_4ch_init,
    .shutdown        = &fmcpico1m_4ch_shutdown,
    .config_defaults = &fmcpico1m_4ch_config_defaults
};

SMIO_MOD_DECLARE(FMCPICO1M_4CH_SDB_DEVID, FMCPICO1M_4CH_SDB_NAME, fmcpico1m_4ch_bootstrap_ops)
