/*
 * Copyright (C) 2021 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _SM_IO_RTMLAMP_OHWR_EXPORTS_H_
#define _SM_IO_RTMLAMP_OHWR_EXPORTS_H_

#include "disptable.h"

extern disp_op_t rtmlamp_ohwr_sta_exp;
extern disp_op_t rtmlamp_ohwr_dac_data_from_wb_exp;
extern disp_op_t rtmlamp_ohwr_amp_iflag_l_exp;
extern disp_op_t rtmlamp_ohwr_amp_tflag_l_exp;
extern disp_op_t rtmlamp_ohwr_amp_iflag_r_exp;
extern disp_op_t rtmlamp_ohwr_amp_tflag_r_exp;
extern disp_op_t rtmlamp_ohwr_amp_en_exp;
extern disp_op_t rtmlamp_ohwr_pi_ol_triang_enable_exp;
extern disp_op_t rtmlamp_ohwr_pi_ol_square_enable_exp;
extern disp_op_t rtmlamp_ohwr_pi_sp_square_enable_exp;
extern disp_op_t rtmlamp_ohwr_pi_enable_exp;
extern disp_op_t rtmlamp_ohwr_pi_kp_exp;
extern disp_op_t rtmlamp_ohwr_pi_ti_exp;
extern disp_op_t rtmlamp_ohwr_pi_sp_exp;
extern disp_op_t rtmlamp_ohwr_dac_data_exp;
extern disp_op_t rtmlamp_ohwr_dac_wr_exp;
extern disp_op_t rtmlamp_ohwr_pi_ol_dac_cnt_max_exp;
extern disp_op_t rtmlamp_ohwr_pi_sp_lim_inf_exp;

extern const disp_op_t *rtmlamp_ohwr_exp_ops [];

#endif
