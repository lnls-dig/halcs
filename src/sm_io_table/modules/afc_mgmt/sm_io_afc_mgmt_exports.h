/*
 * Copyright (C) 2017 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _SM_IO_AFC_MGMT_EXPORTS_H_
#define _SM_IO_AFC_MGMT_EXPORTS_H_

#include "disptable.h"

extern disp_op_t afc_mgmt_si57x_oe_exp;
extern disp_op_t afc_mgmt_si57x_freq_exp;
extern disp_op_t afc_mgmt_si57x_get_defaults_exp;
extern disp_op_t afc_mgmt_si57x_fstartup_exp;

extern const disp_op_t *afc_mgmt_exp_ops [];

#endif

