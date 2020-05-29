/*
 * Copyright (C) 2017 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _AFC_MGMT_H_
#define _AFC_MGMT_H_

/* Known modules IDs (from SDB records defined in FPGA) */
#define AFC_MGMT_SDB_DEVID       0xe7146ebe
#define AFC_MGMT_SDB_NAME        "AFC_MGMT"

extern const smio_bootstrap_ops_t afc_mgmt_bootstrap_ops;

#endif


