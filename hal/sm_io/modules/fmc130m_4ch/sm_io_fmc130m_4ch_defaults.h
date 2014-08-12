/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _FMC130M_4CH_DEFAULTS_H_
#define _FMC130M_4CH_DEFAULTS_H_

#include "sm_io_err.h"

#define FMC130M_4CH_DFLT_PLL_FUNC           0x1

smio_err_e fmc130m_4ch_config_defaults (char *broker_endp, char *service,
        const char *log_file_name);

#endif

