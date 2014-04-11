/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

/* Error definitions and output stringification based on the work available
 * at the libsllp project repository: https://github.com/brunoseivam/libsllp */

#include "ll_io_err.h"

static const char *llio_err [LLIO_ERR_END] =
{
    [LLIO_SUCCESS]              = "Success",
    [LLIO_ERR_ALLOC]            = "Could not allocate memory",
    [LLIO_ERR_FUNC_NOT_IMPL]    = "Function not implemented",
    [LLIO_ERR_INV_FUNC_PARAM]   = "Invalid function parameter",
    [LLIO_ERR_SET_ENDP]         = "Could not change enpoint (device opened)",
    [LLIO_ERR_DEV_CLOSE]        = "Could not close device appropriately"
};

/* Convert enumeration type to string */
const char * llio_err_str (llio_err_e err)
{
    return llio_err [err];
}
