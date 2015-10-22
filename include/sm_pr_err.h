/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

/* Error definitions and output stringification based on the work available
 * at the libsllp project repository: https://github.com/brunoseivam/libsllp */

#ifndef _SMPR_ERR_H_
#define _SMPR_ERR_H_

#ifdef __cplusplus
extern "C" {
#endif

enum _smpr_err_e {
    SMPR_SUCCESS = 0,               /* No error */
    SMPR_ERR_ALLOC,                 /* Could not allocate memory */
    SMPR_ERR_INV_FUNC_PARAM,        /* Invalid function parameter */
    SMPR_ERR_FUNC_NOT_IMPL,         /* Functions not implemented */
    SMPR_ERR_RW_SMIO,               /* Invalid function parameter */
    SMPR_ERR_DUP_HANDLER,           /* Protocol handler already set */
    SMPR_ERR_PROTO_INFO,            /* Could not retrieve protocol information */
    SMPR_ERR_END                    /* End of enum marker */
};

/* Convert enumeration type to string */
const char * smpr_err_str (smpr_err_e err);

#ifdef __cplusplus
}
#endif

#endif
