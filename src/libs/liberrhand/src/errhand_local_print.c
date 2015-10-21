/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include "errhand.h"

char *errhand_lprint_vprintf (const char *format, va_list argptr)
{
    return zsys_vprintf (format, argptr);
}

void errhand_lprint_zmq_msg (zmsg_t *msg, FILE *file)
{
    zmsg_fprint (msg, file);
    fprintf (file, "--------------------------------------\n");
    fflush (file);
}

