/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _SM_PR_SPI_DEFAULTS_H_
#define _SM_PR_SPI_DEFAULTS_H_

#include "wb_spi_regs.h"

#define  SM_PR_SPI_DFLT_SYS_FREQ        100000000       /* 100 MHz */
#define  SM_PR_SPI_DFLT_SPI_FREQ        100000          /* 100 KHz */
/* ASS = 1,
 * TX_NEG = 1 (data changed on falling edge),
 * RX_NEG = 0 (data latched on rising edge) */
#define SM_PR_SPI_DFLT_SPI_CONFIG       (SPI_PROTO_CTRL_ASS | SPI_PROTO_CTRL_TXNEG)
#define SM_PR_SPI_DFLT_BIDIR            0x0             /* Default is off */

#endif

