/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _SWAP_H_
#define _SWAP_H_

#include "sm_io_bootstrap.h"
#include "smio_thsafe_zmq_client.h"
#include "exp_ops_codes.h"
#include "sm_io_swap_core.h"

/* Known modules IDs (from SDB records defined in FPGA) */
#define SWAP_SDB_DEVID       0x12897592
#define SWAP_SDB_NAME        "SWAP"

extern const smio_bootstrap_ops_t swap_bootstrap_ops;

#endif


