/*
 * Copyright (C) 2017 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _FMCPICO1M_4CH_DEFAULTS_H_
#define _FMCPICO1M_4CH_DEFAULTS_H_

/********************** RNG Default Values ***************************/
#define FMCPICO1M_4CH_DFLT_RNG               1

smio_err_e fmcpico1m_4ch_config_defaults (char *broker_endp, char *service,
        const char *log_file_name);

#endif

