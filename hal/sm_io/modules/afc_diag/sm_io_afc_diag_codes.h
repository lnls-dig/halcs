/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _SM_IO_AFC_DIAG_CODES_H_
#define _SM_IO_AFC_DIAG_CODES_H_

#include <inttypes.h>

/* Messaging OPCODES */
#define AFC_DIAG_OPCODE_SIZE                        (sizeof(uint32_t))
#define AFC_DIAG_OPCODE_TYPE                        uint32_t

#define AFC_DIAG_OPCODE_SET_GET_CARD_SLOT           0
#define AFC_DIAG_NAME_SET_GET_CARD_SLOT             "afc_diag_card_slot"
#define AFC_DIAG_OPCODE_SET_GET_IPMI_ADDR           1
#define AFC_DIAG_NAME_SET_GET_IPMI_ADDR             "afc_diag_ipmi_addr"
#define AFC_DIAG_OPCODE_END                         2

#define AFC_DIAG_OK                                 0   /* Operation was successful */
#define AFC_DIAG_ERR                                1   /* Could not set/get value */
#define AFC_DIAG_UNINPL                             2   /* Unimplemented function or operation */
#define AFC_DIAG_REPLY_END                          3   /* End marker */

#endif

