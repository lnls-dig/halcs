/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "ll_io.h"
#include "hal_assert.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, LL_IO, "[ll_io:utils]",       \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_ALLOC(ptr, LL_IO, "[ll_io:utils]",             \
            llio_err_str(LLIO_ERR_ALLOC),                   \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                            \
    CHECK_HAL_ERR(err, LL_IO, "[ll_io:utils]",              \
            llio_err_str (err_type))

/************** Utility functions ****************/

static const llio_types_t llio_dev_types_map [] ={
    {.name = GENERIC_DEV_STR,    .type = GENERIC_DEV},
    {.name = PCIE_DEV_STR,       .type = PCIE_DEV},
    {.name = ETH_DEV_STR,        .type = ETH_DEV},
    {.name = INVALID_DEV_STR,    .type = INVALID_DEV},
    {.name = LLIO_TYPE_NAME_END, .type = LLIO_TYPE_END}        /* End marker */
};

char *llio_gen_type_to_str (int type, const llio_types_t *llio_types)
{
    /* Should be large enough for all possible debug levels */
    int size = 16;
    char *str = zmalloc (size*sizeof (char));
    ASSERT_ALLOC(str, err_alloc_str);

    /* Do a simple linear search to look for matching IDs */
    uint32_t i;
    for (i = 0; llio_types [i].type != LLIO_TYPE_END; ++i) {
        if (type == llio_types [i].type) {
            const char *type_str = llio_types [i].name;
            strncpy (str, type_str, size);
            /* Just be be safe ... */
            str [size-1] = '\0';
            break;
        }
    }

    /* No device found */
    if (llio_types [i].type == LLIO_TYPE_END) {
        free (str);
        str = NULL;
    }

err_alloc_str:
    return str;
}

char *llio_type_to_str (llio_type_e type)
{
    return llio_gen_type_to_str (type, llio_dev_types_map);
}

int llio_str_to_gen_type (const char *type_str, const llio_types_t *llio_types)
{
    assert (type_str);
    int type = LLIO_TYPE_END; /* Not found */

    /* Do a simple linear search to look for matching names */
    uint32_t i;
    for (i = 0; llio_types [i].type != LLIO_TYPE_END; ++i) {
        if (streq (type_str, llio_types [i].name)) {
            type = llio_types [i].type;
            break;
        }
    }

    return type;
}

llio_type_e llio_str_to_type (const char *type_str)
{
    llio_type_e ret = llio_str_to_gen_type (type_str, llio_dev_types_map);

    return (ret == LLIO_TYPE_END)? INVALID_DEV : ret;
}

