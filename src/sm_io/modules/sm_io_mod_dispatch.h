/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _MOD_DISPATCH_H_
#define _MOD_DISPATCH_H_

#include <inttypes.h>

/* #include "sm_io_bootstrap.h" */

/* Number of known specific modules that we can handle */
#define MOD_DISPATCH_END 6

struct _smio_bootstrap_ops_t;

struct _smio_mod_dispatch_t {
    uint32_t id;
    const char *name;
    const struct _smio_bootstrap_ops_t *bootstrap_ops;
};

typedef struct _smio_mod_dispatch_t smio_mod_dispatch_t;

extern const smio_mod_dispatch_t
    smio_mod_dispatch[MOD_DISPATCH_END];

#endif
