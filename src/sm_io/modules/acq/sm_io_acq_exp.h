/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _ACQ_H_
#define _ACQ_H_

#include "sm_io_bootstrap.h"
#include "smio_thsafe_zmq_client.h"
#include "exp_ops_codes.h"
#include "sm_io_acq_core.h"

/* Known modules IDs (from SDB records defined in FPGA) */
#define ACQ_SDB_DEVID       0x4519a0ad
#define ACQ_SDB_NAME        "ACQ"

extern const smio_bootstrap_ops_t acq_bootstrap_ops;

#endif


