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

#define AFC_TIMING_OPCODE_SET_GET_LINK_STATUS               0
#define AFC_TIMING_NAME_SET_GET_LINK_STATUS                 "afc_timing_set_get_link_status"
#define AFC_TIMING_OPCODE_SET_GET_RXEN_STATUS               1
#define AFC_TIMING_NAME_SET_GET_RXEN_STATUS                 "afc_timing_set_get_rxen_status"
#define AFC_TIMING_OPCODE_SET_GET_REF_CLK_LOCKED            2
#define AFC_TIMING_NAME_SET_GET_REF_CLK_LOCKED              "afc_timing_set_get_ref_clk_locked"
#define AFC_TIMING_OPCODE_SET_GET_EVREN                     3
#define AFC_TIMING_NAME_SET_GET_EVREN                       "afc_timing_set_get_evren"
#define AFC_TIMING_OPCODE_SET_GET_ALIVE                     4
#define AFC_TIMING_NAME_SET_GET_ALIVE                       "afc_timing_set_get_alive"
#define AFC_TIMING_OPCODE_SET_GET_AMC_EN                    5
#define AFC_TIMING_NAME_SET_GET_AMC_EN                      "afc_timing_set_get_amc_en"
#define AFC_TIMING_OPCODE_SET_GET_AMC_POL                   6
#define AFC_TIMING_NAME_SET_GET_AMC_POL                     "afc_timing_set_get_amc_pol"
#define AFC_TIMING_OPCODE_SET_GET_AMC_LOG                   7
#define AFC_TIMING_NAME_SET_GET_AMC_LOG                     "afc_timing_set_get_amc_log"
#define AFC_TIMING_OPCODE_SET_GET_AMC_ITL                   8
#define AFC_TIMING_NAME_SET_GET_AMC_ITL                     "afc_timing_set_get_amc_itl"
#define AFC_TIMING_OPCODE_SET_GET_AMC_SRC                   9
#define AFC_TIMING_NAME_SET_GET_AMC_SRC                     "afc_timing_set_get_amc_src"
#define AFC_TIMING_OPCODE_SET_GET_AMC_DIR                   10
#define AFC_TIMING_NAME_SET_GET_AMC_DIR                     "afc_timing_set_get_amc_dir"
#define AFC_TIMING_OPCODE_SET_GET_AMC_COUNT_RST             11
#define AFC_TIMING_NAME_SET_GET_AMC_COUNT_RST               "afc_timing_set_get_amc_count_rst"
#define AFC_TIMING_OPCODE_SET_GET_AMC_PULSES                12
#define AFC_TIMING_NAME_SET_GET_AMC_PULSES                  "afc_timing_set_get_amc_pulses"
#define AFC_TIMING_OPCODE_SET_GET_AMC_COUNT                 13
#define AFC_TIMING_NAME_SET_GET_AMC_COUNT                   "afc_timing_set_get_amc_count"
#define AFC_TIMING_OPCODE_SET_GET_AMC_EVT                   14
#define AFC_TIMING_NAME_SET_GET_AMC_EVT                     "afc_timing_set_get_amc_evt"
#define AFC_TIMING_OPCODE_SET_GET_AMC_DLY                   15
#define AFC_TIMING_NAME_SET_GET_AMC_DLY                     "afc_timing_set_get_amc_dly"
#define AFC_TIMING_OPCODE_SET_GET_AMC_WDT                   16
#define AFC_TIMING_NAME_SET_GET_AMC_WDT                     "afc_timing_set_get_amc_wdt"
#define AFC_TIMING_OPCODE_SET_GET_FMC1_EN                   17
#define AFC_TIMING_NAME_SET_GET_FMC1_EN                     "afc_timing_set_get_fmc1_en"
#define AFC_TIMING_OPCODE_SET_GET_FMC1_POL                  18
#define AFC_TIMING_NAME_SET_GET_FMC1_POL                    "afc_timing_set_get_fmc1_pol"
#define AFC_TIMING_OPCODE_SET_GET_FMC1_LOG                  19
#define AFC_TIMING_NAME_SET_GET_FMC1_LOG                    "afc_timing_set_get_fmc1_log"
#define AFC_TIMING_OPCODE_SET_GET_FMC1_ITL                  20
#define AFC_TIMING_NAME_SET_GET_FMC1_ITL                    "afc_timing_set_get_fmc1_itl"
#define AFC_TIMING_OPCODE_SET_GET_FMC1_SRC                  21
#define AFC_TIMING_NAME_SET_GET_FMC1_SRC                    "afc_timing_set_get_fmc1_src"
#define AFC_TIMING_OPCODE_SET_GET_FMC1_DIR                  22
#define AFC_TIMING_NAME_SET_GET_FMC1_DIR                    "afc_timing_set_get_fmc1_dir"
#define AFC_TIMING_OPCODE_SET_GET_FMC1_COUNT_RST            23
#define AFC_TIMING_NAME_SET_GET_FMC1_COUNT_RST              "afc_timing_set_get_fmc1_count_rst"
#define AFC_TIMING_OPCODE_SET_GET_FMC1_PULSES               24
#define AFC_TIMING_NAME_SET_GET_FMC1_PULSES                 "afc_timing_set_get_fmc1_pulses"
#define AFC_TIMING_OPCODE_SET_GET_FMC1_COUNT                25
#define AFC_TIMING_NAME_SET_GET_FMC1_COUNT                  "afc_timing_set_get_fmc1_count"
#define AFC_TIMING_OPCODE_SET_GET_FMC1_EVT                  26
#define AFC_TIMING_NAME_SET_GET_FMC1_EVT                    "afc_timing_set_get_fmc1_evt"
#define AFC_TIMING_OPCODE_SET_GET_FMC1_DLY                  27
#define AFC_TIMING_NAME_SET_GET_FMC1_DLY                    "afc_timing_set_get_fmc1_dly"
#define AFC_TIMING_OPCODE_SET_GET_FMC1_WDT                  28
#define AFC_TIMING_NAME_SET_GET_FMC1_WDT                    "afc_timing_set_get_fmc1_wdt"
#define AFC_TIMING_OPCODE_SET_GET_FMC2_EN                   29
#define AFC_TIMING_NAME_SET_GET_FMC2_EN                     "afc_timing_set_get_fmc2_en"
#define AFC_TIMING_OPCODE_SET_GET_FMC2_POL                  30
#define AFC_TIMING_NAME_SET_GET_FMC2_POL                    "afc_timing_set_get_fmc2_pol"
#define AFC_TIMING_OPCODE_SET_GET_FMC2_LOG                  31
#define AFC_TIMING_NAME_SET_GET_FMC2_LOG                    "afc_timing_set_get_fmc2_log"
#define AFC_TIMING_OPCODE_SET_GET_FMC2_ITL                  32
#define AFC_TIMING_NAME_SET_GET_FMC2_ITL                    "afc_timing_set_get_fmc2_itl"
#define AFC_TIMING_OPCODE_SET_GET_FMC2_SRC                  33
#define AFC_TIMING_NAME_SET_GET_FMC2_SRC                    "afc_timing_set_get_fmc2_src"
#define AFC_TIMING_OPCODE_SET_GET_FMC2_DIR                  34
#define AFC_TIMING_NAME_SET_GET_FMC2_DIR                    "afc_timing_set_get_fmc2_dir"
#define AFC_TIMING_OPCODE_SET_GET_FMC2_COUNT_RST            35
#define AFC_TIMING_NAME_SET_GET_FMC2_COUNT_RST              "afc_timing_set_get_fmc2_count_rst"
#define AFC_TIMING_OPCODE_SET_GET_FMC2_PULSES               36
#define AFC_TIMING_NAME_SET_GET_FMC2_PULSES                 "afc_timing_set_get_fmc2_pulses"
#define AFC_TIMING_OPCODE_SET_GET_FMC2_COUNT                37
#define AFC_TIMING_NAME_SET_GET_FMC2_COUNT                  "afc_timing_set_get_fmc2_count"
#define AFC_TIMING_OPCODE_SET_GET_FMC2_EVT                  38
#define AFC_TIMING_NAME_SET_GET_FMC2_EVT                    "afc_timing_set_get_fmc2_evt"
#define AFC_TIMING_OPCODE_SET_GET_FMC2_DLY                  39
#define AFC_TIMING_NAME_SET_GET_FMC2_DLY                    "afc_timing_set_get_fmc2_dly"
#define AFC_TIMING_OPCODE_SET_GET_FMC2_WDT                  40
#define AFC_TIMING_NAME_SET_GET_FMC2_WDT                    "afc_timing_set_get_fmc2_wdt"
#define AFC_TIMING_OPCODE_SET_GET_RTM_FREQ_KP               41
#define AFC_TIMING_NAME_SET_GET_RTM_FREQ_KP                 "afc_timing_set_get_rtm_freq_kp"
#define AFC_TIMING_OPCODE_SET_GET_RTM_FREQ_KI               42
#define AFC_TIMING_NAME_SET_GET_RTM_FREQ_KI                 "afc_timing_set_get_rtm_freq_ki"
#define AFC_TIMING_OPCODE_SET_GET_RTM_PHASE_KP              43
#define AFC_TIMING_NAME_SET_GET_RTM_PHASE_KP                "afc_timing_set_get_rtm_phase_kp"
#define AFC_TIMING_OPCODE_SET_GET_RTM_PHASE_KI              44
#define AFC_TIMING_NAME_SET_GET_RTM_PHASE_KI                "afc_timing_set_get_rtm_phase_ki"
#define AFC_TIMING_OPCODE_SET_GET_RTM_PHASE_NAVG            45
#define AFC_TIMING_NAME_SET_GET_RTM_PHASE_NAVG              "afc_timing_set_get_rtm_phase_navg"
#define AFC_TIMING_OPCODE_SET_GET_RTM_PHASE_DIV_EXP         46
#define AFC_TIMING_NAME_SET_GET_RTM_PHASE_DIV_EXP           "afc_timing_set_get_rtm_phase_div_exp"
#define AFC_TIMING_OPCODE_SET_GET_RTM_RFREQ_HI              47
#define AFC_TIMING_NAME_SET_GET_RTM_RFREQ_HI                "afc_timing_set_get_rtm_rfreq_hi"
#define AFC_TIMING_OPCODE_SET_GET_RTM_RFREQ_LO              48
#define AFC_TIMING_NAME_SET_GET_RTM_RFREQ_LO                "afc_timing_set_get_rtm_rfreq_lo"
#define AFC_TIMING_OPCODE_SET_GET_RTM_N1                    49
#define AFC_TIMING_NAME_SET_GET_RTM_N1                      "afc_timing_set_get_rtm_n1"
#define AFC_TIMING_OPCODE_SET_GET_RTM_HS_DIV                50
#define AFC_TIMING_NAME_SET_GET_RTM_HS_DIV                  "afc_timing_set_get_rtm_hs_div"
#define AFC_TIMING_OPCODE_SET_GET_AFC_FREQ_KP               51
#define AFC_TIMING_NAME_SET_GET_AFC_FREQ_KP                 "afc_timing_set_get_afc_freq_kp"
#define AFC_TIMING_OPCODE_SET_GET_AFC_FREQ_KI               52
#define AFC_TIMING_NAME_SET_GET_AFC_FREQ_KI                 "afc_timing_set_get_afc_freq_ki"
#define AFC_TIMING_OPCODE_SET_GET_AFC_PHASE_KP              53
#define AFC_TIMING_NAME_SET_GET_AFC_PHASE_KP                "afc_timing_set_get_afc_phase_kp"
#define AFC_TIMING_OPCODE_SET_GET_AFC_PHASE_KI              54
#define AFC_TIMING_NAME_SET_GET_AFC_PHASE_KI                "afc_timing_set_get_afc_phase_ki"
#define AFC_TIMING_OPCODE_SET_GET_AFC_PHASE_NAVG            55
#define AFC_TIMING_NAME_SET_GET_AFC_PHASE_NAVG              "afc_timing_set_get_afc_phase_navg"
#define AFC_TIMING_OPCODE_SET_GET_AFC_PHASE_DIV_EXP         56
#define AFC_TIMING_NAME_SET_GET_AFC_PHASE_DIV_EXP           "afc_timing_set_get_afc_phase_div_exp"
#define AFC_TIMING_OPCODE_SET_GET_AFC_RFREQ_HI              57
#define AFC_TIMING_NAME_SET_GET_AFC_RFREQ_HI                "afc_timing_set_get_afc_rfreq_hi"
#define AFC_TIMING_OPCODE_SET_GET_AFC_RFREQ_LO              58
#define AFC_TIMING_NAME_SET_GET_AFC_RFREQ_LO                "afc_timing_set_get_afc_rfreq_lo"
#define AFC_TIMING_OPCODE_SET_GET_AFC_N1                    59
#define AFC_TIMING_NAME_SET_GET_AFC_N1                      "afc_timing_set_get_afc_n1"
#define AFC_TIMING_OPCODE_SET_GET_AFC_HS_DIV                60
#define AFC_TIMING_NAME_SET_GET_AFC_HS_DIV                  "afc_timing_set_get_afc_hs_div"

#define AFC_TIMING_OPCODE_END                               61

#endif

