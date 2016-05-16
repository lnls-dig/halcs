/*
 * Copyright (C) 2016 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "sm_io_exports_helper.h"
#include "sm_io_codes.h"

/* Description SMIO TRIGGER_IFACE functions */

disp_op_t trigger_iface_dir_exp = {
    .name = TRIGGER_IFACE_NAME_DIR,
    .opcode = TRIGGER_IFACE_OPCODE_DIR,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t trigger_iface_rcv_count_rst_exp = {
    .name = TRIGGER_IFACE_NAME_RCV_COUNT_RST,
    .opcode = TRIGGER_IFACE_OPCODE_RCV_COUNT_RST,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t trigger_iface_transm_count_rst_exp = {
    .name = TRIGGER_IFACE_NAME_TRANSM_COUNT_RST,
    .opcode = TRIGGER_IFACE_OPCODE_TRANSM_COUNT_RST,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t trigger_iface_rcv_len_exp = {
    .name = TRIGGER_IFACE_NAME_RCV_LEN,
    .opcode = TRIGGER_IFACE_OPCODE_RCV_LEN,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t trigger_iface_transm_len_exp = {
    .name = TRIGGER_IFACE_NAME_TRANSM_LEN,
    .opcode = TRIGGER_IFACE_OPCODE_TRANSM_LEN,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t trigger_iface_count_rcv_exp = {
    .name = TRIGGER_IFACE_NAME_COUNT_RCV,
    .opcode = TRIGGER_IFACE_OPCODE_COUNT_RCV,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

disp_op_t trigger_iface_count_transm_exp = {
    .name = TRIGGER_IFACE_NAME_COUNT_TRANSM,
    .opcode = TRIGGER_IFACE_OPCODE_COUNT_TRANSM,
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
const disp_op_t *trigger_iface_exp_ops [] = {
    &trigger_iface_dir_exp,
    &trigger_iface_rcv_count_rst_exp,
    &trigger_iface_transm_count_rst_exp,
    &trigger_iface_rcv_len_exp,
    &trigger_iface_transm_len_exp,
    &trigger_iface_count_rcv_exp,
    &trigger_iface_count_transm_exp,
    NULL
};

