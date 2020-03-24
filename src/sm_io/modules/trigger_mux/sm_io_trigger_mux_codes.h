/*
 * Copyright (C) 2016 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _SM_IO_TRIGGER_MUX_CODES_H_
#define _SM_IO_TRIGGER_MUX_CODES_H_

/* Messaging OPCODES */
#define TRIGGER_MUX_OPCODE_TYPE                           uint32_t
#define TRIGGER_MUX_OPCODE_SIZE                           (sizeof (TRIGGER_MUX_OPCODE_TYPE))

#define TRIGGER_MUX_OPCODE_RCV_SRC                        0
#define TRIGGER_MUX_NAME_RCV_SRC                          "trigger_mux_rcv_src"
#define TRIGGER_MUX_OPCODE_RCV_IN_SEL                     1
#define TRIGGER_MUX_NAME_RCV_IN_SEL                       "trigger_mux_rcv_in_sel"
#define TRIGGER_MUX_OPCODE_TRANSM_SRC                     2
#define TRIGGER_MUX_NAME_TRANSM_SRC                       "trigger_mux_transm_src"
#define TRIGGER_MUX_OPCODE_TRANSM_OUT_SEL                 3
#define TRIGGER_MUX_NAME_TRANSM_OUT_SEL                   "trigger_mux_transm_out_sel"
#define TRIGGER_MUX_OPCODE_END                            4

/* Messaging Reply OPCODES */
#define TRIGGER_MUX_REPLY_TYPE                            uint32_t
#define TRIGGER_MUX_REPLY_SIZE                            (sizeof (TRIGGER_MUX_REPLY_TYPE))

#define TRIGGER_MUX_OK                                    0   /* Operation was successful */
#define TRIGGER_MUX_ERR                                   1   /* Could not set/get value */
#define TRIGGER_MUX_UNINPL                                2   /* Unimplemented function or operation */
#define TRIGGER_MUX_REPLY_END                             3   /* End marker */

#endif
