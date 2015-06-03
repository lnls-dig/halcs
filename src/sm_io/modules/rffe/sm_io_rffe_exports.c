/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include "sm_io_exports_helper.h"
#include "sm_io_rffe_exports.h"
#include "sm_io_rffe_codes.h"

/* Description SMIO RFFE functions */

disp_op_t rffe_set_get_sw_exp = {
    .name = RFFE_NAME_SET_GET_SW,
    .opcode = RFFE_OPCODE_SET_GET_SW,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t rffe_set_get_att1_exp = {
    .name = RFFE_NAME_SET_GET_ATT1,
    .opcode = RFFE_OPCODE_SET_GET_ATT1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
        DISP_ARG_END
    }
};

disp_op_t rffe_set_get_att2_exp = {
    .name = RFFE_NAME_SET_GET_ATT2,
    .opcode = RFFE_OPCODE_SET_GET_ATT2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
        DISP_ARG_END
    }
};

disp_op_t rffe_set_get_temp1_exp = {
    .name = RFFE_NAME_SET_GET_TEMP1,
    .opcode = RFFE_OPCODE_SET_GET_TEMP1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
        DISP_ARG_END
    }
};

disp_op_t rffe_set_get_temp2_exp = {
    .name = RFFE_NAME_SET_GET_TEMP2,
    .opcode = RFFE_OPCODE_SET_GET_TEMP2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
        DISP_ARG_END
    }
};

disp_op_t rffe_set_get_temp3_exp = {
    .name = RFFE_NAME_SET_GET_TEMP3,
    .opcode = RFFE_OPCODE_SET_GET_TEMP3,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
        DISP_ARG_END
    }
};

disp_op_t rffe_set_get_temp4_exp = {
    .name = RFFE_NAME_SET_GET_TEMP4,
    .opcode = RFFE_OPCODE_SET_GET_TEMP4,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
        DISP_ARG_END
    }
};

disp_op_t rffe_set_get_set_point1_exp = {
    .name = RFFE_NAME_SET_GET_SET_POINT1,
    .opcode = RFFE_OPCODE_SET_GET_SET_POINT1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
        DISP_ARG_END
    }
};

disp_op_t rffe_set_get_set_point2_exp = {
    .name = RFFE_NAME_SET_GET_SET_POINT2,
    .opcode = RFFE_OPCODE_SET_GET_SET_POINT2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
        DISP_ARG_END
    }
};

disp_op_t rffe_set_get_temp_control_exp = {
    .name = RFFE_NAME_SET_GET_TEMP_CONTROL,
    .opcode = RFFE_OPCODE_SET_GET_TEMP_CONTROL,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t rffe_set_get_output1_exp = {
    .name = RFFE_NAME_SET_GET_OUTPUT1,
    .opcode = RFFE_OPCODE_SET_GET_OUTPUT1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
        DISP_ARG_END
    }
};

disp_op_t rffe_set_get_output2_exp = {
    .name = RFFE_NAME_SET_GET_OUTPUT2,
    .opcode = RFFE_OPCODE_SET_GET_OUTPUT2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
        DISP_ARG_END
    }
};

disp_op_t rffe_set_get_reset_exp = {
    .name = RFFE_NAME_SET_GET_RESET,
    .opcode = RFFE_OPCODE_SET_GET_RESET,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t rffe_set_get_reprog_exp = {
    .name = RFFE_NAME_SET_GET_REPROG,
    .opcode = RFFE_OPCODE_SET_GET_REPROG,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t rffe_set_get_data_exp = {
    .name = RFFE_NAME_SET_GET_DATA,
    .opcode = RFFE_OPCODE_SET_GET_DATA,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_STRUCT, smio_rffe_data_block_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_STRUCT, smio_rffe_data_block_t),
        DISP_ARG_END
    }
};

disp_op_t rffe_set_get_version_exp = {
    .name = RFFE_NAME_SET_GET_VERSION,
    .opcode = RFFE_OPCODE_SET_GET_VERSION,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_STRUCT, smio_rffe_version_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_STRUCT, smio_rffe_version_t),
        DISP_ARG_END
    }
};

disp_op_t rffe_set_get_sw_lvl_exp = {
    .name = RFFE_NAME_SET_GET_SW_LVL,
    .opcode = RFFE_OPCODE_SET_GET_SW_LVL,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

/* Exported function description */
const disp_op_t *rffe_exp_ops [] = {
    &rffe_set_get_sw_exp,
    &rffe_set_get_att1_exp,
    &rffe_set_get_att2_exp,
    &rffe_set_get_temp1_exp,
    &rffe_set_get_temp2_exp,
    &rffe_set_get_temp3_exp,
    &rffe_set_get_temp4_exp,
    &rffe_set_get_set_point1_exp,
    &rffe_set_get_set_point2_exp,
    &rffe_set_get_temp_control_exp,
    &rffe_set_get_output1_exp,
    &rffe_set_get_output2_exp,
    &rffe_set_get_reset_exp,
    &rffe_set_get_reprog_exp,
    &rffe_set_get_data_exp,
    &rffe_set_get_version_exp,
    &rffe_set_get_sw_lvl_exp,
    NULL
};

