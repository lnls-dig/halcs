/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
*/

#ifndef _SM_IO_DSP_CORE_H_
#define _SM_IO_DSP_CORE_H_

typedef struct {
    const uint32_t example;
} smio_dsp_t;

/***************** Our methods *****************/

/* Creates a new instance of the smio realization */
smio_dsp_t * smio_dsp_new (smio_t *parent);
/* Destroys the smio realizationn */
smio_err_e smio_dsp_destroy (smio_dsp_t **self_p);

#endif
