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

RW_PARAM_FUNC(afc_timing, evren) {
	SET_GET_PARAM(afc_timing, 0x0, TIMING, STAT, EVREN, SINGLE_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_ALIVE_GLOBAL_MASK                  (0xffffffff)
#define TIMING_ALIVE_GLOBAL_W(val)                (val)
#define TIMING_ALIVE_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, alive) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, ALIVE, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_FREQ_SAMPLE_RATE_PRESCALE_GLOBAL_MASK                  (0xffffffff)
#define TIMING_FREQ_SAMPLE_RATE_PRESCALE_GLOBAL_W(val)                (val)
#define TIMING_FREQ_SAMPLE_RATE_PRESCALE_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, freq_sample_rate_prescale) {
    SET_GET_PARAM(afc_timing, 0x0, TIMING, FREQ_SAMPLE_RATE_PRESCALE, GLOBAL, MULT_BIT_PARAM,
            , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
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

// AMC Channels

/* This must match the FPGA maximum number of channels */
#define TIMING_AMC_NUM_CHAN                          8
#define TIMING_AMC_CHAN_OFFSET                       0x014 /* 5 32-bit registers */

RW_PARAM_FUNC(afc_timing, amc_en) {
    SET_GET_PARAM_CHANNEL(afc_timing, 0x0, TIMING, AMC0, EN, 
            TIMING_AMC_CHAN_OFFSET, TIMING_AMC_NUM_CHAN,
            SINGLE_BIT_PARAM, , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, amc_pol) {
    SET_GET_PARAM_CHANNEL(afc_timing, 0x0, TIMING, AMC0, POL, 
            TIMING_AMC_CHAN_OFFSET, TIMING_AMC_NUM_CHAN,
            SINGLE_BIT_PARAM, , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, amc_log) {
    SET_GET_PARAM_CHANNEL(afc_timing, 0x0, TIMING, AMC0, LOG, 
            TIMING_AMC_CHAN_OFFSET, TIMING_AMC_NUM_CHAN,
            SINGLE_BIT_PARAM, , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, amc_itl) {
    SET_GET_PARAM_CHANNEL(afc_timing, 0x0, TIMING, AMC0, ITL, 
            TIMING_AMC_CHAN_OFFSET, TIMING_AMC_NUM_CHAN,
            SINGLE_BIT_PARAM, , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, amc_src) {
    SET_GET_PARAM_CHANNEL(afc_timing, 0x0, TIMING, AMC0, SRC, 
            TIMING_AMC_CHAN_OFFSET, TIMING_AMC_NUM_CHAN,
            MULT_BIT_PARAM, , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, amc_dir) {
    SET_GET_PARAM_CHANNEL(afc_timing, 0x0, TIMING, AMC0, DIR, 
            TIMING_AMC_CHAN_OFFSET, TIMING_AMC_NUM_CHAN,
            SINGLE_BIT_PARAM, , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_AMC0_PULSES_GLOBAL_MASK                  (0xffffffff)
#define TIMING_AMC0_PULSES_GLOBAL_W(val)                (val)
#define TIMING_AMC0_PULSES_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, amc_pulses) {
    SET_GET_PARAM_CHANNEL(afc_timing, 0x0, TIMING, AMC0_PULSES, GLOBAL, 
            TIMING_AMC_CHAN_OFFSET, TIMING_AMC_NUM_CHAN,
            MULT_BIT_PARAM, , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_AMC0_EVT_GLOBAL_MASK                  (0xffffffff)
#define TIMING_AMC0_EVT_GLOBAL_W(val)                (val)
#define TIMING_AMC0_EVT_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, amc_evt) {
    SET_GET_PARAM_CHANNEL(afc_timing, 0x0, TIMING, AMC0_EVT, GLOBAL, 
            TIMING_AMC_CHAN_OFFSET, TIMING_AMC_NUM_CHAN,
            MULT_BIT_PARAM, , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_AMC0_DLY_GLOBAL_MASK                  (0xffffffff)
#define TIMING_AMC0_DLY_GLOBAL_W(val)                (val)
#define TIMING_AMC0_DLY_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, amc_dly) {
    SET_GET_PARAM_CHANNEL(afc_timing, 0x0, TIMING, AMC0_DLY, GLOBAL, 
            TIMING_AMC_CHAN_OFFSET, TIMING_AMC_NUM_CHAN,
            MULT_BIT_PARAM, , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_AMC0_WDT_GLOBAL_MASK                  (0xffffffff)
#define TIMING_AMC0_WDT_GLOBAL_W(val)                (val)
#define TIMING_AMC0_WDT_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, amc_wdt) {
    SET_GET_PARAM_CHANNEL(afc_timing, 0x0, TIMING, AMC0_WDT, GLOBAL, 
            TIMING_AMC_CHAN_OFFSET, TIMING_AMC_NUM_CHAN,
            MULT_BIT_PARAM, , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

// FMC 1 Channels

/* This must match the FPGA maximum number of channels */
#define TIMING_FMC1_NUM_CHAN                          5
#define TIMING_FMC1_CHAN_OFFSET                       0x014 /* 5 32-bit registers */

RW_PARAM_FUNC(afc_timing, fmc1_en) {
    SET_GET_PARAM_CHANNEL(afc_timing, 0x0, TIMING, FMC1CH0, EN, 
            TIMING_FMC1_CHAN_OFFSET, TIMING_FMC1_NUM_CHAN,
            SINGLE_BIT_PARAM, , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, fmc1_pol) {
    SET_GET_PARAM_CHANNEL(afc_timing, 0x0, TIMING, FMC1CH0, POL, 
            TIMING_FMC1_CHAN_OFFSET, TIMING_FMC1_NUM_CHAN,
            SINGLE_BIT_PARAM, , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, fmc1_log) {
    SET_GET_PARAM_CHANNEL(afc_timing, 0x0, TIMING, FMC1CH0, LOG, 
            TIMING_FMC1_CHAN_OFFSET, TIMING_FMC1_NUM_CHAN,
            SINGLE_BIT_PARAM, , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, fmc1_itl) {
    SET_GET_PARAM_CHANNEL(afc_timing, 0x0, TIMING, FMC1CH0, ITL, 
            TIMING_FMC1_CHAN_OFFSET, TIMING_FMC1_NUM_CHAN,
            SINGLE_BIT_PARAM, , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, fmc1_src) {
    SET_GET_PARAM_CHANNEL(afc_timing, 0x0, TIMING, FMC1CH0, SRC, 
            TIMING_FMC1_CHAN_OFFSET, TIMING_FMC1_NUM_CHAN,
            MULT_BIT_PARAM, , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, fmc1_dir) {
    SET_GET_PARAM_CHANNEL(afc_timing, 0x0, TIMING, FMC1CH0, DIR, 
            TIMING_FMC1_CHAN_OFFSET, TIMING_FMC1_NUM_CHAN,
            SINGLE_BIT_PARAM, , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_FMC1CH0_PULSES_GLOBAL_MASK                  (0xffffffff)
#define TIMING_FMC1CH0_PULSES_GLOBAL_W(val)                (val)
#define TIMING_FMC1CH0_PULSES_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, fmc1_pulses) {
    SET_GET_PARAM_CHANNEL(afc_timing, 0x0, TIMING, FMC1CH0_PULSES, GLOBAL, 
            TIMING_FMC1_CHAN_OFFSET, TIMING_FMC1_NUM_CHAN,
            MULT_BIT_PARAM, , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_FMC1CH0_EVT_GLOBAL_MASK                  (0xffffffff)
#define TIMING_FMC1CH0_EVT_GLOBAL_W(val)                (val)
#define TIMING_FMC1CH0_EVT_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, fmc1_evt) {
    SET_GET_PARAM_CHANNEL(afc_timing, 0x0, TIMING, FMC1CH0_EVT, GLOBAL, 
            TIMING_FMC1_CHAN_OFFSET, TIMING_FMC1_NUM_CHAN,
            MULT_BIT_PARAM, , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_FMC1CH0_DLY_GLOBAL_MASK                  (0xffffffff)
#define TIMING_FMC1CH0_DLY_GLOBAL_W(val)                (val)
#define TIMING_FMC1CH0_DLY_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, fmc1_dly) {
    SET_GET_PARAM_CHANNEL(afc_timing, 0x0, TIMING, FMC1CH0_DLY, GLOBAL, 
            TIMING_FMC1_CHAN_OFFSET, TIMING_FMC1_NUM_CHAN,
            MULT_BIT_PARAM, , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_FMC1CH0_WDT_GLOBAL_MASK                  (0xffffffff)
#define TIMING_FMC1CH0_WDT_GLOBAL_W(val)                (val)
#define TIMING_FMC1CH0_WDT_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, fmc1_wdt) {
    SET_GET_PARAM_CHANNEL(afc_timing, 0x0, TIMING, FMC1CH0_WDT, GLOBAL, 
            TIMING_FMC1_CHAN_OFFSET, TIMING_FMC1_NUM_CHAN,
            MULT_BIT_PARAM, , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

// FMC 2 Channels

/* This must match the FPGA maximum number of channels */
#define TIMING_FMC2_NUM_CHAN                          5
#define TIMING_FMC2_CHAN_OFFSET                       0x014 /* 5 32-bit registers */

RW_PARAM_FUNC(afc_timing, fmc2_en) {
    SET_GET_PARAM_CHANNEL(afc_timing, 0x0, TIMING, FMC2CH0, EN, 
            TIMING_FMC2_CHAN_OFFSET, TIMING_FMC2_NUM_CHAN,
            SINGLE_BIT_PARAM, , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, fmc2_pol) {
    SET_GET_PARAM_CHANNEL(afc_timing, 0x0, TIMING, FMC2CH0, POL, 
            TIMING_FMC2_CHAN_OFFSET, TIMING_FMC2_NUM_CHAN,
            SINGLE_BIT_PARAM, , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, fmc2_log) {
    SET_GET_PARAM_CHANNEL(afc_timing, 0x0, TIMING, FMC2CH0, LOG, 
            TIMING_FMC2_CHAN_OFFSET, TIMING_FMC2_NUM_CHAN,
            SINGLE_BIT_PARAM, , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, fmc2_itl) {
    SET_GET_PARAM_CHANNEL(afc_timing, 0x0, TIMING, FMC2CH0, ITL, 
            TIMING_FMC2_CHAN_OFFSET, TIMING_FMC2_NUM_CHAN,
            SINGLE_BIT_PARAM, , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, fmc2_src) {
    SET_GET_PARAM_CHANNEL(afc_timing, 0x0, TIMING, FMC2CH0, SRC, 
            TIMING_FMC2_CHAN_OFFSET, TIMING_FMC2_NUM_CHAN,
            MULT_BIT_PARAM, , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_timing, fmc2_dir) {
    SET_GET_PARAM_CHANNEL(afc_timing, 0x0, TIMING, FMC2CH0, DIR, 
            TIMING_FMC2_CHAN_OFFSET, TIMING_FMC2_NUM_CHAN,
            SINGLE_BIT_PARAM, , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_FMC2CH0_PULSES_GLOBAL_MASK                  (0xffffffff)
#define TIMING_FMC2CH0_PULSES_GLOBAL_W(val)                (val)
#define TIMING_FMC2CH0_PULSES_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, fmc2_pulses) {
    SET_GET_PARAM_CHANNEL(afc_timing, 0x0, TIMING, FMC2CH0_PULSES, GLOBAL, 
            TIMING_FMC2_CHAN_OFFSET, TIMING_FMC2_NUM_CHAN,
            MULT_BIT_PARAM, , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_FMC2CH0_EVT_GLOBAL_MASK                  (0xffffffff)
#define TIMING_FMC2CH0_EVT_GLOBAL_W(val)                (val)
#define TIMING_FMC2CH0_EVT_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, fmc2_evt) {
    SET_GET_PARAM_CHANNEL(afc_timing, 0x0, TIMING, FMC2CH0_EVT, GLOBAL, 
            TIMING_FMC2_CHAN_OFFSET, TIMING_FMC2_NUM_CHAN,
            MULT_BIT_PARAM, , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_FMC2CH0_DLY_GLOBAL_MASK                  (0xffffffff)
#define TIMING_FMC2CH0_DLY_GLOBAL_W(val)                (val)
#define TIMING_FMC2CH0_DLY_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, fmc2_dly) {
    SET_GET_PARAM_CHANNEL(afc_timing, 0x0, TIMING, FMC2CH0_DLY, GLOBAL, 
            TIMING_FMC2_CHAN_OFFSET, TIMING_FMC2_NUM_CHAN,
            MULT_BIT_PARAM, , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define TIMING_FMC2CH0_WDT_GLOBAL_MASK                  (0xffffffff)
#define TIMING_FMC2CH0_WDT_GLOBAL_W(val)                (val)
#define TIMING_FMC2CH0_WDT_GLOBAL_R(val)                (val)
RW_PARAM_FUNC(afc_timing, fmc2_wdt) {
    SET_GET_PARAM_CHANNEL(afc_timing, 0x0, TIMING, FMC2CH0_WDT, GLOBAL, 
            TIMING_FMC2_CHAN_OFFSET, TIMING_FMC2_NUM_CHAN,
            MULT_BIT_PARAM, , , NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
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

#define TIMING_NAVG_MIN 0
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
    RW_PARAM_FUNC_NAME(afc_timing, evren),
    RW_PARAM_FUNC_NAME(afc_timing, alive),
    RW_PARAM_FUNC_NAME(afc_timing, freq_sample_rate_prescale),
    RW_PARAM_FUNC_NAME(afc_timing, amc_en),
    RW_PARAM_FUNC_NAME(afc_timing, amc_pol),
    RW_PARAM_FUNC_NAME(afc_timing, amc_log),
    RW_PARAM_FUNC_NAME(afc_timing, amc_itl),
    RW_PARAM_FUNC_NAME(afc_timing, amc_src),
    RW_PARAM_FUNC_NAME(afc_timing, amc_dir),
    RW_PARAM_FUNC_NAME(afc_timing, amc_pulses),
    RW_PARAM_FUNC_NAME(afc_timing, amc_evt),
    RW_PARAM_FUNC_NAME(afc_timing, amc_dly),
    RW_PARAM_FUNC_NAME(afc_timing, amc_wdt),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_en),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_pol),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_log),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_itl),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_src),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_dir),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_pulses),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_evt),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_dly),
    RW_PARAM_FUNC_NAME(afc_timing, fmc1_wdt),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_en),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_pol),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_log),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_itl),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_src),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_dir),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_pulses),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_evt),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_dly),
    RW_PARAM_FUNC_NAME(afc_timing, fmc2_wdt),
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
