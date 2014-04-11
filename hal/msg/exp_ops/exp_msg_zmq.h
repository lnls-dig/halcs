/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _EXP_MSG_ZMQ_H_
#define _EXP_MSG_ZMQ_H_

#include <czmq.h>

struct _exp_msg_zmq_t {
    zmsg_t **msg;
    zframe_t *reply_to;
};

typedef struct _exp_msg_zmq_t exp_msg_zmq_t;

#endif
