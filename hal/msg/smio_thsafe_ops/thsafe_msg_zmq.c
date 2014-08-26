/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include "thsafe_msg_zmq.h"

bool thsafe_msg_zmq_is (void *self)
{
    assert (self);
    return ((zmq_server_args_t *) self)->tag == ZMQ_SERVER_ARGS_TAG;
}

