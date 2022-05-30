/*
 * Copyright (C) 2021 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _SM_IO_RTMLAMP_OHWR_CODES_H_
#define _SM_IO_RTMLAMP_OHWR_CODES_H_

/* Messaging OPCODES */
#define RTMLAMP_OHWR_OPCODE_TYPE                           uint32_t
#define RTMLAMP_OHWR_OPCODE_SIZE                           (sizeof (RTMLAMP_OHWR_OPCODE_TYPE))

#define RTMLAMP_OHWR_OPCODE_STA                            0
#define RTMLAMP_OHWR_NAME_STA                              "rtmlamp_ohwr_sta"
#define RTMLAMP_OHWR_OPCODE_AMP_IFLAG_L                    100
#define RTMLAMP_OHWR_NAME_AMP_IFLAG_L                      "rtmlamp_ohwr_amp_iflag_l"
#define RTMLAMP_OHWR_OPCODE_AMP_TFLAG_L                    101
#define RTMLAMP_OHWR_NAME_AMP_TFLAG_L                      "rtmlamp_ohwr_amp_tflag_l"
#define RTMLAMP_OHWR_OPCODE_AMP_IFLAG_R                    102
#define RTMLAMP_OHWR_NAME_AMP_IFLAG_R                      "rtmlamp_ohwr_amp_iflag_r"
#define RTMLAMP_OHWR_OPCODE_AMP_TFLAG_R                    103
#define RTMLAMP_OHWR_NAME_AMP_TFLAG_R                      "rtmlamp_ohwr_amp_tflag_r"
#define RTMLAMP_OHWR_OPCODE_AMP_EN                         104
#define RTMLAMP_OHWR_NAME_AMP_EN                           "rtmlamp_ohwr_amp_en"
#define RTMLAMP_OHWR_OPCODE_MODE                           105
#define RTMLAMP_OHWR_NAME_MODE                             "rtmlamp_ohwr_mode"
#define RTMLAMP_OHWR_OPCODE_PI_KP                          106
#define RTMLAMP_OHWR_NAME_PI_KP                            "rtmlamp_ohwr_pi_kp"
#define RTMLAMP_OHWR_OPCODE_PI_TI                          107
#define RTMLAMP_OHWR_NAME_PI_TI                            "rtmlamp_ohwr_pi_ti"
#define RTMLAMP_OHWR_OPCODE_PI_SP                          108
#define RTMLAMP_OHWR_NAME_PI_SP                            "rtmlamp_ohwr_pi_sp"
#define RTMLAMP_OHWR_OPCODE_DAC_DATA                       109
#define RTMLAMP_OHWR_NAME_DAC_DATA                         "rtmlamp_ohwr_dac_data"
#define RTMLAMP_OHWR_OPCODE_LIM_A                          110
#define RTMLAMP_OHWR_NAME_LIM_A                            "rtmlamp_ohwr_lim_a"
#define RTMLAMP_OHWR_OPCODE_LIM_B                          111
#define RTMLAMP_OHWR_NAME_LIM_B                            "rtmlamp_ohwr_lim_b"
#define RTMLAMP_OHWR_OPCODE_CNT                            112
#define RTMLAMP_OHWR_NAME_CNT                              "rtmlamp_ohwr_cnt"
#define RTMLAMP_OHWR_OPCODE_EFF_ADC                        113
#define RTMLAMP_OHWR_NAME_EFF_ADC                          "rtmlamp_ohwr_eff_adc"
#define RTMLAMP_OHWR_OPCODE_EFF_DAC                        114
#define RTMLAMP_OHWR_NAME_EFF_DAC                          "rtmlamp_ohwr_eff_dac"
#define RTMLAMP_OHWR_OPCODE_EFF_SP                         115
#define RTMLAMP_OHWR_NAME_EFF_SP                           "rtmlamp_ohwr_eff_sp"
#define RTMLAMP_OHWR_OPCODE_END                            116

/* Messaging Reply OPCODES */
#define RTMLAMP_OHWR_REPLY_TYPE                            uint32_t
#define RTMLAMP_OHWR_REPLY_SIZE                            (sizeof (RTMLAMP_OHWR_REPLY_TYPE))

#define RTMLAMP_OHWR_OK                                    0   /* Operation was successful */
#define RTMLAMP_OHWR_ERR                                   1   /* Could not set/get value */
#define RTMLAMP_OHWR_UNINPL                                2   /* Unimplemented function or operation */
#define RTMLAMP_OHWR_REPLY_END                             3   /* End marker */

#endif
