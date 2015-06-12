/*
 * Copyright (C) 2015 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _DEV_IO_UTILS_H_
#define _DEV_IO_UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Device type */
enum _devio_type_e {
    BE_DEVIO = 0,                       /* Back-End DEVIO */
    FE_DEVIO,                           /* Front-End DEVIO */
    INVALID_DEVIO
};

#define BE_DEVIO_STR                    "be"
#define FE_DEVIO_STR                    "fe"
#define INVALID_DEVIO_STR               "invalid"

/* Converts the devio_type enumeration into a string */
devio_type_e devio_str_to_type (const char *type_str);
/* Converts a string to the devio_type enumeration. If no match if found,
 * a INVALID_DEV is returned as the devio device */
char *devio_type_to_str (devio_type_e type);

#ifdef __cplusplus
}
#endif

#endif
