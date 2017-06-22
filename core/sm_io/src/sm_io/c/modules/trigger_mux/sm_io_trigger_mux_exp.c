/*
 * Copyright (C) 2016 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "halcs_server.h"
/* Private headers */
#include "sm_io_trigger_mux_codes.h"
#include "sm_io_trigger_mux_defaults.h"
#include "sm_io_trigger_mux_exports.h"
#include "sm_io_trigger_mux_core.h"
#include "sm_io_trigger_mux_exp.h"
#include "hw/wb_trigger_mux_regs.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...)  \
    ASSERT_HAL_TEST(test_boolean, SM_IO, "[sm_io:trigger_mux_exp]",             \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)                   \
    ASSERT_HAL_ALLOC(ptr, SM_IO, "[sm_io:trigger_mux_exp]",                     \
            smio_err_str(SMIO_ERR_ALLOC),                                       \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                                \
    CHECK_HAL_ERR(err, SM_IO, "[sm_io:trigger_mux_exp]",                        \
            smio_err_str (err_type))

/* This must match the FPGA maximum number of channels */
#define TRIGGER_MUX_NUM_CHAN                          24
#define TRIGGER_MUX_CHAN_OFFSET                       0x008 /* 2 32-bit registers */

/*****************************************************************/
/************ Specific TRIGGER MUX Operations **************/
/*****************************************************************/

#define HALCS_TRIGGER_MUX_RCV_SRC_MIN                         0 /* Trigger Source Selection (= Trigger Backplane) */
#define HALCS_TRIGGER_MUX_RCV_SRC_MAX                         1 /* Trigger Source Selection (= FPGA internal) */
RW_PARAM_FUNC(trigger_mux, rcv_src) {
    SET_GET_PARAM_CHANNEL(trigger_mux, 0x0, WB_TRIG_MUX,
            CH0_CTL, RCV_SRC, TRIGGER_MUX_CHAN_OFFSET, TRIGGER_MUX_NUM_CHAN, SINGLE_BIT_PARAM,
            HALCS_TRIGGER_MUX_RCV_SRC_MIN, HALCS_TRIGGER_MUX_RCV_SRC_MAX, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

#define HALCS_TRIGGER_MUX_RCV_IN_SEL_MIN                       0              /* Minimum selection */
#define HALCS_TRIGGER_MUX_RCV_IN_SEL_MAX                       ((1 << 8) -1)  /* Maximum selection */
RW_PARAM_FUNC(trigger_mux, rcv_in_sel) {
    SET_GET_PARAM_CHANNEL(trigger_mux, 0x0, WB_TRIG_MUX,
            CH0_CTL, RCV_IN_SEL, TRIGGER_MUX_CHAN_OFFSET, TRIGGER_MUX_NUM_CHAN, MULT_BIT_PARAM,
            HALCS_TRIGGER_MUX_RCV_IN_SEL_MIN, HALCS_TRIGGER_MUX_RCV_IN_SEL_MAX, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

#define HALCS_TRIGGER_MUX_TRANSM_SRC_MIN                       0 /* Trigger Source Selection (= Trigger Backplane) */
#define HALCS_TRIGGER_MUX_TRANSM_SRC_MAX                       1 /* Trigger Source Selection (= FPGA internal) */
RW_PARAM_FUNC(trigger_mux, transm_src) {
    SET_GET_PARAM_CHANNEL(trigger_mux, 0x0, WB_TRIG_MUX,
            CH0_CTL, TRANSM_SRC, TRIGGER_MUX_CHAN_OFFSET, TRIGGER_MUX_NUM_CHAN, SINGLE_BIT_PARAM,
            HALCS_TRIGGER_MUX_TRANSM_SRC_MIN, HALCS_TRIGGER_MUX_TRANSM_SRC_MAX, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

#define HALCS_TRIGGER_MUX_TRANSM_OUT_SEL_MIN                   0              /* Minimum selection */
#define HALCS_TRIGGER_MUX_TRANSM_OUT_SEL_MAX                   ((1 << 8) -1)  /* Maximum selection */
RW_PARAM_FUNC(trigger_mux, transm_out_sel) {
    SET_GET_PARAM_CHANNEL(trigger_mux, 0x0, WB_TRIG_MUX,
            CH0_CTL, TRANSM_OUT_SEL, TRIGGER_MUX_CHAN_OFFSET, TRIGGER_MUX_NUM_CHAN, MULT_BIT_PARAM,
            HALCS_TRIGGER_MUX_TRANSM_OUT_SEL_MIN, HALCS_TRIGGER_MUX_TRANSM_OUT_SEL_MAX, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

/* Exported function pointers */
const disp_table_func_fp trigger_mux_exp_fp [] = {
    RW_PARAM_FUNC_NAME(trigger_mux, rcv_src),
    RW_PARAM_FUNC_NAME(trigger_mux, rcv_in_sel),
    RW_PARAM_FUNC_NAME(trigger_mux, transm_src),
    RW_PARAM_FUNC_NAME(trigger_mux, transm_out_sel),
    NULL
};

/************************************************************/
/***************** Export methods functions *****************/
/************************************************************/

static smio_err_e _trigger_mux_do_op (void *owner, void *msg);

/* Attach an instance of sm_io to dev_io function pointer */
smio_err_e trigger_mux_attach (smio_t *self, void *args)
{
    UNUSED(self);
    UNUSED(args);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Deattach an instance of sm_io to dev_io function pointer */
smio_err_e trigger_mux_deattach (smio_t *self)
{
    UNUSED(self);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Export (register) sm_io to handle operations function pointer */
smio_err_e trigger_mux_export_ops (smio_t *self,
        const disp_op_t** smio_exp_ops)
{
    UNUSED(self);
    UNUSED(smio_exp_ops);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Unexport (unregister) sm_io to handle operations function pointer */
smio_err_e trigger_mux_unexport_ops (smio_t *self)
{
    UNUSED(self);
    return SMIO_ERR_FUNC_NOT_IMPL;
}


/* Generic wrapper for receiving opcodes and arguments to specific funtions function pointer */
/* FIXME: Code repetition! _devio_do_smio_op () function does almost the same!!! */
smio_err_e _trigger_mux_do_op (void *owner, void *msg)
{
    UNUSED(owner);
    UNUSED(msg);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

smio_err_e trigger_mux_do_op (void *self, void *msg)
{
    return _trigger_mux_do_op (self, msg);
}

const smio_ops_t trigger_mux_ops = {
    .attach             = &trigger_mux_attach,          /* Attach sm_io instance to dev_io */
    .deattach           = &trigger_mux_deattach,        /* Deattach sm_io instance to dev_io */
    .export_ops         = &trigger_mux_export_ops,      /* Export sm_io operations to dev_io */
    .unexport_ops       = &trigger_mux_unexport_ops,    /* Unexport sm_io operations to dev_io */
    .do_op              = &trigger_mux_do_op            /* Generic wrapper for handling specific operations */
};

/************************************************************/
/****************** Bootstrap Operations ********************/
/************************************************************/

smio_err_e trigger_mux_init (smio_t * self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:trigger_mux_exp] Initializing trigger_mux\n");

    smio_err_e err = SMIO_SUCCESS;

    err = smio_set_id (self, TRIGGER_MUX_SDB_DEVID);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO id", err_set_id);
    err = smio_set_name (self, TRIGGER_MUX_SDB_NAME);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO name", err_set_name);

    /* Set SMIO ops pointers */
    err = smio_set_ops (self, &trigger_mux_ops);
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
    err = smio_init_exp_ops (self, (disp_op_t **) trigger_mux_exp_ops,
            trigger_mux_exp_fp);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not fill SMIO "
            "function descriptors with the callbacks", err_fill_desc);

    err = smio_set_exp_ops (self, trigger_mux_exp_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO exported operations",
            err_smio_set_exp_ops);

    /* Initialize specific structure */
    smio_trigger_mux_t *smio_handler = smio_trigger_mux_new (self);
    ASSERT_ALLOC(smio_handler, err_smio_handler_alloc, SMIO_ERR_ALLOC);
    err = smio_set_handler (self, smio_handler);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO handler",
            err_smio_set_handler);

    return err;

err_smio_set_handler:
    smio_trigger_mux_destroy (&smio_handler);
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

/* Destroy sm_io instance of trigger_mux */
smio_err_e trigger_mux_shutdown (smio_t *self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:trigger_mux_exp] Shutting down trigger_mux\n");

    smio_err_e err = SMIO_SUCCESS;
    smio_trigger_mux_t *trig_iface = smio_get_handler (self);
    ASSERT_TEST(trig_iface != NULL, "Could not get trig_iface handler",
            err_trig_iface_handler, SMIO_ERR_ALLOC /* FIXME: improve return code */);

    /* Destroy SMIO instance */
    smio_trigger_mux_destroy (&trig_iface);
    /* Nullify operation pointers */
    smio_set_exp_ops (self, NULL);
    smio_set_thsafe_client_ops (self, NULL);
    smio_set_ops (self, NULL);

err_trig_iface_handler:
    return err;
}

const smio_bootstrap_ops_t trigger_mux_bootstrap_ops = {
    .init            = &trigger_mux_init,
    .shutdown        = &trigger_mux_shutdown,
    .config_defaults = &trigger_mux_config_defaults
};

SMIO_MOD_DECLARE(TRIGGER_MUX_SDB_DEVID, TRIGGER_MUX_SDB_NAME, trigger_mux_bootstrap_ops)
