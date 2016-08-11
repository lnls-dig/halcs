/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _SM_IO_SWAP_EXPORTS_H_
#define _SM_IO_SWAP_EXPORTS_H_

#include "disptable.h"

extern disp_op_t swap_set_get_sw_exp;
extern disp_op_t swap_set_get_sw_en_exp;
extern disp_op_t swap_set_get_div_clk_exp;
extern disp_op_t swap_set_get_sw_dly_exp;
extern disp_op_t swap_set_get_wdw_en_exp;
extern disp_op_t swap_set_get_wdw_dly_exp;
extern disp_op_t swap_set_get_gain_a_exp;
extern disp_op_t swap_set_get_gain_b_exp;
extern disp_op_t swap_set_get_gain_c_exp;
extern disp_op_t swap_set_get_gain_d_exp;

extern const disp_op_t *swap_exp_ops [];

#endif

