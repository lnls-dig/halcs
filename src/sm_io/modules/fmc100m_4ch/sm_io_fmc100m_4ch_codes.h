/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _SM_IO_FMC_100M_4CH_CODES_H_
#define _SM_IO_FMC_100M_4CH_CODES_H_

/* Messaging OPCODES */
#define FMC_100M_4CH_OPCODE_TYPE                         uint32_t
#define FMC_100M_4CH_OPCODE_SIZE                         (sizeof (FMC_100M_4CH_OPCODE_TYPE))

/*
 * Keep the same IDs for SI571 as FMC_ACTIVE_CLK. So we don't have
 * to reimplement the same client functions
 */
#define FMC_100M_4CH_OPCODE_SI571_OE                     0
#define FMC_100M_4CH_NAME_SI571_OE                       "fmc100m_4ch_si571_oe"
#define FMC_100M_4CH_OPCODE_ACQ_LED                      1
#define FMC_100M_4CH_NAME_ACQ_LED                        "fmc100m_4ch_acq_led"
#define FMC_100M_4CH_OPCODE_SI571_FREQ                   14
#define FMC_100M_4CH_NAME_SI571_FREQ                     "fmc100m_4ch_si571_freq"
#define FMC_100M_4CH_OPCODE_SI571_GET_DEFAULTS           15
#define FMC_100M_4CH_NAME_SI571_GET_DEFAULTS             "fmc100m_4ch_si571_get_defaults"
#define FMC_100M_4CH_OPCODE_SI571_FSTARTUP               19
#define FMC_100M_4CH_NAME_SI571_FSTARTUP                 "fmc100m_4ch_si571_fstartup"
#define FMC_100M_4CH_OPCODE_STATUS                       20
#define FMC_100M_4CH_NAME_STATUS                         "fmc100m_4ch_status"
#define FMC_100M_4CH_OPCODE_ADC_STATUS                   21
#define FMC_100M_4CH_NAME_ADC_STATUS                     "fmc100m_4ch_adc_status"
#define FMC_100M_4CH_OPCODE_ADC_CALIB_GAIN               22
#define FMC_100M_4CH_NAME_ADC_CALIB_GAIN                 "fmc100m_4ch_adc_calib_gain"
#define FMC_100M_4CH_OPCODE_ADC_CALIB_OFFSET             23
#define FMC_100M_4CH_NAME_ADC_CALIB_OFFSET               "fmc100m_4ch_adc_calib_offset"
#define FMC_100M_4CH_OPCODE_ADC_SAT                      24
#define FMC_100M_4CH_NAME_ADC_SAT                        "fmc100m_4ch_adc_sat"
#define FMC_100M_4CH_OPCODE_ADC_SSR                      25
#define FMC_100M_4CH_NAME_ADC_SSR                        "fmc100m_4ch_adc_ssr"
#define FMC_100M_4CH_OPCODE_FS_FREQ                      26
#define FMC_100M_4CH_NAME_FS_FREQ                        "fmc100m_4ch_fs_freq"
#define FMC_100M_4CH_OPCODE_LTC2174_TEST_PATT            27
#define FMC_100M_4CH_NAME_LTC2174_TEST_PATT              "fmc100m_4ch_test_patt"
#define FMC_100M_4CH_OPCODE_LTC2174_TEST_PATT_DATA       28
#define FMC_100M_4CH_NAME_LTC2174_TEST_PATT_DATA         "fmc100m_4ch_test_patt_data"
#define FMC_100M_4CH_OPCODE_LTC2174_RESET                29
#define FMC_100M_4CH_NAME_LTC2174_RESET                  "fmc100m_4ch_reset"
#define FMC_100M_4CH_OPCODE_LTC2174_TWOS_COMPL           30
#define FMC_100M_4CH_NAME_LTC2174_TWOS_COMPL             "fmc100m_4ch_twos_compl"
#define FMC_100M_4CH_OPCODE_END                          30

/* Messaging Reply OPCODES */
#define FMC_100M_4CH_REPLY_TYPE                          uint32_t
#define FMC_100M_4CH_REPLY_SIZE                          (sizeof (FMC_100M_4CH_REPLY_TYPE))

#define FMC_100M_4CH_OK                                  0   /* Operation was successful */
#define FMC_100M_4CH_ERR                                 1   /* Could not set/get value */
#define FMC_100M_4CH_UNINPL                              2   /* Unimplemented function or operation */
#define FMC_100M_4CH_REPLY_END                           3   /* End marker */

#endif
