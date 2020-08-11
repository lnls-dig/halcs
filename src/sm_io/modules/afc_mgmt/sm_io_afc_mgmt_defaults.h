/*
 * Copyright (C) 2017 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _AFC_MGMT_DEFAULTS_H_
#define _AFC_MGMT_DEFAULTS_H_

#include "sm_io_err.h"
#include "chips/si57x_regs.h"

#define AFC_MGMT_DFLT_SI571_OE                   0x1

#define AFC_MGMT_DFLT_SI57X_FOUT_FACTORY         SI57X_FOUT_FACTORY_DFLT
#define AFC_MGMT_DFLT_SI57X_FOUT                 100000000   /* 100.000000 MHz default */

smio_err_e afc_mgmt_config_defaults (char *broker_endp, char *service,
        const char *log_file_name);

#endif

