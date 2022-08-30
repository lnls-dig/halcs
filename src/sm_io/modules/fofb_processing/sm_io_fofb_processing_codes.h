/*
 *  * Copyright (C) 2020 LNLS (www.lnls.br)
 *   * Author: Melissa Aguiar <melissa.aguiar@cnpem.br>
 *    *
 *     * Released according to the GNU GPL, version 3 or any later version.
 *      */

#ifndef _SM_IO_FOFB_PROCESSING_CODES_H_
#define _SM_IO_FOFB_PROCESSING_CODES_H_

/* NOTE: This value should be FOFB_PROCESSING_REGS_COEFFS_RAM_BANK_SIZE/sizeof(uint32_t),
 *       where FOFB_PROCESSING_REGS_COEFFS_RAM_BANK_SIZE is defined on hw/wb_fofb_processing_regs.h
 */
#define FOFB_PROCESSING_DATA_BLOCK_MAX_PARAMS                   512
struct _smio_fofb_processing_data_block_t {
    uint32_t data[FOFB_PROCESSING_DATA_BLOCK_MAX_PARAMS];
};

/* Messaging OPCODES */
#define FOFB_PROCESSING_OPCODE_TYPE                             uint32_t
#define FOFB_PROCESSING_OPCODE_SIZE                             (sizeof (FOFB_PROCESSING_OPCODE_TYPE))

#define FOFB_PROCESSING_OPCODE_COEFFS_RAM_BANK_READ             0
#define FOFB_PROCESSING_NAME_COEFFS_RAM_BANK_READ               "_fofb_processing_coeffs_ram_bank_read"
#define FOFB_PROCESSING_OPCODE_COEFFS_RAM_BANK_WRITE            1
#define FOFB_PROCESSING_NAME_COEFFS_RAM_BANK_WRITE              "_fofb_processing_coeffs_ram_bank_write"
#define FOFB_PROCESSING_OPCODE_SET_GET_COEFFS_FIXED_POINT_POS   2
#define FOFB_PROCESSING_NAME_SET_GET_COEFFS_FIXED_POINT_POS     "fofb_processing_set_get_coeffs_fixed_point_pos"

/* Messaging Reply OPCODES */
#define FOFB_PROCESSING_REPLY_TYPE                              uint32_t
#define FOFB_PROCESSING_REPLY_SIZE                              (sizeof (FOFB_PROCESSING_REPLY_TYPE))

#define FOFB_PROCESSING_OK                                      0   /* Operation was successful */
#define FOFB_PROCESSING_ERR                                     1   /* Generic error */

#endif

