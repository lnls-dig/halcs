/*
 * Copyright (C) 2017 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _ACQ_PM_H_
#define _ACQ_PM_H_

/* Known modules IDs (from SDB records defined in FPGA) */
#define ACQ_PM_SDB_DEVID       0x7f9e3377
#define ACQ_PM_SDB_NAME        "ACQ_PM"

extern const smio_bootstrap_ops_t acq_pm_bootstrap_ops;

#endif

