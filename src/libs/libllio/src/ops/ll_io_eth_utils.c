/*
 * Copyright (C) 2015 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include "ll_io.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, LL_IO, "[ll_io:utils]",           \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)       \
    ASSERT_HAL_ALLOC(ptr, LL_IO, "[ll_io:utils]",                   \
            llio_err_str(LLIO_ERR_ALLOC),                           \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                    \
    CHECK_HAL_ERR(err, LL_IO, "[ll_io:utils]",                      \
            llio_err_str (err_type))

#define LLIO_TYPE_STR_SIZE          16

/************** Utility functions ****************/

const convc_types_t llio_eth_types_map [] ={
    {.name = TCP_ETH_SOCK_STR,      .type = TCP_ETH_SOCK},
    {.name = UDP_ETH_SOCK_STR,      .type = UDP_ETH_SOCK},
    {.name = INVALID_ETH_SOCK_STR,  .type = INVALID_ETH_SOCK},
    {.name = CONVC_TYPE_NAME_END,   .type = CONVC_TYPE_END}        /* End marker */
};

char *llio_eth_type_to_str (llio_eth_type_e type)
{
    return convc_gen_type_to_str (type, llio_eth_types_map);
}

llio_eth_type_e llio_str_to_eth_type (const char *type_str)
{
    llio_eth_type_e ret = convc_str_to_gen_type (type_str, llio_eth_types_map);

    return (ret == CONVC_TYPE_END)? INVALID_ETH_SOCK: ret;
}

