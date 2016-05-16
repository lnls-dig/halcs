/*
 * Copyright (C) 2016 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _SM_IO_TRIGGER_IFACE_CODES_H_
#define _SM_IO_TRIGGER_IFACE_CODES_H_

/* Messaging OPCODES */
#define TRIGGER_IFACE_OPCODE_TYPE                           uint32_t
#define TRIGGER_IFACE_OPCODE_SIZE                           (sizeof (TRIGGER_IFACE_OPCODE_TYPE))

#define TRIGGER_IFACE_OPCODE_DIR                            0
#define TRIGGER_IFACE_NAME_DIR                              "trigger_iface_dir"
#define TRIGGER_IFACE_OPCODE_RCV_COUNT_RST                  1
#define TRIGGER_IFACE_NAME_RCV_COUNT_RST                    "trigger_iface_rcv_count_rst"
#define TRIGGER_IFACE_OPCODE_TRANSM_COUNT_RST               2
#define TRIGGER_IFACE_NAME_TRANSM_COUNT_RST                 "trigger_iface_transm_count_rst"
#define TRIGGER_IFACE_OPCODE_RCV_LEN                        3
#define TRIGGER_IFACE_NAME_RCV_LEN                          "trigger_iface_rcv_len"
#define TRIGGER_IFACE_OPCODE_TRANSM_LEN                     4
#define TRIGGER_IFACE_NAME_TRANSM_LEN                       "trigger_iface_transm_len"
#define TRIGGER_IFACE_OPCODE_COUNT_RCV                      5
#define TRIGGER_IFACE_NAME_COUNT_RCV                        "trigger_iface_count_rcv"
#define TRIGGER_IFACE_OPCODE_COUNT_TRANSM                   6
#define TRIGGER_IFACE_NAME_COUNT_TRANSM                     "trigger_iface_count_transm"
#define TRIGGER_IFACE_OPCODE_END                            7

/* Messaging Reply OPCODES */
#define TRIGGER_IFACE_REPLY_TYPE                            uint32_t
#define TRIGGER_IFACE_REPLY_SIZE                            (sizeof (TRIGGER_IFACE_REPLY_TYPE))

#define TRIGGER_IFACE_OK                                    0   /* Operation was successful */
#define TRIGGER_IFACE_ERR                                   1   /* Could not set/get value */
#define TRIGGER_IFACE_UNINPL                                2   /* Unimplemented function or operation */
#define TRIGGER_IFACE_REPLY_END                             3   /* End marker */

#endif
