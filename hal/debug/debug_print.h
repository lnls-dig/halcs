/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 * Parts taken from lwIP debug system
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _DEBUG_PRINT_H_
#define _DEBUG_PRINT_H_

#include <stdarg.h>
#include "debug_subsys.h"       /* This must come before "hal_opts.h" */
#include "hal_opts.h"

/************** Debug functions declarations **************/
void debug_print (const char *fmt, ...) __attribute__((format(printf,1,2)));
void debug_print_vec (const char *fmt, const char *data, int len);

/********************** Debug macros  **********************/

#if (DBG_SUBSYS_ON & DBG_MSG)
#define LOCAL_MSG_DBG
#endif

#ifdef DBE_DBG

#define dbg_print(fmt, ...) \
    debug_print(fmt, ## __VA_ARGS__)
#define dbg_print_vec(fmt, data, len) \
    debug_print_vec(fmt, data, len)
#else
#define dbg_print(fmt, ...)
#define dbg_print_vec(fmt, data, len)

#endif /* DBE_DBG */

/* dbg has the following format:
 *    31 - 4      3 - 1      0-0
 *  DBG_SUBSYS   DBG_LVL   DBG_HALT
 */

#ifdef DBE_DBG

#define DBE_DEBUG(dbg, fmt, ...)                   \
    do {                                           \
        if (((dbg) & DBG_SUBSYS_ON) &&             \
            (((dbg) & DBG_LVL_MASK) >=             \
            DBG_MIN_LEVEL)) {                      \
            dbg_print(fmt, ## __VA_ARGS__);        \
                                                   \
            if ((dbg) & DBG_LVL_HALT) {            \
                while(1);                          \
            }                                      \
        }                                          \
    } while(0)

#define DBE_DEBUG_ARRAY(dbg, fmt, data, len)       \
    do {                                           \
        if (((dbg) & DBG_SUBSYS_ON) &&             \
            (((dbg) & DBG_LVL_MASK) >=             \
            DBG_MIN_LEVEL)) {                      \
            dbg_print_vec(fmt, data, len);         \
                                                   \
            if ((dbg) & DBG_LVL_HALT)              \
                while(1);                          \
        }                                          \
    } while(0)

#define DBE_ERR(...)                               \
    do {                                           \
        dbg_print(fmt, ##__VA_ARGS__);             \
    } while(0)
#else

#define DBE_DEBUG(dbg, fmt, ...)
#define DBE_DEBUG_ARRAY(dbg, fmt, data, len)
#define DBE_ERR(...)

#endif /* DBE_DBG */

#endif
