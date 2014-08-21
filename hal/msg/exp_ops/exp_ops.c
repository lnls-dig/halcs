/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include "exp_ops.h"
#include "exp_msg_zmq.h"

exp_msg_type_e exp_msg_guess_type (void *msg)
{
    exp_msg_type_e exp_msg_type = EXP_MSG_NOT_SUPPORTED;

    if (exp_msg_zmq_is (msg)) {
        exp_msg_type = EXP_MSG_ZMQ;
    }

    return exp_msg_type;
}

