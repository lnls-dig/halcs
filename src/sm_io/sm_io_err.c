/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

/* Error definitions and output stringification based on the work available
 * at the libsllp project repository: https://github.com/brunoseivam/libsllp */

#include "halcs_server.h"

static const char *smio_err [SMIO_ERR_END] =
{
    [SMIO_SUCCESS]                  = "Success",
    [SMIO_ERR_ALLOC]                = "Could not allocate memory",
    [SMIO_ERR_FUNC_NOT_IMPL]        = "Function not implemented",
    [SMIO_ERR_OPCODE_NOT_SUPP]      = "Opcode not supported",
    [SMIO_ERR_WRONG_PARAM]          = "Wrong parameter value",
    [SMIO_ERR_LLIO]                 = "Low-level I/O could not complete operation",
    [SMIO_ERR_EXPORT_OP]            = "Could not export function",
    [SMIO_ERR_CONFIG_DFLT]          = "Could not configure the default values",
    [SMIO_ERR_MSG_NOT_SUPP]         = "Exported message not supported",
    [SMIO_ERR_BAD_MSG]              = "Malformed message received",
    [SMIO_ERR_INTERRUPTED_POLLER]   = "Poller interrupted. zeroMQ context was "
        "terminated or received interrupt signal",
    [SMIO_ERR_INV_SOCKET]           = "Invalid socket reference",
    [SMIO_ERR_REGISTER_SM]          = "Could not register SMIO"
};

/* Convert enumeration type to string */
const char * smio_err_str (smio_err_e err)
{
    return smio_err [err];
}
