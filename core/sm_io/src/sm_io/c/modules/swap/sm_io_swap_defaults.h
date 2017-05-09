/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _SWAP_DEFAULTS_H_
#define _SWAP_DEFAULTS_H_

/******************** Switching Clock Default Values **************************/
#define SWAP_DFLT_SW                    1               /* 1 is switching in direct state
                                                            No switching */

#define SWAP_DFLT_SW_DLY                30              /* in ADC counts */
#define SWAP_DFLT_DIV_CLK               980             /* in ADC counts */

/************************** Gain Default Values ******************************/

smio_err_e swap_config_defaults (char *broker_endp, char *service,
        const char *log_file_name);

#endif

