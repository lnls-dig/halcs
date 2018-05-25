/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 * Parts taken from lwIP debug system
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _ERRHAND_PRINT_H_
#define _ERRHAND_PRINT_H_

#ifdef __cplusplus
extern "C" {
#endif

struct _zmsg_t;

/************** Error handling functions declarations **************/

void errhand_print (const char *fmt, ...) __attribute__((format(printf,1,2)));
void errhand_print_vec (const char *fmt, const char *data, int len);
void errhand_log_print (int dbg_lvl, const char *fmt, ...) __attribute__((format(printf,2,3)));
FILE *errhand_log_open (const char *log_file_name, const char *mode);
int errhand_log_close (FILE *log_file);
/* Set the output logfile Defaults to STDOUT */
void errhand_log_file_new (FILE *log_file);
int errhand_log_new (const char *log_file_name, const char *mode);
void errhand_log_file_destroy ();
int errhand_log_destroy ();
void errhand_log_print_zmq_msg (struct _zmsg_t *msg);
void errhand_reallog_destroy ();

/********************** Error handling macros  **********************/

/* For compatibility */
#ifndef ERRHAND_DBG
#define ERRHAND_DBG DBE_DBG
#endif

#ifndef ERRHAND_SUBSYS_ON
#define ERRHAND_SUBSYS_ON DBG_SUBSYS_ON
#endif

#ifndef ERRHAND_DBG_MSG
#define ERRHAND_DBG_MSG DBG_MSG
#endif

/* Only enable MSG if we are in trace level */
#if ((ERRHAND_SUBSYS_ON & ERRHAND_DBG_MSG) && (ERRHAND_MIN_LEVEL <= DBG_LVL_TRACE))
#define LOCAL_MSG_DBG
/* For compatibility */
#define ERRHAND_LOCAL_MSG_DBG
#endif

/* dbg has the following format:
 *    31 - 4            3 - 1           0-0
 *  ERRHAND_SUBSYS   ERRHAND_LVL   ERRHAND_SIMPLE
 */

#ifdef ERRHAND_DBG

#define ERRHAND_DEBUG(dbg, fmt, ...)                    \
    do {                                                \
        if (((dbg) & ERRHAND_SUBSYS_ON) &&              \
            (((dbg) & ERRHAND_LVL_MASK) >=              \
            ERRHAND_MIN_LEVEL)) {                       \
            errhand_log_print((dbg) & ERRHAND_LVL_SIMPLE_MASK, \
                    fmt, ## __VA_ARGS__);               \
        }                                               \
    } while(0)

#define ERRHAND_DEBUG_ARRAY(dbg, fmt, data, len)        \
    do {                                                \
        if (((dbg) & ERRHAND_SUBSYS_ON) &&              \
            (((dbg) & ERRHAND_LVL_MASK) >=              \
            ERRHAND_MIN_LEVEL)) {                       \
            errhand_print_vec(fmt, data, len);          \
        }                                               \
    } while(0)

#define ERRHAND_ERR(...)                                \
    do {                                                \
        errhand_print(fmt, ##__VA_ARGS__);              \
    } while(0)
#else

#define ERRHAND_DEBUG(dbg, fmt, ...)
#define ERRHAND_DEBUG_ARRAY(dbg, fmt, data, len)
#define ERRHAND_ERR(...)

#endif /* ERRHAND_DBG */

/* Convenient name */
#define ERRHAND_PRINT           ERRHAND_DEBUG

/* For compatibility */
#define DBE_DEBUG               ERRHAND_DEBUG
#define DBE_DEBUG_ARRAY         ERRHAND_DEBUG_ARRAY
#define DBE_ERR                 ERRHAND_ERR

#ifdef __cplusplus
}
#endif

#endif
