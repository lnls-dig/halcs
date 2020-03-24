/*
 * Copyright (C) 2017 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _SM_IO_AFC_MGMT_CODES_H_
#define _SM_IO_AFC_MGMT_CODES_H_

/* Messaging OPCODES */
#define AFC_MGMT_OPCODE_TYPE                         uint32_t
#define AFC_MGMT_OPCODE_SIZE                         (sizeof (AFC_MGMT_OPCODE_TYPE))

/* FIXME: Reuse FMC_ACTIVE_CLOCK codes so we can use the same
 * client functions */
#define AFC_MGMT_OPCODE_SI57X_OE                     0
#define AFC_MGMT_NAME_SI57X_OE                       "fmc_active_clk_si57x_oe"
#define AFC_MGMT_OPCODE_SI57X_FREQ                   14
#define AFC_MGMT_NAME_SI57X_FREQ                     "fmc_active_clk_si57x_freq"
#define AFC_MGMT_OPCODE_SI57X_GET_DEFAULTS           15
#define AFC_MGMT_NAME_SI57X_GET_DEFAULTS             "fmc_active_clk_si57x_get_defaults"
#define AFC_MGMT_OPCODE_SI57X_FSTARTUP               19
#define AFC_MGMT_NAME_SI57X_FSTARTUP                 "fmc_active_clk_si57x_fstartup"
#define AFC_MGMT_OPCODE_END                          20

/* Messaging Reply OPCODES */
#define AFC_MGMT_REPLY_TYPE                          uint32_t
#define AFC_MGMT_REPLY_SIZE                          (sizeof (AFC_MGMT_REPLY_TYPE))

#define AFC_MGMT_OK                                  0   /* Operation was successful */
#define AFC_MGMT_ERR                                 1   /* Could not set/get value */
#define AFC_MGMT_UNINPL                              2   /* Unimplemented function or operation */
#define AFC_MGMT_REPLY_END                           3   /* End marker */

#endif
