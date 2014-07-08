/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include <czmq.h>

char *local_vprintf (const char *format, va_list argptr)
{
    return zsys_vprintf (format, argptr);
}

