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

/* Somewhat arbitrary maximum block size for read_block funtions */
#define ZMQ_SERVER_BLOCK_SIZE       131072

/* For use by smio_t general structure */
extern const disp_op_t *smio_thsafe_zmq_server_ops [];

#define ZMQ_SERVER_ARGS_TAG         0x0004beef

struct _zmq_server_args_t {
    /* This allows us to do a runtime typing and validation. Idea from
     * CZMQ 3.0.0 available at https://github.com/zeromq/czmq/blob/master/src/zmsg.c */
    uint32_t tag;
    zmsg_t **msg;
    void *reply_to;
};

typedef struct _zmq_server_args_t zmq_server_args_t;

struct _zmq_server_data_block_t {
    uint8_t data[ZMQ_SERVER_BLOCK_SIZE];
};

typedef struct _zmq_server_data_block_t zmq_server_data_block_t;

/* FIXME: These are the same as in exp_msg_zmq.h. We repeat them here, because
 * we can have a differente message for ZMQ_SERVER_ARGS. The same message structure
 * might change over time */
/* ZMQ exported function arguments macros */
#define __THSAFE_MSG_ZMQ_ARGS_2_MSG(args)               ((zmq_server_args_t *) args)
#define THSAFE_MSG_ZMQ(args)                            (*__THSAFE_MSG_ZMQ_ARGS_2_MSG(args)->msg)
#define THSAFE_MSG_ZMQ_POP_NEXT_ARG(args)               zmsg_pop (THSAFE_MSG_ZMQ(args))
#define THSAFE_MSG_CLENUP_ARG(arg)                      zframe_destroy (&arg)
#define THSAFE_MSG_ZMQ_PEEK_NEXT_ARG(args)              zmsg_next (THSAFE_MSG_ZMQ(args))
#define THSAFE_MSG_ZMQ_PEEK_FIRST(args)                 zmsg_first (THSAFE_MSG_ZMQ(args))
#define THSAFE_MSG_ZMQ_PEEK_EXIT(args)                  THSAFE_MSG_ZMQ_PEEK_FIRST(args)
#define THSAFE_MSG_ZMQ_PEEK_RESTART(args)               THSAFE_MSG_ZMQ_PEEK_FIRST(args)

#define THSAFE_MSG_ZMQ_ARG_TYPE                         zframe_t *

#define THSAFE_MSG_ZMQ_ARG_SIZE(frame)                  zframe_size (frame)
#define THSAFE_MSG_ZMQ_ARG_DATA(frame)                  zframe_data (frame)

/* For use in SMIOs exported functions */
#define THSAFE_MSG_ZMQ_FIRST_ARG(args)                  zframe_data (THSAFE_MSG_ZMQ_PEEK_FIRST(args))
#define THSAFE_MSG_ZMQ_NEXT_ARG(args)                   zframe_data (THSAFE_MSG_ZMQ_PEEK_NEXT_ARG(args))

#endif
