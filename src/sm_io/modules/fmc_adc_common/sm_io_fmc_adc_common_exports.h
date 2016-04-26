/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _SM_IO_FMC_ADC_COMMON_EXPORTS_H_
#define _SM_IO_FMC_ADC_COMMON_EXPORTS_H_

#include "disptable.h"

extern disp_op_t fmc_adc_common_leds_exp;
extern disp_op_t fmc_adc_common_test_data_en_exp;
extern disp_op_t fmc_adc_common_trig_dir_exp;
extern disp_op_t fmc_adc_common_trig_term_exp;
extern disp_op_t fmc_adc_common_trig_val_exp;

extern const disp_op_t *fmc_adc_common_exp_ops [];

#endif

