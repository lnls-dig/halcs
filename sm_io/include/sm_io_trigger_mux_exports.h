/*
 * Copyright (C) 2016 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _SM_IO_TRIGGER_MUX_EXPORTS_H_
#define _SM_IO_TRIGGER_MUX_EXPORTS_H_

#include "disptable.h"

extern disp_op_t trigger_mux_rcv_src_exp;
extern disp_op_t trigger_mux_rcv_in_sel_exp;
extern disp_op_t trigger_mux_transm_src_exp;
extern disp_op_t trigger_mux_transm_out_sel_exp;

extern const disp_op_t *trigger_mux_exp_ops [];

#endif

