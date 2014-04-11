/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include "sm_io.h"
#include "exp_ops_codes.h"
#include "hal_assert.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label)  \
    ASSERT_HAL_TEST(test_boolean, SM_IO, "[sm_io]",         \
            err_str, err_goto_label)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label)                   \
    ASSERT_HAL_ALLOC(ptr, SM_IO, "[sm_io]",                 \
            smio_err_str(SMIO_ERR_ALLOC),                   \
            err_goto_label)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                            \
    CHECK_HAL_ERR(err, SM_IO, "[sm_io]",                    \
            smio_err_str (err_type))

#define CHECK_FUNC(func_p)                              \
    do {                                                \
        if(func_p == NULL) {                            \
            DBE_DEBUG (DBG_SM_IO | DBG_LVL_ERR,         \
                    "[sm_io] %s\n",                     \
                    smio_err_str (SMIO_ERR_FUNC_NOT_IMPL)); \
            return -SMIO_ERR_FUNC_NOT_IMPL;             \
        }                                               \
    } while(0)

/* Declare wrapper for all SMIO functions API */
#define SMIO_FUNC_WRAPPER(func_name, ...)               \
{                                                       \
    assert (self);                                      \
    assert (self->thsafe_client_ops);                   \
    CHECK_FUNC (self->thsafe_client_ops->func_name);    \
    return self->thsafe_client_ops->func_name (self, ##__VA_ARGS__);  \
}

#define SMIO_FUNC_OPS_NOFAIL_WRAPPER(err, func_name, ...)   \
    do {                                                \
        if (self->ops && self->ops->func_name) {        \
            smio_err_e local_err = self->ops->func_name (self, ##__VA_ARGS__);  \
            err = (local_err != SMIO_ERR_FUNC_NOT_IMPL) ? \
                local_err : err;                        \
        }                                               \
    } while (0)

/************************************************************/
/**************** SMIO Ops wrapper functions ****************/
/************************************************************/
/* Attach an instance of sm_io to dev_io function pointer */
smio_err_e smio_attach (smio_t *self, struct _devio_t *parent)
{
    assert (self);
    smio_err_e err = SMIO_SUCCESS;
    self->parent = parent;

    SMIO_FUNC_OPS_NOFAIL_WRAPPER(err, attach, parent);
    return err;
}

/* Deattach an instance of sm_io to dev_io function pointer */
smio_err_e smio_deattach (smio_t *self)
{
    assert (self);
    smio_err_e err = SMIO_SUCCESS;
    self->parent = NULL;

    SMIO_FUNC_OPS_NOFAIL_WRAPPER(err, deattach);
    return SMIO_SUCCESS;
}

/* Export (Register) sm_io to handle specific operations */
smio_err_e smio_export_ops (smio_t *self, const smio_exp_ops_t* smio_exp_ops)
{
    assert (self);
    assert (smio_exp_ops);

    /* FIXME: Dispatch table disp_table_insert_all implements almost the
     * same code! Try to reuse it! */
    const smio_exp_ops_t* smio_exp_ops_it = smio_exp_ops;
    smio_err_e err = SMIO_SUCCESS;
    /* FIXME: Iterator is not very good as it has to check an specific field */
    for (smio_exp_ops_it = smio_exp_ops; smio_exp_ops_it->func_fp != NULL;
            smio_exp_ops_it++) {
        halutils_err_e herr = disp_table_insert (self->exp_ops_dtable,
            smio_exp_ops_it->opcode, smio_exp_ops_it->func_fp);

        if (herr != HALUTILS_SUCCESS) {
            err = SMIO_ERR_EXPORT_OP;
            goto err_export_op;
        }
    }

    SMIO_FUNC_OPS_NOFAIL_WRAPPER(err, export_ops, smio_exp_ops);

err_export_op:
    return err;
}

/* Unexport (unregister) sm_io to handle specific operations */
smio_err_e smio_unexport_ops (smio_t *self)
{
    assert (self);

    smio_err_e err = SMIO_SUCCESS;
    halutils_err_e herr = disp_table_remove_all (self->exp_ops_dtable);

    if (herr != HALUTILS_SUCCESS) {
        err = SMIO_ERR_EXPORT_OP;
        goto err_unexport_op;
    }

    SMIO_FUNC_OPS_NOFAIL_WRAPPER(err, unexport_ops);

err_unexport_op:
    return err;
}

smio_err_e smio_do_op (void *owner, void *msg)
{
    assert (owner);
    assert (msg);

    smio_t *self = (smio_t *) owner;
    exp_msg_zmq_t *exp_msg = (exp_msg_zmq_t *) msg;
    smio_err_e err = SMIO_SUCCESS;

    /* TODO: The SMIO do_op must not modify the packet! We could pass a copy of the
     * message to it, but we this is in the critical path! Evaluate the imapct
     * of doing this */
    SMIO_FUNC_OPS_NOFAIL_WRAPPER(err, do_op, msg);
    ASSERT_TEST (err==SMIO_SUCCESS, "Error executing SMIO do_op ()",
            err_do_op);

    /* Our simple packet is composed of:
     * frame 0: operation
     * frame n: arguments*/
    zframe_t *opcode = zmsg_pop (*exp_msg->msg);
    err = (opcode == NULL) ? SMIO_ERR_WRONG_NARGS : SMIO_SUCCESS;
    ASSERT_TEST(opcode != NULL, "Could not receive opcode", err_null_opcode);

    if (zframe_size (opcode) != EXP_OPS_OPCODE_SIZE) {
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_ERR,
                "[sm_io] Invalid opcode size received\n");
        err = SMIO_ERR_WRONG_NARGS;
        goto err_wrong_opcode_size;
    }

    uint32_t opcode_data = *(uint32_t *) zframe_data (opcode);
    if (opcode_data > SMIO_MAX_OPS-1) {
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_ERR,
                "[sm_io] Invalid opcode received\n");
        err = SMIO_ERR_WRONG_NARGS;
        goto err_invalid_opcode;
    }

    /* Do the actual work... */
    disp_table_call (self->exp_ops_dtable, opcode_data, self, exp_msg);

err_invalid_opcode:
err_wrong_opcode_size:
    zframe_destroy (&opcode);
err_null_opcode:
err_do_op:
    /* Should reply_to field be zframe_t ** type ?*/
    zframe_destroy (&exp_msg->reply_to);
    zmsg_destroy (exp_msg->msg);
    return err;
}

/************************************************************/
/************* SMIO thsafe wrapper functions   **************/
/************************************************************/

/**** Open device ****/
int smio_thsafe_client_open (smio_t *self, llio_endpoint_t *endpoint)
    SMIO_FUNC_WRAPPER (thsafe_client_open, endpoint)

/**** Release device ****/
int smio_thsafe_client_release (smio_t *self, llio_endpoint_t *endpoint)
    SMIO_FUNC_WRAPPER (thsafe_client_release, endpoint)

/**** Read data from device ****/
ssize_t smio_thsafe_client_read_16 (smio_t *self, loff_t offs, uint16_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_read_16, offs, data)
ssize_t smio_thsafe_client_read_32 (smio_t *self, loff_t offs, uint32_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_read_32, offs, data)
ssize_t smio_thsafe_client_read_64 (smio_t *self, loff_t offs, uint64_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_read_64, offs, data)

/**** Write data to device ****/
ssize_t smio_thsafe_client_write_16 (smio_t *self, loff_t offs, const uint16_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_write_16, offs, data)
ssize_t smio_thsafe_client_write_32 (smio_t *self, loff_t offs, const uint32_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_write_32, offs, data)
ssize_t smio_thsafe_client_write_64 (smio_t *self, loff_t offs, const uint64_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_write_64, offs, data)

/**** Read data block from device function pointer, size in bytes ****/
ssize_t smio_thsafe_client_read_block (smio_t *self, loff_t offs, size_t size, uint32_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_read_block, offs, size, data)

/**** Write data block from device function pointer, size in bytes ****/
ssize_t smio_thsafe_client_write_block (smio_t *self, loff_t offs, size_t size, const uint32_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_write_block, offs, size, data)

/**** Read data block via DMA from device, size in bytes ****/
ssize_t smio_thsafe_client_read_dma (smio_t *self, loff_t offs, size_t size, uint32_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_read_dma, offs, size, data)

/**** Write data block via DMA from device, size in bytes ****/
ssize_t smio_thsafe_client_write_dma (smio_t *self, loff_t offs, size_t size, const uint32_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_write_dma, offs, size, data)

/**** Read device information function pointer ****/
/* int smio_thsafe_client_read_info (smio_t *self, llio_dev_info_t *dev_info)
    SMIO_FUNC_WRAPPER (thsafe_client_read_info, dev_info) Moved to dev_io */

