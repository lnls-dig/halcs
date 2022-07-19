/*
 * Copyright (C) 2022 LNLS (www.lnls.br)
 * Author: Érico Nogueira <erico.rolim@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
*/

#ifndef _SM_IO_CLK_CNT_CORE_H_
#define _SM_IO_CLK_CNT_CORE_H_

typedef struct {
    const uint32_t example;
} smio_clk_cnt_t;

/***************** Our methods *****************/

/* Creates a new instance of the smio realization */
smio_clk_cnt_t * smio_clk_cnt_new (smio_t *parent);
/* Destroys the smio realization */
smio_err_e smio_clk_cnt_destroy (smio_clk_cnt_t **self_p);

#endif
