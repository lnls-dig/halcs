/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _SM_IO_BOOTSTRAP_H_
#define _SM_IO_BOOTSTRAP_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Create instance of sm_io function pointer. This tells how to create
 * such an object */
typedef smio_err_e (*smio_init_fp)(smio_t *self);
/* Destroy instance of sm)io functuion pointer. This tells how to destroy
 * such an object */
typedef smio_err_e (*smio_shutdown_fp)(smio_t *self);
/* Configure SMIO default values using the libclient */
typedef smio_err_e (*smio_config_defaults_fp)(char *broker_endp, char *service,
        const char *log_file_name);

/* Main class object that every sm_io must implement */
typedef struct {
    smio_init_fp init;
    smio_shutdown_fp shutdown;
    smio_config_defaults_fp config_defaults;
} smio_bootstrap_ops_t;

/* Config thread args structure */
typedef struct {
    uint32_t smio_id;               /* ID of the SMIO instance */
    uint32_t inst_id;               /* SMIO instance ID */
    char *broker;                   /* Endpoint to connect to broker */
    char *service;                  /* Full name of the exported service */
    char *log_file;                 /* Thread log file */
} th_config_args_t;

/************************************************************/
/************************ Our methods ***********************/
/************************************************************/

/* SMIO CZMQ Actor interface */
void smio_startup (zsock_t *pipe, void *args);
/* SMIO CZMQ Actor interface for configuration only */
void smio_config_defaults (zsock_t *pipe, void *args);

#ifdef __cplusplus
}
#endif

#endif
