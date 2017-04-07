/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _FMC_ACTIVE_CLK_DEFAULTS_H_
#define _FMC_ACTIVE_CLK_DEFAULTS_H_

#include "sm_io_err.h"
#include "chips/si57x_regs.h"

#define FMC_ACTIVE_CLK_DFLT_PLL_FUNC                   0x1
#define FMC_ACTIVE_CLK_DFLT_CLK_SEL                    0x0                         /* Clock from FMC front panel */
#define FMC_ACTIVE_CLK_DFLT_SI571_OE                   0x1

#define FMC_ACTIVE_CLK_DFLT_SI57X_FOUT_FACTORY         SI57X_FOUT_FACTORY_DFLT
#define FMC_ACTIVE_CLK_DFLT_SI57X_FOUT                 113040445   /* 113.040445 MHz default */
#define FMC_ACTIVE_CLK_DFLT_RST_ISLA216P               0x1

smio_err_e fmc_active_clk_config_defaults (char *broker_endp, char *service,
        const char *log_file_name);

#endif

