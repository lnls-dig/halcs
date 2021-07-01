/*
 * Copyright (C) 2017 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
*/

#ifndef _SM_IO_INIT_CORE_H_
#define _SM_IO_INIT_CORE_H_

typedef struct {
    char *board_type;       // board type string
} smio_init_t;

/***************** Our methods *****************/

/* Creates a new instance of the smio realization */
smio_init_t * smio_init_new (smio_t *parent);
/* Destroys the smio realizationn */
smio_err_e smio_init_destroy (smio_init_t **self_p);

#endif
