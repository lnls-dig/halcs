/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "errhand.h"

#define ERRHAND_PRINT_PAD_FMT           "-5"
#define ERRHAND_DATE_LENGTH             20
#define ERRHAND_TEXT_LENGTH             1024

/* TODO: We should not be using mutexes really. The correct
 * design is probably to create a socket to send/recv
 * messages from all the threads and treat them accordingly */
typedef pthread_mutex_t errhand_mutex_t;

#define ERRHAND_MUTEX_INITIALIZER   PTHREAD_MUTEX_INITIALIZER
#define ERRHAND_MUTEX_INIT(m)       pthread_mutex_init (&m, NULL);
#define ERRHAND_MUTEX_LOCK(m)       pthread_mutex_lock (&m);
#define ERRHAND_MUTEX_UNLOCK(m)     pthread_mutex_unlock (&m);
#define ERRHAND_MUTEX_DESTROY(m)    pthread_mutex_destroy (&m);

/* Our logfile */
static FILE *_errhand_logfile = NULL;
/* Mutex to guard out logfile handler */
static errhand_mutex_t _logfile_mutex = ERRHAND_MUTEX_INITIALIZER;

void errhand_print (const char *fmt, ...)
{
    va_list args;

    va_start (args, fmt);
    vprintf (fmt, args);
    va_end (args);
}

/* Based on CZMQ s_log () function. Available in
 * https://github.com/zeromq/czmq/blob/master/src/zsys.c.
 * Must be called with the mutex held */
static void _errhand_log_write (char *errhand_lvl_str, char *msg, bool verbose)
{
    ERRHAND_MUTEX_LOCK(_logfile_mutex);
    /* Default to stdout */
    if (!_errhand_logfile) {
        _errhand_logfile = stdout;
    }

    time_t curtime = time (NULL);
    struct tm *loctime = localtime (&curtime);
    char date [ERRHAND_DATE_LENGTH];
    char log_text [ERRHAND_TEXT_LENGTH];

    if (verbose) {
        strftime (date, ERRHAND_DATE_LENGTH, "%y-%m-%d %H:%M:%S", loctime);
        snprintf (log_text, ERRHAND_TEXT_LENGTH, "%" ERRHAND_PRINT_PAD_FMT "s: [%s] %s",
                errhand_lvl_str, date, msg);
    }
    else {
        snprintf (log_text, ERRHAND_TEXT_LENGTH, "%s", msg);
    }

    fprintf (_errhand_logfile, "%s", log_text);
    fflush (_errhand_logfile);
    ERRHAND_MUTEX_UNLOCK(_logfile_mutex);
}

/* Based on CZMQ zsys_error () function. Available in
 * https://github.com/zeromq/czmq/blob/master/src/zsys.c */
void errhand_log_print (int errhand_lvl, const char *fmt, ...)
{
    va_list argptr;
    va_start (argptr, fmt);
    char *msg = errhand_lprint_vprintf (fmt, argptr);
    va_end (argptr);

    /* Check if we opted for the simple print (i.e., no warning level and date) */
    bool verbose = !(ERRHAND_SIMPLE_DEGEN(errhand_lvl) & ERRHAND_LVL_SIMPLE_RAW);

    /* Convert errhand level code to string */
    char *errhand_lvl_str = errhand_lvl_to_str (errhand_lvl);
    _errhand_log_write (errhand_lvl_str, msg, verbose);
    free (msg);
    free (errhand_lvl_str);
}

void errhand_log_print_zmq_msg (zmsg_t *msg)
{
    ERRHAND_MUTEX_LOCK(_logfile_mutex);
    /* Default to stdout */
    if (!_errhand_logfile) {
        _errhand_logfile = stdout;
    }

    errhand_lprint_zmq_msg (msg, _errhand_logfile);
    ERRHAND_MUTEX_UNLOCK(_logfile_mutex);
}

void errhand_print_vec (const char *fmt, const char *data, int len)
{
    int i;
    for (i = 0; i < len; ++i)
        printf (fmt, data[i]);

    printf ("\n");
}

FILE *errhand_log_open (const char *log_file_name, const char *mode)
{
    FILE *log_file = NULL;

    if (log_file_name) {
        if (streq(log_file_name, "stdout")) {
            log_file = stdout;
        }
        else if (streq(log_file_name, "stderr")) {
            log_file = stderr;
        }
        else {
            if (mode == NULL) {
                /* Be conservative if no mode is specified */
                log_file = fopen (log_file_name, "a");
            }
            else {
                log_file = fopen (log_file_name, mode);
            }

            if (log_file == NULL) {
                log_file = stdout;
            }
        }
    }
    else {
        /* Default to stdout */
        log_file = stdout;
    }

    return log_file;
}

int errhand_log_close (FILE *log_file)
{
    return fclose (log_file);
}

static void _errhand_log_file_new (FILE *log_file)
{
    _errhand_logfile = log_file;
}

void errhand_log_file_new (FILE *log_file)
{
    _errhand_log_file_new (log_file);
}

static void _errhand_log_file_destroy ()
{
    _errhand_logfile = NULL;
}

void errhand_log_file_destroy ()
{
    _errhand_log_file_destroy ();
}

int errhand_log_new (const char *log_file_name, const char *mode)
{
    ERRHAND_MUTEX_LOCK(_logfile_mutex);
    int err = 0;
    FILE *log_file = errhand_log_open (log_file_name, mode);

    _errhand_log_file_new (log_file);
    ERRHAND_MUTEX_UNLOCK(_logfile_mutex);
    return err;
}

int errhand_log_destroy ()
{
    ERRHAND_MUTEX_LOCK(_logfile_mutex);
    int err = -1;

    err = errhand_log_close (_errhand_logfile);
    _errhand_log_file_destroy();
    ERRHAND_MUTEX_UNLOCK(_logfile_mutex);
    return err;
}
