/*
 *  * Copyright (C) 2020 LNLS (www.lnls.br)
 *   * Author: Melissa Aguiar <melissa.aguiar@cnpem.br>
 *    *
 *     * Released according to the GNU GPL, version 3 or any later version.
 *      */

#ifndef _SM_IO_FOFB_PROCESSING_CODES_H_
#define _SM_IO_FOFB_PROCESSING_CODES_H_

/* Messaging OPCODES */
#define FOFB_PROCESSING_OPCODE_TYPE                     uint32_t
#define FOFB_PROCESSING_OPCODE_SIZE                     (sizeof (FOFB_PROCESSING_OPCODE_TYPE))

#define FOFB_PROCESSING_OPCODE_SET_GET_RAM_WRITE_ENABLE     0
#define FOFB_PROCESSING_NAME_SET_GET_RAM_WRITE_ENABLE       "fofb_processing_set_get_ram_write_enable"
#define FOFB_PROCESSING_OPCODE_SET_GET_RAM_ADDR_VAL         1
#define FOFB_PROCESSING_NAME_SET_GET_RAM_ADDR_VAL           "fofb_processing_set_get_ram_addr_val"
#define FOFB_PROCESSING_OPCODE_SET_GET_RAM_DATA_IN_VAL      2
#define FOFB_PROCESSING_NAME_SET_GET_RAM_DATA_IN_VAL        "fofb_processing_set_get_ram_data_in_val"
#define FOFB_PROCESSING_OPCODE_SET_GET_RAM_DATA_OUT_VAL     3
#define FOFB_PROCESSING_NAME_SET_GET_RAM_DATA_OUT_VAL       "fofb_processing_set_get_ram_data_out_val"

/* Messaging Reply OPCODES */
#define FOFB_PROCESSING_REPLY_TYPE                      uint32_t
#define FOFB_PROCESSING_REPLY_SIZE                      (sizeof (FOFB_PROCESSING_REPLY_TYPE))

#define FOFB_PROCESSING_OK                              0   /* Operation was successful */
#define FOFB_PROCESSING_ERR                             1   /* Generic error */

#endif

