/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _LL_IO_PCIE_H_
#define _LL_IO_PCIE_H_

#include "ll_io.h"
#include "hw/pcie_regs.h"
#include "lib/pciDriver.h"

/* Default value for Wishbone access granularity */
/* For our FPGA firmware, Wishbone is accesses via a 64-bit BAR */
#ifndef WB_ACC
#define WB_ACC WB_QWORD_ACC
#endif

/* For use by llio_t general structure */
extern const llio_ops_t llio_ops_pcie;

/* Device endpoint */
struct _llio_dev_pcie_t {
    pd_device_t *dev;                   /* PCIe device handler */
    uint32_t *bar0;                     /* PCIe BAR0 */
    uint32_t *bar2;                     /* PCIe BAR2 */
    uint64_t *bar4;                     /* PCIe BAR4 */
};

/* Opaque llio_dev_pcie structure */
typedef struct _llio_dev_pcie_t llio_dev_pcie_t;

/***************** Our methods *****************/

/* Creates a new instance of the Endpoint*/
llio_dev_pcie_t * llio_dev_pcie_new (char *dev_entry);
/* Destroy an instance of the Endpoint */
llio_err_e llio_dev_pcie_destroy (llio_dev_pcie_t **self_p);

#endif
