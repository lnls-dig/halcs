/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include "sm_io_mod_dispatch.h"
/* Include all available module we can handle */
#include "sm_io_fmc130m_4ch_exp.h"

/* Table of all known modules we can handle */
const smio_mod_dispatch_t smio_mod_dispatch[MOD_DISPATCH_END] = {
    [0] = { .id = FMC130M_4CH_SDB_DEVID,
            .name = FMC130M_4CH_SDB_NAME,
            .bootstrap_ops = &fmc130m_4ch_bootstrap_ops
    }
};
