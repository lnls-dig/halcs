/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _SM_CH_RFFE_H_
#define _SM_CH_RFFE_H_

#include <inttypes.h>
#include <sys/types.h>

#include "sm_pr_bsmp.h"
#include "sm_ch_err.h"
#include "hal_stddef.h"

/* RFFE variables IDs. This must match the server! */
#define SMCH_RFFE_SW_ID                                 0
#define SMCH_RFFE_ATT1_ID                               1
#define SMCH_RFFE_ATT2_ID                               2
#define SMCH_RFFE_TEMP1_ID                              3
#define SMCH_RFFE_TEMP2_ID                              4
#define SMCH_RFFE_TEMP3_ID                              5
#define SMCH_RFFE_TEMP4_ID                              6
#define SMCH_RFFE_SET_POINT1_ID                         7
#define SMCH_RFFE_SET_POINT2_ID                         8
#define SMCH_RFFE_TEMP_CONTROL_ID                       9
#define SMCH_RFFE_OUTPUT1_ID                            10
#define SMCH_RFFE_OUTPUT2_ID                            11
#define SMCH_RFFE_RESET_ID                              12
#define SMCH_RFFE_REPROGRAMMING_ID                      13
#define SMCH_RFFE_DATA_ID                               14
#define SMCH_RFFE_VERSION_ID                            15
#define SMCH_RFFE_SWITCH_LEVEL_ID                       16

struct _smch_rffe_t {
    smpr_t *bsmp;                                       /* BSMP protocol object */
};

/* Opaque sm_ch_rffe structure */
typedef struct _smch_rffe_t smch_rffe_t;

/***************** Our methods *****************/

/* Creates a new instance of the SMCH RFFE */
smch_rffe_t * smch_rffe_new (smio_t *parent, int verbose);
/* Destroy an instance of the SMCH RFFE */
smch_err_e smch_rffe_destroy (smch_rffe_t **self_p);

/* Write RFFE variable */
smch_err_e smch_rffe_write_var (smch_rffe_t *self, uint32_t id, uint8_t *data,
        size_t size);
/* Read RFFE variable */
smch_err_e smch_rffe_read_var (smch_rffe_t *self, uint32_t id, uint8_t *data,
        size_t size);

#endif
