/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _SM_IO_RFFE_CODES_H_
#define _SM_IO_RFFE_CODES_H_

#include <inttypes.h>

/* This must match the RFFE BSMP server */
#define RFFE_BLOCK_SIZE                         127

struct _smio_rffe_data_block_t {
    uint8_t data[RFFE_BLOCK_SIZE];              /* data buffer */
};

#define RFFE_VERSION_SIZE                       8

struct _smio_rffe_version_t {
    char data[RFFE_VERSION_SIZE];               /* data buffer */
};

/* Messaging OPCODES */
#define RFFE_OPCODE_TYPE                        uint32_t
#define RFFE_OPCODE_SIZE                        (sizeof (RFFE_OPCODE_TYPE))

#define RFFE_OPCODE_SET_GET_ATT                 0
#define RFFE_NAME_SET_GET_ATT                   "rffe_att"
#define RFFE_OPCODE_SET_GET_TEMP_AC             1
#define RFFE_NAME_SET_GET_TEMP_AC               "rffe_temp_ac"
#define RFFE_OPCODE_SET_GET_TEMP_BD             2
#define RFFE_NAME_SET_GET_TEMP_BD               "rffe_temp_bd"
#define RFFE_OPCODE_SET_GET_SET_POINT_AC        3
#define RFFE_NAME_SET_GET_SET_POINT_AC          "rffe_set_point_ac"
#define RFFE_OPCODE_SET_GET_SET_POINT_BD        4
#define RFFE_NAME_SET_GET_SET_POINT_BD          "rffe_set_point_bd"
#define RFFE_OPCODE_SET_GET_TEMP_CONTROL        5
#define RFFE_NAME_SET_GET_TEMP_CONTROL          "rffe_temp_control"
#define RFFE_OPCODE_SET_GET_HEATER_AC           6
#define RFFE_NAME_SET_GET_HEATER_AC             "rffe_heater_ac"
#define RFFE_OPCODE_SET_GET_HEATER_BD           7
#define RFFE_NAME_SET_GET_HEATER_BD             "rffe_heater_bd"
#define RFFE_OPCODE_SET_GET_RESET               8
#define RFFE_NAME_SET_GET_RESET                 "rffe_reset"
#define RFFE_OPCODE_SET_GET_REPROG              9
#define RFFE_NAME_SET_GET_REPROG                "rffe_reprog"
#define RFFE_OPCODE_SET_GET_DATA                10
#define RFFE_NAME_SET_GET_DATA                  "rffe_data"
#define RFFE_OPCODE_SET_GET_VERSION             11
#define RFFE_NAME_SET_GET_VERSION               "rffe_version"
#define RFFE_OPCODE_SET_GET_PID_AC_KP           12
#define RFFE_NAME_SET_GET_PID_AC_KP             "rffe_pid_ac_kp"
#define RFFE_OPCODE_SET_GET_PID_AC_TI           13
#define RFFE_NAME_SET_GET_PID_AC_TI             "rffe_pid_ac_ti"
#define RFFE_OPCODE_SET_GET_PID_AC_TD           14
#define RFFE_NAME_SET_GET_PID_AC_TD             "rffe_pid_ac_td"
#define RFFE_OPCODE_SET_GET_PID_BD_KP           15
#define RFFE_NAME_SET_GET_PID_BD_KP             "rffe_pid_bd_kp"
#define RFFE_OPCODE_SET_GET_PID_BD_TI           16
#define RFFE_NAME_SET_GET_PID_BD_TI             "rffe_pid_bd_ti"
#define RFFE_OPCODE_SET_GET_PID_BD_TD           17
#define RFFE_NAME_SET_GET_PID_BD_TD             "rffe_pid_bd_td"
#define RFFE_OPCODE_END                         18

/* Messaging Reply OPCODES */
#define RFFE_REPLY_TYPE                         uint32_t
#define RFFE_REPLY_SIZE                         (sizeof (RFFE_REPLY_TYPE))

#define RFFE_OK                                  0   /* Operation was successful */
#define RFFE_ERR                                 1   /* Could not set/get value */
#define RFFE_UNINPL                              2   /* Unimplemented function or operation */
#define RFFE_REPLY_END                           3   /* End marker */

#endif
