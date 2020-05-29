/*
 * Copyright (C) 2017 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "halcs_server.h"
/* Private headers */
#include "sm_io_acq_pm_core.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...)  \
    ASSERT_HAL_TEST(test_boolean, SM_IO, "[sm_io_acq_pm_core]",                 \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_HAL_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)                   \
    ASSERT_HAL_ALLOC(ptr, SM_IO, "[sm_io_acq_pm_core]",                         \
            smio_err_str(SMIO_ERR_ALLOC),                                       \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_HAL_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                                \
    CHECK_HAL_ERR(err, SM_IO, "[sm_io_acq_pm_core]",                            \
            smio_err_str (err_type))

/* Creates a new instance of Device Information */
smio_acq_pm_t * smio_acq_pm_new (smio_t *parent)
{
    smio_acq_pm_t *self = (smio_acq_pm_t *) zmalloc (sizeof *self);
    ASSERT_ALLOC(self, err_self_alloc);
    uint32_t inst_id = smio_get_inst_id (parent);
    uint64_t base = smio_get_base (parent);

    /* Spawn regular ACQ for now */
    smio_err_e serr = smio_register_sm (parent, 0x4519a0ad, base, inst_id);
    ASSERT_TEST(serr == SMIO_SUCCESS, "Could not spawn regular ACQ core",
            err_spawn_acq_core);

    return self;

err_spawn_acq_core:
err_self_alloc:
    return NULL;
}

/* Destroy an instance of the Device Information */
smio_err_e smio_acq_pm_destroy (smio_acq_pm_t **self_p)
{
    assert (self_p);

    if (*self_p) {
        smio_acq_pm_t *self = *self_p;

        free (self);
        *self_p = NULL;
    }

    return SMIO_SUCCESS;
}

