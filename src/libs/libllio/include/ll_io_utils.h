/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _LL_IO_UTILS_H_
#define _LL_IO_UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Device type */
enum _llio_type_e {
    GENERIC_DEV = 0,
    PCIE_DEV = 1,
    ETH_DEV,
    INVALID_DEV,
    /* Give this enum the ability to represent CONVC_TYPE_END */
    END_DEV = CONVC_TYPE_END
};

typedef enum _llio_type_e llio_type_e;

#define GENERIC_DEV_STR             "generic"
#define PCIE_DEV_STR                "pcie"
#define ETH_DEV_STR                 "eth"
#define INVALID_DEV_STR             "invalid"

/************** Utility functions ****************/

/* Converts the llio_type enumeration into a string */
char *llio_type_to_str (llio_type_e type);
/* Converts a string to the llio_type enumeration. If no match if found,
 * a INVALID_DEV is returned as the llio device */
llio_type_e llio_str_to_type (const char *type_str);

#ifdef __cplusplus
}
#endif

#endif
