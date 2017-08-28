/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _TIM_RCV_H_
#define _TIM_RCV_H_

#include "sm_io_bootstrap.h"
#include "smio_thsafe_zmq_client.h"
#include "exp_ops_codes.h"
#include "sm_io_tim_rcv_core.h"

/* Known modules IDs (from SDB records defined in FPGA) */
#define TIM_RCV_SDB_DEVID       0x3fc68642
#define TIM_RCV_SDB_NAME        "TIM_RCV"

extern const smio_bootstrap_ops_t tim_rcv_bootstrap_ops;

#endif


