/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _LL_IO_UTILS_H_
#define _LL_IO_UTILS_H_

#include <inttypes.h>

#define LLIO_TYPE_END               0xFFFF
#define LLIO_TYPE_NAME_END          ""

/* Device type */
enum _llio_type_e {
    GENERIC_DEV = 0,
    PCIE_DEV = 1,
    ETH_DEV,
    INVALID_DEV
};

typedef enum _llio_type_e llio_type_e;

struct _llio_types_t {
    int type;
    char *name;
};

typedef struct _llio_types_t llio_types_t;

#define GENERIC_DEV_STR             "generic"
#define PCIE_DEV_STR                "pcie"
#define ETH_DEV_STR                 "eth"
#define INVALID_DEV_STR             "invalid"

/************** Utility functions ****************/

/* Generic mapping functions */

/* Converts the LLIO type ID into a string */
char *llio_gen_type_to_str (int type, const llio_types_t *llio_types);
/* Converts a string to an LLIO type ID. If no match if found,
 * a INVALID_DEV is returned */
int llio_str_to_gen_type (const char *type_str,
        const llio_types_t *llio_types);

/* Converts the llio_type enumeration into a string */
char *llio_type_to_str (llio_type_e type);
/* Converts a string to the llio_type enumeration. If no match if found,
 * a INVALID_DEV is returned as the llio device */
llio_type_e llio_str_to_type (const char *type_str);

#endif
