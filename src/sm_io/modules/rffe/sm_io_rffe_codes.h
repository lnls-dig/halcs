/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _SM_IO_RFFE_CODES_H_
#define _SM_IO_RFFE_CODES_H_

#include <inttypes.h>

/* This must match the RFFE BSMP server */
#define RFFE_BLOCK_SIZE                         127

typedef struct {
    uint8_t data[RFFE_BLOCK_SIZE];              /* data buffer */
} smio_rffe_data_block_t;

#define RFFE_VERSION_SIZE                       8

typedef struct {
    char data[RFFE_VERSION_SIZE];               /* data buffer */
} smio_rffe_version_t;

/* Messaging OPCODES */
#define RFFE_OPCODE_TYPE                        uint32_t
#define RFFE_OPCODE_SIZE                        (sizeof (RFFE_OPCODE_TYPE))

#define RFFE_OPCODE_SET_GET_SW                  0
#define RFFE_NAME_SET_GET_SW                    "rffe_sw"
#define RFFE_OPCODE_SET_GET_ATT1                1
#define RFFE_NAME_SET_GET_ATT1                  "rffe_att1"
#define RFFE_OPCODE_SET_GET_ATT2                2
#define RFFE_NAME_SET_GET_ATT2                  "rffe_att2"
#define RFFE_OPCODE_SET_GET_TEMP1               3
#define RFFE_NAME_SET_GET_TEMP1                 "rffe_temp1"
#define RFFE_OPCODE_SET_GET_TEMP2               4
#define RFFE_NAME_SET_GET_TEMP2                 "rffe_temp2"
#define RFFE_OPCODE_SET_GET_TEMP3               5
#define RFFE_NAME_SET_GET_TEMP3                 "rffe_temp3"
#define RFFE_OPCODE_SET_GET_TEMP4               6
#define RFFE_NAME_SET_GET_TEMP4                 "rffe_temp4"
#define RFFE_OPCODE_SET_GET_SET_POINT1          7
#define RFFE_NAME_SET_GET_SET_POINT1            "rffe_set_point1"
#define RFFE_OPCODE_SET_GET_SET_POINT2          8
#define RFFE_NAME_SET_GET_SET_POINT2            "rffe_set_point2"
#define RFFE_OPCODE_SET_GET_TEMP_CONTROL        9
#define RFFE_NAME_SET_GET_TEMP_CONTROL          "rffe_temp_control"
#define RFFE_OPCODE_SET_GET_OUTPUT1             10
#define RFFE_NAME_SET_GET_OUTPUT1               "rffe_output1"
#define RFFE_OPCODE_SET_GET_OUTPUT2             11
#define RFFE_NAME_SET_GET_OUTPUT2               "rffe_output2"
#define RFFE_OPCODE_SET_GET_RESET               12
#define RFFE_NAME_SET_GET_RESET                 "rffe_reset"
#define RFFE_OPCODE_SET_GET_REPROG              13
#define RFFE_NAME_SET_GET_REPROG                "rffe_reprog"
#define RFFE_OPCODE_SET_GET_DATA                14
#define RFFE_NAME_SET_GET_DATA                  "rffe_data"
#define RFFE_OPCODE_SET_GET_VERSION             15
#define RFFE_NAME_SET_GET_VERSION               "rffe_version"
#define RFFE_OPCODE_SET_GET_SW_LVL              16
#define RFFE_NAME_SET_GET_SW_LVL                "rffe_sw_lvl"
#define RFFE_OPCODE_END                         17

/* Messaging Reply OPCODES */
#define RFFE_REPLY_TYPE                         uint32_t
#define RFFE_REPLY_SIZE                         (sizeof (RFFE_REPLY_TYPE))

#define RFFE_OK                                  0   /* Operation was successful */
#define RFFE_ERR                                 1   /* Could not set/get value */
#define RFFE_UNINPL                              2   /* Unimplemented function or operation */
#define RFFE_REPLY_END                           3   /* End marker */

#endif
