/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _TIM_RCV_DEFAULTS_H_
#define _TIM_RCV_DEFAULTS_H_

#define TIM_RCV_DFLT_PHASE_MEAS_NAVG              10
#define TIM_RCV_DFLT_DMTD_A_DEGLITCH_THRES        10
#define TIM_RCV_DFLT_DMTD_B_DEGLITCH_THRES        10

smio_err_e tim_rcv_config_defaults (char *broker_endp, char *service,
        const char *log_file_name);

#endif

