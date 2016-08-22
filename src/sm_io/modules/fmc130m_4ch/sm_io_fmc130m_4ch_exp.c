/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "halcs_server.h"
/* Private headers */
#include "sm_io_fmc130m_4ch_codes.h"
#include "sm_io_fmc130m_4ch_defaults.h"
#include "sm_io_fmc130m_4ch_exports.h"
#include "sm_io_fmc130m_4ch_core.h"
#include "sm_io_fmc130m_4ch_exp.h"
#include "hw/wb_fmc130m_4ch_regs.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...)  \
    ASSERT_HAL_TEST(test_boolean, SM_IO, "[sm_io:fmc130m_4ch_exp]", \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_ALLOC(ptr, SM_IO, "[sm_io:fmc130m_4ch_exp]", \
            smio_err_str(SMIO_ERR_ALLOC),                   \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                            \
    CHECK_HAL_ERR(err, SM_IO, "[sm_io:fmc130m_4ch_exp]",    \
            smio_err_str (err_type))

#define FMC130M_4CH_CHECK_ACTIVE(self)                          \
    ({                                                          \
        smio_fmc130m_4ch_t *fmc130m = smio_get_handler (self);  \
        if (fmc130m == NULL) {                                  \
            DBE_DEBUG (DBG_SM_IO | DBG_LVL_ERR, "[sm_io:fmc130m_4ch_exp] "\
                "Could not get SMIO FMC130M handler\n");        \
            return -FMC130M_4CH_ERR;                            \
        }                                                       \
        if (fmc130m->type != TYPE_FMC130M_4CH_ACTIVE) {         \
            DBE_DEBUG (DBG_SM_IO | DBG_LVL_ERR, "[sm_io:fmc130m_4ch_exp] "\
                "Board is not of ACTIVE type. Unimplemented "   \
                "function for this type of FMC130M_4CH board\n"); \
            return -FMC130M_4CH_UNINPL;                         \
        }                                                       \
    })

/************************************************************/
/************ Specific FMC_130M_4CH Operations **************/
/************************************************************/

/***************************** ADC LT2208 Control ***************************/
#define HALCS_FMC130M_4CH_RAND_MIN                0 /* RAND disabled */
#define HALCS_FMC130M_4CH_RAND_MAX                1 /* RAND enabled  */

RW_PARAM_FUNC(fmc130m_4ch, adc_rand) {
    SET_GET_PARAM(fmc130m_4ch, 0x0, WB_FMC_130M_4CH_CSR,
            ADC, RAND, SINGLE_BIT_PARAM,
            HALCS_FMC130M_4CH_RAND_MIN, HALCS_FMC130M_4CH_RAND_MAX, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

#define HALCS_FMC130M_4CH_DITH_MIN                0 /* DITH disabled */
#define HALCS_FMC130M_4CH_DITH_MAX                1 /* DITH enabled  */

RW_PARAM_FUNC(fmc130m_4ch, adc_dith) {
    SET_GET_PARAM(fmc130m_4ch, 0x0, WB_FMC_130M_4CH_CSR,
            ADC, DITH, SINGLE_BIT_PARAM,
            HALCS_FMC130M_4CH_DITH_MIN, HALCS_FMC130M_4CH_DITH_MAX, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

#define HALCS_FMC130M_4CH_SHDN_MIN                0 /* SHDN disabled */
#define HALCS_FMC130M_4CH_SHDN_MAX                1 /* SHDN enabled  */

RW_PARAM_FUNC(fmc130m_4ch, adc_shdn) {
    SET_GET_PARAM(fmc130m_4ch, 0x0, WB_FMC_130M_4CH_CSR,
            ADC, SHDN, SINGLE_BIT_PARAM,
            HALCS_FMC130M_4CH_SHDN_MIN, HALCS_FMC130M_4CH_SHDN_MAX, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

#define HALCS_FMC130M_4CH_PGA_MIN                 0 /* PGA disabled */
#define HALCS_FMC130M_4CH_PGA_MAX                 1 /* PGA enabled  */

RW_PARAM_FUNC(fmc130m_4ch, adc_pga) {
    SET_GET_PARAM(fmc130m_4ch, 0x0, WB_FMC_130M_4CH_CSR,
            ADC, PGA, SINGLE_BIT_PARAM,
            HALCS_FMC130M_4CH_PGA_MIN, HALCS_FMC130M_4CH_PGA_MAX, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

/********************* ADC RAW Data registers (for simple debug) **************/

/* FIXME: Override DATA<channel_number>_VAL macros with the DATA_GLOBAL ones.
 * As the macros are defined with 32-bits and there is a shift involved, the
 * compiler complains about the size of the macro being bigger the the type
 * (uint32_t). So, we define our own macros here and don't use the generated
 * ones*/
#define WB_FMC_130M_4CH_CSR_DATA_GLOBAL_MASK     ((1ULL<<32)-1)
#define WB_FMC_130M_4CH_CSR_DATA_GLOBAL_W(val)   (val)
#define WB_FMC_130M_4CH_CSR_DATA_GLOBAL_R(val)   (val)

/* We receive data as 32-bit, but the 16 MSB are zeroed. It is conveninent
 * to sign extend the bits */
static int _rw_halcs_fmc130m_4ch_data_fmt (uint32_t *data)
{
    *data = WBGEN2_SIGN_EXTEND(*data, 15);

    return PARAM_OK;
}

rw_param_format_fp rw_halcs_fmc130m_4ch_data_fmt_fp = _rw_halcs_fmc130m_4ch_data_fmt;

#define  WB_FMC_130M_4CH_CSR_DATA0_GLOBAL_MASK      WB_FMC_130M_4CH_CSR_DATA_GLOBAL_MASK
#define  WB_FMC_130M_4CH_CSR_DATA0_GLOBAL_W(val)    WB_FMC_130M_4CH_CSR_DATA_GLOBAL_W(val)
#define  WB_FMC_130M_4CH_CSR_DATA0_GLOBAL_R(val)    WB_FMC_130M_4CH_CSR_DATA_GLOBAL_R(val)
RW_PARAM_FUNC(fmc130m_4ch, adc_data0) {
    SET_GET_PARAM(fmc130m_4ch, 0x0, WB_FMC_130M_4CH_CSR,
            DATA0, GLOBAL, MULT_BIT_PARAM,
            /* no minimum value */, /* no maximum value */, NO_CHK_FUNC,
            rw_halcs_fmc130m_4ch_data_fmt_fp, SET_FIELD);
}

#define  WB_FMC_130M_4CH_CSR_DATA1_GLOBAL_MASK      WB_FMC_130M_4CH_CSR_DATA_GLOBAL_MASK
#define  WB_FMC_130M_4CH_CSR_DATA1_GLOBAL_W(val)    WB_FMC_130M_4CH_CSR_DATA_GLOBAL_W(val)
#define  WB_FMC_130M_4CH_CSR_DATA1_GLOBAL_R(val)    WB_FMC_130M_4CH_CSR_DATA_GLOBAL_R(val)
RW_PARAM_FUNC(fmc130m_4ch, adc_data1) {
    SET_GET_PARAM(fmc130m_4ch, 0x0, WB_FMC_130M_4CH_CSR,
            DATA1, GLOBAL, MULT_BIT_PARAM,
            /* no minimum value */, /* no maximum value */, NO_CHK_FUNC,
            rw_halcs_fmc130m_4ch_data_fmt_fp, SET_FIELD);
}

#define  WB_FMC_130M_4CH_CSR_DATA2_GLOBAL_MASK       WB_FMC_130M_4CH_CSR_DATA_GLOBAL_MASK
#define  WB_FMC_130M_4CH_CSR_DATA2_GLOBAL_W(val)    WB_FMC_130M_4CH_CSR_DATA_GLOBAL_W(val)
#define  WB_FMC_130M_4CH_CSR_DATA2_GLOBAL_R(val)    WB_FMC_130M_4CH_CSR_DATA_GLOBAL_R(val)
RW_PARAM_FUNC(fmc130m_4ch, adc_data2) {
    SET_GET_PARAM(fmc130m_4ch, 0x0, WB_FMC_130M_4CH_CSR,
            DATA2, GLOBAL, MULT_BIT_PARAM,
            /* no minimum value */, /* no maximum value */, NO_CHK_FUNC,
            rw_halcs_fmc130m_4ch_data_fmt_fp, SET_FIELD);
}

#define  WB_FMC_130M_4CH_CSR_DATA3_GLOBAL_MASK      WB_FMC_130M_4CH_CSR_DATA_GLOBAL_MASK
#define  WB_FMC_130M_4CH_CSR_DATA3_GLOBAL_W(val)    WB_FMC_130M_4CH_CSR_DATA_GLOBAL_W(val)
#define  WB_FMC_130M_4CH_CSR_DATA3_GLOBAL_R(val)    WB_FMC_130M_4CH_CSR_DATA_GLOBAL_R(val)
RW_PARAM_FUNC(fmc130m_4ch, adc_data3) {
    SET_GET_PARAM(fmc130m_4ch, 0x0, WB_FMC_130M_4CH_CSR,
            DATA3, GLOBAL, MULT_BIT_PARAM,
            /* no minimum value */, /* no maximum value */, NO_CHK_FUNC,
            rw_halcs_fmc130m_4ch_data_fmt_fp, SET_FIELD);
}

/******************************** ADC Delay Values ****************************/

RW_PARAM_FUNC(fmc130m_4ch, adc_dly_val0) {
    SET_GET_PARAM(fmc130m_4ch, 0x0, WB_FMC_130M_4CH_CSR,
            IDELAY0_CAL, VAL, MULT_BIT_PARAM,
            /* no minimum value */, /* no maximum value */, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(fmc130m_4ch, adc_dly_val1) {
    SET_GET_PARAM(fmc130m_4ch, 0x0, WB_FMC_130M_4CH_CSR,
            IDELAY1_CAL, VAL, MULT_BIT_PARAM,
            /* no minimum value */, /* no maximum value */, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(fmc130m_4ch, adc_dly_val2) {
    SET_GET_PARAM(fmc130m_4ch, 0x0, WB_FMC_130M_4CH_CSR,
            IDELAY2_CAL, VAL, MULT_BIT_PARAM,
            /* no minimum value */, /* no maximum value */, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(fmc130m_4ch, adc_dly_val3) {
    SET_GET_PARAM(fmc130m_4ch, 0x0, WB_FMC_130M_4CH_CSR,
            IDELAY3_CAL, VAL, MULT_BIT_PARAM,
            /* no minimum value */, /* no maximum value */, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

/******************************** ADC Delay Lines *****************************/

RW_PARAM_FUNC(fmc130m_4ch, adc_dly_line0) {
    SET_GET_PARAM(fmc130m_4ch, 0x0, WB_FMC_130M_4CH_CSR,
            IDELAY0_CAL, LINE, MULT_BIT_PARAM,
            /* no minimum value */, /* no maximum value */, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(fmc130m_4ch, adc_dly_line1) {
    SET_GET_PARAM(fmc130m_4ch, 0x0, WB_FMC_130M_4CH_CSR,
            IDELAY1_CAL, LINE, MULT_BIT_PARAM,
            /* no minimum value */, /* no maximum value */, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(fmc130m_4ch, adc_dly_line2) {
    SET_GET_PARAM(fmc130m_4ch, 0x0, WB_FMC_130M_4CH_CSR,
            IDELAY2_CAL, LINE, MULT_BIT_PARAM,
            /* no minimum value */, /* no maximum value */, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(fmc130m_4ch, adc_dly_line3) {
    SET_GET_PARAM(fmc130m_4ch, 0x0, WB_FMC_130M_4CH_CSR,
            IDELAY3_CAL, LINE, MULT_BIT_PARAM,
            /* no minimum value */, /* no maximum value */, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

/******************************** ADC Delay Update ****************************/

RW_PARAM_FUNC(fmc130m_4ch, adc_dly_updt0) {
    SET_GET_PARAM(fmc130m_4ch, 0x0, WB_FMC_130M_4CH_CSR,
            IDELAY0_CAL, UPDATE, SINGLE_BIT_PARAM,
            /* no minimum value */, /* no maximum value */, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(fmc130m_4ch, adc_dly_updt1) {
    SET_GET_PARAM(fmc130m_4ch, 0x0, WB_FMC_130M_4CH_CSR,
            IDELAY1_CAL, UPDATE, SINGLE_BIT_PARAM,
            /* no minimum value */, /* no maximum value */, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(fmc130m_4ch, adc_dly_updt2) {
    SET_GET_PARAM(fmc130m_4ch, 0x0, WB_FMC_130M_4CH_CSR,
            IDELAY2_CAL, UPDATE, SINGLE_BIT_PARAM,
            /* no minimum value */, /* no maximum value */, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(fmc130m_4ch, adc_dly_updt3) {
    SET_GET_PARAM(fmc130m_4ch, 0x0, WB_FMC_130M_4CH_CSR,
            IDELAY3_CAL, UPDATE, SINGLE_BIT_PARAM,
            /* no minimum value */, /* no maximum value */, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

/***************************** Convenient ADC Delay ***************************/

#define FMC130M_4CH_IDELAY_CLK_LINE                 (0x01 << 16)    /* Bit 16 is clock */
#define FMC130M_4CH_IDELAY_DATA_LINES               0x00FFFF        /* Bits 0 to 15 are data */
#define FMC130M_4CH_IDELAY_ALL_LINES                (FMC130M_4CH_IDELAY_CLK_LINE | \
                                                        FMC130M_4CH_IDELAY_DATA_LINES)

#define FMC_130M_4CH_IDELAY_LINE_MASK               WB_FMC_130M_4CH_CSR_IDELAY0_CAL_LINE_MASK
#define FMC_130M_4CH_IDELAY_LINE_SHIFT              WB_FMC_130M_4CH_CSR_IDELAY0_CAL_LINE_SHIFT
#define FMC_130M_4CH_IDELAY_LINE_W(value)           WB_FMC_130M_4CH_CSR_IDELAY0_CAL_LINE_W(value)
#define FMC_130M_4CH_IDELAY_LINE_R(reg)             WB_FMC_130M_4CH_CSR_IDELAY0_CAL_LINE_R(reg)
#define FMC_130M_4CH_IDELAY_LINE_UPDT               WB_FMC_130M_4CH_CSR_IDELAY0_CAL_UPDATE

#define FMC_130M_4CH_IDELAY_CAL_VAL_W(value)        WB_FMC_130M_4CH_CSR_IDELAY0_CAL_VAL_W(value)
#define FMC_130M_4CH_IDELAY_CAL_VAL_R(reg)          WB_FMC_130M_4CH_CSR_IDELAY0_CAL_VAL_R(reg)

/* Low-level ADC delay function. Must be called with the correct arguments, so
 * only internal functions shall use this */
static int _fmc130m_4ch_set_adc_dly_ll (smio_t* owner, uint64_t addr, uint32_t dly_val,
        uint32_t dly_type)
{
    uint32_t val = 0;

    if (dly_type & DLY_TYPE_DATA) {
        val = FMC_130M_4CH_IDELAY_LINE_W(FMC130M_4CH_IDELAY_DATA_LINES);
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE,
                "[sm_io:fmc130m_4ch] Setting ADC data line delays...\n");
    }

    if (dly_type & DLY_TYPE_CLK) {
        val |= FMC_130M_4CH_IDELAY_LINE_W(FMC130M_4CH_IDELAY_CLK_LINE);
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE,
                "[sm_io:fmc130m_4ch] Setting ADC clock line delay...\n");
    }

    val |= FMC_130M_4CH_IDELAY_CAL_VAL_W(dly_val);
    smio_thsafe_client_write_32 (owner, addr, &val);
    val |= FMC_130M_4CH_IDELAY_LINE_UPDT;
    smio_thsafe_client_write_32 (owner, addr, &val);

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE,
            "[sm_io:fmc130m_4ch] ADC delay value set to %u\n", dly_val);

    /* Do a readback test to guarantee the delay is set correctly */
    val = 0;
    usleep (1000);
    smio_thsafe_client_read_32 (owner, addr, &val);

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE,
            "[sm_io:fmc130m_4ch] ADC delay read value is %u\n",
            FMC_130M_4CH_IDELAY_CAL_VAL_R(val));
    ASSERT_TEST(FMC_130M_4CH_IDELAY_CAL_VAL_R(val) == dly_val,
            "Could not set ADC delay correctly. Readback test failed", err_adc_dly);

    return -FMC130M_4CH_OK;

err_adc_dly:
    return -FMC130M_4CH_ERR;
}

/* Macros to avoid repetition of the function body */
#define FMC130M_4CH_ADC_DLY_FUNC_NAME(channel)                                  \
    _fmc130m_4ch_adc_dly ## channel

#define FMC130M_4CH_ADC_DLY_FUNC_HEADER(channel)                                \
    static int FMC130M_4CH_ADC_DLY_FUNC_NAME(channel) (void *owner, void *args, void *ret)

#define FMC130M_4CH_ADC_DLY_FUNC_BODY(owner, args, ret, channel)                \
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
        /* Delay value will be masked inside _fmc130m_4ch_set_adc_dly_ll */     \
                                                                                \
        return _fmc130m_4ch_set_adc_dly_ll (self, 0x0 |     \
                WB_FMC_130M_4CH_CSR_REG_IDELAY ## channel ## _CAL, dly_val,     \
                dly_type);                                                      \
                                                                                \
err_dly_type_range:                                                             \
        return -FMC130M_4CH_ERR;                                                \
    } while(0)

FMC130M_4CH_ADC_DLY_FUNC_HEADER(0)
{
    FMC130M_4CH_ADC_DLY_FUNC_BODY(owner, args, ret, 0);
}

FMC130M_4CH_ADC_DLY_FUNC_HEADER(1)
{
    FMC130M_4CH_ADC_DLY_FUNC_BODY(owner, args, ret, 1);
}

FMC130M_4CH_ADC_DLY_FUNC_HEADER(2)
{
    FMC130M_4CH_ADC_DLY_FUNC_BODY(owner, args, ret, 2);
}

FMC130M_4CH_ADC_DLY_FUNC_HEADER(3)
{
    FMC130M_4CH_ADC_DLY_FUNC_BODY(owner, args, ret, 3);
}

/* Exported function pointers */
const disp_table_func_fp fmc130m_4ch_exp_fp [] = {
    RW_PARAM_FUNC_NAME(fmc130m_4ch, adc_rand),
    RW_PARAM_FUNC_NAME(fmc130m_4ch, adc_dith),
    RW_PARAM_FUNC_NAME(fmc130m_4ch, adc_shdn),
    RW_PARAM_FUNC_NAME(fmc130m_4ch, adc_pga),
    RW_PARAM_FUNC_NAME(fmc130m_4ch, adc_data0),
    RW_PARAM_FUNC_NAME(fmc130m_4ch, adc_data1),
    RW_PARAM_FUNC_NAME(fmc130m_4ch, adc_data2),
    RW_PARAM_FUNC_NAME(fmc130m_4ch, adc_data3),
    RW_PARAM_FUNC_NAME(fmc130m_4ch, adc_dly_val0),
    RW_PARAM_FUNC_NAME(fmc130m_4ch, adc_dly_val1),
    RW_PARAM_FUNC_NAME(fmc130m_4ch, adc_dly_val2),
    RW_PARAM_FUNC_NAME(fmc130m_4ch, adc_dly_val3),
    RW_PARAM_FUNC_NAME(fmc130m_4ch, adc_dly_line0),
    RW_PARAM_FUNC_NAME(fmc130m_4ch, adc_dly_line1),
    RW_PARAM_FUNC_NAME(fmc130m_4ch, adc_dly_line2),
    RW_PARAM_FUNC_NAME(fmc130m_4ch, adc_dly_line3),
    RW_PARAM_FUNC_NAME(fmc130m_4ch, adc_dly_updt0),
    RW_PARAM_FUNC_NAME(fmc130m_4ch, adc_dly_updt1),
    RW_PARAM_FUNC_NAME(fmc130m_4ch, adc_dly_updt2),
    RW_PARAM_FUNC_NAME(fmc130m_4ch, adc_dly_updt3),
    FMC130M_4CH_ADC_DLY_FUNC_NAME(0),
    FMC130M_4CH_ADC_DLY_FUNC_NAME(1),
    FMC130M_4CH_ADC_DLY_FUNC_NAME(2),
    FMC130M_4CH_ADC_DLY_FUNC_NAME(3),
    NULL
};

/************************************************************/
/***************** Export methods functions *****************/
/************************************************************/

static smio_err_e _fmc130m_4ch_do_op (void *owner, void *msg);

/* Attach an instance of sm_io to dev_io function pointer */
smio_err_e fmc130m_4ch_attach (smio_t *self, devio_t *parent)
{
    (void) self;
    (void) parent;
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Deattach an instance of sm_io to dev_io function pointer */
smio_err_e fmc130m_4ch_deattach (smio_t *self)
{
    (void) self;
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Export (register) sm_io to handle operations function pointer */
smio_err_e fmc130m_4ch_export_ops (smio_t *self,
        const disp_op_t** smio_exp_ops)
{
    (void) self;
    (void) smio_exp_ops;
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Unexport (unregister) sm_io to handle operations function pointer */
smio_err_e fmc130m_4ch_unexport_ops (smio_t *self)
{
    (void) self;
    return SMIO_ERR_FUNC_NOT_IMPL;
}


/* Generic wrapper for receiving opcodes and arguments to specific funtions function pointer */
/* FIXME: Code repetition! _devio_do_smio_op () function does almost the same!!! */
smio_err_e _fmc130m_4ch_do_op (void *owner, void *msg)
{
    (void) owner;
    (void) msg;
    return SMIO_ERR_FUNC_NOT_IMPL;
}

smio_err_e fmc130m_4ch_do_op (void *self, void *msg)
{
    return _fmc130m_4ch_do_op (self, msg);
}

const smio_ops_t fmc130m_4ch_ops = {
    .attach             = fmc130m_4ch_attach,          /* Attach sm_io instance to dev_io */
    .deattach           = fmc130m_4ch_deattach,        /* Deattach sm_io instance to dev_io */
    .export_ops         = fmc130m_4ch_export_ops,      /* Export sm_io operations to dev_io */
    .unexport_ops       = fmc130m_4ch_unexport_ops,    /* Unexport sm_io operations to dev_io */
    .do_op              = fmc130m_4ch_do_op            /* Generic wrapper for handling specific operations */
};

/************************************************************/
/****************** Bootstrap Operations ********************/
/************************************************************/

smio_err_e fmc130m_4ch_init (smio_t * self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fmc130m_4ch_exp] Initializing fmc130m_4ch\n");

    smio_err_e err = SMIO_SUCCESS;

    err = smio_set_id (self, FMC130M_4CH_SDB_DEVID);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO id", err_set_id);
    err = smio_set_name (self, FMC130M_4CH_SDB_NAME);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO name", err_set_name);

    /* Set SMIO ops pointers */
    err = smio_set_ops (self, &fmc130m_4ch_ops);
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
    err = smio_init_exp_ops (self, (disp_op_t **) fmc130m_4ch_exp_ops,
            fmc130m_4ch_exp_fp);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not fill SMIO "
            "function descriptors with the callbacks", err_fill_desc);

    err = smio_set_exp_ops (self, fmc130m_4ch_exp_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO exported operations",
            err_smio_set_exp_ops);

    /* Initialize specific structure */
    smio_fmc130m_4ch_t *smio_handler = smio_fmc130m_4ch_new (self);
    ASSERT_ALLOC(smio_handler, err_smio_handler_alloc, SMIO_ERR_ALLOC);
    err = smio_set_handler (self, smio_handler);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO handler",
            err_smio_set_handler);

    return err;

err_smio_set_handler:
    smio_fmc130m_4ch_destroy (&smio_handler);
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

/* Destroy sm_io instance of fmc130m_4ch */
smio_err_e fmc130m_4ch_shutdown (smio_t *self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fmc130m_4ch_exp] Shutting down fmc130m_4ch\n");

    smio_err_e err = SMIO_SUCCESS;
    smio_fmc130m_4ch_t *fmc130m = smio_get_handler (self);
    ASSERT_TEST(fmc130m != NULL, "Could not get FMC130M handler",
            err_fmc130m_handler, SMIO_ERR_ALLOC /* FIXME: improve return code */);

    /* Destroy SMIO instance */
    smio_fmc130m_4ch_destroy (&fmc130m);
    /* Nullify operation pointers */
    smio_set_exp_ops (self, NULL);
    smio_set_thsafe_client_ops (self, NULL);
    smio_set_ops (self, NULL);

err_fmc130m_handler:
    return err;
}

const smio_bootstrap_ops_t fmc130m_4ch_bootstrap_ops = {
    .init = fmc130m_4ch_init,
    .shutdown = fmc130m_4ch_shutdown,
    .config_defaults = fmc130m_4ch_config_defaults
};

SMIO_MOD_DECLARE(FMC130M_4CH_SDB_DEVID, FMC130M_4CH_SDB_NAME, fmc130m_4ch_bootstrap_ops)
