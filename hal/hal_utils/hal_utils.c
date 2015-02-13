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
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, HAL_UTILS, "[halultis]",\
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_ALLOC(ptr, HAL_UTILS, "[halutils]",\
            halutils_err_str(HALUTILS_ERR_ALLOC),           \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                            \
    CHECK_HAL_ERR(err, HAL_UTILS, "[halutils]",  \
            halutils_err_str (err_type))

static char *_halutils_concat_strings_raw (const char *str1, const char* str2,
        const char *str3, char sep);

uint32_t num_to_str_len (uint32_t key, uint32_t base)
{
    uint32_t i = 0;
    uint32_t rem = key;

    do {
        rem /= base;
        ++i;
    } while (rem > 0);

    return i;
}

uint32_t hex_to_str_len (uint32_t key)
{
    return num_to_str_len (key, 16);
}

uint32_t dec_to_str_len (uint32_t key)
{
    return num_to_str_len (key, 10);
}

char *halutils_stringify_key (uint32_t key, uint32_t base)
{
    uint32_t key_len = num_to_str_len (key, base) + 1; /* +1 for \0 */
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

char *halutils_stringify_dec_key (uint32_t key)
{
    return halutils_stringify_key (key, 10);
}

char *halutils_stringify_hex_key (uint32_t key)
{
    return halutils_stringify_key (key, 16);
}

#define SEPARATOR_BYTES 1
/* FIXME: poorly written */
static char *_halutils_concat_strings_raw (const char *str1, const char* str2,
        const char *str3, char sep)
{
    assert (str1);
    assert (str2);

    char *str = NULL;
    if (str3 != NULL) {
        str = zmalloc (strlen (str1) + strlen (str2) + strlen (str3) +
                SEPARATOR_BYTES /* separator length */+ 1 /* \0 */);
        ASSERT_ALLOC(str, err_str3_alloc);
        sprintf (str, "%s%c%s%s", str1, sep, str2, str3);
    }
    else {
        str = zmalloc (strlen(str1) + strlen(str2) +
                SEPARATOR_BYTES /* separator length */+ 1 /* \0 */);
        ASSERT_ALLOC(str, err_str2_alloc);
        sprintf (str, "%s%c%s", str1, sep, str2);
    }

    return str;

err_str3_alloc:
err_str2_alloc:
    return NULL;
}

char *halutils_concat_strings (const char *str1, const char* str2, char sep)
{
    return _halutils_concat_strings_raw (str1, str2, NULL, sep);
}

char *halutils_concat_strings3 (const char *str1, const char* str2,
        const char* str3, char sep)
{
    return _halutils_concat_strings_raw (str1, str2, str3, sep);
}
