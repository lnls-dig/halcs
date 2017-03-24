/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "sm_io_exports_helper.h"
#include "sm_io_codes.h"

/* Description SMIO ACQ functions */

disp_op_t acq_data_acquire_exp = {
    .name = ACQ_NAME_DATA_ACQUIRE,
    .opcode = ACQ_OPCODE_DATA_ACQUIRE,
    .retval = DISP_ARG_END,
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t acq_check_data_acquire_exp = {
    .name = ACQ_NAME_CHECK_DATA_ACQUIRE,
    .opcode = ACQ_OPCODE_CHECK_DATA_ACQUIRE,
    .retval = DISP_ARG_END,
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_END
    }
};

disp_op_t acq_get_data_block_exp = {
    .name = ACQ_NAME_GET_DATA_BLOCK,
    .opcode = ACQ_OPCODE_GET_DATA_BLOCK,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_STRUCT, smio_acq_data_block_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t acq_cfg_trig_exp = {
    .name = ACQ_NAME_CFG_TRIG,
    .opcode = ACQ_OPCODE_CFG_TRIG,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t acq_hw_data_trig_pol_exp = {
    .name = ACQ_NAME_HW_DATA_TRIG_POL,
    .opcode = ACQ_OPCODE_HW_DATA_TRIG_POL,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t acq_hw_data_trig_sel_exp = {
    .name = ACQ_NAME_HW_DATA_TRIG_SEL,
    .opcode = ACQ_OPCODE_HW_DATA_TRIG_SEL,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t acq_hw_data_trig_filt_exp = {
    .name = ACQ_NAME_HW_DATA_TRIG_FILT,
    .opcode = ACQ_OPCODE_HW_DATA_TRIG_FILT,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t acq_hw_data_trig_thres_exp = {
    .name = ACQ_NAME_HW_DATA_TRIG_THRES,
    .opcode = ACQ_OPCODE_HW_DATA_TRIG_THRES,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t acq_hw_trig_dly_exp = {
    .name = ACQ_NAME_HW_TRIG_DLY,
    .opcode = ACQ_OPCODE_HW_TRIG_DLY,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t acq_sw_trig_exp = {
    .name = ACQ_NAME_SW_TRIG,
    .opcode = ACQ_OPCODE_SW_TRIG,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t acq_fsm_stop_exp = {
    .name = ACQ_NAME_FSM_STOP,
    .opcode = ACQ_OPCODE_FSM_STOP,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t acq_hw_data_trig_chan_exp = {
    .name = ACQ_NAME_HW_DATA_TRIG_CHAN,
    .opcode = ACQ_OPCODE_HW_DATA_TRIG_CHAN,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t acq_ch_int_width_exp = {
    .name = ACQ_NAME_CH_INT_WIDTH,
    .opcode = ACQ_OPCODE_CH_INT_WIDTH,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t acq_ch_num_coalesce_exp = {
    .name = ACQ_NAME_CH_NUM_COALESCE,
    .opcode = ACQ_OPCODE_CH_NUM_COALESCE,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t acq_ch_num_atoms_exp = {
    .name = ACQ_NAME_CH_NUM_ATOMS,
    .opcode = ACQ_OPCODE_CH_NUM_ATOMS,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t acq_ch_atom_width_exp = {
    .name = ACQ_NAME_CH_ATOM_WIDTH,
    .opcode = ACQ_OPCODE_CH_ATOM_WIDTH,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

/* Exported function description */
const disp_op_t *acq_exp_ops [] = {
    &acq_data_acquire_exp,
    &acq_check_data_acquire_exp,
    &acq_get_data_block_exp,
    &acq_cfg_trig_exp,
    &acq_hw_data_trig_pol_exp,
    &acq_hw_data_trig_sel_exp,
    &acq_hw_data_trig_filt_exp,
    &acq_hw_data_trig_thres_exp,
    &acq_hw_trig_dly_exp,
    &acq_sw_trig_exp,
    &acq_fsm_stop_exp,
    &acq_hw_data_trig_chan_exp,
    &acq_ch_int_width_exp,
    &acq_ch_num_coalesce_exp,
    &acq_ch_num_atoms_exp,
    &acq_ch_atom_width_exp,
    NULL
};

