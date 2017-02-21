/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _SM_CH_24AA64_H_
#define _SM_CH_24AA64_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************** Our methods *****************/

/* Creates a new instance of the SMCH 24AA64 */
smch_24aa64_t * smch_24aa64_new (smio_t *parent, uint64_t base,
        const smpr_proto_ops_t *reg_ops, int verbose);
/* Destroy an instance of the SMCH 24AA64 */
smch_err_e smch_24aa64_destroy (smch_24aa64_t **self_p);

/* Read/Write to 24AA64 single bytes */
smch_err_e smch_24aa64_write_8 (smch_24aa64_t *self, uint16_t addr, const uint8_t *data);
smch_err_e smch_24aa64_read_8 (smch_24aa64_t *self, uint16_t addr, uint8_t *data);

/* Read/Write to 24AA64 blocks of data, up to 32 (size of a page) */
smch_err_e smch_24aa64_write_block (smch_24aa64_t *self, uint16_t addr, const uint32_t *data,
        size_t size);
smch_err_e smch_24aa64_read_block (smch_24aa64_t *self, uint16_t addr, uint32_t *data,
        size_t size);

#ifdef __cplusplus
}
#endif

#endif
