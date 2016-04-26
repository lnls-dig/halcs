/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _FMC_ADC_COMMON_H_
#define _FMC_ADC_COMMON_H_

/* Known modules IDs (from SDB records defined in FPGA) */
#define FMC_ADC_COMMON_SDB_DEVID       0x2403f569
#define FMC_ADC_COMMON_SDB_NAME        "FMC_ADC_COMMON"

extern const smio_bootstrap_ops_t fmc_adc_common_bootstrap_ops;

#endif


