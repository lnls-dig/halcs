/*
 * Copyright (C) 2020 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
*/

#ifndef _SM_IO_FMC_100M_4CH_CORE_H_
#define _SM_IO_FMC_100M_4CH_CORE_H_

#define SMIO_LTC2174_HANDLER(smio_handler) ((smch_ltc2174_t *) smio_handler->smch_ltc2174)
#define SMIO_SI57X_HANDLER(smio_handler) ((smch_si57x_t *) smio_handler->smch_si571)

typedef struct {
    smpr_spi_t *smpr_spi_ltc2174;               /* SPI protocol handler */
    smch_ltc2174_t *smch_ltc2174;               /* LTC2174 chip handler */
    smpr_i2c_t *smpr_i2c_si571;                 /* I2C protocol handler */
    smch_si57x_t *smch_si571;                   /* SI571 chip handler */
} smio_fmc100m_4ch_t;

/***************** Our methods *****************/

/* Creates a new instance of the smio realization */
smio_fmc100m_4ch_t * smio_fmc100m_4ch_new (smio_t *parent);
/* Destroys the smio realization */
smio_err_e smio_fmc100m_4ch_destroy (smio_fmc100m_4ch_t **self_p);

#endif
