/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _FMC_ADC_COMMON_DEFAULTS_H_
#define _FMC_ADC_COMMON_DEFAULTS_H_

#include "sm_io_err.h"

#define FMC_ADC_COMMON_DFLT_TRIG_DIR                   0x0                      /* Output direction */

smio_err_e fmc_adc_common_config_defaults (char *broker_endp, char *service,
        const char *log_file_name);

#endif

