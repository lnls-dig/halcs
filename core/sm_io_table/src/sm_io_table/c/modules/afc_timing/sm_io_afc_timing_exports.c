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

disp_op_t afc_timing_set_get_amc_en0_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_EN0,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_EN0,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_pol0_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_POL0,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_POL0,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_log0_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_LOG0,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_LOG0,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_itl0_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_ITL0,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_ITL0,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_src0_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_SRC0,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_SRC0,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_dir0_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_DIR0,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_DIR0,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_pulses0_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_PULSES0,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_PULSES0,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_evt0_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_EVT0,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_EVT0,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_dly0_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_DLY0,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_DLY0,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_wdt0_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_WDT0,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_WDT0,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_en1_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_EN1,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_EN1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_pol1_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_POL1,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_POL1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_log1_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_LOG1,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_LOG1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_itl1_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_ITL1,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_ITL1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_src1_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_SRC1,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_SRC1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_dir1_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_DIR1,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_DIR1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_pulses1_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_PULSES1,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_PULSES1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_evt1_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_EVT1,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_EVT1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_dly1_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_DLY1,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_DLY1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_wdt1_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_WDT1,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_WDT1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_en2_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_EN2,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_EN2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_pol2_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_POL2,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_POL2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_log2_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_LOG2,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_LOG2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_itl2_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_ITL2,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_ITL2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_src2_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_SRC2,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_SRC2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_dir2_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_DIR2,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_DIR2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_pulses2_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_PULSES2,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_PULSES2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_evt2_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_EVT2,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_EVT2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_dly2_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_DLY2,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_DLY2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_wdt2_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_WDT2,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_WDT2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_en3_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_EN3,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_EN3,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_pol3_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_POL3,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_POL3,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_log3_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_LOG3,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_LOG3,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_itl3_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_ITL3,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_ITL3,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_src3_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_SRC3,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_SRC3,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_dir3_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_DIR3,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_DIR3,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_pulses3_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_PULSES3,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_PULSES3,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_evt3_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_EVT3,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_EVT3,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_dly3_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_DLY3,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_DLY3,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_wdt3_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_WDT3,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_WDT3,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_en4_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_EN4,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_EN4,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_pol4_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_POL4,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_POL4,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_log4_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_LOG4,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_LOG4,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_itl4_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_ITL4,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_ITL4,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_src4_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_SRC4,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_SRC4,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_dir4_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_DIR4,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_DIR4,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_pulses4_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_PULSES4,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_PULSES4,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_evt4_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_EVT4,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_EVT4,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_dly4_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_DLY4,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_DLY4,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_wdt4_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_WDT4,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_WDT4,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_en5_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_EN5,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_EN5,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_pol5_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_POL5,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_POL5,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_log5_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_LOG5,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_LOG5,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_itl5_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_ITL5,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_ITL5,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_src5_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_SRC5,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_SRC5,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_dir5_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_DIR5,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_DIR5,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_pulses5_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_PULSES5,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_PULSES5,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_evt5_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_EVT5,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_EVT5,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_dly5_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_DLY5,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_DLY5,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_wdt5_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_WDT5,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_WDT5,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_en6_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_EN6,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_EN6,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_pol6_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_POL6,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_POL6,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_log6_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_LOG6,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_LOG6,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_itl6_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_ITL6,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_ITL6,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_src6_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_SRC6,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_SRC6,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_dir6_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_DIR6,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_DIR6,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_pulses6_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_PULSES6,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_PULSES6,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_evt6_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_EVT6,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_EVT6,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_dly6_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_DLY6,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_DLY6,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_wdt6_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_WDT6,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_WDT6,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_en7_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_EN7,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_EN7,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_pol7_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_POL7,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_POL7,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_log7_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_LOG7,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_LOG7,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_itl7_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_ITL7,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_ITL7,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_src7_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_SRC7,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_SRC7,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_dir7_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_DIR7,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_DIR7,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_pulses7_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_PULSES7,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_PULSES7,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_evt7_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_EVT7,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_EVT7,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_dly7_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_DLY7,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_DLY7,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_amc_wdt7_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AMC_WDT7,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AMC_WDT7,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_en0_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_EN0,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_EN0,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_pol0_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_POL0,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_POL0,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_log0_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_LOG0,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_LOG0,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_itl0_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_ITL0,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_ITL0,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_src0_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_SRC0,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_SRC0,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_pulses0_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_PULSES0,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_PULSES0,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_evt0_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_EVT0,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_EVT0,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_dly0_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_DLY0,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_DLY0,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_wdt0_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_WDT0,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_WDT0,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_en1_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_EN1,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_EN1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_pol1_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_POL1,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_POL1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_log1_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_LOG1,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_LOG1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_itl1_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_ITL1,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_ITL1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_src1_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_SRC1,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_SRC1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_pulses1_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_PULSES1,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_PULSES1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_evt1_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_EVT1,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_EVT1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_dly1_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_DLY1,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_DLY1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_wdt1_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_WDT1,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_WDT1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_en2_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_EN2,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_EN2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_pol2_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_POL2,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_POL2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_log2_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_LOG2,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_LOG2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_itl2_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_ITL2,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_ITL2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_src2_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_SRC2,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_SRC2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_pulses2_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_PULSES2,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_PULSES2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_evt2_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_EVT2,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_EVT2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_dly2_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_DLY2,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_DLY2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_wdt2_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_WDT2,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_WDT2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_en3_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_EN3,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_EN3,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_pol3_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_POL3,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_POL3,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_log3_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_LOG3,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_LOG3,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_itl3_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_ITL3,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_ITL3,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_src3_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_SRC3,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_SRC3,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_pulses3_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_PULSES3,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_PULSES3,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_evt3_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_EVT3,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_EVT3,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_dly3_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_DLY3,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_DLY3,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_wdt3_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_WDT3,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_WDT3,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_en4_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_EN4,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_EN4,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_pol4_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_POL4,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_POL4,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_log4_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_LOG4,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_LOG4,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_itl4_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_ITL4,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_ITL4,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_src4_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_SRC4,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_SRC4,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_pulses4_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_PULSES4,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_PULSES4,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_evt4_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_EVT4,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_EVT4,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_dly4_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_DLY4,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_DLY4,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc1_wdt4_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC1_WDT4,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC1_WDT4,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_en0_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_EN0,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_EN0,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_pol0_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_POL0,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_POL0,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_log0_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_LOG0,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_LOG0,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_itl0_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_ITL0,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_ITL0,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_src0_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_SRC0,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_SRC0,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_pulses0_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_PULSES0,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_PULSES0,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_evt0_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_EVT0,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_EVT0,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_dly0_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_DLY0,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_DLY0,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_wdt0_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_WDT0,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_WDT0,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_en1_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_EN1,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_EN1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_pol1_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_POL1,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_POL1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_log1_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_LOG1,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_LOG1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_itl1_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_ITL1,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_ITL1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_src1_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_SRC1,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_SRC1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_pulses1_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_PULSES1,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_PULSES1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_evt1_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_EVT1,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_EVT1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_dly1_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_DLY1,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_DLY1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_wdt1_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_WDT1,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_WDT1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_en2_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_EN2,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_EN2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_pol2_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_POL2,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_POL2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_log2_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_LOG2,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_LOG2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_itl2_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_ITL2,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_ITL2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_src2_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_SRC2,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_SRC2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_pulses2_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_PULSES2,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_PULSES2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_evt2_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_EVT2,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_EVT2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_dly2_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_DLY2,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_DLY2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_wdt2_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_WDT2,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_WDT2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_en3_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_EN3,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_EN3,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_pol3_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_POL3,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_POL3,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_log3_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_LOG3,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_LOG3,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_itl3_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_ITL3,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_ITL3,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_src3_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_SRC3,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_SRC3,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_pulses3_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_PULSES3,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_PULSES3,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_evt3_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_EVT3,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_EVT3,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_dly3_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_DLY3,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_DLY3,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_wdt3_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_WDT3,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_WDT3,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_en4_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_EN4,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_EN4,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_pol4_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_POL4,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_POL4,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_log4_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_LOG4,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_LOG4,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_itl4_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_ITL4,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_ITL4,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_src4_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_SRC4,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_SRC4,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_pulses4_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_PULSES4,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_PULSES4,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_evt4_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_EVT4,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_EVT4,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_dly4_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_DLY4,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_DLY4,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_timing_set_get_fmc2_wdt4_exp = {
    .name = AFC_TIMING_NAME_SET_GET_FMC2_WDT4,
    .opcode = AFC_TIMING_OPCODE_SET_GET_FMC2_WDT4,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
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

disp_op_t afc_timing_set_get_rtm_phase_set_exp = {
    .name = AFC_TIMING_NAME_SET_GET_RTM_PHASE_SET,
    .opcode = AFC_TIMING_OPCODE_SET_GET_RTM_PHASE_SET,
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

disp_op_t afc_timing_set_get_afc_phase_set_exp = {
    .name = AFC_TIMING_NAME_SET_GET_AFC_PHASE_SET,
    .opcode = AFC_TIMING_OPCODE_SET_GET_AFC_PHASE_SET,
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
  &afc_timing_set_get_amc_en0_exp,
  &afc_timing_set_get_amc_pol0_exp,
  &afc_timing_set_get_amc_log0_exp,
  &afc_timing_set_get_amc_itl0_exp,
  &afc_timing_set_get_amc_src0_exp,
  &afc_timing_set_get_amc_dir0_exp,
  &afc_timing_set_get_amc_pulses0_exp,
  &afc_timing_set_get_amc_evt0_exp,
  &afc_timing_set_get_amc_dly0_exp,
  &afc_timing_set_get_amc_wdt0_exp,
  &afc_timing_set_get_amc_en1_exp,
  &afc_timing_set_get_amc_pol1_exp,
  &afc_timing_set_get_amc_log1_exp,
  &afc_timing_set_get_amc_itl1_exp,
  &afc_timing_set_get_amc_src1_exp,
  &afc_timing_set_get_amc_dir1_exp,
  &afc_timing_set_get_amc_pulses1_exp,
  &afc_timing_set_get_amc_evt1_exp,
  &afc_timing_set_get_amc_dly1_exp,
  &afc_timing_set_get_amc_wdt1_exp,
  &afc_timing_set_get_amc_en2_exp,
  &afc_timing_set_get_amc_pol2_exp,
  &afc_timing_set_get_amc_log2_exp,
  &afc_timing_set_get_amc_itl2_exp,
  &afc_timing_set_get_amc_src2_exp,
  &afc_timing_set_get_amc_dir2_exp,
  &afc_timing_set_get_amc_pulses2_exp,
  &afc_timing_set_get_amc_evt2_exp,
  &afc_timing_set_get_amc_dly2_exp,
  &afc_timing_set_get_amc_wdt2_exp,
  &afc_timing_set_get_amc_en3_exp,
  &afc_timing_set_get_amc_pol3_exp,
  &afc_timing_set_get_amc_log3_exp,
  &afc_timing_set_get_amc_itl3_exp,
  &afc_timing_set_get_amc_src3_exp,
  &afc_timing_set_get_amc_dir3_exp,
  &afc_timing_set_get_amc_pulses3_exp,
  &afc_timing_set_get_amc_evt3_exp,
  &afc_timing_set_get_amc_dly3_exp,
  &afc_timing_set_get_amc_wdt3_exp,
  &afc_timing_set_get_amc_en4_exp,
  &afc_timing_set_get_amc_pol4_exp,
  &afc_timing_set_get_amc_log4_exp,
  &afc_timing_set_get_amc_itl4_exp,
  &afc_timing_set_get_amc_src4_exp,
  &afc_timing_set_get_amc_dir4_exp,
  &afc_timing_set_get_amc_pulses4_exp,
  &afc_timing_set_get_amc_evt4_exp,
  &afc_timing_set_get_amc_dly4_exp,
  &afc_timing_set_get_amc_wdt4_exp,
  &afc_timing_set_get_amc_en5_exp,
  &afc_timing_set_get_amc_pol5_exp,
  &afc_timing_set_get_amc_log5_exp,
  &afc_timing_set_get_amc_itl5_exp,
  &afc_timing_set_get_amc_src5_exp,
  &afc_timing_set_get_amc_dir5_exp,
  &afc_timing_set_get_amc_pulses5_exp,
  &afc_timing_set_get_amc_evt5_exp,
  &afc_timing_set_get_amc_dly5_exp,
  &afc_timing_set_get_amc_wdt5_exp,
  &afc_timing_set_get_amc_en6_exp,
  &afc_timing_set_get_amc_pol6_exp,
  &afc_timing_set_get_amc_log6_exp,
  &afc_timing_set_get_amc_itl6_exp,
  &afc_timing_set_get_amc_src6_exp,
  &afc_timing_set_get_amc_dir6_exp,
  &afc_timing_set_get_amc_pulses6_exp,
  &afc_timing_set_get_amc_evt6_exp,
  &afc_timing_set_get_amc_dly6_exp,
  &afc_timing_set_get_amc_wdt6_exp,
  &afc_timing_set_get_amc_en7_exp,
  &afc_timing_set_get_amc_pol7_exp,
  &afc_timing_set_get_amc_log7_exp,
  &afc_timing_set_get_amc_itl7_exp,
  &afc_timing_set_get_amc_src7_exp,
  &afc_timing_set_get_amc_dir7_exp,
  &afc_timing_set_get_amc_pulses7_exp,
  &afc_timing_set_get_amc_evt7_exp,
  &afc_timing_set_get_amc_dly7_exp,
  &afc_timing_set_get_amc_wdt7_exp,
  &afc_timing_set_get_fmc1_en0_exp,
  &afc_timing_set_get_fmc1_pol0_exp,
  &afc_timing_set_get_fmc1_log0_exp,
  &afc_timing_set_get_fmc1_itl0_exp,
  &afc_timing_set_get_fmc1_src0_exp,
  &afc_timing_set_get_fmc1_pulses0_exp,
  &afc_timing_set_get_fmc1_evt0_exp,
  &afc_timing_set_get_fmc1_dly0_exp,
  &afc_timing_set_get_fmc1_wdt0_exp,
  &afc_timing_set_get_fmc1_en1_exp,
  &afc_timing_set_get_fmc1_pol1_exp,
  &afc_timing_set_get_fmc1_log1_exp,
  &afc_timing_set_get_fmc1_itl1_exp,
  &afc_timing_set_get_fmc1_src1_exp,
  &afc_timing_set_get_fmc1_pulses1_exp,
  &afc_timing_set_get_fmc1_evt1_exp,
  &afc_timing_set_get_fmc1_dly1_exp,
  &afc_timing_set_get_fmc1_wdt1_exp,
  &afc_timing_set_get_fmc1_en2_exp,
  &afc_timing_set_get_fmc1_pol2_exp,
  &afc_timing_set_get_fmc1_log2_exp,
  &afc_timing_set_get_fmc1_itl2_exp,
  &afc_timing_set_get_fmc1_src2_exp,
  &afc_timing_set_get_fmc1_pulses2_exp,
  &afc_timing_set_get_fmc1_evt2_exp,
  &afc_timing_set_get_fmc1_dly2_exp,
  &afc_timing_set_get_fmc1_wdt2_exp,
  &afc_timing_set_get_fmc1_en3_exp,
  &afc_timing_set_get_fmc1_pol3_exp,
  &afc_timing_set_get_fmc1_log3_exp,
  &afc_timing_set_get_fmc1_itl3_exp,
  &afc_timing_set_get_fmc1_src3_exp,
  &afc_timing_set_get_fmc1_pulses3_exp,
  &afc_timing_set_get_fmc1_evt3_exp,
  &afc_timing_set_get_fmc1_dly3_exp,
  &afc_timing_set_get_fmc1_wdt3_exp,
  &afc_timing_set_get_fmc1_en4_exp,
  &afc_timing_set_get_fmc1_pol4_exp,
  &afc_timing_set_get_fmc1_log4_exp,
  &afc_timing_set_get_fmc1_itl4_exp,
  &afc_timing_set_get_fmc1_src4_exp,
  &afc_timing_set_get_fmc1_pulses4_exp,
  &afc_timing_set_get_fmc1_evt4_exp,
  &afc_timing_set_get_fmc1_dly4_exp,
  &afc_timing_set_get_fmc1_wdt4_exp,
  &afc_timing_set_get_fmc2_en0_exp,
  &afc_timing_set_get_fmc2_pol0_exp,
  &afc_timing_set_get_fmc2_log0_exp,
  &afc_timing_set_get_fmc2_itl0_exp,
  &afc_timing_set_get_fmc2_src0_exp,
  &afc_timing_set_get_fmc2_pulses0_exp,
  &afc_timing_set_get_fmc2_evt0_exp,
  &afc_timing_set_get_fmc2_dly0_exp,
  &afc_timing_set_get_fmc2_wdt0_exp,
  &afc_timing_set_get_fmc2_en1_exp,
  &afc_timing_set_get_fmc2_pol1_exp,
  &afc_timing_set_get_fmc2_log1_exp,
  &afc_timing_set_get_fmc2_itl1_exp,
  &afc_timing_set_get_fmc2_src1_exp,
  &afc_timing_set_get_fmc2_pulses1_exp,
  &afc_timing_set_get_fmc2_evt1_exp,
  &afc_timing_set_get_fmc2_dly1_exp,
  &afc_timing_set_get_fmc2_wdt1_exp,
  &afc_timing_set_get_fmc2_en2_exp,
  &afc_timing_set_get_fmc2_pol2_exp,
  &afc_timing_set_get_fmc2_log2_exp,
  &afc_timing_set_get_fmc2_itl2_exp,
  &afc_timing_set_get_fmc2_src2_exp,
  &afc_timing_set_get_fmc2_pulses2_exp,
  &afc_timing_set_get_fmc2_evt2_exp,
  &afc_timing_set_get_fmc2_dly2_exp,
  &afc_timing_set_get_fmc2_wdt2_exp,
  &afc_timing_set_get_fmc2_en3_exp,
  &afc_timing_set_get_fmc2_pol3_exp,
  &afc_timing_set_get_fmc2_log3_exp,
  &afc_timing_set_get_fmc2_itl3_exp,
  &afc_timing_set_get_fmc2_src3_exp,
  &afc_timing_set_get_fmc2_pulses3_exp,
  &afc_timing_set_get_fmc2_evt3_exp,
  &afc_timing_set_get_fmc2_dly3_exp,
  &afc_timing_set_get_fmc2_wdt3_exp,
  &afc_timing_set_get_fmc2_en4_exp,
  &afc_timing_set_get_fmc2_pol4_exp,
  &afc_timing_set_get_fmc2_log4_exp,
  &afc_timing_set_get_fmc2_itl4_exp,
  &afc_timing_set_get_fmc2_src4_exp,
  &afc_timing_set_get_fmc2_pulses4_exp,
  &afc_timing_set_get_fmc2_evt4_exp,
  &afc_timing_set_get_fmc2_dly4_exp,
  &afc_timing_set_get_fmc2_wdt4_exp,
  &afc_timing_set_get_rtm_freq_kp_exp,
  &afc_timing_set_get_rtm_freq_ki_exp,
  &afc_timing_set_get_rtm_phase_kp_exp,
  &afc_timing_set_get_rtm_phase_ki_exp,
  &afc_timing_set_get_rtm_phase_set_exp,
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
  &afc_timing_set_get_afc_phase_set_exp,
  &afc_timing_set_get_afc_phase_navg_exp,
  &afc_timing_set_get_afc_phase_div_exp_exp,
  &afc_timing_set_get_afc_rfreq_hi_exp,
  &afc_timing_set_get_afc_rfreq_lo_exp,
  &afc_timing_set_get_afc_n1_exp,
  &afc_timing_set_get_afc_hs_div_exp,
  NULL
};
