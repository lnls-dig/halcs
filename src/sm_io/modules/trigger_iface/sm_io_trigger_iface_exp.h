/*
 * Copyright (C) 2016 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _TRIGGER_IFACE_H_
#define _TRIGGER_IFACE_H_

/* Known modules IDs (from SDB records defined in FPGA) */
#define TRIGGER_IFACE_SDB_DEVID       0xbcbb78d2
#define TRIGGER_IFACE_SDB_NAME        "TRIGGER_IFACE"

extern const smio_bootstrap_ops_t trigger_iface_bootstrap_ops;

#endif

