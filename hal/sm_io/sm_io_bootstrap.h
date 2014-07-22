/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _SM_IO_BOOTSTRAP_H_
#define _SM_IO_BOOTSTRAP_H_

#include <inttypes.h>

/* #include "sm_io.h" */
/* #include "dev_io.h" */
#include "sm_io_err.h"
#include "czmq.h"

#define SMIO_FUNC_OPS_NOFAIL_WRAPPER(err, func_name, ...)   \
    do {                                                    \
        if (self->ops && self->ops->func_name) {            \
            smio_err_e local_err = self->ops->func_name (self, ##__VA_ARGS__);  \
            err = (local_err != SMIO_ERR_FUNC_NOT_IMPL) ?   \
                local_err : err;                            \
        }                                                   \
    } while (0)


#define SMIO_DISPATCH_FUNC_WRAPPER_GEN(func_name, ...)      \
    do {                                                    \
        if (smio_mod_dispatch[th_args->smio_id].bootstrap_ops && \
                smio_mod_dispatch[th_args->smio_id].bootstrap_ops->func_name) { \
            smio_mod_dispatch[th_args->smio_id].bootstrap_ops->func_name (__VA_ARGS__);  \
        }                                                   \
    } while (0)

#define SMIO_DISPATCH_FUNC_WRAPPER(func_name, ...)          \
    SMIO_DISPATCH_FUNC_WRAPPER_GEN(func_name, self, ## __VA_ARGS__)

/* Foward declarations. We don't need to include the associated header files.
 * The following should suffice */
struct _smio_t;
struct _devio_t;

/* Create instance of sm_io function pointer. This tells how to create
 * such an object */
typedef smio_err_e (*smio_init_fp)(struct _smio_t *self);
/* Destroy instance of sm)io functuion pointer. This tells how to destroy
 * such an object */
typedef smio_err_e (*smio_shutdown_fp)(struct _smio_t *self);
/* Configure SMIO default values using the libclient */
typedef smio_err_e (*smio_config_defaults_fp)(char *broker_endp, char *service,
        const char *log_file_name);

/* Main class object that every sm_io must implement */
struct _smio_bootstrap_ops_t {
    smio_init_fp init;
    smio_shutdown_fp shutdown;
    smio_config_defaults_fp config_defaults;
};

typedef struct _smio_bootstrap_ops_t smio_bootstrap_ops_t;

/* Thread boot args structure */
struct _th_boot_args_t {
    struct _devio_t *parent;        /* Pointer back to devo parent */
    uint32_t smio_id;               /* ID of the SMIO instance */
    char *broker;                   /* Endpoint to connect to broker */
    char *service;                  /* (part of) the service name to be exported */
    int verbose;                    /* Print trace information to stdout*/
    void *priv;                     /* Pointer to a private strucutre to be passed
                                        untouched by devio to the specific smio instace*/
};

typedef struct _th_boot_args_t th_boot_args_t;

/* Config thread args structure */
struct _th_config_args_t {
    uint32_t smio_id;               /* ID of the SMIO instance */
    char *broker;                   /* Endpoint to connect to broker */
    char *service;                  /* Full name of the exported service */
    char *log_file;                 /* Thread log file */
};

typedef struct _th_config_args_t th_config_args_t;

/************************************************************/
/************************ Our methods ***********************/
/************************************************************/
void smio_startup (void *args, zctx_t *ctx, void *pipe);
void smio_config_defaults (void *args, zctx_t *ctx, void *pipe);
struct _smio_t *smio_new (struct _devio_t *parent, struct _zctx_t *ctx, void *pipe,
        char *broker, char *service, int verbose);
smio_err_e smio_destroy (struct _smio_t **self_p);
smio_err_e smio_loop (struct _smio_t *self);
smio_err_e smio_loop2 (struct _smio_t *self);

#endif
