/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _SM_CH_PCA9547_H_
#define _SM_CH_PCA9547_H_

#include <inttypes.h>
#include <sys/types.h>

#include "sm_pr_i2c.h"
#include "sm_ch_err.h"

#define SMCH_PCA9547_NO_CHANNEL                 0xFF /* No Channel selected */

struct _smch_pca9547_t {
    smpr_t *i2c;                    /* I2C protocol object */
    uint32_t addr;                  /* I2C address for this PCA9547 chip */
};

/* Opaque sm_ch_pca9547 structure */
typedef struct _smch_pca9547_t smch_pca9547_t;

/***************** Our methods *****************/

/* Creates a new instance of the SMCH PCA9547 */
smch_pca9547_t * smch_pca9547_new (smio_t *parent, uint64_t base, uint32_t addr,
        int verbose);
/* Destroy an instance of the SMCH PCA9547 */
smch_err_e smch_pca9547_destroy (smch_pca9547_t **self_p);

/* Read/Write to PCA9547 single bytes */
smch_err_e smch_pca9547_write_8 (smch_pca9547_t *self, const uint8_t *data);
smch_err_e smch_pca9547_read_8 (smch_pca9547_t *self, uint8_t *data);

/* Enable specific I2C channel */
smch_err_e smch_pca9547_en_chan (smch_pca9547_t *self, uint8_t chan);

#endif
