/*
 * Copyright (C) 2017 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "halcs_server.h"
/* Private headers */
#include "sm_io_afc_mgmt_defaults.h"
#include "sm_io_afc_mgmt_core.h"
#include "hw/wb_afc_mgmt_regs.h"
#include "chips_addr.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...)  \
    ASSERT_HAL_TEST(test_boolean, SM_IO, "[sm_io_afc_mgmt_core]",               \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)                   \
    ASSERT_HAL_ALLOC(ptr, SM_IO, "[sm_io_afc_mgmt_core]",                       \
            smio_err_str(SMIO_ERR_ALLOC),                                       \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                                \
    CHECK_HAL_ERR(err, SM_IO, "[sm_io_afc_mgmt_core]",                          \
            smio_err_str (err_type))

/* AFC MGMT chip addresses */
const uint32_t afc_mgmt_si57x_addr = 0x55;

/* Creates a new instance of Device Information */
smio_afc_mgmt_t * smio_afc_mgmt_new (smio_t *parent)
{
    assert (parent);
    smio_afc_mgmt_t *self = (smio_afc_mgmt_t *) zmalloc (sizeof *self);
    ASSERT_ALLOC(self, err_self_alloc);
    uint32_t inst_id = smio_get_inst_id (parent);

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:afc_mgmt_core] Si57x initializing, "
            "addr: 0x%08X, Inst ID: %u\n", afc_mgmt_si57x_addr,
            inst_id);

    /* Create I2C protocol for Si57x chips */
    self->smpr_i2c_si57x = smpr_i2c_new (0, afc_mgmt_si57x_addr);
    ASSERT_ALLOC(self->smpr_i2c_si57x, err_smpr_i2c_si57x_alloc);

    self->smch_si57x = smch_si57x_new (parent, AFC_MGMT_SI57X_I2C_OFFS,
            smpr_i2c_get_ops (self->smpr_i2c_si57x), 0);
    ASSERT_ALLOC(self->smch_si57x, err_smch_si57x_alloc);

    return self;

err_smch_si57x_alloc:
    smpr_i2c_destroy (&self->smpr_i2c_si57x);
err_smpr_i2c_si57x_alloc:
    free (self);
err_self_alloc:
    return NULL;
}

/* Destroy an instance of the Device Information */
smio_err_e smio_afc_mgmt_destroy (smio_afc_mgmt_t **self_p)
{
    assert (self_p);

    if (*self_p) {
        smio_afc_mgmt_t *self = *self_p;

        smch_si57x_destroy (&self->smch_si57x);
        smpr_i2c_destroy (&self->smpr_i2c_si57x);

        free (self);
        *self_p = NULL;
    }

    return SMIO_SUCCESS;
}

