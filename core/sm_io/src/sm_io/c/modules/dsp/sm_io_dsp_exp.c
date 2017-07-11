/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "halcs_server.h"
/* Private headers */
#include "sm_io_dsp_codes.h"
#include "sm_io_dsp_defaults.h"
#include "sm_io_dsp_exports.h"
#include "sm_io_dsp_core.h"
#include "sm_io_dsp_exp.h"
#include "hw/wb_pos_calc_regs.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, SM_IO, "[sm_io:dsp_exp]", \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_ALLOC(ptr, SM_IO, "[sm_io:dsp_exp]",         \
            smio_err_str(SMIO_ERR_ALLOC),                   \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                            \
    CHECK_HAL_ERR(err, SM_IO, "[sm_io:dsp_exp]",            \
            smio_err_str (err_type))

/************************************************************/
/*****************  Specific DSP Operations *****************/
/************************************************************/

#define KX_PARAM_MIN                        1
#define KX_PARAM_MAX                        ((1<<25)-1)
RW_PARAM_FUNC(dsp, kx) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, KX, VAL, MULT_BIT_PARAM,
            KX_PARAM_MIN, KX_PARAM_MAX, NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define KY_PARAM_MIN                        1
#define KY_PARAM_MAX                        ((1<<25)-1)
RW_PARAM_FUNC(dsp, ky) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, KY, VAL, MULT_BIT_PARAM,
            KY_PARAM_MIN, KY_PARAM_MAX, NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define KSUM_PARAM_MIN                      1
#define KSUM_PARAM_MAX                      ((1<<25)-1)
RW_PARAM_FUNC(dsp, ksum) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, KSUM, VAL, MULT_BIT_PARAM,
            KSUM_PARAM_MIN, KSUM_PARAM_MAX, NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define DS_TBT_THRES_MIN                    0
#define DS_TBT_THRES_MAX                    ((1<<26)-1)
RW_PARAM_FUNC(dsp, ds_tbt_thres) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, DS_TBT_THRES, VAL, MULT_BIT_PARAM,
            DS_TBT_THRES_MIN, DS_TBT_THRES_MAX, NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define DS_FOFB_THRES_MIN                   0
#define DS_FOFB_THRES_MAX                   ((1<<26)-1)
RW_PARAM_FUNC(dsp, ds_fofb_thres) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, DS_FOFB_THRES, VAL, MULT_BIT_PARAM,
            DS_FOFB_THRES_MIN, DS_FOFB_THRES_MAX, NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define DS_MONIT_THRES_MIN                  0
#define DS_MONIT_THRES_MAX                  ((1<<26)-1)
RW_PARAM_FUNC(dsp, ds_monit_thres) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, DS_MONIT_THRES, VAL, MULT_BIT_PARAM,
            DS_MONIT_THRES_MIN, DS_MONIT_THRES_MAX, NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define POS_CALC_DDS_CFG_TEST_DATA_MIN      0
#define POS_CALC_DDS_CFG_TEST_DATA_MAX      1
RW_PARAM_FUNC(dsp, pos_calc_cfg_test_data) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, DDS_CFG, TEST_DATA, SINGLE_BIT_PARAM,
            POS_CALC_DDS_CFG_TEST_DATA_MIN, POS_CALC_DDS_CFG_TEST_DATA_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define POS_CALC_DSP_MONIT_AMP_CH0_R(val)       (val)
#define POS_CALC_DSP_MONIT_AMP_CH0_W(val)       (val)
#define POS_CALC_DSP_MONIT_AMP_CH0_MASK         ((1ULL<<32)-1)

RW_PARAM_FUNC(dsp, monit_amp_ch0) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, DSP_MONIT_AMP_CH0, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define POS_CALC_DSP_MONIT_AMP_CH1_R(val)       (val)
#define POS_CALC_DSP_MONIT_AMP_CH1_W(val)       (val)
#define POS_CALC_DSP_MONIT_AMP_CH1_MASK         ((1ULL<<32)-1)

RW_PARAM_FUNC(dsp, monit_amp_ch1) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, DSP_MONIT_AMP_CH1, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define POS_CALC_DSP_MONIT_AMP_CH2_R(val)       (val)
#define POS_CALC_DSP_MONIT_AMP_CH2_W(val)       (val)
#define POS_CALC_DSP_MONIT_AMP_CH2_MASK         ((1ULL<<32)-1)

RW_PARAM_FUNC(dsp, monit_amp_ch2) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, DSP_MONIT_AMP_CH2, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define POS_CALC_DSP_MONIT_AMP_CH3_R(val)       (val)
#define POS_CALC_DSP_MONIT_AMP_CH3_W(val)       (val)
#define POS_CALC_DSP_MONIT_AMP_CH3_MASK         ((1ULL<<32)-1)

RW_PARAM_FUNC(dsp, monit_amp_ch3) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, DSP_MONIT_AMP_CH3, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define POS_CALC_DSP_MONIT_POS_X_R(val)       (val)
#define POS_CALC_DSP_MONIT_POS_X_W(val)       (val)
#define POS_CALC_DSP_MONIT_POS_X_MASK         ((1ULL<<32)-1)

RW_PARAM_FUNC(dsp, monit_pos_x) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, DSP_MONIT_POS_X, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define POS_CALC_DSP_MONIT_POS_Y_R(val)       (val)
#define POS_CALC_DSP_MONIT_POS_Y_W(val)       (val)
#define POS_CALC_DSP_MONIT_POS_Y_MASK         ((1ULL<<32)-1)

RW_PARAM_FUNC(dsp, monit_pos_y) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, DSP_MONIT_POS_Y, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define POS_CALC_DSP_MONIT_POS_Q_R(val)       (val)
#define POS_CALC_DSP_MONIT_POS_Q_W(val)       (val)
#define POS_CALC_DSP_MONIT_POS_Q_MASK         ((1ULL<<32)-1)

RW_PARAM_FUNC(dsp, monit_pos_q) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, DSP_MONIT_POS_Q, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define POS_CALC_DSP_MONIT_POS_SUM_R(val)       (val)
#define POS_CALC_DSP_MONIT_POS_SUM_W(val)       (val)
#define POS_CALC_DSP_MONIT_POS_SUM_MASK         ((1ULL<<32)-1)

RW_PARAM_FUNC(dsp, monit_pos_sum) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, DSP_MONIT_POS_SUM, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define POS_CALC_DSP_MONIT_UPDT_R(val)          (val)
#define POS_CALC_DSP_MONIT_UPDT_W(val)          (val)
#define POS_CALC_DSP_MONIT_UPDT_MASK            ((1ULL<<32)-1)

RW_PARAM_FUNC(dsp, monit_updt) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, DSP_MONIT_UPDT, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

static int _dsp_monit_amp_pos (void *owner, void *args, void *ret)
{
    assert (owner);
    assert (args);
    int err = -DSP_OK;
    RW_REPLY_TYPE rw_err = RW_OK;

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:dsp] "
            "Calling _dsp_monit_amp_pos\n");
    SMIO_OWNER_TYPE *self = SMIO_EXP_OWNER(owner);
    smio_dsp_t *dsp = smio_get_handler (self);
    ASSERT_TEST(dsp != NULL, "Could not get SMIO DSP handler",
            err_get_dsp_handler, -DSP_ERR);

    /* 
     * Message is:
     * frame 0: operation code
     */
    smio_dsp_data_t *data = (smio_dsp_data_t *) ret;

    rw_err |= GET_PARAM(self, dsp, 0x0, POS_CALC,
            DSP_MONIT_AMP_CH0, /* field = NULL */, MULT_BIT_PARAM, 
            data->amp_ch0, NO_FMT_FUNC);
    rw_err |= GET_PARAM(self, dsp, 0x0, POS_CALC,
            DSP_MONIT_AMP_CH1, /* field = NULL */, MULT_BIT_PARAM, 
            data->amp_ch1, NO_FMT_FUNC);
    rw_err |= GET_PARAM(self, dsp, 0x0, POS_CALC,
            DSP_MONIT_AMP_CH2, /* field = NULL */, MULT_BIT_PARAM, 
            data->amp_ch2, NO_FMT_FUNC);
    rw_err |= GET_PARAM(self, dsp, 0x0, POS_CALC,
            DSP_MONIT_AMP_CH3, /* field = NULL */, MULT_BIT_PARAM, 
            data->amp_ch3, NO_FMT_FUNC);
    ASSERT_TEST(rw_err == RW_OK, "Could not get MONIT. amp/pos parameters", 
        err_exit, -DSP_ERR);

    return sizeof (*data);

err_get_dsp_handler:
err_exit:
    return err;
}

/* Exported function pointers */
const disp_table_func_fp dsp_exp_fp [] = {
    RW_PARAM_FUNC_NAME(dsp, kx),
    RW_PARAM_FUNC_NAME(dsp, ky),
    RW_PARAM_FUNC_NAME(dsp, ksum),
    RW_PARAM_FUNC_NAME(dsp, ds_tbt_thres),
    RW_PARAM_FUNC_NAME(dsp, ds_fofb_thres),
    RW_PARAM_FUNC_NAME(dsp, ds_monit_thres),
    RW_PARAM_FUNC_NAME(dsp, pos_calc_cfg_test_data),
    RW_PARAM_FUNC_NAME(dsp, monit_amp_ch0),
    RW_PARAM_FUNC_NAME(dsp, monit_amp_ch1),
    RW_PARAM_FUNC_NAME(dsp, monit_amp_ch2),
    RW_PARAM_FUNC_NAME(dsp, monit_amp_ch3),
    RW_PARAM_FUNC_NAME(dsp, monit_pos_x),
    RW_PARAM_FUNC_NAME(dsp, monit_pos_y),
    RW_PARAM_FUNC_NAME(dsp, monit_pos_q),
    RW_PARAM_FUNC_NAME(dsp, monit_pos_sum),
    RW_PARAM_FUNC_NAME(dsp, monit_updt),
    _dsp_monit_amp_pos,
    NULL
};

/************************************************************/
/***************** Export methods functions *****************/
/************************************************************/

static smio_err_e _dsp_do_op (void *owner, void *msg);

/* Attach an instance of sm_io to dev_io function pointer */
smio_err_e dsp_attach (smio_t *self, void *args)
{
    UNUSED(self);
    UNUSED(args);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Deattach an instance of sm_io to dev_io function pointer */
smio_err_e dsp_deattach (smio_t *self)
{
    UNUSED(self);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Export (register) sm_io to handle operations function pointer */
smio_err_e dsp_export_ops (smio_t *self,
        const disp_op_t **smio_exp_ops)
{
    UNUSED(self);
    UNUSED(smio_exp_ops);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Unexport (unregister) sm_io to handle operations function pointer */
smio_err_e dsp_unexport_ops (smio_t *self)
{
    UNUSED(self);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Generic wrapper for receiving opcodes and arguments to specific funtions function pointer */
/* FIXME: Code repetition! _devio_do_smio_op () function does almost the same!!! */
smio_err_e _dsp_do_op (void *owner, void *msg)
{
    UNUSED(owner);
    UNUSED(msg);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

smio_err_e dsp_do_op (void *self, void *msg)
{
    return _dsp_do_op (self, msg);
}

const smio_ops_t dsp_ops = {
    .attach             = &dsp_attach,          /* Attach sm_io instance to dev_io */
    .deattach           = &dsp_deattach,        /* Deattach sm_io instance to dev_io */
    .export_ops         = &dsp_export_ops,      /* Export sm_io operations to dev_io */
    .unexport_ops       = &dsp_unexport_ops,    /* Unexport sm_io operations to dev_io */
    .do_op              = &dsp_do_op            /* Generic wrapper for handling specific operations */
};

/************************************************************/
/****************** Bootstrap Operations ********************/
/************************************************************/

smio_err_e dsp_init (smio_t * self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:dsp_exp] Initializing dsp\n");

    smio_err_e err = SMIO_SUCCESS;

    err = smio_set_id (self, DSP_SDB_DEVID);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO id", err_set_id);
    err = smio_set_name (self, DSP_SDB_NAME);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO name", err_set_name);

    /* Set SMIO ops pointers */
    err = smio_set_ops (self, &dsp_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO operations",
            err_smio_set_ops);
    err = smio_set_thsafe_client_ops (self, &smio_thsafe_client_zmq_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO thsafe operations",
            err_smio_set_thsafe_ops);

    /* disp_op_t structure is const and all of the functions performing on it
     * obviously receives a const argument, but here (and only on the SMIO
     * initialization) we need to make an exception if we want to keep the
     * functions' description and the function pointers separate */
    err = smio_init_exp_ops (self, (disp_op_t **) dsp_exp_ops, dsp_exp_fp);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not fill SMIO "
            "function descriptors with the callbacks", err_fill_desc);

    err = smio_set_exp_ops (self, dsp_exp_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO exported operations",
            err_smio_set_exp_ops);

    /* Initialize specific structure */
    smio_dsp_t *smio_handler = smio_dsp_new (self);
    ASSERT_ALLOC(smio_handler, err_smio_handler_alloc, SMIO_ERR_ALLOC);
    err = smio_set_handler (self, smio_handler);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO handler",
            err_smio_set_handler);

    return err;

err_smio_set_handler:
    smio_dsp_destroy (&smio_handler);
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

/* Destroy sm_io instance of dsp */
smio_err_e dsp_shutdown (smio_t *self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:dsp_exp] Shutting down dsp\n");

    smio_err_e err = SMIO_SUCCESS;
    smio_dsp_t *dsp = smio_get_handler (self);
    ASSERT_TEST(dsp != NULL, "Could not get DSP handler",
            err_dsp_handler, SMIO_ERR_ALLOC /* FIXME: improve return code */);

    /* Destroy SMIO instance */
    smio_dsp_destroy (&dsp);
    /* Nullify operation pointers */
    smio_set_exp_ops (self, NULL);
    smio_set_thsafe_client_ops (self, NULL);
    smio_set_ops (self, NULL);

err_dsp_handler:
    return err;
}

const smio_bootstrap_ops_t dsp_bootstrap_ops = {
    .init            = &dsp_init,
    .shutdown        = &dsp_shutdown,
    .config_defaults = &dsp_config_defaults
};

SMIO_MOD_DECLARE(DSP_SDB_DEVID, DSP_SDB_NAME, dsp_bootstrap_ops)
