/*
 * Copyright (C) 2015  LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "halcs_server.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, DEV_IO, "[dev_io:epics]",         \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)       \
    ASSERT_HAL_ALLOC(ptr, DEV_IO, "[dev_io:epics]",                 \
            devio_err_str(DEVIO_ERR_ALLOC),                         \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                    \
    CHECK_HAL_ERR(err, DEV_IO, "[dev_io:epics]",                    \
            devio_err_str (err_type))

const board_epics_map_t board_epics_map[NUM_MAX_HALCSS+1] = {
         /* board, halcs*/
    /* 0 (INVALID)  */ {-1, -1},
    /* 1            */ {1,   0},
    /* 2            */ {1,   1},
};

/* Reverse halcs <-> EPICS board mapping */
const board_epics_rev_map_t board_epics_rev_map[NUM_MAX_SLOTS+1][NUM_MAX_HALCS_PER_SLOT] = {
     /* board, halcs */    /* halcs_id */
    /* 0, 0 (INVALID) */ {{-1},
    /* 0, 1 (INVALID) */  {-1}},
    /* 1, 0           */ {{1},
    /* 1, 1           */  {2}}
};
