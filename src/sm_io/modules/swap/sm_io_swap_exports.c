/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "sm_io_exports_helper.h"
#include "sm_io_codes.h"

/* Description SMIO SWAP functions */

disp_op_t swap_set_get_sw_exp = {
    .name = SWAP_NAME_SET_GET_SW,
    .opcode = SWAP_OPCODE_SET_GET_SW,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t swap_set_get_div_clk_exp = {
    .name = SWAP_NAME_SET_GET_DIV_CLK,
    .opcode = SWAP_OPCODE_SET_GET_DIV_CLK,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t swap_set_get_sw_dly_exp = {
    .name = SWAP_NAME_SET_GET_SW_DLY,
    .opcode = SWAP_OPCODE_SET_GET_SW_DLY,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

/* Exported function description */
const disp_op_t *swap_exp_ops [] = {
    &swap_set_get_sw_exp,
    &swap_set_get_div_clk_exp,
    &swap_set_get_sw_dly_exp,
    NULL
};
