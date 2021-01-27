/*
 * Copyright (C) 2021 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "halcs_server.h"
/* Private headers */
#include "sm_io_fofb_ctrl_codes.h"
#include "sm_io_fofb_ctrl_defaults.h"
#include "sm_io_fofb_ctrl_exports.h"
#include "sm_io_fofb_ctrl_core.h"
#include "sm_io_fofb_ctrl_exp.h"
#include "hw/wb_fofb_cc_regs.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, SM_IO, "[sm_io:fofb_ctrl_exp]",              \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)                  \
    ASSERT_HAL_ALLOC(ptr, SM_IO, "[sm_io:fofb_ctrl_exp]",                      \
            smio_err_str(SMIO_ERR_ALLOC),                                      \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                               \
    CHECK_HAL_ERR(err, SM_IO, "[sm_io:fofb_ctrl_exp]",                         \
            smio_err_str (err_type))

/******************************************************************/
/*****************  Specific FOFB_CTRL Operations *****************/
/******************************************************************/

/* Regular registers */

#define FOFB_CC_ACT_PART_MIN      0
#define FOFB_CC_ACT_PART_MAX      1
RW_PARAM_FUNC(fofb_ctrl, act_part) {
    SET_GET_PARAM(fofb_ctrl, 0x0, FOFB_CC, CFG_VAL, ACT_PART, SINGLE_BIT_PARAM,
            FOFB_CC_ACT_PART_MIN, FOFB_CC_ACT_PART_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FOFB_CC_ERR_CLR_MIN      0
#define FOFB_CC_ERR_CLR_MAX      1
RW_PARAM_FUNC(fofb_ctrl, err_clr) {
    SET_GET_PARAM(fofb_ctrl, 0x0, FOFB_CC, CFG_VAL, ERR_CLR, SINGLE_BIT_PARAM,
            FOFB_CC_ERR_CLR_MIN, FOFB_CC_ERR_CLR_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FOFB_CC_CC_ENABLE_MIN      0
#define FOFB_CC_CC_ENABLE_MAX      1
RW_PARAM_FUNC(fofb_ctrl, cc_enable) {
    SET_GET_PARAM(fofb_ctrl, 0x0, FOFB_CC, CFG_VAL, CC_ENABLE, SINGLE_BIT_PARAM,
            FOFB_CC_CC_ENABLE_MIN, FOFB_CC_CC_ENABLE_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FOFB_CC_TFS_OVERRIDE_MIN      0
#define FOFB_CC_TFS_OVERRIDE_MAX      1
RW_PARAM_FUNC(fofb_ctrl, tfs_override) {
    SET_GET_PARAM(fofb_ctrl, 0x0, FOFB_CC, CFG_VAL, TFS_OVERRIDE, SINGLE_BIT_PARAM,
            FOFB_CC_TFS_OVERRIDE_MIN, FOFB_CC_TFS_OVERRIDE_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

/* Read/Write registers. Read/Write from bus, read from device */

#define FOFB_CC_BPM_ID_R(val)       (val)
#define FOFB_CC_BPM_ID_W(val)       (val)
#define FOFB_CC_BPM_ID_MASK         ((1ULL<<32)-1)
RW_PARAM_FUNC(fofb_ctrl, bpm_id) {
    SET_GET_PARAM(fofb_ctrl, 0x0, FOFB_CC, BPM_ID, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FOFB_CC_TIME_FRAME_LEN_R(val)       (val)
#define FOFB_CC_TIME_FRAME_LEN_W(val)       (val)
#define FOFB_CC_TIME_FRAME_LEN_MASK         ((1ULL<<32)-1)
RW_PARAM_FUNC(fofb_ctrl, time_frame_len) {
    SET_GET_PARAM(fofb_ctrl, 0x0, FOFB_CC, TIME_FRAME_LEN, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FOFB_CC_MGT_POWERDOWN_R(val)       (val)
#define FOFB_CC_MGT_POWERDOWN_W(val)       (val)
#define FOFB_CC_MGT_POWERDOWN_MASK         ((1ULL<<32)-1)
RW_PARAM_FUNC(fofb_ctrl, mgt_powerdown) {
    SET_GET_PARAM(fofb_ctrl, 0x0, FOFB_CC, MGT_POWERDOWN, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FOFB_CC_MGT_LOOPBACK_R(val)       (val)
#define FOFB_CC_MGT_LOOPBACK_W(val)       (val)
#define FOFB_CC_MGT_LOOPBACK_MASK         ((1ULL<<32)-1)
RW_PARAM_FUNC(fofb_ctrl, mgt_loopback) {
    SET_GET_PARAM(fofb_ctrl, 0x0, FOFB_CC, MGT_LOOPBACK, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FOFB_CC_TIME_FRAME_DLY_R(val)       (val)
#define FOFB_CC_TIME_FRAME_DLY_W(val)       (val)
#define FOFB_CC_TIME_FRAME_DLY_MASK         ((1ULL<<32)-1)
RW_PARAM_FUNC(fofb_ctrl, time_frame_dly) {
    SET_GET_PARAM(fofb_ctrl, 0x0, FOFB_CC, TIME_FRAME_DLY, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FOFB_CC_GOLDEN_ORB_X_R(val)       (val)
#define FOFB_CC_GOLDEN_ORB_X_W(val)       (val)
#define FOFB_CC_GOLDEN_ORB_X_MASK         ((1ULL<<32)-1)
RW_PARAM_FUNC(fofb_ctrl, golden_orb_x) {
    SET_GET_PARAM(fofb_ctrl, 0x0, FOFB_CC, GOLDEN_ORB_X, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FOFB_CC_GOLDEN_ORB_Y_R(val)       (val)
#define FOFB_CC_GOLDEN_ORB_Y_W(val)       (val)
#define FOFB_CC_GOLDEN_ORB_Y_MASK         ((1ULL<<32)-1)
RW_PARAM_FUNC(fofb_ctrl, golden_orb_y) {
    SET_GET_PARAM(fofb_ctrl, 0x0, FOFB_CC, GOLDEN_ORB_Y, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FOFB_CC_CUST_FEATURE_R(val)       (val)
#define FOFB_CC_CUST_FEATURE_W(val)       (val)
#define FOFB_CC_CUST_FEATURE_MASK         ((1ULL<<32)-1)
RW_PARAM_FUNC(fofb_ctrl, cust_feature) {
    SET_GET_PARAM(fofb_ctrl, 0x0, FOFB_CC, CUST_FEATURE, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FOFB_CC_RXPOLARITY_R(val)       (val)
#define FOFB_CC_RXPOLARITY_W(val)       (val)
#define FOFB_CC_RXPOLARITY_MASK         ((1ULL<<32)-1)
RW_PARAM_FUNC(fofb_ctrl, rx_polarity) {
    SET_GET_PARAM(fofb_ctrl, 0x0, FOFB_CC, RXPOLARITY, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FOFB_CC_PAYLOADSEL_R(val)       (val)
#define FOFB_CC_PAYLOADSEL_W(val)       (val)
#define FOFB_CC_PAYLOADSEL_MASK         ((1ULL<<32)-1)
RW_PARAM_FUNC(fofb_ctrl, payloadsel) {
    SET_GET_PARAM(fofb_ctrl, 0x0, FOFB_CC, PAYLOADSEL, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FOFB_CC_FOFBDATASEL_R(val)       (val)
#define FOFB_CC_FOFBDATASEL_W(val)       (val)
#define FOFB_CC_FOFBDATASEL_MASK         ((1ULL<<32)-1)
RW_PARAM_FUNC(fofb_ctrl, fofbdatasel) {
    SET_GET_PARAM(fofb_ctrl, 0x0, FOFB_CC, FOFBDATASEL, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

/* Read-Only registers. Read from bus, write from device */

#define FOFB_CC_FIRMWARE_VER_R(val)       (val)
#define FOFB_CC_FIRMWARE_VER_W(val)       (val)
#define FOFB_CC_FIRMWARE_VER_MASK         ((1ULL<<32)-1)
RW_PARAM_FUNC(fofb_ctrl, firmware_ver) {
    SET_GET_PARAM(fofb_ctrl, 0x0, FOFB_CC, FIRMWARE_VER, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FOFB_CC_SYS_STATUS_R(val)       (val)
#define FOFB_CC_SYS_STATUS_W(val)       (val)
#define FOFB_CC_SYS_STATUS_MASK         ((1ULL<<32)-1)
RW_PARAM_FUNC(fofb_ctrl, sys_status) {
    SET_GET_PARAM(fofb_ctrl, 0x0, FOFB_CC, SYS_STATUS, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FOFB_CC_LINK_PARTNER_1_R(val)       (val)
#define FOFB_CC_LINK_PARTNER_1_W(val)       (val)
#define FOFB_CC_LINK_PARTNER_1_MASK         ((1ULL<<32)-1)
RW_PARAM_FUNC(fofb_ctrl, link_partner_1) {
    SET_GET_PARAM(fofb_ctrl, 0x0, FOFB_CC, LINK_PARTNER_1, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FOFB_CC_LINK_PARTNER_2_R(val)       (val)
#define FOFB_CC_LINK_PARTNER_2_W(val)       (val)
#define FOFB_CC_LINK_PARTNER_2_MASK         ((1ULL<<32)-1)
RW_PARAM_FUNC(fofb_ctrl, link_partner_2) {
    SET_GET_PARAM(fofb_ctrl, 0x0, FOFB_CC, LINK_PARTNER_2, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FOFB_CC_LINK_PARTNER_3_R(val)       (val)
#define FOFB_CC_LINK_PARTNER_3_W(val)       (val)
#define FOFB_CC_LINK_PARTNER_3_MASK         ((1ULL<<32)-1)
RW_PARAM_FUNC(fofb_ctrl, link_partner_3) {
    SET_GET_PARAM(fofb_ctrl, 0x0, FOFB_CC, LINK_PARTNER_3, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FOFB_CC_LINK_PARTNER_4_R(val)       (val)
#define FOFB_CC_LINK_PARTNER_4_W(val)       (val)
#define FOFB_CC_LINK_PARTNER_4_MASK         ((1ULL<<32)-1)
RW_PARAM_FUNC(fofb_ctrl, link_partner_4) {
    SET_GET_PARAM(fofb_ctrl, 0x0, FOFB_CC, LINK_PARTNER_4, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FOFB_CC_LINK_UP_R(val)       (val)
#define FOFB_CC_LINK_UP_W(val)       (val)
#define FOFB_CC_LINK_UP_MASK         ((1ULL<<32)-1)
RW_PARAM_FUNC(fofb_ctrl, link_up) {
    SET_GET_PARAM(fofb_ctrl, 0x0, FOFB_CC, LINK_UP, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FOFB_CC_TIME_FRAME_COUNT_R(val)       (val)
#define FOFB_CC_TIME_FRAME_COUNT_W(val)       (val)
#define FOFB_CC_TIME_FRAME_COUNT_MASK         ((1ULL<<32)-1)
RW_PARAM_FUNC(fofb_ctrl, time_frame_count) {
    SET_GET_PARAM(fofb_ctrl, 0x0, FOFB_CC, TIME_FRAME_COUNT, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FOFB_CC_HARD_ERR_CNT_1_R(val)       (val)
#define FOFB_CC_HARD_ERR_CNT_1_W(val)       (val)
#define FOFB_CC_HARD_ERR_CNT_1_MASK         ((1ULL<<32)-1)
RW_PARAM_FUNC(fofb_ctrl, hard_err_cnt_1) {
    SET_GET_PARAM(fofb_ctrl, 0x0, FOFB_CC, HARD_ERR_CNT_1, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FOFB_CC_HARD_ERR_CNT_2_R(val)       (val)
#define FOFB_CC_HARD_ERR_CNT_2_W(val)       (val)
#define FOFB_CC_HARD_ERR_CNT_2_MASK         ((1ULL<<32)-1)
RW_PARAM_FUNC(fofb_ctrl, hard_err_cnt_2) {
    SET_GET_PARAM(fofb_ctrl, 0x0, FOFB_CC, HARD_ERR_CNT_2, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FOFB_CC_HARD_ERR_CNT_3_R(val)       (val)
#define FOFB_CC_HARD_ERR_CNT_3_W(val)       (val)
#define FOFB_CC_HARD_ERR_CNT_3_MASK         ((1ULL<<32)-1)
RW_PARAM_FUNC(fofb_ctrl, hard_err_cnt_3) {
    SET_GET_PARAM(fofb_ctrl, 0x0, FOFB_CC, HARD_ERR_CNT_3, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FOFB_CC_HARD_ERR_CNT_4_R(val)       (val)
#define FOFB_CC_HARD_ERR_CNT_4_W(val)       (val)
#define FOFB_CC_HARD_ERR_CNT_4_MASK         ((1ULL<<32)-1)
RW_PARAM_FUNC(fofb_ctrl, hard_err_cnt_4) {
    SET_GET_PARAM(fofb_ctrl, 0x0, FOFB_CC, HARD_ERR_CNT_4, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FOFB_CC_SOFT_ERR_CNT_1_R(val)       (val)
#define FOFB_CC_SOFT_ERR_CNT_1_W(val)       (val)
#define FOFB_CC_SOFT_ERR_CNT_1_MASK         ((1ULL<<32)-1)
RW_PARAM_FUNC(fofb_ctrl, soft_err_cnt_1) {
    SET_GET_PARAM(fofb_ctrl, 0x0, FOFB_CC, SOFT_ERR_CNT_1, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FOFB_CC_SOFT_ERR_CNT_2_R(val)       (val)
#define FOFB_CC_SOFT_ERR_CNT_2_W(val)       (val)
#define FOFB_CC_SOFT_ERR_CNT_2_MASK         ((1ULL<<32)-1)
RW_PARAM_FUNC(fofb_ctrl, soft_err_cnt_2) {
    SET_GET_PARAM(fofb_ctrl, 0x0, FOFB_CC, SOFT_ERR_CNT_2, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FOFB_CC_SOFT_ERR_CNT_3_R(val)       (val)
#define FOFB_CC_SOFT_ERR_CNT_3_W(val)       (val)
#define FOFB_CC_SOFT_ERR_CNT_3_MASK         ((1ULL<<32)-1)
RW_PARAM_FUNC(fofb_ctrl, soft_err_cnt_3) {
    SET_GET_PARAM(fofb_ctrl, 0x0, FOFB_CC, SOFT_ERR_CNT_3, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FOFB_CC_SOFT_ERR_CNT_4_R(val)       (val)
#define FOFB_CC_SOFT_ERR_CNT_4_W(val)       (val)
#define FOFB_CC_SOFT_ERR_CNT_4_MASK         ((1ULL<<32)-1)
RW_PARAM_FUNC(fofb_ctrl, soft_err_cnt_4) {
    SET_GET_PARAM(fofb_ctrl, 0x0, FOFB_CC, SOFT_ERR_CNT_4, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FOFB_CC_FRAME_ERR_CNT_1_R(val)       (val)
#define FOFB_CC_FRAME_ERR_CNT_1_W(val)       (val)
#define FOFB_CC_FRAME_ERR_CNT_1_MASK         ((1ULL<<32)-1)
RW_PARAM_FUNC(fofb_ctrl, frame_err_cnt_1) {
    SET_GET_PARAM(fofb_ctrl, 0x0, FOFB_CC, FRAME_ERR_CNT_1, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FOFB_CC_FRAME_ERR_CNT_2_R(val)       (val)
#define FOFB_CC_FRAME_ERR_CNT_2_W(val)       (val)
#define FOFB_CC_FRAME_ERR_CNT_2_MASK         ((1ULL<<32)-1)
RW_PARAM_FUNC(fofb_ctrl, frame_err_cnt_2) {
    SET_GET_PARAM(fofb_ctrl, 0x0, FOFB_CC, FRAME_ERR_CNT_2, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FOFB_CC_FRAME_ERR_CNT_3_R(val)       (val)
#define FOFB_CC_FRAME_ERR_CNT_3_W(val)       (val)
#define FOFB_CC_FRAME_ERR_CNT_3_MASK         ((1ULL<<32)-1)
RW_PARAM_FUNC(fofb_ctrl, frame_err_cnt_3) {
    SET_GET_PARAM(fofb_ctrl, 0x0, FOFB_CC, FRAME_ERR_CNT_3, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FOFB_CC_FRAME_ERR_CNT_4_R(val)       (val)
#define FOFB_CC_FRAME_ERR_CNT_4_W(val)       (val)
#define FOFB_CC_FRAME_ERR_CNT_4_MASK         ((1ULL<<32)-1)
RW_PARAM_FUNC(fofb_ctrl, frame_err_cnt_4) {
    SET_GET_PARAM(fofb_ctrl, 0x0, FOFB_CC, FRAME_ERR_CNT_4, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FOFB_CC_RX_PCK_CNT_1_R(val)       (val)
#define FOFB_CC_RX_PCK_CNT_1_W(val)       (val)
#define FOFB_CC_RX_PCK_CNT_1_MASK         ((1ULL<<32)-1)
RW_PARAM_FUNC(fofb_ctrl, rx_pck_cnt_1) {
    SET_GET_PARAM(fofb_ctrl, 0x0, FOFB_CC, RX_PCK_CNT_1, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FOFB_CC_RX_PCK_CNT_2_R(val)       (val)
#define FOFB_CC_RX_PCK_CNT_2_W(val)       (val)
#define FOFB_CC_RX_PCK_CNT_2_MASK         ((1ULL<<32)-1)
RW_PARAM_FUNC(fofb_ctrl, rx_pck_cnt_2) {
    SET_GET_PARAM(fofb_ctrl, 0x0, FOFB_CC, RX_PCK_CNT_2, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FOFB_CC_RX_PCK_CNT_3_R(val)       (val)
#define FOFB_CC_RX_PCK_CNT_3_W(val)       (val)
#define FOFB_CC_RX_PCK_CNT_3_MASK         ((1ULL<<32)-1)
RW_PARAM_FUNC(fofb_ctrl, rx_pck_cnt_3) {
    SET_GET_PARAM(fofb_ctrl, 0x0, FOFB_CC, RX_PCK_CNT_3, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FOFB_CC_RX_PCK_CNT_4_R(val)       (val)
#define FOFB_CC_RX_PCK_CNT_4_W(val)       (val)
#define FOFB_CC_RX_PCK_CNT_4_MASK         ((1ULL<<32)-1)
RW_PARAM_FUNC(fofb_ctrl, rx_pck_cnt_4) {
    SET_GET_PARAM(fofb_ctrl, 0x0, FOFB_CC, RX_PCK_CNT_4, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FOFB_CC_TX_PCK_CNT_1_R(val)       (val)
#define FOFB_CC_TX_PCK_CNT_1_W(val)       (val)
#define FOFB_CC_TX_PCK_CNT_1_MASK         ((1ULL<<32)-1)
RW_PARAM_FUNC(fofb_ctrl, tx_pck_cnt_1) {
    SET_GET_PARAM(fofb_ctrl, 0x0, FOFB_CC, TX_PCK_CNT_1, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FOFB_CC_TX_PCK_CNT_2_R(val)       (val)
#define FOFB_CC_TX_PCK_CNT_2_W(val)       (val)
#define FOFB_CC_TX_PCK_CNT_2_MASK         ((1ULL<<32)-1)
RW_PARAM_FUNC(fofb_ctrl, tx_pck_cnt_2) {
    SET_GET_PARAM(fofb_ctrl, 0x0, FOFB_CC, TX_PCK_CNT_2, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FOFB_CC_TX_PCK_CNT_3_R(val)       (val)
#define FOFB_CC_TX_PCK_CNT_3_W(val)       (val)
#define FOFB_CC_TX_PCK_CNT_3_MASK         ((1ULL<<32)-1)
RW_PARAM_FUNC(fofb_ctrl, tx_pck_cnt_3) {
    SET_GET_PARAM(fofb_ctrl, 0x0, FOFB_CC, TX_PCK_CNT_3, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FOFB_CC_TX_PCK_CNT_4_R(val)       (val)
#define FOFB_CC_TX_PCK_CNT_4_W(val)       (val)
#define FOFB_CC_TX_PCK_CNT_4_MASK         ((1ULL<<32)-1)
RW_PARAM_FUNC(fofb_ctrl, tx_pck_cnt_4) {
    SET_GET_PARAM(fofb_ctrl, 0x0, FOFB_CC, TX_PCK_CNT_4, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FOFB_CC_FOD_PROCESS_TIME_R(val)       (val)
#define FOFB_CC_FOD_PROCESS_TIME_W(val)       (val)
#define FOFB_CC_FOD_PROCESS_TIME_MASK         ((1ULL<<32)-1)
RW_PARAM_FUNC(fofb_ctrl, fod_process_time) {
    SET_GET_PARAM(fofb_ctrl, 0x0, FOFB_CC, FOD_PROCESS_TIME, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FOFB_CC_BPM_COUNT_R(val)       (val)
#define FOFB_CC_BPM_COUNT_W(val)       (val)
#define FOFB_CC_BPM_COUNT_MASK         ((1ULL<<32)-1)
RW_PARAM_FUNC(fofb_ctrl, bpm_count) {
    SET_GET_PARAM(fofb_ctrl, 0x0, FOFB_CC, BPM_COUNT, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FOFB_CC_BPM_ID_RDBACK_R(val)       (val)
#define FOFB_CC_BPM_ID_RDBACK_W(val)       (val)
#define FOFB_CC_BPM_ID_RDBACK_MASK         ((1ULL<<32)-1)
RW_PARAM_FUNC(fofb_ctrl, bpm_id_rdback) {
    SET_GET_PARAM(fofb_ctrl, 0x0, FOFB_CC, BPM_ID_RDBACK, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FOFB_CC_TF_LENGTH_RDBACK_R(val)       (val)
#define FOFB_CC_TF_LENGTH_RDBACK_W(val)       (val)
#define FOFB_CC_TF_LENGTH_RDBACK_MASK         ((1ULL<<32)-1)
RW_PARAM_FUNC(fofb_ctrl, tf_length_rdback) {
    SET_GET_PARAM(fofb_ctrl, 0x0, FOFB_CC, TF_LENGTH_RDBACK, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FOFB_CC_POWERDOWN_RDBACK_R(val)       (val)
#define FOFB_CC_POWERDOWN_RDBACK_W(val)       (val)
#define FOFB_CC_POWERDOWN_RDBACK_MASK         ((1ULL<<32)-1)
RW_PARAM_FUNC(fofb_ctrl, powerdown_rdback) {
    SET_GET_PARAM(fofb_ctrl, 0x0, FOFB_CC, POWERDOWN_RDBACK, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FOFB_CC_LOOPBACK_RDBACK_R(val)       (val)
#define FOFB_CC_LOOPBACK_RDBACK_W(val)       (val)
#define FOFB_CC_LOOPBACK_RDBACK_MASK         ((1ULL<<32)-1)
RW_PARAM_FUNC(fofb_ctrl, loopback_rdback) {
    SET_GET_PARAM(fofb_ctrl, 0x0, FOFB_CC, LOOPBACK_RDBACK, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define FOFB_CC_FAIVAL_RDBACK_R(val)       (val)
#define FOFB_CC_FAIVAL_RDBACK_W(val)       (val)
#define FOFB_CC_FAIVAL_RDBACK_MASK         ((1ULL<<32)-1)
RW_PARAM_FUNC(fofb_ctrl, faival_rdback) {
    SET_GET_PARAM(fofb_ctrl, 0x0, FOFB_CC, FAIVAL_RDBACK, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

/* Exported function pointers */
const disp_table_func_fp fofb_ctrl_exp_fp [] = {
    RW_PARAM_FUNC_NAME(fofb_ctrl, act_part),
    RW_PARAM_FUNC_NAME(fofb_ctrl, err_clr),
    RW_PARAM_FUNC_NAME(fofb_ctrl, cc_enable),
    RW_PARAM_FUNC_NAME(fofb_ctrl, tfs_override),
    RW_PARAM_FUNC_NAME(fofb_ctrl, bpm_id),
    RW_PARAM_FUNC_NAME(fofb_ctrl, time_frame_len),
    RW_PARAM_FUNC_NAME(fofb_ctrl, mgt_powerdown),
    RW_PARAM_FUNC_NAME(fofb_ctrl, mgt_loopback),
    RW_PARAM_FUNC_NAME(fofb_ctrl, time_frame_dly),
    RW_PARAM_FUNC_NAME(fofb_ctrl, golden_orb_x),
    RW_PARAM_FUNC_NAME(fofb_ctrl, golden_orb_y),
    RW_PARAM_FUNC_NAME(fofb_ctrl, cust_feature),
    RW_PARAM_FUNC_NAME(fofb_ctrl, rx_polarity),
    RW_PARAM_FUNC_NAME(fofb_ctrl, payloadsel),
    RW_PARAM_FUNC_NAME(fofb_ctrl, fofbdatasel),
    RW_PARAM_FUNC_NAME(fofb_ctrl, firmware_ver),
    RW_PARAM_FUNC_NAME(fofb_ctrl, sys_status),
    RW_PARAM_FUNC_NAME(fofb_ctrl, link_partner_1),
    RW_PARAM_FUNC_NAME(fofb_ctrl, link_partner_2),
    RW_PARAM_FUNC_NAME(fofb_ctrl, link_partner_3),
    RW_PARAM_FUNC_NAME(fofb_ctrl, link_partner_4),
    RW_PARAM_FUNC_NAME(fofb_ctrl, link_up),
    RW_PARAM_FUNC_NAME(fofb_ctrl, time_frame_count),
    RW_PARAM_FUNC_NAME(fofb_ctrl, hard_err_cnt_1),
    RW_PARAM_FUNC_NAME(fofb_ctrl, hard_err_cnt_2),
    RW_PARAM_FUNC_NAME(fofb_ctrl, hard_err_cnt_3),
    RW_PARAM_FUNC_NAME(fofb_ctrl, hard_err_cnt_4),
    RW_PARAM_FUNC_NAME(fofb_ctrl, soft_err_cnt_1),
    RW_PARAM_FUNC_NAME(fofb_ctrl, soft_err_cnt_2),
    RW_PARAM_FUNC_NAME(fofb_ctrl, soft_err_cnt_3),
    RW_PARAM_FUNC_NAME(fofb_ctrl, soft_err_cnt_4),
    RW_PARAM_FUNC_NAME(fofb_ctrl, frame_err_cnt_1),
    RW_PARAM_FUNC_NAME(fofb_ctrl, frame_err_cnt_2),
    RW_PARAM_FUNC_NAME(fofb_ctrl, frame_err_cnt_3),
    RW_PARAM_FUNC_NAME(fofb_ctrl, frame_err_cnt_4),
    RW_PARAM_FUNC_NAME(fofb_ctrl, rx_pck_cnt_1),
    RW_PARAM_FUNC_NAME(fofb_ctrl, rx_pck_cnt_2),
    RW_PARAM_FUNC_NAME(fofb_ctrl, rx_pck_cnt_3),
    RW_PARAM_FUNC_NAME(fofb_ctrl, rx_pck_cnt_4),
    RW_PARAM_FUNC_NAME(fofb_ctrl, tx_pck_cnt_1),
    RW_PARAM_FUNC_NAME(fofb_ctrl, tx_pck_cnt_2),
    RW_PARAM_FUNC_NAME(fofb_ctrl, tx_pck_cnt_3),
    RW_PARAM_FUNC_NAME(fofb_ctrl, tx_pck_cnt_4),
    RW_PARAM_FUNC_NAME(fofb_ctrl, fod_process_time),
    RW_PARAM_FUNC_NAME(fofb_ctrl, bpm_count),
    RW_PARAM_FUNC_NAME(fofb_ctrl, bpm_id_rdback),
    RW_PARAM_FUNC_NAME(fofb_ctrl, tf_length_rdback),
    RW_PARAM_FUNC_NAME(fofb_ctrl, powerdown_rdback),
    RW_PARAM_FUNC_NAME(fofb_ctrl, loopback_rdback),
    RW_PARAM_FUNC_NAME(fofb_ctrl, faival_rdback),
    NULL
};

/************************************************************/
/***************** Export methods functions *****************/
/************************************************************/

static smio_err_e _fofb_ctrl_do_op (void *owner, void *msg);

/* Attach an instance of sm_io to dev_io function pointer */
smio_err_e fofb_ctrl_attach (smio_t *self, void *args)
{
    UNUSED(self);
    UNUSED(args);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Deattach an instance of sm_io to dev_io function pointer */
smio_err_e fofb_ctrl_deattach (smio_t *self)
{
    UNUSED(self);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Export (register) sm_io to handle operations function pointer */
smio_err_e fofb_ctrl_export_ops (smio_t *self,
        const disp_op_t **smio_exp_ops)
{
    UNUSED(self);
    UNUSED(smio_exp_ops);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Unexport (unregister) sm_io to handle operations function pointer */
smio_err_e fofb_ctrl_unexport_ops (smio_t *self)
{
    UNUSED(self);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Generic wrapper for receiving opcodes and arguments to specific funtions function pointer */
/* FIXME: Code repetition! _devio_do_smio_op () function does almost the same!!! */
smio_err_e _fofb_ctrl_do_op (void *owner, void *msg)
{
    UNUSED(owner);
    UNUSED(msg);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

smio_err_e fofb_ctrl_do_op (void *self, void *msg)
{
    return _fofb_ctrl_do_op (self, msg);
}

const smio_ops_t fofb_ctrl_ops = {
    .attach             = &fofb_ctrl_attach,          /* Attach sm_io instance to dev_io */
    .deattach           = &fofb_ctrl_deattach,        /* Deattach sm_io instance to dev_io */
    .export_ops         = &fofb_ctrl_export_ops,      /* Export sm_io operations to dev_io */
    .unexport_ops       = &fofb_ctrl_unexport_ops,    /* Unexport sm_io operations to dev_io */
    .do_op              = &fofb_ctrl_do_op            /* Generic wrapper for handling specific operations */
};

/************************************************************/
/****************** Bootstrap Operations ********************/
/************************************************************/

smio_err_e fofb_ctrl_init (smio_t * self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fofb_ctrl_exp] Initializing fofb_ctrl\n");

    smio_err_e err = SMIO_SUCCESS;

    err = smio_set_id (self, FOFB_CTRL_SDB_DEVID);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO id", err_set_id);
    err = smio_set_name (self, FOFB_CTRL_SDB_NAME);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO name", err_set_name);

    /* Set SMIO ops pointers */
    err = smio_set_ops (self, &fofb_ctrl_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO operations",
            err_smio_set_ops);
    err = smio_set_thsafe_client_ops (self, &smio_thsafe_client_zmq_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO thsafe operations",
            err_smio_set_thsafe_ops);

    /* disp_op_t structure is const and all of the functions performing on it
     * obviously receives a const argument, but here (and only on the SMIO
     * initialization) we need to make an exception if we want to keep the
     * functions' description and the function pointers separate */
    err = smio_init_exp_ops (self, (disp_op_t **) fofb_ctrl_exp_ops, fofb_ctrl_exp_fp);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not fill SMIO "
            "function descriptors with the callbacks", err_fill_desc);

    err = smio_set_exp_ops (self, fofb_ctrl_exp_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO exported operations",
            err_smio_set_exp_ops);

    /* Initialize specific structure */
    smio_fofb_ctrl_t *smio_handler = smio_fofb_ctrl_new (self);
    ASSERT_ALLOC(smio_handler, err_smio_handler_alloc, SMIO_ERR_ALLOC);
    err = smio_set_handler (self, smio_handler);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO handler",
            err_smio_set_handler);

    return err;

err_smio_set_handler:
    smio_fofb_ctrl_destroy (&smio_handler);
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

/* Destroy sm_io instance of fofb_ctrl */
smio_err_e fofb_ctrl_shutdown (smio_t *self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fofb_ctrl_exp] Shutting down fofb_ctrl\n");

    smio_err_e err = SMIO_SUCCESS;
    smio_fofb_ctrl_t *fofb_ctrl = smio_get_handler (self);
    ASSERT_TEST(fofb_ctrl != NULL, "Could not get FOFB_CTRL handler",
            err_fofb_ctrl_handler, SMIO_ERR_ALLOC /* FIXME: improve return code */);

    /* Destroy SMIO instance */
    smio_fofb_ctrl_destroy (&fofb_ctrl);
    /* Nullify operation pointers */
    smio_set_exp_ops (self, NULL);
    smio_set_thsafe_client_ops (self, NULL);
    smio_set_ops (self, NULL);

err_fofb_ctrl_handler:
    return err;
}

const smio_bootstrap_ops_t fofb_ctrl_bootstrap_ops = {
    .init            = &fofb_ctrl_init,
    .shutdown        = &fofb_ctrl_shutdown,
    .config_defaults = &fofb_ctrl_config_defaults
};

SMIO_MOD_DECLARE(FOFB_CTRL_SDB_DEVID, FOFB_CTRL_SDB_NAME, fofb_ctrl_bootstrap_ops)
