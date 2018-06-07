/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Joao Brito <joao.brito@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "sm_io_exports_helper.h"
#include "sm_io_codes.h"

/* Description SMIO AFC_TIMING functions */

disp_op_t afc_timing_set_get_link_status_exp = {
    .name = AFC_TIMING_NAME_SET_GET_LINK_STATUS,
    .opcode = AFC_TIMING_OPCODE_SET_GET_LINK_STATUS,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_rxen_status_exp = {
    .name = AFC_TIMING_NAME_SET_GET_RXEN_STATUS,
    .opcode = AFC_TIMING_OPCODE_SET_GET_RXEN_STATUS,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_ref_clk_locked_exp = {
    .name = AFC_TIMING_NAME_SET_GET_REF_CLK_LOCKED,
    .opcode = AFC_TIMING_OPCODE_SET_GET_REF_CLK_LOCKED,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_evt_code_0_exp = {
    .name = AFC_TIMING_NAME_SET_GET_EVT_IN0,
    .opcode = AFC_TIMING_OPCODE_SET_GET_EVT_IN0,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_evt_code_1_exp = {
    .name = AFC_TIMING_NAME_SET_GET_EVT_IN1,
    .opcode = AFC_TIMING_OPCODE_SET_GET_EVT_IN1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_evt_code_2_exp = {
    .name = AFC_TIMING_NAME_SET_GET_EVT_IN2,
    .opcode = AFC_TIMING_OPCODE_SET_GET_EVT_IN2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_evt_code_3_exp = {
    .name = AFC_TIMING_NAME_SET_GET_EVT_IN3,
    .opcode = AFC_TIMING_OPCODE_SET_GET_EVT_IN3,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_evt_code_4_exp = {
    .name = AFC_TIMING_NAME_SET_GET_EVT_IN4,
    .opcode = AFC_TIMING_OPCODE_SET_GET_EVT_IN4,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_evt_code_5_exp = {
    .name = AFC_TIMING_NAME_SET_GET_EVT_IN5,
    .opcode = AFC_TIMING_OPCODE_SET_GET_EVT_IN5,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_evt_code_6_exp = {
    .name = AFC_TIMING_NAME_SET_GET_EVT_IN6,
    .opcode = AFC_TIMING_OPCODE_SET_GET_EVT_IN6,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_evt_code_7_exp = {
    .name = AFC_TIMING_NAME_SET_GET_EVT_IN7,
    .opcode = AFC_TIMING_OPCODE_SET_GET_EVT_IN7,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_evt_delay_0_exp = {
    .name = AFC_TIMING_NAME_SET_GET_EVT_DLY0,
    .opcode = AFC_TIMING_OPCODE_SET_GET_EVT_DLY0,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_evt_delay_1_exp = {
    .name = AFC_TIMING_NAME_SET_GET_EVT_DLY1,
    .opcode = AFC_TIMING_OPCODE_SET_GET_EVT_DLY1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_evt_delay_2_exp = {
    .name = AFC_TIMING_NAME_SET_GET_EVT_DLY2,
    .opcode = AFC_TIMING_OPCODE_SET_GET_EVT_DLY2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_evt_delay_3_exp = {
    .name = AFC_TIMING_NAME_SET_GET_EVT_DLY3,
    .opcode = AFC_TIMING_OPCODE_SET_GET_EVT_DLY3,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_evt_delay_4_exp = {
    .name = AFC_TIMING_NAME_SET_GET_EVT_DLY4,
    .opcode = AFC_TIMING_OPCODE_SET_GET_EVT_DLY4,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_evt_delay_5_exp = {
    .name = AFC_TIMING_NAME_SET_GET_EVT_DLY5,
    .opcode = AFC_TIMING_OPCODE_SET_GET_EVT_DLY5,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_evt_delay_6_exp = {
    .name = AFC_TIMING_NAME_SET_GET_EVT_DLY6,
    .opcode = AFC_TIMING_OPCODE_SET_GET_EVT_DLY6,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_evt_delay_7_exp = {
    .name = AFC_TIMING_NAME_SET_GET_EVT_DLY7,
    .opcode = AFC_TIMING_OPCODE_SET_GET_EVT_DLY7,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_evt_width_0_exp = {
    .name = AFC_TIMING_NAME_SET_GET_EVT_WDT0,
    .opcode = AFC_TIMING_OPCODE_SET_GET_EVT_WDT0,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_evt_width_1_exp = {
    .name = AFC_TIMING_NAME_SET_GET_EVT_WDT1,
    .opcode = AFC_TIMING_OPCODE_SET_GET_EVT_WDT1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_evt_width_2_exp = {
    .name = AFC_TIMING_NAME_SET_GET_EVT_WDT2,
    .opcode = AFC_TIMING_OPCODE_SET_GET_EVT_WDT2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_evt_width_3_exp = {
    .name = AFC_TIMING_NAME_SET_GET_EVT_WDT3,
    .opcode = AFC_TIMING_OPCODE_SET_GET_EVT_WDT3,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_evt_width_4_exp = {
    .name = AFC_TIMING_NAME_SET_GET_EVT_WDT4,
    .opcode = AFC_TIMING_OPCODE_SET_GET_EVT_WDT4,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_evt_width_5_exp = {
    .name = AFC_TIMING_NAME_SET_GET_EVT_WDT5,
    .opcode = AFC_TIMING_OPCODE_SET_GET_EVT_WDT5,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_evt_width_6_exp = {
    .name = AFC_TIMING_NAME_SET_GET_EVT_WDT6,
    .opcode = AFC_TIMING_OPCODE_SET_GET_EVT_WDT6,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_evt_width_7_exp = {
    .name = AFC_TIMING_NAME_SET_GET_EVT_WDT7,
    .opcode = AFC_TIMING_OPCODE_SET_GET_EVT_WDT7,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_freq_kp_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FREQ_KP,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FREQ_KP,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_freq_ki_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FREQ_KI,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FREQ_KI,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_phase_kp_exp = {
    .name = AFC_TIMING_NAME_SET_GET_PHASE_KP,
    .opcode = AFC_TIMING_OPCODE_SET_GET_PHASE_KP,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_phase_ki_exp = {
    .name = AFC_TIMING_NAME_SET_GET_PHASE_KI,
    .opcode = AFC_TIMING_OPCODE_SET_GET_PHASE_KI,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_phase_set_exp = {
    .name = AFC_TIMING_NAME_SET_GET_PHASE_SET,
    .opcode = AFC_TIMING_OPCODE_SET_GET_PHASE_SET,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_avg_exponent_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AVG_EXPONENT,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AVG_EXPONENT,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_rtm_rfreq_hi_exp = {
    .name = AFC_TIMING_NAME_SET_GET_RTM_RFREQ_HI,
    .opcode = AFC_TIMING_OPCODE_SET_GET_RTM_RFREQ_HI,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_rtm_rfreq_lo_exp = {
    .name = AFC_TIMING_NAME_SET_GET_RTM_RFREQ_LO,
    .opcode = AFC_TIMING_OPCODE_SET_GET_RTM_RFREQ_LO,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_rtm_n1_exp = {
    .name = AFC_TIMING_NAME_SET_GET_RTM_N1,
    .opcode = AFC_TIMING_OPCODE_SET_GET_RTM_N1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_rtm_hs_div_exp = {
    .name = AFC_TIMING_NAME_SET_GET_RTM_HS_DIV,
    .opcode = AFC_TIMING_OPCODE_SET_GET_RTM_HS_DIV,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_afc_rfreq_hi_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AFC_RFREQ_HI,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AFC_RFREQ_HI,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_afc_rfreq_lo_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AFC_RFREQ_LO,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AFC_RFREQ_LO,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_afc_n1_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AFC_N1,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AFC_N1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_afc_hs_div_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AFC_HS_DIV,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AFC_HS_DIV,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

/* Exported function description */
const disp_op_t *afc_timing_exp_ops [] = {
    &afc_timing_set_get_link_status_exp,
    &afc_timing_set_get_rxen_status_exp,
    &afc_timing_set_get_ref_clk_locked_exp,
    &afc_timing_set_get_evt_code_0_exp,
    &afc_timing_set_get_evt_code_1_exp,
    &afc_timing_set_get_evt_code_2_exp,
    &afc_timing_set_get_evt_code_3_exp,
    &afc_timing_set_get_evt_code_4_exp,
    &afc_timing_set_get_evt_code_5_exp,
    &afc_timing_set_get_evt_code_6_exp,
    &afc_timing_set_get_evt_code_7_exp,
    &afc_timing_set_get_evt_delay_0_exp,
    &afc_timing_set_get_evt_delay_1_exp,
    &afc_timing_set_get_evt_delay_2_exp,
    &afc_timing_set_get_evt_delay_3_exp,
    &afc_timing_set_get_evt_delay_4_exp,
    &afc_timing_set_get_evt_delay_5_exp,
    &afc_timing_set_get_evt_delay_6_exp,
    &afc_timing_set_get_evt_delay_7_exp,
    &afc_timing_set_get_evt_width_0_exp,
    &afc_timing_set_get_evt_width_1_exp,
    &afc_timing_set_get_evt_width_2_exp,
    &afc_timing_set_get_evt_width_3_exp,
    &afc_timing_set_get_evt_width_4_exp,
    &afc_timing_set_get_evt_width_5_exp,
    &afc_timing_set_get_evt_width_6_exp,
    &afc_timing_set_get_evt_width_7_exp,
    &afc_timing_set_get_freq_kp_exp,
    &afc_timing_set_get_freq_ki_exp,
    &afc_timing_set_get_phase_kp_exp,
    &afc_timing_set_get_phase_ki_exp,
    &afc_timing_set_get_phase_set_exp,
    &afc_timing_set_get_avg_exponent_exp,
    &afc_timing_set_get_rtm_rfreq_hi_exp,
    &afc_timing_set_get_rtm_rfreq_lo_exp,
    &afc_timing_set_get_rtm_n1_exp,
    &afc_timing_set_get_rtm_hs_div_exp,
    &afc_timing_set_get_afc_rfreq_hi_exp,
    &afc_timing_set_get_afc_rfreq_lo_exp,
    &afc_timing_set_get_afc_n1_exp,
    &afc_timing_set_get_afc_hs_div_exp,
    NULL
};
