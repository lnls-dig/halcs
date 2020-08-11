/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _FMC250M_4CH_H_
#define _FMC250M_4CH_H_

/* Known modules IDs (from SDB records defined in FPGA) */
#define FMC250M_4CH_SDB_DEVID       0x68e3b1af
#define FMC250M_4CH_SDB_NAME        "FMC250M_4CH"

extern const smio_bootstrap_ops_t fmc250m_4ch_bootstrap_ops;

#endif


