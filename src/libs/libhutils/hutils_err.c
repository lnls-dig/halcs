/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

/* Error definitions and output stringification based on the work available
 * at the libsllp project repository: https://github.com/brunoseivam/libsllp */

#include "hutils_err.h"

static const char *hutils_err [HUTILS_ERR_END] =
{
    [HUTILS_SUCCESS]              = "Success",
    [HUTILS_ERR_ALLOC]            = "Could not allocate memory",
    [HUTILS_ERR_CFG]              = "Could not get property from config file"
};

/* Convert enumeration type to string */
const char * hutils_err_str (hutils_err_e err)
{
    return hutils_err [err];
}
