/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _SM_IO_ACQ_CODES_H_
#define _SM_IO_ACQ_CODES_H_

#include <inttypes.h>

/* Messaging OPCODES */
#define ACQ_OPCODE_SIZE                  (sizeof(uint32_t))
#define ACQ_OPCODE_TYPE                  uint32_t

#define ACQ_OPCODE_DATA_ACQUIRE         0
#define ACQ_NAME_DATA_ACQUIRE           "data_acquire"
#define ACQ_OPCODE_GET_DATA_BLOCK       1
#define ACQ_NAME_GET_DATA_BLOCK         "get_data_block"
#define ACQ_OPCODE_CHECK_DATA_ACQUIRE   2
#define ACQ_NAME_CHECK_DATA_ACQUIRE     "check_data_acquire"
#define ACQ_OPCODE_END                  3

/* Messaging Reply OPCODES */
#define ACQ_REPLY_SIZE                  (sizeof(uint32_t))
#define ACQ_REPLY_TYPE                  uint32_t

#define ACQ_OK                          0   /* Operation was successful */
#define ACQ_NUM_SAMPLES_OOR             1   /* Number of samples out of range */
#define ACQ_NOT_COMPLETED               2   /* Acquisition not completed */
#define ACQ_BLOCK_OOR                   3   /* Block number out of range */
#define ACQ_NUM_CHAN_OOR                4   /* Channel number out of range */
#define ACQ_REPLY_END                   5   /* End marker */

#endif
