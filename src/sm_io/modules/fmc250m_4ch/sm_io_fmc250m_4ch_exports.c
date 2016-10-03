/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "sm_io_exports_helper.h"
#include "sm_io_codes.h"

/* Description SMIO FMC250M_4CH functions */

disp_op_t fmc250m_4ch_rst_adcs_exp = {
    .name = FMC250M_4CH_NAME_RST_ADCS,
    .opcode = FMC250M_4CH_OPCODE_RST_ADCS,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc250m_4ch_rst_div_adcs_exp = {
    .name = FMC250M_4CH_NAME_RST_DIV_ADCS,
    .opcode = FMC250M_4CH_OPCODE_RST_DIV_ADCS,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc250m_4ch_sleep_adcs_exp = {
    .name = FMC250M_4CH_NAME_SLEEP_ADCS,
    .opcode = FMC250M_4CH_OPCODE_SLEEP_ADCS,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

#if 0
disp_op_t fmc250m_4ch_adc_rand_exp = {
    .name = FMC250M_4CH_NAME_ADC_RAND,
    .opcode = FMC250M_4CH_OPCODE_ADC_RAND,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc250m_4ch_adc_dith_exp = {
    .name = FMC250M_4CH_NAME_ADC_DITH,
    .opcode = FMC250M_4CH_OPCODE_ADC_DITH,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc250m_4ch_adc_shdn_exp = {
    .name = FMC250M_4CH_NAME_ADC_SHDN,
    .opcode = FMC250M_4CH_OPCODE_ADC_SHDN,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc250m_4ch_adc_pga_exp = {
    .name = FMC250M_4CH_NAME_ADC_PGA,
    .opcode = FMC250M_4CH_OPCODE_ADC_PGA,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};
#endif

disp_op_t fmc250m_4ch_adc_data0_exp = {
    .name = FMC250M_4CH_NAME_ADC_DATA0,
    .opcode = FMC250M_4CH_OPCODE_ADC_DATA0,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc250m_4ch_adc_data1_exp = {
    .name = FMC250M_4CH_NAME_ADC_DATA1,
    .opcode = FMC250M_4CH_OPCODE_ADC_DATA1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc250m_4ch_adc_data2_exp = {
    .name = FMC250M_4CH_NAME_ADC_DATA2,
    .opcode = FMC250M_4CH_OPCODE_ADC_DATA2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc250m_4ch_adc_data3_exp = {
    .name = FMC250M_4CH_NAME_ADC_DATA3,
    .opcode = FMC250M_4CH_OPCODE_ADC_DATA3,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

#if 0
disp_op_t fmc250m_4ch_adc_dly_val0_exp = {
    .name = FMC250M_4CH_NAME_ADC_DLY_VAL0,
    .opcode = FMC250M_4CH_OPCODE_ADC_DLY_VAL0,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc250m_4ch_adc_dly_val1_exp = {
    .name = FMC250M_4CH_NAME_ADC_DLY_VAL1,
    .opcode = FMC250M_4CH_OPCODE_ADC_DLY_VAL1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc250m_4ch_adc_dly_val2_exp = {
    .name = FMC250M_4CH_NAME_ADC_DLY_VAL2,
    .opcode = FMC250M_4CH_OPCODE_ADC_DLY_VAL2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc250m_4ch_adc_dly_val3_exp = {
    .name = FMC250M_4CH_NAME_ADC_DLY_VAL3,
    .opcode = FMC250M_4CH_OPCODE_ADC_DLY_VAL3,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc250m_4ch_adc_dly_line0_exp = {
    .name = FMC250M_4CH_NAME_ADC_DLY_LINE0,
    .opcode = FMC250M_4CH_OPCODE_ADC_DLY_LINE0,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc250m_4ch_adc_dly_line1_exp = {
    .name = FMC250M_4CH_NAME_ADC_DLY_LINE1,
    .opcode = FMC250M_4CH_OPCODE_ADC_DLY_LINE1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};


disp_op_t fmc250m_4ch_adc_dly_line2_exp = {
    .name = FMC250M_4CH_NAME_ADC_DLY_LINE2,
    .opcode = FMC250M_4CH_OPCODE_ADC_DLY_LINE2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc250m_4ch_adc_dly_line3_exp = {
    .name = FMC250M_4CH_NAME_ADC_DLY_LINE3,
    .opcode = FMC250M_4CH_OPCODE_ADC_DLY_LINE3,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc250m_4ch_adc_dly_updt0_exp = {
    .name = FMC250M_4CH_NAME_ADC_DLY_UPDT0,
    .opcode = FMC250M_4CH_OPCODE_ADC_DLY_UPDT0,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc250m_4ch_adc_dly_updt1_exp = {
    .name = FMC250M_4CH_NAME_ADC_DLY_UPDT1,
    .opcode = FMC250M_4CH_OPCODE_ADC_DLY_UPDT1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc250m_4ch_adc_dly_updt2_exp = {
    .name = FMC250M_4CH_NAME_ADC_DLY_UPDT2,
    .opcode = FMC250M_4CH_OPCODE_ADC_DLY_UPDT2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc250m_4ch_adc_dly_updt3_exp = {
    .name = FMC250M_4CH_NAME_ADC_DLY_UPDT3,
    .opcode = FMC250M_4CH_OPCODE_ADC_DLY_UPDT3,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc250m_4ch_adc_dly0_exp = {
    .name = FMC250M_4CH_NAME_ADC_DLY0,
    .opcode = FMC250M_4CH_OPCODE_ADC_DLY0,
    .retval = DISP_ARG_END,
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc250m_4ch_adc_dly1_exp = {
    .name = FMC250M_4CH_NAME_ADC_DLY1,
    .opcode = FMC250M_4CH_OPCODE_ADC_DLY1,
    .retval = DISP_ARG_END,
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc250m_4ch_adc_dly2_exp = {
    .name = FMC250M_4CH_NAME_ADC_DLY2,
    .opcode = FMC250M_4CH_OPCODE_ADC_DLY2,
    .retval = DISP_ARG_END,
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc250m_4ch_adc_dly3_exp = {
    .name = FMC250M_4CH_NAME_ADC_DLY3,
    .opcode = FMC250M_4CH_OPCODE_ADC_DLY3,
    .retval = DISP_ARG_END,
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};
#endif

disp_op_t fmc250m_4ch_test_mode_exp = {
    .name = FMC250M_4CH_NAME_TESTMODE,
    .opcode = FMC250M_4CH_OPCODE_TESTMODE,
    .retval = DISP_ARG_END,
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc250m_4ch_rst_exp = {
    .name = FMC250M_4CH_NAME_RST,
    .opcode = FMC250M_4CH_OPCODE_RST,
    .retval = DISP_ARG_END,
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc250m_4ch_portconfig_exp = {
    .name = FMC250M_4CH_NAME_PORTCONFIG,
    .opcode = FMC250M_4CH_OPCODE_PORTCONFIG,
    .retval = DISP_ARG_END,
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc250m_4ch_reg_exp = {
    .name = FMC250M_4CH_NAME_REG,
    .opcode = FMC250M_4CH_OPCODE_REG,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

/* Exported function description */
const disp_op_t *fmc250m_4ch_exp_ops [] = {
#if 0
    &fmc250m_4ch_adc_rand_exp,
    &fmc250m_4ch_adc_dith_exp,
    &fmc250m_4ch_adc_shdn_exp,
    &fmc250m_4ch_adc_pga_exp,
#endif
    &fmc250m_4ch_adc_data0_exp,
    &fmc250m_4ch_adc_data1_exp,
    &fmc250m_4ch_adc_data2_exp,
    &fmc250m_4ch_adc_data3_exp,
#if 0
    &fmc250m_4ch_adc_dly_val0_exp,
    &fmc250m_4ch_adc_dly_val1_exp,
    &fmc250m_4ch_adc_dly_val2_exp,
    &fmc250m_4ch_adc_dly_val3_exp,
    &fmc250m_4ch_adc_dly_line0_exp,
    &fmc250m_4ch_adc_dly_line1_exp,
    &fmc250m_4ch_adc_dly_line2_exp,
    &fmc250m_4ch_adc_dly_line3_exp,
    &fmc250m_4ch_adc_dly_updt0_exp,
    &fmc250m_4ch_adc_dly_updt1_exp,
    &fmc250m_4ch_adc_dly_updt2_exp,
    &fmc250m_4ch_adc_dly_updt3_exp,
    &fmc250m_4ch_adc_dly0_exp,
    &fmc250m_4ch_adc_dly1_exp,
    &fmc250m_4ch_adc_dly2_exp,
    &fmc250m_4ch_adc_dly3_exp,
#endif
    &fmc250m_4ch_rst_adcs_exp,
    &fmc250m_4ch_rst_div_adcs_exp,
    &fmc250m_4ch_sleep_adcs_exp,
    &fmc250m_4ch_test_mode_exp,
    &fmc250m_4ch_rst_exp,
    &fmc250m_4ch_portconfig_exp,
    &fmc250m_4ch_reg_exp,
    NULL
};

