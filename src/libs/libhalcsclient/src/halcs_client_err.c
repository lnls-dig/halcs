/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

/* Error definitions and output stringification based on the work available
 * at the libsllp project repository: https://github.com/brunoseivam/libsllp */

#include "halcs_client.h"

static const char *halcs_client_err [HALCS_CLIENT_ERR_END] =
{
    [HALCS_CLIENT_SUCCESS]              = "Success",
    [HALCS_CLIENT_ERR_ALLOC]            = "Could not allocate memory",
    [HALCS_CLIENT_ERR_SERVER]           = "Server could not complete request",
    [HALCS_CLIENT_ERR_AGAIN]            = "Operation did not fully complete, try again",
    [HALCS_CLIENT_ERR_TIMEOUT]          = "Timeout occurred",
    [HALCS_CLIENT_ERR_MSG]              = "Unexpected message",
    [HALCS_CLIENT_ERR_INV_PARAM]        = "Invalid function parameters",
    [HALCS_CLIENT_ERR_INV_FUNCTION]     = "Invalid function",
    [HALCS_CLIENT_INT]                  = "Interrupt occured"
};

/* Convert enumeration type to string */
const char * halcs_client_err_str (halcs_client_err_e err)
{
    return halcs_client_err [err];
}
