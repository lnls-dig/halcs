/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _SM_IO_SWAP_USEFUL_MACROS_H_
#define _SM_IO_SWAP_USEFUL_MACROS_H_

#include "hw/wb_bpm_swap_regs.h"

/* Some useful macros */
#define RW_SWAP_GAIN_LOWER_R(val)           BPM_SWAP_A_A_R(val)
#define RW_SWAP_GAIN_UPPER_R(val)           BPM_SWAP_A_C_R(val)
#define RW_SWAP_GAIN_LOWER_W(val)           BPM_SWAP_A_A_W(val)
#define RW_SWAP_GAIN_UPPER_W(val)           BPM_SWAP_A_C_W(val)

#endif
