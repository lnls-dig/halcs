/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

/* Error definitions and output stringification based on the work available
 * at the libsllp project repository: https://github.com/brunoseivam/libsllp */

#ifndef _SMIO_ERR_H_
#define _SMIO_ERR_H_

enum _smio_err_e
{
    SMIO_SUCCESS = 0,               /* No error */
    SMIO_ERR_ALLOC,                 /* Could not allocate memory */
    SMIO_ERR_FUNC_NOT_IMPL,         /* Function not implemented */
    SMIO_ERR_OPCODE_NOT_SUPP,       /* Opcode not supported */
    SMIO_ERR_WRONG_PARAM,           /* Wrong parameter value */
    SMIO_ERR_LLIO,                  /* Low-level I/O could not complete operation */
    SMIO_ERR_EXPORT_OP,             /* Error exporting function */
    SMIO_ERR_CONFIG_DFLT,           /* Error configuring the default values */
    SMIO_ERR_MSG_NOT_SUPP,          /* Exported message not supported */
    SMIO_ERR_END                    /* End of enum marker */
};

typedef enum _smio_err_e smio_err_e;

/* Convert enumeration type to string */
const char * smio_err_str (smio_err_e err);

#endif
