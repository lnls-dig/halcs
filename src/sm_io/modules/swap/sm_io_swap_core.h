/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
*/

#ifndef _SM_IO_SWAP_CORE_H_
#define _SM_IO_SWAP_CORE_H_

typedef struct {
    const uint32_t example;
} smio_swap_t;

/***************** Our methods *****************/

/* Creates a new instance of the smio realization */
smio_swap_t * smio_swap_new (smio_t *parent);
/* Destroys the smio realization */
smio_err_e smio_swap_destroy (smio_swap_t **self_p);

#endif
