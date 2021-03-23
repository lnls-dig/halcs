/*
 * Copyright (C) 2021 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _RTMLAMP_OHWR_DEFAULTS_H_
#define _RTMLAMP_OHWR_DEFAULTS_H_

#include "sm_io_err.h"

smio_err_e rtmlamp_ohwr_config_defaults (char *broker_endp, char *service,
        const char *log_file_name);

#endif
