/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _SM_IO_SWAP_CODES_H_
#define _SM_IO_SWAP_CODES_H_

#include <inttypes.h>

/* Messaging OPCODES */
#define SWAP_OPCODE_TYPE                    uint32_t
#define SWAP_OPCODE_SIZE                    (sizeof (SWAP_OPCODE_TYPE))

#define SWAP_OPCODE_SET_GET_SW              0
#define SWAP_NAME_SET_GET_SW                "swap_set_get_sw"
#define SWAP_OPCODE_SET_GET_DIV_CLK         1
#define SWAP_NAME_SET_GET_DIV_CLK           "swap_set_get_div_clk"
#define SWAP_OPCODE_SET_GET_SW_DLY          2
#define SWAP_NAME_SET_GET_SW_DLY            "swap_set_get_sw_dly"
#define SWAP_OPCODE_SET_GET_DIV_F_CNT_EN    3
#define SWAP_NAME_SET_GET_DIV_F_CNT_EN      "swap_set_get_div_f_cnt_en"
#define SWAP_OPCODE_END                     4

#endif

