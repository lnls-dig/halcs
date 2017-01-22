/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "sm_io_exports_helper.h"
#include "sm_io_codes.h"

/* Description SMIO FMC_ACTIVE_CLK functions */

disp_op_t fmc_active_clk_si571_oe_exp = {
    .name = FMC_ACTIVE_CLK_NAME_SI571_OE,
    .opcode = FMC_ACTIVE_CLK_OPCODE_SI571_OE,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc_active_clk_pll_func_exp = {
    .name = FMC_ACTIVE_CLK_NAME_PLL_FUNCTION,
    .opcode = FMC_ACTIVE_CLK_OPCODE_PLL_FUNCTION,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc_active_clk_pll_status_exp = {
    .name = FMC_ACTIVE_CLK_NAME_PLL_STATUS,
    .opcode = FMC_ACTIVE_CLK_OPCODE_PLL_STATUS,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc_active_clk_clk_sel_exp = {
    .name = FMC_ACTIVE_CLK_NAME_CLK_SEL,
    .opcode = FMC_ACTIVE_CLK_OPCODE_CLK_SEL,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc_active_clk_ad9510_cfg_defaults_exp = {
    .name = FMC_ACTIVE_CLK_NAME_AD9510_CFG_DEFAULTS,
    .opcode = FMC_ACTIVE_CLK_OPCODE_AD9510_CFG_DEFAULTS,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc_active_clk_ad9510_pll_a_div_exp = {
    .name = FMC_ACTIVE_CLK_NAME_AD9510_PLL_A_DIV,
    .opcode = FMC_ACTIVE_CLK_OPCODE_AD9510_PLL_A_DIV,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc_active_clk_ad9510_pll_b_div_exp = {
    .name = FMC_ACTIVE_CLK_NAME_AD9510_PLL_B_DIV,
    .opcode = FMC_ACTIVE_CLK_OPCODE_AD9510_PLL_B_DIV,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc_active_clk_ad9510_pll_prescaler_exp = {
    .name = FMC_ACTIVE_CLK_NAME_AD9510_PLL_PRESCALER,
    .opcode = FMC_ACTIVE_CLK_OPCODE_AD9510_PLL_PRESCALER,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc_active_clk_ad9510_r_div_exp = {
    .name = FMC_ACTIVE_CLK_NAME_AD9510_R_DIV,
    .opcode = FMC_ACTIVE_CLK_OPCODE_AD9510_R_DIV,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc_active_clk_ad9510_pll_pdown_exp = {
    .name = FMC_ACTIVE_CLK_NAME_AD9510_PLL_PDOWN,
    .opcode = FMC_ACTIVE_CLK_OPCODE_AD9510_PLL_PDOWN,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc_active_clk_ad9510_mux_status_exp = {
    .name = FMC_ACTIVE_CLK_NAME_AD9510_MUX_STATUS,
    .opcode = FMC_ACTIVE_CLK_OPCODE_AD9510_MUX_STATUS,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc_active_clk_ad9510_cp_current_exp = {
    .name = FMC_ACTIVE_CLK_NAME_AD9510_CP_CURRENT,
    .opcode = FMC_ACTIVE_CLK_OPCODE_AD9510_CP_CURRENT,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc_active_clk_ad9510_outputs_exp = {
    .name = FMC_ACTIVE_CLK_NAME_AD9510_OUTPUTS,
    .opcode = FMC_ACTIVE_CLK_OPCODE_AD9510_OUTPUTS,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc_active_clk_ad9510_pll_clk_sel_exp = {
    .name = FMC_ACTIVE_CLK_NAME_AD9510_PLL_CLK_SEL,
    .opcode = FMC_ACTIVE_CLK_OPCODE_AD9510_PLL_CLK_SEL,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc_active_clk_si571_freq_exp = {
    .name = FMC_ACTIVE_CLK_NAME_SI571_FREQ,
    .opcode = FMC_ACTIVE_CLK_OPCODE_SI571_FREQ,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
        DISP_ARG_END
    }
};

disp_op_t fmc_active_clk_si571_get_defaults_exp = {
    .name = FMC_ACTIVE_CLK_NAME_SI571_GET_DEFAULTS,
    .opcode = FMC_ACTIVE_CLK_OPCODE_SI571_GET_DEFAULTS,
    .retval = DISP_ARG_END,
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
        DISP_ARG_END
    }
};

disp_op_t fmc_active_clk_rst_isla216p_exp = {
    .name = FMC_ACTIVE_CLK_NAME_RST_ISLA216P,
    .opcode = FMC_ACTIVE_CLK_OPCODE_RST_ISLA216P,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc_active_clk_ad9510_data_exp = {
    .name = FMC_ACTIVE_CLK_NAME_AD9510_DATA,
    .opcode = FMC_ACTIVE_CLK_OPCODE_AD9510_DATA,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

/* Exported function description */
const disp_op_t *fmc_active_clk_exp_ops [] = {
    &fmc_active_clk_si571_oe_exp,
    &fmc_active_clk_pll_func_exp,
    &fmc_active_clk_pll_status_exp,
    &fmc_active_clk_clk_sel_exp,
    &fmc_active_clk_ad9510_cfg_defaults_exp,
    &fmc_active_clk_ad9510_pll_a_div_exp,
    &fmc_active_clk_ad9510_pll_b_div_exp,
    &fmc_active_clk_ad9510_pll_prescaler_exp,
    &fmc_active_clk_ad9510_r_div_exp,
    &fmc_active_clk_ad9510_pll_pdown_exp,
    &fmc_active_clk_ad9510_mux_status_exp,
    &fmc_active_clk_ad9510_cp_current_exp,
    &fmc_active_clk_ad9510_outputs_exp,
    &fmc_active_clk_ad9510_pll_clk_sel_exp,
    &fmc_active_clk_si571_freq_exp,
    &fmc_active_clk_si571_get_defaults_exp,
    &fmc_active_clk_rst_isla216p_exp,
    &fmc_active_clk_ad9510_data_exp,
    NULL
};

