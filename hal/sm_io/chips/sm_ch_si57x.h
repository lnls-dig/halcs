/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _SM_CH_SI57X_H_
#define _SM_CH_SI57X_H_

#include <inttypes.h>
#include <sys/types.h>

#include "sm_pr_i2c.h"
#include "sm_ch_err.h"

/* FIXME: This is valid for our part number. For different part number, it is different */
#define SMCH_SI57X_FOUT_FACTORY_DFLT                155490000LL

#define SMCH_SI57X_MIN_FREQ                         10000000L
#define SMCH_SI57X_MAX_FREQ                         1417500000L

#define SMCH_SI57X_FDCO_MIN                         4850000000LL
#define SMCH_SI57X_FDCO_MAX                         5670000000LL
#define SMCH_SI57X_FDCO_CENTER                      ((FDCO_MIN + FDCO_MAX) / 2)

struct _smch_si57x_t {
    smpr_t *i2c;                    /* I2C protocol object */
    uint32_t addr;                  /* I2C address for this SI57X chip */

    uint64_t fxtal;                 /* Internal crystal frequency */
    unsigned int n1;                /* N1 divider value */
    unsigned int hs_div;            /* High Speed divider value */
    uint64_t rfreq;                 /* RF Freq value */
    uint64_t frequency;             /* Output crystal frequency */
};

/* Opaque sm_ch_si57x structure */
typedef struct _smch_si57x_t smch_si57x_t;

/***************** Our methods *****************/

/* Creates a new instance of the SMCH SI57X */
smch_si57x_t * smch_si57x_new (smio_t *parent, uint32_t base, uint32_t addr,
        int verbose);
/* Destroy an instance of the SMCH SI57X */
smch_err_e smch_si57x_destroy (smch_si57x_t **self_p);

/* Read/Write to SI57X */
smch_err_e smch_si57x_write_8 (smch_si57x_t *self, uint8_t addr,
        const uint8_t *data);
smch_err_e smch_si57x_write_block (smch_si57x_t *self, uint8_t addr,
        const uint8_t *data, size_t size);
smch_err_e smch_si57x_read_8 (smch_si57x_t *self, uint8_t addr,
        uint8_t *data);
smch_err_e smch_si57x_read_block (smch_si57x_t *self, uint8_t addr,
        uint8_t *data, size_t size);

/* Probe bus for I2C devices */
ssize_t smch_si57x_probe_bus (smch_si57x_t *self);

/* Get Si57X divider values */
smch_err_e smch_si57x_get_divs (smch_si57x_t *self, uint64_t *rfreq,
        unsigned int *n1, unsigned int *hs_div);
/* Reset Si57X to its default factory values */
smch_err_e smch_si57x_get_defaults (smch_si57x_t *self, uint64_t fout);

/* Setup new frequency */
smch_err_e smch_si57x_set_freq (smch_si57x_t *self, unsigned long frequency);

#endif
