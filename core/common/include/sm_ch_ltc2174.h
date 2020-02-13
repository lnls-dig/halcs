/*
 * Copyright (C) 2020 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _SM_CH_LTC2174_H_
#define _SM_CH_LTC2174_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************** Our methods *****************/

/* Creates a new instance of the SMCH LTC2174 */
smch_ltc2174_t * smch_ltc2174_new (smio_t *parent, uint64_t base,
        const smpr_proto_ops_t *reg_ops, int verbose, void *args);
/* Destroy an instance of the SMCH LTC2174 */
smch_err_e smch_ltc2174_destroy (smch_ltc2174_t **self_p);

/* Read/Write to/from LTC2174 Registers */
smch_err_e smch_ltc2174_write_8 (smch_ltc2174_t *self, uint8_t addr,
        const uint8_t *data);
smch_err_e smch_ltc2174_read_8 (smch_ltc2174_t *self, uint8_t addr,
        uint8_t *data);

/* LTC2174 PLL divider functions */
smch_err_e smch_ltc2174_set_test_patt (smch_ltc2174_t *self, uint32_t *test_patt);
smch_err_e smch_ltc2174_get_test_patt (smch_ltc2174_t *self, uint32_t *test_patt);

smch_err_e smch_ltc2174_set_test_patt_data (smch_ltc2174_t *self, uint32_t *test_patt_data);
smch_err_e smch_ltc2174_get_test_patt_data (smch_ltc2174_t *self, uint32_t *test_patt_data);

smch_err_e smch_ltc2174_reset (smch_ltc2174_t *self);

smch_err_e smch_ltc2174_set_twos_compl (smch_ltc2174_t *self, uint32_t *twos_compl);
smch_err_e smch_ltc2174_get_twos_compl (smch_ltc2174_t *self, uint32_t *twos_compl);

#ifdef __cplusplus
}
#endif

#endif
