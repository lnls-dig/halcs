/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _SM_IO_RFFE_EXPORTS_H_
#define _SM_IO_RFFE_EXPORTS_H_

#include "dispatch_table.h"

extern disp_op_t rffe_set_get_sw_exp;
extern disp_op_t rffe_set_get_att1_exp;
extern disp_op_t rffe_set_get_att2_exp;
extern disp_op_t rffe_get_temp1_exp;
extern disp_op_t rffe_get_temp2_exp;
extern disp_op_t rffe_get_temp3_exp;
extern disp_op_t rffe_get_temp4_exp;
extern disp_op_t rffe_set_get_set_point1_exp;
extern disp_op_t rffe_set_get_set_point2_exp;
extern disp_op_t rffe_set_get_temp_control_exp;
extern disp_op_t rffe_set_get_output1_exp;
extern disp_op_t rffe_set_get_output2_exp;
extern disp_op_t rffe_set_get_reset_exp;
extern disp_op_t rffe_set_get_reprog_exp;
extern disp_op_t rffe_set_get_data_exp;
extern disp_op_t rffe_get_version_exp;
extern disp_op_t rffe_set_get_sw_lvl_exp;

extern const disp_op_t *rffe_exp_ops [];

#endif

