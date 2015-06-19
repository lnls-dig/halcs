/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include "sm_io_exports_helper.h"
#include "sm_io_codes.h"

/* Description SMIO FMC130M_4CH functions */

disp_op_t fmc130m_4ch_leds_exp = {
    .name = FMC130M_4CH_NAME_LEDS,
    .opcode = FMC130M_4CH_OPCODE_LEDS,
    .retval = DISP_ARG_END,
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc130m_4ch_si571_oe_exp = {
    .name = FMC130M_4CH_NAME_SI571_OE,
    .opcode = FMC130M_4CH_OPCODE_SI571_OE,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};


disp_op_t fmc130m_4ch_pll_func_exp = {
    .name = FMC130M_4CH_NAME_PLL_FUNCTION,
    .opcode = FMC130M_4CH_OPCODE_PLL_FUNCTION,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc130m_4ch_pll_status_exp = {
    .name = FMC130M_4CH_NAME_PLL_STATUS,
    .opcode = FMC130M_4CH_OPCODE_PLL_STATUS,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc130m_4ch_clk_sel_exp = {
    .name = FMC130M_4CH_NAME_CLK_SEL,
    .opcode = FMC130M_4CH_OPCODE_CLK_SEL,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc130m_4ch_adc_rand_exp = {
    .name = FMC130M_4CH_NAME_ADC_RAND,
    .opcode = FMC130M_4CH_OPCODE_ADC_RAND,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc130m_4ch_adc_dith_exp = {
    .name = FMC130M_4CH_NAME_ADC_DITH,
    .opcode = FMC130M_4CH_OPCODE_ADC_DITH,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc130m_4ch_adc_shdn_exp = {
    .name = FMC130M_4CH_NAME_ADC_SHDN,
    .opcode = FMC130M_4CH_OPCODE_ADC_SHDN,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc130m_4ch_adc_pga_exp = {
    .name = FMC130M_4CH_NAME_ADC_PGA,
    .opcode = FMC130M_4CH_OPCODE_ADC_PGA,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc130m_4ch_adc_data0_exp = {
    .name = FMC130M_4CH_NAME_ADC_DATA0,
    .opcode = FMC130M_4CH_OPCODE_ADC_DATA0,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc130m_4ch_adc_data1_exp = {
    .name = FMC130M_4CH_NAME_ADC_DATA1,
    .opcode = FMC130M_4CH_OPCODE_ADC_DATA1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc130m_4ch_adc_data2_exp = {
    .name = FMC130M_4CH_NAME_ADC_DATA2,
    .opcode = FMC130M_4CH_OPCODE_ADC_DATA2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc130m_4ch_adc_data3_exp = {
    .name = FMC130M_4CH_NAME_ADC_DATA3,
    .opcode = FMC130M_4CH_OPCODE_ADC_DATA3,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc130m_4ch_adc_dly_val0_exp = {
    .name = FMC130M_4CH_NAME_ADC_DLY_VAL0,
    .opcode = FMC130M_4CH_OPCODE_ADC_DLY_VAL0,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc130m_4ch_adc_dly_val1_exp = {
    .name = FMC130M_4CH_NAME_ADC_DLY_VAL1,
    .opcode = FMC130M_4CH_OPCODE_ADC_DLY_VAL1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc130m_4ch_adc_dly_val2_exp = {
    .name = FMC130M_4CH_NAME_ADC_DLY_VAL2,
    .opcode = FMC130M_4CH_OPCODE_ADC_DLY_VAL2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc130m_4ch_adc_dly_val3_exp = {
    .name = FMC130M_4CH_NAME_ADC_DLY_VAL3,
    .opcode = FMC130M_4CH_OPCODE_ADC_DLY_VAL3,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc130m_4ch_adc_dly_line0_exp = {
    .name = FMC130M_4CH_NAME_ADC_DLY_LINE0,
    .opcode = FMC130M_4CH_OPCODE_ADC_DLY_LINE0,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc130m_4ch_adc_dly_line1_exp = {
    .name = FMC130M_4CH_NAME_ADC_DLY_LINE1,
    .opcode = FMC130M_4CH_OPCODE_ADC_DLY_LINE1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};


disp_op_t fmc130m_4ch_adc_dly_line2_exp = {
    .name = FMC130M_4CH_NAME_ADC_DLY_LINE2,
    .opcode = FMC130M_4CH_OPCODE_ADC_DLY_LINE2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc130m_4ch_adc_dly_line3_exp = {
    .name = FMC130M_4CH_NAME_ADC_DLY_LINE3,
    .opcode = FMC130M_4CH_OPCODE_ADC_DLY_LINE3,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc130m_4ch_adc_dly_updt0_exp = {
    .name = FMC130M_4CH_NAME_ADC_DLY_UPDT0,
    .opcode = FMC130M_4CH_OPCODE_ADC_DLY_UPDT0,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc130m_4ch_adc_dly_updt1_exp = {
    .name = FMC130M_4CH_NAME_ADC_DLY_UPDT1,
    .opcode = FMC130M_4CH_OPCODE_ADC_DLY_UPDT1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc130m_4ch_adc_dly_updt2_exp = {
    .name = FMC130M_4CH_NAME_ADC_DLY_UPDT2,
    .opcode = FMC130M_4CH_OPCODE_ADC_DLY_UPDT2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc130m_4ch_adc_dly_updt3_exp = {
    .name = FMC130M_4CH_NAME_ADC_DLY_UPDT3,
    .opcode = FMC130M_4CH_OPCODE_ADC_DLY_UPDT3,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc130m_4ch_adc_dly0_exp = {
    .name = FMC130M_4CH_NAME_ADC_DLY0,
    .opcode = FMC130M_4CH_OPCODE_ADC_DLY0,
    .retval = DISP_ARG_END,
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc130m_4ch_adc_dly1_exp = {
    .name = FMC130M_4CH_NAME_ADC_DLY1,
    .opcode = FMC130M_4CH_OPCODE_ADC_DLY1,
    .retval = DISP_ARG_END,
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc130m_4ch_adc_dly2_exp = {
    .name = FMC130M_4CH_NAME_ADC_DLY2,
    .opcode = FMC130M_4CH_OPCODE_ADC_DLY2,
    .retval = DISP_ARG_END,
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc130m_4ch_adc_dly3_exp = {
    .name = FMC130M_4CH_NAME_ADC_DLY3,
    .opcode = FMC130M_4CH_OPCODE_ADC_DLY3,
    .retval = DISP_ARG_END,
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc130m_4ch_test_data_en_exp = {
    .name = FMC130M_4CH_NAME_TEST_DATA_EN,
    .opcode = FMC130M_4CH_OPCODE_TEST_DATA_EN,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc130m_4ch_trig_dir_exp = {
    .name = FMC130M_4CH_NAME_TRIG_DIR,
    .opcode = FMC130M_4CH_OPCODE_TRIG_DIR,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc130m_4ch_trig_term_exp = {
    .name = FMC130M_4CH_NAME_TRIG_TERM,
    .opcode = FMC130M_4CH_OPCODE_TRIG_TERM,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc130m_4ch_trig_val_exp = {
    .name = FMC130M_4CH_NAME_TRIG_VAL,
    .opcode = FMC130M_4CH_OPCODE_TRIG_VAL,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc130m_4ch_ad9510_cfg_defaults_exp = {
    .name = FMC130M_4CH_NAME_AD9510_CFG_DEFAULTS,
    .opcode = FMC130M_4CH_OPCODE_AD9510_CFG_DEFAULTS,
    .retval = DISP_ARG_END,
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_END
    }
};

disp_op_t fmc130m_4ch_ad9510_pll_a_div_exp = {
    .name = FMC130M_4CH_NAME_AD9510_PLL_A_DIV,
    .opcode = FMC130M_4CH_OPCODE_AD9510_PLL_A_DIV,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc130m_4ch_ad9510_pll_b_div_exp = {
    .name = FMC130M_4CH_NAME_AD9510_PLL_B_DIV,
    .opcode = FMC130M_4CH_OPCODE_AD9510_PLL_B_DIV,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc130m_4ch_ad9510_pll_prescaler_exp = {
    .name = FMC130M_4CH_NAME_AD9510_PLL_PRESCALER,
    .opcode = FMC130M_4CH_OPCODE_AD9510_PLL_PRESCALER,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc130m_4ch_ad9510_r_div_exp = {
    .name = FMC130M_4CH_NAME_AD9510_R_DIV,
    .opcode = FMC130M_4CH_OPCODE_AD9510_R_DIV,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc130m_4ch_ad9510_pll_pdown_exp = {
    .name = FMC130M_4CH_NAME_AD9510_PLL_PDOWN,
    .opcode = FMC130M_4CH_OPCODE_AD9510_PLL_PDOWN,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc130m_4ch_ad9510_mux_status_exp = {
    .name = FMC130M_4CH_NAME_AD9510_MUX_STATUS,
    .opcode = FMC130M_4CH_OPCODE_AD9510_MUX_STATUS,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc130m_4ch_ad9510_cp_current_exp = {
    .name = FMC130M_4CH_NAME_AD9510_CP_CURRENT,
    .opcode = FMC130M_4CH_OPCODE_AD9510_CP_CURRENT,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc130m_4ch_ad9510_outputs_exp = {
    .name = FMC130M_4CH_NAME_AD9510_OUTPUTS,
    .opcode = FMC130M_4CH_OPCODE_AD9510_OUTPUTS,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc130m_4ch_ad9510_pll_clk_sel_exp = {
    .name = FMC130M_4CH_NAME_AD9510_PLL_CLK_SEL,
    .opcode = FMC130M_4CH_OPCODE_AD9510_PLL_CLK_SEL,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc130m_4ch_si571_set_freq_exp = {
    .name = FMC130M_4CH_NAME_SI571_SET_FREQ,
    .opcode = FMC130M_4CH_OPCODE_SI571_SET_FREQ,
    .retval = DISP_ARG_END,
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
        DISP_ARG_END
    }
};

disp_op_t fmc130m_4ch_si571_get_defaults_exp = {
    .name = FMC130M_4CH_NAME_SI571_GET_DEFAULTS,
    .opcode = FMC130M_4CH_OPCODE_SI571_GET_DEFAULTS,
    .retval = DISP_ARG_END,
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
        DISP_ARG_END
    }
};

/* Exported function description */
const disp_op_t *fmc130m_4ch_exp_ops [] = {
    &fmc130m_4ch_leds_exp,
    &fmc130m_4ch_si571_oe_exp,
    &fmc130m_4ch_pll_func_exp,
    &fmc130m_4ch_pll_status_exp,
    &fmc130m_4ch_clk_sel_exp,
    &fmc130m_4ch_adc_rand_exp,
    &fmc130m_4ch_adc_dith_exp,
    &fmc130m_4ch_adc_shdn_exp,
    &fmc130m_4ch_adc_pga_exp,
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
    &fmc130m_4ch_trig_dir_exp,
    &fmc130m_4ch_trig_term_exp,
    &fmc130m_4ch_trig_val_exp,
    &fmc130m_4ch_ad9510_cfg_defaults_exp,
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
    NULL
};

