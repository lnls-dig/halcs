/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

/* Error definitions and output stringification based on the work available
 * at the libsllp project repository: https://github.com/brunoseivam/libsllp */

#ifndef _LLIO_ERR_H_
#define _LLIO_ERR_H_

#ifdef __cplusplus
extern "C" {
#endif

enum _llio_err_e {
    LLIO_SUCCESS = 0,               /* No error */
    LLIO_ERR_ALLOC,                 /* Could not allocate memory */
    LLIO_ERR_FUNC_NOT_IMPL,         /* Function not implemented */
    LLIO_ERR_INV_FUNC_PARAM,        /* Invalid function parameter */
    LLIO_ERR_SET_ENDP,              /* Could not set endpoint (device already opened)*/
    LLIO_ERR_DEV_CLOSE,             /* Error closing a device */
    LLIO_ERR_CONN,                  /* Could establish connection to endpoint */
    LLIO_ERR_END                    /* End of enum marker */
};

typedef enum _llio_err_e llio_err_e;

/* Convert enumeration type to string */
const char * llio_err_str (llio_err_e err);

#ifdef __cplusplus
}
#endif

#endif
