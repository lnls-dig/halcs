/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
*/

#ifndef _SM_IO_AFC_DIAG_CORE_H_
#define _SM_IO_AFC_DIAG_CORE_H_

#include <inttypes.h>
#include "sm_io_err.h"
#include "sm_io.h"

#define SMIO_AFC_DIAG_HANDLER(self) ((smio_afc_diag_t *) self->smio_handler)

struct _smio_afc_diag_t {
    const uint32_t example;
};

/* Opaque class structure */
typedef struct _smio_afc_diag_t smio_afc_diag_t;

/***************** Our methods *****************/

/* Creates a new instance of the smio realization */
smio_afc_diag_t * smio_afc_diag_new (smio_t *parent);
/* Destroys the smio realization */
smio_err_e smio_afc_diag_destroy (smio_afc_diag_t **self_p);

#endif
