/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _SM_IO_FMC_ADC_COMMON_CODES_H_
#define _SM_IO_FMC_ADC_COMMON_CODES_H_

/* Messaging OPCODES */
#define FMC_ADC_COMMON_OPCODE_TYPE                          uint32_t
#define FMC_ADC_COMMON_OPCODE_SIZE                          (sizeof (FMC_ADC_COMMON_OPCODE_TYPE))

#define FMC_ADC_COMMON_OPCODE_LEDS                          0
#define FMC_ADC_COMMON_NAME_LEDS                            "fmc_adc_common_leds"
#define FMC_ADC_COMMON_OPCODE_TEST_DATA_EN                  1
#define FMC_ADC_COMMON_NAME_TEST_DATA_EN                    "fmc_adc_common_adc_test_data_en"
#define FMC_ADC_COMMON_OPCODE_TRIG_DIR                      2
#define FMC_ADC_COMMON_NAME_TRIG_DIR                        "fmc_adc_common_trig_dir"
#define FMC_ADC_COMMON_OPCODE_TRIG_TERM                     3
#define FMC_ADC_COMMON_NAME_TRIG_TERM                       "fmc_adc_common_trig_term"
#define FMC_ADC_COMMON_OPCODE_TRIG_VAL                      4
#define FMC_ADC_COMMON_NAME_TRIG_VAL                        "fmc_adc_common_trig_val"
#define FMC_ADC_COMMON_OPCODE_END                           5

/* Messaging Reply OPCODES */
#define FMC_ADC_COMMON_REPLY_TYPE                           uint32_t
#define FMC_ADC_COMMON_REPLY_SIZE                           (sizeof (FMC_ADC_COMMON_REPLY_TYPE))

#define FMC_ADC_COMMON_OK                                   0   /* Operation was successful */
#define FMC_ADC_COMMON_ERR                                  1   /* Could not set/get value */
#define FMC_ADC_COMMON_UNINPL                               2   /* Unimplemented function or operation */
#define FMC_ADC_COMMON_REPLY_END                            3   /* End marker */

#endif
