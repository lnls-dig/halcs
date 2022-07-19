/*
 * Copyright (C) 2022 LNLS (www.lnls.br)
 * Author: Érico Nogueira <erico.rolim@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "sm_io_exports_helper.h"
#include "sm_io_codes.h"

disp_op_t clk_cnt_set_get_trig_act_exp = {
    .name = CLK_CNT_NAME_SET_GET_TRIG_ACT,
    .opcode = CLK_CNT_OPCODE_SET_GET_TRIG_ACT,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t clk_cnt_set_get_cnt_snap_exp = {
    .name = CLK_CNT_NAME_SET_GET_CNT_SNAP,
    .opcode = CLK_CNT_OPCODE_SET_GET_CNT_SNAP,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

/* Exported function description */
const disp_op_t *clk_cnt_exp_ops [] = {
    &clk_cnt_set_get_trig_act_exp,
    &clk_cnt_set_get_cnt_snap_exp,
    NULL
};
