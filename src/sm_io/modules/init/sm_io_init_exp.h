/*
 * Copyright (C) 2017 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _INIT_H_
#define _INIT_H_

/* Known modules IDs (from SDB records defined in FPGA) */
#define INIT_SDB_DEVID       0xdc64e778 
#define INIT_SDB_NAME        "INIT"

extern const smio_bootstrap_ops_t init_bootstrap_ops;

#endif

