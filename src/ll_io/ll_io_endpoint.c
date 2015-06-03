/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include "bpm_server.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, LL_IO, "ll_io_endpoint",          \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)       \
    ASSERT_HAL_ALLOC(ptr, LL_IO, "ll_io_endpoint",                  \
            llio_err_str(LLIO_ERR_ALLOC),                           \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                    \
    CHECK_HAL_ERR(err, LL_IO, "ll_io_endpoint",                     \
            llio_err_str (err_type))

/* Device endpoint structure */
struct _llio_endpoint_t {
    bool open;                          /* True if device if already opened */
    char *name;                         /* Name of the endpoint, e.g., "/dev/fpga0"
                                           and "tcp://192.168.0.100:5556" */
};

/* Set endpoint name */
static llio_err_e _llio_endpoint_set_name (llio_endpoint_t *self, const char *name);
/* Get endpoint name */
static const char *_llio_endpoint_get_name (llio_endpoint_t *self);
/* Clone endpoint name */
static char *_llio_endpoint_clone_name (llio_endpoint_t *self);

/* Creates a new instance of the Endpoint*/
llio_endpoint_t * llio_endpoint_new (const char *endpoint)
{
    llio_endpoint_t *self = (llio_endpoint_t *) zmalloc (sizeof *self);
    ASSERT_ALLOC(self, err_self_alloc);

    /* Initilialize llio_endpoint */
    self->open = false;

    llio_err_e err = _llio_endpoint_set_name (self, endpoint);
    ASSERT_TEST(err==LLIO_SUCCESS, llio_err_str(err), err_name_alloc);

    return self;

err_name_alloc:
    free (self);
err_self_alloc:
    return NULL;
}

/* Destroy an instance of the Endpoint */
llio_err_e llio_endpoint_destroy (llio_endpoint_t **self_p)
{
    assert (self_p);

    if (*self_p) {
        llio_endpoint_t *self = *self_p;

        /* Starting destructing by the last resource */
        free (self->name);
        free (self);
        *self_p = NULL;
    }

    return LLIO_SUCCESS;
}

/* Set endpoint open status */
llio_err_e llio_endpoint_set_open (llio_endpoint_t *self, bool open)
{
    assert (self);
    self->open = open;

    return LLIO_SUCCESS;
}

/* Get endpoint open status */
bool llio_endpoint_get_open (llio_endpoint_t *self)
{
    assert (self);
    assert (open);

    return self->open;
}

/* Register endpoint to llio instance */
llio_err_e llio_endpoint_set_name (llio_endpoint_t *self, const char *name)
{
    assert (self);
    return _llio_endpoint_set_name (self, name);
}

/* Get endpoint from name from instance of llio_endpoint */
const char *llio_endpoint_get_name (llio_endpoint_t *self)
{
    assert (self);
    return _llio_endpoint_get_name (self);
}

/* Clone endpoint from name from instance of llio_endpoint */
char *llio_endpoint_clone_name (llio_endpoint_t *self)
{
    assert (self);
    return _llio_endpoint_clone_name (self);
}

/**************** Helper Functions ***************/

/* Set endpoint name if not already open */
static llio_err_e _llio_endpoint_set_name (llio_endpoint_t *self, const char *name)
{
    assert (self);
    llio_err_e err = LLIO_SUCCESS;

    ASSERT_TEST(self->open == false, "Could not set endpoint name. Endpoint" \
            "is already open\n", err_open_endp, LLIO_ERR_SET_ENDP);

    if (name) {
        if (self->name) {
            free (self->name);
        }
        self->name = strdup (name);
        ASSERT_ALLOC(self->name, err_name_alloc, LLIO_ERR_ALLOC);
    }

err_name_alloc:
err_open_endp:
    return err;
}

/* Get endpoint from name from instance of llio_endpoint */
static const char *_llio_endpoint_get_name (llio_endpoint_t *self)
{
    assert (self);
    return self->name;
}

/* Clone endpoint from name from instance of llio_endpoint */
static char *_llio_endpoint_clone_name (llio_endpoint_t *self)
{
    assert (self);

    char *endpoint = strdup (self->name);
    ASSERT_ALLOC(endpoint, err_alloc);

err_alloc:
    return endpoint;
}
