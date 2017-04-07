/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _RFFE_H_
#define _RFFE_H_

#include "sm_io_bootstrap.h"
#include "smio_thsafe_zmq_client.h"
#include "exp_ops_codes.h"
#include "sm_io_rffe_core.h"

/* Known modules IDs */
#define RFFE_DEVID       0x7af21909
#define RFFE_NAME        "RFFE"

extern const smio_bootstrap_ops_t rffe_bootstrap_ops;

#endif

