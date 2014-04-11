/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include <stdio.h>

#include "hal_utils.h"
#include "hal_utils_err.h"
#include "hal_assert.h"
#include "czmq.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label)  \
    ASSERT_HAL_TEST(test_boolean, HAL_UTILS, "[halultis]",\
            err_str, err_goto_label)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label)                   \
    ASSERT_HAL_ALLOC(ptr, HAL_UTILS, "[halutils]",\
            halutils_err_str(HALUTILS_ERR_ALLOC),           \
            err_goto_label)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                            \
    CHECK_HAL_ERR(err, HAL_UTILS, "[halutils]",  \
            halutils_err_str (err_type))

uint32_t hex_to_str_len (uint32_t key)
{
    uint32_t i = 0;

    do {
        key >>= 4;
        ++i;
    } while (key > 0);

    return i;
}

char *halutils_stringify_key (uint32_t key)
{
    uint32_t key_len = hex_to_str_len (key) + 1; /* +1 for \0 */
    char *key_c = zmalloc (key_len * sizeof (char));
    ASSERT_ALLOC (key_c, err_key_c_alloc);

    snprintf(key_c, key_len, "%x", key);
    /* DBE_DEBUG (DBG_HAL_UTILS | DBG_LVL_TRACE,
            "[halutils:stringify_key] key = %s, key_len = %u\n",
            key_c, key_len); */

    return key_c;

err_key_c_alloc:
    return NULL;
}


