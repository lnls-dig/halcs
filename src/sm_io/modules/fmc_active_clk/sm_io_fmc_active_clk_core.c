/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "bpm_server.h"
/* Private headers */
#include "sm_io_fmc_active_clk_defaults.h"
#include "sm_io_fmc_active_clk_core.h"
#include "chips_addr.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...)  \
    ASSERT_HAL_TEST(test_boolean, SM_IO, "[sm_io_fmc_active_clk_core]",         \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)                   \
    ASSERT_HAL_ALLOC(ptr, SM_IO, "[sm_io_fmc_active_clk_core]",                 \
            smio_err_str(SMIO_ERR_ALLOC),                                       \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                                \
    CHECK_HAL_ERR(err, SM_IO, "[sm_io_fmc_active_clk_core]",                    \
            smio_err_str (err_type))

/* Creates a new instance of Device Information */
smio_fmc_active_clk_t * smio_fmc_active_clk_new (smio_t *parent)
{
    assert (parent);
    smio_fmc_active_clk_t *self = (smio_fmc_active_clk_t *) zmalloc (sizeof *self);
    ASSERT_ALLOC(self, err_self_alloc);
    uint32_t inst_id = smio_get_inst_id (parent);

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fmc_active_clk_core] AD9510 initializing, "
            "addr: 0x%08X, Inst ID: %u\n", fmc_active_clk_ad9510_addr,
            inst_id);
    self->smch_ad9510 = smch_ad9510_new (parent, FMC_ACTIVE_CLK_AD9510_SPI_OFFS,
            fmc_active_clk_ad9510_addr, 0);
    ASSERT_ALLOC(self->smch_ad9510, err_smch_ad9510_alloc);

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fmc_active_clk_core] SI571 initializing, "
            "addr: 0x%08X, Inst ID: %u\n", fmc_active_clk_si571_addr,
            inst_id);
    self->smch_si571 = smch_si57x_new (parent, FMC_ACTIVE_CLK_SI571_I2C_OFFS,
            fmc_active_clk_si571_addr, 0);
    ASSERT_ALLOC(self->smch_si571, err_smch_si571_alloc);

    return self;

err_smch_si571_alloc:
    if (self->smch_ad9510 != NULL) {
        smch_ad9510_destroy (&self->smch_ad9510);
    }
err_smch_ad9510_alloc:
    free (self);
err_self_alloc:
    return NULL;
}

/* Destroy an instance of the Device Information */
smio_err_e smio_fmc_active_clk_destroy (smio_fmc_active_clk_t **self_p)
{
    assert (self_p);

    if (*self_p) {
        smio_fmc_active_clk_t *self = *self_p;

        smch_si57x_destroy (&self->smch_si571);
        smch_ad9510_destroy (&self->smch_ad9510);
        free (self);
        *self_p = NULL;
    }

    return SMIO_SUCCESS;
}

