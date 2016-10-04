/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _SM_IO_FMC250M_4CH_CODES_H_
#define _SM_IO_FMC250M_4CH_CODES_H_

/* Messaging OPCODES */
#define FMC250M_4CH_OPCODE_TYPE                         uint32_t
#define FMC250M_4CH_OPCODE_SIZE                         (sizeof (FMC250M_4CH_OPCODE_TYPE))

#if 0
#define FMC250M_4CH_OPCODE_ADC_RAND                     5
#define FMC250M_4CH_NAME_ADC_RAND                       "fmc250m_4ch_adc_rand"
#define FMC250M_4CH_OPCODE_ADC_DITH                     6
#define FMC250M_4CH_NAME_ADC_DITH                       "fmc250m_4ch_adc_dith"
#define FMC250M_4CH_OPCODE_ADC_SHDN                     7
#define FMC250M_4CH_NAME_ADC_SHDN                       "fmc250m_4ch_adc_shdn"
#define FMC250M_4CH_OPCODE_ADC_PGA                      8
#define FMC250M_4CH_NAME_ADC_PGA                        "fmc250m_4ch_adc_pga"
#endif
#define FMC250M_4CH_OPCODE_ADC_DATA0                    9
#define FMC250M_4CH_NAME_ADC_DATA0                      "fmc250m_4ch_adc_data0"
#define FMC250M_4CH_OPCODE_ADC_DATA1                    10
#define FMC250M_4CH_NAME_ADC_DATA1                      "fmc250m_4ch_adc_data1"
#define FMC250M_4CH_OPCODE_ADC_DATA2                    11
#define FMC250M_4CH_NAME_ADC_DATA2                      "fmc250m_4ch_adc_data2"
#define FMC250M_4CH_OPCODE_ADC_DATA3                    12
#define FMC250M_4CH_NAME_ADC_DATA3                      "fmc250m_4ch_adc_data3"
#if 0
#define FMC250M_4CH_OPCODE_ADC_DLY_VAL0                 13
#define FMC250M_4CH_NAME_ADC_DLY_VAL0                   "fmc250m_4ch_adc_dly_val0"
#define FMC250M_4CH_OPCODE_ADC_DLY_VAL1                 14
#define FMC250M_4CH_NAME_ADC_DLY_VAL1                   "fmc250m_4ch_adc_dly_val1"
#define FMC250M_4CH_OPCODE_ADC_DLY_VAL2                 15
#define FMC250M_4CH_NAME_ADC_DLY_VAL2                   "fmc250m_4ch_adc_dly_val2"
#define FMC250M_4CH_OPCODE_ADC_DLY_VAL3                 16
#define FMC250M_4CH_NAME_ADC_DLY_VAL3                   "fmc250m_4ch_adc_dly_val3"
#define FMC250M_4CH_OPCODE_ADC_DLY_LINE0                17
#define FMC250M_4CH_NAME_ADC_DLY_LINE0                  "fmc250m_4ch_adc_dly_line0"
#define FMC250M_4CH_OPCODE_ADC_DLY_LINE1                18
#define FMC250M_4CH_NAME_ADC_DLY_LINE1                  "fmc250m_4ch_adc_dly_line1"
#define FMC250M_4CH_OPCODE_ADC_DLY_LINE2                19
#define FMC250M_4CH_NAME_ADC_DLY_LINE2                  "fmc250m_4ch_adc_dly_line2"
#define FMC250M_4CH_OPCODE_ADC_DLY_LINE3                20
#define FMC250M_4CH_NAME_ADC_DLY_LINE3                  "fmc250m_4ch_adc_dly_line3"
#define FMC250M_4CH_OPCODE_ADC_DLY_UPDT0                21
#define FMC250M_4CH_NAME_ADC_DLY_UPDT0                  "fmc250m_4ch_adc_dly_updt0"
#define FMC250M_4CH_OPCODE_ADC_DLY_UPDT1                22
#define FMC250M_4CH_NAME_ADC_DLY_UPDT1                  "fmc250m_4ch_adc_dly_updt1"
#define FMC250M_4CH_OPCODE_ADC_DLY_UPDT2                23
#define FMC250M_4CH_NAME_ADC_DLY_UPDT2                  "fmc250m_4ch_adc_dly_updt2"
#define FMC250M_4CH_OPCODE_ADC_DLY_UPDT3                24
#define FMC250M_4CH_NAME_ADC_DLY_UPDT3                  "fmc250m_4ch_adc_dly_updt3"
#define FMC250M_4CH_OPCODE_ADC_DLY0                     25
#define FMC250M_4CH_NAME_ADC_DLY0                       "fmc250m_4ch_adc_dly0"
#define FMC250M_4CH_OPCODE_ADC_DLY1                     26
#define FMC250M_4CH_NAME_ADC_DLY1                       "fmc250m_4ch_adc_dly1"
#define FMC250M_4CH_OPCODE_ADC_DLY2                     27
#define FMC250M_4CH_NAME_ADC_DLY2                       "fmc250m_4ch_adc_dly2"
#define FMC250M_4CH_OPCODE_ADC_DLY3                     28
#define FMC250M_4CH_NAME_ADC_DLY3                       "fmc250m_4ch_adc_dly3"
#endif
#define FMC250M_4CH_OPCODE_RST_ADCS                     45
#define FMC250M_4CH_NAME_RST_ADCS                       "fmc250m_4ch_rst_adcs"
#define FMC250M_4CH_OPCODE_RST_DIV_ADCS                 46
#define FMC250M_4CH_NAME_RST_DIV_ADCS                   "fmc250m_4ch_rst_div_adcs"
#define FMC250M_4CH_OPCODE_SLEEP_ADCS                   47
#define FMC250M_4CH_NAME_SLEEP_ADCS                     "fmc250m_4ch_sleep_adcs"
#define FMC250M_4CH_OPCODE_TESTMODE                     48
#define FMC250M_4CH_NAME_TESTMODE                       "fmc250m_4ch_test_mode"
#define FMC250M_4CH_OPCODE_RST                          49
#define FMC250M_4CH_NAME_RST                            "fmc250m_4ch_rst"
#define FMC250M_4CH_OPCODE_PORTCONFIG                   50
#define FMC250M_4CH_NAME_PORTCONFIG                     "fmc250m_4ch_portconfig"
#define FMC250M_4CH_OPCODE_REG                          51
#define FMC250M_4CH_NAME_REG                            "fmc250m_4ch_reg"
#define FMC250M_4CH_OPCODE_TEMP                         52
#define FMC250M_4CH_NAME_TEMP                           "fmc250m_4ch_temp"
#define FMC250M_4CH_OPCODE_END                          53

/* Messaging Reply OPCODES */
#define FMC250M_4CH_REPLY_TYPE                          uint32_t
#define FMC250M_4CH_REPLY_SIZE                          (sizeof (FMC250M_4CH_REPLY_TYPE))

#define FMC250M_4CH_OK                                  0   /* Operation was successful */
#define FMC250M_4CH_ERR                                 1   /* Could not set/get value */
#define FMC250M_4CH_UNINPL                              2   /* Unimplemented function or operation */
#define FMC250M_4CH_REPLY_END                           3   /* End marker */

#endif
