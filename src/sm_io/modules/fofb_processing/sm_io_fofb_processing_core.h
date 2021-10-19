/*
 * Copyright (C) 2020 LNLS (www.lnls.br)
 * Author: Melissa Aguiar <melissa.aguiar@cnpem.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
*/

#ifndef _SM_IO_fofb_processing_CORE_H_
#define _SM_IO_fofb_processing_CORE_H_

typedef struct {
    const uint32_t example;
} smio_fofb_processing_t;

/***************** Our methods *****************/

/* Creates a new instance of the smio realization */
smio_fofb_processing_t * smio_fofb_processing_new (smio_t *parent);
/* Destroys the smio realizationn */
smio_err_e smio_fofb_processing_destroy (smio_fofb_processing_t **self_p);

#endif
