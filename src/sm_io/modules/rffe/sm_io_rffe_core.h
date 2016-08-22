/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
*/

#ifndef _SM_IO_RFFE_CORE_H_
#define _SM_IO_RFFE_CORE_H_

#define SMIO_CTL_HANDLER(smio_handler) (smio_handler->smch_ctl)

typedef struct {
    smpr_bsmp_t *smpr_ctl;
    smch_rffe_t *smch_ctl;
} smio_rffe_t;

/***************** Our methods *****************/

/* Creates a new instance of the smio realization */
smio_rffe_t * smio_rffe_new (smio_t *parent);
/* Destroys the smio realization */
smio_err_e smio_rffe_destroy (smio_rffe_t **self_p);

#endif
