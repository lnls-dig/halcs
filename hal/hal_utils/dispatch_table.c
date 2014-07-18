/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include <inttypes.h>

#include "hal_utils.h"
#include "dispatch_table.h"
#include "hal_assert.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...)  \
    ASSERT_HAL_TEST(test_boolean, HAL_UTILS, "[halultis:disp_table]",\
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_ALLOC(ptr, HAL_UTILS, "[halutils:disp_table]",\
            halutils_err_str(HALUTILS_ERR_ALLOC),           \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                            \
    CHECK_HAL_ERR(err, HAL_UTILS, "[halutils:disp_table]",  \
            halutils_err_str (err_type))

static halutils_err_e _disp_table_insert (disp_table_t *self, const uint32_t key,
        disp_table_func_fp func_fp);
static void _disp_table_free_item (void *data);

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

halutils_err_e disp_table_insert (disp_table_t *self, uint32_t key,
        disp_table_func_fp func_fp)
{
    return _disp_table_insert (self, key, func_fp);
}

halutils_err_e disp_table_remove (disp_table_t *self, uint32_t key)
{
    char *key_c = halutils_stringify_hex_key (key);
    ASSERT_ALLOC (key_c, err_key_c_alloc);

    DBE_DEBUG (DBG_HAL_UTILS | DBG_LVL_TRACE,
        "[halutils:disp_table] Removing function (key = %u) into dispatch table\n",
        key);
    /* This will trigger the free function previously registered */
    zhash_delete (self->table_h, key_c);

    free (key_c);
    return HALUTILS_SUCCESS;

err_key_c_alloc:
    return HALUTILS_ERR_ALLOC;
}

void *disp_table_call (disp_table_t *self, uint32_t key, void *owner, void *args)
{
    char *key_c = halutils_stringify_hex_key (key);
    ASSERT_ALLOC (key_c, err_key_c_alloc);
    func_fp_wrapper_t *func_fp_wrapper = zhash_lookup (self->table_h, key_c);
    ASSERT_ALLOC (func_fp_wrapper, err_func_p_wrapper_null);

    free (key_c);

    /* DBE_DEBUG (DBG_HAL_UTILS | DBG_LVL_TRACE,
            "[halutils:disp_table] Calling function (key = %u, addr = %p) from dispatch table\n",
            key, func_fp_wrapper->func_fp); */
    /* The function pointer is never NULL */
    return func_fp_wrapper->func_fp (owner, args);

err_func_p_wrapper_null:
    free (key_c);
err_key_c_alloc:
    return NULL;
}

halutils_err_e disp_table_insert_all (disp_table_t *self, const disp_table_func_fp *disp_table_fp,
        const uint32_t *code_table, uint32_t size)
{
    assert (self);
    assert (disp_table_fp);
    assert (code_table);

    unsigned int i;
    halutils_err_e err = HALUTILS_SUCCESS;
    for (i = 0; i < size; ++i) {
        halutils_err_e err = _disp_table_insert (self, code_table [i],
                *disp_table_fp [i]);

        ASSERT_TEST(err == HALUTILS_SUCCESS,
                "disp_table_insert_all: Could not insert function",
                err_disp_insert);
    }

err_disp_insert:
    return err;
}

halutils_err_e disp_table_remove_all (disp_table_t *self)
{
    assert (self);

    zlist_t *hash_keys = zhash_keys (self->table_h);
    void * table_item = zlist_first (hash_keys);

    for (; table_item; table_item = zlist_next (hash_keys)) {
        zhash_delete (self->table_h, (char *) table_item);
    }

    zlist_destroy (&hash_keys);
    return HALUTILS_SUCCESS;
}

/**** Local helper functions ****/

static halutils_err_e _disp_table_insert (disp_table_t *self, uint32_t key,
        disp_table_func_fp func_fp)
{
    if (func_fp == NULL) {
        return HALUTILS_ERR_NULL_POINTER;
    }

    DBE_DEBUG (DBG_HAL_UTILS | DBG_LVL_TRACE,
            "[halutils:disp_table] Registering function (%p) opcode (%u) into dispatch table\n",
            func_fp, key);

    func_fp_wrapper_t *func_fp_wrapper = zmalloc (sizeof *func_fp_wrapper);
    ASSERT_ALLOC (func_fp_wrapper, err_func_wrapper_alloc);

    /* Initialize func_p_wrapper struct */
    func_fp_wrapper->func_fp = func_fp;

    char *key_c = halutils_stringify_hex_key (key);
    ASSERT_ALLOC (key_c, err_key_c_alloc);
    int zerr = zhash_insert (self->table_h, key_c, func_fp_wrapper);
    ASSERT_TEST(zerr == 0, "Could not insert item into dispatch table", err_insert_hash);
    /* Setup free function */
    zhash_freefn (self->table_h, key_c, _disp_table_free_item);

    free (key_c);
    return HALUTILS_SUCCESS;

err_insert_hash:
    free (key_c);
err_key_c_alloc:
    free (func_fp_wrapper);
err_func_wrapper_alloc:
    return HALUTILS_ERR_ALLOC;
}

static void _disp_table_free_item (void *data)
{
    free ((func_fp_wrapper_t *)data);
}

