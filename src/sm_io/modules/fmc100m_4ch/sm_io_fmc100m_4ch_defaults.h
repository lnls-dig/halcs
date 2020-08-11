/*
 * Copyright (C) 2020 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _FMC_100M_4CH_DEFAULTS_H_
#define _FMC_100M_4CH_DEFAULTS_H_

#include "sm_io_err.h"
#include "chips/si57x_regs.h"

#define FMC_100M_4CH_DFLT_SI571_OE                   0x1

#define FMC_100M_4CH_DFLT_SI57X_FOUT_FACTORY         100000000.0
#define FMC_100M_4CH_DFLT_SI57X_FOUT                 100000000   /* 100.000000 MHz default */

smio_err_e fmc100m_4ch_config_defaults (char *broker_endp, char *service,
        const char *log_file_name);

#endif

