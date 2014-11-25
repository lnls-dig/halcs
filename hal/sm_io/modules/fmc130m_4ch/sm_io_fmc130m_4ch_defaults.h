/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _FMC130M_4CH_DEFAULTS_H_
#define _FMC130M_4CH_DEFAULTS_H_

#include "sm_io_err.h"
#include "sm_ch_si57x.h"
#include "sm_ch_pca9547.h"

#define FMC130M_4CH_DFLT_PLL_FUNC                   0x1
#define FMC130M_4CH_DFLT_PCA9547_CFG                SMCH_PCA9547_NO_CHANNEL         /* No channel selected */
#define FMC130M_4CH_DFLT_SI571_OE                   0x1

#define FMC130M_4CH_DFLT_SI57X_FOUT_FACTORY         SMCH_SI57X_FOUT_FACTORY_DFLT

smio_err_e fmc130m_4ch_config_defaults (char *broker_endp, char *service,
        const char *log_file_name);

#endif

