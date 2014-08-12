/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "sm_pr.h"
#include "sm_pr_err.h"
#include "sm_pr_spi.h"
#include "hal_assert.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, SM_PR, "[sm_pr]",         \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_ALLOC(ptr, SM_PR, "[sm_pr]",                 \
            smpr_err_str(SMPR_ERR_ALLOC),                   \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                            \
    CHECK_HAL_ERR(err, SM_PR, "[sm_pr]",                    \
            smpr_err_str (err_type))

static smpr_err_e _smpr_register_proto_ops (smpr_type_e type,
        const smpr_proto_ops_t **ops);
static smpr_err_e _smpr_unregister_proto_ops (const smpr_proto_ops_t **ops);

/* Creates a new instance of the Low-level I/O */
smpr_t * smpr_new (char *name, smio_t *parent, smpr_type_e type, int verbose)
{
    assert (name);
    assert (parent);

    smpr_t *self = (smpr_t *) zmalloc (sizeof *self);
    ASSERT_ALLOC(self, err_self_alloc);

    /* Initialize Protocol type */
    self->type = type;
    self->proto_handler = NULL;       /* This is set by the specific protocol functions */
    self->name = strdup (name);
    ASSERT_ALLOC(self->name, err_name_alloc);
    self->verbose = verbose;

    /* Initilialize SMIO parent */
    self->parent = parent;
    ASSERT_TEST(self->parent != NULL, "SMIO parent cannot be NULL",
            err_parent_null);

    /* Attach protocol operation to instance of smpr */
    smpr_err_e err = _smpr_register_proto_ops (type, &self->ops);
    ASSERT_TEST(err == SMPR_SUCCESS, "Could not register SMPR operation",
            err_register_ops);

    DBE_DEBUG (DBG_SM_PR | DBG_LVL_INFO, "[sm_pr] Created instance of smpr\n");
    return self;

err_register_ops:
err_parent_null:
    free (self->name);
err_name_alloc:
    free (self);
err_self_alloc:
    return NULL;
}

/* Destroy an instance of the Low-level I/O */
smpr_err_e smpr_destroy (smpr_t **self_p)
{
    assert (self_p);

    if (*self_p) {
        smpr_t *self = *self_p;

        _smpr_unregister_proto_ops (&self->ops);
        free (self->name);
        free (self);
        *self_p = NULL;
    }

    return SMPR_SUCCESS;
}

/**************** Helper Functions ***************/

/* Register Specific Protocol operations to smpr instance. Helper function */
static smpr_err_e _smpr_register_proto_ops (smpr_type_e type, const smpr_proto_ops_t **ops)
{
    switch (type) {
        case SMPR_SPI:
            *ops = &smpr_proto_ops_spi;
            break;

        /*case SMPR_I2C:
            *ops = &smpr_proto_ops_i2c;
            break;

        case SMPR_1WIRE:
            *ops = &smpr_proto_ops_1wire;
            break;

        case SMPR_GPIO:
            *ops = &smpr_proto_ops_gpio;
            break;

        case SMPR_BYPASS:
            *ops = &smpr_proto_ops_bypass;
            break;*/

        default:
            *ops = NULL;
            return SMPR_ERR_INV_FUNC_PARAM;
    }

    DBE_DEBUG (DBG_SM_PR | DBG_LVL_INFO, "[sm_pr] Proto ops set\n");
    return SMPR_SUCCESS;
}

/* Unregister Specific Protocol operations to smpr instance. Helper function */
static smpr_err_e _smpr_unregister_proto_ops (const smpr_proto_ops_t **ops)
{
    *ops = NULL;
    DBE_DEBUG (DBG_SM_PR | DBG_LVL_INFO, "[sm_pr] Proto ops unset\n");
    return SMPR_SUCCESS;
}

/********* Low-level generic methods API *********/

#define CHECK_FUNC(func_p)                              \
    do {                                                \
        if(func_p == NULL) {                            \
            DBE_DEBUG (DBG_SM_PR | DBG_LVL_ERR,         \
                    "[ll_io] %s\n",                     \
                    smpr_err_str (SMPR_ERR_FUNC_NOT_IMPL)); \
            return -SMPR_ERR_FUNC_NOT_IMPL;             \
        }                                               \
    } while(0)

/* Declare wrapper for all SMPR functions API */
#define SMPR_FUNC_WRAPPER(func_name, ...)               \
{                                                       \
    assert (self);                                      \
    assert (self->ops);                                 \
    CHECK_FUNC (self->ops->func_name);                  \
    return self->ops->func_name (self, ##__VA_ARGS__);  \
}

/**** Open device ****/
int smpr_open (smpr_t *self, uint32_t base, void* args)
    SMPR_FUNC_WRAPPER (proto_open, base, args)

/**** Release device ****/
int smpr_release (smpr_t *self)
    SMPR_FUNC_WRAPPER (proto_release)

/**** Read data from device ****/
ssize_t smpr_read_16 (smpr_t *self, loff_t offs, uint16_t *data, uint32_t flags)
    SMPR_FUNC_WRAPPER (proto_read_16, offs, data, flags)
ssize_t smpr_read_32 (smpr_t *self, loff_t offs, uint32_t *data, uint32_t flags)
    SMPR_FUNC_WRAPPER (proto_read_32, offs, data, flags)
ssize_t smpr_read_64 (smpr_t *self, loff_t offs, uint64_t *data, uint32_t flags)
    SMPR_FUNC_WRAPPER (proto_read_64, offs, data, flags)

/**** Write data to device ****/
ssize_t smpr_write_16 (smpr_t *self, loff_t offs, const uint16_t *data, uint32_t flags)
    SMPR_FUNC_WRAPPER (proto_write_16, offs, data, flags)
ssize_t smpr_write_32 (smpr_t *self, loff_t offs, const uint32_t *data, uint32_t flags)
    SMPR_FUNC_WRAPPER (proto_write_32, offs, data, flags)
ssize_t smpr_write_64 (smpr_t *self, loff_t offs, const uint64_t *data, uint32_t flags)
    SMPR_FUNC_WRAPPER (proto_write_64, offs, data, flags)

/**** Read data block from device function pointer, size in bytes ****/
ssize_t smpr_read_block (smpr_t *self, loff_t offs, size_t size, uint32_t *data, uint32_t flags)
    SMPR_FUNC_WRAPPER (proto_read_block, offs, size, data, flags)

/**** Write data block from device function pointer, size in bytes ****/
ssize_t smpr_write_block (smpr_t *self, loff_t offs, size_t size, uint32_t *data, uint32_t flags)
    SMPR_FUNC_WRAPPER (proto_write_block, offs, size, data, flags)

/**** Read data block from via DMA from protocol function pointer, size in bytes ****/
ssize_t smpr_read_dma (smpr_t *self, loff_t offs, size_t size, uint32_t *data, uint32_t flags)
    SMPR_FUNC_WRAPPER (proto_read_dma, offs, size, data, flags)

/**** Write data block via DMA from protocol function pointer, size in bytes ****/
ssize_t smpr_write_dma (smpr_t *self, loff_t offs, size_t size, uint32_t *data, uint32_t flags)
    SMPR_FUNC_WRAPPER (proto_write_dma, offs, size, data, flags)
