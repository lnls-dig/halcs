/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include "bpm_server.h"
/* Private headers */
#include "ddr3_map.h"
#include "sm_io_acq_core.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...)  \
    ASSERT_HAL_TEST(test_boolean, SM_IO, "[sm_io_acq_core]",                    \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_HAL_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)                   \
    ASSERT_HAL_ALLOC(ptr, SM_IO, "[sm_io_acq_core]",                            \
            smio_err_str(SMIO_ERR_ALLOC),                                       \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_HAL_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                                \
    CHECK_HAL_ERR(err, SM_IO, "[sm_io_acq_core]",                               \
            smio_err_str (err_type))

/* Creates a new instance of Device Information */
smio_acq_t * smio_acq_new (smio_t *parent, uint32_t num_samples_pre,
        uint32_t num_samples_post, uint32_t num_shots)
{
    smio_acq_t *self = (smio_acq_t *) zmalloc (sizeof *self);
    ASSERT_ALLOC(self, err_self_alloc);
    uint32_t inst_id = smio_get_inst_id (parent);

    /* Set default value for all channels */
    for (uint32_t i = 0; i < END_CHAN_ID; i++) {
        self->acq_params[i].num_samples_pre = num_samples_pre;
        self->acq_params[i].num_samples_post = num_samples_post;
        self->acq_params[i].num_shots = num_shots;
    }

    /* initilize acquisition buffer areas. Defined in ddr3_map.h */
    if (inst_id > NUM_ACQ_CORE_SMIOS-1) {
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_ERR, "[sm_io:acq_core] Instance ID invalid\n");
        return NULL;
    }

    self->acq_buf = __acq_buf[inst_id];

    return self;

err_self_alloc:
    return NULL;
}

/* Destroy an instance of the Device Information */
smio_err_e smio_acq_destroy (smio_acq_t **self_p)
{
    assert (self_p);

    if (*self_p) {
        smio_acq_t *self = *self_p;

        self->acq_buf = NULL;
        free (self);
        *self_p = NULL;
    }

    return SMIO_SUCCESS;
}

