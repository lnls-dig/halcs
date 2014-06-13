/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
*/

#ifndef _SM_IO_DSP_CORE_H_
#define _SM_IO_DSP_CORE_H_

#include <inttypes.h>
#include "sm_io_err.h"

struct _smio_dsp_t {
    const uint32_t example;
};

/* Opaque class structure */
typedef struct _smio_dsp_t smio_dsp_t;

/***************** Our methods *****************/

/* Creates a new instance of the smio realizationn */
smio_dsp_t * smio_dsp_new ();
/* Destroys the smio realizationn */
smio_err_e smio_dsp_destroy (smio_dsp_t **self_p);

#endif
