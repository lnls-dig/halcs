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

disp_op_t fmc250m_4ch_adc_data_dly0_exp = {
    .name = FMC250M_4CH_NAME_ADC_DATA_DLY0,
    .opcode = FMC250M_4CH_OPCODE_ADC_DATA_DLY0,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc250m_4ch_adc_data_dly1_exp = {
    .name = FMC250M_4CH_NAME_ADC_DATA_DLY1,
    .opcode = FMC250M_4CH_OPCODE_ADC_DATA_DLY1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc250m_4ch_adc_data_dly2_exp = {
    .name = FMC250M_4CH_NAME_ADC_DATA_DLY2,
    .opcode = FMC250M_4CH_OPCODE_ADC_DATA_DLY2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc250m_4ch_adc_data_dly3_exp = {
    .name = FMC250M_4CH_NAME_ADC_DATA_DLY3,
    .opcode = FMC250M_4CH_OPCODE_ADC_DATA_DLY3,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc250m_4ch_adc_clk_dly0_exp = {
    .name = FMC250M_4CH_NAME_ADC_CLK_DLY0,
    .opcode = FMC250M_4CH_OPCODE_ADC_CLK_DLY0,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc250m_4ch_adc_clk_dly1_exp = {
    .name = FMC250M_4CH_NAME_ADC_CLK_DLY1,
    .opcode = FMC250M_4CH_OPCODE_ADC_CLK_DLY1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc250m_4ch_adc_clk_dly2_exp = {
    .name = FMC250M_4CH_NAME_ADC_CLK_DLY2,
    .opcode = FMC250M_4CH_OPCODE_ADC_CLK_DLY2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc250m_4ch_adc_clk_dly3_exp = {
    .name = FMC250M_4CH_NAME_ADC_CLK_DLY3,
    .opcode = FMC250M_4CH_OPCODE_ADC_CLK_DLY3,
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

disp_op_t fmc250m_4ch_temp_exp = {
    .name = FMC250M_4CH_NAME_TEMP,
    .opcode = FMC250M_4CH_OPCODE_TEMP,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc250m_4ch_cal_status_exp = {
    .name = FMC250M_4CH_NAME_CAL_STATUS,
    .opcode = FMC250M_4CH_OPCODE_CAL_STATUS,
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
const disp_op_t *fmc250m_4ch_exp_ops [] = {
    &fmc250m_4ch_adc_data0_exp,
    &fmc250m_4ch_adc_data1_exp,
    &fmc250m_4ch_adc_data2_exp,
    &fmc250m_4ch_adc_data3_exp,
    &fmc250m_4ch_adc_data_dly0_exp,
    &fmc250m_4ch_adc_data_dly1_exp,
    &fmc250m_4ch_adc_data_dly2_exp,
    &fmc250m_4ch_adc_data_dly3_exp,
    &fmc250m_4ch_adc_clk_dly0_exp,
    &fmc250m_4ch_adc_clk_dly1_exp,
    &fmc250m_4ch_adc_clk_dly2_exp,
    &fmc250m_4ch_adc_clk_dly3_exp,
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
    &fmc250m_4ch_rst_adcs_exp,
    &fmc250m_4ch_rst_div_adcs_exp,
    &fmc250m_4ch_sleep_adcs_exp,
    &fmc250m_4ch_test_mode_exp,
    &fmc250m_4ch_rst_exp,
    &fmc250m_4ch_portconfig_exp,
    &fmc250m_4ch_reg_exp,
    &fmc250m_4ch_temp_exp,
    &fmc250m_4ch_cal_status_exp,
    NULL
};

