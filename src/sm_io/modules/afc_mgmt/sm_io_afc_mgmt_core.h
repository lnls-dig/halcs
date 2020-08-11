/*
 * Copyright (C) 2017 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
*/

#ifndef _SM_IO_AFC_MGMT_CORE_H_
#define _SM_IO_AFC_MGMT_CORE_H_

#define SMIO_SI57X_HANDLER(smio_handler) ((smch_si57x_t *) smio_handler->smch_si57x)

extern const uint32_t afc_mgmt_si57x_addr;

typedef struct {
    smpr_i2c_t *smpr_i2c_si57x;                 /* I2C protocol handler */
    smch_si57x_t *smch_si57x;                   /* SI57x chip handler */
} smio_afc_mgmt_t;

/***************** Our methods *****************/

/* Creates a new instance of the smio realization */
smio_afc_mgmt_t * smio_afc_mgmt_new (smio_t *parent);
/* Destroys the smio realization */
smio_err_e smio_afc_mgmt_destroy (smio_afc_mgmt_t **self_p);

#endif
