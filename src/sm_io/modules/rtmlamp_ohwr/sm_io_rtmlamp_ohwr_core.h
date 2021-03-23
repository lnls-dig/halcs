/*
 * Copyright (C) 2021 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
*/

#ifndef _SM_IO_RTMLAMP_OHWR_CORE_H_
#define _SM_IO_RTMLAMP_OHWR_CORE_H_

typedef struct {
    const uint32_t example;
} smio_rtmlamp_ohwr_t;

/***************** Our methods *****************/

/* Creates a new instance of the smio realization */
smio_rtmlamp_ohwr_t * smio_rtmlamp_ohwr_new (smio_t *parent);
/* Destroys the smio realizationn */
smio_err_e smio_rtmlamp_ohwr_destroy (smio_rtmlamp_ohwr_t **self_p);

#endif
