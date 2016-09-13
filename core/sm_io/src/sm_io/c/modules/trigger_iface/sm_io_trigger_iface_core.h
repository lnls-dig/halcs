/*
 * Copyright (C) 2016 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
*/

#ifndef _SM_IO_TRIGGER_IFACE_CORE_H_
#define _SM_IO_TRIGGER_IFACE_CORE_H_

typedef struct {
    const uint32_t example;
} smio_trigger_iface_t;

/***************** Our methods *****************/

/* Creates a new instance of the smio realization */
smio_trigger_iface_t * smio_trigger_iface_new (smio_t *parent);
/* Destroys the smio realizationn */
smio_err_e smio_trigger_iface_destroy (smio_trigger_iface_t **self_p);

#endif
