/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _SM_IO_AFC_DIAG_CODES_H_
#define _SM_IO_AFC_DIAG_CODES_H_

#include <inttypes.h>

/* This should suffice for these type of information */
#define AFC_DIAG_REVISION_BLOCK_SIZE            128

struct _smio_afc_diag_revision_data_t {
    uint8_t data[AFC_DIAG_REVISION_BLOCK_SIZE];       /* data buffer */
};

typedef struct _smio_afc_diag_revision_data_t smio_afc_diag_revision_data_t;

/* Messaging OPCODES */
#define AFC_DIAG_OPCODE_SIZE                        (sizeof(uint32_t))
#define AFC_DIAG_OPCODE_TYPE                        uint32_t

#define AFC_DIAG_OPCODE_SET_GET_CARD_SLOT           0
#define AFC_DIAG_NAME_SET_GET_CARD_SLOT             "afc_diag_card_slot"
#define AFC_DIAG_OPCODE_SET_GET_IPMI_ADDR           1
#define AFC_DIAG_NAME_SET_GET_IPMI_ADDR             "afc_diag_ipmi_addr"
#define AFC_DIAG_OPCODE_GET_BUILD_REVISION          2
#define AFC_DIAG_NAME_GET_BUILD_REVISION            "afc_diag_build_revision"
#define AFC_DIAG_OPCODE_GET_BUILD_DATE              3
#define AFC_DIAG_NAME_GET_BUILD_DATE                "afc_diag_build_date"
#define AFC_DIAG_OPCODE_GET_BUILD_USER_NAME         4
#define AFC_DIAG_NAME_GET_BUILD_USER_NAME           "afc_diag_build_user_name"
#define AFC_DIAG_OPCODE_GET_BUILD_USER_EMAIL        5
#define AFC_DIAG_NAME_GET_BUILD_USER_EMAIL          "afc_diag_build_user_email"
#define AFC_DIAG_OPCODE_END                         6

#define AFC_DIAG_OK                                 0   /* Operation was successful */
#define AFC_DIAG_ERR                                1   /* Could not set/get value */
#define AFC_DIAG_UNINPL                             2   /* Unimplemented function or operation */
#define AFC_DIAG_REPLY_END                          3   /* End marker */

#endif

