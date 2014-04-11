/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _HAL_ASSERT_H_
#define _HAL_ASSERT_H_

#include "debug_print.h"

/* Generic macros for asserting in subsystems. The
 * general use case for them is to wrapp these with
 * meaningful strings for differentes subsystems */

#define ASSERT_HAL_TEST(test_boolean, debug_subsys,         \
        debug_name, err_str, err_goto_label)                \
    do {                                                    \
        if(!(test_boolean)) {                               \
            DBE_DEBUG (DBG_ ##debug_subsys | DBG_LVL_FATAL, \
                    "["debug_name"]" " %s\n", err_str);     \
            goto err_goto_label;                            \
        }                                                   \
    } while(0)

#define ASSERT_HAL_ALLOC(ptr, debug_subsys, debug_name,     \
        err_str, err_goto_label)                            \
    ASSERT_HAL_TEST(ptr!=NULL, debug_subsys, debug_name,    \
            err_str, err_goto_label)

#define CHECK_HAL_ERR(err, debug_subsys, debug_name,        \
        err_str)                                            \
    do {                                                    \
        if (err < 0) {                                      \
            DBE_DEBUG (DBG_ ##debug_subsys | DBG_LVL_ERR,   \
                    "["debug_name"]" " %s\n", err_str);     \
            return err;                                     \
        }                                                   \
    } while (0)

#endif
