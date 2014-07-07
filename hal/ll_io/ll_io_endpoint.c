/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "ll_io_endpoint.h"
#include "hal_assert.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, LL_IO, "ll_io_endpoint",  \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_ALLOC(ptr, LL_IO, "ll_io_endpoint",          \
            llio_err_str(LLIO_ERR_ALLOC),                   \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                            \
    CHECK_HAL_ERR(err, LL_IO, "ll_io_endpoint",             \
            llio_err_str (err_type))

static llio_err_e _llio_endpoint_set (llio_endpoint_t *self, const char *name);
static llio_err_e _llio_endpoint_get (llio_endpoint_t *self, char **endpoint);

/* Creates a new instance of the Endpoint*/
llio_endpoint_t * llio_endpoint_new (const char *endpoint)
{
    llio_endpoint_t *self = (llio_endpoint_t *) zmalloc (sizeof *self);
    ASSERT_ALLOC(self, err_self_alloc);

    /* Initilialize llio_endpoint */
    self->opened = false;

    llio_err_e err = _llio_endpoint_set (self, endpoint);
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

/* Register endpoint to llio instance */
llio_err_e llio_endpoint_set (llio_endpoint_t *self, const char *name)
{
    assert (self);
    return _llio_endpoint_set (self, name);
}

/* Get endpoint from name from instance of llio_endpoint */
llio_err_e llio_endpoint_get (llio_endpoint_t *self, char **endpoint)
{
    assert (self);
    return _llio_endpoint_get (self, endpoint);
}

/**************** Helper Functions ***************/

static llio_err_e _llio_endpoint_set (llio_endpoint_t *self, const char *name)
{
    if (self->opened) {
        return LLIO_ERR_SET_ENDP;
    }

    if (name) {
        if (self->name) {
            free (self->name);
        }
        self->name = strdup (name);
        ASSERT_ALLOC(self->name, err_name_alloc);
    }

    return LLIO_SUCCESS;

    /* FIXME Remove goto label as it doesn't enhance anything,
     * as we have only one pssible chance of error */
err_name_alloc:
    return LLIO_ERR_ALLOC;
}

/* Get endpoint from name from instance of llio_endpoint */
static llio_err_e _llio_endpoint_get (llio_endpoint_t *self, char **endpoint)
{
    *endpoint = strdup (self->name);
    ASSERT_ALLOC(*endpoint, err_alloc);

    return LLIO_SUCCESS;

err_alloc:
    return LLIO_ERR_ALLOC;
}
