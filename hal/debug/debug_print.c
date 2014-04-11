/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include <stdio.h>
#include "debug_print.h"

void debug_print (const char *fmt, ...)
{
    va_list args;

    va_start (args, fmt);
    vprintf (fmt, args);
    va_end (args);
}

void debug_print_vec (const char *fmt, const char *data, int len)
{
    int i;
    for (i = 0; i < len; ++i)
        printf (fmt, data[i]);

    printf ("\n");
}
