/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
*/

#ifndef _SM_IO_ACQ_CORE_H_
#define _SM_IO_ACQ_CORE_H_

typedef struct {
      uint32_t num_samples;
} acq_params_t;

typedef struct {
    acq_params_t acq_params[END_CHAN_ID];
    const acq_buf_t *acq_buf;
} smio_acq_t;

/***************** Our methods *****************/

/* Creates a new instance of the smio realization */
smio_acq_t * smio_acq_new (smio_t *parent, uint32_t num_samples);
/* Destroys the smio realizationn */
smio_err_e smio_acq_destroy (smio_acq_t **self_p);

#endif
