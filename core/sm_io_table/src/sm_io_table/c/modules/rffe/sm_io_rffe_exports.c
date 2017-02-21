/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "sm_io_exports_helper.h"
#include "sm_io_codes.h"

disp_op_t rffe_set_get_att_exp = {
    .name = RFFE_NAME_SET_GET_ATT,
    .opcode = RFFE_OPCODE_SET_GET_ATT,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
        DISP_ARG_END
    }
};

disp_op_t rffe_set_get_temp_ac_exp = {
    .name = RFFE_NAME_SET_GET_TEMP_AC,
    .opcode = RFFE_OPCODE_SET_GET_TEMP_AC,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
        DISP_ARG_END
    }
};

disp_op_t rffe_set_get_temp_bd_exp = {
    .name = RFFE_NAME_SET_GET_TEMP_BD,
    .opcode = RFFE_OPCODE_SET_GET_TEMP_BD,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
        DISP_ARG_END
    }
};

disp_op_t rffe_set_get_set_point_ac_exp = {
    .name = RFFE_NAME_SET_GET_SET_POINT_AC,
    .opcode = RFFE_OPCODE_SET_GET_SET_POINT_AC,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
        DISP_ARG_END
    }
};

disp_op_t rffe_set_get_set_point_bd_exp = {
    .name = RFFE_NAME_SET_GET_SET_POINT_BD,
    .opcode = RFFE_OPCODE_SET_GET_SET_POINT_BD,
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
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT8, uint8_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint8_t),
        DISP_ARG_END
    }
};

disp_op_t rffe_set_get_heater_ac_exp = {
    .name = RFFE_NAME_SET_GET_HEATER_AC,
    .opcode = RFFE_OPCODE_SET_GET_HEATER_AC,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
        DISP_ARG_END
    }
};

disp_op_t rffe_set_get_heater_bd_exp = {
    .name = RFFE_NAME_SET_GET_HEATER_BD,
    .opcode = RFFE_OPCODE_SET_GET_HEATER_BD,
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
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT8, uint8_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT8, uint8_t),
        DISP_ARG_END
    }
};

disp_op_t rffe_set_get_reprog_exp = {
    .name = RFFE_NAME_SET_GET_REPROG,
    .opcode = RFFE_OPCODE_SET_GET_REPROG,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT8, uint8_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT8, uint8_t),
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

disp_op_t rffe_set_get_pid_ac_kp_exp = {
    .name = RFFE_NAME_SET_GET_PID_AC_KP,
    .opcode = RFFE_OPCODE_SET_GET_PID_AC_KP,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
        DISP_ARG_END
    }
};

disp_op_t rffe_set_get_pid_ac_ti_exp = {
    .name = RFFE_NAME_SET_GET_PID_AC_TI,
    .opcode = RFFE_OPCODE_SET_GET_PID_AC_TI,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
        DISP_ARG_END
    }
};

disp_op_t rffe_set_get_pid_ac_td_exp = {
    .name = RFFE_NAME_SET_GET_PID_AC_TD,
    .opcode = RFFE_OPCODE_SET_GET_PID_AC_TD,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
        DISP_ARG_END
    }
};

disp_op_t rffe_set_get_pid_bd_kp_exp = {
    .name = RFFE_NAME_SET_GET_PID_BD_KP,
    .opcode = RFFE_OPCODE_SET_GET_PID_BD_KP,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
        DISP_ARG_END
    }
};

disp_op_t rffe_set_get_pid_bd_ti_exp = {
    .name = RFFE_NAME_SET_GET_PID_BD_TI,
    .opcode = RFFE_OPCODE_SET_GET_PID_BD_TI,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
        DISP_ARG_END
    }
};

disp_op_t rffe_set_get_pid_bd_td_exp = {
    .name = RFFE_NAME_SET_GET_PID_BD_TD,
    .opcode = RFFE_OPCODE_SET_GET_PID_BD_TD,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
        DISP_ARG_END
    }
};


/* Exported function description */
const disp_op_t *rffe_exp_ops [] = {
    &rffe_set_get_att_exp,
    &rffe_set_get_temp_ac_exp,
    &rffe_set_get_temp_bd_exp,
    &rffe_set_get_set_point_ac_exp,
    &rffe_set_get_set_point_bd_exp,
    &rffe_set_get_temp_control_exp,
    &rffe_set_get_heater_ac_exp,
    &rffe_set_get_heater_bd_exp,
    &rffe_set_get_reset_exp,
    &rffe_set_get_reprog_exp,
    &rffe_set_get_data_exp,
    &rffe_set_get_version_exp,
    &rffe_set_get_pid_ac_kp_exp,
    &rffe_set_get_pid_ac_ti_exp,
    &rffe_set_get_pid_ac_td_exp,
    &rffe_set_get_pid_bd_kp_exp,
    &rffe_set_get_pid_bd_ti_exp,
    &rffe_set_get_pid_bd_td_exp,
    NULL
};

