/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
*/

#ifndef _SM_IO_TIM_RCV_CORE_H_
#define _SM_IO_TIM_RCV_CORE_H_

typedef struct {
    const uint32_t example;
} smio_tim_rcv_t;

/***************** Our methods *****************/

/* Creates a new instance of the smio realization */
smio_tim_rcv_t * smio_tim_rcv_new (smio_t *parent);
/* Destroys the smio realization */
smio_err_e smio_tim_rcv_destroy (smio_tim_rcv_t **self_p);

#endif
