/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Joao Brito <joao.brito@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _SM_IO_AFC_TIMING_CODES_H_
#define _SM_IO_AFC_TIMING_CODES_H_

#include <inttypes.h>

/* Messaging OPCODES */
#define AFC_TIMING_OPCODE_TYPE                    uint32_t
#define AFC_TIMING_OPCODE_SIZE                    (sizeof (AFC_TIMING_OPCODE_TYPE))

#define AFC_TIMING_OPCODE_SET_GET_LINK_STATUS       0
#define AFC_TIMING_NAME_SET_GET_LINK_STATUS         "afc_timing_set_get_link_status"
#define AFC_TIMING_OPCODE_SET_GET_RXEN_STATUS       1
#define AFC_TIMING_NAME_SET_GET_RXEN_STATUS         "afc_timing_set_get_rxen_status"
#define AFC_TIMING_OPCODE_SET_GET_REF_CLK_LOCKED    2
#define AFC_TIMING_NAME_SET_GET_REF_CLK_LOCKED      "afc_timing_set_get_ref_clk_locked"
#define AFC_TIMING_OPCODE_SET_GET_EVT_IN0           3
#define AFC_TIMING_NAME_SET_GET_EVT_IN0             "afc_timing_set_get_evt_code_0"
#define AFC_TIMING_OPCODE_SET_GET_EVT_IN1           4
#define AFC_TIMING_NAME_SET_GET_EVT_IN1             "afc_timing_set_get_evt_code_1"
#define AFC_TIMING_OPCODE_SET_GET_EVT_IN2           5
#define AFC_TIMING_NAME_SET_GET_EVT_IN2             "afc_timing_set_get_evt_code_2"
#define AFC_TIMING_OPCODE_SET_GET_EVT_IN3           6
#define AFC_TIMING_NAME_SET_GET_EVT_IN3             "afc_timing_set_get_evt_code_3"
#define AFC_TIMING_OPCODE_SET_GET_EVT_IN4           7
#define AFC_TIMING_NAME_SET_GET_EVT_IN4             "afc_timing_set_get_evt_code_4"
#define AFC_TIMING_OPCODE_SET_GET_EVT_IN5           8
#define AFC_TIMING_NAME_SET_GET_EVT_IN5             "afc_timing_set_get_evt_code_5"
#define AFC_TIMING_OPCODE_SET_GET_EVT_IN6           9
#define AFC_TIMING_NAME_SET_GET_EVT_IN6             "afc_timing_set_get_evt_code_6"
#define AFC_TIMING_OPCODE_SET_GET_EVT_DLY0          10
#define AFC_TIMING_NAME_SET_GET_EVT_DLY0            "afc_timing_set_get_evt_delay_0"
#define AFC_TIMING_OPCODE_SET_GET_EVT_DLY1          11
#define AFC_TIMING_NAME_SET_GET_EVT_DLY1            "afc_timing_set_get_evt_delay_1"
#define AFC_TIMING_OPCODE_SET_GET_EVT_DLY2          12
#define AFC_TIMING_NAME_SET_GET_EVT_DLY2            "afc_timing_set_get_evt_delay_2"
#define AFC_TIMING_OPCODE_SET_GET_EVT_DLY3          13
#define AFC_TIMING_NAME_SET_GET_EVT_DLY3            "afc_timing_set_get_evt_delay_3"
#define AFC_TIMING_OPCODE_SET_GET_EVT_DLY4          14
#define AFC_TIMING_NAME_SET_GET_EVT_DLY4            "afc_timing_set_get_evt_delay_4"
#define AFC_TIMING_OPCODE_SET_GET_EVT_DLY5          15
#define AFC_TIMING_NAME_SET_GET_EVT_DLY5            "afc_timing_set_get_evt_delay_5"
#define AFC_TIMING_OPCODE_SET_GET_EVT_DLY6          16
#define AFC_TIMING_NAME_SET_GET_EVT_DLY6            "afc_timing_set_get_evt_delay_6"
#define AFC_TIMING_OPCODE_SET_GET_EVT_WDT0          17
#define AFC_TIMING_NAME_SET_GET_EVT_WDT0            "afc_timing_set_get_evt_width_0"
#define AFC_TIMING_OPCODE_SET_GET_EVT_WDT1          18
#define AFC_TIMING_NAME_SET_GET_EVT_WDT1            "afc_timing_set_get_evt_width_1"
#define AFC_TIMING_OPCODE_SET_GET_EVT_WDT2          19
#define AFC_TIMING_NAME_SET_GET_EVT_WDT2            "afc_timing_set_get_evt_width_2"
#define AFC_TIMING_OPCODE_SET_GET_EVT_WDT3          20
#define AFC_TIMING_NAME_SET_GET_EVT_WDT3            "afc_timing_set_get_evt_width_3"
#define AFC_TIMING_OPCODE_SET_GET_EVT_WDT4          21
#define AFC_TIMING_NAME_SET_GET_EVT_WDT4            "afc_timing_set_get_evt_width_4"
#define AFC_TIMING_OPCODE_SET_GET_EVT_WDT5          22
#define AFC_TIMING_NAME_SET_GET_EVT_WDT5            "afc_timing_set_get_evt_width_5"
#define AFC_TIMING_OPCODE_SET_GET_EVT_WDT6          23
#define AFC_TIMING_NAME_SET_GET_EVT_WDT6            "afc_timing_set_get_evt_width_6"
#define AFC_TIMING_OPCODE_SET_GET_FREQ_KP           24
#define AFC_TIMING_NAME_SET_GET_FREQ_KP             "afc_timing_set_get_freq_kp"
#define AFC_TIMING_OPCODE_SET_GET_FREQ_KI           25
#define AFC_TIMING_NAME_SET_GET_FREQ_KI             "afc_timing_set_get_freq_ki"
#define AFC_TIMING_OPCODE_SET_GET_PHASE_KP          26
#define AFC_TIMING_NAME_SET_GET_PHASE_KP            "afc_timing_set_get_phase_kp"
#define AFC_TIMING_OPCODE_SET_GET_PHASE_KI          27
#define AFC_TIMING_NAME_SET_GET_PHASE_KI            "afc_timing_set_get_phase_ki"
#define AFC_TIMING_OPCODE_SET_GET_PHASE_BIAS        28
#define AFC_TIMING_NAME_SET_GET_PHASE_BIAS          "afc_timing_set_get_phase_bias"
#define AFC_TIMING_OPCODE_SET_GET_AVG_EXPONENT      29
#define AFC_TIMING_NAME_SET_GET_AVG_EXPONENT        "afc_timing_set_get_avg_exponent"
#define AFC_TIMING_OPCODE_END                       30

#endif

