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
};

/* Convert enumeration type to string */
const char * bpm_client_err_str (bpm_client_err_e err)
{
    return bpm_client_err [err];
}
