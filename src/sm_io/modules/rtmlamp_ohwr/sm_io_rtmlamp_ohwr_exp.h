/*
 * Copyright (C) 2021 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _RTMLAMP_OHWR_H_
#define _RTMLAMP_OHWR_H_

/* Known modules IDs (from SDB records defined in FPGA) */
#define RTMLAMP_OHWR_SDB_DEVID       0xa1248bec
#define RTMLAMP_OHWR_SDB_NAME        "RTMLAMP_OHWR"

extern const smio_bootstrap_ops_t rtmlamp_ohwr_bootstrap_ops;

#endif

