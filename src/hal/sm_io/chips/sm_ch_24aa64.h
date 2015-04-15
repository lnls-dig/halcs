/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _SM_CH_24AA64_H_
#define _SM_CH_24AA64_H_

#include <inttypes.h>
#include <sys/types.h>

#include "sm_pr_i2c.h"
#include "sm_ch_err.h"
#include "hal_stddef.h"

struct _smch_24aa64_t {
    smpr_t *i2c;                    /* I2C protocol object */
    uint32_t addr;                  /* I2C address for this 24AA64 chip */
};

/* Opaque sm_ch_24aa64 structure */
typedef struct _smch_24aa64_t smch_24aa64_t;

/***************** Our methods *****************/

/* Creates a new instance of the SMCH 24AA64 */
smch_24aa64_t * smch_24aa64_new (smio_t *parent, uint64_t base, uint32_t addr,
        int verbose);
/* Destroy an instance of the SMCH 24AA64 */
smch_err_e smch_24aa64_destroy (smch_24aa64_t **self_p);

/* Read/Write to 24AA64 single bytes */
smch_err_e smch_24aa64_write_8 (smch_24aa64_t *self, uint16_t addr, const uint8_t *data);
smch_err_e smch_24aa64_read_8 (smch_24aa64_t *self, uint16_t addr, uint8_t *data);

/* Read/Write to 24AA64 blocks of data, up to 32 (size of a page) */
smch_err_e smch_24aa64_write_block (smch_24aa64_t *self, uint16_t addr, const uint32_t *data,
        size_t size);
smch_err_e smch_24aa64_read_block (smch_24aa64_t *self, uint16_t addr, uint32_t *data,
        size_t size);

/* Probe bus for I2C devices */
ssize_t smch_24aa64_probe_bus (smch_24aa64_t *self);

#endif
