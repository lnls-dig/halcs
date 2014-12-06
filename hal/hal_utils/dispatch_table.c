/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

/* This is roughly based on the work of Alessandro Rubini and others
 * "mini-rpc", located on "https://github.com/a-rubini/mini-rpc"
 *
 * In the end, I changed much of the original code and only the main
 * idea with a couple of the original structures remain */

#include <inttypes.h>

#include "hal_utils.h"
#include "dispatch_table.h"
#include "msg.h"
#include "hal_assert.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...)  \
    ASSERT_HAL_TEST(test_boolean, HAL_UTILS, "[halultis:disp_table]",           \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)                   \
    ASSERT_HAL_ALLOC(ptr, HAL_UTILS, "[halutils:disp_table]",                   \
            halutils_err_str(HALUTILS_ERR_ALLOC),                               \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                                \
    CHECK_HAL_ERR(err, HAL_UTILS, "[halutils:disp_table]",                      \
            halutils_err_str (err_type))

const disp_op_t disp_op_end = {
    .name = NULL,
    .opcode = 0,
    .func_fp = NULL,
    .retval = DISP_ARG_END,
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_END
    }
};

static halutils_err_e _disp_table_insert (disp_table_t *self, const disp_op_t* disp_op);
static void _disp_table_free_item (void *data);
static disp_op_t *_disp_table_lookup (disp_table_t *self, uint32_t key);
static halutils_err_e _disp_table_check_args_op (const disp_op_t *disp_op, void *msg);
static halutils_err_e _disp_table_check_args (disp_table_t *self, uint32_t key,
        void *args, void **ret);
static halutils_err_e _disp_table_check_gen_zmq_args (const disp_op_t *disp_op,
            zmsg_t *zmq_msg);
static halutils_err_e _disp_table_check_exp_zmq_args (const disp_op_t *disp_op,
        exp_msg_zmq_t *args);
static halutils_err_e _disp_table_check_thsafe_zmq_args (const disp_op_t *disp_op,
        zmq_server_args_t *args);
static int _disp_table_call (disp_table_t *self, uint32_t key, void *owner, void *args,
        void *ret);
static halutils_err_e _disp_table_alloc_ret (const disp_op_t *disp_op, void **ret);
static halutils_err_e _disp_table_set_ret_op (const disp_op_t * disp_op, void **ret);
static halutils_err_e _disp_table_set_ret (disp_table_t *self, uint32_t key, void **ret);
static halutils_err_e _disp_table_cleanup_args_op (const disp_op_t *disp_op);

disp_table_t *disp_table_new (void)
{
    disp_table_t *self = zmalloc (sizeof *self);
    ASSERT_ALLOC (self, err_self_alloc);
    self->table_h = zhash_new ();
    ASSERT_ALLOC (self->table_h, err_table_h_alloc);
    /* Only work for strings
    zhash_autofree (self->table_h);*/

    return self;

err_table_h_alloc:
    free (self);
err_self_alloc:
    return NULL;
}

halutils_err_e disp_table_destroy (disp_table_t **self_p)
{
    assert (self_p);

    if (*self_p) {
        disp_table_t *self = *self_p;

        zhash_destroy (&self->table_h);
        free (self);
        *self_p = NULL;
    }

    return HALUTILS_SUCCESS;
}

halutils_err_e disp_table_insert (disp_table_t *self, const disp_op_t* disp_op)
{
    return _disp_table_insert (self, disp_op);
}

halutils_err_e disp_table_insert_all (disp_table_t *self, const disp_op_t **disp_ops)
{
    assert (self);
    assert (disp_ops);

    halutils_err_e err = HALUTILS_SUCCESS;
    const disp_op_t** disp_op_it = disp_ops;
    DBE_DEBUG (DBG_HAL_UTILS | DBG_LVL_TRACE,
        "[halutils:disp_table] Preparing to insert function in dispatch table\n");

    for ( ; (*disp_op_it)->func_fp != NULL; ++disp_op_it) {
        halutils_err_e err = _disp_table_insert (self, *disp_op_it);
        ASSERT_TEST(err == HALUTILS_SUCCESS,
                "disp_table_insert_all: Could not insert function",
                err_disp_insert);
    }
    DBE_DEBUG (DBG_HAL_UTILS | DBG_LVL_TRACE,
            "[halutils:disp_table] Exiting insert_all\n");

err_disp_insert:
    return err;
}

halutils_err_e disp_table_remove (disp_table_t *self, uint32_t key)
{
    char *key_c = halutils_stringify_hex_key (key);
    ASSERT_ALLOC (key_c, err_key_c_alloc);

    /* Do a lookup first to free the return value */
    disp_op_t *disp_op = _disp_table_lookup (self, key);
    ASSERT_TEST (disp_op != NULL, "Could not find registered key",
            err_disp_op_null);

    halutils_err_e err = _disp_table_cleanup_args_op (disp_op);
    ASSERT_TEST (err == HALUTILS_SUCCESS, "Could not free registered return value",
            err_disp_op_null);

    DBE_DEBUG (DBG_HAL_UTILS | DBG_LVL_TRACE,
        "[halutils:disp_table] Removing function (key = %u) into dispatch table\n",
        key);
    /* This will trigger the free function previously registered */
    zhash_delete (self->table_h, key_c);

    free (key_c);
    return HALUTILS_SUCCESS;

err_disp_op_null:
    free (key_c);
err_key_c_alloc:
    return HALUTILS_ERR_ALLOC;
}

halutils_err_e disp_table_remove_all (disp_table_t *self)
{
    assert (self);

    zlist_t *hash_keys = zhash_keys (self->table_h);
    void * table_item = zlist_first (hash_keys);

    for ( ; table_item; table_item = zlist_next (hash_keys)) {
        zhash_delete (self->table_h, (char *) table_item);
    }

    zlist_destroy (&hash_keys);
    return HALUTILS_SUCCESS;
}

/* FIXME: unsafe iteration */
halutils_err_e disp_table_fill_desc (disp_table_t *self, disp_op_t **disp_ops,
        const disp_table_func_fp *func_fps, size_t size)
{
    assert (self);
    assert (disp_ops);
    assert (func_fps);

    halutils_err_e err = HALUTILS_SUCCESS;
    disp_op_t **disp_ops_it = disp_ops;
    const disp_table_func_fp *func_fps_it = func_fps;

    uint32_t i;
    for (i = 0; i < size; ++i, ++disp_ops_it, ++func_fps_it) {

        ASSERT_TEST (*func_fps_it != NULL, "Attempt to initialize a function "
                "description with NULL pointer", err_func_fp_null,
                HALUTILS_ERR_NULL_POINTER);
        /* Initialize funcp_fp field with the function pointer passed to it */

        /* */
        (*disp_ops_it)->func_fp = *func_fps_it;
    }

err_func_fp_null:
    return err;
}

halutils_err_e disp_table_check_args (disp_table_t *self, uint32_t key,
        void *args, void **ret)
{
    return _disp_table_check_args (self, key, args, ret);
}

halutils_err_e disp_table_cleanup_args (disp_table_t *self, uint32_t key)
{
    halutils_err_e err = HALUTILS_SUCCESS;
    disp_op_t *disp_op = _disp_table_lookup (self, key);
    ASSERT_TEST (disp_op != NULL, "Could not find registered key",
            err_disp_op_null, HALUTILS_ERR_NO_FUNC_REG);

    err = _disp_table_cleanup_args_op (disp_op);

err_disp_op_null:
    return err;
}

disp_op_t *disp_table_lookup (disp_table_t *self, uint32_t key)
{
    return _disp_table_lookup (self, key);
}

int disp_table_call (disp_table_t *self, uint32_t key, void *owner, void *args,
        void *ret)
{
    return _disp_table_call (self, key, owner, args, ret);
}

int disp_table_check_call (disp_table_t *self, uint32_t key, void *owner, void *args,
        void **ret)
{
    halutils_err_e herr = HALUTILS_SUCCESS;
    int err = -1;

    herr = _disp_table_check_args (self, key, args, ret);
    ASSERT_TEST(herr == HALUTILS_SUCCESS, "Wrong arguments received",
            err_invalid_args, -1);

    /* Received arguments are OK, and the return value "ret" was allocated if the
     * ownership is ours */

    /* Do the actual work... */
    err = _disp_table_call (self, key, owner, args, *ret);

err_invalid_args:
    return err;
}

halutils_err_e disp_table_set_ret (disp_table_t *self, uint32_t key, void **ret)
{
    return _disp_table_set_ret (self, key, ret);
}

/**** Local helper functions ****/

static halutils_err_e _disp_table_insert (disp_table_t *self, const disp_op_t *disp_op)
{
    assert (self);
    assert (disp_op);

    DBE_DEBUG (DBG_HAL_UTILS | DBG_LVL_TRACE,
            "[halutils:disp_table] Registering function \"%s\" (%p) opcode (%u) "
            "into dispatch table\n", disp_op->name, disp_op->func_fp, disp_op->opcode);

    /* FIXME: We want disp_op to be const, but we need to allocate the return value */
    halutils_err_e herr = _disp_table_alloc_ret (disp_op, (void **) &disp_op->ret);
    ASSERT_TEST (herr == HALUTILS_SUCCESS, "Return value could not be allocated",
            err_alloc_ret);

    char *key_c = halutils_stringify_hex_key (disp_op->opcode);
    ASSERT_ALLOC (key_c, err_key_c_alloc);
    int zerr = zhash_insert (self->table_h, key_c, (void *) disp_op);
    ASSERT_TEST(zerr == 0, "Could not insert item into dispatch table",
            err_insert_hash);
    /* Setup free function */
    zhash_freefn (self->table_h, key_c, _disp_table_free_item);

    free (key_c);
    return HALUTILS_SUCCESS;

err_insert_hash:
    free (key_c);
err_key_c_alloc:
    _disp_table_cleanup_args_op (disp_op);
err_alloc_ret:
    return HALUTILS_ERR_ALLOC;
}

static halutils_err_e _disp_table_alloc_ret (const disp_op_t *disp_op, void **ret)
{
    assert (disp_op);
    halutils_err_e err = HALUTILS_SUCCESS;

    if (disp_op->retval_owner == DISP_OWNER_FUNC) {
        goto err_no_ownership;
    }

    uint32_t size = DISP_GET_ASIZE(disp_op->retval);
    if (size == 0) {
        goto err_size_zero;
    }

    DBE_DEBUG (DBG_HAL_UTILS | DBG_LVL_TRACE,
            "[halutils:disp_table] Allocating %u bytes for the return value of"
            " function %s\n", DISP_GET_ASIZE(disp_op->retval), disp_op->name);

    *ret = zmalloc(DISP_GET_ASIZE(disp_op->retval));
    ASSERT_ALLOC (*ret, err_ret_alloc, HALUTILS_ERR_ALLOC);

err_size_zero:
err_ret_alloc:
err_no_ownership:
    return err;
}

static halutils_err_e _disp_table_set_ret_op (const disp_op_t * disp_op, void **ret)
{
    assert (disp_op);
    halutils_err_e err = HALUTILS_SUCCESS;

    /* Check if there is a return value registered */
    if (disp_op->retval == DISP_ARG_END) {
        *ret = NULL;
        err = HALUTILS_SUCCESS;
        goto err_exit;
    }

    DBE_DEBUG (DBG_HAL_UTILS | DBG_LVL_TRACE,
            "[halutils:disp_table] _disp_table_set_ret_op: Setting return value ...\n");

    /* FIXME: This shouldn't happen, as this type of error is caught on
     * initialization of the dispatch function */
    ASSERT_ALLOC (disp_op->ret, err_ret_alloc, HALUTILS_ERR_ALLOC);
    *ret = disp_op->ret;

    DBE_DEBUG (DBG_HAL_UTILS | DBG_LVL_TRACE,
            "[halutils:disp_table] _disp_table_set_ret_op: Return value set\n");

err_ret_alloc:
err_exit:
    return err;
}

static halutils_err_e _disp_table_set_ret (disp_table_t *self, uint32_t key, void **ret)
{
    halutils_err_e err = HALUTILS_SUCCESS;
    const disp_op_t *disp_op = _disp_table_lookup (self, key);
    ASSERT_TEST (disp_op != NULL, "Could not find registered key",
            err_disp_op_null, HALUTILS_ERR_NO_FUNC_REG);

    err = _disp_table_set_ret_op (disp_op, ret);

err_disp_op_null:
    return err;
}

static void _disp_table_free_item (void *data)
{
    (void) data;
}

static halutils_err_e _disp_table_check_args_op (const disp_op_t *disp_op, void *msg)
{
    assert (disp_op);
    halutils_err_e err = HALUTILS_ERR_INV_LESS_ARGS;

    /* Try to guess which type of message we are dealing with */
    switch (msg_guess_type (msg)) {
        case MSG_EXP_ZMQ:
            err = _disp_table_check_exp_zmq_args (disp_op, (exp_msg_zmq_t *) msg);
            break;
        case MSG_THSAFE_ZMQ:
            err = _disp_table_check_thsafe_zmq_args (disp_op, (zmq_server_args_t *) msg);
            break;
        default:
            break;
    }

    return err;
}

static halutils_err_e _disp_table_check_args (disp_table_t *self, uint32_t key,
        void *args, void **ret)
{
    halutils_err_e err = HALUTILS_SUCCESS;
    disp_op_t *disp_op = _disp_table_lookup (self, key);
    ASSERT_TEST (disp_op != NULL, "Could not find registered key",
            err_disp_op_null, HALUTILS_ERR_NO_FUNC_REG);

    /* Check arguments for consistency */
    err = _disp_table_check_args_op (disp_op, args);
    ASSERT_TEST (err == HALUTILS_SUCCESS, "Arguments received are invalid",
            err_inv_args);

    /* Point "ret" to previously allocated return value */
    err = _disp_table_set_ret_op (disp_op, ret);

err_inv_args:
err_disp_op_null:
    return err;
}

static halutils_err_e _disp_table_check_gen_zmq_args (const disp_op_t *disp_op,
        zmsg_t *zmq_msg)
{
    halutils_err_e err = HALUTILS_SUCCESS;
    GEN_MSG_ZMQ_ARG_TYPE zmq_arg = GEN_MSG_ZMQ_PEEK_FIRST(zmq_msg);

    /* Iterate over all arguments and check if they match in size with the
     * specified disp_op parameters */
    const uint32_t *args_it = disp_op->args;
    unsigned i;
    for (i = 0 ; *args_it != DISP_ARG_END; ++args_it, ++i) {
        DBE_DEBUG (DBG_HAL_UTILS | DBG_LVL_TRACE,
                "[halutils:disp_table] Checking argument #%u for function \"%s\"\n",
                i, disp_op->name);
        /* We have argument to check according to disp_op->args */
        if (zmq_arg == NULL) {
            DBE_DEBUG (DBG_HAL_UTILS | DBG_LVL_ERR,
                    "[halutils:disp_table] Missing arguments in message"
                    " received for function \"%s\"\n", disp_op->name);
            err = HALUTILS_ERR_INV_LESS_ARGS;
            goto err_inv_less_args;
        }

        /* We have received something and will check for (byte) size
         * correctness */
        if (GEN_MSG_ZMQ_ARG_SIZE(zmq_arg) != DISP_GET_ASIZE(*args_it)) {
            DBE_DEBUG (DBG_HAL_UTILS | DBG_LVL_ERR,
                    "[halutils:disp_table] Invalid size of argument"
                    " received for function \"%s\"\n", disp_op->name);
            err = HALUTILS_ERR_INV_SIZE_ARG;
            goto err_inv_size_args;
        }

        zmq_arg = GEN_MSG_ZMQ_PEEK_NEXT_ARG(zmq_msg);
    }

    /* According to disp_op->args we are done. So, check if the message received
     * has any more arguments */
    if (zmq_arg != NULL) {
        DBE_DEBUG (DBG_HAL_UTILS | DBG_LVL_ERR,
                "[halutils:disp_table] Extra arguments in message"
                " received for function \"%s\"\n", disp_op->name);
        err = HALUTILS_ERR_INV_MORE_ARGS;
        goto err_inv_more_args;
    }

    DBE_DEBUG (DBG_HAL_UTILS | DBG_LVL_TRACE,
            "[halutils:disp_table] No errors detected on the received arguments "
            "for function \"%s\"\n", disp_op->name);

err_inv_more_args:
err_inv_size_args:
err_inv_less_args:
    GEN_MSG_ZMQ_PEEK_EXIT(zmq_msg);
    return err;
}

static halutils_err_e _disp_table_check_exp_zmq_args (const disp_op_t *disp_op,
        exp_msg_zmq_t *args)
{
    return _disp_table_check_gen_zmq_args (disp_op, EXP_MSG_ZMQ(args));
}

static halutils_err_e _disp_table_check_thsafe_zmq_args (const disp_op_t *disp_op,
        zmq_server_args_t *args)
{
    return _disp_table_check_gen_zmq_args (disp_op, THSAFE_MSG_ZMQ(args));
}

static halutils_err_e _disp_table_cleanup_args_op (const disp_op_t *disp_op)
{
    assert (disp_op);
    if (disp_op->ret) {
        if (disp_op->retval_owner == DISP_OWNER_FUNC) {
            goto err_no_ownership;
        }

        free (disp_op->ret);
    }

err_no_ownership:
    return HALUTILS_SUCCESS;
}

static disp_op_t *_disp_table_lookup (disp_table_t *self, uint32_t key)
{
    disp_op_t *disp_op = NULL;
    char *key_c = halutils_stringify_hex_key (key);
    ASSERT_ALLOC (key_c, err_key_c_alloc);

    disp_op = zhash_lookup (self->table_h, key_c);
    ASSERT_TEST (disp_op != NULL, "Could not find registered function",
            err_func_p_wrapper_null);

err_func_p_wrapper_null:
    free (key_c);
err_key_c_alloc:
    return disp_op;
}

static int _disp_table_call (disp_table_t *self, uint32_t key, void *owner, void *args,
        void *ret)
{
    int err = 0;
    /* DBE_DEBUG (DBG_HAL_UTILS | DBG_LVL_TRACE,
            "[halutils:disp_table] Calling function (key = %u, addr = %p) from dispatch table\n",
            key, disp_op->func_fp); */
    /* The function pointer is never NULL */
    disp_op_t *disp_op = _disp_table_lookup (self, key);
    ASSERT_TEST (disp_op != NULL, "Could not find registered key",
            err_disp_op_null, -1);

    /* Check if there is a registered function */
    ASSERT_TEST (disp_op->func_fp != NULL, "No function registered",
            err_disp_op_func_fp_null, -1);

    /* Check if "ret" is pointing to something valid */
    ASSERT_TEST ((disp_op->retval != DISP_ARG_END && ret != NULL) ||
            (disp_op->retval == DISP_ARG_END && ret == NULL),
            "Invalid return pointer value", err_inv_ret_value_null, -1);

    err = disp_op->func_fp (owner, args, ret);

err_inv_ret_value_null:
err_disp_op_func_fp_null:
err_disp_op_null:
    return err;
}

