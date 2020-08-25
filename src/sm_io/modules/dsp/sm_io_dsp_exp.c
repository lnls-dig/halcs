/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "halcs_server.h"
/* Private headers */
#include "sm_io_dsp_codes.h"
#include "sm_io_dsp_defaults.h"
#include "sm_io_dsp_exports.h"
#include "sm_io_dsp_core.h"
#include "sm_io_dsp_exp.h"
#include "hw/wb_pos_calc_regs.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, SM_IO, "[sm_io:dsp_exp]", \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_ALLOC(ptr, SM_IO, "[sm_io:dsp_exp]",         \
            smio_err_str(SMIO_ERR_ALLOC),                   \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                            \
    CHECK_HAL_ERR(err, SM_IO, "[sm_io:dsp_exp]",            \
            smio_err_str (err_type))

/************************************************************/
/*****************  Specific DSP Operations *****************/
/************************************************************/

#define KX_PARAM_MIN                        1
#define KX_PARAM_MAX                        ((1<<25)-1)
RW_PARAM_FUNC(dsp, kx) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, KX, VAL, MULT_BIT_PARAM,
            KX_PARAM_MIN, KX_PARAM_MAX, NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define KY_PARAM_MIN                        1
#define KY_PARAM_MAX                        ((1<<25)-1)
RW_PARAM_FUNC(dsp, ky) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, KY, VAL, MULT_BIT_PARAM,
            KY_PARAM_MIN, KY_PARAM_MAX, NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define KSUM_PARAM_MIN                      1
#define KSUM_PARAM_MAX                      ((1<<25)-1)
RW_PARAM_FUNC(dsp, ksum) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, KSUM, VAL, MULT_BIT_PARAM,
            KSUM_PARAM_MIN, KSUM_PARAM_MAX, NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define DS_TBT_THRES_MIN                    0
#define DS_TBT_THRES_MAX                    ((1<<26)-1)
RW_PARAM_FUNC(dsp, ds_tbt_thres) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, DS_TBT_THRES, VAL, MULT_BIT_PARAM,
            DS_TBT_THRES_MIN, DS_TBT_THRES_MAX, NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define DS_FOFB_THRES_MIN                   0
#define DS_FOFB_THRES_MAX                   ((1<<26)-1)
RW_PARAM_FUNC(dsp, ds_fofb_thres) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, DS_FOFB_THRES, VAL, MULT_BIT_PARAM,
            DS_FOFB_THRES_MIN, DS_FOFB_THRES_MAX, NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define DS_MONIT_THRES_MIN                  0
#define DS_MONIT_THRES_MAX                  ((1<<26)-1)
RW_PARAM_FUNC(dsp, ds_monit_thres) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, DS_MONIT_THRES, VAL, MULT_BIT_PARAM,
            DS_MONIT_THRES_MIN, DS_MONIT_THRES_MAX, NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define POS_CALC_DDS_CFG_TEST_DATA_MIN      0
#define POS_CALC_DDS_CFG_TEST_DATA_MAX      1
RW_PARAM_FUNC(dsp, pos_calc_cfg_test_data) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, DDS_CFG, TEST_DATA, SINGLE_BIT_PARAM,
            POS_CALC_DDS_CFG_TEST_DATA_MIN, POS_CALC_DDS_CFG_TEST_DATA_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define POS_CALC_DSP_MONIT_AMP_CH0_R(val)       (val)
#define POS_CALC_DSP_MONIT_AMP_CH0_W(val)       (val)
#define POS_CALC_DSP_MONIT_AMP_CH0_MASK         ((1ULL<<32)-1)

RW_PARAM_FUNC(dsp, monit_amp_ch0) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, DSP_MONIT_AMP_CH0, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define POS_CALC_DSP_MONIT_AMP_CH1_R(val)       (val)
#define POS_CALC_DSP_MONIT_AMP_CH1_W(val)       (val)
#define POS_CALC_DSP_MONIT_AMP_CH1_MASK         ((1ULL<<32)-1)

RW_PARAM_FUNC(dsp, monit_amp_ch1) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, DSP_MONIT_AMP_CH1, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define POS_CALC_DSP_MONIT_AMP_CH2_R(val)       (val)
#define POS_CALC_DSP_MONIT_AMP_CH2_W(val)       (val)
#define POS_CALC_DSP_MONIT_AMP_CH2_MASK         ((1ULL<<32)-1)

RW_PARAM_FUNC(dsp, monit_amp_ch2) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, DSP_MONIT_AMP_CH2, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define POS_CALC_DSP_MONIT_AMP_CH3_R(val)       (val)
#define POS_CALC_DSP_MONIT_AMP_CH3_W(val)       (val)
#define POS_CALC_DSP_MONIT_AMP_CH3_MASK         ((1ULL<<32)-1)

RW_PARAM_FUNC(dsp, monit_amp_ch3) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, DSP_MONIT_AMP_CH3, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define POS_CALC_DSP_MONIT_POS_X_R(val)       (val)
#define POS_CALC_DSP_MONIT_POS_X_W(val)       (val)
#define POS_CALC_DSP_MONIT_POS_X_MASK         ((1ULL<<32)-1)

RW_PARAM_FUNC(dsp, monit_pos_x) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, DSP_MONIT_POS_X, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define POS_CALC_DSP_MONIT_POS_Y_R(val)       (val)
#define POS_CALC_DSP_MONIT_POS_Y_W(val)       (val)
#define POS_CALC_DSP_MONIT_POS_Y_MASK         ((1ULL<<32)-1)

RW_PARAM_FUNC(dsp, monit_pos_y) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, DSP_MONIT_POS_Y, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define POS_CALC_DSP_MONIT_POS_Q_R(val)       (val)
#define POS_CALC_DSP_MONIT_POS_Q_W(val)       (val)
#define POS_CALC_DSP_MONIT_POS_Q_MASK         ((1ULL<<32)-1)

RW_PARAM_FUNC(dsp, monit_pos_q) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, DSP_MONIT_POS_Q, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define POS_CALC_DSP_MONIT_POS_SUM_R(val)       (val)
#define POS_CALC_DSP_MONIT_POS_SUM_W(val)       (val)
#define POS_CALC_DSP_MONIT_POS_SUM_MASK         ((1ULL<<32)-1)

RW_PARAM_FUNC(dsp, monit_pos_sum) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, DSP_MONIT_POS_SUM, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define POS_CALC_DSP_MONIT_UPDT_R(val)          (val)
#define POS_CALC_DSP_MONIT_UPDT_W(val)          (val)
#define POS_CALC_DSP_MONIT_UPDT_MASK            ((1ULL<<32)-1)

RW_PARAM_FUNC(dsp, monit_updt) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, DSP_MONIT_UPDT, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

/* MONIT Update time */
static int _dsp_monit_poll_time (void *owner, void *args, void *ret)
{
    assert (owner);
    assert (args);

    int err = -DSP_OK;
    SMIO_OWNER_TYPE *self = SMIO_EXP_OWNER(owner);
    smio_dsp_t *dsp = smio_get_handler (self);
    ASSERT_TEST(dsp != NULL, "Could not get SMIO DSP handler",
            err_get_dsp_handler, -DSP_ERR);

    /* Message is:
     * frame 0: operation code
     * frame 1: rw      R /W    1 = read mode, 0 = write mode
     * frame 2: value to be written (rw = 0) or dummy value (rw = 1)
     * */
     uint32_t rw = *(uint32_t *) EXP_MSG_ZMQ_FIRST_ARG(args);
     uint32_t value = *(uint32_t *) EXP_MSG_ZMQ_NEXT_ARG(args);

    if (rw) {
        smio_send_dsp_monit_mgmt_msg (smio_dsp_get_actor (dsp),
            "s", "MONIT_POLL_TIME_R");
        smio_recv_dsp_monit_mgmt_msg (smio_dsp_get_actor (dsp),
            "4", &value);
        *(uint32_t *) ret = value;
        return sizeof(value);
    }
    else {
        smio_send_dsp_monit_mgmt_msg (smio_dsp_get_actor (dsp),
            "s4", "MONIT_POLL_TIME_W", value);
        return -DSP_OK;
    }

err_get_dsp_handler:
    return err;
}

/* Macros to avoid repetition of the function body Monit */

#define DSP_MONIT_FUNC_NAME(func_name)                                          \
    _dsp_ ## func_name

#define DSP_MONIT_FUNC_NAME_HEADER(func_name)                                   \
    static int DSP_MONIT_FUNC_NAME(func_name) (void *owner, void *args, void *ret)

#define DSP_MONIT_FUNC_BODY(owner, args, ret, reg_prefix)                       \
    do {                                                                        \
        assert (owner);                                                         \
        assert (args);                                                          \
                                                                                \
        int err = -DSP_OK;                                                      \
        uint32_t ampfifo_empty;                                                 \
        uint32_t posfifo_empty;                                                 \
                                                                                \
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:dsp] "                    \
                "Calling _dsp_monit_amp_pos for "#reg_prefix"\n");              \
        SMIO_OWNER_TYPE *self = SMIO_EXP_OWNER(owner);                          \
        smio_dsp_t *dsp = smio_get_handler (self);                              \
        ASSERT_TEST(dsp != NULL, "Could not get SMIO DSP handler",              \
                err_get_dsp_handler, -DSP_ERR);                                 \
                                                                                \
        /*                          */                                          \
        /* Message is:              */                                          \
        /* frame 0: operation code  */                                          \
        /*                          */                                          \
                                                                                \
        smio_dsp_data_t *data = (smio_dsp_data_t *) ret;                        \
        data->new_amp_data = false;                                             \
        data->new_pos_data = false;                                             \
                                                                                \
        /* Check if FIFO is empty before reading from it */                     \
        GET_PARAM(self, dsp, 0x0, POS_CALC,                                     \
            AMPFIFO_## reg_prefix ##_CSR, EMPTY, SINGLE_BIT_PARAM,              \
            ampfifo_empty, NO_FMT_FUNC);                                        \
        GET_PARAM(self, dsp, 0x0, POS_CALC,                                     \
            POSFIFO_## reg_prefix ##_CSR, EMPTY, SINGLE_BIT_PARAM,              \
            posfifo_empty, NO_FMT_FUNC);                                        \
                                                                                \
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:dsp] "                    \
                "ampfifo empty for "#reg_prefix": %u\n", ampfifo_empty);        \
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:dsp] "                    \
                "posfifo empty for "#reg_prefix": %u\n", posfifo_empty);        \
                                                                                \
        if (!ampfifo_empty) {                                                   \
            GET_PARAM(self, dsp, 0x0, POS_CALC,                                 \
                AMPFIFO_## reg_prefix ##_R0, AMP_CH0, MULT_BIT_PARAM,           \
                data->amp_ch0, NO_FMT_FUNC);                                    \
            GET_PARAM(self, dsp, 0x0, POS_CALC,                                 \
                AMPFIFO_## reg_prefix ##_R1, AMP_CH1, MULT_BIT_PARAM,           \
                data->amp_ch1, NO_FMT_FUNC);                                    \
            GET_PARAM(self, dsp, 0x0, POS_CALC,                                 \
                AMPFIFO_## reg_prefix ##_R2, AMP_CH2, MULT_BIT_PARAM,           \
                data->amp_ch2, NO_FMT_FUNC);                                    \
            GET_PARAM(self, dsp, 0x0, POS_CALC,                                 \
                AMPFIFO_## reg_prefix ##_R3, AMP_CH3, MULT_BIT_PARAM,           \
                data->amp_ch3, NO_FMT_FUNC);                                    \
        }                                                                       \
                                                                                \
        if (!posfifo_empty) {                                                   \
            GET_PARAM(self, dsp, 0x0, POS_CALC,                                 \
                POSFIFO_## reg_prefix ##_R0, POS_X, MULT_BIT_PARAM,             \
                data->pos_x, NO_FMT_FUNC);                                      \
            GET_PARAM(self, dsp, 0x0, POS_CALC,                                 \
                POSFIFO_## reg_prefix ##_R1, POS_Y, MULT_BIT_PARAM,             \
                data->pos_y, NO_FMT_FUNC);                                      \
            GET_PARAM(self, dsp, 0x0, POS_CALC,                                 \
                POSFIFO_## reg_prefix ##_R2, POS_Q, MULT_BIT_PARAM,             \
                data->pos_q, NO_FMT_FUNC);                                      \
            GET_PARAM(self, dsp, 0x0, POS_CALC,                                 \
                POSFIFO_## reg_prefix ##_R3, POS_SUM, MULT_BIT_PARAM,           \
                data->pos_sum, NO_FMT_FUNC);                                    \
        }                                                                       \
                                                                                \
        if (!ampfifo_empty) {                                                   \
            DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:dsp_exp] Data AMP "   \
                "for "#reg_prefix": %u %u %u %u\n",                             \
                data->amp_ch0, data->amp_ch1, data->amp_ch2, data->amp_ch3);    \
            data->new_amp_data = true;                                          \
            err = sizeof (*data);                                               \
        }                                                                       \
                                                                                \
        if (!posfifo_empty) {                                                   \
            DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:dsp_exp] Data POS "   \
                "for "#reg_prefix": %d %d %d %d\n",                             \
                data->pos_x, data->pos_y, data->pos_q, data->pos_sum);          \
            data->new_pos_data = true;                                          \
            err = sizeof (*data);                                               \
        }                                                                       \
                                                                                \
        /* Return the whole dsp_data structure even if we don't have new data. */ \
        /* This avoids specific treatment for client handling */                \
        err = sizeof (*data);                                                   \
                                                                                \
err_get_dsp_handler:                                                            \
        return err;                                                             \
    } while(0)

/* Monit declaration */

#define POS_CALC_DSP_MONIT1_AMP_CH0_R(val)       (val)
#define POS_CALC_DSP_MONIT1_AMP_CH0_W(val)       (val)
#define POS_CALC_DSP_MONIT1_AMP_CH0_MASK         ((1ULL<<32)-1)

RW_PARAM_FUNC(dsp, monit1_amp_ch0) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, DSP_MONIT1_AMP_CH0, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define POS_CALC_DSP_MONIT1_AMP_CH1_R(val)       (val)
#define POS_CALC_DSP_MONIT1_AMP_CH1_W(val)       (val)
#define POS_CALC_DSP_MONIT1_AMP_CH1_MASK         ((1ULL<<32)-1)

RW_PARAM_FUNC(dsp, monit1_amp_ch1) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, DSP_MONIT1_AMP_CH1, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define POS_CALC_DSP_MONIT1_AMP_CH2_R(val)       (val)
#define POS_CALC_DSP_MONIT1_AMP_CH2_W(val)       (val)
#define POS_CALC_DSP_MONIT1_AMP_CH2_MASK         ((1ULL<<32)-1)

RW_PARAM_FUNC(dsp, monit1_amp_ch2) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, DSP_MONIT1_AMP_CH2, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define POS_CALC_DSP_MONIT1_AMP_CH3_R(val)       (val)
#define POS_CALC_DSP_MONIT1_AMP_CH3_W(val)       (val)
#define POS_CALC_DSP_MONIT1_AMP_CH3_MASK         ((1ULL<<32)-1)

RW_PARAM_FUNC(dsp, monit1_amp_ch3) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, DSP_MONIT1_AMP_CH3, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define POS_CALC_DSP_MONIT1_POS_X_R(val)       (val)
#define POS_CALC_DSP_MONIT1_POS_X_W(val)       (val)
#define POS_CALC_DSP_MONIT1_POS_X_MASK         ((1ULL<<32)-1)

RW_PARAM_FUNC(dsp, monit1_pos_x) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, DSP_MONIT1_POS_X, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define POS_CALC_DSP_MONIT1_POS_Y_R(val)       (val)
#define POS_CALC_DSP_MONIT1_POS_Y_W(val)       (val)
#define POS_CALC_DSP_MONIT1_POS_Y_MASK         ((1ULL<<32)-1)

RW_PARAM_FUNC(dsp, monit1_pos_y) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, DSP_MONIT1_POS_Y, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define POS_CALC_DSP_MONIT1_POS_Q_R(val)       (val)
#define POS_CALC_DSP_MONIT1_POS_Q_W(val)       (val)
#define POS_CALC_DSP_MONIT1_POS_Q_MASK         ((1ULL<<32)-1)

RW_PARAM_FUNC(dsp, monit1_pos_q) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, DSP_MONIT1_POS_Q, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define POS_CALC_DSP_MONIT1_POS_SUM_R(val)       (val)
#define POS_CALC_DSP_MONIT1_POS_SUM_W(val)       (val)
#define POS_CALC_DSP_MONIT1_POS_SUM_MASK         ((1ULL<<32)-1)

RW_PARAM_FUNC(dsp, monit1_pos_sum) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, DSP_MONIT1_POS_SUM, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define POS_CALC_DSP_MONIT1_UPDT_R(val)          (val)
#define POS_CALC_DSP_MONIT1_UPDT_W(val)          (val)
#define POS_CALC_DSP_MONIT1_UPDT_MASK            ((1ULL<<32)-1)

RW_PARAM_FUNC(dsp, monit1_updt) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, DSP_MONIT1_UPDT, /* No field */,
            MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

DSP_MONIT_FUNC_NAME_HEADER(monit1_amp_pos)
{
    DSP_MONIT_FUNC_BODY(owner, args, ret, MONIT1);
}

#define DSP_MONIT_FUNC_BODY(owner, args, ret, reg_prefix)                       \
    do {                                                                        \
        assert (owner);                                                         \
        assert (args);                                                          \
                                                                                \
        int err = -DSP_OK;                                                      \
        uint32_t ampfifo_empty;                                                 \
        uint32_t posfifo_empty;                                                 \
                                                                                \
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:dsp] "                    \
                "Calling _dsp_monit_amp_pos for "#reg_prefix"\n");              \
        SMIO_OWNER_TYPE *self = SMIO_EXP_OWNER(owner);                          \
        smio_dsp_t *dsp = smio_get_handler (self);                              \
        ASSERT_TEST(dsp != NULL, "Could not get SMIO DSP handler",              \
                err_get_dsp_handler, -DSP_ERR);                                 \
                                                                                \
        /*                          */                                          \
        /* Message is:              */                                          \
        /* frame 0: operation code  */                                          \
        /*                          */                                          \
                                                                                \
        smio_dsp_data_t *data = (smio_dsp_data_t *) ret;                        \
        data->new_amp_data = false;                                             \
        data->new_pos_data = false;                                             \
                                                                                \
        /* Check if FIFO is empty before reading from it */                     \
        GET_PARAM(self, dsp, 0x0, POS_CALC,                                     \
            AMPFIFO_## reg_prefix ##_CSR, EMPTY, SINGLE_BIT_PARAM,              \
            ampfifo_empty, NO_FMT_FUNC);                                        \
        GET_PARAM(self, dsp, 0x0, POS_CALC,                                     \
            POSFIFO_## reg_prefix ##_CSR, EMPTY, SINGLE_BIT_PARAM,              \
            posfifo_empty, NO_FMT_FUNC);                                        \
                                                                                \
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:dsp] "                    \
                "ampfifo empty for "#reg_prefix": %u\n", ampfifo_empty);        \
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:dsp] "                    \
                "posfifo empty for "#reg_prefix": %u\n", posfifo_empty);        \
                                                                                \
        if (!ampfifo_empty) {                                                   \
            GET_PARAM(self, dsp, 0x0, POS_CALC,                                 \
                AMPFIFO_## reg_prefix ##_R0, AMP_CH0, MULT_BIT_PARAM,           \
                data->amp_ch0, NO_FMT_FUNC);                                    \
            GET_PARAM(self, dsp, 0x0, POS_CALC,                                 \
                AMPFIFO_## reg_prefix ##_R1, AMP_CH1, MULT_BIT_PARAM,           \
                data->amp_ch1, NO_FMT_FUNC);                                    \
            GET_PARAM(self, dsp, 0x0, POS_CALC,                                 \
                AMPFIFO_## reg_prefix ##_R2, AMP_CH2, MULT_BIT_PARAM,           \
                data->amp_ch2, NO_FMT_FUNC);                                    \
            GET_PARAM(self, dsp, 0x0, POS_CALC,                                 \
                AMPFIFO_## reg_prefix ##_R3, AMP_CH3, MULT_BIT_PARAM,           \
                data->amp_ch3, NO_FMT_FUNC);                                    \
        }                                                                       \
                                                                                \
        if (!posfifo_empty) {                                                   \
            GET_PARAM(self, dsp, 0x0, POS_CALC,                                 \
                POSFIFO_## reg_prefix ##_R0, POS_X, MULT_BIT_PARAM,             \
                data->pos_x, NO_FMT_FUNC);                                      \
            GET_PARAM(self, dsp, 0x0, POS_CALC,                                 \
                POSFIFO_## reg_prefix ##_R1, POS_Y, MULT_BIT_PARAM,             \
                data->pos_y, NO_FMT_FUNC);                                      \
            GET_PARAM(self, dsp, 0x0, POS_CALC,                                 \
                POSFIFO_## reg_prefix ##_R2, POS_Q, MULT_BIT_PARAM,             \
                data->pos_q, NO_FMT_FUNC);                                      \
            GET_PARAM(self, dsp, 0x0, POS_CALC,                                 \
                POSFIFO_## reg_prefix ##_R3, POS_SUM, MULT_BIT_PARAM,           \
                data->pos_sum, NO_FMT_FUNC);                                    \
        }                                                                       \
                                                                                \
        if (!ampfifo_empty) {                                                   \
            DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:dsp_exp] Data AMP "   \
                "for "#reg_prefix": %u %u %u %u\n",                             \
                data->amp_ch0, data->amp_ch1, data->amp_ch2, data->amp_ch3);    \
            data->new_amp_data = true;                                          \
            err = sizeof (*data);                                               \
        }                                                                       \
                                                                                \
        if (!posfifo_empty) {                                                   \
            DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:dsp_exp] Data POS "   \
                "for "#reg_prefix": %d %d %d %d\n",                             \
                data->pos_x, data->pos_y, data->pos_q, data->pos_sum);          \
            data->new_pos_data = true;                                          \
            err = sizeof (*data);                                               \
        }                                                                       \
                                                                                \
        /* Return the whole dsp_data structure even if we don't have new data. */ \
        /* This avoids specific treatment for client handling */                \
        err = sizeof (*data);                                                   \
                                                                                \
err_get_dsp_handler:                                                            \
        return err;                                                             \
    } while(0)

#define POS_CALC_SW_TAG_EN_MIN                  0
#define POS_CALC_SW_TAG_EN_MAX                  1
RW_PARAM_FUNC(dsp, sw_tag_en) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, SW_TAG, EN,
            SINGLE_BIT_PARAM, POS_CALC_SW_TAG_EN_MIN, POS_CALC_SW_TAG_EN_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define POS_CALC_SW_DATA_MASK_EN_MIN            0
#define POS_CALC_SW_DATA_MASK_EN_MAX            1
RW_PARAM_FUNC(dsp, sw_data_mask_en) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, SW_DATA_MASK, EN,
            SINGLE_BIT_PARAM, POS_CALC_SW_DATA_MASK_EN_MIN, POS_CALC_SW_DATA_MASK_EN_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define POS_CALC_SW_DATA_MASK_SAMPLES_MIN       0
#define POS_CALC_SW_DATA_MASK_SAMPLES_MAX       ((1<<16ULL)-1)
RW_PARAM_FUNC(dsp, sw_data_mask_samples) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, SW_DATA_MASK, SAMPLES,
            MULT_BIT_PARAM, POS_CALC_SW_DATA_MASK_SAMPLES_MIN, POS_CALC_SW_DATA_MASK_SAMPLES_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define POS_CALC_TBT_TAG_EN_MIN                 0
#define POS_CALC_TBT_TAG_EN_MAX                 1
RW_PARAM_FUNC(dsp, tbt_tag_en) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, TBT_TAG, EN,
            SINGLE_BIT_PARAM, POS_CALC_TBT_TAG_EN_MIN, POS_CALC_TBT_TAG_EN_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define POS_CALC_TBT_TAG_DLY_MIN                0
#define POS_CALC_TBT_TAG_DLY_MAX                ((1<<9ULL)-1)
RW_PARAM_FUNC(dsp, tbt_tag_dly) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, TBT_TAG, DLY,
            MULT_BIT_PARAM, POS_CALC_TBT_TAG_DLY_MIN, POS_CALC_TBT_TAG_DLY_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define POS_CALC_TBT_DATA_MASK_EN_MIN           0
#define POS_CALC_TBT_DATA_MASK_EN_MAX           1
RW_PARAM_FUNC(dsp, tbt_data_mask_en) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, TBT_DATA_MASK_CTL, EN,
            SINGLE_BIT_PARAM, POS_CALC_TBT_DATA_MASK_EN_MIN, POS_CALC_TBT_DATA_MASK_EN_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define POS_CALC_TBT_DATA_MASK_SAMPLES_BEG_MIN      0
#define POS_CALC_TBT_DATA_MASK_SAMPLES_BEG_MAX      ((1<<10ULL)-1)
RW_PARAM_FUNC(dsp, tbt_data_mask_samples_beg) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, TBT_DATA_MASK_SAMPLES, BEG,
            MULT_BIT_PARAM, POS_CALC_TBT_DATA_MASK_SAMPLES_BEG_MIN,
            POS_CALC_TBT_DATA_MASK_SAMPLES_BEG_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define POS_CALC_TBT_DATA_MASK_SAMPLES_END_MIN      0
#define POS_CALC_TBT_DATA_MASK_SAMPLES_END_MAX      ((1<<10ULL)-1)
RW_PARAM_FUNC(dsp, tbt_data_mask_samples_end) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, TBT_DATA_MASK_SAMPLES, END,
            MULT_BIT_PARAM, POS_CALC_TBT_DATA_MASK_SAMPLES_END_MIN,
            POS_CALC_TBT_DATA_MASK_SAMPLES_END_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define POS_CALC_SW_TAG_DESYNC_CNT_RST_MIN                  0
#define POS_CALC_SW_TAG_DESYNC_CNT_RST_MAX                  1
RW_PARAM_FUNC(dsp, sw_tag_desync_cnt_rst) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, SW_TAG, DESYNC_CNT_RST,
            SINGLE_BIT_PARAM, POS_CALC_SW_TAG_DESYNC_CNT_RST_MIN,
            POS_CALC_SW_TAG_DESYNC_CNT_RST_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define POS_CALC_SW_TAG_DESYNC_CNT_MIN                     0
#define POS_CALC_SW_TAG_DESYNC_CNT_MAX                     ((1<<14ULL)-1)
RW_PARAM_FUNC(dsp, sw_tag_desync_cnt) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, SW_TAG, DESYNC_CNT,
            MULT_BIT_PARAM, POS_CALC_SW_TAG_DESYNC_CNT_MIN,
            POS_CALC_SW_TAG_DESYNC_CNT_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define POS_CALC_TBT_TAG_DESYNC_CNT_RST_MIN                 0
#define POS_CALC_TBT_TAG_DESYNC_CNT_RST_MAX                 1
RW_PARAM_FUNC(dsp, tbt_tag_desync_cnt_rst) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, TBT_TAG, DESYNC_CNT_RST,
            SINGLE_BIT_PARAM, POS_CALC_TBT_TAG_DESYNC_CNT_RST_MIN,
            POS_CALC_TBT_TAG_DESYNC_CNT_RST_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define POS_CALC_TBT_TAG_DESYNC_CNT_MIN                     0
#define POS_CALC_TBT_TAG_DESYNC_CNT_MAX                     ((1<<14ULL)-1)
RW_PARAM_FUNC(dsp, tbt_tag_desync_cnt) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, TBT_TAG, DESYNC_CNT,
            MULT_BIT_PARAM, POS_CALC_TBT_TAG_DESYNC_CNT_MIN,
            POS_CALC_TBT_TAG_DESYNC_CNT_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define POS_CALC_MONIT1_TAG_EN_MIN                 0
#define POS_CALC_MONIT1_TAG_EN_MAX                 1
RW_PARAM_FUNC(dsp, monit1_tag_en) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, MONIT1_TAG, EN,
            SINGLE_BIT_PARAM, POS_CALC_MONIT1_TAG_EN_MIN, POS_CALC_MONIT1_TAG_EN_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define POS_CALC_MONIT1_TAG_DLY_MIN                0
#define POS_CALC_MONIT1_TAG_DLY_MAX                ((1<<9ULL)-1)
RW_PARAM_FUNC(dsp, monit1_tag_dly) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, MONIT1_TAG, DLY,
            MULT_BIT_PARAM, POS_CALC_MONIT1_TAG_DLY_MIN, POS_CALC_MONIT1_TAG_DLY_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define POS_CALC_MONIT1_DATA_MASK_EN_MIN           0
#define POS_CALC_MONIT1_DATA_MASK_EN_MAX           1
RW_PARAM_FUNC(dsp, monit1_data_mask_en) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, MONIT1_DATA_MASK_CTL, EN,
            SINGLE_BIT_PARAM, POS_CALC_MONIT1_DATA_MASK_EN_MIN, POS_CALC_MONIT1_DATA_MASK_EN_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define POS_CALC_MONIT1_DATA_MASK_SAMPLES_BEG_MIN      0
#define POS_CALC_MONIT1_DATA_MASK_SAMPLES_BEG_MAX      ((1<<10ULL)-1)
RW_PARAM_FUNC(dsp, monit1_data_mask_samples_beg) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, MONIT1_DATA_MASK_SAMPLES, BEG,
            MULT_BIT_PARAM, POS_CALC_MONIT1_DATA_MASK_SAMPLES_BEG_MIN,
            POS_CALC_MONIT1_DATA_MASK_SAMPLES_BEG_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define POS_CALC_MONIT1_DATA_MASK_SAMPLES_END_MIN      0
#define POS_CALC_MONIT1_DATA_MASK_SAMPLES_END_MAX      ((1<<10ULL)-1)
RW_PARAM_FUNC(dsp, monit1_data_mask_samples_end) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, MONIT1_DATA_MASK_SAMPLES, END,
            MULT_BIT_PARAM, POS_CALC_MONIT1_DATA_MASK_SAMPLES_END_MIN,
            POS_CALC_MONIT1_DATA_MASK_SAMPLES_END_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define POS_CALC_MONIT1_TAG_DESYNC_CNT_RST_MIN                 0
#define POS_CALC_MONIT1_TAG_DESYNC_CNT_RST_MAX                 1
RW_PARAM_FUNC(dsp, monit1_tag_desync_cnt_rst) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, MONIT1_TAG, DESYNC_CNT_RST,
            SINGLE_BIT_PARAM, POS_CALC_MONIT1_TAG_DESYNC_CNT_RST_MIN,
            POS_CALC_MONIT1_TAG_DESYNC_CNT_RST_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define POS_CALC_MONIT1_TAG_DESYNC_CNT_MIN                     0
#define POS_CALC_MONIT1_TAG_DESYNC_CNT_MAX                     ((1<<14ULL)-1)
RW_PARAM_FUNC(dsp, monit1_tag_desync_cnt) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, MONIT1_TAG, DESYNC_CNT,
            MULT_BIT_PARAM, POS_CALC_MONIT1_TAG_DESYNC_CNT_MIN,
            POS_CALC_MONIT1_TAG_DESYNC_CNT_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define POS_CALC_MONIT_TAG_EN_MIN                 0
#define POS_CALC_MONIT_TAG_EN_MAX                 1
RW_PARAM_FUNC(dsp, monit_tag_en) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, MONIT_TAG, EN,
            SINGLE_BIT_PARAM, POS_CALC_MONIT_TAG_EN_MIN, POS_CALC_MONIT_TAG_EN_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define POS_CALC_MONIT_TAG_DLY_MIN                0
#define POS_CALC_MONIT_TAG_DLY_MAX                ((1<<9ULL)-1)
RW_PARAM_FUNC(dsp, monit_tag_dly) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, MONIT_TAG, DLY,
            MULT_BIT_PARAM, POS_CALC_MONIT_TAG_DLY_MIN, POS_CALC_MONIT_TAG_DLY_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define POS_CALC_MONIT_DATA_MASK_EN_MIN           0
#define POS_CALC_MONIT_DATA_MASK_EN_MAX           1
RW_PARAM_FUNC(dsp, monit_data_mask_en) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, MONIT_DATA_MASK_CTL, EN,
            SINGLE_BIT_PARAM, POS_CALC_MONIT_DATA_MASK_EN_MIN, POS_CALC_MONIT_DATA_MASK_EN_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define POS_CALC_MONIT_DATA_MASK_SAMPLES_BEG_MIN      0
#define POS_CALC_MONIT_DATA_MASK_SAMPLES_BEG_MAX      ((1<<10ULL)-1)
RW_PARAM_FUNC(dsp, monit_data_mask_samples_beg) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, MONIT_DATA_MASK_SAMPLES, BEG,
            MULT_BIT_PARAM, POS_CALC_MONIT_DATA_MASK_SAMPLES_BEG_MIN,
            POS_CALC_MONIT_DATA_MASK_SAMPLES_BEG_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define POS_CALC_MONIT_DATA_MASK_SAMPLES_END_MIN      0
#define POS_CALC_MONIT_DATA_MASK_SAMPLES_END_MAX      ((1<<10ULL)-1)
RW_PARAM_FUNC(dsp, monit_data_mask_samples_end) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, MONIT_DATA_MASK_SAMPLES, END,
            MULT_BIT_PARAM, POS_CALC_MONIT_DATA_MASK_SAMPLES_END_MIN,
            POS_CALC_MONIT_DATA_MASK_SAMPLES_END_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define POS_CALC_MONIT_TAG_DESYNC_CNT_RST_MIN                 0
#define POS_CALC_MONIT_TAG_DESYNC_CNT_RST_MAX                 1
RW_PARAM_FUNC(dsp, monit_tag_desync_cnt_rst) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, MONIT_TAG, DESYNC_CNT_RST,
            SINGLE_BIT_PARAM, POS_CALC_MONIT_TAG_DESYNC_CNT_RST_MIN,
            POS_CALC_MONIT_TAG_DESYNC_CNT_RST_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define POS_CALC_MONIT_TAG_DESYNC_CNT_MIN                     0
#define POS_CALC_MONIT_TAG_DESYNC_CNT_MAX                     ((1<<14ULL)-1)
RW_PARAM_FUNC(dsp, monit_tag_desync_cnt) {
    SET_GET_PARAM(dsp, 0x0, POS_CALC, MONIT_TAG, DESYNC_CNT,
            MULT_BIT_PARAM, POS_CALC_MONIT_TAG_DESYNC_CNT_MIN,
            POS_CALC_MONIT_TAG_DESYNC_CNT_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

/* Offset X */
#define POS_CALC_OFFSET_X_R(val)       (val)
#define POS_CALC_OFFSET_X_W(val)       (val)
#define POS_CALC_OFFSET_X_MASK         ((1ULL<<32)-1)
static int _dsp_offset_x (void *owner, void *args, void *ret)
{
    assert (owner);
    assert (args);

    int err = -DSP_OK;
    SMIO_OWNER_TYPE *self = SMIO_EXP_OWNER(owner);
    smio_dsp_t *dsp = smio_get_handler (self);
    ASSERT_TEST(dsp != NULL, "Could not get SMIO DSP handler",
            err_get_dsp_handler, -DSP_ERR);

    /* Message is:
     * frame 0: operation code
     * frame 1: rw      R /W    1 = read mode, 0 = write mode
     * frame 2: value to be written (rw = 0) or dummy value (rw = 1)
     * */
     uint32_t rw = *(uint32_t *) EXP_MSG_ZMQ_FIRST_ARG(args);
     int32_t value = *(int32_t *) EXP_MSG_ZMQ_NEXT_ARG(args);

    if (rw) {
        GET_PARAM(self, dsp, 0x0, POS_CALC, OFFSET_X,
            /* No field */, MULT_BIT_PARAM,
            value, NO_FMT_FUNC);
        *(int32_t *) ret = value;
        return sizeof(value);
    }
    else {
        SET_PARAM(self, dsp, 0x0, POS_CALC, OFFSET_X,
            /* No field */, MULT_BIT_PARAM,
            value, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, SET_FIELD);
        return -DSP_OK;
    }

err_get_dsp_handler:
    return err;
}

/* Offset Y */
#define POS_CALC_OFFSET_Y_R(val)       (val)
#define POS_CALC_OFFSET_Y_W(val)       (val)
#define POS_CALC_OFFSET_Y_MASK         ((1ULL<<32)-1)
static int _dsp_offset_y (void *owner, void *args, void *ret)
{
    assert (owner);
    assert (args);

    int err = -DSP_OK;
    SMIO_OWNER_TYPE *self = SMIO_EXP_OWNER(owner);
    smio_dsp_t *dsp = smio_get_handler (self);
    ASSERT_TEST(dsp != NULL, "Could not get SMIO DSP handler",
            err_get_dsp_handler, -DSP_ERR);

    /* Message is:
     * frame 0: operation code
     * frame 1: rw      R /W    1 = read mode, 0 = write mode
     * frame 2: value to be written (rw = 0) or dummy value (rw = 1)
     * */
     uint32_t rw = *(uint32_t *) EXP_MSG_ZMQ_FIRST_ARG(args);
     int32_t value = *(int32_t *) EXP_MSG_ZMQ_NEXT_ARG(args);

    if (rw) {
        GET_PARAM(self, dsp, 0x0, POS_CALC, OFFSET_Y,
            /* No field */, MULT_BIT_PARAM,
            value, NO_FMT_FUNC);
        *(int32_t *) ret = value;
        return sizeof(value);
    }
    else {
        SET_PARAM(self, dsp, 0x0, POS_CALC, OFFSET_Y,
            /* No field */, MULT_BIT_PARAM,
            value, /* No minimum check*/, /* No maximum check */,
            NO_CHK_FUNC, SET_FIELD);
        return -DSP_OK;
    }

err_get_dsp_handler:
    return err;
}

/* Exported function pointers */
const disp_table_func_fp dsp_exp_fp [] = {
    RW_PARAM_FUNC_NAME(dsp, kx),
    RW_PARAM_FUNC_NAME(dsp, ky),
    RW_PARAM_FUNC_NAME(dsp, ksum),
    RW_PARAM_FUNC_NAME(dsp, ds_tbt_thres),
    RW_PARAM_FUNC_NAME(dsp, ds_fofb_thres),
    RW_PARAM_FUNC_NAME(dsp, ds_monit_thres),
    RW_PARAM_FUNC_NAME(dsp, pos_calc_cfg_test_data),
    RW_PARAM_FUNC_NAME(dsp, monit_amp_ch0),
    RW_PARAM_FUNC_NAME(dsp, monit_amp_ch1),
    RW_PARAM_FUNC_NAME(dsp, monit_amp_ch2),
    RW_PARAM_FUNC_NAME(dsp, monit_amp_ch3),
    RW_PARAM_FUNC_NAME(dsp, monit_pos_x),
    RW_PARAM_FUNC_NAME(dsp, monit_pos_y),
    RW_PARAM_FUNC_NAME(dsp, monit_pos_q),
    RW_PARAM_FUNC_NAME(dsp, monit_pos_sum),
    RW_PARAM_FUNC_NAME(dsp, monit_updt),
    _dsp_monit_poll_time,
    RW_PARAM_FUNC_NAME(dsp, monit1_amp_ch0),
    RW_PARAM_FUNC_NAME(dsp, monit1_amp_ch1),
    RW_PARAM_FUNC_NAME(dsp, monit1_amp_ch2),
    RW_PARAM_FUNC_NAME(dsp, monit1_amp_ch3),
    RW_PARAM_FUNC_NAME(dsp, monit1_pos_x),
    RW_PARAM_FUNC_NAME(dsp, monit1_pos_y),
    RW_PARAM_FUNC_NAME(dsp, monit1_pos_q),
    RW_PARAM_FUNC_NAME(dsp, monit1_pos_sum),
    RW_PARAM_FUNC_NAME(dsp, monit1_updt),
    DSP_MONIT_FUNC_NAME(monit1_amp_pos),
    RW_PARAM_FUNC_NAME(dsp, sw_tag_en),
    RW_PARAM_FUNC_NAME(dsp, sw_data_mask_en),
    RW_PARAM_FUNC_NAME(dsp, sw_data_mask_samples),
    RW_PARAM_FUNC_NAME(dsp, tbt_tag_en),
    RW_PARAM_FUNC_NAME(dsp, tbt_tag_dly),
    RW_PARAM_FUNC_NAME(dsp, tbt_data_mask_en),
    RW_PARAM_FUNC_NAME(dsp, tbt_data_mask_samples_beg),
    RW_PARAM_FUNC_NAME(dsp, tbt_data_mask_samples_end),
    RW_PARAM_FUNC_NAME(dsp, sw_tag_desync_cnt_rst),
    RW_PARAM_FUNC_NAME(dsp, sw_tag_desync_cnt),
    RW_PARAM_FUNC_NAME(dsp, tbt_tag_desync_cnt_rst),
    RW_PARAM_FUNC_NAME(dsp, tbt_tag_desync_cnt),
    RW_PARAM_FUNC_NAME(dsp, monit1_tag_en),
    RW_PARAM_FUNC_NAME(dsp, monit1_tag_dly),
    RW_PARAM_FUNC_NAME(dsp, monit1_data_mask_en),
    RW_PARAM_FUNC_NAME(dsp, monit1_data_mask_samples_beg),
    RW_PARAM_FUNC_NAME(dsp, monit1_data_mask_samples_end),
    RW_PARAM_FUNC_NAME(dsp, monit1_tag_desync_cnt_rst),
    RW_PARAM_FUNC_NAME(dsp, monit1_tag_desync_cnt),
    RW_PARAM_FUNC_NAME(dsp, monit_tag_en),
    RW_PARAM_FUNC_NAME(dsp, monit_tag_dly),
    RW_PARAM_FUNC_NAME(dsp, monit_data_mask_en),
    RW_PARAM_FUNC_NAME(dsp, monit_data_mask_samples_beg),
    RW_PARAM_FUNC_NAME(dsp, monit_data_mask_samples_end),
    RW_PARAM_FUNC_NAME(dsp, monit_tag_desync_cnt_rst),
    RW_PARAM_FUNC_NAME(dsp, monit_tag_desync_cnt),
    _dsp_offset_x,
    _dsp_offset_y,
    NULL
};

/************************************************************/
/***************** Export methods functions *****************/
/************************************************************/

static smio_err_e _dsp_do_op (void *owner, void *msg);

/* Attach an instance of sm_io to dev_io function pointer */
smio_err_e dsp_attach (smio_t *self, void *args)
{
    UNUSED(self);
    UNUSED(args);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Deattach an instance of sm_io to dev_io function pointer */
smio_err_e dsp_deattach (smio_t *self)
{
    UNUSED(self);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Export (register) sm_io to handle operations function pointer */
smio_err_e dsp_export_ops (smio_t *self,
        const disp_op_t **smio_exp_ops)
{
    UNUSED(self);
    UNUSED(smio_exp_ops);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Unexport (unregister) sm_io to handle operations function pointer */
smio_err_e dsp_unexport_ops (smio_t *self)
{
    UNUSED(self);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Generic wrapper for receiving opcodes and arguments to specific funtions function pointer */
/* FIXME: Code repetition! _devio_do_smio_op () function does almost the same!!! */
smio_err_e _dsp_do_op (void *owner, void *msg)
{
    UNUSED(owner);
    UNUSED(msg);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

smio_err_e dsp_do_op (void *self, void *msg)
{
    return _dsp_do_op (self, msg);
}

const smio_ops_t dsp_ops = {
    .attach             = &dsp_attach,          /* Attach sm_io instance to dev_io */
    .deattach           = &dsp_deattach,        /* Deattach sm_io instance to dev_io */
    .export_ops         = &dsp_export_ops,      /* Export sm_io operations to dev_io */
    .unexport_ops       = &dsp_unexport_ops,    /* Unexport sm_io operations to dev_io */
    .do_op              = &dsp_do_op            /* Generic wrapper for handling specific operations */
};

/************************************************************/
/****************** Bootstrap Operations ********************/
/************************************************************/

smio_err_e dsp_init (smio_t * self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:dsp_exp] Initializing dsp\n");

    smio_err_e err = SMIO_SUCCESS;

    err = smio_set_id (self, DSP_SDB_DEVID);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO id", err_set_id);
    err = smio_set_name (self, DSP_SDB_NAME);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO name", err_set_name);

    /* Set SMIO ops pointers */
    err = smio_set_ops (self, &dsp_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO operations",
            err_smio_set_ops);
    err = smio_set_thsafe_client_ops (self, &smio_thsafe_client_zmq_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO thsafe operations",
            err_smio_set_thsafe_ops);

    /* disp_op_t structure is const and all of the functions performing on it
     * obviously receives a const argument, but here (and only on the SMIO
     * initialization) we need to make an exception if we want to keep the
     * functions' description and the function pointers separate */
    err = smio_init_exp_ops (self, (disp_op_t **) dsp_exp_ops, dsp_exp_fp);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not fill SMIO "
            "function descriptors with the callbacks", err_fill_desc);

    err = smio_set_exp_ops (self, dsp_exp_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO exported operations",
            err_smio_set_exp_ops);

    /* Initialize specific structure */
    smio_dsp_t *smio_handler = smio_dsp_new (self);
    ASSERT_ALLOC(smio_handler, err_smio_handler_alloc, SMIO_ERR_ALLOC);
    err = smio_set_handler (self, smio_handler);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO handler",
            err_smio_set_handler);

    return err;

err_smio_set_handler:
    smio_dsp_destroy (&smio_handler);
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

/* Destroy sm_io instance of dsp */
smio_err_e dsp_shutdown (smio_t *self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:dsp_exp] Shutting down dsp\n");

    smio_err_e err = SMIO_SUCCESS;
    smio_dsp_t *dsp = smio_get_handler (self);
    ASSERT_TEST(dsp != NULL, "Could not get DSP handler",
            err_dsp_handler, SMIO_ERR_ALLOC /* FIXME: improve return code */);

    /* Destroy SMIO instance */
    smio_dsp_destroy (&dsp);
    /* Nullify operation pointers */
    smio_set_exp_ops (self, NULL);
    smio_set_thsafe_client_ops (self, NULL);
    smio_set_ops (self, NULL);

err_dsp_handler:
    return err;
}

const smio_bootstrap_ops_t dsp_bootstrap_ops = {
    .init            = &dsp_init,
    .shutdown        = &dsp_shutdown,
    .config_defaults = &dsp_config_defaults
};

SMIO_MOD_DECLARE(DSP_SDB_DEVID, DSP_SDB_NAME, dsp_bootstrap_ops)
