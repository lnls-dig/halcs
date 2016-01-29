/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "bpm_server.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...)  \
    ASSERT_HAL_TEST(test_boolean, DEV_MNGR, "dev_mngr_dev_info",        \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)           \
    ASSERT_HAL_ALLOC(ptr, DEV_MNGR, "dev_mngr_dev_info",                \
            dmngr_err_str(DMNGR_ERR_ALLOC),                             \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                        \
    CHECK_HAL_ERR(err, DEV_MNGR, "dev_mngr_dev_info",                   \
            dmngr_err_str (err_type))

struct _devio_info_t {
    uint32_t id;                        /* Device IO ID */
    llio_type_e type;                   /* Device type */
    devio_type_e devio_type;            /* DEVIO type */
    uint32_t smio_inst_id;              /* Optional SMIO instance ID for DEVIOs
                                           with a single SMIO */
    char *dev_pathname;                 /* /dev pathname */
    devio_state_e state;                /* Device IO state */
};

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

dmngr_err_e devio_info_set_id (devio_info_t *self, uint32_t id)
{
    assert (self);

    dmngr_err_e err = DMNGR_SUCCESS;
    /* Only allow to change the property if we are in INACTIVE or READY_TO_RUN states */
    ASSERT_TEST (self->state == INACTIVE ||
            self->state == READY_TO_RUN, "Could not set ID. Not in INACTIVE or "
            "READY_TO_RUN state", err_devio_info_state, DMNGR_ERR_INCOMP_STATE);
    self->id = id;

err_devio_info_state:
    return err;
}

uint32_t devio_info_get_id (devio_info_t *self)
{
    assert (self);
    return self->id;
}

dmngr_err_e devio_info_set_llio_type (devio_info_t *self, llio_type_e llio_type)
{
    assert (self);

    dmngr_err_e err = DMNGR_SUCCESS;
    /* Only allow to change the property if we are in INACTIVE or READY_TO_RUN states */
    ASSERT_TEST (self->state == INACTIVE ||
            self->state == READY_TO_RUN, "Could not set LLIO type. Not in "
            "INACTIVE or READY_TO_RUN state", err_devio_info_state,
            DMNGR_ERR_INCOMP_STATE);
    self->type = llio_type;

err_devio_info_state:
    return err;
}

llio_type_e devio_info_get_llio_type (devio_info_t *self)
{
    assert (self);
    return self->type;
}

dmngr_err_e devio_info_set_devio_type (devio_info_t *self, devio_type_e devio_type)
{
    assert (self);

    dmngr_err_e err = DMNGR_SUCCESS;
    /* Only allow to change the property if we are in INACTIVE or READY_TO_RUN states */
    ASSERT_TEST (self->state == INACTIVE ||
            self->state == READY_TO_RUN, "Could not set ID. Not in INACTIVE or "
            "READY_TO_RUN state", err_devio_info_state, DMNGR_ERR_INCOMP_STATE);
    self->devio_type = devio_type;

err_devio_info_state:
    return err;
}

devio_type_e devio_info_get_devio_type (devio_info_t *self)
{
    assert (self);
    return self->devio_type;
}

dmngr_err_e devio_info_set_smio_inst_id (devio_info_t *self, uint32_t smio_inst_id)
{
    assert (self);

    dmngr_err_e err = DMNGR_SUCCESS;
    /* Only allow to change smio_inst_id if we are in INACTIVE or READY_TO_RUN states */
    ASSERT_TEST (self->state == INACTIVE ||
            self->state == READY_TO_RUN, "Could not set SMIO instance id. "
            "Not in INACTIVE or READY_TO_RUN state", err_devio_info_state,
            DMNGR_ERR_INCOMP_STATE);
    self->smio_inst_id = smio_inst_id;

err_devio_info_state:
    return err;
}

uint32_t devio_info_get_smio_inst_id (devio_info_t *self)
{
    assert (self);
    return self->smio_inst_id;
}

dmngr_err_e devio_info_set_dev_pathname (devio_info_t *self, const char *dev_pathname)
{
    assert (self);

    dmngr_err_e err = DMNGR_SUCCESS;
    /* Only allow to change smio_inst_id if we are in INACTIVE or READY_TO_RUN states */
    ASSERT_TEST (self->state == INACTIVE ||
            self->state == READY_TO_RUN, "Could not set SMIO instance id. "
            "Not in INACTIVE or READY_TO_RUN state", err_devio_info_state,
            DMNGR_ERR_INCOMP_STATE);

    if (dev_pathname) {
        if (self->dev_pathname) {
            free (self->dev_pathname);
        }
        self->dev_pathname = strdup (dev_pathname);
        ASSERT_ALLOC(self->dev_pathname, err_name_alloc, DMNGR_ERR_ALLOC);
    }

err_name_alloc:
err_devio_info_state:
    return err;
}

const char *devio_info_get_dev_pathname (devio_info_t *self)
{
    assert (self);
    return self->dev_pathname;
}

char *devio_info_clone_dev_pathname (devio_info_t *self)
{
    assert (self);
    char *dev_pathname = strdup (self->dev_pathname);
    ASSERT_ALLOC(dev_pathname, err_alloc);

err_alloc:
    return dev_pathname;
}

dmngr_err_e devio_info_set_state (devio_info_t *self, devio_state_e state)
{
    assert (self);

    dmngr_err_e err = DMNGR_SUCCESS;
    self->state = state;

    return err;
}

devio_state_e devio_info_get_state (devio_info_t *self)
{
    assert (self);
    return self->state;
}
