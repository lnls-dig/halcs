/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include <string.h>

#include "dev_mngr_dev_info.h"
#include "errhand.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...)  \
    ASSERT_HAL_TEST(test_boolean, DEV_MNGR, "dev_mngr_dev_info",\
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_ALLOC(ptr, DEV_MNGR, "dev_mngr_dev_info",      \
            dmngr_err_str(DMNGR_ERR_ALLOC),                 \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                            \
    CHECK_HAL_ERR(err, DEV_MNGR, "dev_mngr_dev_info",       \
            dmngr_err_str (err_type))

/* Creates a new instance of the Device Manager */
devio_info_t * devio_info_new (char *dev_pathname, uint32_t id, llio_type_e type,
        devio_type_e devio_type, uint32_t smio_inst_id, devio_state_e state)
{
    assert (dev_pathname);

    devio_info_t *self = zmalloc (sizeof *self);
    ASSERT_ALLOC (self, err_devio_info_alloc);

    self->dev_pathname = strdup (dev_pathname);
    ASSERT_ALLOC (self->dev_pathname, err_pathname_alloc);
    self->id = id;
    self->type = type;
    self->devio_type = devio_type;
    self->smio_inst_id = smio_inst_id;
    self->state = state;

    return self;

err_pathname_alloc:
    free (self);
err_devio_info_alloc:
    return NULL;
}

/* Destroy an instance of the Device Manager */
dmngr_err_e devio_info_destroy (devio_info_t **self_p)
{
    assert (self_p);

    if (*self_p) {
        devio_info_t *self = *self_p;

        free (self->dev_pathname);
        free (self);
        *self_p = NULL;
    }

    return DMNGR_SUCCESS;
}
