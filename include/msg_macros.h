/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _MSG_MACROS_H_
#define _MSG_MACROS_H_

/* SMIO THSAFE ZMQ server function arguments macros */
#define GEN_MSG_ZMQ_TYPE                               zmsg_t *
#define GEN_MSG_ZMQ(args)                              ((zmsg_t *) args)

#define GEN_MSG_ZMQ_POP_NEXT_ARG(zmq_msg)               zmsg_pop (zmq_msg)
#define GEN_MSG_ZMQ_CLENUP_ARG(zmq_frame_p)             zframe_destroy (zmq_frame_p)
#define GEN_MSG_ZMQ_PEEK_NEXT_ARG(zmq_msg)              zmsg_next (zmq_msg)
#define GEN_MSG_ZMQ_PEEK_FIRST(zmq_msg)                 zmsg_first (zmq_msg)
#define GEN_MSG_ZMQ_PEEK_EXIT(zmq_msg)                  GEN_MSG_ZMQ_PEEK_FIRST(zmq_msg)
#define GEN_MSG_ZMQ_PEEK_RESTART(zmq_msg)               GEN_MSG_ZMQ_PEEK_FIRST(zmq_msg)

#define GEN_MSG_ZMQ_ARG_TYPE                            zframe_t *

#define GEN_MSG_ZMQ_ARG_SIZE(zmq_frame)                 zframe_size (zmq_frame)
#define GEN_MSG_ZMQ_ARG_DATA(zmq_frame)                 zframe_data (zmq_frame)

#define GEN_MSG_ZMQ_FIRST_ARG(zmq_msg)                  zframe_data (GEN_MSG_ZMQ_PEEK_FIRST(zmq_msg))
#define GEN_MSG_ZMQ_NEXT_ARG(zmq_msg)                   zframe_data (GEN_MSG_ZMQ_PEEK_NEXT_ARG(zmq_msg))

#endif
