/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _THSAFE_MSG_ZMQ_H_
#define _THSAFE_MSG_ZMQ_H_

#ifdef __cplusplus
extern "C" {
#endif

#define ZMQ_SERVER_ARGS_TAG         0x0004beef

struct _zmq_server_args_t {
    /* This allows us to do a runtime typing and validation. Idea from
     * CZMQ 3.0.0 available at https://github.com/zeromq/czmq/blob/master/src/zmsg.c */
    uint32_t tag;
    devio_proto_t *proto;
    zsock_t *reply_to;
};

/* SMIO THSAFE ZMQ server function arguments macros */
#define __THSAFE_MSG_ZMQ_ARGS_2_MSG(args)               ((zmq_server_args_t *) args)
#define THSAFE_MSG_ZMQ(args)                            (devio_proto_get_content (__THSAFE_MSG_ZMQ_ARGS_2_MSG(args)->proto))

#define THSAFE_MSG_ZMQ_POP_NEXT_ARG(args)               GEN_MSG_ZMQ_POP_NEXT_ARG(THSAFE_MSG_ZMQ(args))
#define THSAFE_MSG_CLENUP_ARG(arg_p)                    GEN_MSG_ZMQ_CLENUP_ARG(arg_p)
#define THSAFE_MSG_ZMQ_PEEK_NEXT_ARG(args)              GEN_MSG_ZMQ_PEEK_NEXT_ARG(THSAFE_MSG_ZMQ(args))
#define THSAFE_MSG_ZMQ_PEEK_FIRST(args)                 GEN_MSG_ZMQ_PEEK_FIRST(THSAFE_MSG_ZMQ(args))
#define THSAFE_MSG_ZMQ_PEEK_EXIT(args)                  GEN_MSG_ZMQ_PEEK_EXIT(THSAFE_MSG_ZMQ(args))
#define THSAFE_MSG_ZMQ_PEEK_RESTART(args)               GEN_MSG_ZMQ_PEEK_RESTART(THSAFE_MSG_ZMQ(args))

#define THSAFE_MSG_ZMQ_ARG_TYPE                         GEN_MSG_ZMQ_ARG_TYPE

#define THSAFE_MSG_ZMQ_ARG_SIZE(arg)                    GEN_MSG_ZMQ_ARG_SIZE(arg)
#define THSAFE_MSG_ZMQ_ARG_DATA(arg)                    GEN_MSG_ZMQ_ARG_DATA(arg)

/* For use in SMIOs exported functions */
#define THSAFE_MSG_ZMQ_FIRST_ARG(args)                  GEN_MSG_ZMQ_FIRST_ARG(THSAFE_MSG_ZMQ(args))
#define THSAFE_MSG_ZMQ_NEXT_ARG(args)                   GEN_MSG_ZMQ_NEXT_ARG(THSAFE_MSG_ZMQ(args))

bool thsafe_msg_zmq_is (void *self);

#ifdef __cplusplus
}
#endif

#endif
