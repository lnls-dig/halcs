/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _SM_IO_RFFE_EXPORTS_H_
#define _SM_IO_RFFE_EXPORTS_H_

#include "disptable.h"

extern disp_op_t rffe_set_get_att_exp;
extern disp_op_t rffe_get_temp_ac_exp;
extern disp_op_t rffe_get_temp_bd_exp;
extern disp_op_t rffe_set_get_set_point_ac_exp;
extern disp_op_t rffe_set_get_set_point_bd_exp;
extern disp_op_t rffe_set_get_temp_control_exp;
extern disp_op_t rffe_set_get_heater_ac_exp;
extern disp_op_t rffe_set_get_heater_bd_exp;
extern disp_op_t rffe_set_get_reset_exp;
extern disp_op_t rffe_set_get_reprog_exp;
extern disp_op_t rffe_set_get_data_exp;
extern disp_op_t rffe_get_version_exp;
extern disp_op_t rffe_set_get_pid_ac_kp_exp;
extern disp_op_t rffe_set_get_pid_ac_ti_exp;
extern disp_op_t rffe_set_get_pid_ac_td_exp;
extern disp_op_t rffe_set_get_pid_bd_kp_exp;
extern disp_op_t rffe_set_get_pid_bd_ti_exp;
extern disp_op_t rffe_set_get_pid_bd_td_exp;

extern const disp_op_t *rffe_exp_ops [];

#endif

