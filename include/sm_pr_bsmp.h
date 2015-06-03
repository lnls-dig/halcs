/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _SM_PR_BSMP_H_
#define _SM_PR_BSMP_H_

#ifdef __cplusplus
extern "C" {
#endif

/* For use by llio_t general structure */
extern const smpr_proto_ops_t smpr_proto_ops_bsmp;

/***************** SMPR proto BSMP methods ************************************/

/* Read/Write to RFFE vars by ID */
smpr_err_e smpr_bsmp_read_var_by_id (smpr_t *self, uint32_t id, uint8_t *data,
        size_t size);
smpr_err_e smpr_bsmp_write_var_by_id (smpr_t *self, uint32_t id, uint8_t *data,
        size_t size);
/* Call RFFE functions by ID */
smpr_err_e smpr_bsmp_func_exec_by_id (smpr_t *self, uint32_t id, uint8_t *write_data,
        size_t write_size, uint8_t *read_data, size_t read_size);
/* Read RFFE curves by ID */
smpr_err_e smpr_bsmp_read_curve_by_id (smpr_t *self, uint32_t id, uint8_t *read_data,
        size_t size, size_t *valid_bytes);

#ifdef __cplusplus
}
#endif

#endif
