/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _SWAP_DEFAULTS_H_
#define _SWAP_DEFAULTS_H_

/******************** Switching Clock Default Values **************************/
#define SWAP_DFLT_SW                    1               /* 1 is switching in direct state
                                                            No switching */
#define SWAP_DFLT_SW_EN                 0               /* Switching clock is disable */

#define _SWAP_DFLT_DIV_CLK              980             /* in ADC counts */

/* This parameter was not supposed to be like this. We need to divide by 2,
 * because the RFFE uses the FPGA switching clock as a trigger to regenerate
 * it internally */
#define SWAP_DFLT_DIV_CLK                (_SWAP_DFLT_DIV_CLK/2)   /* in ADC counts */

/************************** Gain Default Values ******************************/

#define _SWAP_DFLT_MAX_GAIN             ((1 << 16)-1)   /* 16-bit gain */
#define _SWAP_DFLT_MIN_GAIN             0
/* Default gain is just the mean of the gain range */
#define _SWAP_DFLT_GAIN                 ( ((_SWAP_DFLT_MAX_GAIN + \
                                            _SWAP_DFLT_MIN_GAIN) + 1) / 2)

#define SWAP_DFLT_GAIN_AA               _SWAP_DFLT_GAIN
#define SWAP_DFLT_GAIN_AC               _SWAP_DFLT_GAIN

#define SWAP_DFLT_GAIN_BB               _SWAP_DFLT_GAIN
#define SWAP_DFLT_GAIN_BD               _SWAP_DFLT_GAIN

#define SWAP_DFLT_GAIN_CC               _SWAP_DFLT_GAIN
#define SWAP_DFLT_GAIN_CA               _SWAP_DFLT_GAIN

#define SWAP_DFLT_GAIN_DD               _SWAP_DFLT_GAIN
#define SWAP_DFLT_GAIN_DB               _SWAP_DFLT_GAIN

smio_err_e swap_config_defaults (char *broker_endp, char *service,
        const char *log_file_name);

#endif

