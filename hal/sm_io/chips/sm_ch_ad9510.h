/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _SM_CH_AD9510_H_
#define _SM_CH_AD9510_H_

#include <inttypes.h>
#include <sys/types.h>

#include "sm_pr_spi.h"
#include "sm_ch_err.h"

struct _smch_ad9510_t {
    smpr_t *spi;                    /* SPI protocol object */
    uint32_t ss;                    /* Slave select line for this AD9510 chip */
};

/* Opaque sm_ch_ad9510 structure */
typedef struct _smch_ad9510_t smch_ad9510_t;

/***************** Our methods *****************/

/* Creates a new instance of the SMCH AD9510 */
smch_ad9510_t * smch_ad9510_new (smio_t *parent, uint32_t base, uint32_t ss,
        int verbose);
/* Destroy an instance of the SMCH AD9510 */
smch_err_e smch_ad9510_destroy (smch_ad9510_t **self_p);

/* Read/Write to AD9510 Registers */
smch_err_e smch_ad9510_write_8 (smch_ad9510_t *self, uint8_t addr, const uint8_t *data);
smch_err_e smch_ad9510_read_8 (smch_ad9510_t *self, uint8_t addr, uint8_t *data);

/* Update AD9510 registers */
smch_err_e smch_ad9510_reg_update (smch_ad9510_t *self);

/* Simple test for configuring a few AD9510 regsiters */
smch_err_e smch_ad9510_cfg_defaults (smch_ad9510_t *self);

smch_err_e smch_ad9510_div_out0 (smch_ad9510_t *self, uint32_t div);

#endif
