/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

/* Error definitions and output stringification based on the work available
 * at the libsllp project repository: https://github.com/brunoseivam/libsllp */

#ifndef _DEVIO_ERR_H_
#define _DEVIO_ERR_H_

#ifdef __cplusplus
extern "C" {
#endif

enum _devio_err_e {
    DEVIO_SUCCESS = 0,              /* No error */
    DEVIO_ERR_ALLOC,                /* Could not allocate memory */
    DEVIO_ERR_FUNC_NOT_IMPL,        /* Function not implemented */
    DEVIO_ERR_SMIO_DO_OP,           /* Error calling sm_io specific function */
    DEVIO_ERR_NO_SMIO_OP,           /* No sm_io registered for the opcode argument */
    DEVIO_ERR_NO_SMIO_ID,           /* No sm_io registered with the id argument */
    DEVIO_ERR_MAX_SMIOS,            /* Maximum number os SMIOs reached */
    DEVIO_ERR_NO_NODES,             /* No thread nodes available */
    DEVIO_ERR_UNINIT_POLLER,        /* Uninitliazed poller */
    DEVIO_ERR_INTERRUPTED_POLLER,   /* Poller interrupted. zeroMQ context was terminated or received interrupt signal */
    DEVIO_ERR_BAD_MSG,              /* Malformed message received */
    DEVIO_ERR_TERMINATED,           /* Terminated devio instance */
    DEVIO_ERR_SMIO_DESTROY,         /* Could not destroy sm_io instance */
    DEVIO_ERR_INV_SOCKET,           /* Invalid socket reference */
    DEVIO_ERR_MOD_LLIO,             /* Error modifying LLIO instance */
    DEVIO_ERR_CFG,                  /* Could not get property from config file */
    DEVIO_ERR_END                   /* End of enum marker */
};

/* Convert enumeration type to string */
const char * devio_err_str (devio_err_e err);

#ifdef __cplusplus
}
#endif

#endif
