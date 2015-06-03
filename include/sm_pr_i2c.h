/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
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

/* For use by llio_t general structure */
extern const smpr_proto_ops_t smpr_proto_ops_i2c;

#ifdef __cplusplus
}
#endif

#endif
