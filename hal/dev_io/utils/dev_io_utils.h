/*
 * Copyright (C) 2015 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _DEV_IO_UTILS_H_
#define _DEV_IO_UTILS_H_

#include <inttypes.h>

/* Device type */
enum _devio_type_e {
    BE_DEVIO = 0,                       /* Back-End DEVIO */
    FE_DEVIO,                           /* Front-End DEVIO */
    INVALID_DEVIO
};

typedef enum _devio_type_e devio_type_e;

#define BE_DEVIO_STR                    "be"
#define FE_DEVIO_STR                    "fe"
#define INVALID_DEVIO_STR               "invalid"

devio_type_e devio_str_to_type (const char *type_str);
char *devio_type_to_str (devio_type_e type);

#endif
