/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _DEV_IO_EXPORTS_H_
#define _DEV_IO_EXPORTS_H_

#include "smio_thsafe_zmq_server.h"

#define DEVIO_OWNER_TYPE                        devio_t
#define DEVIO_EXP_OWNER(owner)                  ((devio_t *) owner)

#endif
