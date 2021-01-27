/*
 * Copyright (C) 2031 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
*/

#ifndef _SM_IO_FOFB_CTRL_CORE_H_
#define _SM_IO_FOFB_CTRL_CORE_H_

typedef struct {
    const uint32_t example;
} smio_fofb_ctrl_t;

/***************** Our methods *****************/

/* Creates a new instance of the smio realization */
smio_fofb_ctrl_t * smio_fofb_ctrl_new (smio_t *parent);
/* Destroys the smio realization */
smio_err_e smio_fofb_ctrl_destroy (smio_fofb_ctrl_t **self_p);

#endif
