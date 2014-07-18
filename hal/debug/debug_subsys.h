/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _DEBUG_SUBSYS_H_
#define _DEBUG_SUBSYS_H_

/****************** Debug subsys macros ******************/
/*
 * DBG_SUBSYS variable is one-hot encoded between
 * the bits 31 and 4:
 *  bit 31  ... bit 4   bit 3 ... bit 0
 *   [1|0]      [1|0]    X         X
 */

#define DBG_SUBSYS_SHIFT        4

/* Up to 28 debug subsystems (one-hot encoded) */
#define DBG_SUBSYS_MAX          28
#define DBG_SUBSYS_MASK_RAW     ((1 << DBG_SUBSYS_MAX)-1)
#define DBG_SUBSYS_MASK         (DBG_SUBSYS_MASK_RAW << DBG_SUBSYS_SHIFT)

#define DBG_SUBSYS_GEN(val)    ((val & DBG_SUBSYS_MASK_RAW) << DBG_SUBSYS_SHIFT)
#define DBG_SUBSYS_DEGEN(val)  ((val & DBG_SUBSYS_MASK) >> DBG_SUBSYS_SHIFT)

/* Debug subsys raw */
#define DBG_DEV_MNGR_RAW        0x1
#define DBG_DEV_IO_RAW          0x2
#define DBG_LL_IO_RAW           0x4
#define DBG_SM_IO_RAW           0x8
#define DBG_MSG_RAW             0x10
#define DBG_HAL_UTILS_RAW       0x20
#define DBG_LIB_CLIENT_RAW      0x40    /* The client library shares this macros */

#define DBG_DEV_MNGR             DBG_SUBSYS_GEN(DBG_DEV_MNGR_RAW)
#define DBG_DEV_IO               DBG_SUBSYS_GEN(DBG_DEV_IO_RAW)
#define DBG_LL_IO                DBG_SUBSYS_GEN(DBG_LL_IO_RAW)
#define DBG_SM_IO                DBG_SUBSYS_GEN(DBG_SM_IO_RAW)
#define DBG_MSG                  DBG_SUBSYS_GEN(DBG_MSG_RAW)
#define DBG_HAL_UTILS            DBG_SUBSYS_GEN(DBG_HAL_UTILS_RAW)
/* The client library shares this macros */
#define DBG_LIB_CLIENT           DBG_SUBSYS_GEN(DBG_LIB_CLIENT_RAW)

/****************** Debug levels macros ******************/
/*
 * DBG_LVL variable is binary encoded between
 * the bits 3 and 1:
 *  bit 31  ... bit 4   bit 3 ... bit 1  bit 0
 *    X          X      [1|0]     [1|0]    X
 */

#define DBG_LVL_SHIFT          1
#define DBG_LVL_MAX            3
#define DBG_LVL_MASK_RAW       ((1 << DBG_LVL_MAX)-1)
#define DBG_LVL_MASK           (DBG_LVL_MASK_RAW << DBG_LVL_SHIFT)

/* Up to 2^3 debug levels */
#define DBG_LVL_GEN(val)       ((val & DBG_LVL_MASK_RAW) << DBG_LVL_SHIFT)
#define DBG_LVL_DEGEN(val)     ((val & DBG_LVL_MASK) >> DBG_LVL_SHIFT)

#define DBG_LVL_NUM            5

/* Debug levels raw */
#define DBG_LVL_TRACE_RAW      0x1
#define DBG_LVL_INFO_RAW       0x2
#define DBG_LVL_WARN_RAW       0x3
#define DBG_LVL_ERR_RAW        0x4
#define DBG_LVL_FATAL_RAW      0x5

/* Debug levels mask'ed and shift'ed */
#define DBG_LVL_TRACE          DBG_LVL_GEN(DBG_LVL_TRACE_RAW)
#define DBG_LVL_INFO           DBG_LVL_GEN(DBG_LVL_INFO_RAW)
#define DBG_LVL_WARN           DBG_LVL_GEN(DBG_LVL_WARN_RAW)
#define DBG_LVL_ERR            DBG_LVL_GEN(DBG_LVL_ERR_RAW)
#define DBG_LVL_FATAL          DBG_LVL_GEN(DBG_LVL_FATAL_RAW)

#define DBG_LVL_TRACE_STR      "TRACE"
#define DBG_LVL_INFO_STR       "INFO"
#define DBG_LVL_WARN_STR       "WARN"
#define DBG_LVL_ERR_STR        "ERR"
#define DBG_LVL_FATAL_STR      "FATAL"

extern const char *dbg_lvl_str [DBG_LVL_NUM];
char *dbg_lvl_to_str (int dbg_lvl);

/****************** Debug halt macros ******************/
/*
 * DBG_HALT variable is binary encoded in bit 0:
 *  bit 31  ... bit 4   bit 3 ... bit 1   bit 0
 *    X          X        X         X     [1|0]
 */

/* Debug halt */
#define DBG_HALT_SHIFT         0
#define DBG_HALT_MAX           1
#define DBG_HALT_MASK_RAW      ((1 << DBG_HALT_SHIFT)-1)
#define DBG_HALT_MASK          (DBG_HALT_MASK_RAW << DBG_HALT_SHIFT)

/* 1 halt signal */
#define DBG_HALT_GEN(val)      ((val & DBG_HALT_MASK_RAW) << DBG_HALT_SHIFT)
#define DBG_HALT_DEGEN(val)    ((val & DBG_HALT_MASK) >> DBG_HALT_SHIFT)

/* Debug halt raw */
#define DBG_LVL_HALT_RAW       0x1

/* Debug halt mask'ed and shift'ed */
#define DBG_LVL_HALT           DBG_HALT_GEN(DBG_LVL_HALT_RAW)


#endif
