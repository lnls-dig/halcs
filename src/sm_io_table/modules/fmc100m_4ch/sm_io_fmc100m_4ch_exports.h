/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _SM_IO_FMC_100M_4CH_EXPORTS_H_
#define _SM_IO_FMC_100M_4CH_EXPORTS_H_

#include "disptable.h"

extern disp_op_t fmc100m_4ch_si571_oe_exp;
extern disp_op_t fmc100m_4ch_si571_freq_exp;
extern disp_op_t fmc100m_4ch_si571_get_defaults_exp;
extern disp_op_t fmc100m_4ch_si571_fstartup_exp;

extern const disp_op_t *fmc100m_4ch_exp_ops [];

#endif

