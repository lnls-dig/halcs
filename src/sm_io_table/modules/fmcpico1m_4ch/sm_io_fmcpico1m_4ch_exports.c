/*
 * Copyright (C) 2017 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "sm_io_exports_helper.h"
#include "sm_io_codes.h"

/* Description SMIO FMCPICO1M_4CH functions */

disp_op_t fmcpico1m_4ch_set_get_led1_exp = {
    .name = FMCPICO1M_4CH_NAME_SET_GET_LED1,
    .opcode = FMCPICO1M_4CH_OPCODE_SET_GET_LED1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmcpico1m_4ch_set_get_led2_exp = {
    .name = FMCPICO1M_4CH_NAME_SET_GET_LED2,
    .opcode = FMCPICO1M_4CH_OPCODE_SET_GET_LED2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmcpico1m_4ch_set_get_rng_r0_exp = {
    .name = FMCPICO1M_4CH_NAME_SET_GET_RNG_R0,
    .opcode = FMCPICO1M_4CH_OPCODE_SET_GET_RNG_R0,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmcpico1m_4ch_set_get_rng_r1_exp = {
    .name = FMCPICO1M_4CH_NAME_SET_GET_RNG_R1,
    .opcode = FMCPICO1M_4CH_OPCODE_SET_GET_RNG_R1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmcpico1m_4ch_set_get_rng_r2_exp = {
    .name = FMCPICO1M_4CH_NAME_SET_GET_RNG_R2,
    .opcode = FMCPICO1M_4CH_OPCODE_SET_GET_RNG_R2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmcpico1m_4ch_set_get_rng_r3_exp = {
    .name = FMCPICO1M_4CH_NAME_SET_GET_RNG_R3,
    .opcode = FMCPICO1M_4CH_OPCODE_SET_GET_RNG_R3,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmcpico1m_4ch_set_get_adc_data0_exp = {
    .name = FMCPICO1M_4CH_NAME_SET_GET_ADC_DATA0,
    .opcode = FMCPICO1M_4CH_OPCODE_SET_GET_ADC_DATA0,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmcpico1m_4ch_set_get_adc_data1_exp = {
    .name = FMCPICO1M_4CH_NAME_SET_GET_ADC_DATA1,
    .opcode = FMCPICO1M_4CH_OPCODE_SET_GET_ADC_DATA1,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmcpico1m_4ch_set_get_adc_data2_exp = {
    .name = FMCPICO1M_4CH_NAME_SET_GET_ADC_DATA2,
    .opcode = FMCPICO1M_4CH_OPCODE_SET_GET_ADC_DATA2,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t fmcpico1m_4ch_set_get_adc_data3_exp = {
    .name = FMCPICO1M_4CH_NAME_SET_GET_ADC_DATA3,
    .opcode = FMCPICO1M_4CH_OPCODE_SET_GET_ADC_DATA3,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

/* Exported function description */
const disp_op_t *fmcpico1m_4ch_exp_ops [] = {
    &fmcpico1m_4ch_set_get_led1_exp,
    &fmcpico1m_4ch_set_get_led2_exp,
    &fmcpico1m_4ch_set_get_rng_r0_exp,
    &fmcpico1m_4ch_set_get_rng_r1_exp,
    &fmcpico1m_4ch_set_get_rng_r2_exp,
    &fmcpico1m_4ch_set_get_rng_r3_exp,
    &fmcpico1m_4ch_set_get_adc_data0_exp,
    &fmcpico1m_4ch_set_get_adc_data1_exp,
    &fmcpico1m_4ch_set_get_adc_data2_exp,
    &fmcpico1m_4ch_set_get_adc_data3_exp,
    NULL
};

