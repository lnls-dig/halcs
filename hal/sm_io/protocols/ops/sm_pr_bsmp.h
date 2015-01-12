/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _SM_PR_BSMP_H_
#define _SM_PR_BSMP_H_

#include <bsmp/client.h>
#include "sm_pr.h"
#include "sm_pr_err.h"

#if defined( __GNUC__)
#define PACKED __attribute__ ((packed))
#else
#error "Unsupported compiler?"
#endif

#define SMPR_PROTO_BSMP(self)                ((smpr_proto_bsmp_t *) self->proto_handler)
#define SMPR_PROTO_BSMP_CLIENT(self)         (SMPR_PROTO_BSMP(self)->client)

/* BSMP glue structure. Needed to overcome the need of global variables */
struct _smpr_proto_glue_bsmp_t {
    smio_t *parent;
};

typedef struct _smpr_proto_glue_bsmp_t smpr_proto_glue_bsmp_t;

/* For use by llio_t general structure */
extern const smpr_proto_ops_t smpr_proto_ops_bsmp;

/* Device endpoint */
struct _smpr_proto_bsmp_t {
    uint32_t base;                                      /* Core base address */
    bsmp_client_t *client;                              /* BSMP client instance */
    struct bsmp_func_info_list *funcs_list;             /* BSMP function handler */
    struct bsmp_var_info_list *vars_list;               /* BSMP variables handler */
    struct bsmp_curve_info_list *curves_list;           /* BSMP curves handler */
    /* Unused */
    struct bsmp_group_list *groups_list;                /* BSMP groups handler */
};

/* Opaque smpr_proto_bsmp structure */
typedef struct _smpr_proto_bsmp_t smpr_proto_bsmp_t;

/***************** Our methods *****************/

/* Creates a new instance of the Endpoint*/
smpr_proto_bsmp_t * smpr_proto_bsmp_new (uint32_t base);
/* Destroy an instance of the Endpoint */
smpr_err_e smpr_proto_bsmp_destroy (smpr_proto_bsmp_t **self_p);

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

#endif
