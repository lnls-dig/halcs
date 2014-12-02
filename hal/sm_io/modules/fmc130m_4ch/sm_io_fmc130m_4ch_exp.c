/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include <stdlib.h>

#include "sm_io_fmc130m_4ch_exp.h"
#include "sm_io_fmc130m_4ch_codes.h"
#include "sm_io_fmc130m_4ch_defaults.h"
#include "sm_io.h"
#include "dev_io.h"
#include "board.h"
#include "hal_assert.h"
#include "rw_param.h"
#include "wb_fmc130m_4ch_regs.h"

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

/************************************************************/
/************ Specific FMC_130M_4CH Operations **************/
/************************************************************/
static int _fmc130m_4ch_leds (void *owner, void *args, void *ret)
{
    (void) ret;
    assert (owner);
    assert (args);

    SMIO_OWNER_TYPE *self = SMIO_EXP_OWNER(owner);
    uint32_t leds = *(uint32_t *) EXP_MSG_ZMQ_FIRST_ARG(args);

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fmc130m_4ch] Calling _fmc130m_4ch_leds\n");
    smio_thsafe_client_write_32 (self, FMC_130M_CTRL_REGS_OFFS |
            WB_FMC_130M_4CH_CSR_REG_MONITOR , &leds);
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fmc130m_4ch] Led write: 0x%08x\n",
            leds);

    return -FMC130M_4CH_OK;
}

disp_op_t fmc130m_4ch_leds_exp = {
    .name = FMC130M_4CH_NAME_LEDS,
    .opcode = FMC130M_4CH_OPCODE_LEDS,
    .func_fp = _fmc130m_4ch_leds,
    .retval = DISP_ARG_END,
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

#define BPM_FMC130M_4CH_PLL_FUNC_MIN            0 /* PLL FUNCTION pin 0 */
#define BPM_FMC130M_4CH_PLL_FUNC_MAX            1 /* PLL FUNCTION pin 1 */

RW_PARAM_FUNC(fmc130m_4ch, pll_func) {
    SET_GET_PARAM(fmc130m_4ch, FMC_130M_CTRL_REGS_OFFS, WB_FMC_130M_4CH_CSR,
            CLK_DISTRIB, PLL_FUNCTION, SINGLE_BIT_PARAM,
            BPM_FMC130M_4CH_PLL_FUNC_MIN, BPM_FMC130M_4CH_PLL_FUNC_MAX, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

disp_op_t fmc130m_4ch_pll_func_exp = {
    .name = FMC130M_4CH_NAME_PLL_FUNCTION,
    .opcode = FMC130M_4CH_OPCODE_PLL_FUNCTION,
    .func_fp = RW_PARAM_FUNC_NAME(fmc130m_4ch, pll_func),
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

static int _fmc130m_4ch_ad9510_cfg_defaults (void *owner, void *args, void *ret)
{
    (void) ret;
    assert (owner);
    assert (args);

    int err = -FMC130M_4CH_OK;
    SMIO_OWNER_TYPE *self = SMIO_EXP_OWNER(owner);
    smch_ad9510_t *smch_ad9510 = SMIO_AD9510_HANDLER(self);

    FMC130M_4CH_CHECK_ACTIVE(self);

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fmc130m_4ch] Calling "
            "_fmc130m_4ch_ad9510_cfg_defaults\n");
    smch_err_e serr = smch_ad9510_cfg_defaults (smch_ad9510);
    ASSERT_TEST(serr == SMCH_SUCCESS, "Could not config AD9510 defaults",
            err_smpr_write, -FMC130M_4CH_ERR);

err_smpr_write:
    return err;
}

disp_op_t fmc130m_4ch_ad9510_cfg_defaults_exp = {
    .name = FMC130M_4CH_NAME_AD9510_CFG_DEFAULTS,
    .opcode = FMC130M_4CH_OPCODE_AD9510_CFG_DEFAULTS,
    .func_fp = _fmc130m_4ch_ad9510_cfg_defaults,
    .retval = DISP_ARG_END,
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_END
    }
};

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
static int _rw_bpm_fmc130m_4ch_data_fmt (uint32_t *data)
{
    *data = WBGEN2_SIGN_EXTEND(*data, 15);

    return PARAM_OK;
}

rw_param_format_fp rw_bpm_fmc130m_4ch_data_fmt_fp = _rw_bpm_fmc130m_4ch_data_fmt;

#define  WB_FMC_130M_4CH_CSR_DATA0_GLOBAL_MASK      WB_FMC_130M_4CH_CSR_DATA_GLOBAL_MASK
#define  WB_FMC_130M_4CH_CSR_DATA0_GLOBAL_W(val)    WB_FMC_130M_4CH_CSR_DATA_GLOBAL_W(val)
#define  WB_FMC_130M_4CH_CSR_DATA0_GLOBAL_R(val)    WB_FMC_130M_4CH_CSR_DATA_GLOBAL_R(val)
RW_PARAM_FUNC(fmc130m_4ch, adc_data0) {
    SET_GET_PARAM(fmc130m_4ch, FMC_130M_CTRL_REGS_OFFS, WB_FMC_130M_4CH_CSR,
            DATA0, GLOBAL, MULT_BIT_PARAM,
            /* no minimum value */, /* no maximum value */, NO_CHK_FUNC,
            rw_bpm_fmc130m_4ch_data_fmt_fp, SET_FIELD);
}

disp_op_t fmc130m_4ch_adc_data0_exp = {
    .name = FMC130M_4CH_NAME_ADC_DATA0,
    .opcode = FMC130M_4CH_OPCODE_ADC_DATA0,
    .func_fp = RW_PARAM_FUNC_NAME(fmc130m_4ch, adc_data0),
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

#define  WB_FMC_130M_4CH_CSR_DATA1_GLOBAL_MASK      WB_FMC_130M_4CH_CSR_DATA_GLOBAL_MASK
#define  WB_FMC_130M_4CH_CSR_DATA1_GLOBAL_W(val)    WB_FMC_130M_4CH_CSR_DATA_GLOBAL_W(val)
#define  WB_FMC_130M_4CH_CSR_DATA1_GLOBAL_R(val)    WB_FMC_130M_4CH_CSR_DATA_GLOBAL_R(val)
RW_PARAM_FUNC(fmc130m_4ch, adc_data1) {
    SET_GET_PARAM(fmc130m_4ch, FMC_130M_CTRL_REGS_OFFS, WB_FMC_130M_4CH_CSR,
            DATA1, GLOBAL, MULT_BIT_PARAM,
            /* no minimum value */, /* no maximum value */, NO_CHK_FUNC,
            rw_bpm_fmc130m_4ch_data_fmt_fp, SET_FIELD);
}

disp_op_t fmc130m_4ch_adc_data1_exp = {
    .name = FMC130M_4CH_NAME_ADC_DATA1,
    .opcode = FMC130M_4CH_OPCODE_ADC_DATA1,
    .func_fp = RW_PARAM_FUNC_NAME(fmc130m_4ch, adc_data1),
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

#define  WB_FMC_130M_4CH_CSR_DATA2_GLOBAL_MASK       WB_FMC_130M_4CH_CSR_DATA_GLOBAL_MASK
#define  WB_FMC_130M_4CH_CSR_DATA2_GLOBAL_W(val)    WB_FMC_130M_4CH_CSR_DATA_GLOBAL_W(val)
#define  WB_FMC_130M_4CH_CSR_DATA2_GLOBAL_R(val)    WB_FMC_130M_4CH_CSR_DATA_GLOBAL_R(val)
RW_PARAM_FUNC(fmc130m_4ch, adc_data2) {
    SET_GET_PARAM(fmc130m_4ch, FMC_130M_CTRL_REGS_OFFS, WB_FMC_130M_4CH_CSR,
            DATA2, GLOBAL, MULT_BIT_PARAM,
            /* no minimum value */, /* no maximum value */, NO_CHK_FUNC,
            rw_bpm_fmc130m_4ch_data_fmt_fp, SET_FIELD);
}

disp_op_t fmc130m_4ch_adc_data2_exp = {
    .name = FMC130M_4CH_NAME_ADC_DATA2,
    .opcode = FMC130M_4CH_OPCODE_ADC_DATA2,
    .func_fp = RW_PARAM_FUNC_NAME(fmc130m_4ch, adc_data2),
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

#define  WB_FMC_130M_4CH_CSR_DATA3_GLOBAL_MASK      WB_FMC_130M_4CH_CSR_DATA_GLOBAL_MASK
#define  WB_FMC_130M_4CH_CSR_DATA3_GLOBAL_W(val)    WB_FMC_130M_4CH_CSR_DATA_GLOBAL_W(val)
#define  WB_FMC_130M_4CH_CSR_DATA3_GLOBAL_R(val)    WB_FMC_130M_4CH_CSR_DATA_GLOBAL_R(val)
RW_PARAM_FUNC(fmc130m_4ch, adc_data3) {
    SET_GET_PARAM(fmc130m_4ch, FMC_130M_CTRL_REGS_OFFS, WB_FMC_130M_4CH_CSR,
            DATA3, GLOBAL, MULT_BIT_PARAM,
            /* no minimum value */, /* no maximum value */, NO_CHK_FUNC,
            rw_bpm_fmc130m_4ch_data_fmt_fp, SET_FIELD);
}

disp_op_t fmc130m_4ch_adc_data3_exp = {
    .name = FMC130M_4CH_NAME_ADC_DATA3,
    .opcode = FMC130M_4CH_OPCODE_ADC_DATA3,
    .func_fp = RW_PARAM_FUNC_NAME(fmc130m_4ch, adc_data3),
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

/******************************** ADC Delay Values ****************************/

RW_PARAM_FUNC(fmc130m_4ch, adc_dly_val0) {
    SET_GET_PARAM(fmc130m_4ch, FMC_130M_CTRL_REGS_OFFS, WB_FMC_130M_4CH_CSR,
            IDELAY0_CAL, VAL, MULT_BIT_PARAM,
            /* no minimum value */, /* no maximum value */, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

disp_op_t fmc130m_4ch_adc_dly_val0_exp = {
    .name = FMC130M_4CH_NAME_ADC_DLY_VAL0,
    .opcode = FMC130M_4CH_OPCODE_ADC_DLY_VAL0,
    .func_fp = RW_PARAM_FUNC_NAME(fmc130m_4ch, adc_dly_val0),
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

RW_PARAM_FUNC(fmc130m_4ch, adc_dly_val1) {
    SET_GET_PARAM(fmc130m_4ch, FMC_130M_CTRL_REGS_OFFS, WB_FMC_130M_4CH_CSR,
            IDELAY1_CAL, VAL, MULT_BIT_PARAM,
            /* no minimum value */, /* no maximum value */, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

disp_op_t fmc130m_4ch_adc_dly_val1_exp = {
    .name = FMC130M_4CH_NAME_ADC_DLY_VAL1,
    .opcode = FMC130M_4CH_OPCODE_ADC_DLY_VAL1,
    .func_fp = RW_PARAM_FUNC_NAME(fmc130m_4ch, adc_dly_val1),
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

RW_PARAM_FUNC(fmc130m_4ch, adc_dly_val2) {
    SET_GET_PARAM(fmc130m_4ch, FMC_130M_CTRL_REGS_OFFS, WB_FMC_130M_4CH_CSR,
            IDELAY2_CAL, VAL, MULT_BIT_PARAM,
            /* no minimum value */, /* no maximum value */, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

disp_op_t fmc130m_4ch_adc_dly_val2_exp = {
    .name = FMC130M_4CH_NAME_ADC_DLY_VAL2,
    .opcode = FMC130M_4CH_OPCODE_ADC_DLY_VAL2,
    .func_fp = RW_PARAM_FUNC_NAME(fmc130m_4ch, adc_dly_val2),
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

RW_PARAM_FUNC(fmc130m_4ch, adc_dly_val3) {
    SET_GET_PARAM(fmc130m_4ch, FMC_130M_CTRL_REGS_OFFS, WB_FMC_130M_4CH_CSR,
            IDELAY3_CAL, VAL, MULT_BIT_PARAM,
            /* no minimum value */, /* no maximum value */, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

disp_op_t fmc130m_4ch_adc_dly_val3_exp = {
    .name = FMC130M_4CH_NAME_ADC_DLY_VAL3,
    .opcode = FMC130M_4CH_OPCODE_ADC_DLY_VAL3,
    .func_fp = RW_PARAM_FUNC_NAME(fmc130m_4ch, adc_dly_val3),
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

/******************************** ADC Delay Lines *****************************/

RW_PARAM_FUNC(fmc130m_4ch, adc_dly_line0) {
    SET_GET_PARAM(fmc130m_4ch, FMC_130M_CTRL_REGS_OFFS, WB_FMC_130M_4CH_CSR,
            IDELAY0_CAL, LINE, MULT_BIT_PARAM,
            /* no minimum value */, /* no maximum value */, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

disp_op_t fmc130m_4ch_adc_dly_line0_exp = {
    .name = FMC130M_4CH_NAME_ADC_DLY_LINE0,
    .opcode = FMC130M_4CH_OPCODE_ADC_DLY_LINE0,
    .func_fp = RW_PARAM_FUNC_NAME(fmc130m_4ch, adc_dly_line0),
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

RW_PARAM_FUNC(fmc130m_4ch, adc_dly_line1) {
    SET_GET_PARAM(fmc130m_4ch, FMC_130M_CTRL_REGS_OFFS, WB_FMC_130M_4CH_CSR,
            IDELAY1_CAL, LINE, MULT_BIT_PARAM,
            /* no minimum value */, /* no maximum value */, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

disp_op_t fmc130m_4ch_adc_dly_line1_exp = {
    .name = FMC130M_4CH_NAME_ADC_DLY_LINE1,
    .opcode = FMC130M_4CH_OPCODE_ADC_DLY_LINE1,
    .func_fp = RW_PARAM_FUNC_NAME(fmc130m_4ch, adc_dly_line1),
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

RW_PARAM_FUNC(fmc130m_4ch, adc_dly_line2) {
    SET_GET_PARAM(fmc130m_4ch, FMC_130M_CTRL_REGS_OFFS, WB_FMC_130M_4CH_CSR,
            IDELAY2_CAL, LINE, MULT_BIT_PARAM,
            /* no minimum value */, /* no maximum value */, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

disp_op_t fmc130m_4ch_adc_dly_line2_exp = {
    .name = FMC130M_4CH_NAME_ADC_DLY_LINE2,
    .opcode = FMC130M_4CH_OPCODE_ADC_DLY_LINE2,
    .func_fp = RW_PARAM_FUNC_NAME(fmc130m_4ch, adc_dly_line2),
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

RW_PARAM_FUNC(fmc130m_4ch, adc_dly_line3) {
    SET_GET_PARAM(fmc130m_4ch, FMC_130M_CTRL_REGS_OFFS, WB_FMC_130M_4CH_CSR,
            IDELAY3_CAL, LINE, MULT_BIT_PARAM,
            /* no minimum value */, /* no maximum value */, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

disp_op_t fmc130m_4ch_adc_dly_line3_exp = {
    .name = FMC130M_4CH_NAME_ADC_DLY_LINE3,
    .opcode = FMC130M_4CH_OPCODE_ADC_DLY_LINE3,
    .func_fp = RW_PARAM_FUNC_NAME(fmc130m_4ch, adc_dly_line3),
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

/******************************** ADC Delay Update ****************************/

RW_PARAM_FUNC(fmc130m_4ch, adc_dly_updt0) {
    SET_GET_PARAM(fmc130m_4ch, FMC_130M_CTRL_REGS_OFFS, WB_FMC_130M_4CH_CSR,
            IDELAY0_CAL, UPDATE, SINGLE_BIT_PARAM,
            /* no minimum value */, /* no maximum value */, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

disp_op_t fmc130m_4ch_adc_dly_updt0_exp = {
    .name = FMC130M_4CH_NAME_ADC_DLY_UPDT0,
    .opcode = FMC130M_4CH_OPCODE_ADC_DLY_UPDT0,
    .func_fp = RW_PARAM_FUNC_NAME(fmc130m_4ch, adc_dly_updt0),
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

RW_PARAM_FUNC(fmc130m_4ch, adc_dly_updt1) {
    SET_GET_PARAM(fmc130m_4ch, FMC_130M_CTRL_REGS_OFFS, WB_FMC_130M_4CH_CSR,
            IDELAY1_CAL, UPDATE, SINGLE_BIT_PARAM,
            /* no minimum value */, /* no maximum value */, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

disp_op_t fmc130m_4ch_adc_dly_updt1_exp = {
    .name = FMC130M_4CH_NAME_ADC_DLY_UPDT1,
    .opcode = FMC130M_4CH_OPCODE_ADC_DLY_UPDT1,
    .func_fp = RW_PARAM_FUNC_NAME(fmc130m_4ch, adc_dly_updt1),
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

RW_PARAM_FUNC(fmc130m_4ch, adc_dly_updt2) {
    SET_GET_PARAM(fmc130m_4ch, FMC_130M_CTRL_REGS_OFFS, WB_FMC_130M_4CH_CSR,
            IDELAY2_CAL, UPDATE, SINGLE_BIT_PARAM,
            /* no minimum value */, /* no maximum value */, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

disp_op_t fmc130m_4ch_adc_dly_updt2_exp = {
    .name = FMC130M_4CH_NAME_ADC_DLY_UPDT2,
    .opcode = FMC130M_4CH_OPCODE_ADC_DLY_UPDT2,
    .func_fp = RW_PARAM_FUNC_NAME(fmc130m_4ch, adc_dly_updt2),
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

RW_PARAM_FUNC(fmc130m_4ch, adc_dly_updt3) {
    SET_GET_PARAM(fmc130m_4ch, FMC_130M_CTRL_REGS_OFFS, WB_FMC_130M_4CH_CSR,
            IDELAY3_CAL, UPDATE, SINGLE_BIT_PARAM,
            /* no minimum value */, /* no maximum value */, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

disp_op_t fmc130m_4ch_adc_dly_updt3_exp = {
    .name = FMC130M_4CH_NAME_ADC_DLY_UPDT3,
    .opcode = FMC130M_4CH_OPCODE_ADC_DLY_UPDT3,
    .func_fp = RW_PARAM_FUNC_NAME(fmc130m_4ch, adc_dly_updt3),
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

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
static int _fmc130m_4ch_set_adc_dly_ll (smio_t* owner, uint32_t addr, uint32_t dly_val,
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
        return _fmc130m_4ch_set_adc_dly_ll (self, FMC_130M_CTRL_REGS_OFFS |     \
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

disp_op_t fmc130m_4ch_adc_dly0_exp = {
    .name = FMC130M_4CH_NAME_ADC_DLY0,
    .opcode = FMC130M_4CH_OPCODE_ADC_DLY0,
    .func_fp = FMC130M_4CH_ADC_DLY_FUNC_NAME(0),
    .retval = DISP_ARG_END,
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

FMC130M_4CH_ADC_DLY_FUNC_HEADER(1)
{
    FMC130M_4CH_ADC_DLY_FUNC_BODY(owner, args, ret, 1);
}

disp_op_t fmc130m_4ch_adc_dly1_exp = {
    .name = FMC130M_4CH_NAME_ADC_DLY1,
    .opcode = FMC130M_4CH_OPCODE_ADC_DLY1,
    .func_fp = FMC130M_4CH_ADC_DLY_FUNC_NAME(1),
    .retval = DISP_ARG_END,
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

FMC130M_4CH_ADC_DLY_FUNC_HEADER(2)
{
    FMC130M_4CH_ADC_DLY_FUNC_BODY(owner, args, ret, 2);
}

disp_op_t fmc130m_4ch_adc_dly2_exp = {
    .name = FMC130M_4CH_NAME_ADC_DLY2,
    .opcode = FMC130M_4CH_OPCODE_ADC_DLY2,
    .func_fp = FMC130M_4CH_ADC_DLY_FUNC_NAME(2),
    .retval = DISP_ARG_END,
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

FMC130M_4CH_ADC_DLY_FUNC_HEADER(3)
{
    FMC130M_4CH_ADC_DLY_FUNC_BODY(owner, args, ret, 3);
}

disp_op_t fmc130m_4ch_adc_dly3_exp = {
    .name = FMC130M_4CH_NAME_ADC_DLY3,
    .opcode = FMC130M_4CH_OPCODE_ADC_DLY3,
    .func_fp = FMC130M_4CH_ADC_DLY_FUNC_NAME(3),
    .retval = DISP_ARG_END,
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

/***************************** FPGA Control ***************************/

#define BPM_FMC130M_4CH_TEST_DATA_EN_MIN            0 /* TEST data enable off */
#define BPM_FMC130M_4CH_TEST_DATA_EN_MAX            1 /* TEST data enable on */

RW_PARAM_FUNC(fmc130m_4ch, test_data_en) {
    SET_GET_PARAM(fmc130m_4ch, FMC_130M_CTRL_REGS_OFFS, WB_FMC_130M_4CH_CSR,
            FPGA_CTRL, TEST_DATA_EN, SINGLE_BIT_PARAM,
            BPM_FMC130M_4CH_TEST_DATA_EN_MIN, BPM_FMC130M_4CH_TEST_DATA_EN_MAX,
            NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

disp_op_t fmc130m_4ch_test_data_en_exp = {
    .name = FMC130M_4CH_NAME_TEST_DATA_EN,
    .opcode = FMC130M_4CH_OPCODE_TEST_DATA_EN,
    .func_fp = RW_PARAM_FUNC_NAME(fmc130m_4ch, test_data_en),
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

#define BPM_FMC130M_4CH_SI571_OE_MIN            0 /* SI571 Output disable */
#define BPM_FMC130M_4CH_SI571_OE_MAX            1 /* SI571 Output enable */

RW_PARAM_FUNC(fmc130m_4ch, si571_oe) {
    SET_GET_PARAM(fmc130m_4ch, FMC_130M_CTRL_REGS_OFFS, WB_FMC_130M_4CH_CSR,
            CLK_DISTRIB, SI571_OE, SINGLE_BIT_PARAM,
            BPM_FMC130M_4CH_SI571_OE_MIN, BPM_FMC130M_4CH_SI571_OE_MAX, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

disp_op_t fmc130m_4ch_si571_oe_exp = {
    .name = FMC130M_4CH_NAME_SI571_OE,
    .opcode = FMC130M_4CH_OPCODE_SI571_OE,
    .func_fp = RW_PARAM_FUNC_NAME(fmc130m_4ch, si571_oe),
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

/******************************** Chips Export functions *************************/

/* Macros to avoid repetition of the function body AD9510 */
typedef smch_err_e (*smch_ad9510_func_fp) (smch_ad9510_t *self, uint32_t param);

#define FMC130M_4CH_AD9510_FUNC_NAME(func_name)                                 \
    _fmc130m_4ch_ad9510_ ## func_name

#define FMC130M_4CH_AD9510_FUNC_NAME_HEADER(func_name)                          \
    static int FMC130M_4CH_AD9510_FUNC_NAME(func_name) (void *owner, void *args, void *ret)

#define FMC130M_4CH_AD9510_FUNC_BODY(owner, args, ret, func, error_msg)         \
    do {                                                                        \
        (void) ret;                                                             \
        assert (owner);                                                         \
        assert (args);                                                          \
                                                                                \
        int err = -FMC130M_4CH_OK;                                              \
        SMIO_OWNER_TYPE *self = SMIO_EXP_OWNER(owner);                          \
        smch_ad9510_t *smch_ad9510 = SMIO_AD9510_HANDLER(self);                 \
        uint32_t rw = *(uint32_t *) EXP_MSG_ZMQ_FIRST_ARG(args);                \
        (void) rw;  /* Ignored for now */                                       \
        uint32_t param = *(uint32_t *) EXP_MSG_ZMQ_NEXT_ARG(args);              \
                                                                                \
        FMC130M_4CH_CHECK_ACTIVE(self);                                         \
                                                                                \
        /* Call specific function */                                            \
        smch_err_e serr = ((smch_ad9510_func_fp) func) (smch_ad9510, param);    \
        ASSERT_TEST(serr == SMCH_SUCCESS, error_msg,                            \
                err_smpr_write, -FMC130M_4CH_ERR);                              \
                                                                                \
err_smpr_write:                                                                 \
        return err;                                                             \
                                                                                \
    } while(0)

FMC130M_4CH_AD9510_FUNC_NAME_HEADER(pll_a_div)
{
    FMC130M_4CH_AD9510_FUNC_BODY(owner, args, ret, smch_ad9510_pll_a_div,
            "Could not set AD9510 PLL A divider");
}

disp_op_t fmc130m_4ch_ad9510_pll_a_div_exp = {
    .name = FMC130M_4CH_NAME_AD9510_PLL_A_DIV,
    .opcode = FMC130M_4CH_OPCODE_AD9510_PLL_A_DIV,
    .func_fp = FMC130M_4CH_AD9510_FUNC_NAME(pll_a_div),
    .retval = DISP_ARG_END,
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

FMC130M_4CH_AD9510_FUNC_NAME_HEADER(pll_b_div)
{
    FMC130M_4CH_AD9510_FUNC_BODY(owner, args, ret, smch_ad9510_pll_b_div,
            "Could not set AD9510 PLL B divider");
}

disp_op_t fmc130m_4ch_ad9510_pll_b_div_exp = {
    .name = FMC130M_4CH_NAME_AD9510_PLL_B_DIV,
    .opcode = FMC130M_4CH_OPCODE_AD9510_PLL_B_DIV,
    .func_fp = FMC130M_4CH_AD9510_FUNC_NAME(pll_b_div),
    .retval = DISP_ARG_END,
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

FMC130M_4CH_AD9510_FUNC_NAME_HEADER(pll_prescaler)
{
    FMC130M_4CH_AD9510_FUNC_BODY(owner, args, ret, smch_ad9510_pll_prescaler,
            "Could not set AD9510 PLL Prescaler");
}

disp_op_t fmc130m_4ch_ad9510_pll_prescaler_exp = {
    .name = FMC130M_4CH_NAME_AD9510_PLL_PRESCALER,
    .opcode = FMC130M_4CH_OPCODE_AD9510_PLL_PRESCALER,
    .func_fp = FMC130M_4CH_AD9510_FUNC_NAME(pll_prescaler),
    .retval = DISP_ARG_END,
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

FMC130M_4CH_AD9510_FUNC_NAME_HEADER(r_div)
{
    FMC130M_4CH_AD9510_FUNC_BODY(owner, args, ret, smch_ad9510_r_div,
            "Could not set AD9510 R Divider");
}

disp_op_t fmc130m_4ch_ad9510_r_div_exp = {
    .name = FMC130M_4CH_NAME_AD9510_R_DIV,
    .opcode = FMC130M_4CH_OPCODE_AD9510_R_DIV,
    .func_fp = FMC130M_4CH_AD9510_FUNC_NAME(r_div),
    .retval = DISP_ARG_END,
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

FMC130M_4CH_AD9510_FUNC_NAME_HEADER(pll_pdown)
{
    FMC130M_4CH_AD9510_FUNC_BODY(owner, args, ret, smch_ad9510_pll_pdown,
            "Could not power down AD9510 PLL");
}

disp_op_t fmc130m_4ch_ad9510_pll_pdown_exp = {
    .name = FMC130M_4CH_NAME_AD9510_PLL_PDOWN,
    .opcode = FMC130M_4CH_OPCODE_AD9510_PLL_PDOWN,
    .func_fp = FMC130M_4CH_AD9510_FUNC_NAME(pll_pdown),
    .retval = DISP_ARG_END,
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

FMC130M_4CH_AD9510_FUNC_NAME_HEADER(mux_status)
{
    FMC130M_4CH_AD9510_FUNC_BODY(owner, args, ret, smch_ad9510_mux_status,
            "Could not set AD9510 MUX status");
}

disp_op_t fmc130m_4ch_ad9510_mux_status_exp = {
    .name = FMC130M_4CH_NAME_AD9510_MUX_STATUS,
    .opcode = FMC130M_4CH_OPCODE_AD9510_MUX_STATUS,
    .func_fp = FMC130M_4CH_AD9510_FUNC_NAME(mux_status),
    .retval = DISP_ARG_END,
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

FMC130M_4CH_AD9510_FUNC_NAME_HEADER(cp_current)
{
    FMC130M_4CH_AD9510_FUNC_BODY(owner, args, ret, smch_ad9510_cp_current,
            "Could not set AD9510 CP current");
}

disp_op_t fmc130m_4ch_ad9510_cp_current_exp = {
    .name = FMC130M_4CH_NAME_AD9510_CP_CURRENT,
    .opcode = FMC130M_4CH_OPCODE_AD9510_CP_CURRENT,
    .func_fp = FMC130M_4CH_AD9510_FUNC_NAME(cp_current),
    .retval = DISP_ARG_END,
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

FMC130M_4CH_AD9510_FUNC_NAME_HEADER(outputs)
{
    FMC130M_4CH_AD9510_FUNC_BODY(owner, args, ret, smch_ad9510_outputs,
            "Could not set AD9510 Outputs");
}

disp_op_t fmc130m_4ch_ad9510_outputs_exp = {
    .name = FMC130M_4CH_NAME_AD9510_OUTPUTS,
    .opcode = FMC130M_4CH_OPCODE_AD9510_OUTPUTS,
    .func_fp = FMC130M_4CH_AD9510_FUNC_NAME(outputs),
    .retval = DISP_ARG_END,
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

FMC130M_4CH_AD9510_FUNC_NAME_HEADER(pll_clk_sel)
{
    FMC130M_4CH_AD9510_FUNC_BODY(owner, args, ret, smch_ad9510_pll_clk_sel,
            "Could not set AD9510 PLL CLK selection");
}

disp_op_t fmc130m_4ch_ad9510_pll_clk_sel_exp = {
    .name = FMC130M_4CH_NAME_AD9510_PLL_CLK_SEL,
    .opcode = FMC130M_4CH_OPCODE_AD9510_PLL_CLK_SEL,
    .func_fp = FMC130M_4CH_AD9510_FUNC_NAME(pll_clk_sel),
    .retval = DISP_ARG_END,
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

/* Macros to avoid repetition of the function body Si57X */
typedef smch_err_e (*smch_si57x_func_fp) (smch_si57x_t *self, double param);

#define FMC130M_4CH_SI571_FUNC_NAME(func_name)                                  \
    _fmc130m_4ch_si571_ ## func_name

#define FMC130M_4CH_SI571_FUNC_NAME_HEADER(func_name)                           \
    static int FMC130M_4CH_SI571_FUNC_NAME(func_name) (void *owner, void *args, void *ret)

#define FMC130M_4CH_SI571_FUNC_BODY(owner, args, ret, func, error_msg)          \
    do {                                                                        \
        (void) ret;                                                             \
        assert (owner);                                                         \
        assert (args);                                                          \
                                                                                \
        int err = -FMC130M_4CH_OK;                                              \
        SMIO_OWNER_TYPE *self = SMIO_EXP_OWNER(owner);                          \
        smch_si57x_t *smch_si57x = SMIO_SI57X_HANDLER(self);                    \
        uint32_t rw = *(uint32_t *) EXP_MSG_ZMQ_FIRST_ARG(args);                \
        (void) rw;  /* Ignored for now */                                       \
        uint32_t param = *(uint32_t *) EXP_MSG_ZMQ_NEXT_ARG(args);              \
                                                                                \
        FMC130M_4CH_CHECK_ACTIVE(self);                                         \
                                                                                \
        /* Call specific function */                                            \
        smch_err_e serr = ((smch_si57x_func_fp) func) (smch_si57x, param);      \
        ASSERT_TEST(serr == SMCH_SUCCESS, error_msg,                            \
                err_smpr_write, -FMC130M_4CH_ERR);                              \
                                                                                \
err_smpr_write:                                                                 \
        return err;                                                             \
                                                                                \
    } while(0)

FMC130M_4CH_SI571_FUNC_NAME_HEADER(set_freq)
{
    FMC130M_4CH_SI571_FUNC_BODY(owner, args, ret, smch_si57x_set_freq,
            "Could not set SI571 frequency");
}

disp_op_t fmc130m_4ch_si571_set_freq_exp = {
    .name = FMC130M_4CH_NAME_SI571_SET_FREQ,
    .opcode = FMC130M_4CH_OPCODE_SI571_SET_FREQ,
    .func_fp = FMC130M_4CH_SI571_FUNC_NAME(set_freq),
    .retval = DISP_ARG_END,
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
        DISP_ARG_END
    }
};

FMC130M_4CH_SI571_FUNC_NAME_HEADER(get_defaults)
{
    FMC130M_4CH_SI571_FUNC_BODY(owner, args, ret, smch_si57x_get_defaults,
            "Could not restart SI571 to its defaults");
}

disp_op_t fmc130m_4ch_si571_get_defaults_exp = {
    .name = FMC130M_4CH_NAME_SI571_GET_DEFAULTS,
    .opcode = FMC130M_4CH_OPCODE_SI571_GET_DEFAULTS,
    .func_fp = FMC130M_4CH_SI571_FUNC_NAME(get_defaults),
    .retval = DISP_ARG_END,
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
        DISP_ARG_END
    }
};

const disp_op_t *fmc130m_exp_ops [] = {
    &fmc130m_4ch_leds_exp,
    &fmc130m_4ch_pll_func_exp,
    &fmc130m_4ch_ad9510_cfg_defaults_exp,
    &fmc130m_4ch_adc_data0_exp,
    &fmc130m_4ch_adc_data1_exp,
    &fmc130m_4ch_adc_data2_exp,
    &fmc130m_4ch_adc_data3_exp,
    &fmc130m_4ch_adc_dly_val0_exp,
    &fmc130m_4ch_adc_dly_val1_exp,
    &fmc130m_4ch_adc_dly_val2_exp,
    &fmc130m_4ch_adc_dly_val3_exp,
    &fmc130m_4ch_adc_dly_line0_exp,
    &fmc130m_4ch_adc_dly_line1_exp,
    &fmc130m_4ch_adc_dly_line2_exp,
    &fmc130m_4ch_adc_dly_line3_exp,
    &fmc130m_4ch_adc_dly_updt0_exp,
    &fmc130m_4ch_adc_dly_updt1_exp,
    &fmc130m_4ch_adc_dly_updt2_exp,
    &fmc130m_4ch_adc_dly_updt3_exp,
    &fmc130m_4ch_adc_dly0_exp,
    &fmc130m_4ch_adc_dly1_exp,
    &fmc130m_4ch_adc_dly2_exp,
    &fmc130m_4ch_adc_dly3_exp,
    &fmc130m_4ch_test_data_en_exp,
    &fmc130m_4ch_si571_oe_exp,
    &fmc130m_4ch_ad9510_pll_a_div_exp,
    &fmc130m_4ch_ad9510_pll_b_div_exp,
    &fmc130m_4ch_ad9510_pll_prescaler_exp,
    &fmc130m_4ch_ad9510_r_div_exp,
    &fmc130m_4ch_ad9510_pll_pdown_exp,
    &fmc130m_4ch_ad9510_mux_status_exp,
    &fmc130m_4ch_ad9510_cp_current_exp,
    &fmc130m_4ch_ad9510_outputs_exp,
    &fmc130m_4ch_ad9510_pll_clk_sel_exp,
    &fmc130m_4ch_si571_set_freq_exp,
    &fmc130m_4ch_si571_get_defaults_exp,
    &disp_op_end
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

    self->id = FMC130M_4CH_SDB_DEVID;
    self->name = strdup (FMC130M_4CH_SDB_NAME);
    ASSERT_ALLOC(self->name, err_name_alloc, SMIO_ERR_ALLOC);

    /* Set SMIO ops pointers */
    self->ops = &fmc130m_4ch_ops;
    self->thsafe_client_ops = &smio_thsafe_client_zmq_ops;
    self->exp_ops = fmc130m_exp_ops;

    /* Initialize specific structure */
    self->smio_handler = smio_fmc130m_4ch_new (self);
    ASSERT_ALLOC(self->smio_handler, err_smio_handler_alloc, SMIO_ERR_ALLOC);

    return err;

err_smio_handler_alloc:
    free (self->name);
err_name_alloc:
    return err;
}

/* Destroy sm_io instance of fmc130m_4ch */
smio_err_e fmc130m_4ch_shutdown (smio_t *self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fmc130m_4ch_exp] Shutting down fmc130m_4ch\n");

    smio_fmc130m_4ch_destroy ((smio_fmc130m_4ch_t **) &self->smio_handler);
    self->exp_ops = NULL;
    self->thsafe_client_ops = NULL;
    self->ops = NULL;
    free (self->name);

    return SMIO_SUCCESS;
}

const smio_bootstrap_ops_t fmc130m_4ch_bootstrap_ops = {
    .init = fmc130m_4ch_init,
    .shutdown = fmc130m_4ch_shutdown,
    .config_defaults = fmc130m_4ch_config_defaults
};
