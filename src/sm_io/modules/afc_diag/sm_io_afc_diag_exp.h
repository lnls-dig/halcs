/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _AFC_DIAG_H_
#define _AFC_DIAG_H_

#include "sm_io_bootstrap.h"
#include "smio_thsafe_zmq_client.h"
#include "exp_ops_codes.h"
#include "sm_io_afc_diag_core.h"

/* Known modules IDs */
#define AFC_DIAG_DEVID       0x51954750
#define AFC_DIAG_NAME        "AFC_DIAG"

extern const smio_bootstrap_ops_t afc_diag_bootstrap_ops;

#endif

