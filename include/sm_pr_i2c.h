/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _SM_PR_I2C_H_
#define _SM_PR_I2C_H_

#ifdef __cplusplus
extern "C" {
#endif

/* I2C open arguments */
typedef struct {
    uint32_t sys_freq;          /* System clock [Hz] */
    uint32_t i2c_freq;          /* I2C clock [Hz] */
    uint32_t init_config;       /* I2C initial config register */
} i2c_proto_args_t;

typedef enum {
    I2C_MODE_NORMAL = 0,        /* Single transaction mode */
    I2C_MODE_REP_START,         /* Repetitive start mode */
} i2c_mode_e;

/* Creates a new instance of the proto_i2c */
smpr_i2c_t *smpr_i2c_new (uint32_t rep_start, uint32_t trans_size, uint32_t addr);
/* Destroy an instance of the i2c */
smpr_err_e smpr_i2c_destroy (smpr_i2c_t **self_p);

/* Set I2C rep_start parameter */
smpr_err_e smpr_i2c_set_rep_start (smpr_i2c_t *self, uint32_t rep_start);
/* Get I2C rep_start parameter */
uint32_t smpr_i2c_get_rep_start (smpr_i2c_t *self);

/* Set I2C trans_size parameter */
smpr_err_e smpr_i2c_set_trans_size (smpr_i2c_t *self, uint32_t trans_size);
/* Get I2C trans_size parameter */
uint32_t smpr_i2c_get_trans_size (smpr_i2c_t *self);

/* Set I2C addr parameter */
smpr_err_e smpr_i2c_set_addr (smpr_i2c_t *self, uint32_t addr);
/* Get I2C addr parameter */
uint32_t smpr_i2c_get_addr (smpr_i2c_t *self);

/* Get I2C operations */
const smpr_proto_ops_t *smpr_i2c_get_ops (smpr_i2c_t *self);

#ifdef __cplusplus
}
#endif

#endif
