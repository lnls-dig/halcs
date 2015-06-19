/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _FMC130M_4CH_H_
#define _FMC130M_4CH_H_

/* Known modules IDs (from SDB records defined in FPGA) */
#define FMC130M_4CH_SDB_DEVID       0x7085ef15
#define FMC130M_4CH_SDB_NAME        "FMC130M_4CH"

extern const smio_bootstrap_ops_t fmc130m_4ch_bootstrap_ops;

#endif


