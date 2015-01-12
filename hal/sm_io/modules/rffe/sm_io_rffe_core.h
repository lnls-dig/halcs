/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
*/

#ifndef _SM_IO_RFFE_CORE_H_
#define _SM_IO_RFFE_CORE_H_

#include <inttypes.h>
#include "sm_io_err.h"
#include "sm_io.h"
#include "sm_ch_rffe.h"

#define SMIO_RFFE_HANDLER(self) ((smio_rffe_t *) self->smio_handler)
#define SMIO_CTL_HANDLER(self) (SMIO_RFFE_HANDLER(self)->ctl)

struct _smio_rffe_t {
    smch_rffe_t *ctl;
};

/* Opaque class structure */
typedef struct _smio_rffe_t smio_rffe_t;

/***************** Our methods *****************/

/* Creates a new instance of the smio realization */
smio_rffe_t * smio_rffe_new (smio_t *parent);
/* Destroys the smio realization */
smio_err_e smio_rffe_destroy (smio_rffe_t **self_p);

#endif
