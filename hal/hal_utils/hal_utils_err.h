/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

/* Error definitions and output stringification based on the work available
 * at the libsllp project repository: https://github.com/brunoseivam/libsllp */

#ifndef _HALUTILS_ERR_H_
#define _HALUTILS_ERR_H_

enum _halutils_err_e
{
    HALUTILS_SUCCESS = 0,               /* No error */
    HALUTILS_ERR_ALLOC,                 /* Could not allocate memory */
    HALUTILS_ERR_NULL_POINTER,          /* Null pointer received */
    HALUTILS_ERR_END
};

typedef enum _halutils_err_e halutils_err_e;

/* Convert enumeration type to string */
const char * halutils_err_str (halutils_err_e err);

#endif
