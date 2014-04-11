/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _LL_IO_DEV_INFO_H_
#define _LL_IO_DEV_INFO_H_

#include <inttypes.h>
#include <sys/types.h>
#include <stdbool.h>

#include "czmq.h"
#include "ll_io_dev_info.h"
#include "ll_io_err.h"

/* This hold the SDB structures of the device */
struct _llio_dev_info_t {
    loff_t sdb_addr;                    /* Location of the SDB structures in device */
    bool parsed;                        /* True if device was already parsed */
    zlist_t *sdb;                       /* List holding the SDB structures of the device */
};

/* Opaque llio_dev_info structure */
typedef struct _llio_dev_info_t llio_dev_info_t;

/***************** Our methods *****************/

/* Creates a new instance of Device Information */
llio_dev_info_t * llio_dev_info_new ();

/* Destroy an instance of the Device Information */
llio_err_e llio_dev_info_destroy (llio_dev_info_t **self_p);

#endif
