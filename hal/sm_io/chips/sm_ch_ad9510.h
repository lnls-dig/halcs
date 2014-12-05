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

/* Read/Write to/from AD9510 Registers */
smch_err_e smch_ad9510_write_8 (smch_ad9510_t *self, uint8_t addr,
        const uint8_t *data);
smch_err_e smch_ad9510_read_8 (smch_ad9510_t *self, uint8_t addr,
        uint8_t *data);

/* Read/Write to/from AD9510 Registers with Update command */
smch_err_e smch_ad9510_write_8_update (smch_ad9510_t *self, uint8_t addr,
        const uint8_t *data);
smch_err_e smch_ad9510_read_8_update (smch_ad9510_t *self, uint8_t addr,
        uint8_t *data);

/* Update AD9510 registers */
smch_err_e smch_ad9510_reg_update (smch_ad9510_t *self);

/* Simple test for configuring a few AD9510 registers */
smch_err_e smch_ad9510_cfg_defaults (smch_ad9510_t *self);

/* AD9510 PLL divider functions */
smch_err_e smch_ad9510_pll_a_div (smch_ad9510_t *self, uint32_t div);
smch_err_e smch_ad9510_pll_b_div (smch_ad9510_t *self, uint32_t div);
smch_err_e smch_ad9510_pll_prescaler (smch_ad9510_t *self, uint32_t pre);
smch_err_e smch_ad9510_r_div (smch_ad9510_t *self, uint32_t div);

/* AD9510 PLL control functions */
smch_err_e smch_ad9510_pll_pdown (smch_ad9510_t *self, uint32_t pdown);
smch_err_e smch_ad9510_mux_status (smch_ad9510_t *self, uint32_t mux);
smch_err_e smch_ad9510_cp_current (smch_ad9510_t *self, uint32_t cp_current);
smch_err_e smch_ad9510_outputs (smch_ad9510_t *self, uint32_t out_en);
smch_err_e smch_ad9510_pll_clk_sel (smch_ad9510_t *self, uint32_t clk_num);

#endif

