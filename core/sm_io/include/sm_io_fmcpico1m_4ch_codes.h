/*
 * Copyright (C) 2017 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _SM_IO_FMCPICO1M_4CH_CODES_H_
#define _SM_IO_FMCPICO1M_4CH_CODES_H_

/* Messaging OPCODES */
#define FMCPICO1M_4CH_OPCODE_TYPE                     uint32_t
#define FMCPICO1M_4CH_OPCODE_SIZE                     (sizeof (FMCPICO1M_4CH_OPCODE_TYPE))

#define FMCPICO1M_4CH_OPCODE_SET_GET_LED1             0
#define FMCPICO1M_4CH_NAME_SET_GET_LED1               "fmcpico1m_4ch_set_get_led1"
#define FMCPICO1M_4CH_OPCODE_SET_GET_LED2             1
#define FMCPICO1M_4CH_NAME_SET_GET_LED2               "fmcpico1m_4ch_set_get_led2"
#define FMCPICO1M_4CH_OPCODE_SET_GET_RNG_R0           2
#define FMCPICO1M_4CH_NAME_SET_GET_RNG_R0             "fmcpico1m_4ch_set_get_rng_r0"
#define FMCPICO1M_4CH_OPCODE_SET_GET_RNG_R1           3
#define FMCPICO1M_4CH_NAME_SET_GET_RNG_R1             "fmcpico1m_4ch_set_get_rng_r1"
#define FMCPICO1M_4CH_OPCODE_SET_GET_RNG_R2           4
#define FMCPICO1M_4CH_NAME_SET_GET_RNG_R2             "fmcpico1m_4ch_set_get_rng_r2"
#define FMCPICO1M_4CH_OPCODE_SET_GET_RNG_R3           5
#define FMCPICO1M_4CH_NAME_SET_GET_RNG_R3             "fmcpico1m_4ch_set_get_rng_r3"
#define FMCPICO1M_4CH_OPCODE_SET_GET_ADC_DATA0        6
#define FMCPICO1M_4CH_NAME_SET_GET_ADC_DATA0          "fmcpico1m_4ch_set_get_adc_data0"
#define FMCPICO1M_4CH_OPCODE_SET_GET_ADC_DATA1        7
#define FMCPICO1M_4CH_NAME_SET_GET_ADC_DATA1          "fmcpico1m_4ch_set_get_adc_data1"
#define FMCPICO1M_4CH_OPCODE_SET_GET_ADC_DATA2        8
#define FMCPICO1M_4CH_NAME_SET_GET_ADC_DATA2          "fmcpico1m_4ch_set_get_adc_data2"
#define FMCPICO1M_4CH_OPCODE_SET_GET_ADC_DATA3        9
#define FMCPICO1M_4CH_NAME_SET_GET_ADC_DATA3          "fmcpico1m_4ch_set_get_adc_data3"
#define FMCPICO1M_4CH_OPCODE_END                      10

#endif
