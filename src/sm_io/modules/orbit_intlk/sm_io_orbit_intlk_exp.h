/*
 * Copyright (C) 2020 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _ORBIT_INTLK_H_
#define _ORBIT_INTLK_H_

/* Known modules IDs (from SDB records defined in FPGA) */
#define ORBIT_INTLK_SDB_DEVID       0x87efeda8
#define ORBIT_INTLK_SDB_NAME        "ORBIT_INTLK"

extern const smio_bootstrap_ops_t orbit_intlk_bootstrap_ops;

#endif

