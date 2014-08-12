/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _SM_IO_FMC130M_4CH_CODES_H_
#define _SM_IO_FMC130M_4CH_CODES_H_

#include <inttypes.h>

/* Messaging OPCODES */
#define FMC130M_4CH_OPCODE_SIZE                 (sizeof(uint32_t))
#define FMC130M_4CH_OPCODE_TYPE                 uint32_t

#define FMC130M_4CH_OPCODE_LEDS                 0
#define FMC130M_4CH_NAME_LEDS                   "leds"
#define FMC130M_4CH_OPCODE_PLL_FUNCTION         1
#define FMC130M_4CH_NAME_PLL_FUNCTION           "pll_function"
#define FMC130M_4CH_OPCODE_AD9510_CFG_TEST      2
#define FMC130M_4CH_NAME_AD9510_CFG_TEST        "ad9510_cfg_test"
#define FMC130M_4CH_OPCODE_END                  3

/* Messaging Reply OPCODES */
#define FMC130M_4CH_REPLY_SIZE          (sizeof(uint32_t))
#define FMC130M_4CH_REPLY_TYPE          uint32_t

#define FMC130M_4CH_OK                  0   /* Operation was successful */
#define FMC130M_4CH_REPLY_END           1   /* End marker */

#endif
