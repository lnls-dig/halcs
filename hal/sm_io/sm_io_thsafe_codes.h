/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _SM_IO_THSAFE_CODES_H_
#define _SM_IO_THSAFE_CODES_H_

#include <inttypes.h>

/* Messaging OPCODES */
#define THSAFE_OPCODE_TYPE                  uint32_t
#define THSAFE_OPCODE_SIZE                  (sizeof (THSAFE_OPCODE_TYPE))
#define THSAFE_READ_16_DSIZE                (sizeof (uint16_t))
#define THSAFE_READ_32_DSIZE                (sizeof (uint32_t))
#define THSAFE_READ_64_DSIZE                (sizeof (uint64_t))
#define THSAFE_WRITE_16_DSIZE               THSAFE_READ_16_DSIZE
#define THSAFE_WRITE_32_DSIZE               THSAFE_READ_32_DSIZE
#define THSAFE_WRITE_64_DSIZE               THSAFE_READ_64_DSIZE

#define THSAFE_OPCODE_OPEN                  0
#define THSAFE_NAME_OPEN                    "open"
#define THSAFE_OPCODE_RELEASE               1
#define THSAFE_NAME_RELEASE                 "release"
#define THSAFE_OPCODE_READ_16               2
#define THSAFE_NAME_READ_16                 "read_16"
#define THSAFE_OPCODE_READ_32               3
#define THSAFE_NAME_READ_32                 "read_32"
#define THSAFE_OPCODE_READ_64               4
#define THSAFE_NAME_READ_64                 "read_64"
#define THSAFE_OPCODE_WRITE_16              5
#define THSAFE_NAME_WRITE_16                "write_16"
#define THSAFE_OPCODE_WRITE_32              6
#define THSAFE_NAME_WRITE_32                "write_32"
#define THSAFE_OPCODE_WRITE_64              7
#define THSAFE_NAME_WRITE_64                "write_64"
#define THSAFE_OPCODE_READ_BLOCK            8
#define THSAFE_NAME_READ_BLOCK              "read_block"
#define THSAFE_OPCODE_WRITE_BLOCK           9
#define THSAFE_NAME_WRITE_BLOCK             "write_block"
#define THSAFE_OPCODE_READ_DMA              10
#define THSAFE_NAME_READ_DMA                "read_dma"
#define THSAFE_OPCODE_WRITE_DMA             11
#define THSAFE_NAME_WRITE_DMA               "write_dma"
//#define THSAFE_OPCODE_READ_INFO           12
#define THSAFE_OPCODE_END                   12
//#define THSAFE_OPCODE_END                 13

/* Messaging Reply OPCODES */
#define THSAFE_REPLY_TYPE                   uint32_t
#define THSAFE_REPLY_SIZE                   (sizeof (THSAFE_REPLY_TYPE))

#define THSAFE_OK                           0
#define THSAFE_ERR                          1
#define THSAFE_REPLY_END                    2

/* Messaging Return Codes */
#define THSAFE_RETURN_TYPE                  int32_t
#define THSAFE_RETURN_SIZE                  (sizeof (THSAFE_RETURN_TYPE))
#define THSAFE_RETURN_UTYPE                 uint32_t
#define THSAFE_RETURN_USIZE                 (sizeof (THSAFE_RETURN_TYPE))

#endif
