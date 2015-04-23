/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "ll_io_utils.h"
#include "errhand.h"
#include "convc.h"

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

#define LLIO_TYPE_STR_SIZE          16

/************** Utility functions ****************/

static const convc_types_t llio_dev_types_map [] ={
    {.name = GENERIC_DEV_STR,       .type = GENERIC_DEV},
    {.name = PCIE_DEV_STR,          .type = PCIE_DEV},
    {.name = ETH_DEV_STR,           .type = ETH_DEV},
    {.name = INVALID_DEV_STR,       .type = INVALID_DEV},
    {.name = CONVC_TYPE_NAME_END,   .type = CONVC_TYPE_END}        /* End marker */
};

char *llio_type_to_str (llio_type_e type)
{
    return convc_gen_type_to_str (type, llio_dev_types_map);
}

llio_type_e llio_str_to_type (const char *type_str)
{
    llio_type_e ret = convc_str_to_gen_type (type_str, llio_dev_types_map);

    return (ret == CONVC_TYPE_END)? INVALID_DEV : ret;
}

