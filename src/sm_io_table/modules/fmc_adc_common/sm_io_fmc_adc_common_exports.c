/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "sm_io_exports_helper.h"
#include "sm_io_codes.h"

/* Description SMIO FMC_ADC_COMMON functions */

disp_op_t fmc_adc_common_leds_exp = {
    .name = FMC_ADC_COMMON_NAME_LEDS,
    .opcode = FMC_ADC_COMMON_OPCODE_LEDS,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc_adc_common_mmcm_rst_exp = {
    .name = FMC_ADC_COMMON_NAME_MMCM_RST,
    .opcode = FMC_ADC_COMMON_OPCODE_MMCM_RST,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc_adc_common_test_data_en_exp = {
    .name = FMC_ADC_COMMON_NAME_TEST_DATA_EN,
    .opcode = FMC_ADC_COMMON_OPCODE_TEST_DATA_EN,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc_adc_common_trig_dir_exp = {
    .name = FMC_ADC_COMMON_NAME_TRIG_DIR,
    .opcode = FMC_ADC_COMMON_OPCODE_TRIG_DIR,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc_adc_common_trig_term_exp = {
    .name = FMC_ADC_COMMON_NAME_TRIG_TERM,
    .opcode = FMC_ADC_COMMON_OPCODE_TRIG_TERM,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmc_adc_common_trig_val_exp = {
    .name = FMC_ADC_COMMON_NAME_TRIG_VAL,
    .opcode = FMC_ADC_COMMON_OPCODE_TRIG_VAL,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

/* Exported function description */
const disp_op_t *fmc_adc_common_exp_ops [] = {
    &fmc_adc_common_leds_exp,
    &fmc_adc_common_mmcm_rst_exp,
    &fmc_adc_common_test_data_en_exp,
    &fmc_adc_common_trig_dir_exp,
    &fmc_adc_common_trig_term_exp,
    &fmc_adc_common_trig_val_exp,
    NULL
};

