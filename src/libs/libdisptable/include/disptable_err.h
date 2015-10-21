/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

/* Error definitions and output stringification based on the work available
 * at the libsllp project repository: https://github.com/brunoseivam/libsllp */

#ifndef _DISPTABLE_ERR_H_
#define _DISPTABLE_ERR_H_

#ifdef __cplusplus
extern "C" {
#endif

enum _disp_table_err_e
{
    DISP_TABLE_SUCCESS = 0,               /* No error */
    DISP_TABLE_ERR_ALLOC,                 /* Could not allocate memory */
    DISP_TABLE_ERR_NULL_POINTER,          /* Null pointer received */
    DISP_TABLE_ERR_NO_FUNC_REG,           /* No function registered */
    DISP_TABLE_ERR_BAD_MSG,               /* Bad message detected */
    DISP_TABLE_ERR_END
};

typedef enum _disp_table_err_e disp_table_err_e;

/* Convert enumeration type to string */
const char * disp_table_err_str (disp_table_err_e err);

#ifdef __cplusplus
}
#endif

#endif
