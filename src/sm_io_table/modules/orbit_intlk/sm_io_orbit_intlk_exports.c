/*
 * Copyright (C) 2020 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "sm_io_exports_helper.h"
#include "sm_io_codes.h"

/* Description SMIO ORBIT_INTLK functions */

disp_op_t orbit_intlk_set_get_en_exp = {
    .name = ORBIT_INTLK_NAME_SET_GET_EN,
    .opcode = ORBIT_INTLK_OPCODE_SET_GET_EN,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t orbit_intlk_set_get_clr_exp = {
    .name = ORBIT_INTLK_NAME_SET_GET_CLR,
    .opcode = ORBIT_INTLK_OPCODE_SET_GET_CLR,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t orbit_intlk_set_get_min_sum_en_exp = {
    .name = ORBIT_INTLK_NAME_SET_GET_MIN_SUM_EN,
    .opcode = ORBIT_INTLK_OPCODE_SET_GET_MIN_SUM_EN,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t orbit_intlk_set_get_trans_en_exp = {
    .name = ORBIT_INTLK_NAME_SET_GET_TRANS_EN,
    .opcode = ORBIT_INTLK_OPCODE_SET_GET_TRANS_EN,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t orbit_intlk_set_get_trans_clr_exp = {
    .name = ORBIT_INTLK_NAME_SET_GET_TRANS_CLR,
    .opcode = ORBIT_INTLK_OPCODE_SET_GET_TRANS_CLR,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t orbit_intlk_set_get_ang_en_exp = {
    .name = ORBIT_INTLK_NAME_SET_GET_ANG_EN,
    .opcode = ORBIT_INTLK_OPCODE_SET_GET_ANG_EN,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t orbit_intlk_set_get_ang_clr_exp = {
    .name = ORBIT_INTLK_NAME_SET_GET_ANG_CLR,
    .opcode = ORBIT_INTLK_OPCODE_SET_GET_ANG_CLR,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t orbit_intlk_set_get_trans_bigger_x_exp = {
    .name = ORBIT_INTLK_NAME_SET_GET_TRANS_BIGGER_X,
    .opcode = ORBIT_INTLK_OPCODE_SET_GET_TRANS_BIGGER_X,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t orbit_intlk_set_get_trans_bigger_y_exp = {
    .name = ORBIT_INTLK_NAME_SET_GET_TRANS_BIGGER_Y,
    .opcode = ORBIT_INTLK_OPCODE_SET_GET_TRANS_BIGGER_Y,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t orbit_intlk_set_get_trans_bigger_ltc_x_exp = {
    .name = ORBIT_INTLK_NAME_SET_GET_TRANS_BIGGER_LTC_X,
    .opcode = ORBIT_INTLK_OPCODE_SET_GET_TRANS_BIGGER_LTC_X,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t orbit_intlk_set_get_trans_bigger_ltc_y_exp = {
    .name = ORBIT_INTLK_NAME_SET_GET_TRANS_BIGGER_LTC_Y,
    .opcode = ORBIT_INTLK_OPCODE_SET_GET_TRANS_BIGGER_LTC_Y,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t orbit_intlk_set_get_trans_bigger_any_exp = {
    .name = ORBIT_INTLK_NAME_SET_GET_TRANS_BIGGER_ANY,
    .opcode = ORBIT_INTLK_OPCODE_SET_GET_TRANS_BIGGER_ANY,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t orbit_intlk_set_get_trans_bigger_exp = {
    .name = ORBIT_INTLK_NAME_SET_GET_TRANS_BIGGER,
    .opcode = ORBIT_INTLK_OPCODE_SET_GET_TRANS_BIGGER,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t orbit_intlk_set_get_trans_bigger_ltc_exp = {
    .name = ORBIT_INTLK_NAME_SET_GET_TRANS_BIGGER_LTC,
    .opcode = ORBIT_INTLK_OPCODE_SET_GET_TRANS_BIGGER_LTC,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t orbit_intlk_set_get_ang_bigger_x_exp = {
    .name = ORBIT_INTLK_NAME_SET_GET_ANG_BIGGER_X,
    .opcode = ORBIT_INTLK_OPCODE_SET_GET_ANG_BIGGER_X,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t orbit_intlk_set_get_ang_bigger_y_exp = {
    .name = ORBIT_INTLK_NAME_SET_GET_ANG_BIGGER_Y,
    .opcode = ORBIT_INTLK_OPCODE_SET_GET_ANG_BIGGER_Y,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t orbit_intlk_set_get_ang_bigger_ltc_x_exp = {
    .name = ORBIT_INTLK_NAME_SET_GET_ANG_BIGGER_LTC_X,
    .opcode = ORBIT_INTLK_OPCODE_SET_GET_ANG_BIGGER_LTC_X,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t orbit_intlk_set_get_ang_bigger_ltc_y_exp = {
    .name = ORBIT_INTLK_NAME_SET_GET_ANG_BIGGER_LTC_Y,
    .opcode = ORBIT_INTLK_OPCODE_SET_GET_ANG_BIGGER_LTC_Y,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t orbit_intlk_set_get_ang_bigger_any_exp = {
    .name = ORBIT_INTLK_NAME_SET_GET_ANG_BIGGER_ANY,
    .opcode = ORBIT_INTLK_OPCODE_SET_GET_ANG_BIGGER_ANY,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t orbit_intlk_set_get_ang_bigger_exp = {
    .name = ORBIT_INTLK_NAME_SET_GET_ANG_BIGGER,
    .opcode = ORBIT_INTLK_OPCODE_SET_GET_ANG_BIGGER,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t orbit_intlk_set_get_ang_bigger_ltc_exp = {
    .name = ORBIT_INTLK_NAME_SET_GET_ANG_BIGGER_LTC,
    .opcode = ORBIT_INTLK_OPCODE_SET_GET_ANG_BIGGER_LTC,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t orbit_intlk_set_get_intlk_bigger_exp = {
    .name = ORBIT_INTLK_NAME_SET_GET_INTLK_BIGGER,
    .opcode = ORBIT_INTLK_OPCODE_SET_GET_INTLK_BIGGER,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t orbit_intlk_set_get_intlk_bigger_ltc_exp = {
    .name = ORBIT_INTLK_NAME_SET_GET_INTLK_BIGGER_LTC,
    .opcode = ORBIT_INTLK_OPCODE_SET_GET_INTLK_BIGGER_LTC,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t orbit_intlk_set_get_min_sum_exp = {
    .name = ORBIT_INTLK_NAME_SET_GET_MIN_SUM,
    .opcode = ORBIT_INTLK_OPCODE_SET_GET_MIN_SUM,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t orbit_intlk_set_get_trans_max_x_exp = {
    .name = ORBIT_INTLK_NAME_SET_GET_TRANS_MAX_X,
    .opcode = ORBIT_INTLK_OPCODE_SET_GET_TRANS_MAX_X,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t orbit_intlk_set_get_trans_max_y_exp = {
    .name = ORBIT_INTLK_NAME_SET_GET_TRANS_MAX_Y,
    .opcode = ORBIT_INTLK_OPCODE_SET_GET_TRANS_MAX_Y,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t orbit_intlk_set_get_ang_max_x_exp = {
    .name = ORBIT_INTLK_NAME_SET_GET_ANG_MAX_X,
    .opcode = ORBIT_INTLK_OPCODE_SET_GET_ANG_MAX_X,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t orbit_intlk_set_get_ang_max_y_exp = {
    .name = ORBIT_INTLK_NAME_SET_GET_ANG_MAX_Y,
    .opcode = ORBIT_INTLK_OPCODE_SET_GET_ANG_MAX_Y,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

/* Exported function description */
const disp_op_t *orbit_intlk_exp_ops [] = {
    &orbit_intlk_set_get_en_exp,
    &orbit_intlk_set_get_clr_exp,
    &orbit_intlk_set_get_min_sum_en_exp,
    &orbit_intlk_set_get_trans_en_exp,
    &orbit_intlk_set_get_trans_clr_exp,
    &orbit_intlk_set_get_ang_en_exp,
    &orbit_intlk_set_get_ang_clr_exp,
    &orbit_intlk_set_get_trans_bigger_x_exp,
    &orbit_intlk_set_get_trans_bigger_y_exp,
    &orbit_intlk_set_get_trans_bigger_ltc_x_exp,
    &orbit_intlk_set_get_trans_bigger_ltc_y_exp,
    &orbit_intlk_set_get_trans_bigger_any_exp,
    &orbit_intlk_set_get_trans_bigger_exp,
    &orbit_intlk_set_get_trans_bigger_ltc_exp,
    &orbit_intlk_set_get_ang_bigger_x_exp,
    &orbit_intlk_set_get_ang_bigger_y_exp,
    &orbit_intlk_set_get_ang_bigger_ltc_x_exp,
    &orbit_intlk_set_get_ang_bigger_ltc_y_exp,
    &orbit_intlk_set_get_ang_bigger_any_exp,
    &orbit_intlk_set_get_ang_bigger_exp,
    &orbit_intlk_set_get_ang_bigger_ltc_exp,
    &orbit_intlk_set_get_intlk_bigger_exp,
    &orbit_intlk_set_get_intlk_bigger_ltc_exp,
    &orbit_intlk_set_get_min_sum_exp,
    &orbit_intlk_set_get_trans_max_x_exp,
    &orbit_intlk_set_get_trans_max_y_exp,
    &orbit_intlk_set_get_ang_max_x_exp,
    &orbit_intlk_set_get_ang_max_y_exp,
    NULL
};
