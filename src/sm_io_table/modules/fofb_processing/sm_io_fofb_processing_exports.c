/*
** Copyright (C) 2020 LNLS (www.lnls.br)
** Author: Melissa Aguiar <melissa.aguiar@cnpem.br>
**
** Released according to the GNU GPL, version 3 or any later version.
**/

#include "sm_io_exports_helper.h"
#include "sm_io_codes.h"

/* Description SMIO FOFB_PROCESSING functions */
disp_op_t fofb_processing_coeffs_ram_bank_read_exp = {
    .name = FOFB_PROCESSING_NAME_COEFFS_RAM_BANK_READ,
    .opcode = FOFB_PROCESSING_OPCODE_COEFFS_RAM_BANK_READ,
    .retval = DISP_ARG_ENCODE_RAW(DISP_ATYPE_STRUCT,
        sizeof(smio_fofb_processing_data_block_t)),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_processing_coeffs_ram_bank_write_exp = {
    .name = FOFB_PROCESSING_NAME_COEFFS_RAM_BANK_WRITE,
    .opcode = FOFB_PROCESSING_OPCODE_COEFFS_RAM_BANK_WRITE,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE_RAW(DISP_ATYPE_STRUCT,
            sizeof(smio_fofb_processing_data_block_t)),
        DISP_ARG_END
    }
};

disp_op_t fofb_processing_set_get_coeffs_fixed_point_pos_exp = {
    .name = FOFB_PROCESSING_NAME_SET_GET_COEFFS_FIXED_POINT_POS,
    .opcode = FOFB_PROCESSING_OPCODE_SET_GET_COEFFS_FIXED_POINT_POS,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

/* Exported function description */
const disp_op_t *fofb_processing_exp_ops [] = {
    &fofb_processing_coeffs_ram_bank_read_exp,
    &fofb_processing_coeffs_ram_bank_write_exp,
    &fofb_processing_set_get_coeffs_fixed_point_pos_exp,
    NULL
};
