/*
 * Copyright (C) 2016 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "halcs_server.h"
/* Private headers */
#include "sm_io_trigger_iface_codes.h"
#include "sm_io_trigger_iface_defaults.h"
#include "sm_io_trigger_iface_exports.h"
#include "sm_io_trigger_iface_core.h"
#include "sm_io_trigger_iface_exp.h"
#include "hw/wb_trigger_iface_regs.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...)  \
    ASSERT_HAL_TEST(test_boolean, SM_IO, "[sm_io:trigger_iface_exp]",           \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)                   \
    ASSERT_HAL_ALLOC(ptr, SM_IO, "[sm_io:trigger_iface_exp]",                   \
            smio_err_str(SMIO_ERR_ALLOC),                                       \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                                \
    CHECK_HAL_ERR(err, SM_IO, "[sm_io:trigger_iface_exp]",                      \
            smio_err_str (err_type))

/* This must match the FPGA maximum number of channels */
#define TRIGGER_IFACE_NUM_CHAN                          24
#define TRIGGER_IFACE_CHAN_OFFSET                       0x00c /* 3 32-bit registers */

/*****************************************************************/
/************ Specific TRIGGER INTERFACE Operations **************/
/*****************************************************************/

#define HALCS_TRIGGER_IFACE_DIR_MIN                       0 /* Bidirection Buffer set to Output */
#define HALCS_TRIGGER_IFACE_DIR_MAX                       1 /* Bidirection Buffer set to Input */
RW_PARAM_FUNC(trigger_iface, dir) {
    SET_GET_PARAM_CHANNEL(trigger_iface, 0x0, WB_TRIG_IFACE,
            CH0_CTL, DIR, TRIGGER_IFACE_CHAN_OFFSET, TRIGGER_IFACE_NUM_CHAN, SINGLE_BIT_PARAM,
            HALCS_TRIGGER_IFACE_DIR_MIN, HALCS_TRIGGER_IFACE_DIR_MAX, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

#define HALCS_TRIGGER_IFACE_DIR_POL_MIN                   0 /* Direction polarity kept */
#define HALCS_TRIGGER_IFACE_DIR_POL_MAX                   1 /* Direction polarity reversed */
RW_PARAM_FUNC(trigger_iface, dir_pol) {
    SET_GET_PARAM_CHANNEL(trigger_iface, 0x0, WB_TRIG_IFACE,
            CH0_CTL, DIR_POL, TRIGGER_IFACE_CHAN_OFFSET, TRIGGER_IFACE_NUM_CHAN, SINGLE_BIT_PARAM,
            HALCS_TRIGGER_IFACE_DIR_POL_MIN, HALCS_TRIGGER_IFACE_DIR_POL_MAX, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

#define HALCS_TRIGGER_IFACE_RCV_COUNT_RST_MIN             0 /* Receive Counter Reset */
#define HALCS_TRIGGER_IFACE_RCV_COUNT_RST_MAX             1 /* Receive Counter Reset */
RW_PARAM_FUNC(trigger_iface, rcv_count_rst) {
    SET_GET_PARAM_CHANNEL(trigger_iface, 0x0, WB_TRIG_IFACE,
            CH0_CTL, RCV_COUNT_RST, TRIGGER_IFACE_CHAN_OFFSET, TRIGGER_IFACE_NUM_CHAN, SINGLE_BIT_PARAM,
            HALCS_TRIGGER_IFACE_RCV_COUNT_RST_MIN, HALCS_TRIGGER_IFACE_RCV_COUNT_RST_MAX, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

#define HALCS_TRIGGER_IFACE_TRANSM_COUNT_RST_MIN          0 /* Transmit Counter Reset */
#define HALCS_TRIGGER_IFACE_TRANSM_COUNT_RST_MAX          1 /* Transmit Counter Reset */
RW_PARAM_FUNC(trigger_iface, transm_count_rst) {
    SET_GET_PARAM_CHANNEL(trigger_iface, 0x0, WB_TRIG_IFACE,
            CH0_CTL, TRANSM_COUNT_RST, TRIGGER_IFACE_CHAN_OFFSET, TRIGGER_IFACE_NUM_CHAN, SINGLE_BIT_PARAM,
            HALCS_TRIGGER_IFACE_TRANSM_COUNT_RST_MIN, HALCS_TRIGGER_IFACE_TRANSM_COUNT_RST_MAX, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

#define HALCS_TRIGGER_IFACE_RCV_LEN_MIN                   0               /* Receiver Debounce Length */
#define HALCS_TRIGGER_IFACE_RCV_LEN_MAX                   ((1 << 8) -1)   /* Receiver Debounce Length */
RW_PARAM_FUNC(trigger_iface, rcv_len) {
    SET_GET_PARAM_CHANNEL(trigger_iface, 0x0, WB_TRIG_IFACE,
            CH0_CFG, RCV_LEN, TRIGGER_IFACE_CHAN_OFFSET, TRIGGER_IFACE_NUM_CHAN, MULT_BIT_PARAM,
            HALCS_TRIGGER_IFACE_RCV_LEN_MIN, HALCS_TRIGGER_IFACE_RCV_LEN_MAX, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

#define HALCS_TRIGGER_IFACE_TRANSM_LEN_MIN                 0              /* Receiver Debounce Length */
#define HALCS_TRIGGER_IFACE_TRANSM_LEN_MAX                 ((1 << 8) -1)  /* Receiver Debounce Length */
RW_PARAM_FUNC(trigger_iface, transm_len) {
    SET_GET_PARAM_CHANNEL(trigger_iface, 0x0, WB_TRIG_IFACE,
            CH0_CFG, TRANSM_LEN, TRIGGER_IFACE_CHAN_OFFSET, TRIGGER_IFACE_NUM_CHAN, MULT_BIT_PARAM,
            /* no minimum value */, /* no maximum value */, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(trigger_iface, count_rcv) {
    SET_GET_PARAM_CHANNEL(trigger_iface, 0x0, WB_TRIG_IFACE,
            CH0_COUNT, RCV, TRIGGER_IFACE_CHAN_OFFSET, TRIGGER_IFACE_NUM_CHAN, MULT_BIT_PARAM,
            /* no minimum value */, /* no maximum value */, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(trigger_iface, count_transm) {
    SET_GET_PARAM_CHANNEL(trigger_iface, 0x0, WB_TRIG_IFACE,
            CH0_COUNT, TRANSM, TRIGGER_IFACE_CHAN_OFFSET, TRIGGER_IFACE_NUM_CHAN, MULT_BIT_PARAM,
            /* no minimum value */, /* no maximum value */, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

/* Exported function pointers */
const disp_table_func_fp trigger_iface_exp_fp [] = {
    RW_PARAM_FUNC_NAME(trigger_iface, dir),
    RW_PARAM_FUNC_NAME(trigger_iface, dir_pol),
    RW_PARAM_FUNC_NAME(trigger_iface, rcv_count_rst),
    RW_PARAM_FUNC_NAME(trigger_iface, transm_count_rst),
    RW_PARAM_FUNC_NAME(trigger_iface, rcv_len),
    RW_PARAM_FUNC_NAME(trigger_iface, transm_len),
    RW_PARAM_FUNC_NAME(trigger_iface, count_rcv),
    RW_PARAM_FUNC_NAME(trigger_iface, count_transm),
    NULL
};

/************************************************************/
/***************** Export methods functions *****************/
/************************************************************/

static smio_err_e _trigger_iface_do_op (void *owner, void *msg);

/* Attach an instance of sm_io to dev_io function pointer */
smio_err_e trigger_iface_attach (smio_t *self, devio_t *parent)
{
    (void) self;
    (void) parent;
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Deattach an instance of sm_io to dev_io function pointer */
smio_err_e trigger_iface_deattach (smio_t *self)
{
    (void) self;
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Export (register) sm_io to handle operations function pointer */
smio_err_e trigger_iface_export_ops (smio_t *self,
        const disp_op_t** smio_exp_ops)
{
    (void) self;
    (void) smio_exp_ops;
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Unexport (unregister) sm_io to handle operations function pointer */
smio_err_e trigger_iface_unexport_ops (smio_t *self)
{
    (void) self;
    return SMIO_ERR_FUNC_NOT_IMPL;
}


/* Generic wrapper for receiving opcodes and arguments to specific funtions function pointer */
/* FIXME: Code repetition! _devio_do_smio_op () function does almost the same!!! */
smio_err_e _trigger_iface_do_op (void *owner, void *msg)
{
    (void) owner;
    (void) msg;
    return SMIO_ERR_FUNC_NOT_IMPL;
}

smio_err_e trigger_iface_do_op (void *self, void *msg)
{
    return _trigger_iface_do_op (self, msg);
}

const smio_ops_t trigger_iface_ops = {
    .attach             = trigger_iface_attach,          /* Attach sm_io instance to dev_io */
    .deattach           = trigger_iface_deattach,        /* Deattach sm_io instance to dev_io */
    .export_ops         = trigger_iface_export_ops,      /* Export sm_io operations to dev_io */
    .unexport_ops       = trigger_iface_unexport_ops,    /* Unexport sm_io operations to dev_io */
    .do_op              = trigger_iface_do_op            /* Generic wrapper for handling specific operations */
};

/************************************************************/
/****************** Bootstrap Operations ********************/
/************************************************************/

smio_err_e trigger_iface_init (smio_t * self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:trigger_iface_exp] Initializing trigger_iface\n");

    smio_err_e err = SMIO_SUCCESS;

    err = smio_set_id (self, TRIGGER_IFACE_SDB_DEVID);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO id", err_set_id);
    err = smio_set_name (self, TRIGGER_IFACE_SDB_NAME);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO name", err_set_name);

    /* Set SMIO ops pointers */
    err = smio_set_ops (self, &trigger_iface_ops);
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
    err = smio_init_exp_ops (self, (disp_op_t **) trigger_iface_exp_ops,
            trigger_iface_exp_fp);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not fill SMIO "
            "function descriptors with the callbacks", err_fill_desc);

    err = smio_set_exp_ops (self, trigger_iface_exp_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO exported operations",
            err_smio_set_exp_ops);

    /* Initialize specific structure */
    smio_trigger_iface_t *smio_handler = smio_trigger_iface_new (self);
    ASSERT_ALLOC(smio_handler, err_smio_handler_alloc, SMIO_ERR_ALLOC);
    err = smio_set_handler (self, smio_handler);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO handler",
            err_smio_set_handler);

    return err;

err_smio_set_handler:
    smio_trigger_iface_destroy (&smio_handler);
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

/* Destroy sm_io instance of trigger_iface */
smio_err_e trigger_iface_shutdown (smio_t *self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:trigger_iface_exp] Shutting down trigger_iface\n");

    smio_err_e err = SMIO_SUCCESS;
    smio_trigger_iface_t *trig_iface = smio_get_handler (self);
    ASSERT_TEST(trig_iface != NULL, "Could not get trig_iface handler",
            err_trig_iface_handler, SMIO_ERR_ALLOC /* FIXME: improve return code */);

    /* Destroy SMIO instance */
    smio_trigger_iface_destroy (&trig_iface);
    /* Nullify operation pointers */
    smio_set_exp_ops (self, NULL);
    smio_set_thsafe_client_ops (self, NULL);
    smio_set_ops (self, NULL);

err_trig_iface_handler:
    return err;
}

const smio_bootstrap_ops_t trigger_iface_bootstrap_ops = {
    .init = trigger_iface_init,
    .shutdown = trigger_iface_shutdown,
    .config_defaults = trigger_iface_config_defaults
};

SMIO_MOD_DECLARE(TRIGGER_IFACE_SDB_DEVID, TRIGGER_IFACE_SDB_NAME, trigger_iface_bootstrap_ops)
