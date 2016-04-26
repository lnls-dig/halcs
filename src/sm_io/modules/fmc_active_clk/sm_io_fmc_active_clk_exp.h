/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _FMC_ACTIVE_CLK_H_
#define _FMC_ACTIVE_CLK_H_

/* Known modules IDs (from SDB records defined in FPGA) */
#define FMC_ACTIVE_CLK_SDB_DEVID       0x88c67d9c
#define FMC_ACTIVE_CLK_SDB_NAME        "FMC_ACTIVE_CLK"

extern const smio_bootstrap_ops_t fmc_active_clk_bootstrap_ops;

#endif


