/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _FMC250M_4CH_DEFAULTS_H_
#define _FMC250M_4CH_DEFAULTS_H_

#include "sm_io_err.h"
#include "chips/si57x_regs.h"
#include "sm_ch_pca9547.h"

#define FMC250M_4CH_DFLT_PLL_FUNC                   0x1
#define FMC250M_4CH_DFLT_CLK_SEL                    0x0                         /* Clock from FMC front panel */
#define FMC250M_4CH_DFLT_TRIG_DIR                   0x0                         /* Output direction */
#define FMC250M_4CH_DFLT_PCA9547_CFG                SMCH_PCA9547_NO_CHANNEL     /* No channel selected */
#define FMC250M_4CH_DFLT_SI571_OE                   0x1

#define FMC250M_4CH_DFLT_SI57X_FOUT_FACTORY         SI57X_FOUT_FACTORY_DFLT
#define FMC250M_4CH_DFLT_SI57X_FOUT                 113040445   /* 113.040445 MHz default */

smio_err_e fmc250m_4ch_config_defaults (char *broker_endp, char *service,
        const char *log_file_name);

#endif

