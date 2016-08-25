/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Joao Brito <joao.brito@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _AFC_TIMING_DEFAULTS_H_
#define _AFC_TIMING_DEFAULTS_H_

#define AFC_TIMING_DFLT_PHASE_SET                                        0x5b
#define AFC_TIMING_DFLT_DDMTD_AVG_EXP                                    0x8
#define AFC_TIMING_DFLT_RTM_RFREQ_HI                                     0x57d2
#define AFC_TIMING_DFLT_RTM_RFREQ_LO                                     0x184ea
#define AFC_TIMING_DFLT_RTM_N1                                           0x5
#define AFC_TIMING_DFLT_RTM_HS_DIV                                       0x3
#define AFC_TIMING_DFLT_AFC_RFREQ_HI                                     0x57d2
#define AFC_TIMING_DFLT_AFC_RFREQ_LO                                     0x184ea
#define AFC_TIMING_DFLT_AFC_N1                                           0xb
#define AFC_TIMING_DFLT_AFC_HS_DIV                                       0x3
#define AFC_TIMING_DFLT_FREQ_KP                                          0x1
#define AFC_TIMING_DFLT_FREQ_KI                                          0x64
#define AFC_TIMING_DFLT_PHASE_KP                                         0x10
#define AFC_TIMING_DFLT_PHASE_KI                                         0x6
#define AFC_TIMING_DFLT_EVT_IN0                                          0x1
#define AFC_TIMING_DFLT_EVT_IN1                                          0x1
#define AFC_TIMING_DFLT_EVT_IN2                                          0x1
#define AFC_TIMING_DFLT_EVT_IN3                                          0x1
#define AFC_TIMING_DFLT_EVT_IN4                                          0x1
#define AFC_TIMING_DFLT_EVT_IN5                                          0x1
#define AFC_TIMING_DFLT_EVT_IN6                                          0x1
#define AFC_TIMING_DFLT_EVT_DLY0                                         0x0
#define AFC_TIMING_DFLT_EVT_DLY1                                         0x0
#define AFC_TIMING_DFLT_EVT_DLY2                                         0x0
#define AFC_TIMING_DFLT_EVT_DLY3                                         0x0
#define AFC_TIMING_DFLT_EVT_DLY4                                         0x0
#define AFC_TIMING_DFLT_EVT_DLY5                                         0x0
#define AFC_TIMING_DFLT_EVT_DLY6                                         0x0
#define AFC_TIMING_DFLT_EVT_WDT0                                         0x5
#define AFC_TIMING_DFLT_EVT_WDT1                                         0x5
#define AFC_TIMING_DFLT_EVT_WDT2                                         0x5
#define AFC_TIMING_DFLT_EVT_WDT3                                         0x5
#define AFC_TIMING_DFLT_EVT_WDT4                                         0x5
#define AFC_TIMING_DFLT_EVT_WDT5                                         0x5
#define AFC_TIMING_DFLT_EVT_WDT6                                         0x5

smio_err_e afc_timing_config_defaults (char *broker_endp, char *service,
        const char *log_file_name);

#endif

