/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _SM_CH_ISLA216P_H_
#define _SM_CH_ISLA216P_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************** Our methods *****************/

/* Creates a new instance of the SMCH ISLA216P */
smch_isla216p_t * smch_isla216p_new (smio_t *parent, uint64_t base,
        const smpr_proto_ops_t *reg_ops, int verbose);
/* Destroy an instance of the SMCH ISLA216P */
smch_err_e smch_isla216p_destroy (smch_isla216p_t **self_p);

/* Read/Write to/from ISLA216P Registers */
smch_err_e smch_isla216p_write_8 (smch_isla216p_t *self, uint8_t addr,
        const uint8_t *data);
smch_err_e smch_isla216p_read_8 (smch_isla216p_t *self, uint8_t addr,
        uint8_t *data);

/* ISLA216P Test functions */
smch_err_e smch_isla216p_set_test_mode (smch_isla216p_t *self, uint8_t mode);

/* Chid ID */
smch_err_e smch_isla216p_get_chipid (smch_isla216p_t *self, uint8_t *chipid);
/* CHIP Version */
smch_err_e smch_isla216p_get_chipver (smch_isla216p_t *self, uint8_t *chipver);
/* Reset ISLA216P */
smch_err_e smch_isla216p_set_rst (smch_isla216p_t *self, uint8_t rst_operation);
/* Set port config */
smch_err_e smch_isla216p_set_portconfig (smch_isla216p_t *self, uint8_t config);
/* Read temperature code */
smch_err_e smch_isla216p_get_temp (smch_isla216p_t *self, uint16_t *temp);

#ifdef __cplusplus
}
#endif

#endif
