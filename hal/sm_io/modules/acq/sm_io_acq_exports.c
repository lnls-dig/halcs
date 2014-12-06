/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include "sm_io_acq_exports.h"
#include "sm_io_acq_codes.h"

/* Description SMIO ACQ functions */

disp_op_t acq_data_acquire_exp = {
    .name = ACQ_NAME_DATA_ACQUIRE,
    .opcode = ACQ_OPCODE_DATA_ACQUIRE,
    .retval = DISP_ARG_END,
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
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

