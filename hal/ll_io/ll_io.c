/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "ll_io.h"
#include "ll_io_pcie.h"
#include "ll_io_eth.h"
#include "hal_assert.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, LL_IO, "[ll_io]",         \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_ALLOC(ptr, LL_IO, "[ll_io]",                 \
            llio_err_str(LLIO_ERR_ALLOC),                   \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                            \
    CHECK_HAL_ERR(err, LL_IO, "[ll_io]",                    \
            llio_err_str (err_type))

/* Register Low-level operations to llio instance. Helpper function */
static llio_err_e _llio_register_ops (llio_type_e type, const llio_ops_t **llio_ops);

/* Creates a new instance of the Low-level I/O */
llio_t * llio_new (char *name, char *endpoint, llio_type_e type, int verbose)
{
    assert (name);
    assert (endpoint);

    llio_t *self = (llio_t *) zmalloc (sizeof *self);
    ASSERT_ALLOC(self, err_self_alloc);

    /* Initialize Low-level IO type */
    self->type = type;
    self->dev_handler = NULL;       /* This is set by the device functions */
    self->name = strdup (name);
    ASSERT_ALLOC(self->name, err_name_alloc);
    self->verbose = verbose;

    /* Initilialize llio_endpoint */
    self->endpoint = llio_endpoint_new (endpoint);
    ASSERT_ALLOC(self->endpoint, err_endpoint_alloc);

    /* Initilialize llio_dev_info */
    /* self->dev_info = llio_dev_info_new ();
    ASSERT_ALLOC(self->dev_info, err_dev_info_alloc); Moved to dev_io */

    /* Initilialize llio_ops */
    /* self->ops = (llio_ops_t *) zmalloc (sizeof *self->ops); */
    /* ASSERT_ALLOC(self->ops, err_ops_alloc); */
    /* Nullify every ops field to indicate a non-implemented function */
    /* *self->ops = (const llio_ops_t) {0}; */
    /* Attach Low-level operation to instance of llio */
    _llio_register_ops (type, &self->ops);

    DBE_DEBUG (DBG_LL_IO | DBG_LVL_INFO, "[ll_io] Created instance of llio\n");
    return self;

    /*err_ops_alloc:
      zlist_destroy(&self->dev_info->sdb); */
/* err_dev_info_alloc:
    llio_endpoint_destroy (&self->endpoint); */
err_endpoint_alloc:
    free (self->name);
err_name_alloc:
    free (self);
err_self_alloc:
    return NULL;
}

/* Destroy an instance of the Low-level I/O */
llio_err_e llio_destroy (llio_t **self_p)
{
    assert (self_p);

    if (*self_p) {
        llio_t *self = *self_p;

        /* Starting destructing by the last resource */
        /* llio_dev_info_destroy (&self->dev_info); Moved to dev_io */
        llio_endpoint_destroy (&self->endpoint);
        free (self->name);

        free (self);
        *self_p = NULL;
    }

    return LLIO_SUCCESS;
}

llio_err_e llio_set_endpoint (llio_t *self, const char *name)
{
    return llio_endpoint_set (self->endpoint, name);
}

llio_err_e llio_get_endpoint (llio_t *self, char **name)
{
    return llio_endpoint_get (self->endpoint, name);
}

/**************** Helper Functions ***************/

/* Register Low-level operations to llio instance. Helpper function */
static llio_err_e _llio_register_ops (llio_type_e type, const llio_ops_t **ops)
{
    switch (type) {
        case GENERIC_DEV:
            *ops = NULL;
            return LLIO_ERR_INV_FUNC_PARAM;

        case PCIE_DEV:
            *ops = &llio_ops_pcie;
            break;

        case ETH_DEV:
            *ops = NULL;
            return LLIO_ERR_INV_FUNC_PARAM;

        default:
            *ops = NULL;
            return LLIO_ERR_INV_FUNC_PARAM;
    }

    DBE_DEBUG (DBG_LL_IO | DBG_LVL_INFO, "[ll_io] Ops set\n");
    return LLIO_SUCCESS;
}

/********* Low-level generic methods API *********/

#define CHECK_FUNC(func_p)                              \
    do {                                                \
        if(func_p == NULL) {                            \
            DBE_DEBUG (DBG_LL_IO | DBG_LVL_ERR,         \
                    "[ll_io] %s\n",                     \
                    llio_err_str (LLIO_ERR_FUNC_NOT_IMPL)); \
            return -LLIO_ERR_FUNC_NOT_IMPL;             \
        }                                               \
    } while(0)

/* Declare wrapper for all LLIO functions API */
#define LLIO_FUNC_WRAPPER(func_name, ...)               \
{                                                       \
    assert (self);                                      \
    assert (self->ops);                                 \
    CHECK_FUNC (self->ops->func_name);                  \
    return self->ops->func_name (self, ##__VA_ARGS__);  \
}

/**** Open device ****/
int llio_open (llio_t *self, llio_endpoint_t *endpoint)
    LLIO_FUNC_WRAPPER (open, endpoint)

/**** Release device ****/
int llio_release (llio_t *self, llio_endpoint_t *endpoint)
    LLIO_FUNC_WRAPPER (release, endpoint)

/**** Read data from device ****/
ssize_t llio_read_16 (llio_t *self, loff_t offs, uint16_t *data)
    LLIO_FUNC_WRAPPER (read_16, offs, data)
ssize_t llio_read_32 (llio_t *self, loff_t offs, uint32_t *data)
    LLIO_FUNC_WRAPPER (read_32, offs, data)
ssize_t llio_read_64 (llio_t *self, loff_t offs, uint64_t *data)
    LLIO_FUNC_WRAPPER (read_64, offs, data)

/**** Write data to device ****/
ssize_t llio_write_16 (llio_t *self, loff_t offs, const uint16_t *data)
    LLIO_FUNC_WRAPPER (write_16, offs, data)
ssize_t llio_write_32 (llio_t *self, loff_t offs, const uint32_t *data)
    LLIO_FUNC_WRAPPER (write_32, offs, data)
ssize_t llio_write_64 (llio_t *self, loff_t offs, const uint64_t *data)
    LLIO_FUNC_WRAPPER (write_64, offs, data)

/**** Read data block from device function pointer, size in bytes ****/
ssize_t llio_read_block (llio_t *self, loff_t offs, size_t size, uint32_t *data)
    LLIO_FUNC_WRAPPER (read_block, offs, size, data)

/**** Write data block from device function pointer, size in bytes ****/
ssize_t llio_write_block (llio_t *self, loff_t offs, size_t size, uint32_t *data)
    LLIO_FUNC_WRAPPER (write_block, offs, size, data)

/**** Read data block via DMA from device, size in bytes ****/
ssize_t llio_read_dma (llio_t *self, loff_t offs, size_t size, uint32_t *data)
    LLIO_FUNC_WRAPPER (read_dma, offs, size, data)

/**** Write data block via DMA from device, size in bytes ****/
ssize_t llio_write_dma (llio_t *self, loff_t offs, size_t size, uint32_t *data)
    LLIO_FUNC_WRAPPER (write_dma, offs, size, data)

/**** Read device information function pointer ****/
/* int llio_read_info (llio_t *self, llio_dev_info_t *dev_info)
    LLIO_FUNC_WRAPPER (read_info, dev_info) Moved to dev_io */

