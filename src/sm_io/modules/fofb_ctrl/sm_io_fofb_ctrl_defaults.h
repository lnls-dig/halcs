/*
 * Copyright (C) 2021 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _FOFB_CTRL_DEFAULTS_H_
#define _FOFB_CTRL_DEFAULTS_H_

#define FOFB_CTRL_DFLT_CC_ENABLE              0x1         /* Enabled */
#define FOFB_CTRL_DFLT_TIME_FRAME_LEN         0x1D4C      /* */
#define FOFB_CTRL_DFLT_TIME_FRAME_DLY         0           /* */
#define FOFB_CTRL_DFLT_BPM_ID                 0xFFFFFFFF  /* MAX ID. Must be changed before operation */

smio_err_e fofb_ctrl_config_defaults (char *broker_endp, char *service,
        const char *log_file_name);

#endif

