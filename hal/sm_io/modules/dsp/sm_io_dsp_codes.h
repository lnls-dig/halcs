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
#endif

#endif
