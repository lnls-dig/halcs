/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

/* Error definitions and output stringification based on the work available
 * at the libsllp project repository: https://github.com/brunoseivam/libsllp */

#include "sm_pr_err.h"

static const char *smpr_err [SMPR_ERR_END] =
{
    [SMPR_SUCCESS]              = "Success",
    [SMPR_ERR_ALLOC]            = "Could not allocate memory",
    [SMPR_ERR_INV_FUNC_PARAM]   = "Invalid function parameter",
    [SMPR_ERR_RW_SMIO]          = "Could not Read/Write to/from SMIO",
    [SMPR_ERR_FUNC_NOT_IMPL]    = "Function not implemented",
    [SMPR_ERR_DUP_HANDLER]      = "Protocol handler already set"
};

/* Convert enumeration type to string */
const char * smpr_err_str (smpr_err_e err)
{
    return smpr_err [err];
}
