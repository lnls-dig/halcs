/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _SM_IO_FMC250M_4CH_EXPORTS_H_
#define _SM_IO_FMC250M_4CH_EXPORTS_H_

#include "disptable.h"

#if 0
extern disp_op_t fmc250m_4ch_adc_rand_exp;
extern disp_op_t fmc250m_4ch_adc_dith_exp;
extern disp_op_t fmc250m_4ch_adc_shdn_exp;
extern disp_op_t fmc250m_4ch_adc_pga_exp;
#endif
extern disp_op_t fmc250m_4ch_adc_data0_exp;
extern disp_op_t fmc250m_4ch_adc_data1_exp;
extern disp_op_t fmc250m_4ch_adc_data2_exp;
extern disp_op_t fmc250m_4ch_adc_data3_exp;
#if 0
extern disp_op_t fmc250m_4ch_adc_dly_val0_exp;
extern disp_op_t fmc250m_4ch_adc_dly_val1_exp;
extern disp_op_t fmc250m_4ch_adc_dly_val2_exp;
extern disp_op_t fmc250m_4ch_adc_dly_val3_exp;
extern disp_op_t fmc250m_4ch_adc_dly_line0_exp;
extern disp_op_t fmc250m_4ch_adc_dly_line1_exp;
extern disp_op_t fmc250m_4ch_adc_dly_line2_exp;
extern disp_op_t fmc250m_4ch_adc_dly_line3_exp;
extern disp_op_t fmc250m_4ch_adc_dly_updt0_exp;
extern disp_op_t fmc250m_4ch_adc_dly_updt1_exp;
extern disp_op_t fmc250m_4ch_adc_dly_updt2_exp;
extern disp_op_t fmc250m_4ch_adc_dly_updt3_exp;
extern disp_op_t fmc250m_4ch_adc_dly0_exp;
extern disp_op_t fmc250m_4ch_adc_dly1_exp;
extern disp_op_t fmc250m_4ch_adc_dly2_exp;
extern disp_op_t fmc250m_4ch_adc_dly3_exp;
#endif
extern disp_op_t fmc250m_4ch_rst_adcs_exp;
extern disp_op_t fmc250m_4ch_rst_div_adcs_exp;
extern disp_op_t fmc250m_4ch_test_mode0_exp;
extern disp_op_t fmc250m_4ch_test_mode1_exp;
extern disp_op_t fmc250m_4ch_test_mode2_exp;
extern disp_op_t fmc250m_4ch_test_mode3_exp;

extern const disp_op_t *fmc250m_4ch_exp_ops [];

#endif

