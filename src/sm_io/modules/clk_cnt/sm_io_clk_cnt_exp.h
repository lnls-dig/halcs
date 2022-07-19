/*
 * Copyright (C) 2022 LNLS (www.lnls.br)
 * Author: Érico Nogueira <erico.rolim@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _CLK_CNT_H_
#define _CLK_CNT_H_

/* Known modules IDs (from SDB records defined in FPGA) */
#define CLK_CNT_SDB_DEVID       0x9d1db297
#define CLK_CNT_SDB_NAME        "CLK_CNT"

extern const smio_bootstrap_ops_t clk_cnt_bootstrap_ops;

#endif

