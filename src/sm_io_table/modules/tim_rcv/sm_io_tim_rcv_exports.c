/*
 * Copyright (C) 2017 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "sm_io_codes.h"
#include "sm_io_exports_helper.h"

/* Description SMIO TIM_RCV functions */

disp_op_t tim_rcv_phase_meas_navg_exp = {
    .name = TIM_RCV_NAME_PHASE_MEAS_NAVG,
    .opcode = TIM_RCV_OPCODE_PHASE_MEAS_NAVG,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t tim_rcv_dmtd_a_deglitcher_thres_exp = {
    .name = TIM_RCV_NAME_DMTD_A_DEGLITCHER_THRES,
    .opcode = TIM_RCV_OPCODE_DMTD_A_DEGLITCHER_THRES,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t tim_rcv_dmtd_b_deglitcher_thres_exp = {
    .name = TIM_RCV_NAME_DMTD_B_DEGLITCHER_THRES,
    .opcode = TIM_RCV_OPCODE_DMTD_B_DEGLITCHER_THRES,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t tim_rcv_phase_meas_exp = {
    .name = TIM_RCV_NAME_PHASE_MEAS,
    .opcode = TIM_RCV_OPCODE_PHASE_MEAS,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t tim_rcv_dmtd_a_freq_exp = {
    .name = TIM_RCV_NAME_DMTD_A_FREQ,
    .opcode = TIM_RCV_OPCODE_DMTD_A_FREQ,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t tim_rcv_dmtd_a_valid_exp = {
    .name = TIM_RCV_NAME_DMTD_A_VALID,
    .opcode = TIM_RCV_OPCODE_DMTD_A_VALID,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t tim_rcv_dmtd_b_freq_exp = {
    .name = TIM_RCV_NAME_DMTD_B_FREQ,
    .opcode = TIM_RCV_OPCODE_DMTD_B_FREQ,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t tim_rcv_dmtd_b_valid_exp = {
    .name = TIM_RCV_NAME_DMTD_B_VALID,
    .opcode = TIM_RCV_OPCODE_DMTD_B_VALID,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

/* Exported function description */
const disp_op_t *tim_rcv_exp_ops [] = {
    &tim_rcv_phase_meas_navg_exp,
    &tim_rcv_dmtd_a_deglitcher_thres_exp,
    &tim_rcv_dmtd_b_deglitcher_thres_exp,
    &tim_rcv_phase_meas_exp,
    &tim_rcv_dmtd_a_freq_exp,
    &tim_rcv_dmtd_a_valid_exp,
    &tim_rcv_dmtd_b_freq_exp,
    &tim_rcv_dmtd_b_valid_exp,
    NULL
};

