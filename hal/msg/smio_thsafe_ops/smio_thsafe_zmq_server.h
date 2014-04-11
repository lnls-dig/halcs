/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _SMIO_THSAFE_ZMQ_H_
#define _SMIO_THSAFE_ZMQ_H_

#include "dev_io_core.h"
#include "sm_io_thsafe_codes.h"

/* For use by smio_t general structure */
extern const smio_thsafe_server_ops_t smio_thsafe_zmq_server_ops;

struct _zmq_server_args_t {
    zmsg_t **msg;
    void *reply_to;
};

typedef struct _zmq_server_args_t zmq_server_args_t;

#endif
