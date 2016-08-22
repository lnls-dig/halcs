/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "halcs_server.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, MSG, "[smio_thsafe_server:zmq]", \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_ALLOC(ptr, MSG, "[smio_thsafe_server:zmq]",  \
            msg_err_str(MSG_ERR_ALLOC),                     \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                            \
    CHECK_HAL_ERR(err, MSG, "[smio_thsafe_server:zmq]",     \
            msg_err_str (err_type))

/**** Open device ****/
static int _thsafe_zmq_server_open (void *owner, void *args, void *ret)
{
    assert (owner);
    assert (args);

    DEVIO_OWNER_TYPE *self = DEVIO_EXP_OWNER(owner);
    llio_t *llio = devio_get_llio (self);

    DBE_DEBUG (DBG_MSG | DBG_LVL_TRACE, "[smio_thsafe_server:zmq] Calling thsafe_open\n");
    DBE_DEBUG (DBG_MSG | DBG_LVL_TRACE, "[smio_thsafe_server:zmq] Received message:\n");
#ifdef LOCAL_MSG_DBG
    errhand_log_print_zmq_msg (THSAFE_MSG_ZMQ(args));
#endif
    /* Message is:
     * frame null: OPEN opcode (removed by dev_io)
     * frame 0: endpoint struct (FIXME?) */
    llio_endpoint_t *llio_endpoint = (llio_endpoint_t *) THSAFE_MSG_ZMQ_FIRST_ARG(args);

    /* Call llio to actually perform the operation */
    int32_t llio_ret = llio_open (llio, llio_endpoint);
    *(int32_t *) ret = llio_ret;

    return (llio_ret < 0) ? llio_ret : (int) sizeof(int32_t);
}

disp_op_t thsafe_zmq_server_open_exp = {
    .name = THSAFE_NAME_OPEN,
    .opcode = THSAFE_OPCODE_OPEN,
    .func_fp = _thsafe_zmq_server_open,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_INT32, int32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        /* FIXME: this is unused */
        DISP_ARG_ENCODE(DISP_ATYPE_STRUCT, 0),
        DISP_ARG_END
    }
};

/**** Release device ****/
static int _thsafe_zmq_server_release (void *owner, void *args, void *ret)
{
    assert (owner);
    assert (args);

    DEVIO_OWNER_TYPE *self = DEVIO_EXP_OWNER(owner);
    llio_t *llio = devio_get_llio (self);

    DBE_DEBUG (DBG_MSG | DBG_LVL_TRACE, "[smio_thsafe_server:zmq] Calling thsafe_release\n");
    DBE_DEBUG (DBG_MSG | DBG_LVL_TRACE, "[smio_thsafe_server:zmq] Received message:\n");
#ifdef LOCAL_MSG_DBG
    errhand_log_print_zmq_msg (THSAFE_MSG_ZMQ(args));
#endif
    /* Message is:
     * frame null: OPEN opcode (removed by dev_io)
     * frame 0: endpopint struct (FIXME?) */
    llio_endpoint_t *llio_endpoint = (llio_endpoint_t *) THSAFE_MSG_ZMQ_FIRST_ARG(args);

    /* Call llio to actually perform the operation */
    int32_t llio_ret = llio_release (llio, llio_endpoint);
    *(int32_t *) ret = llio_ret;

    return (llio_ret < 0) ? llio_ret : (int) sizeof(int32_t);
}

disp_op_t thsafe_zmq_server_release_exp = {
    .name = THSAFE_NAME_RELEASE,
    .opcode = THSAFE_OPCODE_RELEASE,
    .func_fp = _thsafe_zmq_server_release,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_INT32, int32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        /* FIXME: this is unused */
        DISP_ARG_ENCODE(DISP_ATYPE_STRUCT, 0),
        DISP_ARG_END
    }
};

/**** Read data from device ****/

static int _thsafe_zmq_server_read_16 (void *owner, void *args, void *ret)
{
    assert (owner);
    assert (args);

    DEVIO_OWNER_TYPE *self = DEVIO_EXP_OWNER(owner);
    llio_t *llio = devio_get_llio (self);

    DBE_DEBUG (DBG_MSG | DBG_LVL_TRACE, "[smio_thsafe_server:zmq] Calling thsafe_read_16\n");
    uint64_t offset = *(uint64_t *) THSAFE_MSG_ZMQ_FIRST_ARG(args);

    /* Call llio to perform the actual operation */
    int32_t llio_ret = llio_read_16 (llio, offset, (uint16_t *) ret);

    return llio_ret;
}

disp_op_t thsafe_zmq_server_read_16_exp = {
    .name = THSAFE_NAME_READ_16,
    .opcode = THSAFE_OPCODE_READ_16,
    .func_fp = _thsafe_zmq_server_read_16,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT16, uint16_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT64, uint64_t),
        DISP_ARG_END
    }
};

/* TODO: Reduce code repetition */
/* Changed only the necessary parameters out of laziness */
static int _thsafe_zmq_server_read_32 (void *owner, void *args, void *ret)
{
    assert (owner);
    assert (args);

    DEVIO_OWNER_TYPE *self = DEVIO_EXP_OWNER(owner);
    llio_t *llio = devio_get_llio (self);

    DBE_DEBUG (DBG_MSG | DBG_LVL_TRACE, "[smio_thsafe_server:zmq] Calling thsafe_read_32\n");
    uint64_t offset = *(uint64_t *) THSAFE_MSG_ZMQ_FIRST_ARG(args);

    /* Call llio to perform the actual operation */
    int32_t llio_ret = llio_read_32 (llio, offset, (uint32_t *) ret);

    return llio_ret;
}

disp_op_t thsafe_zmq_server_read_32_exp = {
    .name = THSAFE_NAME_READ_32,
    .opcode = THSAFE_OPCODE_READ_32,
    .func_fp = _thsafe_zmq_server_read_32,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT64, uint64_t),
        DISP_ARG_END
    }
};

/* TODO: Reduce code repetition */
/* Changed only the necessary parameters out of laziness */
static int _thsafe_zmq_server_read_64 (void *owner, void *args, void *ret)
{
    assert (owner);
    assert (args);

    DEVIO_OWNER_TYPE *self = DEVIO_EXP_OWNER(owner);
    llio_t *llio = devio_get_llio (self);

    DBE_DEBUG (DBG_MSG | DBG_LVL_TRACE, "[smio_thsafe_server:zmq] Calling thsafe_read_64\n");
    uint64_t offset = *(uint64_t *) THSAFE_MSG_ZMQ_FIRST_ARG(args);

    /* Call llio to perform the actual operation */
    int32_t llio_ret = llio_read_64 (llio, offset, (uint64_t *) ret);

    return llio_ret;
}

disp_op_t thsafe_zmq_server_read_64_exp = {
    .name = THSAFE_NAME_READ_64,
    .opcode = THSAFE_OPCODE_READ_64,
    .func_fp = _thsafe_zmq_server_read_64,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_UINT64, uint64_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT64, uint64_t),
        DISP_ARG_END
    }
};

/**** Write data to device ****/

static int _thsafe_zmq_server_write_16 (void *owner, void *args, void *ret)
{
    assert (owner);
    assert (args);

    DEVIO_OWNER_TYPE *self = DEVIO_EXP_OWNER(owner);
    llio_t *llio = devio_get_llio (self);

    DBE_DEBUG (DBG_MSG | DBG_LVL_TRACE, "[smio_thsafe_server:zmq] Calling thsafe_write_16\n");
    uint64_t offset = *(uint64_t *) THSAFE_MSG_ZMQ_FIRST_ARG(args);
    uint16_t *data_write = (uint16_t *) THSAFE_MSG_ZMQ_NEXT_ARG(args);

    /* Call llio to perform the actual operation */
    int32_t llio_ret = llio_write_16 (llio, offset, data_write);
    *(int32_t *) ret = llio_ret;

    return sizeof (int32_t);
}

disp_op_t thsafe_zmq_server_write_16_exp = {
    .name = THSAFE_NAME_WRITE_16,
    .opcode = THSAFE_OPCODE_WRITE_16,
    .func_fp = _thsafe_zmq_server_write_16,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_INT32, int32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT64, uint64_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT16, uint16_t),
        DISP_ARG_END
    }
};

static int _thsafe_zmq_server_write_32 (void *owner, void *args, void *ret)
{
    assert (owner);
    assert (args);

    DEVIO_OWNER_TYPE *self = DEVIO_EXP_OWNER(owner);
    llio_t *llio = devio_get_llio (self);

    DBE_DEBUG (DBG_MSG | DBG_LVL_TRACE, "[smio_thsafe_server:zmq] Calling thsafe_write_32\n");
    uint64_t offset = *(uint64_t *) THSAFE_MSG_ZMQ_FIRST_ARG(args);
    uint32_t *data_write = (uint32_t *) THSAFE_MSG_ZMQ_NEXT_ARG(args);

    /* Call llio to perform the actual operation */
    int32_t llio_ret = llio_write_32 (llio, offset, data_write);
    *(int32_t *) ret = llio_ret;

    return sizeof (int32_t);
}

disp_op_t thsafe_zmq_server_write_32_exp = {
    .name = THSAFE_NAME_WRITE_32,
    .opcode = THSAFE_OPCODE_WRITE_32,
    .func_fp = _thsafe_zmq_server_write_32,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_INT32, int32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT64, uint64_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

static int _thsafe_zmq_server_write_64 (void *owner, void *args, void *ret)
{
    assert (owner);
    assert (args);

    DEVIO_OWNER_TYPE *self = DEVIO_EXP_OWNER(owner);
    llio_t *llio = devio_get_llio (self);

    DBE_DEBUG (DBG_MSG | DBG_LVL_TRACE, "[smio_thsafe_server:zmq] Calling thsafe_write_64\n");
    uint64_t offset = *(uint64_t *) THSAFE_MSG_ZMQ_FIRST_ARG(args);
    uint64_t *data_write = (uint64_t *) THSAFE_MSG_ZMQ_NEXT_ARG(args);

    /* Call llio to perform the actual operation */
    int32_t llio_ret = llio_write_64 (llio, offset, data_write);
    *(int32_t *) ret = llio_ret;

    return sizeof (int32_t);
}

disp_op_t thsafe_zmq_server_write_64_exp = {
    .name = THSAFE_NAME_WRITE_64,
    .opcode = THSAFE_OPCODE_WRITE_64,
    .func_fp = _thsafe_zmq_server_write_64,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_INT32, int32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT64, uint64_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT64, uint64_t),
        DISP_ARG_END
    }
};

/**** Read data block from device function pointer, size in bytes ****/
static int _thsafe_zmq_server_read_block (void *owner, void *args, void *ret)
{
    assert (owner);
    assert (args);
    DEVIO_OWNER_TYPE *self = DEVIO_EXP_OWNER(owner);
    llio_t *llio = devio_get_llio (self);

    DBE_DEBUG (DBG_MSG | DBG_LVL_TRACE, "[smio_thsafe_server:zmq] Calling thsafe_read_block\n");
    uint64_t offset = *(uint64_t *) THSAFE_MSG_ZMQ_FIRST_ARG(args);
    size_t read_bsize = *(size_t *) THSAFE_MSG_ZMQ_NEXT_ARG(args);

    DBE_DEBUG (DBG_MSG | DBG_LVL_TRACE, "[smio_thsafe_server:zmq] Offset = %"PRIu64", "
            "size = %zd\n", offset, read_bsize);
    /* Call llio to perform the actual operation */
    int32_t llio_ret = llio_read_block (llio, offset, read_bsize,
            (uint32_t *) ret);

    return llio_ret;
}

disp_op_t thsafe_zmq_server_read_block_exp = {
    .name = THSAFE_NAME_READ_BLOCK,
    .opcode = THSAFE_OPCODE_READ_BLOCK,
    .func_fp = _thsafe_zmq_server_read_block,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_VAR, zmq_server_data_block_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT64, uint64_t),
        DISP_ARG_ENCODE(DISP_ATYPE_STRUCT, size_t),
        DISP_ARG_END
    }
};

/**** Write data block from device function pointer, size in bytes ****/
static int _thsafe_zmq_server_write_block (void *owner, void *args, void *ret)
{
    assert (owner);
    assert (args);
    DEVIO_OWNER_TYPE *self = DEVIO_EXP_OWNER(owner);
    llio_t *llio = devio_get_llio (self);

    DBE_DEBUG (DBG_MSG | DBG_LVL_TRACE, "[smio_thsafe_server:zmq] Calling thsafe_write_block\n");
    THSAFE_MSG_ZMQ_ARG_TYPE offset_arg = THSAFE_MSG_ZMQ_POP_NEXT_ARG(args);
    uint64_t offset = *(uint64_t *) GEN_MSG_ZMQ_ARG_DATA(offset_arg);
    /* We now own the argument and must clean it after use */
    THSAFE_MSG_ZMQ_ARG_TYPE data_write_arg = THSAFE_MSG_ZMQ_POP_NEXT_ARG(args);
    uint32_t *data_write = (uint32_t *)
        ((zmq_server_data_block_t *) THSAFE_MSG_ZMQ_ARG_DATA(data_write_arg))->data;
    uint32_t data_write_size = THSAFE_MSG_ZMQ_ARG_SIZE(data_write_arg);
    DBE_DEBUG (DBG_MSG | DBG_LVL_TRACE, "[smio_thsafe_server:zmq] Arg is %u bytes\n",
            data_write_size);

    /* We must accept every block size. So, we just perform the actual LLIO
     * operation */
    int32_t llio_ret = llio_write_block (llio, offset, data_write_size,
            data_write);
    *(int32_t *) ret = llio_ret;

    /* Cleanup arguments that we now own */
    THSAFE_MSG_CLENUP_ARG(&offset_arg);
    THSAFE_MSG_CLENUP_ARG(&data_write_arg);

    return sizeof (int32_t);
}

disp_op_t thsafe_zmq_server_write_block_exp = {
    .name = THSAFE_NAME_WRITE_BLOCK,
    .opcode = THSAFE_OPCODE_WRITE_BLOCK,
    .func_fp = _thsafe_zmq_server_write_block,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_INT32, int32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT64, uint64_t),
        DISP_ARG_ENCODE(DISP_ATYPE_VAR, zmq_server_data_block_t),
        DISP_ARG_END
    }
};

/**** Read data block via DMA from device, size in bytes ****/
static int _thsafe_zmq_server_read_dma (void *owner, void *args, void *ret)
{
    (void) owner;
    (void) args;
    (void) ret;
    return -1;
}

disp_op_t thsafe_zmq_server_read_dma_exp = {
    .name = THSAFE_NAME_READ_DMA,
    .opcode = THSAFE_OPCODE_READ_DMA,
    .func_fp = _thsafe_zmq_server_read_dma,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_VAR, zmq_server_data_block_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT64, uint64_t),
        DISP_ARG_ENCODE(DISP_ATYPE_STRUCT, size_t),
        DISP_ARG_END
    }
};

/**** Write data block via DMA from device, size in bytes ****/
static int _thsafe_zmq_server_write_dma (void *owner, void *args, void *ret)
{
    (void) owner;
    (void) args;
    (void) ret;
    return -1;
}

disp_op_t thsafe_zmq_server_write_dma_exp = {
    .name = THSAFE_NAME_WRITE_DMA,
    .opcode = THSAFE_OPCODE_WRITE_DMA,
    .func_fp = _thsafe_zmq_server_write_dma,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_INT32, int32_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT64, uint64_t),
        DISP_ARG_ENCODE(DISP_ATYPE_VAR, zmq_server_data_block_t),
        DISP_ARG_END
    }
};

/**** Read device information function pointer ****/
/* int thsafe_zmq_server_read_info (void *owner, void *args, void *ret)
 *{
 *  (void) owner;
 *  (void) args;
 *  (void) ret;
 *  return NULL;
 *} */

/*************** Our constant structure **************/

const disp_op_t *smio_thsafe_zmq_server_ops [] = {
    &thsafe_zmq_server_open_exp,
    &thsafe_zmq_server_release_exp,
    &thsafe_zmq_server_read_16_exp,
    &thsafe_zmq_server_read_32_exp,
    &thsafe_zmq_server_read_64_exp,
    &thsafe_zmq_server_write_16_exp,
    &thsafe_zmq_server_write_32_exp,
    &thsafe_zmq_server_write_64_exp,
    &thsafe_zmq_server_read_block_exp,
    &thsafe_zmq_server_write_block_exp,
    &thsafe_zmq_server_read_dma_exp,
    &thsafe_zmq_server_write_dma_exp,
    NULL
};

