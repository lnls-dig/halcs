/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

/* Error definitions and output stringification based on the work available
 * at the libsllp project repository: https://github.com/brunoseivam/libsllp */

#include "disptable.h"

static const char *disp_table_err [DISP_TABLE_ERR_END] =
{
    [DISP_TABLE_SUCCESS]              = "Success",
    [DISP_TABLE_ERR_ALLOC]            = "Could not allocate memory",
    [DISP_TABLE_ERR_NULL_POINTER]     = "Null pointer received",
    [DISP_TABLE_ERR_NO_FUNC_REG]      = "No function registered",
    [DISP_TABLE_ERR_BAD_MSG]          = "Bad message detected"
};

/* Convert enumeration type to string */
const char * disp_table_err_str (disp_table_err_e err)
{
    return disp_table_err [err];
}
