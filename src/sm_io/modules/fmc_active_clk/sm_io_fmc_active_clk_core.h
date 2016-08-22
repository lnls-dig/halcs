/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
*/

#ifndef _SM_IO_FMC_ACTIVE_CLK_CORE_H_
#define _SM_IO_FMC_ACTIVE_CLK_CORE_H_

#define SMIO_AD9510_HANDLER(smio_handler) ((smch_ad9510_t *) smio_handler->smch_ad9510)
#define SMIO_SI57X_HANDLER(smio_handler) ((smch_si57x_t *) smio_handler->smch_si571)

typedef struct {
    smpr_spi_t *smpr_spi_ad9510;                /* SPI protocol handler */
    smch_ad9510_t *smch_ad9510;                 /* AD9510 chip handler */
    smpr_i2c_t *smpr_i2c_si571;                 /* I2C protocol handler */
    smch_si57x_t *smch_si571;                   /* SI571 chip handler */
} smio_fmc_active_clk_t;

/***************** Our methods *****************/

/* Creates a new instance of the smio realization */
smio_fmc_active_clk_t * smio_fmc_active_clk_new (smio_t *parent);
/* Destroys the smio realization */
smio_err_e smio_fmc_active_clk_destroy (smio_fmc_active_clk_t **self_p);

#endif
