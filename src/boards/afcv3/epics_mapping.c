/*
 * Copyright (C) 2015  LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "bpm_server.h"

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

/* Reverse bpm-sw <-> EPICS board mapping */
const board_epics_rev_map_t board_epics_rev_map[NUM_MAX_SLOTS+1][NUM_MAX_BPM_PER_SLOT] = {
     /* board, bpm */    /* bpm_id */
    /* 0, 0 (INVALID) */ {{-1},
    /* 0, 1 (INVALID) */  {-1}},
    /* 1, 0           */ {{0},
    /* 1, 1           */  {1}},
    /* 2, 0           */ {{2},
    /* 2, 1           */  {3}},
    /* 3, 0           */ {{4},
    /* 3, 1           */  {5}},
    /* 4, 0           */ {{6},
    /* 4, 1           */  {7}},
    /* 5, 0           */ {{8},
    /* 5, 1           */  {9}},
    /* 6, 0           */ {{10},
    /* 6, 1           */  {11}},
    /* 7, 0           */ {{12},
    /* 7, 1           */  {13}},
    /* 8, 0           */ {{14},
    /* 8, 1           */  {15}},
    /* 9, 0           */ {{16},
    /* 9, 1           */  {17}},
    /* 10, 0          */ {{18},
    /* 10, 1          */  {19}},
    /* 11, 0          */ {{20},
    /* 11, 1          */  {21}},
    /* 12, 0          */ {{22},
    /* 12, 1          */  {23}}
};

const board_epics_opts_t board_epics_opts[NUM_MAX_SLOTS+1][NUM_MAX_BPM_PER_SLOT] = {
     /* board, bpm */    /* bpm_id */
    /* 0, 0 (INVALID) */ {{-1},
    /* 0, 1 (INVALID) */  {-1}},
    /* 1, 0           */ {{20000 + 0},
    /* 1, 1           */  {20000 + 1}},
    /* 2, 0           */ {{20000 + 2},
    /* 2, 1           */  {20000 + 3}},
    /* 3, 0           */ {{20000 + 4},
    /* 3, 1           */  {20000 + 5}},
    /* 4, 0           */ {{20000 + 6},
    /* 4, 1           */  {20000 + 7}},
    /* 5, 0           */ {{20000 + 8},
    /* 5, 1           */  {20000 + 9}},
    /* 6, 0           */ {{20000 + 10},
    /* 6, 1           */  {20000 + 11}},
    /* 7, 0           */ {{20000 + 12},
    /* 7, 1           */  {20000 + 13}},
    /* 8, 0           */ {{20000 + 14},
    /* 8, 1           */  {20000 + 15}},
    /* 9, 0           */ {{20000 + 16},
    /* 9, 1           */  {20000 + 17}},
    /* 10, 0          */ {{20000 + 18},
    /* 10, 1          */  {20000 + 19}},
    /* 11, 0          */ {{20000 + 20},
    /* 11, 1          */  {20000 + 21}},
    /* 12, 0          */ {{20000 + 22},
    /* 12, 1          */  {20000 + 23}}
};
