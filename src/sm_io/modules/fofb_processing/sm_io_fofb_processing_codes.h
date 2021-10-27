/*
 * Copyright (C) 2020 LNLS (www.lnls.br)
 * Author: Melissa Aguiar <melissa.aguiar@cnpem.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _SM_IO_FOFB_PROCESSING_CODES_H_
#define _SM_IO_FOFB_PROCESSING_CODES_H_

/* Messaging OPCODES */
#define FOFB_PROCESSING_OPCODE_TYPE                     uint32_t
#define FOFB_PROCESSING_OPCODE_SIZE                     (sizeof (FOFB_PROCESSING_OPCODE_TYPE))

#define FOFB_PROCESSING_OPCODE_SET_GET_RAM_WRITE        0
#define FOFB_PROCESSING_NAME_SET_GET_RAM_WRITE          "fofb_processing_set_get_ram_write"
#define FOFB_PROCESSING_OPCODE_SET_GET_RAM_ADDR         1
#define FOFB_PROCESSING_NAME_SET_GET_RAM_ADDR           "fofb_processing_set_get_ram_addr"
#define FOFB_PROCESSING_OPCODE_SET_GET_RAM_DATA         2
#define FOFB_PROCESSING_NAME_SET_GET_RAM_DATA           "fofb_processing_set_get_ram_data"

/* For compatibility with older API. Don't remove these without changing libclient on clients */
#define FOFB_PROCESSING_OPCODE_SET_GET_INTLK_BIGGER         FOFB_PROCESSING_OPCODE_SET_GET_INTLK
#define FOFB_PROCESSING_NAME_SET_GET_INTLK_BIGGER           FOFB_PROCESSING_NAME_SET_GET_INTLK
#define FOFB_PROCESSING_OPCODE_SET_GET_INTLK_BIGGER_LTC     FOFB_PROCESSING_OPCODE_SET_GET_INTLK_LTC
#define FOFB_PROCESSING_NAME_SET_GET_INTLK_BIGGER_LTC       FOFB_PROCESSING_NAME_SET_GET_INTLK_LTC

/* Messaging Reply OPCODES */
#define FOFB_PROCESSING_REPLY_TYPE                      uint32_t
#define FOFB_PROCESSING_REPLY_SIZE                      (sizeof (FOFB_PROCESSING_REPLY_TYPE))

#define FOFB_PROCESSING_OK                              0   /* Operation was successful */
#define FOFB_PROCESSING_ERR                             1   /* Generic error */

#endif
