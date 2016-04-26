/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _SM_IO_FMC_ACTIVE_CLK_EXPORTS_H_
#define _SM_IO_FMC_ACTIVE_CLK_EXPORTS_H_

#include "disptable.h"

extern disp_op_t fmc_active_clk_si571_oe_exp;
extern disp_op_t fmc_active_clk_pll_func_exp;
extern disp_op_t fmc_active_clk_pll_status_exp;
extern disp_op_t fmc_active_clk_clk_sel_exp;
extern disp_op_t fmc_active_clk_ad9510_cfg_defaults_exp;
extern disp_op_t fmc_active_clk_ad9510_pll_a_div_exp;
extern disp_op_t fmc_active_clk_ad9510_pll_b_div_exp;
extern disp_op_t fmc_active_clk_ad9510_pll_prescaler_exp;
extern disp_op_t fmc_active_clk_ad9510_r_div_exp;
extern disp_op_t fmc_active_clk_ad9510_pll_pdown_exp;
extern disp_op_t fmc_active_clk_ad9510_mux_status_exp;
extern disp_op_t fmc_active_clk_ad9510_cp_current_exp;
extern disp_op_t fmc_active_clk_ad9510_outputs_exp;
extern disp_op_t fmc_active_clk_ad9510_pll_clk_sel_exp;
extern disp_op_t fmc_active_clk_si571_set_freq_exp;
extern disp_op_t fmc_active_clk_si571_get_defaults_exp;

extern const disp_op_t *fmc_active_clk_exp_ops [];

#endif

