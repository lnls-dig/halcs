/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include "errhand.h"

#define ERRHAND_PRINT_PAD_FMT           "-5"

/* Our logfile */
static FILE *_errhand_logfile = NULL;

void errhand_print (const char *fmt, ...)
{
    va_list args;

    va_start (args, fmt);
    vprintf (fmt, args);
    va_end (args);
}

/* Based on CZMQ s_log () function. Available in
 * https://github.com/zeromq/czmq/blob/master/src/zsys.c */
static void _errhand_log_write (char *errhand_lvl_str, char *msg)
{
    /* Default to stdout */
    if (!_errhand_logfile) {
        _errhand_logfile = stdout;
    }

    time_t curtime = time (NULL);
    struct tm *loctime = localtime (&curtime);
    char date [20];

    strftime (date, 20, "%y-%m-%d %H:%M:%S", loctime);

    char log_text [1024];
    snprintf (log_text, 1024, "%" ERRHAND_PRINT_PAD_FMT "s: [%s] %s",
            errhand_lvl_str, date, msg);
    fprintf (_errhand_logfile, "%s", log_text);
    fflush (_errhand_logfile);
}

/* Based on CZMQ zsys_error () function. Available in
 * https://github.com/zeromq/czmq/blob/master/src/zsys.c */
void errhand_log_print (int errhand_lvl, const char *fmt, ...)
{
    va_list argptr;
    va_start (argptr, fmt);
    char *msg = errhand_lprint_vprintf (fmt, argptr);
    va_end (argptr);

    /* Convert errhand level code to string */
    char *errhand_lvl_str = errhand_lvl_to_str (errhand_lvl);
    _errhand_log_write (errhand_lvl_str, msg);
    free (msg);
    free (errhand_lvl_str);
}

void errhand_log_print_zmq_msg (zmsg_t *msg)
{
    /* Default to stdout */
    if (!_errhand_logfile) {
        _errhand_logfile = stdout;
    }

    errhand_lprint_zmq_msg (msg, _errhand_logfile);
}

void errhand_print_vec (const char *fmt, const char *data, int len)
{
    int i;
    for (i = 0; i < len; ++i)
        printf (fmt, data[i]);

    printf ("\n");
}

static void _errhand_set_log_file (FILE *log_file)
{
    _errhand_logfile = log_file;
}

void errhand_set_log_file (FILE *log_file)
{
    _errhand_set_log_file (log_file);
}

int errhand_set_log (const char *log_file_name, const char *mode)
{
    int err = -1;    /* Error */
    FILE *log_file = NULL;

    if (log_file_name) {
        if (streq(log_file_name, "stdout")) {
            log_file = stdout;
            err = 1;
        }
        else if (streq(log_file_name, "stderr")) {
            log_file = stderr;
            err = 2;
        }
        else {
            if (mode == NULL) {
                /* Be conservative if no mode is specified */
                log_file = fopen (log_file_name, "a");
            }
            else {
                log_file = fopen (log_file_name, mode);
            }

            err = 0;

            if (log_file == NULL) {
                log_file = stdout;
                err = 2;
            }
        }
    }
    else {
        /* Default to stdout */
        log_file = stdout;
    }

    _errhand_set_log_file (log_file);
    return err;
}

