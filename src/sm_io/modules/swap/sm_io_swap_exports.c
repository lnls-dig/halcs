/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
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

disp_op_t swap_set_get_sw_en_exp = {
    .name = SWAP_NAME_SET_GET_SW_EN,
    .opcode = SWAP_OPCODE_SET_GET_SW_EN,
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

disp_op_t swap_set_get_wdw_en_exp = {
    .name = SWAP_NAME_SET_GET_WDW_EN,
    .opcode = SWAP_OPCODE_SET_GET_WDW_EN,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t swap_set_get_wdw_dly_exp = {
    .name = SWAP_NAME_SET_GET_WDW_DLY,
    .opcode = SWAP_OPCODE_SET_GET_WDW_DLY,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t swap_set_get_gain_a_exp = {
    .name = SWAP_NAME_SET_GET_GAIN_A,
    .opcode = SWAP_OPCODE_SET_GET_GAIN_A,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t swap_set_get_gain_b_exp = {
    .name = SWAP_NAME_SET_GET_GAIN_B,
    .opcode = SWAP_OPCODE_SET_GET_GAIN_B,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t swap_set_get_gain_c_exp = {
    .name = SWAP_NAME_SET_GET_GAIN_C,
    .opcode = SWAP_OPCODE_SET_GET_GAIN_C,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t swap_set_get_gain_d_exp = {
    .name = SWAP_NAME_SET_GET_GAIN_D,
    .opcode = SWAP_OPCODE_SET_GET_GAIN_D,
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
    &swap_set_get_sw_en_exp,
    &swap_set_get_div_clk_exp,
    &swap_set_get_sw_dly_exp,
    &swap_set_get_wdw_en_exp,
    &swap_set_get_wdw_dly_exp,
    &swap_set_get_gain_a_exp,
    &swap_set_get_gain_b_exp,
    &swap_set_get_gain_c_exp,
    &swap_set_get_gain_d_exp,
    NULL
};
