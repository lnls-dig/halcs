/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "bpm_server.h"

bool exp_msg_zmq_is (void *self)
{
    assert (self);
    return ((exp_msg_zmq_t *) self)->tag == EXP_MSG_ZMQ_TAG;
}

