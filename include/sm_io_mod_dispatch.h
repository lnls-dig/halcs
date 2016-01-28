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

/*
 * WARNING! Using SMIO_MOD_DECLARE requires .smio_mod_dispatch section in linker script
 * .smio_mod_dispatch : ALIGN(4)
 * {
 * 	 _smio_mod_dispatch = .;
 * 	 KEEP(*(.smio_mod_dispatch))
 *   _esmio_mod_dispatch = .;
 * }
*/

extern const smio_mod_dispatch_t _smio_mod_dispatch;
extern const smio_mod_dispatch_t _esmio_mod_dispatch;

#define SMIO_MOD_DECLARE(mod_id, mod_name, mod_bootstrap_ops)                   \
    const smio_mod_dispatch_t __attribute__ ((section (".smio_mod_dispatch")))  \
        smio_mod_ ## mod_id ## _ ## mod_name = {                                \
            .id = mod_id,                                                       \
            .name = mod_name,                                                   \
            .bootstrap_ops = &mod_bootstrap_ops                                 \
        };

#ifdef __cplusplus
}
#endif

#endif
