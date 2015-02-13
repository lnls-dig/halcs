/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

/* Error definitions and output stringification based on the work available
 * at the libsllp project repository: https://github.com/brunoseivam/libsllp */

#ifndef _DEV_MNGR_CORE_ERR_H_
#define _DEV_MNGR_CORE_ERR_H_

enum _dmngr_err_e
{
    DMNGR_SUCCESS = 0,              /* No error */
    DMNGR_ERR_ALLOC,                /* Could not allocate memory */
    DMNGR_ERR_FUNC_NOT_IMPL,        /* Function not implemented */
    DMNGR_ERR_SIGACTION,            /* Could not register signal */
    DMNGR_ERR_WAITCHLD,             /* Wait child routine error */
    DMNGR_ERR_SPAWNCHLD,            /* Spawn child routine error */
    DMNGR_ERR_BROK_RUNN,            /* Broker already running error */
    DMNGR_ERR_CFG,                  /* Could not get property from config file */
    DMNGR_ERR_END                   /* End of enum marker */
};

typedef enum _dmngr_err_e dmngr_err_e;

/* Convert enumeration type to string */
const char * dmngr_err_str (dmngr_err_e err);

#endif
