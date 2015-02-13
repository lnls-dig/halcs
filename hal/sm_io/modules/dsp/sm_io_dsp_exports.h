/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _SM_IO_DSP_EXPORTS_H_
#define _SM_IO_DSP_EXPORTS_H_

#include "dispatch_table.h"

extern disp_op_t dsp_set_get_kx_exp;
extern disp_op_t dsp_set_get_ky_exp;
extern disp_op_t dsp_set_get_ksum_exp;
extern disp_op_t dsp_set_get_ds_tbt_thres_exp;
extern disp_op_t dsp_set_get_ds_fofb_thres_exp;
extern disp_op_t dsp_set_get_ds_monit_thres_exp;
extern disp_op_t dsp_set_get_monit_amp_ch0_exp;
extern disp_op_t dsp_set_get_monit_amp_ch1_exp;
extern disp_op_t dsp_set_get_monit_amp_ch2_exp;
extern disp_op_t dsp_set_get_monit_amp_ch3_exp;
extern disp_op_t dsp_set_get_monit_pos_x_exp;
extern disp_op_t dsp_set_get_monit_pos_y_exp;
extern disp_op_t dsp_set_get_monit_pos_q_exp;
extern disp_op_t dsp_set_get_monit_pos_sum_exp;

extern const disp_op_t *dsp_exp_ops [];

#endif

