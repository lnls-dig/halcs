/*
 * Copyright (C) 2016 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _SM_IO_TRIGGER_IFACE_EXPORTS_H_
#define _SM_IO_TRIGGER_IFACE_EXPORTS_H_

#include "disptable.h"

extern disp_op_t trigger_iface_dir_exp;
extern disp_op_t trigger_iface_rcv_count_rst_exp;
extern disp_op_t trigger_iface_transm_count_rst_exp;
extern disp_op_t trigger_iface_rcv_len_exp;
extern disp_op_t trigger_iface_transm_len_exp;
extern disp_op_t trigger_iface_count_rcv_exp;
extern disp_op_t trigger_iface_count_transm_exp;

extern const disp_op_t *trigger_iface_exp_ops [];

#endif

