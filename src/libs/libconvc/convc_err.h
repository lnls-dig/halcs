/*
 * Copyright (C) 2015 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

/* Error definitions and output stringification based on the work available
 * at the libsllp project repository: https://github.com/brunoseivam/libsllp */

#ifndef _CONVC_ERR_H_
#define _CONVC_ERR_H_

enum _convc_err_e
{
    CONVC_SUCCESS = 0,              /* No error */
    CONVC_ERR_ALLOC,                /* Could not allocate memory */
    CONVC_ERR_END                   /* End of enum marker */
};

typedef enum _convc_err_e convc_err_e;

/* Convert enumeration type to string */
const char * convc_err_str (convc_err_e err);

#endif
