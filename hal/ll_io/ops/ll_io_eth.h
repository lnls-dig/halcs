/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _LL_IO_ETH_H_
#define _LL_IO_ETH_H_

#include "ll_io.h"

#define LLIO_ETH_HANDLER(self) ((llio_dev_eth_t *) self->dev_handler)

enum _llio_eth_type_e {
    TCP_ETH_SOCK = 0,
    UDP_ETH_SOCK = 1,
    INVALID_ETH_SOCK
};

#define TCP_ETH_SOCK_STR            "tcp"
#define UDP_ETH_SOCK_STR            "udp"
#define INVALID_ETH_SOCK_STR        "invalid"

typedef enum _llio_eth_type_e llio_eth_type_e;

/* For use by llio_t general structure */
extern const llio_ops_t llio_ops_eth;

/* Device endpoint */
struct _llio_dev_eth_t {
    llio_eth_type_e type;
    int fd;
    char *hostname;
    char *port;
};

/* Opaque llio_dev_eth structure */
typedef struct _llio_dev_eth_t llio_dev_eth_t;

/***************** Our methods *****************/

/* Creates a new instance of the Ethernet endpoint*/
llio_dev_eth_t * llio_dev_eth_new (const char *sock_type, const char *hostname,
        const char *port);
/* Destroy an instance of the Ethernet Endpoint */
llio_err_e llio_dev_eth_destroy (llio_dev_eth_t **self_p);

#endif
