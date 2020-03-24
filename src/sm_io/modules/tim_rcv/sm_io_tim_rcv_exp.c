/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "halcs_server.h"
/* Private headers */
#include "sm_io_tim_rcv_codes.h"
#include "sm_io_tim_rcv_defaults.h"
#include "sm_io_tim_rcv_exports.h"
#include "sm_io_tim_rcv_core.h"
#include "sm_io_tim_rcv_exp.h"
#include "hw/wb_tim_rcv_core_regs.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...)  \
    ASSERT_HAL_TEST(test_boolean, SM_IO, "[sm_io:tim_rcv_exp]",                 \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)                   \
    ASSERT_HAL_ALLOC(ptr, SM_IO, "[sm_io:tim_rcv_exp]",                         \
            smio_err_str(SMIO_ERR_ALLOC),                                       \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                                \
    CHECK_HAL_ERR(err, SM_IO, "[sm_io:tim_rcv_exp]",                            \
            smio_err_str (err_type))

/************************************************************/
/************    Specific TIM_RCV operations     ************/
/************************************************************/

#define PHASE_MEAS_NAVG_MIN                         0
#define PHASE_MEAS_NAVG_MAX                         ((1ULL<<32)-1)
#define TIM_RCV_CORE_PHASE_MEAS_NAVG_R(val)         (val)
#define TIM_RCV_CORE_PHASE_MEAS_NAVG_W(val)         (val)
#define TIM_RCV_CORE_PHASE_MEAS_NAVG_MASK           ((1ULL<<32)-1)

RW_PARAM_FUNC(tim_rcv, phase_meas_navg) {
    SET_GET_PARAM(tim_rcv, 0x0, TIM_RCV_CORE, PHASE_MEAS_NAVG, /* no field */, MULT_BIT_PARAM,
            PHASE_MEAS_NAVG_MIN, PHASE_MEAS_NAVG_MAX, NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define DMTD_A_CTL_DEGLITCHER_THRES_MIN             0
#define DMTD_A_CTL_DEGLITCHER_THRES_MAX             ((1ULL<<16)-1)

RW_PARAM_FUNC(tim_rcv, dmtd_a_deglitcher_thres) {
    SET_GET_PARAM(tim_rcv, 0x0, TIM_RCV_CORE, DMTD_A_CTL, DEGLITCHER_THRES, MULT_BIT_PARAM,
            DMTD_A_CTL_DEGLITCHER_THRES_MIN, DMTD_A_CTL_DEGLITCHER_THRES_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define DMTD_B_CTL_DEGLITCHER_THRES_MIN             0
#define DMTD_B_CTL_DEGLITCHER_THRES_MAX             ((1ULL<<16)-1)

RW_PARAM_FUNC(tim_rcv, dmtd_b_deglitcher_thres) {
    SET_GET_PARAM(tim_rcv, 0x0, TIM_RCV_CORE, DMTD_B_CTL, DEGLITCHER_THRES, MULT_BIT_PARAM,
            DMTD_B_CTL_DEGLITCHER_THRES_MIN, DMTD_B_CTL_DEGLITCHER_THRES_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define PHASE_MEAS_MIN                              0
#define PHASE_MEAS_MAX                              ((1ULL<<32)-1)
#define TIM_RCV_CORE_PHASE_MEAS_R(val)              (val)
#define TIM_RCV_CORE_PHASE_MEAS_W(val)              (val)
#define TIM_RCV_CORE_PHASE_MEAS_MASK                ((1ULL<<32)-1)

RW_PARAM_FUNC(tim_rcv, phase_meas) {
    SET_GET_PARAM(tim_rcv, 0x0, TIM_RCV_CORE, PHASE_MEAS, /* no field */, MULT_BIT_PARAM,
            PHASE_MEAS_MIN, PHASE_MEAS_MAX, NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define F_DMTD_A_FREQ_MIN                             0
#define F_DMTD_A_FREQ_MAX                             ((1ULL<<28)-1)

RW_PARAM_FUNC(tim_rcv, dmtd_a_freq) {
    SET_GET_PARAM(tim_rcv, 0x0, TIM_RCV_CORE, F_DMTD_A, FREQ, MULT_BIT_PARAM,
            F_DMTD_A_FREQ_MIN, F_DMTD_A_FREQ_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define F_DMTD_A_VALID_MIN                             0
#define F_DMTD_A_VALID_MAX                             1

RW_PARAM_FUNC(tim_rcv, dmtd_a_valid) {
    SET_GET_PARAM(tim_rcv, 0x0, TIM_RCV_CORE, F_DMTD_A, VALID, SINGLE_BIT_PARAM,
            F_DMTD_A_VALID_MIN, F_DMTD_A_VALID_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define F_DMTD_B_FREQ_MIN                             0
#define F_DMTD_B_FREQ_MAX                             ((1ULL<<28)-1)

RW_PARAM_FUNC(tim_rcv, dmtd_b_freq) {
    SET_GET_PARAM(tim_rcv, 0x0, TIM_RCV_CORE, F_DMTD_B, FREQ, MULT_BIT_PARAM,
            F_DMTD_B_FREQ_MIN, F_DMTD_B_FREQ_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define F_DMTD_B_VALID_MIN                             0
#define F_DMTD_B_VALID_MAX                             1

RW_PARAM_FUNC(tim_rcv, dmtd_b_valid) {
    SET_GET_PARAM(tim_rcv, 0x0, TIM_RCV_CORE, F_DMTD_B, VALID, SINGLE_BIT_PARAM,
            F_DMTD_B_VALID_MIN, F_DMTD_B_VALID_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}


/* Exported function pointers */
const disp_table_func_fp tim_rcv_exp_fp [] = {
    RW_PARAM_FUNC_NAME(tim_rcv, phase_meas_navg),
    RW_PARAM_FUNC_NAME(tim_rcv, dmtd_a_deglitcher_thres),
    RW_PARAM_FUNC_NAME(tim_rcv, dmtd_b_deglitcher_thres),
    RW_PARAM_FUNC_NAME(tim_rcv, phase_meas),
    RW_PARAM_FUNC_NAME(tim_rcv, dmtd_a_freq),
    RW_PARAM_FUNC_NAME(tim_rcv, dmtd_a_valid),
    RW_PARAM_FUNC_NAME(tim_rcv, dmtd_b_freq),
    RW_PARAM_FUNC_NAME(tim_rcv, dmtd_b_valid),
    NULL
};

/************************************************************/
/***************** Export methods functions *****************/
/************************************************************/

static smio_err_e _tim_rcv_do_op (void *owner, void *msg);

/* Attach an instance of sm_io to dev_io function pointer */
smio_err_e tim_rcv_attach (smio_t *self, void *args)
{
    UNUSED(self);
    UNUSED(args);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Deattach an instance of sm_io to dev_io function pointer */
smio_err_e tim_rcv_deattach (smio_t *self)
{
    UNUSED(self);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Export (register) sm_io to handle operations function pointer */
smio_err_e tim_rcv_export_ops (smio_t *self,
        const disp_op_t **smio_exp_ops)
{
    UNUSED(self);
    UNUSED(smio_exp_ops);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Unexport (unregister) sm_io to handle operations function pointer */
smio_err_e tim_rcv_unexport_ops (smio_t *self)
{
    UNUSED(self);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

smio_err_e _tim_rcv_do_mgmt_op (void *owner, void *msg)
{
    UNUSED (owner);
    UNUSED (msg);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Generic wrapper for receiving opcodes and arguments to specific funtions function pointer */
/* FIXME: Code repetition! _devio_do_smio_op () function does almost the same!!! */
smio_err_e _tim_rcv_do_op (void *owner, void *msg)
{
    UNUSED(owner);
    UNUSED(msg);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

smio_err_e tim_rcv_do_op (void *self, void *msg)
{
    return _tim_rcv_do_op (self, msg);
}

smio_err_e tim_rcv_do_mgmt_op (void *self, void *msg)
{
    return _tim_rcv_do_mgmt_op (self, msg);
}

const smio_ops_t tim_rcv_ops = {
    .attach             = &tim_rcv_attach,          /* Attach sm_io instance to dev_io */
    .deattach           = &tim_rcv_deattach,        /* Deattach sm_io instance to dev_io */
    .export_ops         = &tim_rcv_export_ops,      /* Export sm_io operations to dev_io */
    .unexport_ops       = &tim_rcv_unexport_ops,    /* Unexport sm_io operations to dev_io */
    .do_op              = &tim_rcv_do_op,           /* Generic wrapper for handling specific operations */
    .do_mgmt_op         = &tim_rcv_do_mgmt_op       /* Generic wrapper for handling internal SMIO operations */
};

/************************************************************/
/****************** Bootstrap Operations ********************/
/************************************************************/

smio_err_e tim_rcv_init (smio_t * self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:tim_rcv_exp] Initializing tim_rcv\n");

    smio_err_e err = SMIO_SUCCESS;

    err = smio_set_id (self, TIM_RCV_SDB_DEVID);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO id", err_set_id);
    err = smio_set_name (self, TIM_RCV_SDB_NAME);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO name", err_set_name);

    /* Set SMIO ops pointers */
    err = smio_set_ops (self, &tim_rcv_ops);
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
    err = smio_init_exp_ops (self, (disp_op_t **) tim_rcv_exp_ops, tim_rcv_exp_fp);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not fill SMIO "
            "function descriptors with the callbacks", err_fill_desc);

    err = smio_set_exp_ops (self, tim_rcv_exp_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO exported operations",
            err_smio_set_exp_ops);

    /* Initialize specific structure */
    smio_tim_rcv_t *smio_handler = smio_tim_rcv_new (self);
    ASSERT_ALLOC(smio_handler, err_smio_handler_alloc, SMIO_ERR_ALLOC);
    err = smio_set_handler (self, smio_handler);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO handler",
            err_smio_set_handler);

    return err;

err_smio_set_handler:
    smio_tim_rcv_destroy (&smio_handler);
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

/* Destroy sm_io instance of tim_rcv */
smio_err_e tim_rcv_shutdown (smio_t *self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:tim_rcv_exp] Shutting down tim_rcv\n");

    smio_err_e err = SMIO_SUCCESS;
    smio_tim_rcv_t *tim_rcv = smio_get_handler (self);
    ASSERT_TEST(tim_rcv != NULL, "Could not get TIM_RCV handler",
            err_tim_rcv_handler, SMIO_ERR_ALLOC /* FIXME: improve return code */);

    /* Destroy SMIO instance */
    smio_tim_rcv_destroy (&tim_rcv);
    /* Nullify operation pointers */
    smio_set_exp_ops (self, NULL);
    smio_set_thsafe_client_ops (self, NULL);
    smio_set_ops (self, NULL);

err_tim_rcv_handler:
    return err;
}

const smio_bootstrap_ops_t tim_rcv_bootstrap_ops = {
    .init            = &tim_rcv_init,
    .shutdown        = &tim_rcv_shutdown,
    .config_defaults = &tim_rcv_config_defaults
};

SMIO_MOD_DECLARE(TIM_RCV_SDB_DEVID, TIM_RCV_SDB_NAME, tim_rcv_bootstrap_ops)
