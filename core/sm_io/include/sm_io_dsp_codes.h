/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _SM_IO_DSP_CODES_H_
#define _SM_IO_DSP_CODES_H_

/* Messaging OPCODES */
#define DSP_OPCODE_TYPE                     uint32_t
#define DSP_OPCODE_SIZE                     (sizeof (DSP_OPCODE_TYPE))

#define DSP_OPCODE_SET_GET_KX               0
#define DSP_NAME_SET_GET_KX                 "dsp_set_get_kx"
#define DSP_OPCODE_SET_GET_KY               1
#define DSP_NAME_SET_GET_KY                 "dsp_set_get_ky"
#define DSP_OPCODE_SET_GET_KSUM             2
#define DSP_NAME_SET_GET_KSUM               "dsp_set_get_ksum"
#define DSP_OPCODE_SET_GET_DS_TBT_THRES     3
#define DSP_NAME_SET_GET_DS_TBT_THRES       "dsp_set_get_tbt_thres"
#define DSP_OPCODE_SET_GET_DS_FOFB_THRES    4
#define DSP_NAME_SET_GET_DS_FOFB_THRES      "dsp_set_get_fofb_thres"
#define DSP_OPCODE_SET_GET_DS_MONIT_THRES   5
#define DSP_NAME_SET_GET_DS_MONIT_THRES     "dsp_set_get_monit_thres"
#define DSP_OPCODE_SET_GET_CFG_TEST_DATA    6
#define DSP_NAME_SET_GET_CFG_TEST_DATA      "dsp_set_get_cfg_test_data"
#define DSP_OPCODE_SET_GET_MONIT_AMP_CH0    7
#define DSP_NAME_SET_GET_MONIT_AMP_CH0      "dsp_set_get_monit_amp_ch0"
#define DSP_OPCODE_SET_GET_MONIT_AMP_CH1    8
#define DSP_NAME_SET_GET_MONIT_AMP_CH1      "dsp_set_get_monit_amp_ch1"
#define DSP_OPCODE_SET_GET_MONIT_AMP_CH2    9
#define DSP_NAME_SET_GET_MONIT_AMP_CH2      "dsp_set_get_monit_amp_ch2"
#define DSP_OPCODE_SET_GET_MONIT_AMP_CH3    10
#define DSP_NAME_SET_GET_MONIT_AMP_CH3      "dsp_set_get_monit_amp_ch3"
#define DSP_OPCODE_SET_GET_MONIT_POS_X      11
#define DSP_NAME_SET_GET_MONIT_POS_X        "dsp_set_get_monit_pos_x"
#define DSP_OPCODE_SET_GET_MONIT_POS_Y      12
#define DSP_NAME_SET_GET_MONIT_POS_Y        "dsp_set_get_monit_pos_y"
#define DSP_OPCODE_SET_GET_MONIT_POS_Q      13
#define DSP_NAME_SET_GET_MONIT_POS_Q        "dsp_set_get_monit_pos_q"
#define DSP_OPCODE_SET_GET_MONIT_POS_SUM    14
#define DSP_NAME_SET_GET_MONIT_POS_SUM      "dsp_set_get_monit_pos_sum"
#define DSP_OPCODE_SET_GET_MONIT_UPDT       15
#define DSP_NAME_SET_GET_MONIT_UPDT         "dsp_set_get_monit_updt"
#define DSP_OPCODE_END                      16

#endif
