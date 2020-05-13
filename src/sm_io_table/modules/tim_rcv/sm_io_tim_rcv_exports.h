/*
 * Copyright (C) 2017 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _SM_IO_TIM_RCV_EXPORTS_H_
#define _SM_IO_TIM_RCV_EXPORTS_H_

#include "disptable.h"

extern disp_op_t tim_rcv_phase_meas_navg_exp;
extern disp_op_t tim_rcv_dmtd_a_deglitcher_thres_exp;
extern disp_op_t tim_rcv_dmtd_b_deglitcher_thres_exp;
extern disp_op_t tim_rcv_phase_meas_exp;
extern disp_op_t tim_rcv_dmtd_a_freq_exp;
extern disp_op_t tim_rcv_dmtd_a_valid_exp;
extern disp_op_t tim_rcv_dmtd_b_freq_exp;
extern disp_op_t tim_rcv_dmtd_b_valid_exp;

extern const disp_op_t *tim_rcv_exp_ops [];

#endif

