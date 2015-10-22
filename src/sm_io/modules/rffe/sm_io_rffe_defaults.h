/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _RFFE_DEFAULTS_H_
#define _RFFE_DEFAULTS_H_

#define RFFE_DFLT_SW                                0x1             /* No switching, direct position */
#define RFFE_DFLT_ATT1                              31.5            /* 31.1 dB attenuation */
#define RFFE_DFLT_ATT2                              31.5            /* 31.1 dB attenuation */

smio_err_e rffe_config_defaults (char *broker_endp, char *service,
        const char *log_file_name);

#endif

