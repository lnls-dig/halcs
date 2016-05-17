/*
 * Copyright (C) 2016 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _TRIGGER_MUX_DEFAULTS_H_
#define _TRIGGER_MUX_DEFAULTS_H_

#include "sm_io_err.h"

#define TRIGGER_MUX_DFLT_DIR                      1      /* Input */
#define TRIGGER_MUX_DFLT_RCV_RST                  1      /* Pulse Reset */
#define TRIGGER_MUX_DFLT_TRANSM_RST               1      /* Pulse Reset */
#define TRIGGER_MUX_DFLT_RCV_LEN                  1      /* Debounce Length */
#define TRIGGER_MUX_DFLT_TRANSM_LEN               1      /* Pulse Extension Length */

smio_err_e trigger_mux_config_defaults (char *broker_endp, char *service,
        const char *log_file_name);

#endif

