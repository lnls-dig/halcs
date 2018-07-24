/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Joao Brito <joao.brito@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "halcs_server.h"
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
    ASSERT_HAL_TEST(test_boolean, SM_IO, "[sm_io:afc_timing_exp]",             \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)                  \
    ASSERT_HAL_ALLOC(ptr, SM_IO, "[sm_io:afc_timing_exp]",                     \
            smio_err_str(SMIO_ERR_ALLOC),                                      \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                               \
    CHECK_HAL_ERR(err, SM_IO, "[sm_io:afc_timing_exp]",                        \
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
static int _rw_afc_timing_evtcode_chk (uint32_t evtcode)
{
    if (evtcode==0x7F || evtcode==0x7E || evtcode==0x70) {
        return PARAM_ERR;
    }

    return PARAM_OK;
}

rw_param_check_fp rw_afc_timing_evtcode_chk_fp = _rw_afc_timing_evtcode_chk;

// AMC 0
RW_PARAM_FUNC(afc_timing, amc_en0) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC0, EN, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, amc_pol0) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC0, POL, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, amc_log0) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC0, LOG, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, amc_itl0) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC0, ITL, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, amc_src0) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC0, SRC, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, amc_dir0) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC0, DIR, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_AMC0_PULSES_GLOBAL_MASK                  (0xffffffff)
#define TIMING_AMC0_PULSES_GLOBAL_W(val)                (val)
#define TIMING_AMC0_PULSES_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, amc_pulses0) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC0_PULSES, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_AMC0_EVT_GLOBAL_MASK                  (0xffffffff)
#define TIMING_AMC0_EVT_GLOBAL_W(val)                (val)
#define TIMING_AMC0_EVT_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, amc_evt0) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC0_EVT, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_AMC0_DLY_GLOBAL_MASK                  (0xffffffff)
#define TIMING_AMC0_DLY_GLOBAL_W(val)                (val)
#define TIMING_AMC0_DLY_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, amc_dly0) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC0_DLY, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_AMC0_WDT_GLOBAL_MASK                  (0xffffffff)
#define TIMING_AMC0_WDT_GLOBAL_W(val)                (val)
#define TIMING_AMC0_WDT_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, amc_wdt0) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC0_WDT, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

// AMC 1
RW_PARAM_FUNC(afc_timing, amc_en1) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC1, EN, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, amc_pol1) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC1, POL, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, amc_log1) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC1, LOG, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, amc_itl1) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC1, ITL, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, amc_src1) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC1, SRC, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, amc_dir1) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC1, DIR, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_AMC1_PULSES_GLOBAL_MASK                  (0xffffffff)
#define TIMING_AMC1_PULSES_GLOBAL_W(val)                (val)
#define TIMING_AMC1_PULSES_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, amc_pulses1) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC1_PULSES, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_AMC1_EVT_GLOBAL_MASK                  (0xffffffff)
#define TIMING_AMC1_EVT_GLOBAL_W(val)                (val)
#define TIMING_AMC1_EVT_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, amc_evt1) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC1_EVT, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_AMC1_DLY_GLOBAL_MASK                  (0xffffffff)
#define TIMING_AMC1_DLY_GLOBAL_W(val)                (val)
#define TIMING_AMC1_DLY_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, amc_dly1) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC1_DLY, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_AMC1_WDT_GLOBAL_MASK                  (0xffffffff)
#define TIMING_AMC1_WDT_GLOBAL_W(val)                (val)
#define TIMING_AMC1_WDT_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, amc_wdt1) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC1_WDT, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

// AMC 2
RW_PARAM_FUNC(afc_timing, amc_en2) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC2, EN, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, amc_pol2) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC2, POL, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, amc_log2) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC2, LOG, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, amc_itl2) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC2, ITL, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, amc_src2) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC2, SRC, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, amc_dir2) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC2, DIR, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_AMC2_PULSES_GLOBAL_MASK                  (0xffffffff)
#define TIMING_AMC2_PULSES_GLOBAL_W(val)                (val)
#define TIMING_AMC2_PULSES_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, amc_pulses2) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC2_PULSES, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_AMC2_EVT_GLOBAL_MASK                  (0xffffffff)
#define TIMING_AMC2_EVT_GLOBAL_W(val)                (val)
#define TIMING_AMC2_EVT_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, amc_evt2) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC2_EVT, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_AMC2_DLY_GLOBAL_MASK                  (0xffffffff)
#define TIMING_AMC2_DLY_GLOBAL_W(val)                (val)
#define TIMING_AMC2_DLY_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, amc_dly2) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC2_DLY, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_AMC2_WDT_GLOBAL_MASK                  (0xffffffff)
#define TIMING_AMC2_WDT_GLOBAL_W(val)                (val)
#define TIMING_AMC2_WDT_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, amc_wdt2) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC2_WDT, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

// AMC 3
RW_PARAM_FUNC(afc_timing, amc_en3) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC3, EN, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, amc_pol3) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC3, POL, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, amc_log3) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC3, LOG, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, amc_itl3) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC3, ITL, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, amc_src3) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC3, SRC, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, amc_dir3) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC3, DIR, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_AMC3_PULSES_GLOBAL_MASK                  (0xffffffff)
#define TIMING_AMC3_PULSES_GLOBAL_W(val)                (val)
#define TIMING_AMC3_PULSES_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, amc_pulses3) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC3_PULSES, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_AMC3_EVT_GLOBAL_MASK                  (0xffffffff)
#define TIMING_AMC3_EVT_GLOBAL_W(val)                (val)
#define TIMING_AMC3_EVT_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, amc_evt3) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC3_EVT, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_AMC3_DLY_GLOBAL_MASK                  (0xffffffff)
#define TIMING_AMC3_DLY_GLOBAL_W(val)                (val)
#define TIMING_AMC3_DLY_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, amc_dly3) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC3_DLY, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_AMC3_WDT_GLOBAL_MASK                  (0xffffffff)
#define TIMING_AMC3_WDT_GLOBAL_W(val)                (val)
#define TIMING_AMC3_WDT_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, amc_wdt3) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC3_WDT, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

// AMC 4
RW_PARAM_FUNC(afc_timing, amc_en4) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC4, EN, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, amc_pol4) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC4, POL, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, amc_log4) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC4, LOG, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, amc_itl4) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC4, ITL, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, amc_src4) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC4, SRC, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, amc_dir4) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC4, DIR, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_AMC4_PULSES_GLOBAL_MASK                  (0xffffffff)
#define TIMING_AMC4_PULSES_GLOBAL_W(val)                (val)
#define TIMING_AMC4_PULSES_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, amc_pulses4) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC4_PULSES, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_AMC4_EVT_GLOBAL_MASK                  (0xffffffff)
#define TIMING_AMC4_EVT_GLOBAL_W(val)                (val)
#define TIMING_AMC4_EVT_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, amc_evt4) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC4_EVT, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_AMC4_DLY_GLOBAL_MASK                  (0xffffffff)
#define TIMING_AMC4_DLY_GLOBAL_W(val)                (val)
#define TIMING_AMC4_DLY_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, amc_dly4) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC4_DLY, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_AMC4_WDT_GLOBAL_MASK                  (0xffffffff)
#define TIMING_AMC4_WDT_GLOBAL_W(val)                (val)
#define TIMING_AMC4_WDT_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, amc_wdt4) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC4_WDT, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

// AMC 5
RW_PARAM_FUNC(afc_timing, amc_en5) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC5, EN, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, amc_pol5) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC5, POL, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, amc_log5) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC5, LOG, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, amc_itl5) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC5, ITL, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, amc_src5) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC5, SRC, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, amc_dir5) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC5, DIR, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_AMC5_PULSES_GLOBAL_MASK                  (0xffffffff)
#define TIMING_AMC5_PULSES_GLOBAL_W(val)                (val)
#define TIMING_AMC5_PULSES_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, amc_pulses5) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC5_PULSES, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_AMC5_EVT_GLOBAL_MASK                  (0xffffffff)
#define TIMING_AMC5_EVT_GLOBAL_W(val)                (val)
#define TIMING_AMC5_EVT_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, amc_evt5) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC5_EVT, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_AMC5_DLY_GLOBAL_MASK                  (0xffffffff)
#define TIMING_AMC5_DLY_GLOBAL_W(val)                (val)
#define TIMING_AMC5_DLY_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, amc_dly5) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC5_DLY, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_AMC5_WDT_GLOBAL_MASK                  (0xffffffff)
#define TIMING_AMC5_WDT_GLOBAL_W(val)                (val)
#define TIMING_AMC5_WDT_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, amc_wdt5) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC5_WDT, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

// AMC 6
RW_PARAM_FUNC(afc_timing, amc_en6) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC6, EN, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, amc_pol6) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC6, POL, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, amc_log6) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC6, LOG, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, amc_itl6) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC6, ITL, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, amc_src6) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC6, SRC, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, amc_dir6) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC6, DIR, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_AMC6_PULSES_GLOBAL_MASK                  (0xffffffff)
#define TIMING_AMC6_PULSES_GLOBAL_W(val)                (val)
#define TIMING_AMC6_PULSES_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, amc_pulses6) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC6_PULSES, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_AMC6_EVT_GLOBAL_MASK                  (0xffffffff)
#define TIMING_AMC6_EVT_GLOBAL_W(val)                (val)
#define TIMING_AMC6_EVT_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, amc_evt6) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC6_EVT, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_AMC6_DLY_GLOBAL_MASK                  (0xffffffff)
#define TIMING_AMC6_DLY_GLOBAL_W(val)                (val)
#define TIMING_AMC6_DLY_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, amc_dly6) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC6_DLY, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_AMC6_WDT_GLOBAL_MASK                  (0xffffffff)
#define TIMING_AMC6_WDT_GLOBAL_W(val)                (val)
#define TIMING_AMC6_WDT_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, amc_wdt6) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC6_WDT, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

// AMC 7
RW_PARAM_FUNC(afc_timing, amc_en7) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC7, EN, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, amc_pol7) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC7, POL, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, amc_log7) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC7, LOG, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, amc_itl7) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC7, ITL, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, amc_src7) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC7, SRC, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, amc_dir7) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC7, DIR, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_AMC7_PULSES_GLOBAL_MASK                  (0xffffffff)
#define TIMING_AMC7_PULSES_GLOBAL_W(val)                (val)
#define TIMING_AMC7_PULSES_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, amc_pulses7) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC7_PULSES, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_AMC7_EVT_GLOBAL_MASK                  (0xffffffff)
#define TIMING_AMC7_EVT_GLOBAL_W(val)                (val)
#define TIMING_AMC7_EVT_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, amc_evt7) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC7_EVT, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_AMC7_DLY_GLOBAL_MASK                  (0xffffffff)
#define TIMING_AMC7_DLY_GLOBAL_W(val)                (val)
#define TIMING_AMC7_DLY_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, amc_dly7) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC7_DLY, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_AMC7_WDT_GLOBAL_MASK                  (0xffffffff)
#define TIMING_AMC7_WDT_GLOBAL_W(val)                (val)
#define TIMING_AMC7_WDT_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, amc_wdt7) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AMC7_WDT, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

// FMC 1 CH0
RW_PARAM_FUNC(afc_timing, fmc1_en0) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC1CH0, EN, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, fmc1_pol0) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC1CH0, POL, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, fmc1_log0) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC1CH0, LOG, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, fmc1_itl0) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC1CH0, ITL, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, fmc1_src0) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC1CH0, SRC, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_FMC1CH0_PULSES_GLOBAL_MASK                  (0xffffffff)
#define TIMING_FMC1CH0_PULSES_GLOBAL_W(val)                (val)
#define TIMING_FMC1CH0_PULSES_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, fmc1_pulses0) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC1CH0_PULSES, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_FMC1CH0_EVT_GLOBAL_MASK                  (0xffffffff)
#define TIMING_FMC1CH0_EVT_GLOBAL_W(val)                (val)
#define TIMING_FMC1CH0_EVT_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, fmc1_evt0) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC1CH0_EVT, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_FMC1CH0_DLY_GLOBAL_MASK                  (0xffffffff)
#define TIMING_FMC1CH0_DLY_GLOBAL_W(val)                (val)
#define TIMING_FMC1CH0_DLY_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, fmc1_dly0) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC1CH0_DLY, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_FMC1CH0_WDT_GLOBAL_MASK                  (0xffffffff)
#define TIMING_FMC1CH0_WDT_GLOBAL_W(val)                (val)
#define TIMING_FMC1CH0_WDT_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, fmc1_wdt0) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC1CH0_WDT, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

// FMC 1 CH1
RW_PARAM_FUNC(afc_timing, fmc1_en1) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC1CH1, EN, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, fmc1_pol1) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC1CH1, POL, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, fmc1_log1) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC1CH1, LOG, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, fmc1_itl1) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC1CH1, ITL, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, fmc1_src1) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC1CH1, SRC, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_FMC1CH1_PULSES_GLOBAL_MASK                  (0xffffffff)
#define TIMING_FMC1CH1_PULSES_GLOBAL_W(val)                (val)
#define TIMING_FMC1CH1_PULSES_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, fmc1_pulses1) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC1CH1_PULSES, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_FMC1CH1_EVT_GLOBAL_MASK                  (0xffffffff)
#define TIMING_FMC1CH1_EVT_GLOBAL_W(val)                (val)
#define TIMING_FMC1CH1_EVT_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, fmc1_evt1) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC1CH1_EVT, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_FMC1CH1_DLY_GLOBAL_MASK                  (0xffffffff)
#define TIMING_FMC1CH1_DLY_GLOBAL_W(val)                (val)
#define TIMING_FMC1CH1_DLY_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, fmc1_dly1) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC1CH1_DLY, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_FMC1CH1_WDT_GLOBAL_MASK                  (0xffffffff)
#define TIMING_FMC1CH1_WDT_GLOBAL_W(val)                (val)
#define TIMING_FMC1CH1_WDT_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, fmc1_wdt1) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC1CH1_WDT, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

// FMC 1 CH2
RW_PARAM_FUNC(afc_timing, fmc1_en2) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC1CH2, EN, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, fmc1_pol2) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC1CH2, POL, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, fmc1_log2) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC1CH2, LOG, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, fmc1_itl2) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC1CH2, ITL, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, fmc1_src2) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC1CH2, SRC, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_FMC1CH2_PULSES_GLOBAL_MASK                  (0xffffffff)
#define TIMING_FMC1CH2_PULSES_GLOBAL_W(val)                (val)
#define TIMING_FMC1CH2_PULSES_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, fmc1_pulses2) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC1CH2_PULSES, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_FMC1CH2_EVT_GLOBAL_MASK                  (0xffffffff)
#define TIMING_FMC1CH2_EVT_GLOBAL_W(val)                (val)
#define TIMING_FMC1CH2_EVT_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, fmc1_evt2) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC1CH2_EVT, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_FMC1CH2_DLY_GLOBAL_MASK                  (0xffffffff)
#define TIMING_FMC1CH2_DLY_GLOBAL_W(val)                (val)
#define TIMING_FMC1CH2_DLY_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, fmc1_dly2) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC1CH2_DLY, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_FMC1CH2_WDT_GLOBAL_MASK                  (0xffffffff)
#define TIMING_FMC1CH2_WDT_GLOBAL_W(val)                (val)
#define TIMING_FMC1CH2_WDT_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, fmc1_wdt2) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC1CH2_WDT, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

// FMC 1 CH3
RW_PARAM_FUNC(afc_timing, fmc1_en3) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC1CH3, EN, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, fmc1_pol3) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC1CH3, POL, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, fmc1_log3) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC1CH3, LOG, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, fmc1_itl3) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC1CH3, ITL, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, fmc1_src3) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC1CH3, SRC, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_FMC1CH3_PULSES_GLOBAL_MASK                  (0xffffffff)
#define TIMING_FMC1CH3_PULSES_GLOBAL_W(val)                (val)
#define TIMING_FMC1CH3_PULSES_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, fmc1_pulses3) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC1CH3_PULSES, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_FMC1CH3_EVT_GLOBAL_MASK                  (0xffffffff)
#define TIMING_FMC1CH3_EVT_GLOBAL_W(val)                (val)
#define TIMING_FMC1CH3_EVT_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, fmc1_evt3) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC1CH3_EVT, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_FMC1CH3_DLY_GLOBAL_MASK                  (0xffffffff)
#define TIMING_FMC1CH3_DLY_GLOBAL_W(val)                (val)
#define TIMING_FMC1CH3_DLY_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, fmc1_dly3) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC1CH3_DLY, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_FMC1CH3_WDT_GLOBAL_MASK                  (0xffffffff)
#define TIMING_FMC1CH3_WDT_GLOBAL_W(val)                (val)
#define TIMING_FMC1CH3_WDT_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, fmc1_wdt3) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC1CH3_WDT, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

// FMC 1 CH4
RW_PARAM_FUNC(afc_timing, fmc1_en4) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC1CH4, EN, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, fmc1_pol4) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC1CH4, POL, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, fmc1_log4) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC1CH4, LOG, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, fmc1_itl4) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC1CH4, ITL, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, fmc1_src4) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC1CH4, SRC, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_FMC1CH4_PULSES_GLOBAL_MASK                  (0xffffffff)
#define TIMING_FMC1CH4_PULSES_GLOBAL_W(val)                (val)
#define TIMING_FMC1CH4_PULSES_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, fmc1_pulses4) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC1CH4_PULSES, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_FMC1CH4_EVT_GLOBAL_MASK                  (0xffffffff)
#define TIMING_FMC1CH4_EVT_GLOBAL_W(val)                (val)
#define TIMING_FMC1CH4_EVT_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, fmc1_evt4) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC1CH4_EVT, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_FMC1CH4_DLY_GLOBAL_MASK                  (0xffffffff)
#define TIMING_FMC1CH4_DLY_GLOBAL_W(val)                (val)
#define TIMING_FMC1CH4_DLY_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, fmc1_dly4) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC1CH4_DLY, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_FMC1CH4_WDT_GLOBAL_MASK                  (0xffffffff)
#define TIMING_FMC1CH4_WDT_GLOBAL_W(val)                (val)
#define TIMING_FMC1CH4_WDT_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, fmc1_wdt4) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC1CH4_WDT, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

// FMC 2 CH0
RW_PARAM_FUNC(afc_timing, fmc2_en0) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC2CH0, EN, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, fmc2_pol0) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC2CH0, POL, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, fmc2_log0) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC2CH0, LOG, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, fmc2_itl0) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC2CH0, ITL, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, fmc2_src0) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC2CH0, SRC, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_FMC2CH0_PULSES_GLOBAL_MASK                  (0xffffffff)
#define TIMING_FMC2CH0_PULSES_GLOBAL_W(val)                (val)
#define TIMING_FMC2CH0_PULSES_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, fmc2_pulses0) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC2CH0_PULSES, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_FMC2CH0_EVT_GLOBAL_MASK                  (0xffffffff)
#define TIMING_FMC2CH0_EVT_GLOBAL_W(val)                (val)
#define TIMING_FMC2CH0_EVT_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, fmc2_evt0) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC2CH0_EVT, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_FMC2CH0_DLY_GLOBAL_MASK                  (0xffffffff)
#define TIMING_FMC2CH0_DLY_GLOBAL_W(val)                (val)
#define TIMING_FMC2CH0_DLY_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, fmc2_dly0) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC2CH0_DLY, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_FMC2CH0_WDT_GLOBAL_MASK                  (0xffffffff)
#define TIMING_FMC2CH0_WDT_GLOBAL_W(val)                (val)
#define TIMING_FMC2CH0_WDT_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, fmc2_wdt0) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC2CH0_WDT, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

// FMC 2 CH1
RW_PARAM_FUNC(afc_timing, fmc2_en1) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC2CH1, EN, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, fmc2_pol1) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC2CH1, POL, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, fmc2_log1) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC2CH1, LOG, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, fmc2_itl1) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC2CH1, ITL, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, fmc2_src1) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC2CH1, SRC, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_FMC2CH1_PULSES_GLOBAL_MASK                  (0xffffffff)
#define TIMING_FMC2CH1_PULSES_GLOBAL_W(val)                (val)
#define TIMING_FMC2CH1_PULSES_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, fmc2_pulses1) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC2CH1_PULSES, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_FMC2CH1_EVT_GLOBAL_MASK                  (0xffffffff)
#define TIMING_FMC2CH1_EVT_GLOBAL_W(val)                (val)
#define TIMING_FMC2CH1_EVT_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, fmc2_evt1) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC2CH1_EVT, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_FMC2CH1_DLY_GLOBAL_MASK                  (0xffffffff)
#define TIMING_FMC2CH1_DLY_GLOBAL_W(val)                (val)
#define TIMING_FMC2CH1_DLY_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, fmc2_dly1) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC2CH1_DLY, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_FMC2CH1_WDT_GLOBAL_MASK                  (0xffffffff)
#define TIMING_FMC2CH1_WDT_GLOBAL_W(val)                (val)
#define TIMING_FMC2CH1_WDT_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, fmc2_wdt1) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC2CH1_WDT, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

// FMC 2 CH2
RW_PARAM_FUNC(afc_timing, fmc2_en2) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC2CH2, EN, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, fmc2_pol2) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC2CH2, POL, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, fmc2_log2) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC2CH2, LOG, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, fmc2_itl2) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC2CH2, ITL, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, fmc2_src2) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC2CH2, SRC, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_FMC2CH2_PULSES_GLOBAL_MASK                  (0xffffffff)
#define TIMING_FMC2CH2_PULSES_GLOBAL_W(val)                (val)
#define TIMING_FMC2CH2_PULSES_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, fmc2_pulses2) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC2CH2_PULSES, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_FMC2CH2_EVT_GLOBAL_MASK                  (0xffffffff)
#define TIMING_FMC2CH2_EVT_GLOBAL_W(val)                (val)
#define TIMING_FMC2CH2_EVT_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, fmc2_evt2) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC2CH2_EVT, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_FMC2CH2_DLY_GLOBAL_MASK                  (0xffffffff)
#define TIMING_FMC2CH2_DLY_GLOBAL_W(val)                (val)
#define TIMING_FMC2CH2_DLY_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, fmc2_dly2) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC2CH2_DLY, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_FMC2CH2_WDT_GLOBAL_MASK                  (0xffffffff)
#define TIMING_FMC2CH2_WDT_GLOBAL_W(val)                (val)
#define TIMING_FMC2CH2_WDT_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, fmc2_wdt2) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC2CH2_WDT, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

// FMC 2 CH3
RW_PARAM_FUNC(afc_timing, fmc2_en3) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC2CH3, EN, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, fmc2_pol3) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC2CH3, POL, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, fmc2_log3) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC2CH3, LOG, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, fmc2_itl3) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC2CH3, ITL, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, fmc2_src3) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC2CH3, SRC, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_FMC2CH3_PULSES_GLOBAL_MASK                  (0xffffffff)
#define TIMING_FMC2CH3_PULSES_GLOBAL_W(val)                (val)
#define TIMING_FMC2CH3_PULSES_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, fmc2_pulses3) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC2CH3_PULSES, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_FMC2CH3_EVT_GLOBAL_MASK                  (0xffffffff)
#define TIMING_FMC2CH3_EVT_GLOBAL_W(val)                (val)
#define TIMING_FMC2CH3_EVT_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, fmc2_evt3) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC2CH3_EVT, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_FMC2CH3_DLY_GLOBAL_MASK                  (0xffffffff)
#define TIMING_FMC2CH3_DLY_GLOBAL_W(val)                (val)
#define TIMING_FMC2CH3_DLY_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, fmc2_dly3) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC2CH3_DLY, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_FMC2CH3_WDT_GLOBAL_MASK                  (0xffffffff)
#define TIMING_FMC2CH3_WDT_GLOBAL_W(val)                (val)
#define TIMING_FMC2CH3_WDT_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, fmc2_wdt3) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC2CH3_WDT, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

// FMC 2 CH4
RW_PARAM_FUNC(afc_timing, fmc2_en4) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC2CH4, EN, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, fmc2_pol4) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC2CH4, POL, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, fmc2_log4) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC2CH4, LOG, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, fmc2_itl4) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC2CH4, ITL, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, fmc2_src4) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC2CH4, SRC, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_FMC2CH4_PULSES_GLOBAL_MASK                  (0xffffffff)
#define TIMING_FMC2CH4_PULSES_GLOBAL_W(val)                (val)
#define TIMING_FMC2CH4_PULSES_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, fmc2_pulses4) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC2CH4_PULSES, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_FMC2CH4_EVT_GLOBAL_MASK                  (0xffffffff)
#define TIMING_FMC2CH4_EVT_GLOBAL_W(val)                (val)
#define TIMING_FMC2CH4_EVT_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, fmc2_evt4) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC2CH4_EVT, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_FMC2CH4_DLY_GLOBAL_MASK                  (0xffffffff)
#define TIMING_FMC2CH4_DLY_GLOBAL_W(val)                (val)
#define TIMING_FMC2CH4_DLY_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, fmc2_dly4) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC2CH4_DLY, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_FMC2CH4_WDT_GLOBAL_MASK                  (0xffffffff)
#define TIMING_FMC2CH4_WDT_GLOBAL_W(val)                (val)
#define TIMING_FMC2CH4_WDT_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, fmc2_wdt4) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FMC2CH4_WDT, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

/* RTM Phase and frequency control */
#define TIMING_CONTROL_GAIN_MIN 0
#define TIMING_CONTROL_GAIN_MAX 0xFFFF

RW_PARAM_FUNC(afc_timing, rtm_freq_kp) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, RTM_FREQ, KP, MULT_BIT_PARAM,
            TIMING_CONTROL_GAIN_MIN, TIMING_CONTROL_GAIN_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, rtm_freq_ki) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, RTM_FREQ, KI, MULT_BIT_PARAM,
            TIMING_CONTROL_GAIN_MIN, TIMING_CONTROL_GAIN_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, rtm_phase_kp) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, RTM_PHASE, KP, MULT_BIT_PARAM,
            TIMING_CONTROL_GAIN_MIN, TIMING_CONTROL_GAIN_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, rtm_phase_ki) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, RTM_PHASE, KI, MULT_BIT_PARAM,
            TIMING_CONTROL_GAIN_MIN, TIMING_CONTROL_GAIN_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_RTM_PHASE_SET_GLOBAL_MASK                  (0xffffffff)
#define TIMING_RTM_PHASE_SET_GLOBAL_W(val)                (val)
#define TIMING_RTM_PHASE_SET_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, rtm_phase_set) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, RTM_PHASE_SET, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_NAVG_MIN 1
#define TIMING_NAVG_MAX 0xFFF
RW_PARAM_FUNC(afc_timing, rtm_phase_navg) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, RTM_MAF, NAVG, MULT_BIT_PARAM,
            TIMING_NAVG_MIN, TIMING_NAVG_MAX, NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_DIV_EXP_MIN 0
#define TIMING_DIV_EXP_MAX 0x1F
RW_PARAM_FUNC(afc_timing, rtm_phase_div_exp) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, RTM_MAF, DIV_EXP, MULT_BIT_PARAM,
            TIMING_DIV_EXP_MIN, TIMING_DIV_EXP_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_RTM_RFREQ_HI_GLOBAL_MASK                  (0xffffffff)
#define TIMING_RTM_RFREQ_HI_GLOBAL_W(val)                (val)
#define TIMING_RTM_RFREQ_HI_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, rtm_rfreq_hi) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, RTM_RFREQ_HI, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_RTM_RFREQ_LO_GLOBAL_MASK                  (0xffffffff)
#define TIMING_RTM_RFREQ_LO_GLOBAL_W(val)                (val)
#define TIMING_RTM_RFREQ_LO_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, rtm_rfreq_lo) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, RTM_RFREQ_LO, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, rtm_n1) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, RTM, N1, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, rtm_hs_div) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, RTM, HS_DIV, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

/* AFC Phase and frequency control */
#define TIMING_CONTROL_GAIN_MIN 0
#define TIMING_CONTROL_GAIN_MAX 0xFFFF

RW_PARAM_FUNC(afc_timing, afc_freq_kp) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AFC_FREQ, KP, MULT_BIT_PARAM,
            TIMING_CONTROL_GAIN_MIN, TIMING_CONTROL_GAIN_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, afc_freq_ki) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AFC_FREQ, KI, MULT_BIT_PARAM,
            TIMING_CONTROL_GAIN_MIN, TIMING_CONTROL_GAIN_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, afc_phase_kp) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AFC_PHASE, KP, MULT_BIT_PARAM,
            TIMING_CONTROL_GAIN_MIN, TIMING_CONTROL_GAIN_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, afc_phase_ki) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AFC_PHASE, KI, MULT_BIT_PARAM,
            TIMING_CONTROL_GAIN_MIN, TIMING_CONTROL_GAIN_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_AFC_PHASE_SET_GLOBAL_MASK                  (0xffffffff)
#define TIMING_AFC_PHASE_SET_GLOBAL_W(val)                (val)
#define TIMING_AFC_PHASE_SET_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, afc_phase_set) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AFC_PHASE_SET, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, afc_phase_navg) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AFC_MAF, NAVG, MULT_BIT_PARAM,
            TIMING_NAVG_MIN, TIMING_NAVG_MAX, NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, afc_phase_div_exp) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AFC_MAF, DIV_EXP, MULT_BIT_PARAM,
            TIMING_DIV_EXP_MIN, TIMING_DIV_EXP_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_AFC_RFREQ_HI_GLOBAL_MASK                  (0xffffffff)
#define TIMING_AFC_RFREQ_HI_GLOBAL_W(val)                (val)
#define TIMING_AFC_RFREQ_HI_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, afc_rfreq_hi) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AFC_RFREQ_HI, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_AFC_RFREQ_LO_GLOBAL_MASK                  (0xffffffff)
#define TIMING_AFC_RFREQ_LO_GLOBAL_W(val)                (val)
#define TIMING_AFC_RFREQ_LO_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, afc_rfreq_lo) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AFC_RFREQ_LO, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, afc_n1) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AFC, N1, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, afc_hs_div) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, AFC, HS_DIV, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

/* Exported function pointers */
const disp_table_func_fp afc_timing_exp_fp [] = {
    RW_PARAM_FUNC_NAME(afc_timing, link_status),
    RW_PARAM_FUNC_NAME(afc_timing, rxen_status),
    RW_PARAM_FUNC_NAME(afc_timing, ref_clk_locked),
    RW_PARAM_FUNC_NAME(afc_timing, amc_en0),
    RW_PARAM_FUNC_NAME(afc_timing, amc_pol0),
    RW_PARAM_FUNC_NAME(afc_timing, amc_log0),
    RW_PARAM_FUNC_NAME(afc_timing, amc_itl0),
    RW_PARAM_FUNC_NAME(afc_timing, amc_src0),
    RW_PARAM_FUNC_NAME(afc_timing, amc_dir0),
    RW_PARAM_FUNC_NAME(afc_timing, amc_pulses0),
    RW_PARAM_FUNC_NAME(afc_timing, amc_evt0),
    RW_PARAM_FUNC_NAME(afc_timing, amc_dly0),
    RW_PARAM_FUNC_NAME(afc_timing, amc_wdt0),
    RW_PARAM_FUNC_NAME(afc_timing, amc_en1),
    RW_PARAM_FUNC_NAME(afc_timing, amc_pol1),
    RW_PARAM_FUNC_NAME(afc_timing, amc_log1),
    RW_PARAM_FUNC_NAME(afc_timing, amc_itl1),
    RW_PARAM_FUNC_NAME(afc_timing, amc_src1),
    RW_PARAM_FUNC_NAME(afc_timing, amc_dir1),
    RW_PARAM_FUNC_NAME(afc_timing, amc_pulses1),
    RW_PARAM_FUNC_NAME(afc_timing, amc_evt1),
    RW_PARAM_FUNC_NAME(afc_timing, amc_dly1),
    RW_PARAM_FUNC_NAME(afc_timing, amc_wdt1),
    RW_PARAM_FUNC_NAME(afc_timing, amc_en2),
    RW_PARAM_FUNC_NAME(afc_timing, amc_pol2),
    RW_PARAM_FUNC_NAME(afc_timing, amc_log2),
    RW_PARAM_FUNC_NAME(afc_timing, amc_itl2),
    RW_PARAM_FUNC_NAME(afc_timing, amc_src2),
    RW_PARAM_FUNC_NAME(afc_timing, amc_dir2),
    RW_PARAM_FUNC_NAME(afc_timing, amc_pulses2),
    RW_PARAM_FUNC_NAME(afc_timing, amc_evt2),
    RW_PARAM_FUNC_NAME(afc_timing, amc_dly2),
    RW_PARAM_FUNC_NAME(afc_timing, amc_wdt2),
    RW_PARAM_FUNC_NAME(afc_timing, amc_en3),
    RW_PARAM_FUNC_NAME(afc_timing, amc_pol3),
    RW_PARAM_FUNC_NAME(afc_timing, amc_log3),
    RW_PARAM_FUNC_NAME(afc_timing, amc_itl3),
    RW_PARAM_FUNC_NAME(afc_timing, amc_src3),
    RW_PARAM_FUNC_NAME(afc_timing, amc_dir3),
    RW_PARAM_FUNC_NAME(afc_timing, amc_pulses3),
    RW_PARAM_FUNC_NAME(afc_timing, amc_evt3),
    RW_PARAM_FUNC_NAME(afc_timing, amc_dly3),
    RW_PARAM_FUNC_NAME(afc_timing, amc_wdt3),
    RW_PARAM_FUNC_NAME(afc_timing, amc_en4),
    RW_PARAM_FUNC_NAME(afc_timing, amc_pol4),
    RW_PARAM_FUNC_NAME(afc_timing, amc_log4),
    RW_PARAM_FUNC_NAME(afc_timing, amc_itl4),
    RW_PARAM_FUNC_NAME(afc_timing, amc_src4),
    RW_PARAM_FUNC_NAME(afc_timing, amc_dir4),
    RW_PARAM_FUNC_NAME(afc_timing, amc_pulses4),
    RW_PARAM_FUNC_NAME(afc_timing, amc_evt4),
    RW_PARAM_FUNC_NAME(afc_timing, amc_dly4),
    RW_PARAM_FUNC_NAME(afc_timing, amc_wdt4),
    RW_PARAM_FUNC_NAME(afc_timing, amc_en5),
    RW_PARAM_FUNC_NAME(afc_timing, amc_pol5),
    RW_PARAM_FUNC_NAME(afc_timing, amc_log5),
    RW_PARAM_FUNC_NAME(afc_timing, amc_itl5),
    RW_PARAM_FUNC_NAME(afc_timing, amc_src5),
    RW_PARAM_FUNC_NAME(afc_timing, amc_dir5),
    RW_PARAM_FUNC_NAME(afc_timing, amc_pulses5),
    RW_PARAM_FUNC_NAME(afc_timing, amc_evt5),
    RW_PARAM_FUNC_NAME(afc_timing, amc_dly5),
    RW_PARAM_FUNC_NAME(afc_timing, amc_wdt5),
    RW_PARAM_FUNC_NAME(afc_timing, amc_en6),
    RW_PARAM_FUNC_NAME(afc_timing, amc_pol6),
    RW_PARAM_FUNC_NAME(afc_timing, amc_log6),
    RW_PARAM_FUNC_NAME(afc_timing, amc_itl6),
    RW_PARAM_FUNC_NAME(afc_timing, amc_src6),
    RW_PARAM_FUNC_NAME(afc_timing, amc_dir6),
    RW_PARAM_FUNC_NAME(afc_timing, amc_pulses6),
    RW_PARAM_FUNC_NAME(afc_timing, amc_evt6),
    RW_PARAM_FUNC_NAME(afc_timing, amc_dly6),
    RW_PARAM_FUNC_NAME(afc_timing, amc_wdt6),
    RW_PARAM_FUNC_NAME(afc_timing, amc_en7),
    RW_PARAM_FUNC_NAME(afc_timing, amc_pol7),
    RW_PARAM_FUNC_NAME(afc_timing, amc_log7),
    RW_PARAM_FUNC_NAME(afc_timing, amc_itl7),
    RW_PARAM_FUNC_NAME(afc_timing, amc_src7),
    RW_PARAM_FUNC_NAME(afc_timing, amc_dir7),
    RW_PARAM_FUNC_NAME(afc_timing, amc_pulses7),
    RW_PARAM_FUNC_NAME(afc_timing, amc_evt7),
    RW_PARAM_FUNC_NAME(afc_timing, amc_dly7),
    RW_PARAM_FUNC_NAME(afc_timing, amc_wdt7),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_en0),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_pol0),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_log0),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_itl0),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_src0),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_pulses0),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_evt0),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_dly0),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_wdt0),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_en1),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_pol1),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_log1),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_itl1),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_src1),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_pulses1),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_evt1),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_dly1),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_wdt1),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_en2),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_pol2),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_log2),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_itl2),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_src2),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_pulses2),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_evt2),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_dly2),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_wdt2),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_en3),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_pol3),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_log3),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_itl3),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_src3),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_pulses3),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_evt3),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_dly3),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_wdt3),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_en4),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_pol4),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_log4),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_itl4),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_src4),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_pulses4),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_evt4),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_dly4),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_wdt4),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_en0),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_pol0),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_log0),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_itl0),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_src0),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_pulses0),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_evt0),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_dly0),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_wdt0),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_en1),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_pol1),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_log1),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_itl1),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_src1),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_pulses1),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_evt1),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_dly1),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_wdt1),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_en2),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_pol2),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_log2),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_itl2),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_src2),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_pulses2),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_evt2),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_dly2),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_wdt2),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_en3),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_pol3),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_log3),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_itl3),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_src3),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_pulses3),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_evt3),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_dly3),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_wdt3),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_en4),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_pol4),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_log4),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_itl4),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_src4),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_pulses4),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_evt4),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_dly4),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_wdt4),
    RW_PARAM_FUNC_NAME(afc_timing, rtm_freq_kp),
    RW_PARAM_FUNC_NAME(afc_timing, rtm_freq_ki),
    RW_PARAM_FUNC_NAME(afc_timing, rtm_phase_kp),
    RW_PARAM_FUNC_NAME(afc_timing, rtm_phase_ki),
    RW_PARAM_FUNC_NAME(afc_timing, rtm_phase_set),
    RW_PARAM_FUNC_NAME(afc_timing, rtm_phase_navg),
    RW_PARAM_FUNC_NAME(afc_timing, rtm_phase_div_exp),
    RW_PARAM_FUNC_NAME(afc_timing, rtm_rfreq_hi),
    RW_PARAM_FUNC_NAME(afc_timing, rtm_rfreq_lo),
    RW_PARAM_FUNC_NAME(afc_timing, rtm_n1),
    RW_PARAM_FUNC_NAME(afc_timing, rtm_hs_div),
    RW_PARAM_FUNC_NAME(afc_timing, afc_freq_kp),
    RW_PARAM_FUNC_NAME(afc_timing, afc_freq_ki),
    RW_PARAM_FUNC_NAME(afc_timing, afc_phase_kp),
    RW_PARAM_FUNC_NAME(afc_timing, afc_phase_ki),
    RW_PARAM_FUNC_NAME(afc_timing, afc_phase_set),
    RW_PARAM_FUNC_NAME(afc_timing, afc_phase_navg),
    RW_PARAM_FUNC_NAME(afc_timing, afc_phase_div_exp),
    RW_PARAM_FUNC_NAME(afc_timing, afc_rfreq_hi),
    RW_PARAM_FUNC_NAME(afc_timing, afc_rfreq_lo),
    RW_PARAM_FUNC_NAME(afc_timing, afc_n1),
    RW_PARAM_FUNC_NAME(afc_timing, afc_hs_div),
    NULL
};

/************************************************************/
/***************** Export methods functions *****************/
/************************************************************/

static smio_err_e _afc_timing_do_op (void *owner, void *msg);

/* Attach an instance of sm_io to dev_io function pointer */
smio_err_e afc_timing_attach (smio_t *self, void *args)
{
    UNUSED(self);
    UNUSED(args);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Deattach an instance of sm_io to dev_io function pointer */
smio_err_e afc_timing_deattach (smio_t *self)
{
    UNUSED(self);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Export (register) sm_io to handle operations function pointer */
smio_err_e afc_timing_export_ops (smio_t *self,
        const disp_op_t **smio_exp_ops)
{
    UNUSED(self);
    UNUSED(smio_exp_ops);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Unexport (unregister) sm_io to handle operations function pointer */
smio_err_e afc_timing_unexport_ops (smio_t *self)
{
    UNUSED(self);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Generic wrapper for receiving opcodes and arguments to specific funtions function pointer */
/* FIXME: Code repetition! _devio_do_smio_op () function does almost the same!!! */
smio_err_e _afc_timing_do_op (void *owner, void *msg)
{
    UNUSED(owner);
    UNUSED(msg);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

smio_err_e afc_timing_do_op (void *self, void *msg)
{
    return _afc_timing_do_op (self, msg);
}

const smio_ops_t afc_timing_ops = {
    .attach             = &afc_timing_attach,          /* Attach sm_io instance to dev_io */
    .deattach           = &afc_timing_deattach,        /* Deattach sm_io instance to dev_io */
    .export_ops         = &afc_timing_export_ops,      /* Export sm_io operations to dev_io */
    .unexport_ops       = &afc_timing_unexport_ops,    /* Unexport sm_io operations to dev_io */
    .do_op              = &afc_timing_do_op            /* Generic wrapper for handling specific operations */
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
    .init            = &afc_timing_init,
    .shutdown        = &afc_timing_shutdown,
    .config_defaults = &afc_timing_config_defaults
};

SMIO_MOD_DECLARE(AFC_TIMING_SDB_DEVID, AFC_TIMING_SDB_NAME, afc_timing_bootstrap_ops)
