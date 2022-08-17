/*
 * Copyright (C) 2021 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "sm_io_exports_helper.h"
#include "sm_io_codes.h"

/* Description SMIO RTMLAMP_OHWR functions */

disp_op_t rtmlamp_ohwr_sta_exp = {
    .name = RTMLAMP_OHWR_NAME_STA,
    .opcode = RTMLAMP_OHWR_OPCODE_STA,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t rtmlamp_ohwr_amp_iflag_l_exp = {
    .name = RTMLAMP_OHWR_NAME_AMP_IFLAG_L,
    .opcode = RTMLAMP_OHWR_OPCODE_AMP_IFLAG_L,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t rtmlamp_ohwr_amp_tflag_l_exp = {
    .name = RTMLAMP_OHWR_NAME_AMP_TFLAG_L,
    .opcode = RTMLAMP_OHWR_OPCODE_AMP_TFLAG_L,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t rtmlamp_ohwr_amp_iflag_r_exp = {
    .name = RTMLAMP_OHWR_NAME_AMP_IFLAG_R,
    .opcode = RTMLAMP_OHWR_OPCODE_AMP_IFLAG_R,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t rtmlamp_ohwr_amp_tflag_r_exp = {
    .name = RTMLAMP_OHWR_NAME_AMP_TFLAG_R,
    .opcode = RTMLAMP_OHWR_OPCODE_AMP_TFLAG_R,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t rtmlamp_ohwr_amp_en_exp = {
    .name = RTMLAMP_OHWR_NAME_AMP_EN,
    .opcode = RTMLAMP_OHWR_OPCODE_AMP_EN,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t rtmlamp_ohwr_trig_en_exp = {
    .name = RTMLAMP_OHWR_NAME_TRIG_EN,
    .opcode = RTMLAMP_OHWR_OPCODE_TRIG_EN,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t rtmlamp_ohwr_mode_exp = {
    .name = RTMLAMP_OHWR_NAME_MODE,
    .opcode = RTMLAMP_OHWR_OPCODE_MODE,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t rtmlamp_ohwr_pi_kp_exp = {
    .name = RTMLAMP_OHWR_NAME_PI_KP,
    .opcode = RTMLAMP_OHWR_OPCODE_PI_KP,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t rtmlamp_ohwr_pi_ti_exp = {
    .name = RTMLAMP_OHWR_NAME_PI_TI,
    .opcode = RTMLAMP_OHWR_OPCODE_PI_TI,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t rtmlamp_ohwr_pi_sp_exp = {
    .name = RTMLAMP_OHWR_NAME_PI_SP,
    .opcode = RTMLAMP_OHWR_OPCODE_PI_SP,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t rtmlamp_ohwr_dac_data_exp = {
    .name = RTMLAMP_OHWR_NAME_DAC_DATA,
    .opcode = RTMLAMP_OHWR_OPCODE_DAC_DATA,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t rtmlamp_ohwr_lim_a_exp = {
    .name = RTMLAMP_OHWR_NAME_LIM_A,
    .opcode = RTMLAMP_OHWR_OPCODE_LIM_A,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t rtmlamp_ohwr_lim_b_exp = {
    .name = RTMLAMP_OHWR_NAME_LIM_B,
    .opcode = RTMLAMP_OHWR_OPCODE_LIM_B,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t rtmlamp_ohwr_cnt_exp = {
    .name = RTMLAMP_OHWR_NAME_CNT,
    .opcode = RTMLAMP_OHWR_OPCODE_CNT,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t rtmlamp_ohwr_eff_adc_exp = {
    .name = RTMLAMP_OHWR_NAME_EFF_ADC,
    .opcode = RTMLAMP_OHWR_OPCODE_EFF_ADC,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t rtmlamp_ohwr_eff_dac_exp = {
    .name = RTMLAMP_OHWR_NAME_EFF_DAC,
    .opcode = RTMLAMP_OHWR_OPCODE_EFF_DAC,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t rtmlamp_ohwr_eff_sp_exp = {
    .name = RTMLAMP_OHWR_NAME_EFF_SP,
    .opcode = RTMLAMP_OHWR_OPCODE_EFF_SP,
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
const disp_op_t *rtmlamp_ohwr_exp_ops [] = {
    &rtmlamp_ohwr_sta_exp,
    &rtmlamp_ohwr_amp_iflag_l_exp,
    &rtmlamp_ohwr_amp_tflag_l_exp,
    &rtmlamp_ohwr_amp_iflag_r_exp,
    &rtmlamp_ohwr_amp_tflag_r_exp,
    &rtmlamp_ohwr_amp_en_exp,
    &rtmlamp_ohwr_mode_exp,
    &rtmlamp_ohwr_pi_kp_exp,
    &rtmlamp_ohwr_pi_ti_exp,
    &rtmlamp_ohwr_pi_sp_exp,
    &rtmlamp_ohwr_dac_data_exp,
    &rtmlamp_ohwr_lim_a_exp,
    &rtmlamp_ohwr_lim_b_exp,
    &rtmlamp_ohwr_cnt_exp,
    &rtmlamp_ohwr_eff_adc_exp,
    &rtmlamp_ohwr_eff_dac_exp,
    &rtmlamp_ohwr_eff_sp_exp,
    &rtmlamp_ohwr_trig_en_exp,
    NULL
};

