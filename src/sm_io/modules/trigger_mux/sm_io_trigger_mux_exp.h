/*
 * Copyright (C) 2016 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _TRIGGER_MUX_H_
#define _TRIGGER_MUX_H_

/* Known modules IDs (from SDB records defined in FPGA) */
#define TRIGGER_MUX_SDB_DEVID       0x84b6a5ac
#define TRIGGER_MUX_SDB_NAME        "TRIGGER_MUX"

extern const smio_bootstrap_ops_t trigger_mux_bootstrap_ops;

#endif

