/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "bpm_server.h"
/* Private headers */
#include "sm_io_fmc250m_4ch_codes.h"
#include "sm_io_fmc250m_4ch_defaults.h"
#include "sm_io_fmc250m_4ch_exports.h"
#include "sm_io_fmc250m_4ch_core.h"
#include "sm_io_fmc250m_4ch_exp.h"
#include "hw/wb_fmc250m_4ch_regs.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...)  \
    ASSERT_HAL_TEST(test_boolean, SM_IO, "[sm_io:fmc250m_4ch_exp]", \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_ALLOC(ptr, SM_IO, "[sm_io:fmc250m_4ch_exp]", \
            smio_err_str(SMIO_ERR_ALLOC),                   \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                            \
    CHECK_HAL_ERR(err, SM_IO, "[sm_io:fmc250m_4ch_exp]",    \
            smio_err_str (err_type))

#define FMC250M_4CH_CHECK_ACTIVE(self)                          \
    ({                                                          \
        smio_fmc250m_4ch_t *fmc250m = smio_get_handler (self);  \
        if (fmc250m == NULL) {                                  \
            DBE_DEBUG (DBG_SM_IO | DBG_LVL_ERR, "[sm_io:fmc250m_4ch_exp] "\
                "Could not get SMIO FMC250M handler\n");        \
            return -FMC250M_4CH_ERR;                            \
        }                                                       \
        if (fmc250m->type != TYPE_FMC250M_4CH_ACTIVE) {         \
            DBE_DEBUG (DBG_SM_IO | DBG_LVL_ERR, "[sm_io:fmc250m_4ch_exp] "\
                "Board is not of ACTIVE type. Unimplemented "   \
                "function for this type of FMC250M_4CH board\n"); \
            return -FMC250M_4CH_UNINPL;                         \
        }                                                       \
    })

static smch_err_e smch_isla216p_test_mode_compat (smch_isla216p_t *self,
        uint32_t *mode);

/************************************************************/
/************ Specific FMC_250M_4CH Operations **************/
/************************************************************/

/********************* ADC RAW Data registers (for simple debug) **************/

/* FIXME: Override DATA<channel_number>_VAL macros with the DATA_GLOBAL ones.
 * As the macros are defined with 32-bits and there is a shift involved, the
 * compiler complains about the size of the macro being bigger the the type
 * (uint32_t). So, we define our own macros here and don't use the generated
 * ones*/
#define WB_FMC_250M_4CH_CSR_DATA_GLOBAL_MASK     ((1ULL<<32)-1)
#define WB_FMC_250M_4CH_CSR_DATA_GLOBAL_W(val)   (val)
#define WB_FMC_250M_4CH_CSR_DATA_GLOBAL_R(val)   (val)

/* We receive data as 32-bit, but the 16 MSB are zeroed. It is conveninent
 * to sign extend the bits */
static int _rw_bpm_fmc250m_4ch_data_fmt (uint32_t *data)
{
    *data = WBGEN2_SIGN_EXTEND(*data, 15);

    return PARAM_OK;
}

rw_param_format_fp rw_bpm_fmc250m_4ch_data_fmt_fp = _rw_bpm_fmc250m_4ch_data_fmt;

RW_PARAM_FUNC(fmc250m_4ch, adc_data0) {
    SET_GET_PARAM(fmc250m_4ch, FMC_250M_CTRL_REGS_OFFS, WB_FMC_250M_4CH_CSR,
            CH0_STA, VAL, MULT_BIT_PARAM,
            /* no minimum value */, /* no maximum value */, NO_CHK_FUNC,
            rw_bpm_fmc250m_4ch_data_fmt_fp, SET_FIELD);
}

RW_PARAM_FUNC(fmc250m_4ch, adc_data1) {
    SET_GET_PARAM(fmc250m_4ch, FMC_250M_CTRL_REGS_OFFS, WB_FMC_250M_4CH_CSR,
            CH1_STA, VAL, MULT_BIT_PARAM,
            /* no minimum value */, /* no maximum value */, NO_CHK_FUNC,
            rw_bpm_fmc250m_4ch_data_fmt_fp, SET_FIELD);
}

RW_PARAM_FUNC(fmc250m_4ch, adc_data2) {
    SET_GET_PARAM(fmc250m_4ch, FMC_250M_CTRL_REGS_OFFS, WB_FMC_250M_4CH_CSR,
            CH2_STA, VAL, MULT_BIT_PARAM,
            /* no minimum value */, /* no maximum value */, NO_CHK_FUNC,
            rw_bpm_fmc250m_4ch_data_fmt_fp, SET_FIELD);
}

RW_PARAM_FUNC(fmc250m_4ch, adc_data3) {
    SET_GET_PARAM(fmc250m_4ch, FMC_250M_CTRL_REGS_OFFS, WB_FMC_250M_4CH_CSR,
            CH3_STA, VAL, MULT_BIT_PARAM,
            /* no minimum value */, /* no maximum value */, NO_CHK_FUNC,
            rw_bpm_fmc250m_4ch_data_fmt_fp, SET_FIELD);
}

#if 0
/******************************** ADC Delay Values ****************************/

RW_PARAM_FUNC(fmc250m_4ch, adc_dly_val0) {
    SET_GET_PARAM(fmc250m_4ch, FMC_250M_CTRL_REGS_OFFS, WB_FMC_250M_4CH_CSR,
            IDELAY0_CAL, VAL, MULT_BIT_PARAM,
            /* no minimum value */, /* no maximum value */, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(fmc250m_4ch, adc_dly_val1) {
    SET_GET_PARAM(fmc250m_4ch, FMC_250M_CTRL_REGS_OFFS, WB_FMC_250M_4CH_CSR,
            IDELAY1_CAL, VAL, MULT_BIT_PARAM,
            /* no minimum value */, /* no maximum value */, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(fmc250m_4ch, adc_dly_val2) {
    SET_GET_PARAM(fmc250m_4ch, FMC_250M_CTRL_REGS_OFFS, WB_FMC_250M_4CH_CSR,
            IDELAY2_CAL, VAL, MULT_BIT_PARAM,
            /* no minimum value */, /* no maximum value */, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(fmc250m_4ch, adc_dly_val3) {
    SET_GET_PARAM(fmc250m_4ch, FMC_250M_CTRL_REGS_OFFS, WB_FMC_250M_4CH_CSR,
            IDELAY3_CAL, VAL, MULT_BIT_PARAM,
            /* no minimum value */, /* no maximum value */, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

/******************************** ADC Delay Lines *****************************/

RW_PARAM_FUNC(fmc250m_4ch, adc_dly_line0) {
    SET_GET_PARAM(fmc250m_4ch, FMC_250M_CTRL_REGS_OFFS, WB_FMC_250M_4CH_CSR,
            IDELAY0_CAL, LINE, MULT_BIT_PARAM,
            /* no minimum value */, /* no maximum value */, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(fmc250m_4ch, adc_dly_line1) {
    SET_GET_PARAM(fmc250m_4ch, FMC_250M_CTRL_REGS_OFFS, WB_FMC_250M_4CH_CSR,
            IDELAY1_CAL, LINE, MULT_BIT_PARAM,
            /* no minimum value */, /* no maximum value */, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(fmc250m_4ch, adc_dly_line2) {
    SET_GET_PARAM(fmc250m_4ch, FMC_250M_CTRL_REGS_OFFS, WB_FMC_250M_4CH_CSR,
            IDELAY2_CAL, LINE, MULT_BIT_PARAM,
            /* no minimum value */, /* no maximum value */, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(fmc250m_4ch, adc_dly_line3) {
    SET_GET_PARAM(fmc250m_4ch, FMC_250M_CTRL_REGS_OFFS, WB_FMC_250M_4CH_CSR,
            IDELAY3_CAL, LINE, MULT_BIT_PARAM,
            /* no minimum value */, /* no maximum value */, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

/******************************** ADC Delay Update ****************************/

RW_PARAM_FUNC(fmc250m_4ch, adc_dly_updt0) {
    SET_GET_PARAM(fmc250m_4ch, FMC_250M_CTRL_REGS_OFFS, WB_FMC_250M_4CH_CSR,
            IDELAY0_CAL, UPDATE, SINGLE_BIT_PARAM,
            /* no minimum value */, /* no maximum value */, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(fmc250m_4ch, adc_dly_updt1) {
    SET_GET_PARAM(fmc250m_4ch, FMC_250M_CTRL_REGS_OFFS, WB_FMC_250M_4CH_CSR,
            IDELAY1_CAL, UPDATE, SINGLE_BIT_PARAM,
            /* no minimum value */, /* no maximum value */, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(fmc250m_4ch, adc_dly_updt2) {
    SET_GET_PARAM(fmc250m_4ch, FMC_250M_CTRL_REGS_OFFS, WB_FMC_250M_4CH_CSR,
            IDELAY2_CAL, UPDATE, SINGLE_BIT_PARAM,
            /* no minimum value */, /* no maximum value */, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(fmc250m_4ch, adc_dly_updt3) {
    SET_GET_PARAM(fmc250m_4ch, FMC_250M_CTRL_REGS_OFFS, WB_FMC_250M_4CH_CSR,
            IDELAY3_CAL, UPDATE, SINGLE_BIT_PARAM,
            /* no minimum value */, /* no maximum value */, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

/***************************** Convenient ADC Delay ***************************/

#define FMC250M_4CH_IDELAY_CLK_LINE                 (0x01 << 16)    /* Bit 16 is clock */
#define FMC250M_4CH_IDELAY_DATA_LINES               0x00FFFF        /* Bits 0 to 15 are data */
#define FMC250M_4CH_IDELAY_ALL_LINES                (FMC250M_4CH_IDELAY_CLK_LINE | \
                                                        FMC250M_4CH_IDELAY_DATA_LINES)

#define FMC_250M_4CH_IDELAY_LINE_MASK               WB_FMC_250M_4CH_CSR_IDELAY0_CAL_LINE_MASK
#define FMC_250M_4CH_IDELAY_LINE_SHIFT              WB_FMC_250M_4CH_CSR_IDELAY0_CAL_LINE_SHIFT
#define FMC_250M_4CH_IDELAY_LINE_W(value)           WB_FMC_250M_4CH_CSR_IDELAY0_CAL_LINE_W(value)
#define FMC_250M_4CH_IDELAY_LINE_R(reg)             WB_FMC_250M_4CH_CSR_IDELAY0_CAL_LINE_R(reg)
#define FMC_250M_4CH_IDELAY_LINE_UPDT               WB_FMC_250M_4CH_CSR_IDELAY0_CAL_UPDATE

#define FMC_250M_4CH_IDELAY_CAL_VAL_W(value)        WB_FMC_250M_4CH_CSR_IDELAY0_CAL_VAL_W(value)
#define FMC_250M_4CH_IDELAY_CAL_VAL_R(reg)          WB_FMC_250M_4CH_CSR_IDELAY0_CAL_VAL_R(reg)

/* Low-level ADC delay function. Must be called with the correct arguments, so
 * only internal functions shall use this */
static int _fmc250m_4ch_set_adc_dly_ll (smio_t* owner, uint64_t addr, uint32_t dly_val,
        uint32_t dly_type)
{
    uint32_t val = 0;

    if (dly_type & DLY_TYPE_DATA) {
        val = FMC_250M_4CH_IDELAY_LINE_W(FMC250M_4CH_IDELAY_DATA_LINES);
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE,
                "[sm_io:fmc250m_4ch] Setting ADC data line delays...\n");
    }

    if (dly_type & DLY_TYPE_CLK) {
        val |= FMC_250M_4CH_IDELAY_LINE_W(FMC250M_4CH_IDELAY_CLK_LINE);
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE,
                "[sm_io:fmc250m_4ch] Setting ADC clock line delay...\n");
    }

    val |= FMC_250M_4CH_IDELAY_CAL_VAL_W(dly_val);
    smio_thsafe_client_write_32 (owner, addr, &val);
    val |= FMC_250M_4CH_IDELAY_LINE_UPDT;
    smio_thsafe_client_write_32 (owner, addr, &val);

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE,
            "[sm_io:fmc250m_4ch] ADC delay value set to %u\n", dly_val);

    /* Do a readback test to guarantee the delay is set correctly */
    val = 0;
    usleep (1000);
    smio_thsafe_client_read_32 (owner, addr, &val);

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE,
            "[sm_io:fmc250m_4ch] ADC delay read value is %u\n",
            FMC_250M_4CH_IDELAY_CAL_VAL_R(val));
    ASSERT_TEST(FMC_250M_4CH_IDELAY_CAL_VAL_R(val) == dly_val,
            "Could not set ADC delay correctly. Readback test failed", err_adc_dly);

    return -FMC250M_4CH_OK;

err_adc_dly:
    return -FMC250M_4CH_ERR;
}

/* Macros to avoid repetition of the function body */
#define FMC250M_4CH_ADC_DLY_FUNC_NAME(channel)                                  \
    _fmc250m_4ch_adc_dly ## channel

#define FMC250M_4CH_ADC_DLY_FUNC_HEADER(channel)                                \
    static int FMC250M_4CH_ADC_DLY_FUNC_NAME(channel) (void *owner, void *args, void *ret)

#define FMC250M_4CH_ADC_DLY_FUNC_BODY(owner, args, ret, channel)                \
    do {                                                                        \
        (void) ret;                                                             \
        assert (owner);                                                         \
        assert (args);                                                          \
                                                                                \
        SMIO_OWNER_TYPE *self = SMIO_EXP_OWNER(owner);                          \
        uint32_t dly_type = *(uint32_t *) EXP_MSG_ZMQ_FIRST_ARG(args);          \
        uint32_t dly_val = *(uint32_t *) EXP_MSG_ZMQ_NEXT_ARG(args);            \
                                                                                \
        /* Check if dly_type is withing range */                                \
        ASSERT_TEST((dly_type & ~DLY_TYPE_ALL) == 0, "Delay type is invalid",   \
                err_dly_type_range);                                            \
                                                                                \
        /* Delay value will be masked inside _fmc250m_4ch_set_adc_dly_ll */     \
                                                                                \
        return _fmc250m_4ch_set_adc_dly_ll (self, FMC_250M_CTRL_REGS_OFFS |     \
                WB_FMC_250M_4CH_CSR_REG_IDELAY ## channel ## _CAL, dly_val,     \
                dly_type);                                                      \
                                                                                \
err_dly_type_range:                                                             \
        return -FMC250M_4CH_ERR;                                                \
    } while(0)

FMC250M_4CH_ADC_DLY_FUNC_HEADER(0)
{
    FMC250M_4CH_ADC_DLY_FUNC_BODY(owner, args, ret, 0);
}

FMC250M_4CH_ADC_DLY_FUNC_HEADER(1)
{
    FMC250M_4CH_ADC_DLY_FUNC_BODY(owner, args, ret, 1);
}

FMC250M_4CH_ADC_DLY_FUNC_HEADER(2)
{
    FMC250M_4CH_ADC_DLY_FUNC_BODY(owner, args, ret, 2);
}

FMC250M_4CH_ADC_DLY_FUNC_HEADER(3)
{
    FMC250M_4CH_ADC_DLY_FUNC_BODY(owner, args, ret, 3);
}
#endif

/***************************** ADC ISLA216P Control ***************************/

#define BPM_FMC250M_4CH_RST_ADCS_MIN            0 /* Do nothing on RST_ADCS pin */
#define BPM_FMC250M_4CH_RST_ADCS_MAX            1 /* Pulse RST_ADCS pin */

RW_PARAM_FUNC(fmc250m_4ch, rst_adcs) {
    SET_GET_PARAM(fmc250m_4ch, FMC_250M_CTRL_REGS_OFFS, WB_FMC_250M_4CH_CSR,
            ADC_CTL, RST_ADCS, SINGLE_BIT_PARAM,
            BPM_FMC250M_4CH_RST_ADCS_MIN, BPM_FMC250M_4CH_RST_ADCS_MAX, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

#define BPM_FMC250M_4CH_RST_DIV_ADCS_MIN        0  /* Do nothing on RST_DIV_ADCS pin */
#define BPM_FMC250M_4CH_RST_DIV_ADCS_MAX        1  /* Pulse RST_DIV_ADCS pin */

RW_PARAM_FUNC(fmc250m_4ch, rst_div_adcs) {
    SET_GET_PARAM(fmc250m_4ch, FMC_250M_CTRL_REGS_OFFS, WB_FMC_250M_4CH_CSR,
            ADC_CTL, RST_DIV_ADCS, SINGLE_BIT_PARAM,
            BPM_FMC250M_4CH_RST_DIV_ADCS_MIN, BPM_FMC250M_4CH_RST_DIV_ADCS_MAX, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

/* Macros to avoid repetition of the function body ISLA216P */
typedef smch_err_e (*smch_isla216p_func_fp) (smch_isla216p_t *self, uint32_t *param);

#define FMC250M_4CH_ISLA216P_FUNC_NAME(func_name)                                 \
        _fmc250m_4ch_isla216p_ ## func_name

#define FMC250M_4CH_ISLA216P_FUNC_NAME_HEADER(func_name)                          \
        static int FMC250M_4CH_ISLA216P_FUNC_NAME(func_name) (void *owner, void *args, void *ret)

#define FMC250M_4CH_ISLA216P_FUNC_BODY(owner, args, ret, inst, read_func, write_func,   \
                error_msg)                                                        \
        do {                                                                      \
            assert (owner);                                                       \
            assert (args);                                                        \
                                                                                  \
            int err = -FMC250M_4CH_OK;                                            \
            SMIO_OWNER_TYPE *self = SMIO_EXP_OWNER(owner);                        \
            smio_fmc250m_4ch_t *fmc250m = smio_get_handler (self);                \
            ASSERT_TEST(fmc250m != NULL, "Could not get SMIO FMC250M handler",    \
                    err_get_fmc250m_handler, -FMC250M_4CH_ERR);                   \
            smch_isla216p_t *smch_isla216p = SMIO_ISLA216P_HANDLER(fmc250m, inst); \
            uint32_t rw = *(uint32_t *) EXP_MSG_ZMQ_FIRST_ARG(args);              \
            (void) rw;                                                            \
            uint32_t param = *(uint32_t *) EXP_MSG_ZMQ_NEXT_ARG(args);            \
                                                                                  \
            DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fmc250m_4ch_exp] Calling " \
                    "ISLA216P function\n");                                       \
                                                                                   \
            smch_err_e serr = SMCH_SUCCESS;                                        \
            /* Call specific function */                                           \
            if (rw) {                                                              \
                WHEN(ISEMPTY(read_func))(                                          \
                    (void) ret;                                                    \
                    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fmc250m_4ch_exp] " \
                            "ISLA216P read function not implemented\n");           \
                    err = -FMC250M_4CH_UNINPL;                                     \
                    return err;                                                    \
                )                                                                  \
                WHENNOT(ISEMPTY(read_func))(                                       \
                    uint32_t value = 0;                                            \
                    serr = ((smch_isla216p_func_fp) read_func) (smch_isla216p, \
                            &value);                                               \
                    if (serr != SMCH_SUCCESS) {                                    \
                        err = -FMC250M_4CH_ERR;                                    \
                    }                                                              \
                    else {                                                         \
                        *((uint32_t *) ret) = value;                               \
                        err = sizeof (value);                                      \
                        DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fmc250m_4ch_exp] " \
                                "ISLA216P function read value = 0x%08X\n", value); \
                    }                                                              \
                )                                                                  \
            }                                                                      \
            else {                                                                 \
                WHEN(ISEMPTY(write_func))(                                         \
                    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fmc250m_4ch_exp] " \
                            "ISLA216P write function not implemented\n");          \
                    err = -FMC250M_4CH_UNINPL;                                     \
                    return err;                                                    \
                )                                                                  \
                WHENNOT(ISEMPTY(write_func))(                                      \
                    serr = ((smch_isla216p_func_fp) write_func) (smch_isla216p, \
                            &param);                                               \
                    if (serr != SMCH_SUCCESS) {                                    \
                        err = -FMC250M_4CH_ERR;                                    \
                    }                                                              \
                    else {                                                         \
                        err = -FMC250M_4CH_OK;                                     \
                    }                                                              \
                )                                                                  \
            }                                                                      \
                                                                                   \
    err_get_fmc250m_handler:                                                       \
            return err;                                                            \
        } while(0)

static smch_err_e smch_isla216p_test_mode_compat (smch_isla216p_t *self,
        uint32_t *mode)
{
    uint8_t test_mode = *(uint8_t *) mode;
    return smch_isla216p_set_test_mode (self, test_mode);
}

FMC250M_4CH_ISLA216P_FUNC_NAME_HEADER(test_mode0)
{
    FMC250M_4CH_ISLA216P_FUNC_BODY(owner, args, ret, 0, /* No read function */,
            smch_isla216p_test_mode_compat, "Could not set/get ISLA216P test mode");
}

FMC250M_4CH_ISLA216P_FUNC_NAME_HEADER(test_mode1)
{
    FMC250M_4CH_ISLA216P_FUNC_BODY(owner, args, ret, 1, /* No read function */,
            smch_isla216p_test_mode_compat, "Could not set/get ISLA216P test mode");
}

FMC250M_4CH_ISLA216P_FUNC_NAME_HEADER(test_mode2)
{
    FMC250M_4CH_ISLA216P_FUNC_BODY(owner, args, ret, 2, /* No read function */,
            smch_isla216p_test_mode_compat, "Could not set/get ISLA216P test mode");
}

FMC250M_4CH_ISLA216P_FUNC_NAME_HEADER(test_mode3)
{
    FMC250M_4CH_ISLA216P_FUNC_BODY(owner, args, ret, 3, /* No read function */,
            smch_isla216p_test_mode_compat, "Could not set/get ISLA216P test mode");
}

/* Exported function pointers */
const disp_table_func_fp fmc250m_4ch_exp_fp [] = {
#if 0
    RW_PARAM_FUNC_NAME(fmc250m_4ch, adc_rand),
    RW_PARAM_FUNC_NAME(fmc250m_4ch, adc_dith),
    RW_PARAM_FUNC_NAME(fmc250m_4ch, adc_shdn),
    RW_PARAM_FUNC_NAME(fmc250m_4ch, adc_pga),
#endif
    RW_PARAM_FUNC_NAME(fmc250m_4ch, adc_data0),
    RW_PARAM_FUNC_NAME(fmc250m_4ch, adc_data1),
    RW_PARAM_FUNC_NAME(fmc250m_4ch, adc_data2),
    RW_PARAM_FUNC_NAME(fmc250m_4ch, adc_data3),
#if 0
    RW_PARAM_FUNC_NAME(fmc250m_4ch, adc_dly_val0),
    RW_PARAM_FUNC_NAME(fmc250m_4ch, adc_dly_val1),
    RW_PARAM_FUNC_NAME(fmc250m_4ch, adc_dly_val2),
    RW_PARAM_FUNC_NAME(fmc250m_4ch, adc_dly_val3),
    RW_PARAM_FUNC_NAME(fmc250m_4ch, adc_dly_line0),
    RW_PARAM_FUNC_NAME(fmc250m_4ch, adc_dly_line1),
    RW_PARAM_FUNC_NAME(fmc250m_4ch, adc_dly_line2),
    RW_PARAM_FUNC_NAME(fmc250m_4ch, adc_dly_line3),
    RW_PARAM_FUNC_NAME(fmc250m_4ch, adc_dly_updt0),
    RW_PARAM_FUNC_NAME(fmc250m_4ch, adc_dly_updt1),
    RW_PARAM_FUNC_NAME(fmc250m_4ch, adc_dly_updt2),
    RW_PARAM_FUNC_NAME(fmc250m_4ch, adc_dly_updt3),
    FMC250M_4CH_ADC_DLY_FUNC_NAME(0),
    FMC250M_4CH_ADC_DLY_FUNC_NAME(1),
    FMC250M_4CH_ADC_DLY_FUNC_NAME(2),
    FMC250M_4CH_ADC_DLY_FUNC_NAME(3),
#endif
    RW_PARAM_FUNC_NAME(fmc250m_4ch, rst_adcs),
    RW_PARAM_FUNC_NAME(fmc250m_4ch, rst_div_adcs),
    FMC250M_4CH_ISLA216P_FUNC_NAME(test_mode0),
    FMC250M_4CH_ISLA216P_FUNC_NAME(test_mode1),
    FMC250M_4CH_ISLA216P_FUNC_NAME(test_mode2),
    FMC250M_4CH_ISLA216P_FUNC_NAME(test_mode3),
    NULL
};

/************************************************************/
/***************** Export methods functions *****************/
/************************************************************/

static smio_err_e _fmc250m_4ch_do_op (void *owner, void *msg);

/* Attach an instance of sm_io to dev_io function pointer */
smio_err_e fmc250m_4ch_attach (smio_t *self, devio_t *parent)
{
    (void) self;
    (void) parent;
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Deattach an instance of sm_io to dev_io function pointer */
smio_err_e fmc250m_4ch_deattach (smio_t *self)
{
    (void) self;
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Export (register) sm_io to handle operations function pointer */
smio_err_e fmc250m_4ch_export_ops (smio_t *self,
        const disp_op_t** smio_exp_ops)
{
    (void) self;
    (void) smio_exp_ops;
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Unexport (unregister) sm_io to handle operations function pointer */
smio_err_e fmc250m_4ch_unexport_ops (smio_t *self)
{
    (void) self;
    return SMIO_ERR_FUNC_NOT_IMPL;
}


/* Generic wrapper for receiving opcodes and arguments to specific funtions function pointer */
/* FIXME: Code repetition! _devio_do_smio_op () function does almost the same!!! */
smio_err_e _fmc250m_4ch_do_op (void *owner, void *msg)
{
    (void) owner;
    (void) msg;
    return SMIO_ERR_FUNC_NOT_IMPL;
}

smio_err_e fmc250m_4ch_do_op (void *self, void *msg)
{
    return _fmc250m_4ch_do_op (self, msg);
}

const smio_ops_t fmc250m_4ch_ops = {
    .attach             = fmc250m_4ch_attach,          /* Attach sm_io instance to dev_io */
    .deattach           = fmc250m_4ch_deattach,        /* Deattach sm_io instance to dev_io */
    .export_ops         = fmc250m_4ch_export_ops,      /* Export sm_io operations to dev_io */
    .unexport_ops       = fmc250m_4ch_unexport_ops,    /* Unexport sm_io operations to dev_io */
    .do_op              = fmc250m_4ch_do_op            /* Generic wrapper for handling specific operations */
};

/************************************************************/
/****************** Bootstrap Operations ********************/
/************************************************************/

smio_err_e fmc250m_4ch_init (smio_t * self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fmc250m_4ch_exp] Initializing fmc250m_4ch\n");

    smio_err_e err = SMIO_SUCCESS;

    err = smio_set_id (self, FMC250M_4CH_SDB_DEVID);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO id", err_set_id);
    err = smio_set_name (self, FMC250M_4CH_SDB_NAME);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO name", err_set_name);

    /* Set SMIO ops pointers */
    err = smio_set_ops (self, &fmc250m_4ch_ops);
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
    err = smio_init_exp_ops (self, (disp_op_t **) fmc250m_4ch_exp_ops,
            fmc250m_4ch_exp_fp);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not fill SMIO "
            "function descriptors with the callbacks", err_fill_desc);

    err = smio_set_exp_ops (self, fmc250m_4ch_exp_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO exported operations",
            err_smio_set_exp_ops);

    /* Initialize specific structure */
    smio_fmc250m_4ch_t *smio_handler = smio_fmc250m_4ch_new (self);
    ASSERT_ALLOC(smio_handler, err_smio_handler_alloc, SMIO_ERR_ALLOC);
    err = smio_set_handler (self, smio_handler);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO handler",
            err_smio_set_handler);

    return err;

err_smio_set_handler:
    smio_fmc250m_4ch_destroy (&smio_handler);
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

/* Destroy sm_io instance of fmc250m_4ch */
smio_err_e fmc250m_4ch_shutdown (smio_t *self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fmc250m_4ch_exp] Shutting down fmc250m_4ch\n");

    smio_err_e err = SMIO_SUCCESS;
    smio_fmc250m_4ch_t *fmc250m = smio_get_handler (self);
    ASSERT_TEST(fmc250m != NULL, "Could not get FMC250M handler",
            err_fmc250m_handler, SMIO_ERR_ALLOC /* FIXME: improve return code */);

    /* Destroy SMIO instance */
    smio_fmc250m_4ch_destroy (&fmc250m);
    /* Nullify operation pointers */
    smio_set_exp_ops (self, NULL);
    smio_set_thsafe_client_ops (self, NULL);
    smio_set_ops (self, NULL);

err_fmc250m_handler:
    return err;
}

const smio_bootstrap_ops_t fmc250m_4ch_bootstrap_ops = {
    .init = fmc250m_4ch_init,
    .shutdown = fmc250m_4ch_shutdown,
    .config_defaults = fmc250m_4ch_config_defaults
};

SMIO_MOD_DECLARE(FMC250M_4CH_SDB_DEVID, FMC250M_4CH_SDB_NAME, fmc250m_4ch_bootstrap_ops)
