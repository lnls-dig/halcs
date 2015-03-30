/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _SM_PR_I2C_H_
#define _SM_PR_I2C_H_

#include "sm_pr.h"
#include "sm_pr_err.h"
#include "wb_i2c_regs.h"
#include "smpr_i2c_regs.h"

#define SM_PR_I2C_MAX_TRIES                 10
#define SM_PR_I2C_USLEEP                    1000

#define SMPR_PROTO_I2C(self)                ((smpr_proto_i2c_t *) self->proto_handler)

/* I2C open arguments */
struct _i2c_proto_args_t {
    uint32_t sys_freq;          /* System clock [Hz] */
    uint32_t i2c_freq;          /* I2C clock [Hz] */
    uint32_t init_config;       /* I2C initial config register */
};

typedef struct _i2c_proto_args_t i2c_proto_args_t;

enum _i2c_mode_e {
    I2C_MODE_NORMAL = 0,        /* Single transaction mode */
    I2C_MODE_REP_START,         /* Repetitive start mode */
};

typedef enum _i2c_mode_e i2c_mode_e;

/* For use by llio_t general structure */
extern const smpr_proto_ops_t smpr_proto_ops_i2c;

/* Device endpoint */
struct _smpr_proto_i2c_t {
    uint64_t base;              /* Core base address */
    uint32_t sys_freq;          /* System clock [Hz] */
    uint32_t i2c_freq;          /* I2C clock [Hz] */
    uint32_t init_config;       /* I2C initial config register */
    i2c_mode_e mode;            /* I2C mode */
};

/* Opaque smpr_proto_i2c structure */
typedef struct _smpr_proto_i2c_t smpr_proto_i2c_t;

/***************** Our methods *****************/

/* Creates a new instance of the Endpoint*/
smpr_proto_i2c_t * smpr_proto_i2c_new (uint64_t base);
/* Destroy an instance of the Endpoint */
smpr_err_e smpr_proto_i2c_destroy (smpr_proto_i2c_t **self_p);

#endif
