/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

/* Error definitions and output stringification based on the work available
 * at the libsllp project repository: https://github.com/brunoseivam/libsllp */

#include "bpm_server.h"

static const char *devio_err [DEVIO_ERR_END] =
{
    [DEVIO_SUCCESS]                     = "Success",
    [DEVIO_ERR_ALLOC]                   = "Could not allocate memory",
    [DEVIO_ERR_FUNC_NOT_IMPL]           = "Function not implemented",
    [DEVIO_ERR_SMIO_DO_OP]              = "Error calling specific sm_io function",
    [DEVIO_ERR_NO_SMIO_OP]              = "No sm_io registered for this opcode",
    [DEVIO_ERR_NO_SMIO_ID]              = "No sm_io registered with this ID",
    [DEVIO_ERR_MAX_SMIOS]               = "Maximum number os SMIOs reached",
    [DEVIO_ERR_NO_NODES]                = "No thread nodes available",
    [DEVIO_ERR_UNINIT_POLLER]           = "Poller uninitilized",
    [DEVIO_ERR_INTERRUPTED_POLLER]      = "Poller interrupted. zeroMQ context was terminated or received interrupt signal",
    [DEVIO_ERR_BAD_MSG]                 = "Malformed message received",
    [DEVIO_ERR_TERMINATED]              = "Terminated devio instance",
    [DEVIO_ERR_SMIO_DESTROY]            = "Could not destroy sm_io instance",
    [DEVIO_ERR_INV_SOCKET]              = "Invalid socket reference",
    [DEVIO_ERR_MOD_LLIO]                = "Error modifying LLIO instance",
    [DEVIO_ERR_CFG]                     = "Could not get property from config file"
};

/* Convert enumeration type to string */
const char * devio_err_str (devio_err_e err)
{
    return devio_err [err];
}
