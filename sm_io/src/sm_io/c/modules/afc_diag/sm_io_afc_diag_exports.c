/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "sm_io_exports_helper.h"
#include "sm_io_codes.h"

/* Description SMIO AFC_DIAG functions */

disp_op_t afc_diag_set_get_card_slot_exp = {
    .name = AFC_DIAG_NAME_SET_GET_CARD_SLOT,
    .opcode = AFC_DIAG_OPCODE_SET_GET_CARD_SLOT,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
        }
};

disp_op_t afc_diag_set_get_ipmi_addr_exp = {
    .name = AFC_DIAG_NAME_SET_GET_IPMI_ADDR,
    .opcode = AFC_DIAG_OPCODE_SET_GET_IPMI_ADDR,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_diag_get_build_revision_exp = {
    .name = AFC_DIAG_NAME_GET_BUILD_REVISION,
    .opcode = AFC_DIAG_OPCODE_GET_BUILD_REVISION,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_VAR, smio_afc_diag_revision_data_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_diag_get_build_date_exp = {
    .name = AFC_DIAG_NAME_GET_BUILD_DATE,
    .opcode = AFC_DIAG_OPCODE_GET_BUILD_DATE,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_VAR, smio_afc_diag_revision_data_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_diag_get_build_user_name_exp = {
    .name = AFC_DIAG_NAME_GET_BUILD_USER_NAME,
    .opcode = AFC_DIAG_OPCODE_GET_BUILD_USER_NAME,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_VAR, smio_afc_diag_revision_data_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t afc_diag_get_build_user_email_exp = {
    .name = AFC_DIAG_NAME_GET_BUILD_USER_EMAIL,
    .opcode = AFC_DIAG_OPCODE_GET_BUILD_USER_EMAIL,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_VAR, smio_afc_diag_revision_data_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

/* Exported function description */
const disp_op_t *afc_diag_exp_ops [] = {
    &afc_diag_set_get_card_slot_exp,
    &afc_diag_set_get_ipmi_addr_exp,
    &afc_diag_get_build_revision_exp,
    &afc_diag_get_build_date_exp,
    &afc_diag_get_build_user_name_exp,
    &afc_diag_get_build_user_email_exp,
    NULL
};

