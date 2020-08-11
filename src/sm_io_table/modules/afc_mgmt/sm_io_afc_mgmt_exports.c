/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "sm_io_codes.h"
#include "sm_io_exports_helper.h"

/* Description SMIO AFC_MGMT functions */

disp_op_t afc_mgmt_si57x_oe_exp = {
    .name = AFC_MGMT_NAME_SI57X_OE,
    .opcode = AFC_MGMT_OPCODE_SI57X_OE,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_mgmt_si57x_freq_exp = {
    .name = AFC_MGMT_NAME_SI57X_FREQ,
    .opcode = AFC_MGMT_OPCODE_SI57X_FREQ,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
        DISP_ARG_END
    }
};

disp_op_t afc_mgmt_si57x_get_defaults_exp = {
    .name = AFC_MGMT_NAME_SI57X_GET_DEFAULTS,
    .opcode = AFC_MGMT_OPCODE_SI57X_GET_DEFAULTS,
    .retval = DISP_ARG_END,
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
        DISP_ARG_END
    }
};

disp_op_t afc_mgmt_si57x_fstartup_exp = {
    .name = AFC_MGMT_NAME_SI57X_FSTARTUP,
    .opcode = AFC_MGMT_OPCODE_SI57X_FSTARTUP,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_DOUBLE, double),
        DISP_ARG_END
    }
};

/* Exported function description */
const disp_op_t *afc_mgmt_exp_ops [] = {
    &afc_mgmt_si57x_oe_exp,
    &afc_mgmt_si57x_freq_exp,
    &afc_mgmt_si57x_get_defaults_exp,
    &afc_mgmt_si57x_fstartup_exp,
    NULL
};

