/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _TIMING_H_
#define _TIMING_H_

#include "sm_io_bootstrap.h"
#include "smio_thsafe_zmq_client.h"
#include "exp_ops_codes.h"
#include "sm_io_afc_timing_core.h"

/* Known modules IDs (from SDB records defined in FPGA) */
#define AFC_TIMING_SDB_DEVID       0xbe10be10
#define AFC_TIMING_SDB_NAME        "LNLS_AFC_TIMING"

extern const smio_bootstrap_ops_t afc_timing_bootstrap_ops;

#endif


