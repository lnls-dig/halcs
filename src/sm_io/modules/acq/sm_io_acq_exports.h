/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _SM_IO_ACQ_EXPORTS_H_
#define _SM_IO_ACQ_EXPORTS_H_

#include "disp_table.h"

extern disp_op_t acq_data_acquire_exp;
extern disp_op_t acq_check_data_acquire_exp;
extern disp_op_t acq_get_data_block_exp;
extern disp_op_t acq_cfg_trig_exp;
extern disp_op_t acq_hw_data_trig_pol_exp;
extern disp_op_t acq_hw_data_trig_sel_exp;
extern disp_op_t acq_hw_data_trig_filt_exp;
extern disp_op_t acq_hw_data_trig_thres_exp;
extern disp_op_t acq_hw_trig_dly_exp;
extern disp_op_t acq_sw_trig_exp;

extern const disp_op_t *acq_exp_ops [];

#endif

