/*
 *  * Copyright (C) 2020 LNLS (www.lnls.br)
 *   * Author: Melissa Aguiar <melissa.aguiar@cnpem.br>
 *    *
 *     * Released according to the GNU GPL, version 3 or any later version.
 *      */

#include "sm_io_exports_helper.h"
#include "sm_io_codes.h"

/* Description SMIO FOFB_PROCESSING functions */


disp_op_t fofb_processing_coeff_ram_bank_write_exp = {
    .name = FOFB_PROCESSING_NAME_COEFF_RAM_BANK_WRITE,
    .opcode = FOFB_PROCESSING_OPCODE_COEFF_RAM_BANK_WRITE,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE_RAW(DISP_ATYPE_STRUCT,
            sizeof(smio_fofb_processing_data_block_t)),
        DISP_ARG_END
    }
};

/* Exported function description */
const disp_op_t *fofb_processing_exp_ops [] = {
    &fofb_processing_coeff_ram_bank_write_exp,
    NULL
};
