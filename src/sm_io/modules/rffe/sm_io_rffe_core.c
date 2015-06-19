/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include "bpm_server.h"
/* Private headers */
#include "sm_io_rffe_core.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, SM_IO, "[sm_io_rffe_core]",       \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)       \
    ASSERT_HAL_ALLOC(ptr, SM_IO, "[sm_io_rffe_core]",               \
            smio_err_str(SMIO_ERR_ALLOC),                           \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                    \
    CHECK_HAL_ERR(err, SM_IO, "[sm_io_rffe_core]",                  \
            smio_err_str (err_type))

/* Creates a new instance of Device Information */
smio_rffe_t * smio_rffe_new (smio_t *parent)
{
    (void) parent;

    smio_rffe_t *self = (smio_rffe_t *) zmalloc (sizeof *self);
    ASSERT_ALLOC(self, err_self_alloc);

    self->ctl = smch_rffe_new (parent, 0);
    ASSERT_ALLOC(self->ctl, err_rffe_alloc);

    return self;

err_rffe_alloc:
    free (self);
err_self_alloc:
    return NULL;
}

/* Destroy an instance of the Device Information */
smio_err_e smio_rffe_destroy (smio_rffe_t **self_p)
{
    assert (self_p);

    if (*self_p) {
        smio_rffe_t *self = *self_p;

        smch_rffe_destroy (&self->ctl);
        free (self);
        *self_p = NULL;
    }

    return SMIO_SUCCESS;
}

