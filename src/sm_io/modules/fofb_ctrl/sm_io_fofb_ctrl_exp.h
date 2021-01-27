/*
 * Copyright (C) 2021 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _FOFB_CTRL_H_
#define _FOFB_CTRL_H_

/* Known modules IDs (from SDB records defined in FPGA) */
#define FOFB_CTRL_SDB_DEVID       0x4a1df147
#define FOFB_CTRL_SDB_NAME        "FOFB_CTRL"

extern const smio_bootstrap_ops_t fofb_ctrl_bootstrap_ops;

#endif

