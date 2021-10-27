/*
 * Copyright (C) 2020 LNLS (www.lnls.br)
 * Author: Melissa Aguiar <melissa.aguiar@cnpem.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "sm_io_exports_helper.h"
#include "sm_io_codes.h"

/* Description SMIO FOFB_PROCESSING functions */

disp_op_t fofb_processing_set_get_ram_write_exp = {
    .name = FOFB_PROCESSING_NAME_SET_GET_RAM_WRITE,
    .opcode = FOFB_PROCESSING_OPCODE_SET_GET_RAM_WRITE,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_processing_set_get_ram_addr_exp = {
    .name = FOFB_PROCESSING_NAME_SET_GET_RAM_ADDR,
    .opcode = FOFB_PROCESSING_OPCODE_SET_GET_RAM_ADDR,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_processing_set_get_ram_data_in_exp = {
    .name = FOFB_PROCESSING_NAME_SET_GET_RAM_DATA_IN,
    .opcode = FOFB_PROCESSING_OPCODE_SET_GET_RAM_DATA_IN,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_processing_set_get_ram_data_out_exp = {
    .name = FOFB_PROCESSING_NAME_SET_GET_RAM_DATA_OUT,
    .opcode = FOFB_PROCESSING_OPCODE_SET_GET_RAM_DATA_OUT,
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
    &fofb_processing_set_get_ram_write_exp,
    &fofb_processing_set_get_ram_addr_exp,
    &fofb_processing_set_get_ram_data_in_exp,
    &fofb_processing_set_get_ram_data_out_exp,
    NULL
};

