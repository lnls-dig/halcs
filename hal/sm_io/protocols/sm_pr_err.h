/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

/* Error definitions and output stringification based on the work available
 * at the libsllp project repository: https://github.com/brunoseivam/libsllp */

#ifndef _SMPR_ERR_H_
#define _SMPR_ERR_H_

enum _smpr_err_e
{
    SMPR_SUCCESS = 0,               /* No error */
    SMPR_ERR_ALLOC,                 /* Could not allocate memory */
    SMPR_ERR_INV_FUNC_PARAM,        /* Invalid function paramter */
    SMPR_ERR_FUNC_NOT_IMPL,         /* Fucntions not implemented */
    SMPR_ERR_RW_SMIO,               /* Invalid function parameter */
    SMPR_ERR_END                    /* End of enum marker */
};

typedef enum _smpr_err_e smpr_err_e;

/* Convert enumeration type to string */
const char * smpr_err_str (smpr_err_e err);

#endif
