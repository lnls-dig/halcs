/*
 * Copyright (C) 2020 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "halcs_server.h"
/* Private headers */
#include "sm_io_orbit_intlk_codes.h"
#include "sm_io_orbit_intlk_exports.h"
#include "sm_io_orbit_intlk_core.h"
#include "sm_io_orbit_intlk_exp.h"
#include "hw/wb_orbit_intlk_regs.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, SM_IO, "[sm_io:orbit_intlk_exp]",            \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)                  \
    ASSERT_HAL_ALLOC(ptr, SM_IO, "[sm_io:orbit_intlk_exp]",                    \
            smio_err_str(SMIO_ERR_ALLOC),                                      \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                               \
    CHECK_HAL_ERR(err, SM_IO, "[sm_io:orbit_intlk_exp]",                       \
            smio_err_str (err_type))

/************************************************************/
/*************  Specific ORBIT_INTLK Operations *************/
/************************************************************/

#define ORBIT_INTLK_CTRL_EN_MIN                            0
#define ORBIT_INTLK_CTRL_EN_MAX                            1
RW_PARAM_FUNC(orbit_intlk, en) {
    SET_GET_PARAM(orbit_intlk, 0x0, ORBIT_INTLK, CTRL, EN,
            SINLE_BIT_PARAM, ORBIT_INTLK_CTRL_EN_MIN, ORBIT_INTLK_CTRL_EN_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define ORBIT_INTLK_CTRL_CLR_MIN                          0
#define ORBIT_INTLK_CTRL_CLR_MAX                          1
RW_PARAM_FUNC(orbit_intlk, clr) {
    SET_GET_PARAM(orbit_intlk, 0x0, ORBIT_INTLK, CTRL, CLR,
            SINLE_BIT_PARAM, ORBIT_INTLK_CTRL_CLR_MIN, ORBIT_INTLK_CTRL_CLR_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define ORBIT_INTLK_CTRL_MIN_SUM_EN_MIN                   0
#define ORBIT_INTLK_CTRL_MIN_SUM_EN_MAX                   1
RW_PARAM_FUNC(orbit_intlk, min_sum_en) {
    SET_GET_PARAM(orbit_intlk, 0x0, ORBIT_INTLK, CTRL, MIN_SUM_EN,
            SINLE_BIT_PARAM, ORBIT_INTLK_CTRL_MIN_SUM_EN_MIN, ORBIT_INTLK_CTRL_MIN_SUM_EN_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define ORBIT_INTLK_CTRL_TRANS_EN_MIN                     0
#define ORBIT_INTLK_CTRL_TRANS_EN_MAX                     1
RW_PARAM_FUNC(orbit_intlk, trans_en) {
    SET_GET_PARAM(orbit_intlk, 0x0, ORBIT_INTLK, CTRL, TRANS_EN,
            SINLE_BIT_PARAM, ORBIT_INTLK_CTRL_TRANS_EN_MIN, ORBIT_INTLK_CTRL_TRANS_EN_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define ORBIT_INTLK_CTRL_TRANS_CLR_MIN                    0
#define ORBIT_INTLK_CTRL_TRANS_CLR_MAX                    1
RW_PARAM_FUNC(orbit_intlk, trans_clr) {
    SET_GET_PARAM(orbit_intlk, 0x0, ORBIT_INTLK, CTRL, TRANS_CLR,
            SINLE_BIT_PARAM, ORBIT_INTLK_CTRL_TRANS_CLR_MIN, ORBIT_INTLK_CTRL_TRANS_CLR_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define ORBIT_INTLK_CTRL_ANG_EN_MIN                       0
#define ORBIT_INTLK_CTRL_ANG_EN_MAX                       1
RW_PARAM_FUNC(orbit_intlk, ang_en) {
    SET_GET_PARAM(orbit_intlk, 0x0, ORBIT_INTLK, CTRL, ANG_EN,
            SINLE_BIT_PARAM, ORBIT_INTLK_CTRL_ANG_EN_MIN, ORBIT_INTLK_CTRL_ANG_EN_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define ORBIT_INTLK_CTRL_ANG_CLR_MIN                      0
#define ORBIT_INTLK_CTRL_ANG_CLR_MAX                      1
RW_PARAM_FUNC(orbit_intlk, ang_clr) {
    SET_GET_PARAM(orbit_intlk, 0x0, ORBIT_INTLK, CTRL, ANG_CLR,
            SINLE_BIT_PARAM, ORBIT_INTLK_CTRL_ANG_CLR_MIN, ORBIT_INTLK_CTRL_ANG_CLR_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(orbit_intlk, trans_bigger_x) {
    SET_GET_PARAM(orbit_intlk, 0x0, ORBIT_INTLK, STS, TRANS_BIGGER_X,
            SINLE_BIT_PARAM, /* No minimum value */, /* No maximum value */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}


RW_PARAM_FUNC(orbit_intlk, trans_bigger_y) {
    SET_GET_PARAM(orbit_intlk, 0x0, ORBIT_INTLK, STS, TRANS_BIGGER_Y,
            SINLE_BIT_PARAM, /* No minimum value */, /* No maximum value */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(orbit_intlk, trans_bigger_ltc_x) {
    SET_GET_PARAM(orbit_intlk, 0x0, ORBIT_INTLK, STS, TRANS_BIGGER_LTC_X,
            SINLE_BIT_PARAM, /* No minimum value */, /* No maximum value */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(orbit_intlk, trans_bigger_ltc_y) {
    SET_GET_PARAM(orbit_intlk, 0x0, ORBIT_INTLK, STS, TRANS_BIGGER_LTC_Y,
            SINLE_BIT_PARAM, /* No minimum value */, /* No maximum value */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(orbit_intlk, trans_bigger_any) {
    SET_GET_PARAM(orbit_intlk, 0x0, ORBIT_INTLK, STS, TRANS_BIGGER_ANY,
            SINLE_BIT_PARAM, /* No minimum value */, /* No maximum value */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(orbit_intlk, trans_bigger) {
    SET_GET_PARAM(orbit_intlk, 0x0, ORBIT_INTLK, STS, TRANS_BIGGER,
            SINLE_BIT_PARAM, /* No minimum value */, /* No maximum value */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(orbit_intlk, trans_bigger_ltc) {
    SET_GET_PARAM(orbit_intlk, 0x0, ORBIT_INTLK, STS, TRANS_BIGGER_LTC,
            SINLE_BIT_PARAM, /* No minimum value */, /* No maximum value */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(orbit_intlk, ang_bigger_x) {
    SET_GET_PARAM(orbit_intlk, 0x0, ORBIT_INTLK, STS, ANG_BIGGER_X,
            SINLE_BIT_PARAM, /* No minimum value */, /* No maximum value */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}


RW_PARAM_FUNC(orbit_intlk, ang_bigger_y) {
    SET_GET_PARAM(orbit_intlk, 0x0, ORBIT_INTLK, STS, ANG_BIGGER_Y,
            SINLE_BIT_PARAM, /* No minimum value */, /* No maximum value */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(orbit_intlk, ang_bigger_ltc_x) {
    SET_GET_PARAM(orbit_intlk, 0x0, ORBIT_INTLK, STS, ANG_BIGGER_LTC_X,
            SINLE_BIT_PARAM, /* No minimum value */, /* No maximum value */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(orbit_intlk, ang_bigger_ltc_y) {
    SET_GET_PARAM(orbit_intlk, 0x0, ORBIT_INTLK, STS, ANG_BIGGER_LTC_Y,
            SINLE_BIT_PARAM, /* No minimum value */, /* No maximum value */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(orbit_intlk, ang_bigger_any) {
    SET_GET_PARAM(orbit_intlk, 0x0, ORBIT_INTLK, STS, ANG_BIGGER_ANY,
            SINLE_BIT_PARAM, /* No minimum value */, /* No maximum value */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(orbit_intlk, ang_bigger) {
    SET_GET_PARAM(orbit_intlk, 0x0, ORBIT_INTLK, STS, ANG_BIGGER,
            SINLE_BIT_PARAM, /* No minimum value */, /* No maximum value */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(orbit_intlk, ang_bigger_ltc) {
    SET_GET_PARAM(orbit_intlk, 0x0, ORBIT_INTLK, STS, ANG_BIGGER_LTC,
            SINLE_BIT_PARAM, /* No minimum value */, /* No maximum value */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(orbit_intlk, intlk_bigger) {
    SET_GET_PARAM(orbit_intlk, 0x0, ORBIT_INTLK, STS, INTLK_BIGGER,
            SINLE_BIT_PARAM, /* No minimum value */, /* No maximum value */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(orbit_intlk, intlk_bigger_ltc) {
    SET_GET_PARAM(orbit_intlk, 0x0, ORBIT_INTLK, STS, INTLK_BIGGER_LTC,
            SINLE_BIT_PARAM, /* No minimum value */, /* No maximum value */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define ORBIT_INTLK_MIN_SUM_R(val)           (val)
#define ORBIT_INTLK_MIN_SUM_W(val)           (val)
#define ORBIT_INTLK_MIN_SUM_MASK             ((1ULL<<32)-1)
RW_PARAM_FUNC(orbit_intlk, intlk_min_sum) {
    SET_GET_PARAM(orbit_intlk, 0x0, ORBIT_INTLK, MIN_SUM, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define ORBIT_INTLK_TRANS_MAX_X_R(val)           (val)
#define ORBIT_INTLK_TRANS_MAX_X_W(val)           (val)
#define ORBIT_INTLK_TRANS_MAX_X_MASK             ((1ULL<<32)-1)
RW_PARAM_FUNC(orbit_intlk, intlk_trans_max_x) {
    SET_GET_PARAM(orbit_intlk, 0x0, ORBIT_INTLK, TRANS_MAX_X, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define ORBIT_INTLK_TRANS_MAX_Y_R(val)           (val)
#define ORBIT_INTLK_TRANS_MAX_Y_W(val)           (val)
#define ORBIT_INTLK_TRANS_MAX_Y_MASK             ((1ULL<<32)-1)
RW_PARAM_FUNC(orbit_intlk, intlk_trans_max_y) {
    SET_GET_PARAM(orbit_intlk, 0x0, ORBIT_INTLK, TRANS_MAX_Y, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define ORBIT_INTLK_ANG_MAX_X_R(val)           (val)
#define ORBIT_INTLK_ANG_MAX_X_W(val)           (val)
#define ORBIT_INTLK_ANG_MAX_X_MASK             ((1ULL<<32)-1)
RW_PARAM_FUNC(orbit_intlk, intlk_ang_max_x) {
    SET_GET_PARAM(orbit_intlk, 0x0, ORBIT_INTLK, ANG_MAX_X, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define ORBIT_INTLK_ANG_MAX_Y_R(val)           (val)
#define ORBIT_INTLK_ANG_MAX_Y_W(val)           (val)
#define ORBIT_INTLK_ANG_MAX_Y_MASK             ((1ULL<<32)-1)
RW_PARAM_FUNC(orbit_intlk, intlk_ang_max_y) {
    SET_GET_PARAM(orbit_intlk, 0x0, ORBIT_INTLK, ANG_MAX_Y, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(orbit_intlk, trans_smaller_x) {
    SET_GET_PARAM(orbit_intlk, 0x0, ORBIT_INTLK, STS, TRANS_SMALLER_X,
            SINLE_BIT_PARAM, /* No minimum value */, /* No maximum value */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}


RW_PARAM_FUNC(orbit_intlk, trans_smaller_y) {
    SET_GET_PARAM(orbit_intlk, 0x0, ORBIT_INTLK, STS, TRANS_SMALLER_Y,
            SINLE_BIT_PARAM, /* No minimum value */, /* No maximum value */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(orbit_intlk, trans_smaller_ltc_x) {
    SET_GET_PARAM(orbit_intlk, 0x0, ORBIT_INTLK, STS, TRANS_SMALLER_LTC_X,
            SINLE_BIT_PARAM, /* No minimum value */, /* No maximum value */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(orbit_intlk, trans_smaller_ltc_y) {
    SET_GET_PARAM(orbit_intlk, 0x0, ORBIT_INTLK, STS, TRANS_SMALLER_LTC_Y,
            SINLE_BIT_PARAM, /* No minimum value */, /* No maximum value */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(orbit_intlk, trans_smaller_any) {
    SET_GET_PARAM(orbit_intlk, 0x0, ORBIT_INTLK, STS, TRANS_SMALLER_ANY,
            SINLE_BIT_PARAM, /* No minimum value */, /* No maximum value */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(orbit_intlk, trans_smaller) {
    SET_GET_PARAM(orbit_intlk, 0x0, ORBIT_INTLK, STS, TRANS_SMALLER,
            SINLE_BIT_PARAM, /* No minimum value */, /* No maximum value */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(orbit_intlk, trans_smaller_ltc) {
    SET_GET_PARAM(orbit_intlk, 0x0, ORBIT_INTLK, STS, TRANS_SMALLER_LTC,
            SINLE_BIT_PARAM, /* No minimum value */, /* No maximum value */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(orbit_intlk, ang_smaller_x) {
    SET_GET_PARAM(orbit_intlk, 0x0, ORBIT_INTLK, STS, ANG_SMALLER_X,
            SINLE_BIT_PARAM, /* No minimum value */, /* No maximum value */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}


RW_PARAM_FUNC(orbit_intlk, ang_smaller_y) {
    SET_GET_PARAM(orbit_intlk, 0x0, ORBIT_INTLK, STS, ANG_SMALLER_Y,
            SINLE_BIT_PARAM, /* No minimum value */, /* No maximum value */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(orbit_intlk, ang_smaller_ltc_x) {
    SET_GET_PARAM(orbit_intlk, 0x0, ORBIT_INTLK, STS, ANG_SMALLER_LTC_X,
            SINLE_BIT_PARAM, /* No minimum value */, /* No maximum value */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(orbit_intlk, ang_smaller_ltc_y) {
    SET_GET_PARAM(orbit_intlk, 0x0, ORBIT_INTLK, STS, ANG_SMALLER_LTC_Y,
            SINLE_BIT_PARAM, /* No minimum value */, /* No maximum value */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(orbit_intlk, ang_smaller_any) {
    SET_GET_PARAM(orbit_intlk, 0x0, ORBIT_INTLK, STS, ANG_SMALLER_ANY,
            SINLE_BIT_PARAM, /* No minimum value */, /* No maximum value */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(orbit_intlk, ang_smaller) {
    SET_GET_PARAM(orbit_intlk, 0x0, ORBIT_INTLK, STS, ANG_SMALLER,
            SINLE_BIT_PARAM, /* No minimum value */, /* No maximum value */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(orbit_intlk, ang_smaller_ltc) {
    SET_GET_PARAM(orbit_intlk, 0x0, ORBIT_INTLK, STS, ANG_SMALLER_LTC,
            SINLE_BIT_PARAM, /* No minimum value */, /* No maximum value */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define ORBIT_INTLK_TRANS_MIN_X_R(val)           (val)
#define ORBIT_INTLK_TRANS_MIN_X_W(val)           (val)
#define ORBIT_INTLK_TRANS_MIN_X_MASK             ((1ULL<<32)-1)
RW_PARAM_FUNC(orbit_intlk, intlk_trans_min_x) {
    SET_GET_PARAM(orbit_intlk, 0x0, ORBIT_INTLK, TRANS_MIN_X, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No minimum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define ORBIT_INTLK_TRANS_MIN_Y_R(val)           (val)
#define ORBIT_INTLK_TRANS_MIN_Y_W(val)           (val)
#define ORBIT_INTLK_TRANS_MIN_Y_MASK             ((1ULL<<32)-1)
RW_PARAM_FUNC(orbit_intlk, intlk_trans_min_y) {
    SET_GET_PARAM(orbit_intlk, 0x0, ORBIT_INTLK, TRANS_MIN_Y, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No minimum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define ORBIT_INTLK_ANG_MIN_X_R(val)           (val)
#define ORBIT_INTLK_ANG_MIN_X_W(val)           (val)
#define ORBIT_INTLK_ANG_MIN_X_MASK             ((1ULL<<32)-1)
RW_PARAM_FUNC(orbit_intlk, intlk_ang_min_x) {
    SET_GET_PARAM(orbit_intlk, 0x0, ORBIT_INTLK, ANG_MIN_X, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No minimum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define ORBIT_INTLK_ANG_MIN_Y_R(val)           (val)
#define ORBIT_INTLK_ANG_MIN_Y_W(val)           (val)
#define ORBIT_INTLK_ANG_MIN_Y_MASK             ((1ULL<<32)-1)
RW_PARAM_FUNC(orbit_intlk, intlk_ang_min_y) {
    SET_GET_PARAM(orbit_intlk, 0x0, ORBIT_INTLK, ANG_MIN_Y, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No minimum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

/* Exported function pointers */
const disp_table_func_fp orbit_intlk_exp_fp [] = {
    RW_PARAM_FUNC_NAME(orbit_intlk, en),
    RW_PARAM_FUNC_NAME(orbit_intlk, clr),
    RW_PARAM_FUNC_NAME(orbit_intlk, min_sum_en),
    RW_PARAM_FUNC_NAME(orbit_intlk, trans_en),
    RW_PARAM_FUNC_NAME(orbit_intlk, trans_clr),
    RW_PARAM_FUNC_NAME(orbit_intlk, ang_en),
    RW_PARAM_FUNC_NAME(orbit_intlk, ang_clr),
    RW_PARAM_FUNC_NAME(orbit_intlk, trans_bigger_x),
    RW_PARAM_FUNC_NAME(orbit_intlk, trans_bigger_y),
    RW_PARAM_FUNC_NAME(orbit_intlk, trans_bigger_ltc_x),
    RW_PARAM_FUNC_NAME(orbit_intlk, trans_bigger_ltc_y),
    RW_PARAM_FUNC_NAME(orbit_intlk, trans_bigger_any),
    RW_PARAM_FUNC_NAME(orbit_intlk, trans_bigger),
    RW_PARAM_FUNC_NAME(orbit_intlk, trans_bigger_ltc),
    RW_PARAM_FUNC_NAME(orbit_intlk, ang_bigger_x),
    RW_PARAM_FUNC_NAME(orbit_intlk, ang_bigger_y),
    RW_PARAM_FUNC_NAME(orbit_intlk, ang_bigger_ltc_x),
    RW_PARAM_FUNC_NAME(orbit_intlk, ang_bigger_ltc_y),
    RW_PARAM_FUNC_NAME(orbit_intlk, ang_bigger_any),
    RW_PARAM_FUNC_NAME(orbit_intlk, ang_bigger),
    RW_PARAM_FUNC_NAME(orbit_intlk, ang_bigger_ltc),
    RW_PARAM_FUNC_NAME(orbit_intlk, intlk_bigger),
    RW_PARAM_FUNC_NAME(orbit_intlk, intlk_bigger_ltc),
    RW_PARAM_FUNC_NAME(orbit_intlk, intlk_min_sum),
    RW_PARAM_FUNC_NAME(orbit_intlk, intlk_trans_max_x),
    RW_PARAM_FUNC_NAME(orbit_intlk, intlk_trans_max_y),
    RW_PARAM_FUNC_NAME(orbit_intlk, intlk_ang_max_x),
    RW_PARAM_FUNC_NAME(orbit_intlk, intlk_ang_max_y),
    RW_PARAM_FUNC_NAME(orbit_intlk, trans_smaller_x),
    RW_PARAM_FUNC_NAME(orbit_intlk, trans_smaller_y),
    RW_PARAM_FUNC_NAME(orbit_intlk, trans_smaller_ltc_x),
    RW_PARAM_FUNC_NAME(orbit_intlk, trans_smaller_ltc_y),
    RW_PARAM_FUNC_NAME(orbit_intlk, trans_smaller_any),
    RW_PARAM_FUNC_NAME(orbit_intlk, trans_smaller),
    RW_PARAM_FUNC_NAME(orbit_intlk, trans_smaller_ltc),
    RW_PARAM_FUNC_NAME(orbit_intlk, ang_smaller_x),
    RW_PARAM_FUNC_NAME(orbit_intlk, ang_smaller_y),
    RW_PARAM_FUNC_NAME(orbit_intlk, ang_smaller_ltc_x),
    RW_PARAM_FUNC_NAME(orbit_intlk, ang_smaller_ltc_y),
    RW_PARAM_FUNC_NAME(orbit_intlk, ang_smaller_any),
    RW_PARAM_FUNC_NAME(orbit_intlk, ang_smaller),
    RW_PARAM_FUNC_NAME(orbit_intlk, ang_smaller_ltc),
    RW_PARAM_FUNC_NAME(orbit_intlk, intlk_trans_min_x),
    RW_PARAM_FUNC_NAME(orbit_intlk, intlk_trans_min_y),
    RW_PARAM_FUNC_NAME(orbit_intlk, intlk_ang_min_x),
    RW_PARAM_FUNC_NAME(orbit_intlk, intlk_ang_min_y),
    NULL
};

/************************************************************/
/***************** Export methods functions *****************/
/************************************************************/

static smio_err_e _orbit_intlk_do_op (void *owner, void *msg);

/* Attach an instance of sm_io to dev_io function pointer */
smio_err_e orbit_intlk_attach (smio_t *self, void *args)
{
    UNUSED(self);
    UNUSED(args);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Deattach an instance of sm_io to dev_io function pointer */
smio_err_e orbit_intlk_deattach (smio_t *self)
{
    UNUSED(self);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Export (register) sm_io to handle operations function pointer */
smio_err_e orbit_intlk_export_ops (smio_t *self,
        const disp_op_t **smio_exp_ops)
{
    UNUSED(self);
    UNUSED(smio_exp_ops);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Unexport (unregister) sm_io to handle operations function pointer */
smio_err_e orbit_intlk_unexport_ops (smio_t *self)
{
    UNUSED(self);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Generic wrapper for receiving opcodes and arguments to specific funtions function pointer */
/* FIXME: Code repetition! _devio_do_smio_op () function does almost the same!!! */
smio_err_e _orbit_intlk_do_op (void *owner, void *msg)
{
    UNUSED(owner);
    UNUSED(msg);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

smio_err_e orbit_intlk_do_op (void *self, void *msg)
{
    return _orbit_intlk_do_op (self, msg);
}

const smio_ops_t orbit_intlk_ops = {
    .attach             = &orbit_intlk_attach,          /* Attach sm_io instance to dev_io */
    .deattach           = &orbit_intlk_deattach,        /* Deattach sm_io instance to dev_io */
    .export_ops         = &orbit_intlk_export_ops,      /* Export sm_io operations to dev_io */
    .unexport_ops       = &orbit_intlk_unexport_ops,    /* Unexport sm_io operations to dev_io */
    .do_op              = &orbit_intlk_do_op            /* Generic wrapper for handling specific operations */
};

/************************************************************/
/****************** Bootstrap Operations ********************/
/************************************************************/

smio_err_e orbit_intlk_init (smio_t * self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:orbit_intlk_exp] Initializing orbit_intlk\n");

    smio_err_e err = SMIO_SUCCESS;

    err = smio_set_id (self, ORBIT_INTLK_SDB_DEVID);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO id", err_set_id);
    err = smio_set_name (self, ORBIT_INTLK_SDB_NAME);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO name", err_set_name);

    /* Set SMIO ops pointers */
    err = smio_set_ops (self, &orbit_intlk_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO operations",
            err_smio_set_ops);
    err = smio_set_thsafe_client_ops (self, &smio_thsafe_client_zmq_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO thsafe operations",
            err_smio_set_thsafe_ops);

    /* disp_op_t structure is const and all of the functions performing on it
     * obviously receives a const argument, but here (and only on the SMIO
     * initialization) we need to make an exception if we want to keep the
     * functions' description and the function pointers separate */
    err = smio_init_exp_ops (self, (disp_op_t **) orbit_intlk_exp_ops, orbit_intlk_exp_fp);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not fill SMIO "
            "function descriptors with the callbacks", err_fill_desc);

    err = smio_set_exp_ops (self, orbit_intlk_exp_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO exported operations",
            err_smio_set_exp_ops);

    /* Initialize specific structure */
    smio_orbit_intlk_t *smio_handler = smio_orbit_intlk_new (self);
    ASSERT_ALLOC(smio_handler, err_smio_handler_alloc, SMIO_ERR_ALLOC);
    err = smio_set_handler (self, smio_handler);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO handler",
            err_smio_set_handler);

    return err;

err_smio_set_handler:
    smio_orbit_intlk_destroy (&smio_handler);
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

/* Destroy sm_io instance of orbit_intlk */
smio_err_e orbit_intlk_shutdown (smio_t *self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:orbit_intlk_exp] Shutting down orbit_intlk\n");

    smio_err_e err = SMIO_SUCCESS;
    smio_orbit_intlk_t *orbit_intlk = smio_get_handler (self);
    ASSERT_TEST(orbit_intlk != NULL, "Could not get ORBIT_INTLK handler",
            err_orbit_intlk_handler, SMIO_ERR_ALLOC /* FIXME: improve return code */);

    /* Destroy SMIO instance */
    smio_orbit_intlk_destroy (&orbit_intlk);
    /* Nullify operation pointers */
    smio_set_exp_ops (self, NULL);
    smio_set_thsafe_client_ops (self, NULL);
    smio_set_ops (self, NULL);

err_orbit_intlk_handler:
    return err;
}

const smio_bootstrap_ops_t orbit_intlk_bootstrap_ops = {
    .init            = &orbit_intlk_init,
    .shutdown        = &orbit_intlk_shutdown
};

SMIO_MOD_DECLARE(ORBIT_INTLK_SDB_DEVID, ORBIT_INTLK_SDB_NAME, orbit_intlk_bootstrap_ops)
