/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
*/

#ifndef _SM_IO_FMC130M_4CH_CORE_H_
#define _SM_IO_FMC130M_4CH_CORE_H_

#include <inttypes.h>
#include "sm_io_err.h"

struct _smio_fmc130m_4ch_t {
    uint32_t dummy;                 /* Just a dummy value to illustrate the idea */
};

/* Opaque class structure */
typedef struct _smio_fmc130m_4ch_t smio_fmc130m_4ch_t;

/***************** Our methods *****************/

/* Creates a new instance of the smio realizationn */
smio_fmc130m_4ch_t * smio_fmc130m_4ch_new (uint32_t dummy);
/* Destroys the smio realizationn */
smio_err_e smio_fmc130m_4ch_destroy (smio_fmc130m_4ch_t **self_p);

#endif
