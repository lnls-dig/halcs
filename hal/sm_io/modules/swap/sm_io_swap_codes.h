/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _SM_IO_SWAP_CODES_H_
#define _SM_IO_SWAP_CODES_H_

#include <inttypes.h>

/* Messaging OPCODES */
#define SWAP_OPCODE_SIZE                    (sizeof(uint32_t))
#define SWAP_OPCODE_TYPE                    uint32_t

#define SWAP_OPCODE_SET_GET_SW		        0
#define SWAP_NAME_SET_GET_SW				"set_get_sw"
#define SWAP_OPCODE_SET_GET_SW_EN	        1
#define SWAP_NAME_SET_GET_SW_EN		    	"set_get_sw_en"
#define SWAP_OPCODE_SET_GET_DIV_CLK	        2
#define SWAP_NAME_SET_GET_DIV_CLK			"set_get_div_clk"
#define SWAP_OPCODE_SET_GET_SW_DLY  		4
#define SWAP_NAME_SET_GET_SW_DLY			"set_get_sw_dly"
#define SWAP_OPCODE_SET_GET_WDW		    	5
#define SWAP_NAME_SET_GET_WDW	    		"set_get_wdw"
#define SWAP_OPCODE_SET_GET_WDW_DLY			6
#define SWAP_NAME_SET_GET_WDW_DLY			"set_get_wdw_dly"
#define SWAP_OPCODE_SET_GET_GAIN_A			7
#define SWAP_NAME_SET_GET_GAIN_A			"set_get_gain_a"
#define SWAP_OPCODE_SET_GET_GAIN_B			8
#define SWAP_NAME_SET_GET_GAIN_B			"set_get_gain_b"
#define SWAP_OPCODE_SET_GET_GAIN_C			9
#define SWAP_NAME_SET_GET_GAIN_C			"set_get_gain_c"
#define SWAP_OPCODE_SET_GET_GAIN_D			10
#define SWAP_NAME_SET_GET_GAIN_D			"set_get_gain_d"
#define SWAP_OPCODE_END                  	11

#endif

