/*
 * Copyright (C) 2016 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

/* Error definitions and output stringification based on the work available
 * at the libsllp project repository: https://github.com/brunoseivam/libsllp */

#include "sdbutils.h"

static const char *sdbutils_err [SDBUTILS_ERR_END] =
{
    [SDBUTILS_SUCCESS]              = "Success",
    [SDBUTILS_ERR_ALLOC]            = "Could not allocate memory",
    [SDBUTILS_ERR_OPEN]             = "Could not open SDBFS",
    [SDBUTILS_ERR_UNK_REC]          = "Unknown record type"
};

/* Convert enumeration type to string */
const char * sdbutils_err_str (sdbutils_err_e err)
{
    return sdbutils_err [err];
}
