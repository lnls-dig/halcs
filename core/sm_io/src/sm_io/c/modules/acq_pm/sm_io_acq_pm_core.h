/*
 * Copyright (C) 2017 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
*/

#ifndef _SM_IO_ACQ_PM_CORE_H_
#define _SM_IO_ACQ_PM_CORE_H_

typedef struct {
    uint32_t dummy;
} smio_acq_pm_t;

/***************** Our methods *****************/

/* Creates a new instance of the smio realization */
smio_acq_pm_t * smio_acq_pm_new (smio_t *parent);
/* Destroys the smio realizationn */
smio_err_e smio_acq_pm_destroy (smio_acq_pm_t **self_p);

#endif
