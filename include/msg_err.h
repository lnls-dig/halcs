/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

/* Error definitions and output stringification based on the work available
 * at the libsllp project repository: https://github.com/brunoseivam/libsllp */

#ifndef _MSG_ERR_H_
#define _MSG_ERR_H_

#ifdef __cplusplus
extern "C" {
#endif

enum _msg_err_e {
    MSG_SUCCESS = 0,               /* No error */
    MSG_ERR_ALLOC,                 /* Could not allocate message */
    MSG_FUNC_NOT_IMPL,             /* Function not implemented */
    MSG_ERR_INV,                   /* Invalid message structure */
    MSG_ERR_WRONG_ARGS,            /* Wrong arguments received (number or size) */
    MSG_ERR_UNEX_MSG,              /* Unexpected message type */
    MSG_ERR_INV_LESS_ARGS,         /* Less arguments than specified passed */
    MSG_ERR_INV_MORE_ARGS,         /* More arguments than specified passed */
    MSG_ERR_INV_SIZE_ARG,          /* Invalid size of argument size */
    MSG_ERR_END
};

/* Convert enumeration type to string */
const char * msg_err_str (msg_err_e err);

#ifdef __cplusplus
}
#endif

#endif
