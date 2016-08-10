/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Joao Brito <joao.brito@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "bpm_server.h"
/* Private headers */
#include "sm_io_afc_timing_codes.h"
#include "sm_io_afc_timing_defaults.h"
//#include "sm_io_afc_timing_useful_macros.h"
#include "sm_io_afc_timing_exports.h"
#include "sm_io_afc_timing_core.h"
#include "sm_io_afc_timing_exp.h"
#include "hw/wb_slave_afc_timing_regs.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, SM_IO, "[sm_io:afc_timing_exp]",    \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)   \
    ASSERT_HAL_ALLOC(ptr, SM_IO, "[sm_io:afc_timing_exp]",            \
            smio_err_str(SMIO_ERR_ALLOC),                       \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                \
    CHECK_HAL_ERR(err, SM_IO, "[sm_io:afc_timing_exp]",               \
            smio_err_str (err_type))

/************************************************************/
/*************    Specific TIMING operations     **************/
/************************************************************/
RW_PARAM_FUNC(afc_timing, link_status) {
	SET_GET_PARAM(afc_timing, 0x0, TIMING, STAT, LINK, SINGLE_BIT_PARAM,
            /*read only*/,/*read only*/ , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, rxen_status) {
	SET_GET_PARAM(afc_timing, 0x0, TIMING, STAT, RXEN, SINGLE_BIT_PARAM,
            /*read only*/,/*read only*/ , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, ref_clk_locked) {
	SET_GET_PARAM(afc_timing, 0x0, TIMING, STAT, REFCLKLOCK, SINGLE_BIT_PARAM,
            /*read only*/,/*read only*/ , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

/****************** Event receiver configuration ******************/
/* Custom parameter check */
static int _rw_afc_timing_evt_code_chk (uint32_t evt_code)
{
    if (evt_code==0x7F || evt_code==0x7E || evt_code==0x70) {
        return PARAM_ERR;
    }

    return PARAM_OK;
}

rw_param_check_fp rw_afc_timing_evt_code_chk_fp = _rw_afc_timing_evt_code_chk;

/* Event code monitored by each channel */
#define TIMING_EVT_IN0_GLOBAL_MASK                  (0xffffffff)
#define TIMING_EVT_IN0_GLOBAL_W(val)                (val)
#define TIMING_EVT_IN0_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, evt_code_0) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, EVT_IN0, GLOBAL, MULT_BIT_PARAM,
            , , rw_afc_timing_evt_code_chk_fp, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_EVT_IN1_GLOBAL_MASK                  (0xffffffff)
#define TIMING_EVT_IN1_GLOBAL_W(val)                (val)
#define TIMING_EVT_IN1_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, evt_code_1) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, EVT_IN1, GLOBAL, MULT_BIT_PARAM,
            , , rw_afc_timing_evt_code_chk_fp, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_EVT_IN2_GLOBAL_MASK                  (0xffffffff)
#define TIMING_EVT_IN2_GLOBAL_W(val)                (val)
#define TIMING_EVT_IN2_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, evt_code_2) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, EVT_IN2, GLOBAL, MULT_BIT_PARAM,
            , , rw_afc_timing_evt_code_chk_fp, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_EVT_IN3_GLOBAL_MASK                  (0xffffffff)
#define TIMING_EVT_IN3_GLOBAL_W(val)                (val)
#define TIMING_EVT_IN3_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, evt_code_3) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, EVT_IN3, GLOBAL, MULT_BIT_PARAM,
            , , rw_afc_timing_evt_code_chk_fp, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_EVT_IN4_GLOBAL_MASK                  (0xffffffff)
#define TIMING_EVT_IN4_GLOBAL_W(val)                (val)
#define TIMING_EVT_IN4_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, evt_code_4) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, EVT_IN4, GLOBAL, MULT_BIT_PARAM,
            , , rw_afc_timing_evt_code_chk_fp, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_EVT_IN5_GLOBAL_MASK                  (0xffffffff)
#define TIMING_EVT_IN5_GLOBAL_W(val)                (val)
#define TIMING_EVT_IN5_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, evt_code_5) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, EVT_IN5, GLOBAL, MULT_BIT_PARAM,
            , , rw_afc_timing_evt_code_chk_fp, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_EVT_IN6_GLOBAL_MASK                  (0xffffffff)
#define TIMING_EVT_IN6_GLOBAL_W(val)                (val)
#define TIMING_EVT_IN6_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, evt_code_6) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, EVT_IN6, GLOBAL, MULT_BIT_PARAM,
            , , rw_afc_timing_evt_code_chk_fp, NO_FMT_FUNC, SET_FIELD);
}

/* Delay to tigger generation after event receive */
#define TIMING_EVT_DLY0_GLOBAL_MASK                  (0xffffffff)
#define TIMING_EVT_DLY0_GLOBAL_W(val)                (val)
#define TIMING_EVT_DLY0_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, evt_delay_0) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, EVT_DLY0, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_EVT_DLY1_GLOBAL_MASK                  (0xffffffff)
#define TIMING_EVT_DLY1_GLOBAL_W(val)                (val)
#define TIMING_EVT_DLY1_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, evt_delay_1) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, EVT_DLY1, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_EVT_DLY2_GLOBAL_MASK                  (0xffffffff)
#define TIMING_EVT_DLY2_GLOBAL_W(val)                (val)
#define TIMING_EVT_DLY2_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, evt_delay_2) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, EVT_DLY2, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_EVT_DLY3_GLOBAL_MASK                  (0xffffffff)
#define TIMING_EVT_DLY3_GLOBAL_W(val)                (val)
#define TIMING_EVT_DLY3_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, evt_delay_3) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, EVT_DLY3, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_EVT_DLY4_GLOBAL_MASK                  (0xffffffff)
#define TIMING_EVT_DLY4_GLOBAL_W(val)                (val)
#define TIMING_EVT_DLY4_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, evt_delay_4) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, EVT_DLY4, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_EVT_DLY5_GLOBAL_MASK                  (0xffffffff)
#define TIMING_EVT_DLY5_GLOBAL_W(val)                (val)
#define TIMING_EVT_DLY5_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, evt_delay_5) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, EVT_DLY5, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_EVT_DLY6_GLOBAL_MASK                  (0xffffffff)
#define TIMING_EVT_DLY6_GLOBAL_W(val)                (val)
#define TIMING_EVT_DLY6_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, evt_delay_6) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, EVT_DLY6, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

/* Width of the trigger generated by an event receive */
#define TIMING_EVT_WDT0_GLOBAL_MASK                  (0xffffffff)
#define TIMING_EVT_WDT0_GLOBAL_W(val)                (val)
#define TIMING_EVT_WDT0_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, evt_width_0) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, EVT_WDT0, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_EVT_WDT1_GLOBAL_MASK                  (0xffffffff)
#define TIMING_EVT_WDT1_GLOBAL_W(val)                (val)
#define TIMING_EVT_WDT1_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, evt_width_1) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, EVT_WDT1, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_EVT_WDT2_GLOBAL_MASK                  (0xffffffff)
#define TIMING_EVT_WDT2_GLOBAL_W(val)                (val)
#define TIMING_EVT_WDT2_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, evt_width_2) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, EVT_WDT2, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_EVT_WDT3_GLOBAL_MASK                  (0xffffffff)
#define TIMING_EVT_WDT3_GLOBAL_W(val)                (val)
#define TIMING_EVT_WDT3_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, evt_width_3) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, EVT_WDT3, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_EVT_WDT4_GLOBAL_MASK                  (0xffffffff)
#define TIMING_EVT_WDT4_GLOBAL_W(val)                (val)
#define TIMING_EVT_WDT4_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, evt_width_4) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, EVT_WDT4, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_EVT_WDT5_GLOBAL_MASK                  (0xffffffff)
#define TIMING_EVT_WDT5_GLOBAL_W(val)                (val)
#define TIMING_EVT_WDT5_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, evt_width_5) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, EVT_WDT5, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_EVT_WDT6_GLOBAL_MASK                  (0xffffffff)
#define TIMING_EVT_WDT6_GLOBAL_W(val)                (val)
#define TIMING_EVT_WDT6_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, evt_width_6) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, EVT_WDT6, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

/* Phase and frequency control feedback gains */
#define TIMING_CONTROL_GAIN_MIN 1
#define TIMING_CONTROL_GAIN_MAX 0xFF

RW_PARAM_FUNC(afc_timing, freq_kp) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FREQ, KP, MULT_BIT_PARAM,
            TIMING_CONTROL_GAIN_MIN, TIMING_CONTROL_GAIN_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, freq_ki) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FREQ, KI, MULT_BIT_PARAM,
            TIMING_CONTROL_GAIN_MIN, TIMING_CONTROL_GAIN_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, phase_kp) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, PHASE, KP, MULT_BIT_PARAM,
            TIMING_CONTROL_GAIN_MIN, TIMING_CONTROL_GAIN_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, phase_ki) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, PHASE, KI, MULT_BIT_PARAM,
            TIMING_CONTROL_GAIN_MIN, TIMING_CONTROL_GAIN_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_PHASE_BIAS_GLOBAL_MASK                  (0xffffffff)
#define TIMING_PHASE_BIAS_GLOBAL_W(val)                (val)
#define TIMING_PHASE_BIAS_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, phase_bias) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, PHASE_BIAS, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_AVG_EXP_MIN 1
#define TIMING_AVG_EXP_MAX 0xF
RW_PARAM_FUNC(afc_timing, avg_exponent) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, DDMTD, AVG_EXP, MULT_BIT_PARAM,
            TIMING_AVG_EXP_MIN, TIMING_AVG_EXP_MAX, NO_CHK_FUNC, 
            NO_FMT_FUNC, SET_FIELD);
}


/* Exported function pointers */
const disp_table_func_fp afc_timing_exp_fp [] = {
    RW_PARAM_FUNC_NAME(afc_timing, link_status),
    RW_PARAM_FUNC_NAME(afc_timing, rxen_status),
    RW_PARAM_FUNC_NAME(afc_timing, ref_clk_locked),
    RW_PARAM_FUNC_NAME(afc_timing, evt_code_0),
    RW_PARAM_FUNC_NAME(afc_timing, evt_code_1),
    RW_PARAM_FUNC_NAME(afc_timing, evt_code_2),
    RW_PARAM_FUNC_NAME(afc_timing, evt_code_3),
    RW_PARAM_FUNC_NAME(afc_timing, evt_code_4),
    RW_PARAM_FUNC_NAME(afc_timing, evt_code_5),
    RW_PARAM_FUNC_NAME(afc_timing, evt_code_6),
    RW_PARAM_FUNC_NAME(afc_timing, evt_delay_0),
    RW_PARAM_FUNC_NAME(afc_timing, evt_delay_1),
    RW_PARAM_FUNC_NAME(afc_timing, evt_delay_2),
    RW_PARAM_FUNC_NAME(afc_timing, evt_delay_3),
    RW_PARAM_FUNC_NAME(afc_timing, evt_delay_4),
    RW_PARAM_FUNC_NAME(afc_timing, evt_delay_5),
    RW_PARAM_FUNC_NAME(afc_timing, evt_delay_6),
    RW_PARAM_FUNC_NAME(afc_timing, evt_width_0),
    RW_PARAM_FUNC_NAME(afc_timing, evt_width_1),
    RW_PARAM_FUNC_NAME(afc_timing, evt_width_2),
    RW_PARAM_FUNC_NAME(afc_timing, evt_width_3),
    RW_PARAM_FUNC_NAME(afc_timing, evt_width_4),
    RW_PARAM_FUNC_NAME(afc_timing, evt_width_5),
    RW_PARAM_FUNC_NAME(afc_timing, evt_width_6),
    RW_PARAM_FUNC_NAME(afc_timing, freq_kp),
    RW_PARAM_FUNC_NAME(afc_timing, freq_ki),
    RW_PARAM_FUNC_NAME(afc_timing, phase_kp),
    RW_PARAM_FUNC_NAME(afc_timing, phase_ki),
    RW_PARAM_FUNC_NAME(afc_timing, phase_bias),
    RW_PARAM_FUNC_NAME(afc_timing, avg_exponent),
    NULL
};

/************************************************************/
/***************** Export methods functions *****************/
/************************************************************/

static smio_err_e _afc_timing_do_op (void *owner, void *msg);

/* Attach an instance of sm_io to dev_io function pointer */
smio_err_e afc_timing_attach (smio_t *self, devio_t *parent)
{
    (void) self;
    (void) parent;
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Deattach an instance of sm_io to dev_io function pointer */
smio_err_e afc_timing_deattach (smio_t *self)
{
    (void) self;
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Export (register) sm_io to handle operations function pointer */
smio_err_e afc_timing_export_ops (smio_t *self,
        const disp_op_t **smio_exp_ops)
{
    (void) self;
    (void) smio_exp_ops;
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Unexport (unregister) sm_io to handle operations function pointer */
smio_err_e afc_timing_unexport_ops (smio_t *self)
{
    (void) self;
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Generic wrapper for receiving opcodes and arguments to specific funtions function pointer */
/* FIXME: Code repetition! _devio_do_smio_op () function does almost the same!!! */
smio_err_e _afc_timing_do_op (void *owner, void *msg)
{
    (void) owner;
    (void) msg;
    return SMIO_ERR_FUNC_NOT_IMPL;
}

smio_err_e afc_timing_do_op (void *self, void *msg)
{
    return _afc_timing_do_op (self, msg);
}

const smio_ops_t afc_timing_ops = {
    .attach             = afc_timing_attach,          /* Attach sm_io instance to dev_io */
    .deattach           = afc_timing_deattach,        /* Deattach sm_io instance to dev_io */
    .export_ops         = afc_timing_export_ops,      /* Export sm_io operations to dev_io */
    .unexport_ops       = afc_timing_unexport_ops,    /* Unexport sm_io operations to dev_io */
    .do_op              = afc_timing_do_op            /* Generic wrapper for handling specific operations */
};

/************************************************************/
/****************** Bootstrap Operations ********************/
/************************************************************/

smio_err_e afc_timing_init (smio_t * self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:afc_timing_exp] Initializing afc_timing\n");

    smio_err_e err = SMIO_SUCCESS;

    err = smio_set_id (self, AFC_TIMING_SDB_DEVID);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO id", err_set_id);
    err = smio_set_name (self, AFC_TIMING_SDB_NAME);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO name", err_set_name);

    /* Set SMIO ops pointers */
    err = smio_set_ops (self, &afc_timing_ops);
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
    err = smio_init_exp_ops (self, (disp_op_t **) afc_timing_exp_ops, afc_timing_exp_fp);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not fill SMIO "
            "function descriptors with the callbacks", err_fill_desc);

    err = smio_set_exp_ops (self, afc_timing_exp_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO exported operations",
            err_smio_set_exp_ops);

    /* Initialize specific structure */
    smio_afc_timing_t *smio_handler = smio_afc_timing_new (self);
    ASSERT_ALLOC(smio_handler, err_smio_handler_alloc, SMIO_ERR_ALLOC);
    err = smio_set_handler (self, smio_handler);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO handler",
            err_smio_set_handler);

    return err;

err_smio_set_handler:
    smio_afc_timing_destroy (&smio_handler);
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

/* Destroy sm_io instance of afc_timing */
smio_err_e afc_timing_shutdown (smio_t *self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:afc_timing_exp] Shutting down afc_timing\n");

    smio_err_e err = SMIO_SUCCESS;
    smio_afc_timing_t *afc_timing = smio_get_handler (self);
    ASSERT_TEST(afc_timing != NULL, "Could not get AFC Timing handler",
            err_afc_timing_handler, SMIO_ERR_ALLOC /* FIXME: improve return code */);

    /* Destroy SMIO instance */
    smio_afc_timing_destroy (&afc_timing);
    /* Nullify operation pointers */
    smio_set_exp_ops (self, NULL);
    smio_set_thsafe_client_ops (self, NULL);
    smio_set_ops (self, NULL);

err_afc_timing_handler:
    return err;
}

const smio_bootstrap_ops_t afc_timing_bootstrap_ops = {
    .init = afc_timing_init,
    .shutdown = afc_timing_shutdown,
    .config_defaults = afc_timing_config_defaults
};

SMIO_MOD_DECLARE(AFC_TIMING_SDB_DEVID, AFC_TIMING_SDB_NAME, afc_timing_bootstrap_ops)
