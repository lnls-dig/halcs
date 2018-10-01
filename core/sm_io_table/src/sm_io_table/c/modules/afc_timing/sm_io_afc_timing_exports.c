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

disp_op_t afc_timing_set_get_evren_exp = {
    .name = AFC_TIMING_NAME_SET_GET_EVREN,
    .opcode = AFC_TIMING_OPCODE_SET_GET_EVREN,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_alive_exp = {
    .name = AFC_TIMING_NAME_SET_GET_ALIVE,
    .opcode = AFC_TIMING_OPCODE_SET_GET_ALIVE,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_freq_sample_rate_prescale_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FREQ_SAMPLE_RATE_PRESCALE,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FREQ_SAMPLE_RATE_PRESCALE,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_en_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_EN,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_EN,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_pol_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_POL,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_POL,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_log_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_LOG,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_LOG,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_itl_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_ITL,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_ITL,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_src_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_SRC,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_SRC,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_dir_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_DIR,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_DIR,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_pulses_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_PULSES,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_PULSES,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_evt_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_EVT,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_EVT,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_dly_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_DLY,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_DLY,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_wdt_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_WDT,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_WDT,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_en_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_EN,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_EN,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_pol_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_POL,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_POL,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_log_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_LOG,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_LOG,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_itl_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_ITL,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_ITL,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_src_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_SRC,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_SRC,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_dir_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_DIR,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_DIR,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_pulses_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_PULSES,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_PULSES,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_evt_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_EVT,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_EVT,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_dly_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_DLY,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_DLY,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_wdt_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_WDT,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_WDT,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_en_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_EN,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_EN,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_pol_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_POL,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_POL,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_log_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_LOG,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_LOG,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_itl_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_ITL,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_ITL,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_src_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_SRC,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_SRC,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_dir_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_DIR,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_DIR,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_pulses_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_PULSES,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_PULSES,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_evt_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_EVT,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_EVT,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_dly_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_DLY,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_DLY,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_wdt_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_WDT,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_WDT,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_rtm_freq_kp_exp = {
    .name = AFC_TIMING_NAME_SET_GET_RTM_FREQ_KP,
    .opcode = AFC_TIMING_OPCODE_SET_GET_RTM_FREQ_KP,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_rtm_freq_ki_exp = {
    .name = AFC_TIMING_NAME_SET_GET_RTM_FREQ_KI,
    .opcode = AFC_TIMING_OPCODE_SET_GET_RTM_FREQ_KI,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_rtm_phase_kp_exp = {
    .name = AFC_TIMING_NAME_SET_GET_RTM_PHASE_KP,
    .opcode = AFC_TIMING_OPCODE_SET_GET_RTM_PHASE_KP,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_rtm_phase_ki_exp = {
    .name = AFC_TIMING_NAME_SET_GET_RTM_PHASE_KI,
    .opcode = AFC_TIMING_OPCODE_SET_GET_RTM_PHASE_KI,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_rtm_phase_navg_exp = {
    .name = AFC_TIMING_NAME_SET_GET_RTM_PHASE_NAVG,
    .opcode = AFC_TIMING_OPCODE_SET_GET_RTM_PHASE_NAVG,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_rtm_phase_div_exp_exp = {
    .name = AFC_TIMING_NAME_SET_GET_RTM_PHASE_DIV_EXP,
    .opcode = AFC_TIMING_OPCODE_SET_GET_RTM_PHASE_DIV_EXP,
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

disp_op_t afc_timing_set_get_afc_freq_kp_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AFC_FREQ_KP,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AFC_FREQ_KP,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_afc_freq_ki_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AFC_FREQ_KI,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AFC_FREQ_KI,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_afc_phase_kp_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AFC_PHASE_KP,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AFC_PHASE_KP,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_afc_phase_ki_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AFC_PHASE_KI,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AFC_PHASE_KI,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_afc_phase_navg_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AFC_PHASE_NAVG,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AFC_PHASE_NAVG,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_afc_phase_div_exp_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AFC_PHASE_DIV_EXP,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AFC_PHASE_DIV_EXP,
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
  &afc_timing_set_get_evren_exp,
  &afc_timing_set_get_alive_exp,
  &afc_timing_set_get_freq_sample_rate_prescale_exp,
  &afc_timing_set_get_amc_en_exp,
  &afc_timing_set_get_amc_pol_exp,
  &afc_timing_set_get_amc_log_exp,
  &afc_timing_set_get_amc_itl_exp,
  &afc_timing_set_get_amc_src_exp,
  &afc_timing_set_get_amc_dir_exp,
  &afc_timing_set_get_amc_pulses_exp,
  &afc_timing_set_get_amc_evt_exp,
  &afc_timing_set_get_amc_dly_exp,
  &afc_timing_set_get_amc_wdt_exp,
  &afc_timing_set_get_fmc1_en_exp,
  &afc_timing_set_get_fmc1_pol_exp,
  &afc_timing_set_get_fmc1_log_exp,
  &afc_timing_set_get_fmc1_itl_exp,
  &afc_timing_set_get_fmc1_src_exp,
  &afc_timing_set_get_fmc1_dir_exp,
  &afc_timing_set_get_fmc1_pulses_exp,
  &afc_timing_set_get_fmc1_evt_exp,
  &afc_timing_set_get_fmc1_dly_exp,
  &afc_timing_set_get_fmc1_wdt_exp,
  &afc_timing_set_get_fmc2_en_exp,
  &afc_timing_set_get_fmc2_pol_exp,
  &afc_timing_set_get_fmc2_log_exp,
  &afc_timing_set_get_fmc2_itl_exp,
  &afc_timing_set_get_fmc2_src_exp,
  &afc_timing_set_get_fmc2_dir_exp,
  &afc_timing_set_get_fmc2_pulses_exp,
  &afc_timing_set_get_fmc2_evt_exp,
  &afc_timing_set_get_fmc2_dly_exp,
  &afc_timing_set_get_fmc2_wdt_exp,
  &afc_timing_set_get_rtm_freq_kp_exp,
  &afc_timing_set_get_rtm_freq_ki_exp,
  &afc_timing_set_get_rtm_phase_kp_exp,
  &afc_timing_set_get_rtm_phase_ki_exp,
  &afc_timing_set_get_rtm_phase_navg_exp,
  &afc_timing_set_get_rtm_phase_div_exp_exp,
  &afc_timing_set_get_rtm_rfreq_hi_exp,
  &afc_timing_set_get_rtm_rfreq_lo_exp,
  &afc_timing_set_get_rtm_n1_exp,
  &afc_timing_set_get_rtm_hs_div_exp,
  &afc_timing_set_get_afc_freq_kp_exp,
  &afc_timing_set_get_afc_freq_ki_exp,
  &afc_timing_set_get_afc_phase_kp_exp,
  &afc_timing_set_get_afc_phase_ki_exp,
  &afc_timing_set_get_afc_phase_navg_exp,
  &afc_timing_set_get_afc_phase_div_exp_exp,
  &afc_timing_set_get_afc_rfreq_hi_exp,
  &afc_timing_set_get_afc_rfreq_lo_exp,
  &afc_timing_set_get_afc_n1_exp,
  &afc_timing_set_get_afc_hs_div_exp,
  NULL
};
