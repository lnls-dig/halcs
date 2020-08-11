/*
 * Copyright (C) 2017 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _SM_IO_TIM_RCV_CODES_H_
#define _SM_IO_TIM_RCV_CODES_H_

#include <inttypes.h>

/* Messaging OPCODES */
#define TIM_RCV_OPCODE_TYPE                          uint32_t
#define TIM_RCV_OPCODE_SIZE                          (sizeof (TIM_RCV_OPCODE_TYPE))

#define TIM_RCV_OPCODE_PHASE_MEAS_NAVG               0
#define TIM_RCV_NAME_PHASE_MEAS_NAVG                 "tim_rcv_phase_meas_navg"
#define TIM_RCV_OPCODE_DMTD_A_DEGLITCHER_THRES       1
#define TIM_RCV_NAME_DMTD_A_DEGLITCHER_THRES         "tim_rcv_dmtd_a_deglitcher_thres"
#define TIM_RCV_OPCODE_DMTD_B_DEGLITCHER_THRES       2
#define TIM_RCV_NAME_DMTD_B_DEGLITCHER_THRES         "tim_rcv_dmtd_b_deglitcher_thres"
#define TIM_RCV_OPCODE_PHASE_MEAS                    3
#define TIM_RCV_NAME_PHASE_MEAS                      "tim_rcv_phase_meas"
#define TIM_RCV_OPCODE_DMTD_A_FREQ                   4
#define TIM_RCV_NAME_DMTD_A_FREQ                     "tim_rcv_dmtd_a_freq"
#define TIM_RCV_OPCODE_DMTD_A_VALID                  5
#define TIM_RCV_NAME_DMTD_A_VALID                    "tim_rcv_dmtd_a_valid"
#define TIM_RCV_OPCODE_DMTD_B_FREQ                   6
#define TIM_RCV_NAME_DMTD_B_FREQ                     "tim_rcv_dmtd_b_freq"
#define TIM_RCV_OPCODE_DMTD_B_VALID                  7
#define TIM_RCV_NAME_DMTD_B_VALID                    "tim_rcv_dmtd_b_valid"
#define TIM_RCV_OPCODE_END                           8

/* Messaging Reply OPCODES */
#define AFC_MGMT_REPLY_TYPE                          uint32_t
#define AFC_MGMT_REPLY_SIZE                          (sizeof (AFC_MGMT_REPLY_TYPE))

#define AFC_MGMT_OK                                  0   /* Operation was successful */
#define AFC_MGMT_ERR                                 1   /* Could not set/get value */
#define AFC_MGMT_UNINPL                              2   /* Unimplemented function or operation */
#define AFC_MGMT_REPLY_END                           3   /* End marker */

#endif

