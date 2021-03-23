/*
 * Copyright (C) 2021 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "sm_io_exports_helper.h"
#include "sm_io_codes.h"

/* Description SMIO FOFB_CTRL functions */
disp_op_t fofb_ctrl_set_get_act_part_exp = {
    .name = FOFB_CTRL_NAME_SET_GET_ACT_PART,
    .opcode = FOFB_CTRL_OPCODE_SET_GET_ACT_PART,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_ctrl_set_get_err_clr_exp = {
    .name = FOFB_CTRL_NAME_SET_GET_ERR_CLR,
    .opcode = FOFB_CTRL_OPCODE_SET_GET_ERR_CLR,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_ctrl_set_get_cc_enable_exp = {
    .name = FOFB_CTRL_NAME_SET_GET_CC_ENABLE,
    .opcode = FOFB_CTRL_OPCODE_SET_GET_CC_ENABLE,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_ctrl_set_get_tfs_override_exp = {
    .name = FOFB_CTRL_NAME_SET_GET_TFS_OVERRIDE,
    .opcode = FOFB_CTRL_OPCODE_SET_GET_TFS_OVERRIDE,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_ctrl_set_get_bpm_id_exp = {
    .name = FOFB_CTRL_NAME_SET_GET_BPM_ID,
    .opcode = FOFB_CTRL_OPCODE_SET_GET_BPM_ID,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_ctrl_set_get_time_frame_len_exp = {
    .name = FOFB_CTRL_NAME_SET_GET_TIME_FRAME_LEN,
    .opcode = FOFB_CTRL_OPCODE_SET_GET_TIME_FRAME_LEN,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_ctrl_set_get_mgt_powerdown_exp = {
    .name = FOFB_CTRL_NAME_SET_GET_MGT_POWERDOWN,
    .opcode = FOFB_CTRL_OPCODE_SET_GET_MGT_POWERDOWN,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_ctrl_set_get_mgt_loopback_exp = {
    .name = FOFB_CTRL_NAME_SET_GET_MGT_LOOPBACK,
    .opcode = FOFB_CTRL_OPCODE_SET_GET_MGT_LOOPBACK,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_ctrl_set_get_time_frame_dly_exp = {
    .name = FOFB_CTRL_NAME_SET_GET_TIME_FRAME_DLY,
    .opcode = FOFB_CTRL_OPCODE_SET_GET_TIME_FRAME_DLY,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_ctrl_set_get_golden_orb_x_exp = {
    .name = FOFB_CTRL_NAME_SET_GET_GOLDEN_ORB_X,
    .opcode = FOFB_CTRL_OPCODE_SET_GET_GOLDEN_ORB_X,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_ctrl_set_get_golden_orb_y_exp = {
    .name = FOFB_CTRL_NAME_SET_GET_GOLDEN_ORB_Y,
    .opcode = FOFB_CTRL_OPCODE_SET_GET_GOLDEN_ORB_Y,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_ctrl_set_get_cust_feature_exp = {
    .name = FOFB_CTRL_NAME_SET_GET_CUST_FEATURE,
    .opcode = FOFB_CTRL_OPCODE_SET_GET_CUST_FEATURE,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_ctrl_set_get_rx_polarity_exp = {
    .name = FOFB_CTRL_NAME_SET_GET_RX_POLARITY,
    .opcode = FOFB_CTRL_OPCODE_SET_GET_RX_POLARITY,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_ctrl_set_get_payloadsel_exp = {
    .name = FOFB_CTRL_NAME_SET_GET_PAYLOADSEL,
    .opcode = FOFB_CTRL_OPCODE_SET_GET_PAYLOADSEL,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_ctrl_set_get_fofbdatasel_exp = {
    .name = FOFB_CTRL_NAME_SET_GET_FOFBDATASEL,
    .opcode = FOFB_CTRL_OPCODE_SET_GET_FOFBDATASEL,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_ctrl_set_get_firmware_ver_exp = {
    .name = FOFB_CTRL_NAME_SET_GET_FIRMWARE_VER,
    .opcode = FOFB_CTRL_OPCODE_SET_GET_FIRMWARE_VER,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_ctrl_set_get_sys_status_exp = {
    .name = FOFB_CTRL_NAME_SET_GET_SYS_STATUS,
    .opcode = FOFB_CTRL_OPCODE_SET_GET_SYS_STATUS,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_ctrl_set_get_link_partner_1_exp = {
    .name = FOFB_CTRL_NAME_SET_GET_LINK_PARTNER_1,
    .opcode = FOFB_CTRL_OPCODE_SET_GET_LINK_PARTNER_1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_ctrl_set_get_link_partner_2_exp = {
    .name = FOFB_CTRL_NAME_SET_GET_LINK_PARTNER_2,
    .opcode = FOFB_CTRL_OPCODE_SET_GET_LINK_PARTNER_2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_ctrl_set_get_link_partner_3_exp = {
    .name = FOFB_CTRL_NAME_SET_GET_LINK_PARTNER_3,
    .opcode = FOFB_CTRL_OPCODE_SET_GET_LINK_PARTNER_3,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_ctrl_set_get_link_partner_4_exp = {
    .name = FOFB_CTRL_NAME_SET_GET_LINK_PARTNER_4,
    .opcode = FOFB_CTRL_OPCODE_SET_GET_LINK_PARTNER_4,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_ctrl_set_get_link_up_exp = {
    .name = FOFB_CTRL_NAME_SET_GET_LINK_UP,
    .opcode = FOFB_CTRL_OPCODE_SET_GET_LINK_UP,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_ctrl_set_get_time_frame_count_exp = {
    .name = FOFB_CTRL_NAME_SET_GET_TIME_FRAME_COUNT,
    .opcode = FOFB_CTRL_OPCODE_SET_GET_TIME_FRAME_COUNT,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_ctrl_set_get_hard_err_cnt_1_exp = {
    .name = FOFB_CTRL_NAME_SET_GET_HARD_ERR_CNT_1,
    .opcode = FOFB_CTRL_OPCODE_SET_GET_HARD_ERR_CNT_1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_ctrl_set_get_hard_err_cnt_2_exp = {
    .name = FOFB_CTRL_NAME_SET_GET_HARD_ERR_CNT_2,
    .opcode = FOFB_CTRL_OPCODE_SET_GET_HARD_ERR_CNT_2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_ctrl_set_get_hard_err_cnt_3_exp = {
    .name = FOFB_CTRL_NAME_SET_GET_HARD_ERR_CNT_3,
    .opcode = FOFB_CTRL_OPCODE_SET_GET_HARD_ERR_CNT_3,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_ctrl_set_get_hard_err_cnt_4_exp = {
    .name = FOFB_CTRL_NAME_SET_GET_HARD_ERR_CNT_4,
    .opcode = FOFB_CTRL_OPCODE_SET_GET_HARD_ERR_CNT_4,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_ctrl_set_get_soft_err_cnt_1_exp = {
    .name = FOFB_CTRL_NAME_SET_GET_SOFT_ERR_CNT_1,
    .opcode = FOFB_CTRL_OPCODE_SET_GET_SOFT_ERR_CNT_1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_ctrl_set_get_soft_err_cnt_2_exp = {
    .name = FOFB_CTRL_NAME_SET_GET_SOFT_ERR_CNT_2,
    .opcode = FOFB_CTRL_OPCODE_SET_GET_SOFT_ERR_CNT_2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_ctrl_set_get_soft_err_cnt_3_exp = {
    .name = FOFB_CTRL_NAME_SET_GET_SOFT_ERR_CNT_3,
    .opcode = FOFB_CTRL_OPCODE_SET_GET_SOFT_ERR_CNT_3,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_ctrl_set_get_soft_err_cnt_4_exp = {
    .name = FOFB_CTRL_NAME_SET_GET_SOFT_ERR_CNT_4,
    .opcode = FOFB_CTRL_OPCODE_SET_GET_SOFT_ERR_CNT_4,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_ctrl_set_get_frame_err_cnt_1_exp = {
    .name = FOFB_CTRL_NAME_SET_GET_FRAME_ERR_CNT_1,
    .opcode = FOFB_CTRL_OPCODE_SET_GET_FRAME_ERR_CNT_1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_ctrl_set_get_frame_err_cnt_2_exp = {
    .name = FOFB_CTRL_NAME_SET_GET_FRAME_ERR_CNT_2,
    .opcode = FOFB_CTRL_OPCODE_SET_GET_FRAME_ERR_CNT_2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_ctrl_set_get_frame_err_cnt_3_exp = {
    .name = FOFB_CTRL_NAME_SET_GET_FRAME_ERR_CNT_3,
    .opcode = FOFB_CTRL_OPCODE_SET_GET_FRAME_ERR_CNT_3,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_ctrl_set_get_frame_err_cnt_4_exp = {
    .name = FOFB_CTRL_NAME_SET_GET_FRAME_ERR_CNT_4,
    .opcode = FOFB_CTRL_OPCODE_SET_GET_FRAME_ERR_CNT_4,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_ctrl_set_get_rx_pck_cnt_1_exp = {
    .name = FOFB_CTRL_NAME_SET_GET_RX_PCK_CNT_1,
    .opcode = FOFB_CTRL_OPCODE_SET_GET_RX_PCK_CNT_1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_ctrl_set_get_rx_pck_cnt_2_exp = {
    .name = FOFB_CTRL_NAME_SET_GET_RX_PCK_CNT_2,
    .opcode = FOFB_CTRL_OPCODE_SET_GET_RX_PCK_CNT_2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_ctrl_set_get_rx_pck_cnt_3_exp = {
    .name = FOFB_CTRL_NAME_SET_GET_RX_PCK_CNT_3,
    .opcode = FOFB_CTRL_OPCODE_SET_GET_RX_PCK_CNT_3,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_ctrl_set_get_rx_pck_cnt_4_exp = {
    .name = FOFB_CTRL_NAME_SET_GET_RX_PCK_CNT_4,
    .opcode = FOFB_CTRL_OPCODE_SET_GET_RX_PCK_CNT_4,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_ctrl_set_get_tx_pck_cnt_1_exp = {
    .name = FOFB_CTRL_NAME_SET_GET_TX_PCK_CNT_1,
    .opcode = FOFB_CTRL_OPCODE_SET_GET_TX_PCK_CNT_1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_ctrl_set_get_tx_pck_cnt_2_exp = {
    .name = FOFB_CTRL_NAME_SET_GET_TX_PCK_CNT_2,
    .opcode = FOFB_CTRL_OPCODE_SET_GET_TX_PCK_CNT_2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_ctrl_set_get_tx_pck_cnt_3_exp = {
    .name = FOFB_CTRL_NAME_SET_GET_TX_PCK_CNT_3,
    .opcode = FOFB_CTRL_OPCODE_SET_GET_TX_PCK_CNT_3,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_ctrl_set_get_tx_pck_cnt_4_exp = {
    .name = FOFB_CTRL_NAME_SET_GET_TX_PCK_CNT_4,
    .opcode = FOFB_CTRL_OPCODE_SET_GET_TX_PCK_CNT_4,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_ctrl_set_get_fod_process_time_exp = {
    .name = FOFB_CTRL_NAME_SET_GET_FOD_PROCESS_TIME,
    .opcode = FOFB_CTRL_OPCODE_SET_GET_FOD_PROCESS_TIME,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_ctrl_set_get_bpm_count_exp = {
    .name = FOFB_CTRL_NAME_SET_GET_BPM_COUNT,
    .opcode = FOFB_CTRL_OPCODE_SET_GET_BPM_COUNT,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_ctrl_set_get_bpm_id_rdback_exp = {
    .name = FOFB_CTRL_NAME_SET_GET_BPM_ID_RDBACK,
    .opcode = FOFB_CTRL_OPCODE_SET_GET_BPM_ID_RDBACK,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_ctrl_set_get_tf_length_rdback_exp = {
    .name = FOFB_CTRL_NAME_SET_GET_TF_LENGTH_RDBACK,
    .opcode = FOFB_CTRL_OPCODE_SET_GET_TF_LENGTH_RDBACK,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_ctrl_set_get_powerdown_rdback_exp = {
    .name = FOFB_CTRL_NAME_SET_GET_POWERDOWN_RDBACK,
    .opcode = FOFB_CTRL_OPCODE_SET_GET_POWERDOWN_RDBACK,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_ctrl_set_get_loopback_rdback_exp = {
    .name = FOFB_CTRL_NAME_SET_GET_LOOPBACK_RDBACK,
    .opcode = FOFB_CTRL_OPCODE_SET_GET_LOOPBACK_RDBACK,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fofb_ctrl_set_get_faival_rdback_exp = {
    .name = FOFB_CTRL_NAME_SET_GET_FAIVAL_RDBACK,
    .opcode = FOFB_CTRL_OPCODE_SET_GET_FAIVAL_RDBACK,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

/* Exported function description */
const disp_op_t *fofb_ctrl_exp_ops [] = {
    &fofb_ctrl_set_get_act_part_exp,
    &fofb_ctrl_set_get_err_clr_exp,
    &fofb_ctrl_set_get_cc_enable_exp,
    &fofb_ctrl_set_get_tfs_override_exp,
    &fofb_ctrl_set_get_bpm_id_exp,
    &fofb_ctrl_set_get_time_frame_len_exp,
    &fofb_ctrl_set_get_mgt_powerdown_exp,
    &fofb_ctrl_set_get_mgt_loopback_exp,
    &fofb_ctrl_set_get_time_frame_dly_exp,
    &fofb_ctrl_set_get_golden_orb_x_exp,
    &fofb_ctrl_set_get_golden_orb_y_exp,
    &fofb_ctrl_set_get_cust_feature_exp,
    &fofb_ctrl_set_get_rx_polarity_exp,
    &fofb_ctrl_set_get_payloadsel_exp,
    &fofb_ctrl_set_get_fofbdatasel_exp,
    &fofb_ctrl_set_get_firmware_ver_exp,
    &fofb_ctrl_set_get_sys_status_exp,
    &fofb_ctrl_set_get_link_partner_1_exp,
    &fofb_ctrl_set_get_link_partner_2_exp,
    &fofb_ctrl_set_get_link_partner_3_exp,
    &fofb_ctrl_set_get_link_partner_4_exp,
    &fofb_ctrl_set_get_link_up_exp,
    &fofb_ctrl_set_get_time_frame_count_exp,
    &fofb_ctrl_set_get_hard_err_cnt_1_exp,
    &fofb_ctrl_set_get_hard_err_cnt_2_exp,
    &fofb_ctrl_set_get_hard_err_cnt_3_exp,
    &fofb_ctrl_set_get_hard_err_cnt_4_exp,
    &fofb_ctrl_set_get_soft_err_cnt_1_exp,
    &fofb_ctrl_set_get_soft_err_cnt_2_exp,
    &fofb_ctrl_set_get_soft_err_cnt_3_exp,
    &fofb_ctrl_set_get_soft_err_cnt_4_exp,
    &fofb_ctrl_set_get_frame_err_cnt_1_exp,
    &fofb_ctrl_set_get_frame_err_cnt_2_exp,
    &fofb_ctrl_set_get_frame_err_cnt_3_exp,
    &fofb_ctrl_set_get_frame_err_cnt_4_exp,
    &fofb_ctrl_set_get_rx_pck_cnt_1_exp,
    &fofb_ctrl_set_get_rx_pck_cnt_2_exp,
    &fofb_ctrl_set_get_rx_pck_cnt_3_exp,
    &fofb_ctrl_set_get_rx_pck_cnt_4_exp,
    &fofb_ctrl_set_get_tx_pck_cnt_1_exp,
    &fofb_ctrl_set_get_tx_pck_cnt_2_exp,
    &fofb_ctrl_set_get_tx_pck_cnt_3_exp,
    &fofb_ctrl_set_get_tx_pck_cnt_4_exp,
    &fofb_ctrl_set_get_fod_process_time_exp,
    &fofb_ctrl_set_get_bpm_count_exp,
    &fofb_ctrl_set_get_bpm_id_rdback_exp,
    &fofb_ctrl_set_get_tf_length_rdback_exp,
    &fofb_ctrl_set_get_powerdown_rdback_exp,
    &fofb_ctrl_set_get_loopback_rdback_exp,
    &fofb_ctrl_set_get_faival_rdback_exp,
    NULL
};
