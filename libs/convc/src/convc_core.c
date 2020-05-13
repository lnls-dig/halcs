/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "convc.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, HAL_UTILS, "[convc]",             \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)       \
    ASSERT_HAL_ALLOC(ptr, HAL_UTILS, "[convc]",                     \
            convc_err_str(CONVC_ERR_ALLOC),                         \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                    \
    CHECK_HAL_ERR(err, HAL_UTILS, "[convc]",                        \
            convc_err_str (err_type))

#define CONVC_TYPE_STR_SIZE          16

/************** Utility functions ****************/

char *convc_gen_type_to_str (int type, const convc_types_t *convc_types)
{
    /* Should be large enough for all possible debug levels */
    size_t size = CONVC_TYPE_STR_SIZE;
    char *str = zmalloc (size*sizeof (char));
    ASSERT_ALLOC(str, err_alloc_str);

    /* Do a simple linear search to look for matching IDs */
    uint32_t i;
    for (i = 0; convc_types [i].type != CONVC_TYPE_END; ++i) {
        if (type == convc_types [i].type) {
            const char *type_str = convc_types [i].name;

            int errs = snprintf (str, size, "%s", type_str);
            ASSERT_TEST (errs >= 0,
                    "[convc] Could not clone string. Enconding error?\n",
                    err_copy_str);
            /* This shouldn't happen. Only when the number of characters written is
             * less than the whole buffer, it is guaranteed that the string was
             * written successfully */
            ASSERT_TEST ((size_t) errs < size,
                    "[convc] Cloned string was truncated\n", err_trunc_str);

            break;
        }
    }

    /* No device found */
    if (convc_types [i].type == CONVC_TYPE_END) {
        free (str);
        str = NULL;
    }

    return str;

    /* Fail miserably for now ... */
err_trunc_str:
err_copy_str:
    free (str);
    str = NULL;
err_alloc_str:
    return NULL;
}

int convc_str_to_gen_type (const char *type_str, const convc_types_t *convc_types)
{
    assert (type_str);
    int type = CONVC_TYPE_END; /* Not found */

    /* Do a simple linear search to look for matching names */
    uint32_t i;
    for (i = 0; convc_types [i].type != CONVC_TYPE_END; ++i) {
        if (streq (type_str, convc_types [i].name)) {
            type = convc_types [i].type;
            break;
        }
    }

    return type;
}

