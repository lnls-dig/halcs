/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _SM_IO_AFC_TIMING_EXPORTS_H_
#define _SM_IO_AFC_TIMING_EXPORTS_H_

#include "disptable.h"

extern disp_op_t afc_timing_set_get_link_status_exp;
extern disp_op_t afc_timing_set_get_rxen_status_exp;
extern disp_op_t afc_timing_set_get_ref_clk_locked_exp;
extern disp_op_t afc_timing_set_get_evt_code_0_exp;
extern disp_op_t afc_timing_set_get_evt_code_1_exp;
extern disp_op_t afc_timing_set_get_evt_code_2_exp;
extern disp_op_t afc_timing_set_get_evt_code_3_exp;
extern disp_op_t afc_timing_set_get_evt_code_4_exp;
extern disp_op_t afc_timing_set_get_evt_code_5_exp;
extern disp_op_t afc_timing_set_get_evt_code_6_exp;
extern disp_op_t afc_timing_set_get_evt_delay_0_exp;
extern disp_op_t afc_timing_set_get_evt_delay_1_exp;
extern disp_op_t afc_timing_set_get_evt_delay_2_exp;
extern disp_op_t afc_timing_set_get_evt_delay_3_exp;
extern disp_op_t afc_timing_set_get_evt_delay_4_exp;
extern disp_op_t afc_timing_set_get_evt_delay_5_exp;
extern disp_op_t afc_timing_set_get_evt_delay_6_exp;
extern disp_op_t afc_timing_set_get_evt_width_0_exp;
extern disp_op_t afc_timing_set_get_evt_width_1_exp;
extern disp_op_t afc_timing_set_get_evt_width_2_exp;
extern disp_op_t afc_timing_set_get_evt_width_3_exp;
extern disp_op_t afc_timing_set_get_evt_width_4_exp;
extern disp_op_t afc_timing_set_get_evt_width_5_exp;
extern disp_op_t afc_timing_set_get_evt_width_6_exp;
extern disp_op_t afc_timing_set_get_freq_kp_exp;
extern disp_op_t afc_timing_set_get_freq_ki_exp;
extern disp_op_t afc_timing_set_get_phase_kp_exp;
extern disp_op_t afc_timing_set_get_phase_ki_exp;
extern disp_op_t afc_timing_set_get_phase_bias_exp;
extern disp_op_t afc_timing_set_get_avg_exponent_exp;

extern const disp_op_t *afc_timing_exp_ops [];

#endif

