/*
 * Copyright (C) 2021 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "halcs_server.h"
/* Private headers */
#include "sm_io_rtmlamp_ohwr_codes.h"
#include "sm_io_rtmlamp_ohwr_defaults.h"
#include "sm_io_rtmlamp_ohwr_exports.h"
#include "sm_io_rtmlamp_ohwr_core.h"
#include "sm_io_rtmlamp_ohwr_exp.h"
#include "hw/wb_rtmlamp_ohwr_regs.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...)  \
    ASSERT_HAL_TEST(test_boolean, SM_IO, "[sm_io:rtmlamp_ohwr_exp]",           \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)                   \
    ASSERT_HAL_ALLOC(ptr, SM_IO, "[sm_io:rtmlamp_ohwr_exp]",                   \
            smio_err_str(SMIO_ERR_ALLOC),                                       \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                                \
    CHECK_HAL_ERR(err, SM_IO, "[sm_io:rtmlamp_ohwr_exp]",                      \
            smio_err_str (err_type))

/* This must match the FPGA maximum number of channels */
#define RTMLAMP_OHWR_NUM_CHAN                          12
#define RTMLAMP_OHWR_CHAN_OFFSET                       0x00c /* 3 32-bit registers */

/*****************************************************************/
/************ Specific RTMLAMP_OHWR INTERFACE Operations **************/
/*****************************************************************/

#define RTMLAMP_OHWR_STA_R(val)       (val)
#define RTMLAMP_OHWR_STA_W(val)       (val)
#define RTMLAMP_OHWR_STA_MASK         ((1ULL<<32)-1)
RW_PARAM_FUNC(rtmlamp_ohwr, sta) {
    SET_GET_PARAM(rtmlamp_ohwr, 0x0, RTMLAMP_OHWR, STA, /* No field */,
            MULT_BIT_PARAM, /* No min check */, /* No max check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define RTMLAMP_OHWR_CTL_DAC_DATA_FROM_WB_MIN            0
#define RTMLAMP_OHWR_CTL_DAC_DATA_FROM_WB_MAX            1
RW_PARAM_FUNC(rtmlamp_ohwr, dac_data_from_wb) {
    SET_GET_PARAM(rtmlamp_ohwr, 0x0, RTMLAMP_OHWR, CTL, DAC_DATA_FROM_WB,
            SINGLE_BIT_PARAM, RTMLAMP_OHWR_CTL_DAC_DATA_FROM_WB_MIN, 
            RTMLAMP_OHWR_CTL_DAC_DATA_FROM_WB_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define HALCS_RTMLAMP_OHWR_STA_AMP_IFLAG_L_MIN           0 
#define HALCS_RTMLAMP_OHWR_STA_AMP_IFLAG_L_MAX           1 
RW_PARAM_FUNC(rtmlamp_ohwr, amp_iflag_l) {
    SET_GET_PARAM_CHANNEL(rtmlamp_ohwr, 0x0, RTMLAMP_OHWR,
            CH_0_STA, AMP_IFLAG_L, RTMLAMP_OHWR_CHAN_OFFSET, RTMLAMP_OHWR_NUM_CHAN, SINGLE_BIT_PARAM,
            HALCS_RTMLAMP_OHWR_STA_AMP_IFLAG_L_MIN, HALCS_RTMLAMP_OHWR_STA_AMP_IFLAG_L_MAX, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

#define HALCS_RTMLAMP_OHWR_STA_AMP_TFLAG_L_MIN           0 
#define HALCS_RTMLAMP_OHWR_STA_AMP_TFLAG_L_MAX           1 
RW_PARAM_FUNC(rtmlamp_ohwr, amp_tflag_l) {
    SET_GET_PARAM_CHANNEL(rtmlamp_ohwr, 0x0, RTMLAMP_OHWR,
            CH_0_STA, AMP_TFLAG_L, RTMLAMP_OHWR_CHAN_OFFSET, RTMLAMP_OHWR_NUM_CHAN, SINGLE_BIT_PARAM,
            HALCS_RTMLAMP_OHWR_STA_AMP_TFLAG_L_MIN, HALCS_RTMLAMP_OHWR_STA_AMP_TFLAG_L_MAX, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

#define HALCS_RTMLAMP_OHWR_STA_AMP_IFLAG_R_MIN           0 
#define HALCS_RTMLAMP_OHWR_STA_AMP_IFLAG_R_MAX           1 
RW_PARAM_FUNC(rtmlamp_ohwr, amp_iflag_r) {
    SET_GET_PARAM_CHANNEL(rtmlamp_ohwr, 0x0, RTMLAMP_OHWR,
            CH_0_STA, AMP_IFLAG_R, RTMLAMP_OHWR_CHAN_OFFSET, RTMLAMP_OHWR_NUM_CHAN, SINGLE_BIT_PARAM,
            HALCS_RTMLAMP_OHWR_STA_AMP_IFLAG_R_MIN, HALCS_RTMLAMP_OHWR_STA_AMP_IFLAG_R_MAX, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

#define HALCS_RTMLAMP_OHWR_STA_AMP_TFLAG_R_MIN           0 
#define HALCS_RTMLAMP_OHWR_STA_AMP_TFLAG_R_MAX           1 
RW_PARAM_FUNC(rtmlamp_ohwr, amp_tflag_r) {
    SET_GET_PARAM_CHANNEL(rtmlamp_ohwr, 0x0, RTMLAMP_OHWR,
            CH_0_STA, AMP_TFLAG_R, RTMLAMP_OHWR_CHAN_OFFSET, RTMLAMP_OHWR_NUM_CHAN, SINGLE_BIT_PARAM,
            HALCS_RTMLAMP_OHWR_STA_AMP_TFLAG_R_MIN, HALCS_RTMLAMP_OHWR_STA_AMP_TFLAG_R_MAX, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

#define HALCS_RTMLAMP_OHWR_CTL_AMP_EN_MIN                0 
#define HALCS_RTMLAMP_OHWR_CTL_AMP_EN_MAX                1 
RW_PARAM_FUNC(rtmlamp_ohwr, amp_en) {
    SET_GET_PARAM_CHANNEL(rtmlamp_ohwr, 0x0, RTMLAMP_OHWR,
            CH_0_CTL, AMP_EN, RTMLAMP_OHWR_CHAN_OFFSET, RTMLAMP_OHWR_NUM_CHAN, SINGLE_BIT_PARAM,
            HALCS_RTMLAMP_OHWR_CTL_AMP_EN_MIN, HALCS_RTMLAMP_OHWR_CTL_AMP_EN_MAX, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

#define HALCS_RTMLAMP_OHWR_DAC_DATA_MIN                 0 
#define HALCS_RTMLAMP_OHWR_DAC_DATA_MAX                 RTMLAMP_OHWR_CH_0_DAC_DATA_MASK
RW_PARAM_FUNC(rtmlamp_ohwr, dac_data) {
    SET_GET_PARAM_CHANNEL(rtmlamp_ohwr, 0x0, RTMLAMP_OHWR,
            CH_0_DAC, DATA, RTMLAMP_OHWR_CHAN_OFFSET, RTMLAMP_OHWR_NUM_CHAN, MULT_BIT_PARAM,
            HALCS_RTMLAMP_OHWR_DAC_DATA_MIN, HALCS_RTMLAMP_OHWR_DAC_DATA_MAX, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

#define HALCS_RTMLAMP_OHWR_DAC_WR_MIN                    0 
#define HALCS_RTMLAMP_OHWR_DAC_WR_MAX                    1
RW_PARAM_FUNC(rtmlamp_ohwr, dac_wr) {
    SET_GET_PARAM_CHANNEL(rtmlamp_ohwr, 0x0, RTMLAMP_OHWR,
            CH_0_DAC, WR, RTMLAMP_OHWR_CHAN_OFFSET, RTMLAMP_OHWR_NUM_CHAN, SINGLE_BIT_PARAM,
            HALCS_RTMLAMP_OHWR_DAC_WR_MIN, HALCS_RTMLAMP_OHWR_DAC_WR_MAX, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}


/* Exported function pointers */
const disp_table_func_fp rtmlamp_ohwr_exp_fp [] = {
    RW_PARAM_FUNC_NAME(rtmlamp_ohwr, sta),
    RW_PARAM_FUNC_NAME(rtmlamp_ohwr, dac_data_from_wb),
    RW_PARAM_FUNC_NAME(rtmlamp_ohwr, amp_iflag_l),
    RW_PARAM_FUNC_NAME(rtmlamp_ohwr, amp_tflag_l),
    RW_PARAM_FUNC_NAME(rtmlamp_ohwr, amp_iflag_r),
    RW_PARAM_FUNC_NAME(rtmlamp_ohwr, amp_tflag_r),
    RW_PARAM_FUNC_NAME(rtmlamp_ohwr, amp_en),
    RW_PARAM_FUNC_NAME(rtmlamp_ohwr, dac_data),
    RW_PARAM_FUNC_NAME(rtmlamp_ohwr, dac_wr),
    NULL
};

/************************************************************/
/***************** Export methods functions *****************/
/************************************************************/

static smio_err_e _rtmlamp_ohwr_do_op (void *owner, void *msg);

/* Attach an instance of sm_io to dev_io function pointer */
smio_err_e rtmlamp_ohwr_attach (smio_t *self, void *args)
{
    UNUSED(self);
    UNUSED(args);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Deattach an instance of sm_io to dev_io function pointer */
smio_err_e rtmlamp_ohwr_deattach (smio_t *self)
{
    UNUSED(self);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Export (register) sm_io to handle operations function pointer */
smio_err_e rtmlamp_ohwr_export_ops (smio_t *self,
        const disp_op_t** smio_exp_ops)
{
    UNUSED(self);
    UNUSED(smio_exp_ops);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Unexport (unregister) sm_io to handle operations function pointer */
smio_err_e rtmlamp_ohwr_unexport_ops (smio_t *self)
{
    UNUSED(self);
    return SMIO_ERR_FUNC_NOT_IMPL;
}


/* Generic wrapper for receiving opcodes and arguments to specific funtions function pointer */
/* FIXME: Code repetition! _devio_do_smio_op () function does almost the same!!! */
smio_err_e _rtmlamp_ohwr_do_op (void *owner, void *msg)
{
    UNUSED(owner);
    UNUSED(msg);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

smio_err_e rtmlamp_ohwr_do_op (void *self, void *msg)
{
    return _rtmlamp_ohwr_do_op (self, msg);
}

const smio_ops_t rtmlamp_ohwr_ops = {
    .attach             = &rtmlamp_ohwr_attach,          /* Attach sm_io instance to dev_io */
    .deattach           = &rtmlamp_ohwr_deattach,        /* Deattach sm_io instance to dev_io */
    .export_ops         = &rtmlamp_ohwr_export_ops,      /* Export sm_io operations to dev_io */
    .unexport_ops       = &rtmlamp_ohwr_unexport_ops,    /* Unexport sm_io operations to dev_io */
    .do_op              = &rtmlamp_ohwr_do_op            /* Generic wrapper for handling specific operations */
};

/************************************************************/
/****************** Bootstrap Operations ********************/
/************************************************************/

smio_err_e rtmlamp_ohwr_init (smio_t * self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:rtmlamp_ohwr_exp] Initializing rtmlamp_ohwr\n");

    smio_err_e err = SMIO_SUCCESS;

    err = smio_set_id (self, RTMLAMP_OHWR_SDB_DEVID);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO id", err_set_id);
    err = smio_set_name (self, RTMLAMP_OHWR_SDB_NAME);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO name", err_set_name);

    /* Set SMIO ops pointers */
    err = smio_set_ops (self, &rtmlamp_ohwr_ops);
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
    err = smio_init_exp_ops (self, (disp_op_t **) rtmlamp_ohwr_exp_ops,
            rtmlamp_ohwr_exp_fp);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not fill SMIO "
            "function descriptors with the callbacks", err_fill_desc);

    err = smio_set_exp_ops (self, rtmlamp_ohwr_exp_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO exported operations",
            err_smio_set_exp_ops);

    /* Initialize specific structure */
    smio_rtmlamp_ohwr_t *smio_handler = smio_rtmlamp_ohwr_new (self);
    ASSERT_ALLOC(smio_handler, err_smio_handler_alloc, SMIO_ERR_ALLOC);
    err = smio_set_handler (self, smio_handler);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO handler",
            err_smio_set_handler);

    return err;

err_smio_set_handler:
    smio_rtmlamp_ohwr_destroy (&smio_handler);
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

/* Destroy sm_io instance of rtmlamp_ohwr */
smio_err_e rtmlamp_ohwr_shutdown (smio_t *self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:rtmlamp_ohwr_exp] Shutting down rtmlamp_ohwr\n");

    smio_err_e err = SMIO_SUCCESS;
    smio_rtmlamp_ohwr_t *trig_iface = smio_get_handler (self);
    ASSERT_TEST(trig_iface != NULL, "Could not get trig_iface handler",
            err_trig_iface_handler, SMIO_ERR_ALLOC /* FIXME: improve return code */);

    /* Destroy SMIO instance */
    smio_rtmlamp_ohwr_destroy (&trig_iface);
    /* Nullify operation pointers */
    smio_set_exp_ops (self, NULL);
    smio_set_thsafe_client_ops (self, NULL);
    smio_set_ops (self, NULL);

err_trig_iface_handler:
    return err;
}

const smio_bootstrap_ops_t rtmlamp_ohwr_bootstrap_ops = {
    .init            = &rtmlamp_ohwr_init,
    .shutdown        = &rtmlamp_ohwr_shutdown,
    .config_defaults = &rtmlamp_ohwr_config_defaults
};

SMIO_MOD_DECLARE(RTMLAMP_OHWR_SDB_DEVID, RTMLAMP_OHWR_SDB_NAME, rtmlamp_ohwr_bootstrap_ops)
