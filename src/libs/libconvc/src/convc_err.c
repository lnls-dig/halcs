/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

/* Error definitions and output stringification based on the work available
 * at the libsllp project repository: https://github.com/brunoseivam/libsllp */

#include "convc.h"

static const char *convc_err [CONVC_ERR_END] =
{
    [CONVC_SUCCESS]              = "Success",
    [CONVC_ERR_ALLOC]            = "Could not allocate memory"
};

/* Convert enumeration type to string */
const char * convc_err_str (convc_err_e err)
{
    return convc_err [err];
}
