/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Joao Brito <joao.brito@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _AFC_TIMING_DEFAULTS_H_
#define _AFC_TIMING_DEFAULTS_H_

#define AFC_TIMING_DFLT_RTM_RFREQ_HI                                     0x2dee
#define AFC_TIMING_DFLT_RTM_RFREQ_LO                                     0x7dfb9
#define AFC_TIMING_DFLT_RTM_N1                                           0x5
#define AFC_TIMING_DFLT_RTM_HS_DIV                                       0x3
#define AFC_TIMING_DFLT_AFC_RFREQ_HI                                     0x2dee
#define AFC_TIMING_DFLT_AFC_RFREQ_LO                                     0x7dfb9
#define AFC_TIMING_DFLT_AFC_N1                                           0xb
#define AFC_TIMING_DFLT_AFC_HS_DIV                                       0x3
#define AFC_TIMING_DFLT_AFC_FREQ_KP                                      0x1
#define AFC_TIMING_DFLT_AFC_FREQ_KI                                      0xc00
#define AFC_TIMING_DFLT_AFC_PHASE_KP                                     0x28
#define AFC_TIMING_DFLT_AFC_PHASE_KI                                     0x0
#define AFC_TIMING_DFLT_AFC_PHASE_SET                                    0x5c
#define AFC_TIMING_DFLT_AFC_PHASE_NAVG                                   0x7
#define AFC_TIMING_DFLT_AFC_PHASE_DIV                                    0x1

smio_err_e afc_timing_config_defaults (char *broker_endp, char *service,
        const char *log_file_name);

#endif

