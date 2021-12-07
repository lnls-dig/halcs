/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "sm_io_exports_helper.h"
#include "sm_io_codes.h"

/* Description SMIO DSP functions */

disp_op_t dsp_set_get_kx_exp = {
    .name = DSP_NAME_SET_GET_KX,
    .opcode = DSP_OPCODE_SET_GET_KX,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_ky_exp = {
    .name = DSP_NAME_SET_GET_KY,
    .opcode = DSP_OPCODE_SET_GET_KY,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_ksum_exp = {
    .name = DSP_NAME_SET_GET_KSUM,
    .opcode = DSP_OPCODE_SET_GET_KSUM,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_ds_tbt_thres_exp = {
    .name = DSP_NAME_SET_GET_DS_TBT_THRES,
    .opcode = DSP_OPCODE_SET_GET_DS_TBT_THRES,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_ds_fofb_thres_exp = {
    .name = DSP_NAME_SET_GET_DS_FOFB_THRES,
    .opcode = DSP_OPCODE_SET_GET_DS_FOFB_THRES,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_ds_monit_thres_exp = {
    .name = DSP_NAME_SET_GET_DS_MONIT_THRES,
    .opcode = DSP_OPCODE_SET_GET_DS_MONIT_THRES,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_cfg_test_data_exp = {
    .name = DSP_NAME_SET_GET_CFG_TEST_DATA,
    .opcode = DSP_OPCODE_SET_GET_CFG_TEST_DATA,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_monit_amp_ch0_exp = {
    .name = DSP_NAME_SET_GET_MONIT_AMP_CH0,
    .opcode = DSP_OPCODE_SET_GET_MONIT_AMP_CH0,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_monit_amp_ch1_exp = {
    .name = DSP_NAME_SET_GET_MONIT_AMP_CH1,
    .opcode = DSP_OPCODE_SET_GET_MONIT_AMP_CH1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_monit_amp_ch2_exp = {
    .name = DSP_NAME_SET_GET_MONIT_AMP_CH2,
    .opcode = DSP_OPCODE_SET_GET_MONIT_AMP_CH2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_monit_amp_ch3_exp = {
    .name = DSP_NAME_SET_GET_MONIT_AMP_CH3,
    .opcode = DSP_OPCODE_SET_GET_MONIT_AMP_CH3,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_monit_pos_x_exp = {
    .name = DSP_NAME_SET_GET_MONIT_POS_X,
    .opcode = DSP_OPCODE_SET_GET_MONIT_POS_X,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_monit_pos_y_exp = {
    .name = DSP_NAME_SET_GET_MONIT_POS_Y,
    .opcode = DSP_OPCODE_SET_GET_MONIT_POS_Y,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_monit_pos_q_exp = {
    .name = DSP_NAME_SET_GET_MONIT_POS_Q,
    .opcode = DSP_OPCODE_SET_GET_MONIT_POS_Q,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_monit_pos_sum_exp = {
    .name = DSP_NAME_SET_GET_MONIT_POS_SUM,
    .opcode = DSP_OPCODE_SET_GET_MONIT_POS_SUM,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_monit_updt_exp = {
    .name = DSP_NAME_SET_GET_MONIT_UPDT,
    .opcode = DSP_OPCODE_SET_GET_MONIT_UPDT,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_monit_poll_time_exp = {
    .name = DSP_NAME_SET_GET_MONIT_POLL_TIME,
    .opcode = DSP_OPCODE_SET_GET_MONIT_POLL_TIME,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_monit1_amp_ch0_exp = {
    .name = DSP_NAME_SET_GET_MONIT1_AMP_CH0,
    .opcode = DSP_OPCODE_SET_GET_MONIT1_AMP_CH0,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_monit1_amp_ch1_exp = {
    .name = DSP_NAME_SET_GET_MONIT1_AMP_CH1,
    .opcode = DSP_OPCODE_SET_GET_MONIT1_AMP_CH1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_monit1_amp_ch2_exp = {
    .name = DSP_NAME_SET_GET_MONIT1_AMP_CH2,
    .opcode = DSP_OPCODE_SET_GET_MONIT1_AMP_CH2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_monit1_amp_ch3_exp = {
    .name = DSP_NAME_SET_GET_MONIT1_AMP_CH3,
    .opcode = DSP_OPCODE_SET_GET_MONIT1_AMP_CH3,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_monit1_pos_x_exp = {
    .name = DSP_NAME_SET_GET_MONIT1_POS_X,
    .opcode = DSP_OPCODE_SET_GET_MONIT1_POS_X,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_monit1_pos_y_exp = {
    .name = DSP_NAME_SET_GET_MONIT1_POS_Y,
    .opcode = DSP_OPCODE_SET_GET_MONIT1_POS_Y,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_monit1_pos_q_exp = {
    .name = DSP_NAME_SET_GET_MONIT1_POS_Q,
    .opcode = DSP_OPCODE_SET_GET_MONIT1_POS_Q,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_monit1_pos_sum_exp = {
    .name = DSP_NAME_SET_GET_MONIT1_POS_SUM,
    .opcode = DSP_OPCODE_SET_GET_MONIT1_POS_SUM,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_monit1_updt_exp = {
    .name = DSP_NAME_SET_GET_MONIT1_UPDT,
    .opcode = DSP_OPCODE_SET_GET_MONIT1_UPDT,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_monit1_amp_pos_exp = {
    .name = DSP_NAME_SET_GET_MONIT1_AMP_POS,
    .opcode = DSP_OPCODE_SET_GET_MONIT1_AMP_POS,
    .retval = DISP_ARG_ENCODE_RAW(DISP_ATYPE_STRUCT,
            sizeof(smio_dsp_data_t)),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_sw_tag_en_exp = {
    .name = DSP_NAME_SET_GET_SW_TAG_EN,
    .opcode = DSP_OPCODE_SET_GET_SW_TAG_EN,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_sw_data_mask_en_exp = {
    .name = DSP_NAME_SET_GET_SW_DATA_MASK_EN,
    .opcode = DSP_OPCODE_SET_GET_SW_DATA_MASK_EN,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_sw_data_mask_samples_exp = {
    .name = DSP_NAME_SET_GET_SW_DATA_MASK_SAMPLES,
    .opcode = DSP_OPCODE_SET_GET_SW_DATA_MASK_SAMPLES,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_tbt_tag_en_exp = {
    .name = DSP_NAME_SET_GET_TBT_TAG_EN,
    .opcode = DSP_OPCODE_SET_GET_TBT_TAG_EN,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_tbt_tag_dly_exp = {
    .name = DSP_NAME_SET_GET_TBT_TAG_DLY,
    .opcode = DSP_OPCODE_SET_GET_TBT_TAG_DLY,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_tbt_data_mask_en_exp = {
    .name = DSP_NAME_SET_GET_TBT_DATA_MASK_EN,
    .opcode = DSP_OPCODE_SET_GET_TBT_DATA_MASK_EN,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_tbt_data_mask_samples_beg_exp = {
    .name = DSP_NAME_SET_GET_TBT_DATA_MASK_SAMPLES_BEG,
    .opcode = DSP_OPCODE_SET_GET_TBT_DATA_MASK_SAMPLES_BEG,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_tbt_data_mask_samples_end_exp = {
    .name = DSP_NAME_SET_GET_TBT_DATA_MASK_SAMPLES_END,
    .opcode = DSP_OPCODE_SET_GET_TBT_DATA_MASK_SAMPLES_END,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_sw_tag_desync_cnt_rst_exp = {
    .name = DSP_NAME_SET_GET_SW_TAG_DESYNC_CNT_RST,
    .opcode = DSP_OPCODE_SET_GET_SW_TAG_DESYNC_CNT_RST,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_sw_tag_desync_cnt_exp = {
    .name = DSP_NAME_SET_GET_SW_TAG_DESYNC_CNT,
    .opcode = DSP_OPCODE_SET_GET_SW_TAG_DESYNC_CNT,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};


disp_op_t dsp_set_get_tbt_tag_desync_cnt_rst_exp = {
    .name = DSP_NAME_SET_GET_TBT_TAG_DESYNC_CNT_RST,
    .opcode = DSP_OPCODE_SET_GET_TBT_TAG_DESYNC_CNT_RST,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_tbt_tag_desync_cnt_exp = {
    .name = DSP_NAME_SET_GET_TBT_TAG_DESYNC_CNT,
    .opcode = DSP_OPCODE_SET_GET_TBT_TAG_DESYNC_CNT,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_monit1_tag_en_exp = {
    .name = DSP_NAME_SET_GET_MONIT1_TAG_EN,
    .opcode = DSP_OPCODE_SET_GET_MONIT1_TAG_EN,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_monit1_tag_dly_exp = {
    .name = DSP_NAME_SET_GET_MONIT1_TAG_DLY,
    .opcode = DSP_OPCODE_SET_GET_MONIT1_TAG_DLY,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_monit1_data_mask_en_exp = {
    .name = DSP_NAME_SET_GET_MONIT1_DATA_MASK_EN,
    .opcode = DSP_OPCODE_SET_GET_MONIT1_DATA_MASK_EN,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_monit1_data_mask_samples_beg_exp = {
    .name = DSP_NAME_SET_GET_MONIT1_DATA_MASK_SAMPLES_BEG,
    .opcode = DSP_OPCODE_SET_GET_MONIT1_DATA_MASK_SAMPLES_BEG,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_monit1_data_mask_samples_end_exp = {
    .name = DSP_NAME_SET_GET_MONIT1_DATA_MASK_SAMPLES_END,
    .opcode = DSP_OPCODE_SET_GET_MONIT1_DATA_MASK_SAMPLES_END,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_monit1_tag_desync_cnt_rst_exp = {
    .name = DSP_NAME_SET_GET_MONIT1_TAG_DESYNC_CNT_RST,
    .opcode = DSP_OPCODE_SET_GET_MONIT1_TAG_DESYNC_CNT_RST,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_monit1_tag_desync_cnt_exp = {
    .name = DSP_NAME_SET_GET_MONIT1_TAG_DESYNC_CNT,
    .opcode = DSP_OPCODE_SET_GET_MONIT1_TAG_DESYNC_CNT,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_monit_tag_en_exp = {
    .name = DSP_NAME_SET_GET_MONIT_TAG_EN,
    .opcode = DSP_OPCODE_SET_GET_MONIT_TAG_EN,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_monit_tag_dly_exp = {
    .name = DSP_NAME_SET_GET_MONIT_TAG_DLY,
    .opcode = DSP_OPCODE_SET_GET_MONIT_TAG_DLY,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_monit_data_mask_en_exp = {
    .name = DSP_NAME_SET_GET_MONIT_DATA_MASK_EN,
    .opcode = DSP_OPCODE_SET_GET_MONIT_DATA_MASK_EN,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_monit_data_mask_samples_beg_exp = {
    .name = DSP_NAME_SET_GET_MONIT_DATA_MASK_SAMPLES_BEG,
    .opcode = DSP_OPCODE_SET_GET_MONIT_DATA_MASK_SAMPLES_BEG,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_monit_data_mask_samples_end_exp = {
    .name = DSP_NAME_SET_GET_MONIT_DATA_MASK_SAMPLES_END,
    .opcode = DSP_OPCODE_SET_GET_MONIT_DATA_MASK_SAMPLES_END,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_monit_tag_desync_cnt_rst_exp = {
    .name = DSP_NAME_SET_GET_MONIT_TAG_DESYNC_CNT_RST,
    .opcode = DSP_OPCODE_SET_GET_MONIT_TAG_DESYNC_CNT_RST,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_monit_tag_desync_cnt_exp = {
    .name = DSP_NAME_SET_GET_MONIT_TAG_DESYNC_CNT,
    .opcode = DSP_OPCODE_SET_GET_MONIT_TAG_DESYNC_CNT,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_offset_x_exp = {
    .name = DSP_NAME_SET_GET_OFFSET_X,
    .opcode = DSP_OPCODE_SET_GET_OFFSET_X,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_INT32, int32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, int32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_offset_y_exp = {
    .name = DSP_NAME_SET_GET_OFFSET_Y,
    .opcode = DSP_OPCODE_SET_GET_OFFSET_Y,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_INT32, int32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, int32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_amp_gain_ch0_data_exp = {
    .name = DSP_NAME_SET_GET_AMP_GAIN_CH0_DATA,
    .opcode = DSP_OPCODE_SET_GET_AMP_GAIN_CH0_DATA,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_INT32, int32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, int32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_amp_gain_ch1_data_exp = {
    .name = DSP_NAME_SET_GET_AMP_GAIN_CH1_DATA,
    .opcode = DSP_OPCODE_SET_GET_AMP_GAIN_CH1_DATA,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_INT32, int32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, int32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_amp_gain_ch2_data_exp = {
    .name = DSP_NAME_SET_GET_AMP_GAIN_CH2_DATA,
    .opcode = DSP_OPCODE_SET_GET_AMP_GAIN_CH2_DATA,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_INT32, int32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, int32_t),
        DISP_ARG_END
    }
};

disp_op_t dsp_set_get_amp_gain_ch3_data_exp = {
    .name = DSP_NAME_SET_GET_AMP_GAIN_CH3_DATA,
    .opcode = DSP_OPCODE_SET_GET_AMP_GAIN_CH3_DATA,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_INT32, int32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, int32_t),
        DISP_ARG_END
    }
};

/* Exported function description */
const disp_op_t *dsp_exp_ops [] = {
    &dsp_set_get_kx_exp,
    &dsp_set_get_ky_exp,
    &dsp_set_get_ksum_exp,
    &dsp_set_get_ds_tbt_thres_exp,
    &dsp_set_get_ds_fofb_thres_exp,
    &dsp_set_get_ds_monit_thres_exp,
    &dsp_set_get_cfg_test_data_exp,
    &dsp_set_get_monit_amp_ch0_exp,
    &dsp_set_get_monit_amp_ch1_exp,
    &dsp_set_get_monit_amp_ch2_exp,
    &dsp_set_get_monit_amp_ch3_exp,
    &dsp_set_get_monit_pos_x_exp,
    &dsp_set_get_monit_pos_y_exp,
    &dsp_set_get_monit_pos_q_exp,
    &dsp_set_get_monit_pos_sum_exp,
    &dsp_set_get_monit_updt_exp,
    &dsp_set_get_monit_poll_time_exp,
    &dsp_set_get_monit1_amp_ch0_exp,
    &dsp_set_get_monit1_amp_ch1_exp,
    &dsp_set_get_monit1_amp_ch2_exp,
    &dsp_set_get_monit1_amp_ch3_exp,
    &dsp_set_get_monit1_pos_x_exp,
    &dsp_set_get_monit1_pos_y_exp,
    &dsp_set_get_monit1_pos_q_exp,
    &dsp_set_get_monit1_pos_sum_exp,
    &dsp_set_get_monit1_updt_exp,
    &dsp_set_get_monit1_amp_pos_exp,
    &dsp_set_get_sw_tag_en_exp,
    &dsp_set_get_sw_data_mask_en_exp,
    &dsp_set_get_sw_data_mask_samples_exp,
    &dsp_set_get_tbt_tag_en_exp,
    &dsp_set_get_tbt_tag_dly_exp,
    &dsp_set_get_tbt_data_mask_en_exp,
    &dsp_set_get_tbt_data_mask_samples_beg_exp,
    &dsp_set_get_tbt_data_mask_samples_end_exp,
    &dsp_set_get_sw_tag_desync_cnt_rst_exp,
    &dsp_set_get_sw_tag_desync_cnt_exp,
    &dsp_set_get_tbt_tag_desync_cnt_rst_exp,
    &dsp_set_get_tbt_tag_desync_cnt_exp,
    &dsp_set_get_monit1_tag_en_exp,
    &dsp_set_get_monit1_tag_dly_exp,
    &dsp_set_get_monit1_data_mask_en_exp,
    &dsp_set_get_monit1_data_mask_samples_beg_exp,
    &dsp_set_get_monit1_data_mask_samples_end_exp,
    &dsp_set_get_monit1_tag_desync_cnt_rst_exp,
    &dsp_set_get_monit1_tag_desync_cnt_exp,
    &dsp_set_get_monit_tag_en_exp,
    &dsp_set_get_monit_tag_dly_exp,
    &dsp_set_get_monit_data_mask_en_exp,
    &dsp_set_get_monit_data_mask_samples_beg_exp,
    &dsp_set_get_monit_data_mask_samples_end_exp,
    &dsp_set_get_monit_tag_desync_cnt_rst_exp,
    &dsp_set_get_monit_tag_desync_cnt_exp,
    &dsp_set_get_offset_x_exp,
    &dsp_set_get_offset_y_exp,
    &dsp_set_get_amp_gain_ch0_data_exp,
    &dsp_set_get_amp_gain_ch1_data_exp,
    &dsp_set_get_amp_gain_ch2_data_exp,
    &dsp_set_get_amp_gain_ch3_data_exp,
    NULL
};


