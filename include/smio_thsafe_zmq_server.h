/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _SMIO_THSAFE_ZMQ_SERVER_H_
#define _SMIO_THSAFE_ZMQ_SERVER_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Somewhat arbitrary maximum block size for read_block funtions */
#define ZMQ_SERVER_BLOCK_SIZE       (1 * (1 << 20))

/* We want a flexible array member but we can't do it*/
typedef struct {
    uint8_t data[1];
} zmq_server_data_block_t;

#define ZMQ_SERVER_DATA_BLOCK_DATA_SIZE 1

/* For use by smio_t general structure */
extern const disp_op_t *smio_thsafe_zmq_server_ops [];

#ifdef __cplusplus
}
#endif

#endif
