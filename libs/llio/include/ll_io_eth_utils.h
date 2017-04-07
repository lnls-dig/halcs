/*
 * Copyright (C) 2015 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _LL_IO_ETH_UTILS_H_
#define _LL_IO_ETH_UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

enum _llio_eth_type_e {
    TCP_ETH_SOCK = 0,
    UDP_ETH_SOCK = 1,
    INVALID_ETH_SOCK,
    /* Give this enum the ability to represent CONVC_TYPE_END */
    END_ETH_SOCK = CONVC_TYPE_END
};

typedef enum _llio_eth_type_e llio_eth_type_e;

#define TCP_ETH_SOCK_STR            "tcp"
#define UDP_ETH_SOCK_STR            "udp"
#define INVALID_ETH_SOCK_STR        "invalid"

/************** Utility functions ****************/

/* Converts the llio_type enumeration into a string */
char *llio_eth_type_to_str (llio_eth_type_e type);
/* Converts a string to the llio_type enumeration. If no match if found,
 * a INVALID_DEV is returned as the llio device */
llio_eth_type_e llio_str_to_eth_type (const char *type_str);

#ifdef __cplusplus
}
#endif

#endif
