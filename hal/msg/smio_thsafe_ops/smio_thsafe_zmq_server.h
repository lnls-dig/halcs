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
#include "thsafe_msg_zmq.h"

/* Somewhat arbitrary maximum block size for read_block funtions */
#define ZMQ_SERVER_BLOCK_SIZE       131072

struct _zmq_server_data_block_t {
    uint8_t data[ZMQ_SERVER_BLOCK_SIZE];
};

typedef struct _zmq_server_data_block_t zmq_server_data_block_t;

/* For use by smio_t general structure */
extern const disp_op_t *smio_thsafe_zmq_server_ops [];

#endif
