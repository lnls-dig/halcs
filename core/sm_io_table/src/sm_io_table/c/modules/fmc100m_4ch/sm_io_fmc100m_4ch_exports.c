/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "sm_io_codes.h"
#include "sm_io_exports_helper.h"

/* Description SMIO FMC_100M_4CH functions */

disp_op_t fmc100m_4ch_si571_oe_exp = {
    .name = FMC_100M_4CH_NAME_SI571_OE,
    .opcode = FMC_100M_4CH_OPCODE_SI571_OE,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc100m_4ch_acq_led_exp = {
    .name = FMC_100M_4CH_NAME_ACQ_LED,
    .opcode = FMC_100M_4CH_OPCODE_ACQ_LED,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc100m_4ch_status_exp = {
    .name = FMC_100M_4CH_NAME_STATUS,
    .opcode = FMC_100M_4CH_OPCODE_STATUS,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc100m_4ch_adc_status_exp = {
    .name = FMC_100M_4CH_NAME_STATUS,
    .opcode = FMC_100M_4CH_OPCODE_ADC_STATUS,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc100m_4ch_adc_calib_gain_exp = {
    .name = FMC_100M_4CH_NAME_ADC_CALIB_GAIN,
    .opcode = FMC_100M_4CH_OPCODE_ADC_CALIB_GAIN,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc100m_4ch_adc_calib_offset_exp = {
    .name = FMC_100M_4CH_NAME_ADC_CALIB_OFFSET,
    .opcode = FMC_100M_4CH_OPCODE_ADC_CALIB_OFFSET,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc100m_4ch_adc_sat_exp = {
    .name = FMC_100M_4CH_NAME_ADC_SAT,
    .opcode = FMC_100M_4CH_OPCODE_ADC_SAT,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc100m_4ch_adc_ssr_exp = {
    .name = FMC_100M_4CH_NAME_ADC_SSR,
    .opcode = FMC_100M_4CH_OPCODE_ADC_SSR,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc100m_4ch_fs_freq_exp = {
    .name = FMC_100M_4CH_NAME_FS_FREQ,
    .opcode = FMC_100M_4CH_OPCODE_FS_FREQ,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc100m_4ch_si571_freq_exp = {
    .name = FMC_100M_4CH_NAME_SI571_FREQ,
    .opcode = FMC_100M_4CH_OPCODE_SI571_FREQ,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
        DISP_ARG_END
    }
};

disp_op_t fmc100m_4ch_si571_get_defaults_exp = {
    .name = FMC_100M_4CH_NAME_SI571_GET_DEFAULTS,
    .opcode = FMC_100M_4CH_OPCODE_SI571_GET_DEFAULTS,
    .retval = DISP_ARG_END,
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
        DISP_ARG_END
    }
};

disp_op_t fmc100m_4ch_si571_fstartup_exp = {
    .name = FMC_100M_4CH_NAME_SI571_FSTARTUP,
    .opcode = FMC_100M_4CH_OPCODE_SI571_FSTARTUP,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
        DISP_ARG_END
    }
};

disp_op_t fmc100m_4ch_ltc2174_test_patt_exp = {
    .name = FMC_100M_4CH_NAME_LTC2174_TEST_PATT,
    .opcode = FMC_100M_4CH_OPCODE_LTC2174_TEST_PATT,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc100m_4ch_ltc2174_test_patt_data_exp = {
    .name = FMC_100M_4CH_NAME_LTC2174_TEST_PATT_DATA,
    .opcode = FMC_100M_4CH_OPCODE_LTC2174_TEST_PATT_DATA,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc100m_4ch_ltc2174_reset_exp = {
    .name = FMC_100M_4CH_NAME_LTC2174_RESET,
    .opcode = FMC_100M_4CH_OPCODE_LTC2174_RESET,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc100m_4ch_ltc2174_twos_compl_exp = {
    .name = FMC_100M_4CH_NAME_LTC2174_TWOS_COMPL,
    .opcode = FMC_100M_4CH_OPCODE_LTC2174_TWOS_COMPL,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

/* Exported function description */
const disp_op_t *fmc100m_4ch_exp_ops [] = {
    &fmc100m_4ch_si571_oe_exp,
    &fmc100m_4ch_acq_led_exp,
    &fmc100m_4ch_status_exp,
    &fmc100m_4ch_adc_status_exp,
    &fmc100m_4ch_adc_calib_gain_exp,
    &fmc100m_4ch_adc_calib_offset_exp,
    &fmc100m_4ch_adc_sat_exp,
    &fmc100m_4ch_adc_ssr_exp,
    &fmc100m_4ch_fs_freq_exp,
    &fmc100m_4ch_si571_freq_exp,
    &fmc100m_4ch_si571_get_defaults_exp,
    &fmc100m_4ch_si571_fstartup_exp,
    &fmc100m_4ch_ltc2174_test_patt_exp,
    &fmc100m_4ch_ltc2174_test_patt_data_exp,
    &fmc100m_4ch_ltc2174_reset_exp,
    &fmc100m_4ch_ltc2174_twos_compl_exp,
    NULL
};

