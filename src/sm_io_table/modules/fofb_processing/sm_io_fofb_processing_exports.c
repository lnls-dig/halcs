/*
** Copyright (C) 2020 LNLS (www.lnls.br)
** Author: Melissa Aguiar <melissa.aguiar@cnpem.br>
**
** Released according to the GNU GPL, version 3 or any later version.
**/

#include "sm_io_exports_helper.h"
#include "sm_io_codes.h"

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

disp_op_t fofb_processing_set_get_acc_gains_fixed_point_pos_exp = {
    .name = FOFB_PROCESSING_NAME_SET_GET_ACC_GAINS_FIXED_POINT_POS,
    .opcode = FOFB_PROCESSING_OPCODE_SET_GET_ACC_GAINS_FIXED_POINT_POS,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_processing_set_get_acc_gain_exp = {
    .name = FOFB_PROCESSING_NAME_SET_GET_ACC_GAIN,
    .opcode = FOFB_PROCESSING_OPCODE_SET_GET_ACC_GAIN,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_processing_set_get_acc_ctl_clear_exp = {
    .name = FOFB_PROCESSING_NAME_SET_GET_ACC_CTL_CLEAR,
    .opcode = FOFB_PROCESSING_OPCODE_SET_GET_ACC_CTL_CLEAR,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_processing_set_get_acc_ctl_freeze_exp = {
    .name = FOFB_PROCESSING_NAME_SET_GET_ACC_CTL_FREEZE,
    .opcode = FOFB_PROCESSING_OPCODE_SET_GET_ACC_CTL_FREEZE,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_processing_set_get_sp_max_exp = {
    .name = FOFB_PROCESSING_NAME_SET_GET_SP_MAX,
    .opcode = FOFB_PROCESSING_OPCODE_SET_GET_SP_MAX,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};
disp_op_t fofb_processing_set_get_sp_min_exp = {
    .name = FOFB_PROCESSING_NAME_SET_GET_SP_MIN,
    .opcode = FOFB_PROCESSING_OPCODE_SET_GET_SP_MIN,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

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
    .retval = DISP_ARG_END,
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE_RAW(DISP_ATYPE_STRUCT,
            sizeof(smio_fofb_processing_data_block_t)),
        DISP_ARG_END
    }
};

disp_op_t fofb_processing_setpoints_ram_bank_read_exp = {
    .name = FOFB_PROCESSING_NAME_SETPOINTS_RAM_BANK_READ,
    .opcode = FOFB_PROCESSING_OPCODE_SETPOINTS_RAM_BANK_READ,
    .retval = DISP_ARG_ENCODE_RAW(DISP_ATYPE_STRUCT,
        sizeof(smio_fofb_processing_data_block_t)),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_END
    }
};

disp_op_t fofb_processing_setpoints_ram_bank_write_exp = {
    .name = FOFB_PROCESSING_NAME_SETPOINTS_RAM_BANK_WRITE,
    .opcode = FOFB_PROCESSING_OPCODE_SETPOINTS_RAM_BANK_WRITE,
    .retval = DISP_ARG_END,
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE_RAW(DISP_ATYPE_STRUCT,
            sizeof(smio_fofb_processing_data_block_t)),
        DISP_ARG_END
    }
};

/* Exported function description */
const disp_op_t *fofb_processing_exp_ops [] = {
    &fofb_processing_set_get_coeffs_fixed_point_pos_exp,
    &fofb_processing_set_get_acc_gains_fixed_point_pos_exp,
    &fofb_processing_set_get_acc_gain_exp,
    &fofb_processing_set_get_acc_ctl_clear_exp,
    &fofb_processing_set_get_acc_ctl_freeze_exp,
    &fofb_processing_set_get_sp_max_exp,
    &fofb_processing_set_get_sp_min_exp,
    &fofb_processing_coeffs_ram_bank_read_exp,
    &fofb_processing_coeffs_ram_bank_write_exp,
    &fofb_processing_setpoints_ram_bank_read_exp,
    &fofb_processing_setpoints_ram_bank_write_exp,
    NULL
};
