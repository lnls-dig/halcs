/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

/* Error definitions and output stringification based on the work available
 * at the libsllp project repository: https://github.com/brunoseivam/libsllp */

#include "dev_mngr_err.h"

static const char *dmngr_err [DMNGR_ERR_END] =
{
    [DMNGR_SUCCESS]                 = "Success",
    [DMNGR_ERR_ALLOC]               = "Could not allocate memory",
    [DMNGR_ERR_FUNC_NOT_IMPL]       = "Function not implemented",
    [DMNGR_ERR_SIGACTION]           = "Signal registration error",
    [DMNGR_ERR_WAITCHLD]            = "Could not complete wait child routine",
    [DMNGR_ERR_SPAWNCHLD]           = "Could not complete spawn child routine",
    [DMNGR_ERR_BROK_RUNN]           = "Broker already running"
};

/* Convert enumeration type to string */
const char * dmngr_err_str (dmngr_err_e err)
{
    return dmngr_err [err];
}
