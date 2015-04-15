/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

/* Error definitions and output stringification based on the work available
 * at the libsllp project repository: https://github.com/brunoseivam/libsllp */

#include "hal_utils_err.h"

static const char *halutils_err [HALUTILS_ERR_END] =
{
    [HALUTILS_SUCCESS]              = "Success",
    [HALUTILS_ERR_ALLOC]            = "Could not allocate memory",
    [HALUTILS_ERR_NULL_POINTER]     = "Null pointer received",
    [HALUTILS_ERR_NO_FUNC_REG]      = "No function registered",
    [HALUTILS_ERR_INV_LESS_ARGS]    = "Less arguments than specified passed",
    [HALUTILS_ERR_INV_MORE_ARGS]    = "More arguments than specified passed",
    [HALUTILS_ERR_INV_SIZE_ARG]     = "Invalid size of argument size"
};

/* Convert enumeration type to string */
const char * halutils_err_str (halutils_err_e err)
{
    return halutils_err [err];
}
