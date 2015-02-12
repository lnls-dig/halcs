/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _SM_IO_FMC130M_4CH_EXPORTS_H_
#define _SM_IO_FMC130M_4CH_EXPORTS_H_

#include "dispatch_table.h"

extern disp_op_t fmc130m_4ch_leds_exp;
extern disp_op_t fmc130m_4ch_si571_oe_exp;
extern disp_op_t fmc130m_4ch_pll_func_exp;
extern disp_op_t fmc130m_4ch_pll_status_exp;
extern disp_op_t fmc130m_4ch_clk_sel_exp;
extern disp_op_t fmc130m_4ch_adc_data0_exp;
extern disp_op_t fmc130m_4ch_adc_data1_exp;
extern disp_op_t fmc130m_4ch_adc_data2_exp;
extern disp_op_t fmc130m_4ch_adc_data3_exp;
extern disp_op_t fmc130m_4ch_adc_dly_val0_exp;
extern disp_op_t fmc130m_4ch_adc_dly_val1_exp;
extern disp_op_t fmc130m_4ch_adc_dly_val2_exp;
extern disp_op_t fmc130m_4ch_adc_dly_val3_exp;
extern disp_op_t fmc130m_4ch_adc_dly_line0_exp;
extern disp_op_t fmc130m_4ch_adc_dly_line1_exp;
extern disp_op_t fmc130m_4ch_adc_dly_line2_exp;
extern disp_op_t fmc130m_4ch_adc_dly_line3_exp;
extern disp_op_t fmc130m_4ch_adc_dly_updt0_exp;
extern disp_op_t fmc130m_4ch_adc_dly_updt1_exp;
extern disp_op_t fmc130m_4ch_adc_dly_updt2_exp;
extern disp_op_t fmc130m_4ch_adc_dly_updt3_exp;
extern disp_op_t fmc130m_4ch_adc_dly0_exp;
extern disp_op_t fmc130m_4ch_adc_dly1_exp;
extern disp_op_t fmc130m_4ch_adc_dly2_exp;
extern disp_op_t fmc130m_4ch_adc_dly3_exp;
extern disp_op_t fmc130m_4ch_test_data_en_exp;
extern disp_op_t fmc130m_4ch_trig_dir_exp;
extern disp_op_t fmc130m_4ch_trig_term_exp;
extern disp_op_t fmc130m_4ch_trig_val_exp;
extern disp_op_t fmc130m_4ch_ad9510_cfg_defaults_exp;
extern disp_op_t fmc130m_4ch_ad9510_pll_a_div_exp;
extern disp_op_t fmc130m_4ch_ad9510_pll_b_div_exp;
extern disp_op_t fmc130m_4ch_ad9510_pll_prescaler_exp;
extern disp_op_t fmc130m_4ch_ad9510_r_div_exp;
extern disp_op_t fmc130m_4ch_ad9510_pll_pdown_exp;
extern disp_op_t fmc130m_4ch_ad9510_mux_status_exp;
extern disp_op_t fmc130m_4ch_ad9510_cp_current_exp;
extern disp_op_t fmc130m_4ch_ad9510_outputs_exp;
extern disp_op_t fmc130m_4ch_ad9510_pll_clk_sel_exp;
extern disp_op_t fmc130m_4ch_si571_set_freq_exp;
extern disp_op_t fmc130m_4ch_si571_get_defaults_exp;

extern const disp_op_t *fmc130m_4ch_exp_ops [];

#endif

