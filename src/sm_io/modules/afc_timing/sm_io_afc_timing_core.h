/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
*/

#ifndef _SM_IO_AFC_TIMING_CORE_H_
#define _SM_IO_AFC_TIMING_CORE_H_

typedef struct {
    const uint32_t example;
} smio_afc_timing_t;

/***************** Our methods *****************/

/* Creates a new instance of the smio realization */
smio_afc_timing_t * smio_afc_timing_new (smio_t *parent);
/* Destroys the smio realization */
smio_err_e smio_afc_timing_destroy (smio_afc_timing_t **self_p);

#endif
