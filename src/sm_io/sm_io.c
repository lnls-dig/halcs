/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include "sm_io.h"
#include "exp_ops_codes.h"
#include "rw_param.h"
#include "errhand.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, SM_IO, "[sm_io]",         \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_ALLOC(ptr, SM_IO, "[sm_io]",                 \
            smio_err_str(SMIO_ERR_ALLOC),                   \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                            \
    CHECK_HAL_ERR(err, SM_IO, "[sm_io]",                    \
            smio_err_str (err_type))

static smio_err_e _smio_do_op (void *owner, void *msg);

/************************************************************/
/**************** SMIO Ops wrapper functions ****************/
/************************************************************/
/* Attach an instance of sm_io to dev_io function pointer */
smio_err_e smio_attach (smio_t *self, struct _devio_t *parent)
{
    assert (self);
    smio_err_e err = SMIO_SUCCESS;
    self->parent = parent;

    err = SMIO_FUNC_OPS_NOFAIL_WRAPPER(err, attach, parent);
    ASSERT_TEST(err == SMIO_SUCCESS, "Registered SMIO \"attach\" function error",
        err_func);

err_func:
    return err;
}

/* Deattach an instance of sm_io to dev_io function pointer */
smio_err_e smio_deattach (smio_t *self)
{
    assert (self);
    smio_err_e err = SMIO_SUCCESS;
    self->parent = NULL;

    err = SMIO_FUNC_OPS_NOFAIL_WRAPPER(err, deattach);
    ASSERT_TEST(err == SMIO_SUCCESS, "Registered SMIO \"deattach\" function error",
        err_func);

err_func:
    return err;
}

/* Export (Register) sm_io to handle specific operations */
smio_err_e smio_export_ops (smio_t *self, const disp_op_t** smio_exp_ops)
{
    assert (self);
    assert (smio_exp_ops);

    smio_err_e err = SMIO_SUCCESS;

    hutils_err_e herr = disp_table_insert_all (self->exp_ops_dtable, smio_exp_ops);
    ASSERT_TEST(herr == HUTILS_SUCCESS, "smio_export_ops: Could not export"
            " SMIO ops", err_export_op, SMIO_ERR_EXPORT_OP);

    err = SMIO_FUNC_OPS_NOFAIL_WRAPPER(err, export_ops, smio_exp_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Registered SMIO \"export_ops\" function error",
        err_func);

err_func:
err_export_op:
    return err;
}

/* Unexport (unregister) sm_io to handle specific operations */
smio_err_e smio_unexport_ops (smio_t *self)
{
    assert (self);

    smio_err_e err = SMIO_SUCCESS;
    hutils_err_e herr = disp_table_remove_all (self->exp_ops_dtable);

    ASSERT_TEST(herr == HUTILS_SUCCESS, "smio_export_ops: Could not unexport SMIO ops",
            err_unexport_op, SMIO_ERR_EXPORT_OP);

    err = SMIO_FUNC_OPS_NOFAIL_WRAPPER(err, unexport_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Registered SMIO \"unexport_ops\" function error",
        err_func);

err_func:
err_unexport_op:
    return err;
}

smio_err_e smio_do_op (void *owner, void *msg)
{
    return _smio_do_op (owner, msg);
}

smio_err_e smio_init_exp_ops (smio_t *self, disp_op_t** smio_exp_ops,
        const disp_table_func_fp *func_fps)
{
    assert(self);

    hutils_err_e herr = HUTILS_SUCCESS;
    smio_err_e err = SMIO_SUCCESS;
    herr = disp_table_fill_desc (self->exp_ops_dtable, smio_exp_ops, func_fps);
    ASSERT_TEST(herr == HUTILS_SUCCESS, "smio_export_ops: Could not export"
            " fill SMIO ops description", err_fill_desc_ops, SMIO_ERR_EXPORT_OP);

err_fill_desc_ops:
    return err;
}

/**************** Static Functions ***************/

static smio_err_e _smio_do_op (void *owner, void *msg)
{
    assert (owner);
    assert (msg);

    SMIO_OWNER_TYPE *self = SMIO_EXP_OWNER(owner);
    smio_err_e err = SMIO_SUCCESS;

    /* TODO: The SMIO do_op must not modify the packet! We could pass a copy of the
     * message to it, but we this is in the critical path! Evaluate the impact
     * of doing this */
    SMIO_FUNC_OPS_NOFAIL_WRAPPER(err, do_op, msg);
    ASSERT_TEST (err == SMIO_SUCCESS, "Registered SMIO \"do_op\" function error",
            err_do_op);

    disp_table_t *disp_table = self->exp_ops_dtable;
    msg_err_e merr = msg_handle_mlm_request (owner, msg, disp_table);
    ASSERT_TEST (merr == MSG_SUCCESS, "Error handling request", err_hand_req,
           SMIO_ERR_MSG_NOT_SUPP /* returning a more meaningful error? */);

err_hand_req:
err_do_op:
    return err;
}

/************************************************************/
/************* SMIO thsafe wrapper functions   **************/
/************************************************************/

#define CHECK_FUNC(func_p)                                  \
    do {                                                    \
        if(func_p == NULL) {                                \
            DBE_DEBUG (DBG_SM_IO | DBG_LVL_ERR,             \
                    "[sm_io] %s\n",                         \
                    smio_err_str (SMIO_ERR_FUNC_NOT_IMPL)); \
            return -SMIO_ERR_FUNC_NOT_IMPL;                 \
        }                                                   \
    } while(0)

#define ASSERT_FUNC(func_name)                              \
    do {                                                    \
        assert (self);                                      \
        assert (self->thsafe_client_ops);                   \
        CHECK_FUNC (self->thsafe_client_ops->func_name);    \
    } while(0)

/* Declare wrapper for all SMIO functions API */
#define SMIO_FUNC_WRAPPER(func_name, ...)                   \
{                                                           \
    ASSERT_FUNC(func_name);                                 \
    return self->thsafe_client_ops->func_name (self, ##__VA_ARGS__);  \
}

/**** Open device ****/
int smio_thsafe_client_open (smio_t *self, llio_endpoint_t *endpoint)
    SMIO_FUNC_WRAPPER (thsafe_client_open, endpoint)

/**** Release device ****/
int smio_thsafe_client_release (smio_t *self, llio_endpoint_t *endpoint)
    SMIO_FUNC_WRAPPER (thsafe_client_release, endpoint)

/**** Read data from device ****/
ssize_t smio_thsafe_client_read_16 (smio_t *self, uint64_t offs, uint16_t *data)
{
    ASSERT_FUNC(thsafe_client_read_16);
    return self->thsafe_client_ops->thsafe_client_read_16 (self, self->base | offs, data);
}

ssize_t smio_thsafe_client_read_32 (smio_t *self, uint64_t offs, uint32_t *data)
{
    ASSERT_FUNC(thsafe_client_read_32);
    return self->thsafe_client_ops->thsafe_client_read_32 (self, self->base | offs, data);
}

ssize_t smio_thsafe_client_read_64 (smio_t *self, uint64_t offs, uint64_t *data)
{
    ASSERT_FUNC(thsafe_client_read_64);
    return self->thsafe_client_ops->thsafe_client_read_64 (self, self->base | offs, data);
}

ssize_t smio_thsafe_raw_client_read_16 (smio_t *self, uint64_t offs, uint16_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_read_16, offs, data)
ssize_t smio_thsafe_raw_client_read_32 (smio_t *self, uint64_t offs, uint32_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_read_32, offs, data)
ssize_t smio_thsafe_raw_client_read_64 (smio_t *self, uint64_t offs, uint64_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_read_64, offs, data)

/**** Write data to device ****/
ssize_t smio_thsafe_client_write_16 (smio_t *self, uint64_t offs, const uint16_t *data)
{
    ASSERT_FUNC(thsafe_client_write_16);
    return self->thsafe_client_ops->thsafe_client_write_16 (self, self->base | offs, data);
}

ssize_t smio_thsafe_client_write_32 (smio_t *self, uint64_t offs, const uint32_t *data)
{
    ASSERT_FUNC(thsafe_client_write_32);
    return self->thsafe_client_ops->thsafe_client_write_32 (self, self->base | offs, data);
}

ssize_t smio_thsafe_client_write_64 (smio_t *self, uint64_t offs, const uint64_t *data)
{
    ASSERT_FUNC(thsafe_client_write_64);
    return self->thsafe_client_ops->thsafe_client_write_64 (self, self->base | offs, data);
}

ssize_t smio_thsafe_raw_client_write_16 (smio_t *self, uint64_t offs, const uint16_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_write_16, offs, data)
ssize_t smio_thsafe_raw_client_write_32 (smio_t *self, uint64_t offs, const uint32_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_write_32, offs, data)
ssize_t smio_thsafe_raw_client_write_64 (smio_t *self, uint64_t offs, const uint64_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_write_64, offs, data)

/**** Read data block from device function pointer, size in bytes ****/
ssize_t smio_thsafe_client_read_block (smio_t *self, uint64_t offs, size_t size,
        uint32_t *data)
{
    ASSERT_FUNC(thsafe_client_read_block);
    return self->thsafe_client_ops->thsafe_client_read_block (self, self->base | offs,
            size, data);
}

ssize_t smio_thsafe_raw_client_read_block (smio_t *self, uint64_t offs, size_t size, uint32_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_read_block, offs, size, data)

/**** Write data block from device function pointer, size in bytes ****/
ssize_t smio_thsafe_client_write_block (smio_t *self, uint64_t offs, size_t size,
        const uint32_t *data)
{
    ASSERT_FUNC(thsafe_client_write_block);
    return self->thsafe_client_ops->thsafe_client_write_block (self, self->base | offs,
            size, data);
}

ssize_t smio_thsafe_raw_client_write_block (smio_t *self, uint64_t offs, size_t size, const uint32_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_write_block, offs, size, data)

/**** Read data block via DMA from device, size in bytes ****/
ssize_t smio_thsafe_client_read_dma (smio_t *self, uint64_t offs, size_t size,
        uint32_t *data)
{
    ASSERT_FUNC(thsafe_client_read_dma);
    return self->thsafe_client_ops->thsafe_client_read_dma (self, self->base | offs,
            size, data);
}

ssize_t smio_thsafe_raw_client_read_dma (smio_t *self, uint64_t offs, size_t size, uint32_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_read_dma, offs, size, data)

/**** Write data block via DMA from device, size in bytes ****/
ssize_t smio_thsafe_client_write_dma (smio_t *self, uint64_t offs, size_t size,
        const uint32_t *data)
{
    ASSERT_FUNC(thsafe_client_write_dma);
    return self->thsafe_client_ops->thsafe_client_write_dma (self, self->base | offs,
            size, data);
}

ssize_t smio_thsafe_raw_client_write_dma (smio_t *self, uint64_t offs, size_t size, const uint32_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_write_dma, offs, size, data)

/**** Read device information function pointer ****/
/* int smio_thsafe_raw_client_read_info (smio_t *self, llio_dev_info_t *dev_info)
    SMIO_FUNC_WRAPPER (thsafe_client_read_info, dev_info) Moved to dev_io */

