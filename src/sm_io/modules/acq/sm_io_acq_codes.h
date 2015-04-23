/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _SM_IO_ACQ_CODES_H_
#define _SM_IO_ACQ_CODES_H_

#include <inttypes.h>
#include <acq_chan_gen_defs.h>

struct _smio_acq_data_block_t {
    uint32_t valid_bytes;           /* how much of the BLOCK_SIZE bytes are valid */
    uint8_t data[BLOCK_SIZE];       /* data buffer */
};

typedef struct _smio_acq_data_block_t smio_acq_data_block_t;

/* Messaging OPCODES */
#define ACQ_OPCODE_TYPE                  uint32_t
#define ACQ_OPCODE_SIZE                  (sizeof (ACQ_OPCODE_TYPE))

#define ACQ_OPCODE_DATA_ACQUIRE         0
#define ACQ_NAME_DATA_ACQUIRE           "acq_data_acquire"
#define ACQ_OPCODE_GET_DATA_BLOCK       1
#define ACQ_NAME_GET_DATA_BLOCK         "acq_get_data_block"
#define ACQ_OPCODE_CHECK_DATA_ACQUIRE   2
#define ACQ_NAME_CHECK_DATA_ACQUIRE     "acq_check_data_acquire"
#define ACQ_OPCODE_END                  3

/* Messaging Reply OPCODES */
#define ACQ_REPLY_TYPE                  uint32_t
#define ACQ_REPLY_SIZE                  (sizeof (ACQ_REPLY_TYPE))

#define ACQ_OK                          0   /* Operation was successful */
#define ACQ_NUM_SAMPLES_OOR             1   /* Number of samples out of range */
#define ACQ_NOT_COMPLETED               2   /* Acquisition not completed */
#define ACQ_BLOCK_OOR                   3   /* Block number out of range */
#define ACQ_NUM_CHAN_OOR                4   /* Channel number out of range */
#define ACQ_COULD_NOT_READ              5   /* Could not read memory block */
#define ACQ_REPLY_END                   6   /* End marker */

#endif
