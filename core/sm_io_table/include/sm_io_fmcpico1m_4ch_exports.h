/*
 * Copyright (C) 2017 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _SM_IO_FMCPICO1M_4CH_EXPORTS_H_
#define _SM_IO_FMCPICO1M_4CH_EXPORTS_H_

#include "disptable.h"

extern disp_op_t fmcpico1m_4ch_set_get_led1_exp;
extern disp_op_t fmcpico1m_4ch_set_get_led2_exp;
extern disp_op_t fmcpico1m_4ch_set_get_rng_r0_exp;
extern disp_op_t fmcpico1m_4ch_set_get_rng_r1_exp;
extern disp_op_t fmcpico1m_4ch_set_get_rng_r2_exp;
extern disp_op_t fmcpico1m_4ch_set_get_rng_r3_exp;
extern disp_op_t fmcpico1m_4ch_set_get_adc_data0_exp;
extern disp_op_t fmcpico1m_4ch_set_get_adc_data1_exp;
extern disp_op_t fmcpico1m_4ch_set_get_adc_data2_exp;
extern disp_op_t fmcpico1m_4ch_set_get_adc_data3_exp;

extern const disp_op_t *fmcpico1m_4ch_exp_ops [];

#endif

