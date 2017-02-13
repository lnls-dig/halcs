/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "halcs_server.h"
/* Private headers */
#include "sm_io_fmc_adc_common_codes.h"
#include "sm_io_fmc_adc_common_defaults.h"
#include "sm_io_fmc_adc_common_exports.h"
#include "sm_io_fmc_adc_common_core.h"
#include "sm_io_fmc_adc_common_exp.h"
#include "hw/wb_fmc_adc_common_regs.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...)  \
    ASSERT_HAL_TEST(test_boolean, SM_IO, "[sm_io:fmc_adc_common_exp]",          \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)                   \
    ASSERT_HAL_ALLOC(ptr, SM_IO, "[sm_io:fmc_adc_common_exp]",                  \
            smio_err_str(SMIO_ERR_ALLOC),                                       \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                                \
    CHECK_HAL_ERR(err, SM_IO, "[sm_io:fmc_adc_common_exp]",                     \
            smio_err_str (err_type))

/************************************************************/
/************ Specific FMC_ADC_COMMON Operations **************/
/************************************************************/

#define HALCS_FMC_ADC_COMMON_LEDS_MIN                     0       /* LED 0 = 0, LED 1 = 0, LED 3 = 0*/
#define HALCS_FMC_ADC_COMMON_LEDS_MAX                     7       /* LED 0 = 1, LED 1 = 1, LED 2 = 1*/
#define WB_FMC_ADC_COMMON_CSR_MONITOR_GLOBAL_MASK       WBGEN2_GEN_MASK(1, 3)
#define WB_FMC_ADC_COMMON_CSR_MONITOR_GLOBAL_W(val)     WBGEN2_GEN_WRITE(val, 1, 3)
#define WB_FMC_ADC_COMMON_CSR_MONITOR_GLOBAL_R(reg)     WBGEN2_GEN_READ(reg, 1, 3)

RW_PARAM_FUNC(fmc_adc_common, leds) {
    SET_GET_PARAM(fmc_adc_common, 0x0, WB_FMC_ADC_COMMON_CSR,
            MONITOR, GLOBAL, MULT_BIT_PARAM,
            HALCS_FMC_ADC_COMMON_LEDS_MIN, HALCS_FMC_ADC_COMMON_LEDS_MAX, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

/***************************** FPGA Control ***************************/

#define HALCS_FMC_ADC_COMMON_TEST_DATA_EN_MIN             0 /* TEST data enable off */
#define HALCS_FMC_ADC_COMMON_TEST_DATA_EN_MAX             1 /* TEST data enable on */

RW_PARAM_FUNC(fmc_adc_common, test_data_en) {
    SET_GET_PARAM(fmc_adc_common, 0x0, WB_FMC_ADC_COMMON_CSR,
            MONITOR, TEST_DATA_EN, SINGLE_BIT_PARAM,
            HALCS_FMC_ADC_COMMON_TEST_DATA_EN_MIN, HALCS_FMC_ADC_COMMON_TEST_DATA_EN_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define HALCS_FMC_ADC_COMMON_TRIG_DIR_MIN                 0 /* Trigger direction input */
#define HALCS_FMC_ADC_COMMON_TRIG_DIR_MAX                 1 /* Trigger direction output */

RW_PARAM_FUNC(fmc_adc_common, trig_dir) {
    SET_GET_PARAM(fmc_adc_common, 0x0, WB_FMC_ADC_COMMON_CSR,
            TRIGGER, DIR, SINGLE_BIT_PARAM,
            HALCS_FMC_ADC_COMMON_TRIG_DIR_MIN, HALCS_FMC_ADC_COMMON_TRIG_DIR_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define HALCS_FMC_ADC_COMMON_TRIG_TERM_MIN                0 /* Trigger termination disabled */
#define HALCS_FMC_ADC_COMMON_TRIG_TERM_MAX                1 /* Trigger termination enabled */

RW_PARAM_FUNC(fmc_adc_common, trig_term) {
    SET_GET_PARAM(fmc_adc_common, 0x0, WB_FMC_ADC_COMMON_CSR,
            TRIGGER, TERM, SINGLE_BIT_PARAM,
            HALCS_FMC_ADC_COMMON_TRIG_TERM_MIN, HALCS_FMC_ADC_COMMON_TRIG_TERM_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define HALCS_FMC_ADC_COMMON_TRIG_VAL_MIN                 0 /* Trigger value 0 */
#define HALCS_FMC_ADC_COMMON_TRIG_VAL_MAX                 1 /* Trigger value 1 */

RW_PARAM_FUNC(fmc_adc_common, trig_val) {
    SET_GET_PARAM(fmc_adc_common, 0x0, WB_FMC_ADC_COMMON_CSR,
            TRIGGER, TRIG_VAL, SINGLE_BIT_PARAM,
            HALCS_FMC_ADC_COMMON_TRIG_VAL_MIN, HALCS_FMC_ADC_COMMON_TRIG_VAL_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

/* Exported function pointers */
const disp_table_func_fp fmc_adc_common_exp_fp [] = {
    RW_PARAM_FUNC_NAME(fmc_adc_common, leds),
    RW_PARAM_FUNC_NAME(fmc_adc_common, test_data_en),
    RW_PARAM_FUNC_NAME(fmc_adc_common, trig_dir),
    RW_PARAM_FUNC_NAME(fmc_adc_common, trig_term),
    RW_PARAM_FUNC_NAME(fmc_adc_common, trig_val),
    NULL
};

/************************************************************/
/***************** Export methods functions *****************/
/************************************************************/

static smio_err_e _fmc_adc_common_do_op (void *owner, void *msg);

/* Attach an instance of sm_io to dev_io function pointer */
smio_err_e fmc_adc_common_attach (smio_t *self, devio_t *parent)
{
    UNUSED(self);
    UNUSED(parent);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Deattach an instance of sm_io to dev_io function pointer */
smio_err_e fmc_adc_common_deattach (smio_t *self)
{
    UNUSED(self);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Export (register) sm_io to handle operations function pointer */
smio_err_e fmc_adc_common_export_ops (smio_t *self,
        const disp_op_t** smio_exp_ops)
{
    UNUSED(self);
    UNUSED(smio_exp_ops);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Unexport (unregister) sm_io to handle operations function pointer */
smio_err_e fmc_adc_common_unexport_ops (smio_t *self)
{
    UNUSED(self);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Generic wrapper for receiving opcodes and arguments to specific funtions function pointer */
/* FIXME: Code repetition! _devio_do_smio_op () function does almost the same!!! */
smio_err_e _fmc_adc_common_do_op (void *owner, void *msg)
{
    UNUSED(owner);
    UNUSED(msg);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

smio_err_e fmc_adc_common_do_op (void *self, void *msg)
{
    return _fmc_adc_common_do_op (self, msg);
}

const smio_ops_t fmc_adc_common_ops = {
    .attach             = fmc_adc_common_attach,          /* Attach sm_io instance to dev_io */
    .deattach           = fmc_adc_common_deattach,        /* Deattach sm_io instance to dev_io */
    .export_ops         = fmc_adc_common_export_ops,      /* Export sm_io operations to dev_io */
    .unexport_ops       = fmc_adc_common_unexport_ops,    /* Unexport sm_io operations to dev_io */
    .do_op              = fmc_adc_common_do_op            /* Generic wrapper for handling specific operations */
};

/************************************************************/
/****************** Bootstrap Operations ********************/
/************************************************************/

smio_err_e fmc_adc_common_init (smio_t * self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fmc_adc_common_exp] Initializing fmc_adc_common\n");

    smio_err_e err = SMIO_SUCCESS;

    err = smio_set_id (self, FMC_ADC_COMMON_SDB_DEVID);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO id", err_set_id);
    err = smio_set_name (self, FMC_ADC_COMMON_SDB_NAME);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO name", err_set_name);

    /* Set SMIO ops pointers */
    err = smio_set_ops (self, &fmc_adc_common_ops);
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
    err = smio_init_exp_ops (self, (disp_op_t **) fmc_adc_common_exp_ops,
            fmc_adc_common_exp_fp);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not fill SMIO "
            "function descriptors with the callbacks", err_fill_desc);

    err = smio_set_exp_ops (self, fmc_adc_common_exp_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO exported operations",
            err_smio_set_exp_ops);

    /* Initialize specific structure */
    smio_fmc_adc_common_t *smio_handler = smio_fmc_adc_common_new (self);
    ASSERT_ALLOC(smio_handler, err_smio_handler_alloc, SMIO_ERR_ALLOC);
    err = smio_set_handler (self, smio_handler);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO handler",
            err_smio_set_handler);

    return err;

err_smio_set_handler:
    smio_fmc_adc_common_destroy (&smio_handler);
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

/* Destroy sm_io instance of fmc_adc_common */
smio_err_e fmc_adc_common_shutdown (smio_t *self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fmc_adc_common_exp] Shutting down fmc_adc_common\n");

    smio_err_e err = SMIO_SUCCESS;
    smio_fmc_adc_common_t *fmcacommon = smio_get_handler (self);
    ASSERT_TEST(fmcacommon != NULL, "Could not get FMC ADC COMMON handler",
            err_fmcacommon_handler, SMIO_ERR_ALLOC /* FIXME: improve return code */);

    /* Destroy SMIO instance */
    smio_fmc_adc_common_destroy (&fmcacommon);
    /* Nullify operation pointers */
    smio_set_exp_ops (self, NULL);
    smio_set_thsafe_client_ops (self, NULL);
    smio_set_ops (self, NULL);

err_fmcacommon_handler:
    return err;
}

const smio_bootstrap_ops_t fmc_adc_common_bootstrap_ops = {
    .init = fmc_adc_common_init,
    .shutdown = fmc_adc_common_shutdown,
    .config_defaults = fmc_adc_common_config_defaults
};

SMIO_MOD_DECLARE(FMC_ADC_COMMON_SDB_DEVID, FMC_ADC_COMMON_SDB_NAME, fmc_adc_common_bootstrap_ops)
