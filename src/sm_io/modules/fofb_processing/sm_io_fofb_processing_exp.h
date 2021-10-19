/*
 * Copyright (C) 2020 LNLS (www.lnls.br)
 * Author: Melissa Aguiar <melissa.aguiar@cnpem.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _FOFB_PROCESSING_H_
#define _FOFB_PROCESSING_H_

/* Known modules IDs (from SDB records defined in FPGA) */
#define FOFB_PROCESSING_SDB_DEVID       0x49681ca6
#define FOFB_PROCESSING_SDB_NAME        "FOFB_PROCESSING"

extern const smio_bootstrap_ops_t fofb_processing_bootstrap_ops;

#endif

