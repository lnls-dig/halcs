/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _FMC250M_4CH_DEFAULTS_H_
#define _FMC250M_4CH_DEFAULTS_H_

#include "sm_io_err.h"
#include "sm_ch_pca9547.h"
#include "chips/isla216p_regs.h"

#define FMC250M_4CH_DFLT_RST_ADCS                   0x1
#define FMC250M_4CH_DFLT_RST_DIV_ADCS               0x1
#define FMC250M_4CH_DFLT_SLEEP_ADCS                 0x0

#define FMC250M_4CH_DFLT_RST_MODE_ADC               (ISLA216P_NAPSLP_NORMAL_OPERATION)
#define FMC250M_4CH_PINCTRL_RST_MODE_ADC            (ISLA216P_NAPSLP_PIN_CONTROL)
#define FMC250M_4CH_DFLT_PORTCONFIG_ADC             (ISLA216P_PORTCONFIG_SDO_ACTIVE)
#define FMC250M_4CH_DFLT_RESET_ADC                  (ISLA216P_PORTCONFIG_SDO_ACTIVE | ISLA216P_PORTCONFIG_SOFT_RESET)

smio_err_e fmc250m_4ch_config_defaults (char *broker_endp, char *service,
        const char *log_file_name);

#endif

