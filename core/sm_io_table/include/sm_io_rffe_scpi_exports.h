/****************************************************************************
 * core/sm_io_table/include/sm_io_rffe_scpi_exports.h
 *
 *   Copyright (C) 2014 LNLS (www.lnls.br). All rights reserved.
 *   Copyright (C) 2019 Augusto Fraga Giachero. All rights reserved.
 *   Author: Augusto Fraga Giachero <afg@augustofg.net>
 *
 * This file is part of HALCS.
 *
 * HALCS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * HALCS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with HALCS. If not, see <https://www.gnu.org/licenses/>.
 *
 ****************************************************************************/

#ifndef _SM_IO_RFFE_SCPI_EXPORTS_H_
#define _SM_IO_RFFE_SCPI_EXPORTS_H_

#include "disptable.h"

extern disp_op_t rffe_scpi_set_get_att_exp;
extern disp_op_t rffe_scpi_get_temp_ac_exp;
extern disp_op_t rffe_scpi_get_temp_bd_exp;
extern disp_op_t rffe_scpi_set_get_set_point_ac_exp;
extern disp_op_t rffe_scpi_set_get_set_point_bd_exp;
extern disp_op_t rffe_scpi_set_get_temp_control_exp;
extern disp_op_t rffe_scpi_set_get_heater_ac_exp;
extern disp_op_t rffe_scpi_set_get_heater_bd_exp;
extern disp_op_t rffe_scpi_set_get_reset_exp;
extern disp_op_t rffe_scpi_set_get_reprog_exp;
extern disp_op_t rffe_scpi_set_get_data_exp;
extern disp_op_t rffe_scpi_get_version_exp;
extern disp_op_t rffe_scpi_set_get_pid_ac_kp_exp;
extern disp_op_t rffe_scpi_set_get_pid_ac_ti_exp;
extern disp_op_t rffe_scpi_set_get_pid_ac_td_exp;
extern disp_op_t rffe_scpi_set_get_pid_bd_kp_exp;
extern disp_op_t rffe_scpi_set_get_pid_bd_ti_exp;
extern disp_op_t rffe_scpi_set_get_pid_bd_td_exp;

extern const disp_op_t *rffe_scpi_exp_ops [];

#endif

