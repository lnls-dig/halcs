/*
 * Copyright (C) 2020 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _SM_IO_ORBIT_INTLK_EXPORTS_H_
#define _SM_IO_ORBIT_INTLK_EXPORTS_H_

#include "disptable.h"

extern disp_op_t orbit_intlk_set_get_led1_exp;
extern disp_op_t orbit_intlk_set_get_en_exp;
extern disp_op_t orbit_intlk_set_get_clr_exp;
extern disp_op_t orbit_intlk_set_get_min_sum_en_exp;
extern disp_op_t orbit_intlk_set_get_trans_en_exp;
extern disp_op_t orbit_intlk_set_get_trans_clr_exp;
extern disp_op_t orbit_intlk_set_get_ang_en_exp;
extern disp_op_t orbit_intlk_set_get_ang_clr_exp;
extern disp_op_t orbit_intlk_set_get_trans_bigger_x_exp;
extern disp_op_t orbit_intlk_set_get_trans_bigger_y_exp;
extern disp_op_t orbit_intlk_set_get_trans_bigger_ltc_x_exp;
extern disp_op_t orbit_intlk_set_get_trans_bigger_ltc_y_exp;
extern disp_op_t orbit_intlk_set_get_trans_bigger_any_exp;
extern disp_op_t orbit_intlk_set_get_trans_bigger_exp;
extern disp_op_t orbit_intlk_set_get_trans_bigger_ltc_exp;
extern disp_op_t orbit_intlk_set_get_ang_bigger_x_exp;
extern disp_op_t orbit_intlk_set_get_ang_bigger_y_exp;
extern disp_op_t orbit_intlk_set_get_ang_bigger_ltc_x_exp;
extern disp_op_t orbit_intlk_set_get_ang_bigger_ltc_y_exp;
extern disp_op_t orbit_intlk_set_get_ang_bigger_any_exp;
extern disp_op_t orbit_intlk_set_get_ang_bigger_exp;
extern disp_op_t orbit_intlk_set_get_ang_bigger_ltc_exp;
extern disp_op_t orbit_intlk_set_get_intlk_bigger_exp;
extern disp_op_t orbit_intlk_set_get_intlk_bigger_ltc_exp;
extern disp_op_t orbit_intlk_set_get_min_sum_exp;
extern disp_op_t orbit_intlk_set_get_trans_max_x_exp;
extern disp_op_t orbit_intlk_set_get_trans_max_y_exp;
extern disp_op_t orbit_intlk_set_get_ang_max_x_exp;
extern disp_op_t orbit_intlk_set_get_ang_max_y_exp;

extern const disp_op_t *orbit_intlk_exp_ops [];

#endif

