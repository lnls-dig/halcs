/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

/* Error definitions and output stringification based on the work available
 * at the libsllp project repository: https://github.com/brunoseivam/libsllp */

#include "bpm_server.h"

static const char *smch_err [SMCH_ERR_END] =
{
    [SMCH_SUCCESS]              = "Success",
    [SMCH_ERR_ALLOC]            = "Could not allocate memory",
    [SMCH_ERR_INV_FUNC_PARAM]   = "Invalid function parameter",
    [SMCH_ERR_RW_SMPR]          = "Could not Read/Write to/from specified SMPR protocol",
    [SMCH_ERR_TIMEOUT]          = "Timeout occured or maximum number of tries reached"
};

/* Convert enumeration type to string */
const char * smch_err_str (smch_err_e err)
{
    return smch_err [err];
}
