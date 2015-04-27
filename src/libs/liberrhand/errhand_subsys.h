/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _ERRHAND_SUBSYS_H_
#define _ERRHAND_SUBSYS_H_

/****************** Debug subsys macros ******************/
/*
 * ERRHAND_SUBSYS variable is one-hot encoded between
 * the bits 31 and 4:
 *  bit 31  ... bit 4   bit 3 ... bit 0
 *   [1|0]      [1|0]    X         X
 */

#define ERRHAND_SUBSYS_SHIFT        4

/* Up to 28 debug subsystems (one-hot encoded) */
#define ERRHAND_SUBSYS_MAX          28
#define ERRHAND_SUBSYS_MASK_RAW     ((1 << ERRHAND_SUBSYS_MAX)-1)
#define ERRHAND_SUBSYS_MASK         (ERRHAND_SUBSYS_MASK_RAW << ERRHAND_SUBSYS_SHIFT)

#define ERRHAND_SUBSYS_GEN(val)     ((val & ERRHAND_SUBSYS_MASK_RAW) << ERRHAND_SUBSYS_SHIFT)
#define ERRHAND_SUBSYS_DEGEN(val)   ((val & ERRHAND_SUBSYS_MASK) >> ERRHAND_SUBSYS_SHIFT)

/* Debug subsys raw */
#define ERRHAND_DEV_MNGR_RAW        0x1
#define ERRHAND_DEV_IO_RAW          0x2
#define ERRHAND_LL_IO_RAW           0x4
#define ERRHAND_SM_IO_RAW           0x8
#define ERRHAND_MSG_RAW             0x10
#define ERRHAND_HAL_UTILS_RAW       0x20
#define ERRHAND_LIB_CLIENT_RAW      0x40    /* The client library shares this macros */
#define ERRHAND_SM_PR_RAW           0x80
#define ERRHAND_SM_CH_RAW           0x100

#define ERRHAND_DEV_MNGR            ERRHAND_SUBSYS_GEN(ERRHAND_DEV_MNGR_RAW)
#define ERRHAND_DEV_IO              ERRHAND_SUBSYS_GEN(ERRHAND_DEV_IO_RAW)
#define ERRHAND_LL_IO               ERRHAND_SUBSYS_GEN(ERRHAND_LL_IO_RAW)
#define ERRHAND_SM_IO               ERRHAND_SUBSYS_GEN(ERRHAND_SM_IO_RAW)
#define ERRHAND_MSG                 ERRHAND_SUBSYS_GEN(ERRHAND_MSG_RAW)
#define ERRHAND_HAL_UTILS           ERRHAND_SUBSYS_GEN(ERRHAND_HAL_UTILS_RAW)
/* The client library shares this macros */
#define ERRHAND_LIB_CLIENT          ERRHAND_SUBSYS_GEN(ERRHAND_LIB_CLIENT_RAW)
#define ERRHAND_SM_PR               ERRHAND_SUBSYS_GEN(ERRHAND_SM_PR_RAW)
#define ERRHAND_SM_CH               ERRHAND_SUBSYS_GEN(ERRHAND_SM_CH_RAW)

/* For compatibility */
#define DBG_DEV_MNGR                ERRHAND_DEV_MNGR
#define DBG_DEV_IO                  ERRHAND_DEV_IO
#define DBG_LL_IO                   ERRHAND_LL_IO
#define DBG_SM_IO                   ERRHAND_SM_IO
#define DBG_MSG                     ERRHAND_MSG
#define DBG_HAL_UTILS               ERRHAND_HAL_UTILS
#define DBG_LIB_CLIENT              ERRHAND_LIB_CLIENT
#define DBG_SM_PR                   ERRHAND_SM_PR
#define DBG_SM_CH                   ERRHAND_SM_CH

/****************** Debug levels macros ******************/
/*
 * ERRHAND_LVL variable is binary encoded between
 * the bits 3 and 1:
 *  bit 31  ... bit 4   bit 3 ... bit 1  bit 0
 *    X          X      [1|0]     [1|0]    X
 */

#define ERRHAND_LVL_SHIFT           1
#define ERRHAND_LVL_MAX             3
#define ERRHAND_LVL_MASK_RAW        ((1 << ERRHAND_LVL_MAX)-1)
#define ERRHAND_LVL_MASK            (ERRHAND_LVL_MASK_RAW << ERRHAND_LVL_SHIFT)

/* Up to 2^3 debug levels */
#define ERRHAND_LVL_GEN(val)        ((val & ERRHAND_LVL_MASK_RAW) << ERRHAND_LVL_SHIFT)
#define ERRHAND_LVL_DEGEN(val)      ((val & ERRHAND_LVL_MASK) >> ERRHAND_LVL_SHIFT)

#define ERRHAND_LVL_NUM             5

/* Debug levels raw */
#define ERRHAND_LVL_TRACE_RAW       0x1
#define ERRHAND_LVL_INFO_RAW        0x2
#define ERRHAND_LVL_WARN_RAW        0x3
#define ERRHAND_LVL_ERR_RAW         0x4
#define ERRHAND_LVL_FATAL_RAW       0x5

/* Debug levels mask'ed and shift'ed */
#define ERRHAND_LVL_TRACE           ERRHAND_LVL_GEN(ERRHAND_LVL_TRACE_RAW)
#define ERRHAND_LVL_INFO            ERRHAND_LVL_GEN(ERRHAND_LVL_INFO_RAW)
#define ERRHAND_LVL_WARN            ERRHAND_LVL_GEN(ERRHAND_LVL_WARN_RAW)
#define ERRHAND_LVL_ERR             ERRHAND_LVL_GEN(ERRHAND_LVL_ERR_RAW)
#define ERRHAND_LVL_FATAL           ERRHAND_LVL_GEN(ERRHAND_LVL_FATAL_RAW)

/* For compatibility */
#define DBG_LVL_TRACE               ERRHAND_LVL_TRACE
#define DBG_LVL_INFO                ERRHAND_LVL_INFO
#define DBG_LVL_WARN                ERRHAND_LVL_WARN
#define DBG_LVL_ERR                 ERRHAND_LVL_ERR
#define DBG_LVL_FATAL               ERRHAND_LVL_FATAL

#define ERRHAND_LVL_TRACE_STR       "TRACE"
#define ERRHAND_LVL_INFO_STR        "INFO"
#define ERRHAND_LVL_WARN_STR        "WARN"
#define ERRHAND_LVL_ERR_STR         "ERR"
#define ERRHAND_LVL_FATAL_STR       "FATAL"

extern const char *errhand_lvl_str [ERRHAND_LVL_NUM];
char *errhand_lvl_to_str (int errhand_lvl);

/****************** Debug halt macros ******************/
/*
 * ERRHAND_HALT variable is binary encoded in bit 0:
 *  bit 31  ... bit 4   bit 3 ... bit 1   bit 0
 *    X          X        X         X     [1|0]
 */

/* Debug halt */
#define ERRHAND_HALT_SHIFT          0
#define ERRHAND_HALT_MAX            1
#define ERRHAND_HALT_MASK_RAW       ((1 << ERRHAND_HALT_SHIFT)-1)
#define ERRHAND_HALT_MASK           (ERRHAND_HALT_MASK_RAW << ERRHAND_HALT_SHIFT)

/* 1 halt signal */
#define ERRHAND_HALT_GEN(val)       ((val & ERRHAND_HALT_MASK_RAW) << ERRHAND_HALT_SHIFT)
#define ERRHAND_HALT_DEGEN(val)     ((val & ERRHAND_HALT_MASK) >> ERRHAND_HALT_SHIFT)

/* Debug halt raw */
#define ERRHAND_LVL_HALT_RAW        0x1

/* Debug halt mask'ed and shift'ed */
#define ERRHAND_LVL_HALT            ERRHAND_HALT_GEN(ERRHAND_LVL_HALT_RAW)

/* For compatibility */
#define DBG_LVL_HALT                ERRHAND_LVL_HALT

#endif

