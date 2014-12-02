/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _SM_IO_FMC130M_4CH_CODES_H_
#define _SM_IO_FMC130M_4CH_CODES_H_

#include <inttypes.h>

/* Messaging OPCODES */
#define FMC130M_4CH_OPCODE_SIZE                         (sizeof(uint32_t))
#define FMC130M_4CH_OPCODE_TYPE                         uint32_t

#define FMC130M_4CH_OPCODE_LEDS                         0
#define FMC130M_4CH_NAME_LEDS                           "leds"
#define FMC130M_4CH_OPCODE_PLL_FUNCTION                 1
#define FMC130M_4CH_NAME_PLL_FUNCTION                   "pll_function"
#define FMC130M_4CH_OPCODE_AD9510_CFG_DEFAULTS          2
#define FMC130M_4CH_NAME_AD9510_CFG_DEFAULTS            "ad9510_cfg_defaults"
#define FMC130M_4CH_OPCODE_ADC_DATA0                    3
#define FMC130M_4CH_NAME_ADC_DATA0                      "adc_data0"
#define FMC130M_4CH_OPCODE_ADC_DATA1                    4
#define FMC130M_4CH_NAME_ADC_DATA1                      "adc_data1"
#define FMC130M_4CH_OPCODE_ADC_DATA2                    5
#define FMC130M_4CH_NAME_ADC_DATA2                      "adc_data2"
#define FMC130M_4CH_OPCODE_ADC_DATA3                    6
#define FMC130M_4CH_NAME_ADC_DATA3                      "adc_data3"
#define FMC130M_4CH_OPCODE_ADC_DLY_VAL0                 7
#define FMC130M_4CH_NAME_ADC_DLY_VAL0                   "adc_dly_val0"
#define FMC130M_4CH_OPCODE_ADC_DLY_VAL1                 8
#define FMC130M_4CH_NAME_ADC_DLY_VAL1                   "adc_dly_val1"
#define FMC130M_4CH_OPCODE_ADC_DLY_VAL2                 9
#define FMC130M_4CH_NAME_ADC_DLY_VAL2                   "adc_dly_val2"
#define FMC130M_4CH_OPCODE_ADC_DLY_VAL3                 10
#define FMC130M_4CH_NAME_ADC_DLY_VAL3                   "adc_dly_val3"
#define FMC130M_4CH_OPCODE_ADC_DLY_LINE0                11
#define FMC130M_4CH_NAME_ADC_DLY_LINE0                  "adc_dly_line0"
#define FMC130M_4CH_OPCODE_ADC_DLY_LINE1                12
#define FMC130M_4CH_NAME_ADC_DLY_LINE1                  "adc_dly_line1"
#define FMC130M_4CH_OPCODE_ADC_DLY_LINE2                13
#define FMC130M_4CH_NAME_ADC_DLY_LINE2                  "adc_dly_line2"
#define FMC130M_4CH_OPCODE_ADC_DLY_LINE3                14
#define FMC130M_4CH_NAME_ADC_DLY_LINE3                  "adc_dly_line3"
#define FMC130M_4CH_OPCODE_ADC_DLY_UPDT0                15
#define FMC130M_4CH_NAME_ADC_DLY_UPDT0                  "adc_dly_updt0"
#define FMC130M_4CH_OPCODE_ADC_DLY_UPDT1                16
#define FMC130M_4CH_NAME_ADC_DLY_UPDT1                  "adc_dly_updt1"
#define FMC130M_4CH_OPCODE_ADC_DLY_UPDT2                17
#define FMC130M_4CH_NAME_ADC_DLY_UPDT2                  "adc_dly_updt2"
#define FMC130M_4CH_OPCODE_ADC_DLY_UPDT3                18
#define FMC130M_4CH_NAME_ADC_DLY_UPDT3                  "adc_dly_updt3"
#define FMC130M_4CH_OPCODE_ADC_DLY0                     19
#define FMC130M_4CH_NAME_ADC_DLY0                       "adc_dly0"
#define FMC130M_4CH_OPCODE_ADC_DLY1                     20
#define FMC130M_4CH_NAME_ADC_DLY1                       "adc_dly1"
#define FMC130M_4CH_OPCODE_ADC_DLY2                     21
#define FMC130M_4CH_NAME_ADC_DLY2                       "adc_dly2"
#define FMC130M_4CH_OPCODE_ADC_DLY3                     22
#define FMC130M_4CH_NAME_ADC_DLY3                       "adc_dly3"
#define FMC130M_4CH_OPCODE_TEST_DATA_EN                 23
#define FMC130M_4CH_NAME_TEST_DATA_EN                   "adc_test_data_en"
#define FMC130M_4CH_OPCODE_SI571_OE                     24
#define FMC130M_4CH_NAME_SI571_OE                       "si571_oe"
#define FMC130M_4CH_OPCODE_AD9510_PLL_A_DIV             25
#define FMC130M_4CH_NAME_AD9510_PLL_A_DIV               "ad9510_pll_a_div"
#define FMC130M_4CH_OPCODE_AD9510_PLL_B_DIV             26
#define FMC130M_4CH_NAME_AD9510_PLL_B_DIV               "ad9510_pll_b_div"
#define FMC130M_4CH_OPCODE_AD9510_PLL_PRESCALER         27
#define FMC130M_4CH_NAME_AD9510_PLL_PRESCALER           "ad9510_pll_prescaler"
#define FMC130M_4CH_OPCODE_AD9510_R_DIV                 28
#define FMC130M_4CH_NAME_AD9510_R_DIV                   "ad9510_r_div"
#define FMC130M_4CH_OPCODE_AD9510_PLL_PDOWN             29
#define FMC130M_4CH_NAME_AD9510_PLL_PDOWN               "ad9510_pll_pdown"
#define FMC130M_4CH_OPCODE_AD9510_MUX_STATUS            30
#define FMC130M_4CH_NAME_AD9510_MUX_STATUS              "ad9510_mux_status"
#define FMC130M_4CH_OPCODE_AD9510_CP_CURRENT            31
#define FMC130M_4CH_NAME_AD9510_CP_CURRENT              "ad9510_cp_current"
#define FMC130M_4CH_OPCODE_AD9510_OUTPUTS               32
#define FMC130M_4CH_NAME_AD9510_OUTPUTS                 "ad9510_outputs"
#define FMC130M_4CH_OPCODE_AD9510_PLL_CLK_SEL           33
#define FMC130M_4CH_NAME_AD9510_PLL_CLK_SEL             "ad9510_pll_clk_sel"
#define FMC130M_4CH_OPCODE_SI571_SET_FREQ               34
#define FMC130M_4CH_NAME_SI571_SET_FREQ                 "si571_set_freq"
#define FMC130M_4CH_OPCODE_SI571_GET_DEFAULTS           34
#define FMC130M_4CH_NAME_SI571_GET_DEFAULTS             "si571_get_defaults"
#define FMC130M_4CH_OPCODE_END                          35

/* Messaging Reply OPCODES */
#define FMC130M_4CH_REPLY_SIZE                          (sizeof(uint32_t))
#define FMC130M_4CH_REPLY_TYPE                          uint32_t

#define FMC130M_4CH_OK                                  0   /* Operation was successful */
#define FMC130M_4CH_ERR                                 1   /* Could not set/get value */
#define FMC130M_4CH_UNINPL                              2   /* Unimplemented function or operation */
#define FMC130M_4CH_REPLY_END                           3   /* End marker */

#endif
