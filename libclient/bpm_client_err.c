/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

/* Error definitions and output stringification based on the work available
 * at the libsllp project repository: https://github.com/brunoseivam/libsllp */

#include "bpm_client_err.h"

static const char *bpm_client_err [BPM_CLIENT_ERR_END] =
{
    [BPM_CLIENT_SUCCESS]              = "Success",
    [BPM_CLIENT_ERR_ALLOC]            = "Could not allocate memory",
    [BPM_CLIENT_ERR_SERVER]           = "Server could not complete request",
    [BPM_CLIENT_ERR_AGAIN]            = "Operation did not fully complete, try again",
    [BPM_CLIENT_ERR_TIMEOUT]          = "Timeout occurred",
    [BPM_CLIENT_ERR_MSG]              = "Unexpected message",
    [BPM_CLIENT_ERR_INV_PARAM]        = "Invalid function parameters",
    [BPM_CLIENT_ERR_INV_FUNC]         = "Invalid function",
    [BPM_CLIENT_INT]                  = "Interrupt occured"
};

/* Convert enumeration type to string */
const char * bpm_client_err_str (bpm_client_err_e err)
{
    return bpm_client_err [err];
}
