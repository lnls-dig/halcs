/*
 * Copyright (C) 2017 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _SM_IO_CFG_H_
#define _SM_IO_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Config thread args structure */
typedef struct {
    volatile const smio_mod_dispatch_t *smio_handler;           /* SMIO table handler */
    uint32_t inst_id;                                           /* SMIO instance ID */
    char *broker;                                               /* Endpoint to connect to broker */
    char *service;                                              /* Full name of the exported service */
    char *log_file;                                             /* Thread log file */
} th_config_args_t;

/***************** Our methods *****************/

/* Creates new instance of smio_cfg */
smio_cfg_t *smio_cfg_new (th_config_args_t *config_args,
        volatile const smio_mod_dispatch_t *smio_mod_dispatch,
        zsock_t *pipe, char *service, char *inst_id_str);
/* Destroys instance of smio_cfg */
smio_err_e smio_cfg_destroy (smio_cfg_t **self_p);
/* Loop through all interface sockets executing registered callbacks on them */
smio_err_e smio_cfg_loop (smio_cfg_t *self);

/************************************************************/
/********************* Accessor methods *********************/
/************************************************************/

/* Get SMIO service */
const char *smio_cfg_get_service (smio_cfg_t *self);
/* Clone SMIO service */
char *smio_cfg_clone_service (smio_cfg_t *self);
/* Get pipe socket */
zsock_t *smio_cfg_get_pipe (smio_cfg_t *self);

#ifdef __cplusplus
}
#endif

#endif
