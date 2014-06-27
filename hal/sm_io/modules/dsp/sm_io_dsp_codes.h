/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _SM_IO_DSP_CODES_H_
#define _SM_IO_DSP_CODES_H_

#include <inttypes.h>

/* Messaging OPCODES */
#define DSP_OPCODE_SIZE                  (sizeof(uint32_t))
#define DSP_OPCODE_TYPE                  uint32_t

#define DSP_OPCODE_SET_GET_KX				0
#define DSP_NAME_SET_GET_KX					"set_get_kx"
#define DSP_OPCODE_SET_GET_KY				1
#define DSP_NAME_SET_GET_KY					"set_get_ky"
#define DSP_OPCODE_SET_GET_KSUM				2
#define DSP_NAME_SET_GET_KSUM				"set_get_ksum"
#define DSP_OPCODE_SET_GET_DS_TBT_THRES		3
#define DSP_NAME_SET_GET_DS_TBT_THRES		"set_get_tbt_thres"
#define DSP_OPCODE_SET_GET_DS_FOFB_THRES	4
#define DSP_NAME_SET_GET_DS_FOFB_THRES		"set_get_fofb_thres"
#define DSP_OPCODE_SET_GET_DS_MONIT_THRES	5
#define DSP_NAME_SET_GET_DS_MONIT_THRES		"set_get_monit_thres"

#define DSP_OPCODE_END                      6

#if 0
#define DSP_OPCODE_SET_GET_DDS_FREQ			3
#define DSP_NAME_SET_GET_DDS_FREQ			"set_get_dds_freq"

#define DSP_OPCODE_END                  	4

#define DSP_OPCODE_SET_GET_SW_ON			3
#define DSP_NAME_SET_GET_SW_ON				"set_get_sw_on"
#define DSP_OPCODE_SET_GET_SW_OFF			4
#define DSP_NAME_SET_GET_SW_OFF				"set_get_sw_off"
#define DSP_OPCODE_SET_GET_SW_CLK_EN_ON		5
#define DSP_NAME_SET_GET_SW_CLK_EN_ON		"set_get_sw_clk_en_on"
#define DSP_OPCODE_SET_GET_SW_CLK_EN_OFF	6
#define DSP_NAME_SET_GET_SW_CLK_EN_OFF		"set_get_sw_clk_en_off"
#define DSP_OPCODE_SET_GET_SW_DIVCLK		7
#define DSP_NAME_SET_GET_SW_DIVCLK			"set_get_sw_divclk"
#define DSP_OPCODE_SET_GET_SW_PHASE			8
#define DSP_NAME_SET_GET_SW_PHASE			"set_get_sw_phase"
#define DSP_OPCODE_SET_GET_WDW_ON			9
#define DSP_NAME_SET_GET_WDW_ON				"set_get_wdw_on"
#define DSP_OPCODE_SET_GET_WDW_OFF			10
#define DSP_NAME_SET_GET_WDW_OFF			"set_get_wdw_off"
#define DSP_OPCODE_SET_GET_WDW_DLY			11
#define DSP_NAME_SET_GET_WDW_DLY			"set_get_wdw_dly"
#define DSP_OPCODE_SET_GET_ADC_CLK			12
#define DSP_NAME_SET_GET_ADC_CLK			"set_get_adc_clk"
#define DSP_OPCODE_END                  	14
#endif

#endif
