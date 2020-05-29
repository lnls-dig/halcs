/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _STR_MSG_MACROS_H_
#define _STR_MSG_MACROS_H_

#define STR_MSG_ZMQ_TYPE                               char

#define STR_MSG_ZMQ_POP_NEXT_ARG(zmq_msg)               zmsg_popstr (zmq_msg)
#define STR_MSG_ZMQ_CLENUP_ARG(arg)                     free (arg)

#define STR_MSG_ZMQ_FIRST_ARG(zmq_msg)                  STR_MSG_ZMQ_POP_NEXT_ARG(zmq_msg)
#define STR_MSG_ZMQ_NEXT_ARG(zmq_msg)                   STR_MSG_ZMQ_POP_NEXT_ARG(zmq_msg)

#endif
