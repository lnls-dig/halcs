/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _SM_CH_RFFE_H_
#define _SM_CH_RFFE_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************** Our methods *****************/

/* Creates a new instance of the SMCH RFFE */
smch_rffe_t * smch_rffe_new (smio_t *parent, const smpr_proto_ops_t *reg_ops, int verbose);
/* Destroy an instance of the SMCH RFFE */
smch_err_e smch_rffe_destroy (smch_rffe_t **self_p);

/* Write RFFE variable */
smch_err_e smch_rffe_write_var (smch_rffe_t *self, uint32_t id, uint8_t *data,
        size_t size);
/* Read RFFE variable */
smch_err_e smch_rffe_read_var (smch_rffe_t *self, uint32_t id, uint8_t *data,
        size_t size);

#ifdef __cplusplus
}
#endif

#endif
