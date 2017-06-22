/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "halcs_server.h"
/* Private headers */
#include "sm_io_swap_codes.h"
#include "sm_io_swap_defaults.h"
#include "sm_io_swap_exports.h"
#include "sm_io_swap_core.h"
#include "sm_io_swap_exp.h"
#include "hw/wb_bpm_swap_regs.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, SM_IO, "[sm_io:swap_exp]",    \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)   \
    ASSERT_HAL_ALLOC(ptr, SM_IO, "[sm_io:swap_exp]",            \
            smio_err_str(SMIO_ERR_ALLOC),                       \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                \
    CHECK_HAL_ERR(err, SM_IO, "[sm_io:swap_exp]",               \
            smio_err_str (err_type))

/************************************************************/
/*************    Specific SWAP operations     **************/
/************************************************************/
/* 0: Outputs matched. No signal
 * 1: Outputs in direct path
 * 2: Outputs in inverted path
 * 3: Outputs switching
 * */
#define SW_MIN                                  0
#define SW_MAX                                  3
RW_PARAM_FUNC(swap, sw) {
    SET_GET_PARAM(swap, 0x0, BPM_SWAP, CTRL, MODE, MULT_BIT_PARAM,
            SW_MIN, SW_MAX, NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define BPM_SWAP_DIV_F_MIN                      1
#define BPM_SWAP_DIV_F_MAX                      ((1<<16)-1)
RW_PARAM_FUNC(swap, div_clk) {
    SET_GET_PARAM(swap, 0x0, BPM_SWAP, CTRL, SWAP_DIV_F, MULT_BIT_PARAM,
            BPM_SWAP_DIV_F_MIN, BPM_SWAP_DIV_F_MAX, NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

/* Number of clock cycles between the switching and deswitching */
#define BPM_SWAP_SW_DLY_MIN                     0
#define BPM_SWAP_SW_DLY_MAX                     ((1<<16)-1)
RW_PARAM_FUNC(swap, sw_dly) {
    SET_GET_PARAM(swap, 0x0, BPM_SWAP, DLY, DESWAP, MULT_BIT_PARAM,
            BPM_SWAP_SW_DLY_MIN, BPM_SWAP_SW_DLY_MAX, NO_CHK_FUNC, NO_FMT_FUNC,
            SET_FIELD);
}

/* Exported function pointers */
const disp_table_func_fp swap_exp_fp [] = {
    RW_PARAM_FUNC_NAME(swap, sw),
    RW_PARAM_FUNC_NAME(swap, div_clk),
    RW_PARAM_FUNC_NAME(swap, sw_dly),
    NULL
};

/************************************************************/
/***************** Export methods functions *****************/
/************************************************************/

static smio_err_e _swap_do_op (void *owner, void *msg);

/* Attach an instance of sm_io to dev_io function pointer */
smio_err_e swap_attach (smio_t *self, void *args)
{
    UNUSED(self);
    UNUSED(args);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Deattach an instance of sm_io to dev_io function pointer */
smio_err_e swap_deattach (smio_t *self)
{
    UNUSED(self);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Export (register) sm_io to handle operations function pointer */
smio_err_e swap_export_ops (smio_t *self,
        const disp_op_t **smio_exp_ops)
{
    UNUSED(self);
    UNUSED(smio_exp_ops);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Unexport (unregister) sm_io to handle operations function pointer */
smio_err_e swap_unexport_ops (smio_t *self)
{
    UNUSED(self);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

smio_err_e _swap_do_mgmt_op (void *owner, void *msg)
{
    assert (owner);
    assert (msg);

    smio_err_e err = SMIO_SUCCESS;
    SMIO_OWNER_TYPE *self = SMIO_EXP_OWNER(owner);
    GEN_MSG_ZMQ_TYPE *recv_msg = GEN_MSG_ZMQ(msg);
    uint32_t my_inst_id = smio_get_inst_id (self);

    /* We expect the following */
    /*
     * Arg1:    (uint32_t) smio_id
     * Arg2:    (uint64_t) base
     * Arg3:    (uint32_t) inst_id
     * Arg4:    (uint32_t) dest_smio_id
     * Arg5:    (uint32_t) dest_inst_id
     * Arg6:    (string)   message
     * */
    STR_MSG_ZMQ_TYPE *smio_id_str = STR_MSG_ZMQ_FIRST_ARG(recv_msg);
    STR_MSG_ZMQ_TYPE *base_str = STR_MSG_ZMQ_NEXT_ARG(recv_msg);
    STR_MSG_ZMQ_TYPE *inst_id_str = STR_MSG_ZMQ_NEXT_ARG(recv_msg);
    STR_MSG_ZMQ_TYPE *dest_smio_id_str = STR_MSG_ZMQ_NEXT_ARG(recv_msg);
    STR_MSG_ZMQ_TYPE * dest_inst_id_str = STR_MSG_ZMQ_NEXT_ARG(recv_msg);
    STR_MSG_ZMQ_TYPE *mgmt_msg_zmq_str = STR_MSG_ZMQ_NEXT_ARG(recv_msg);

    /* Conversions */
    uint32_t inst_id = strtol(inst_id_str, NULL, 10);

    /* Check if FMC_ACTIVE_CLK SMIO sent this message */
    if (inst_id == my_inst_id && streq (mgmt_msg_zmq_str, "INIT_OK")) {
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:_swap_do_mgmt_op] Resetting module\n");
            SET_PARAM(self, swap, 0x0, BPM_SWAP, CTRL,
                    SWAP_DIV_F, MULT_BIT_PARAM, SWAP_DFLT_DIV_CLK, /* min */, /* max */,
                    NO_CHK_FUNC, SET_FIELD);
            SET_PARAM(self, swap, 0x0, BPM_SWAP, CTRL,
                    MODE, MULT_BIT_PARAM, SWAP_DFLT_SW, /* min */, /* max */,
                    NO_CHK_FUNC, SET_FIELD);
            SET_PARAM(self, swap, 0x0, BPM_SWAP, DLY,
                    DESWAP, MULT_BIT_PARAM, SWAP_DFLT_SW_DLY, /* min */, /* max */,
                    NO_CHK_FUNC, SET_FIELD);
    }
    else {
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_WARN, "[sm_io:_swap_do_mgmt_op] Unexpected message received\n");
    }

    /* Clenup strings */
    STR_MSG_ZMQ_CLENUP_ARG(smio_id_str);
    STR_MSG_ZMQ_CLENUP_ARG(base_str);
    STR_MSG_ZMQ_CLENUP_ARG(inst_id_str);
    STR_MSG_ZMQ_CLENUP_ARG(dest_smio_id_str);
    STR_MSG_ZMQ_CLENUP_ARG(dest_inst_id_str);
    STR_MSG_ZMQ_CLENUP_ARG(mgmt_msg_zmq_str);

    return err;
}

/* Generic wrapper for receiving opcodes and arguments to specific funtions function pointer */
/* FIXME: Code repetition! _devio_do_smio_op () function does almost the same!!! */
smio_err_e _swap_do_op (void *owner, void *msg)
{
    UNUSED(owner);
    UNUSED(msg);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

smio_err_e swap_do_op (void *self, void *msg)
{
    return _swap_do_op (self, msg);
}

smio_err_e swap_do_mgmt_op (void *self, void *msg)
{
    return _swap_do_mgmt_op (self, msg);
}

const smio_ops_t swap_ops = {
    .attach             = &swap_attach,          /* Attach sm_io instance to dev_io */
    .deattach           = &swap_deattach,        /* Deattach sm_io instance to dev_io */
    .export_ops         = &swap_export_ops,      /* Export sm_io operations to dev_io */
    .unexport_ops       = &swap_unexport_ops,    /* Unexport sm_io operations to dev_io */
    .do_op              = &swap_do_op,           /* Generic wrapper for handling specific operations */
    .do_mgmt_op         = &swap_do_mgmt_op       /* Generic wrapper for handling internal SMIO operations */
};

/************************************************************/
/****************** Bootstrap Operations ********************/
/************************************************************/

smio_err_e swap_init (smio_t * self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:swap_exp] Initializing swap\n");

    smio_err_e err = SMIO_SUCCESS;

    err = smio_set_id (self, SWAP_SDB_DEVID);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO id", err_set_id);
    err = smio_set_name (self, SWAP_SDB_NAME);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO name", err_set_name);

    /* Set SMIO ops pointers */
    err = smio_set_ops (self, &swap_ops);
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
    err = smio_init_exp_ops (self, (disp_op_t **) swap_exp_ops, swap_exp_fp);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not fill SMIO "
            "function descriptors with the callbacks", err_fill_desc);

    err = smio_set_exp_ops (self, swap_exp_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO exported operations",
            err_smio_set_exp_ops);

    /* Initialize specific structure */
    smio_swap_t *smio_handler = smio_swap_new (self);
    ASSERT_ALLOC(smio_handler, err_smio_handler_alloc, SMIO_ERR_ALLOC);
    err = smio_set_handler (self, smio_handler);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO handler",
            err_smio_set_handler);

    return err;

err_smio_set_handler:
    smio_swap_destroy (&smio_handler);
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

/* Destroy sm_io instance of swap */
smio_err_e swap_shutdown (smio_t *self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:swap_exp] Shutting down swap\n");

    smio_err_e err = SMIO_SUCCESS;
    smio_swap_t *swap = smio_get_handler (self);
    ASSERT_TEST(swap != NULL, "Could not get SWAP handler",
            err_swap_handler, SMIO_ERR_ALLOC /* FIXME: improve return code */);

    /* Destroy SMIO instance */
    smio_swap_destroy (&swap);
    /* Nullify operation pointers */
    smio_set_exp_ops (self, NULL);
    smio_set_thsafe_client_ops (self, NULL);
    smio_set_ops (self, NULL);

err_swap_handler:
    return err;
}

const smio_bootstrap_ops_t swap_bootstrap_ops = {
    .init            = &swap_init,
    .shutdown        = &swap_shutdown,
    .config_defaults = &swap_config_defaults
};

SMIO_MOD_DECLARE(SWAP_SDB_DEVID, SWAP_SDB_NAME, swap_bootstrap_ops)
