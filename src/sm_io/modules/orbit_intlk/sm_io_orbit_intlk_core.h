/*
 * Copyright (C) 2020 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
*/

#ifndef _SM_IO_ORBIT_INTLK_CORE_H_
#define _SM_IO_ORBIT_INTLK_CORE_H_

typedef struct {
    const uint32_t example;
} smio_orbit_intlk_t;

/***************** Our methods *****************/

/* Creates a new instance of the smio realization */
smio_orbit_intlk_t * smio_orbit_intlk_new (smio_t *parent);
/* Destroys the smio realizationn */
smio_err_e smio_orbit_intlk_destroy (smio_orbit_intlk_t **self_p);

#endif
