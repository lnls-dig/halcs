/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _LL_IO_ENDPOINT_H_
#define _LL_IO_ENDPOINT_H_

#include <inttypes.h>
#include <stdbool.h>

#include "czmq.h"
#include "ll_io_err.h"

/* Device endpoint */
struct _llio_endpoint_t {
    bool opened;                        /* True if device if already opened */
    char *name;                         /* Name of the endpoint, e.g., "/dev/fpga0"
                                           and "tcp://192.168.0.100:5556" */
};

/* Opaque llio_endpoint structure */
typedef struct _llio_endpoint_t llio_endpoint_t;

/***************** Our methods *****************/

/* Creates a new instance of the Endpoint*/
llio_endpoint_t * llio_endpoint_new (const char *endpoint);
/* Destroy an instance of the Endpoint */
llio_err_e llio_endpoint_destroy (llio_endpoint_t **self_p);

/* Set endpoint to llio instance in case it was not done in llio_endpoint_new() */
llio_err_e llio_endpoint_set (llio_endpoint_t *self, const char *endpoint);
/* Get endpoint from name from instance of llio_endpoint */
llio_err_e llio_endpoint_get (llio_endpoint_t *self, char **endpoint);

#endif
