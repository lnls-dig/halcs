/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include <string.h>
#include <stdio.h>

#include "ll_io_dev_info.h"
#include "hal_assert.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label)  \
    ASSERT_HAL_TEST(test_boolean, LL_IO, "ll_io_dev_info",  \
            err_str, err_goto_label)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label)                   \
    ASSERT_HAL_ALLOC(ptr, LL_IO, "ll_io_dev_info",          \
            llio_err_str(LLIO_ERR_ALLOC),                   \
            err_goto_label)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                            \
    CHECK_HAL_ERR(err, LL_IO, "ll_io_dev_info",             \
            llio_err_str (err_type))

/* Creates a new instance of Device Information */
llio_dev_info_t * llio_dev_info_new ()
{
    llio_dev_info_t *self = (llio_dev_info_t *) zmalloc (sizeof *self);
    ASSERT_ALLOC(self, err_self_alloc);

    /* Initilialize llio_dev_info */
    /* Put some default values as these are going to be filled
       later by read_dev_info() */
    self->sdb_addr = 0x0;
    self->parsed = false;
    self->sdb = zlist_new (); /* HASH!? */
    ASSERT_ALLOC(self->sdb, err_sdb_alloc);

    return self;

err_sdb_alloc:
    free (self);
err_self_alloc:
    return NULL;
}

/* Destroy an instance of the Device Information */
llio_err_e llio_dev_info_destroy (llio_dev_info_t **self_p)
{
    assert (self_p);

    if (*self_p) {
        llio_dev_info_t *self = *self_p;

        /* Starting destructing by the last resource */
        zlist_destroy (&self->sdb);
        free (self);
        *self_p = NULL;
    }

    return LLIO_SUCCESS;
}
