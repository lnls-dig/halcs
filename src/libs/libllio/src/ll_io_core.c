/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "ll_io.h"

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

/* LLIO class object */
struct _llio_t {
    void *dev_handler;                  /* Generic pointer to a device handler. This
                                            must be cast to a specific type by the
                                            devices functions */
    char *name;                         /* Identification of this llio instance */
    int verbose;                        /* Print activity to stdout */
    uint64_t sdb_prefix_addr;           /* SDB prefix address. Used to read/write to the
                                           SDB address space. To be set by the specific ops */

    /* Endpoint to connect to */
    llio_endpoint_t *endpoint;
    /* Device operations */
    const llio_ops_t *ops;
};

/* Register Low-level operations to llio instance. Helpper function */
static llio_err_e _llio_register_ops (const llio_ops_t **ops, const llio_ops_t *reg_ops);
/* Unregister Low-level operations to llio instance. Helpper function */
static llio_err_e _llio_unregister_ops (const llio_ops_t **ops);
/* Get open endpoint status */
static bool _llio_get_endpoint_open (llio_t *self);

/* Creates a new instance of the Low-level I/O */
llio_t * llio_new (char *name, char *endpoint, const llio_ops_t *reg_ops, int verbose)
{
    assert (name);
    assert (endpoint);

    llio_t *self = (llio_t *) zmalloc (sizeof *self);
    ASSERT_ALLOC(self, err_self_alloc);

    /* Initialize Low-level IO type */
    self->dev_handler = NULL;       /* This is set by the device functions */
    self->name = strdup (name);
    ASSERT_ALLOC(self->name, err_name_alloc);
    self->verbose = verbose;
    /* This shoule be set by the specific operations (e.g., PCIe, ETH) */
    self->sdb_prefix_addr = 0x0;

    /* Initilialize llio_endpoint */
    self->endpoint = NULL;
    if (endpoint != NULL) {
        self->endpoint = llio_endpoint_new (endpoint);
        ASSERT_ALLOC(self->endpoint, err_endpoint_alloc);
    }

    /* Initilialize llio_dev_info */
    /* self->dev_info = llio_dev_info_new ();
    ASSERT_ALLOC(self->dev_info, err_dev_info_alloc); Moved to dev_io */

    /* Initilialize llio_ops */
    self->ops = NULL;
    /* Attach Low-level operation to instance of llio */
    _llio_register_ops (&self->ops, reg_ops);

    DBE_DEBUG (DBG_LL_IO | DBG_LVL_INFO, "[ll_io] Created instance of llio\n");
    return self;

    /*err_ops_alloc:
      zlistx_destroy(&self->dev_info->sdb); */
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
        _llio_unregister_ops (&self->ops);
        /* llio_dev_info_destroy (&self->dev_info); Moved to dev_io */
        llio_endpoint_destroy (&self->endpoint);
        free (self->name);

        free (self);
        *self_p = NULL;
    }

    return LLIO_SUCCESS;
}

/* Set endpoint */
llio_err_e llio_set_endpoint (llio_t *self, llio_endpoint_t *endpoint)
{
    assert (self);
    llio_err_e err = LLIO_SUCCESS;

    /* Check if device is already opened. If it is, we can't change the
     * endpoint */
    ASSERT_TEST(_llio_get_endpoint_open (self), "Could not set endpoint. "
            "Endpoint is open", err_endpoint_set, LLIO_ERR_SET_ENDP);

    err = llio_endpoint_destroy (&self->endpoint);
    ASSERT_TEST(err == LLIO_SUCCESS, "Could not close endpoint",
            err_endpoint_close, LLIO_ERR_SET_ENDP);
    self->endpoint = endpoint;

err_endpoint_close:
err_endpoint_set:
    return err;
}

/* Get endpoint */
const llio_endpoint_t *llio_get_endpoint (llio_t *self)
{
    assert (self);
    return self->endpoint;
}

llio_err_e llio_set_endpoint_open (llio_t *self, bool open)
{
    return llio_endpoint_set_open (self->endpoint, open);
}

bool llio_get_endpoint_open (llio_t *self)
{
    return _llio_get_endpoint_open (self);
}

llio_err_e llio_set_endpoint_name (llio_t *self, const char *name)
{
    return llio_endpoint_set_name (self->endpoint, name);
}

const char *llio_get_endpoint_name (llio_t *self)
{
    return llio_endpoint_get_name (self->endpoint);
}

char *llio_clone_endpoint_name (llio_t *self)
{
    return llio_endpoint_clone_name (self->endpoint);
}

llio_err_e llio_set_dev_handler (llio_t *self, void *dev_handler)
{
    self->dev_handler = dev_handler;
    return LLIO_SUCCESS;
}

void *llio_get_dev_handler (llio_t *self)
{
    return self->dev_handler;
}

const char *llio_get_ops_name (llio_t *self)
{
    assert (self);

    if (self->ops == NULL) {
        return NULL;
    }
    return self->ops->name;
}

llio_err_e llio_set_sdb_prefix_addr (llio_t *self, uint64_t sdb_prefix_addr)
{
    assert (self);
    self->sdb_prefix_addr = sdb_prefix_addr;
    return LLIO_SUCCESS;
}

uint64_t llio_get_sdb_prefix_addr (llio_t *self)
{
    assert (self);
    return self->sdb_prefix_addr;
}

/**************** Static function ****************/

static bool _llio_get_endpoint_open (llio_t *self)
{
    return llio_endpoint_get_open (self->endpoint);
}

/**************** Helper Functions ***************/

/* Register Low-level operations to llio instance. Helpper function */
static llio_err_e _llio_register_ops (const llio_ops_t **ops, const llio_ops_t *reg_ops)
{
    *ops = reg_ops;
    DBE_DEBUG (DBG_LL_IO | DBG_LVL_INFO, "[ll_io] Ops set\n");
    return LLIO_SUCCESS;
}

static llio_err_e _llio_unregister_ops (const llio_ops_t **ops)
{
    *ops = NULL;
    DBE_DEBUG (DBG_LL_IO | DBG_LVL_INFO, "[ll_io] Ops unset\n");
    return LLIO_SUCCESS;
}

/************************************************************/
/**************** Low Level generic methods API *************/
/************************************************************/

#define CHECK_FUNC(func_p)                              \
    do {                                                \
        if(func_p == NULL) {                            \
            DBE_DEBUG (DBG_LL_IO | DBG_LVL_ERR,         \
                    "[ll_io] %s\n",                     \
                    llio_err_str (LLIO_ERR_FUNC_NOT_IMPL)); \
            return -LLIO_ERR_FUNC_NOT_IMPL;             \
        }                                               \
    } while(0)

#define ASSERT_FUNC(func_name)                          \
    do {                                                \
        assert (self);                                  \
        assert (self->ops);                             \
        CHECK_FUNC (self->ops->func_name);              \
    } while(0)

/* Declare wrapper for all LLIO functions API */
#define LLIO_FUNC_WRAPPER(func_name, ...)               \
{                                                       \
    ASSERT_FUNC(func_name);                             \
    return self->ops->func_name (self, ##__VA_ARGS__);  \
}

/**** Open device ****/
int llio_open (llio_t *self, llio_endpoint_t *endpoint)
    LLIO_FUNC_WRAPPER (open, endpoint)

/**** Release device ****/
int llio_release (llio_t *self, llio_endpoint_t *endpoint)
    LLIO_FUNC_WRAPPER (release, endpoint)

/**** Read data from device ****/
ssize_t llio_read_16 (llio_t *self, uint64_t offs, uint16_t *data)
    LLIO_FUNC_WRAPPER (read_16, offs, data)
ssize_t llio_read_32 (llio_t *self, uint64_t offs, uint32_t *data)
    LLIO_FUNC_WRAPPER (read_32, offs, data)
ssize_t llio_read_64 (llio_t *self, uint64_t offs, uint64_t *data)
    LLIO_FUNC_WRAPPER (read_64, offs, data)

/**** Write data to device ****/
ssize_t llio_write_16 (llio_t *self, uint64_t offs, const uint16_t *data)
    LLIO_FUNC_WRAPPER (write_16, offs, data)
ssize_t llio_write_32 (llio_t *self, uint64_t offs, const uint32_t *data)
    LLIO_FUNC_WRAPPER (write_32, offs, data)
ssize_t llio_write_64 (llio_t *self, uint64_t offs, const uint64_t *data)
    LLIO_FUNC_WRAPPER (write_64, offs, data)

/**** Read data block from device function pointer, size in bytes ****/
ssize_t llio_read_block (llio_t *self, uint64_t offs, size_t size, uint32_t *data)
    LLIO_FUNC_WRAPPER (read_block, offs, size, data)

/**** Write data block from device function pointer, size in bytes ****/
ssize_t llio_write_block (llio_t *self, uint64_t offs, size_t size, uint32_t *data)
    LLIO_FUNC_WRAPPER (write_block, offs, size, data)

/**** Read data block via DMA from device, size in bytes ****/
ssize_t llio_read_dma (llio_t *self, uint64_t offs, size_t size, uint32_t *data)
    LLIO_FUNC_WRAPPER (read_dma, offs, size, data)

/**** Write data block via DMA from device, size in bytes ****/
ssize_t llio_write_dma (llio_t *self, uint64_t offs, size_t size, uint32_t *data)
    LLIO_FUNC_WRAPPER (write_dma, offs, size, data)

/**** Read device information function pointer ****/
/* int llio_read_info (llio_t *self, llio_dev_info_t *dev_info)
    LLIO_FUNC_WRAPPER (read_info, dev_info) Moved to dev_io */

