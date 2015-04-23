/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

/* Error definitions and output stringification based on the work available
 * at the libsllp project repository: https://github.com/brunoseivam/libsllp */

#ifndef _SMCH_ERR_H_
#define _SMCH_ERR_H_

enum _smch_err_e
{
    SMCH_SUCCESS = 0,               /* No error */
    SMCH_ERR_ALLOC,                 /* Could not allocate memory */
    SMCH_ERR_INV_FUNC_PARAM,        /* Invalid function paramter */
    SMCH_ERR_RW_SMPR,               /* Could not Read/Write to/from specified SMPR protocol */
    SMCH_ERR_TIMEOUT,               /* Timeout occured or maximum number of tries reached */
    SMCH_ERR_END                    /* End of enum marker */
};

typedef enum _smch_err_e smch_err_e;

/* Convert enumeration type to string */
const char * smch_err_str (smch_err_e err);

#endif
