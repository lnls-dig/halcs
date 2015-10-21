/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _LL_IO_PCIE_H_
#define _LL_IO_PCIE_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Default value for Wishbone access granularity */
/* For our FPGA firmware, Wishbone is accesses via a 64-bit BAR */
#ifndef WB_ACC
#define WB_ACC WB_QWORD_ACC
#endif

/* For use by llio_t general structure */
extern const llio_ops_t llio_ops_pcie;

#ifdef __cplusplus
}
#endif

#endif
