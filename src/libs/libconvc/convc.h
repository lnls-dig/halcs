/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _CONVC_H_
#define _CONVC_H_

#include <inttypes.h>

#define CONVC_TYPE_END               0xFFFF
#define CONVC_TYPE_NAME_END          ""

struct _convc_types_t {
    int type;
    char *name;
};

typedef struct _convc_types_t convc_types_t;

/************** Utility functions ****************/

/* Generic mapping functions */

/* Converts the CONVC type ID into a string */
char *convc_gen_type_to_str (int type, const convc_types_t *convc_types);
/* Converts a string to an CONVC type ID. If no match if found,
 * a INVALID_DEV is returned */
int convc_str_to_gen_type (const char *type_str,
        const convc_types_t *convc_types);

#endif
