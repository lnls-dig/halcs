/*
** Copyright (C) 2020 LNLS (www.lnls.br)
** Author: Melissa Aguiar <melissa.aguiar@cnpem.br>
**
** Released according to the GNU GPL, version 3 or any later version.
**/

#ifndef _SM_IO_FOFB_PROCESSING_EXPORTS_H_
#define _SM_IO_FOFB_PROCESSING_EXPORTS_H_

#include "disptable.h"

extern disp_op_t fofb_processing_set_get_coeffs_fixed_point_pos_exp;
extern disp_op_t fofb_processing_set_get_acc_gains_fixed_point_pos_exp;
extern disp_op_t fofb_processing_set_get_acc_gain_exp;
extern disp_op_t fofb_processing_set_get_acc_ctl_clear_exp;
extern disp_op_t fofb_processing_set_get_acc_ctl_freeze_exp;
extern disp_op_t fofb_processing_coeffs_ram_bank_read_exp;
extern disp_op_t fofb_processing_coeffs_ram_bank_write_exp;
extern disp_op_t fofb_processing_setpoints_ram_bank_read_exp;
extern disp_op_t fofb_processing_setpoints_ram_bank_write_exp;

extern const disp_op_t *fofb_processing_exp_ops [];

#endif
