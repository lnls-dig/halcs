/*
 * Copyright (C) 2016 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _TRIGGER_MUX_DEFAULTS_H_
#define _TRIGGER_MUX_DEFAULTS_H_

#include "sm_io_err.h"

#define TRIGGER_MUX_DFLT_RCV_SRC                  0      /* Trigger Backplane */
#define TRIGGER_MUX_DFLT_RCV_IN_SEL               0      /* Trigger Index 0 */
#define TRIGGER_MUX_DFLT_TRANSM_SRC               0      /* Trigger Backplane */
#define TRIGGER_MUX_DFLT_TRANSM_IN_SEL            0      /* Trigger Index 0 */

/* FIXME. This shoulw be somewhere else*/
#define TRIGGER_MUX_SW_CLK_CHAN                   17     /* Trigger Channel */
#define TRIGGER_MUX_SW_CLK_DFLT_RCV_SRC           1      /* FPGA Internal */
#define TRIGGER_MUX_SW_CLK_DFLT_RCV_IN_SEL        1      /* Trigger Index 1 */
#define TRIGGER_MUX_SW_CLK_DFLT_TRANSM_SRC        0      /* FPGA Internal */
#define TRIGGER_MUX_SW_CLK_DFLT_TRANSM_IN_SEL     0      /* Trigger Index 0 */

smio_err_e trigger_mux_config_defaults (char *broker_endp, char *service,
        const char *log_file_name);

#endif

