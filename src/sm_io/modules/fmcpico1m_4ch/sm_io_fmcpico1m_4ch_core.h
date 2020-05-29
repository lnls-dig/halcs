/*
 * Copyright (C) 2017 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
*/

#ifndef _SM_IO_FMCPICO1M_4CH_CORE_H_
#define _SM_IO_FMCPICO1M_4CH_CORE_H_

typedef struct {
    const uint32_t example;
} smio_fmcpico1m_4ch_t;

/***************** Our methods *****************/

/* Creates a new instance of the smio realization */
smio_fmcpico1m_4ch_t * smio_fmcpico1m_4ch_new (smio_t *parent);
/* Destroys the smio realizationn */
smio_err_e smio_fmcpico1m_4ch_destroy (smio_fmcpico1m_4ch_t **self_p);

#endif
