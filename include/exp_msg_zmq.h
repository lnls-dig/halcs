/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _EXP_MSG_ZMQ_H_
#define _EXP_MSG_ZMQ_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Same TAG as defined in https://github.com/zeromq/czmq/blob/master/src/zmsg.c */
#define EXP_MSG_ZMQ_TAG                             0x0003cafe

struct _exp_msg_zmq_t {
    /* This allows us to do a runtime typing and validation. Idea from
     * CZMQ 3.0.0 available at https://github.com/zeromq/czmq/blob/master/src/zmsg.c */
    uint32_t tag;
    zmsg_t **msg;
    zframe_t *reply_to;
};

/* SMIO THSAFE ZMQ server function arguments macros */
#define __EXP_MSG_ZMQ_ARGS_2_MSG(args)                 ((exp_msg_zmq_t *) args)
#define EXP_MSG_ZMQ(args)                              (*__EXP_MSG_ZMQ_ARGS_2_MSG(args)->msg)

#define EXP_MSG_ZMQ_POP_NEXT_ARG(args)                  GEN_MSG_ZMQ_POP_NEXT_ARG(EXP_MSG_ZMQ(args))
#define EXP_MSG_CLENUP_ARG(arg_p)                       GEN_MSG_ZMQ_CLENUP_ARG(arg_p)
#define EXP_MSG_ZMQ_PEEK_NEXT_ARG(args)                 GEN_MSG_ZMQ_PEEK_NEXT_ARG(EXP_MSG_ZMQ(args))
#define EXP_MSG_ZMQ_PEEK_FIRST(args)                    GEN_MSG_ZMQ_PEEK_FIRST(EXP_MSG_ZMQ(args))
#define EXP_MSG_ZMQ_PEEK_EXIT(args)                     GEN_MSG_ZMQ_PEEK_EXIT(EXP_MSG_ZMQ(args))
#define EXP_MSG_ZMQ_PEEK_RESTART(args)                  GEN_MSG_ZMQ_PEEK_RESTART(EXP_MSG_ZMQ(args))

#define EXP_MSG_ZMQ_ARG_TYPE                            GEN_MSG_ZMQ_ARG_TYPE

#define EXP_MSG_ZMQ_ARG_SIZE(arg)                       GEN_MSG_ZMQ_ARG_SIZE(arg)
#define EXP_MSG_ZMQ_ARG_DATA(arg)                       GEN_MSG_ZMQ_ARG_DATA(arg)

/* For use in SMIOs exported functions */
#define EXP_MSG_ZMQ_FIRST_ARG(args)                     GEN_MSG_ZMQ_FIRST_ARG(EXP_MSG_ZMQ(args))
#define EXP_MSG_ZMQ_NEXT_ARG(args)                      GEN_MSG_ZMQ_NEXT_ARG(EXP_MSG_ZMQ(args))

/* Try to guess if the message is of exp_msg_zmq type */
bool exp_msg_zmq_is (void *self);

#ifdef __cplusplus
}
#endif

#endif
