/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _MOD_DISPATCH_H_
#define _MOD_DISPATCH_H_

#ifdef __cplusplus
extern "C" {
#endif

#define SMIO_DISPATCH_END_ID       0xdeaddead

typedef struct {
    uint32_t id;
    const char *name;
    const smio_bootstrap_ops_t *bootstrap_ops;
} smio_mod_dispatch_t;

extern const smio_mod_dispatch_t smio_mod_dispatch [];

#ifdef __cplusplus
}
#endif

#endif
