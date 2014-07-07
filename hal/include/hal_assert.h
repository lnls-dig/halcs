/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _HAL_ASSERT_H_
#define _HAL_ASSERT_H_

#include "debug_print.h"
#include "hal_varg.h"

/* TODO: find a way avoid modifying the "global" err variable */
#define SET_ERR_VAR(err_code)  WHENNOT(ISEMPTY(err_code))(  \
            err = err_code)

/* Generic macros for asserting in subsystems. The
 * general use case for them is to wrapp these with
 * meaningful strings for differentes subsystems */

#define ASSERT_HAL_TEST(test_boolean, debug_subsys,         \
        debug_name, err_str, err_goto_label, /* err_code*/ ...) \
    do {                                                    \
        if(!(test_boolean)) {                               \
            DBE_DEBUG (DBG_ ##debug_subsys | DBG_LVL_FATAL, \
                    "["debug_name"]" " %s\n", err_str);     \
            SET_ERR_VAR(/* err_code*/ __VA_ARGS__);         \
            goto err_goto_label;                            \
        }                                                   \
    } while(0)

#define ASSERT_HAL_ALLOC(ptr, debug_subsys, debug_name,     \
        err_str, err_goto_label, /* err_code*/ ...)         \
    ASSERT_HAL_TEST(ptr!=NULL, debug_subsys, debug_name,    \
            err_str, err_goto_label, /* err_code*/ __VA_ARGS__)

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
