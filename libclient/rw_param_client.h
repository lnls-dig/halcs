/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _RW_PARAM_CLIENT_H_
#define _RW_PARAM_CLIENT_H_

#include <inttypes.h>
#include <mdp.h>

#include "bpm_client_codes.h"
#include "bpm_client_err.h"

#define READ_MODE                   1
#define WRITE_MODE                  0

/* Write function name */
#define PARAM_FUNC_CLIENT_NAME_WRITE(reg)                                       \
    bpm_set ## _ ## reg

/* Read function name */
#define PARAM_FUNC_CLIENT_NAME_READ(reg)                                        \
    bpm_get ## _ ## reg

/* Write function declaration */
#define PARAM_FUNC_CLIENT_WRITE(reg)                                            \
    bpm_client_err_e PARAM_FUNC_CLIENT_NAME_WRITE(reg) (bpm_client_t *self,     \
            char *service, uint32_t reg)

#define PARAM_FUNC_CLIENT_WRITE2(reg, param1, param2)                           \
    bpm_client_err_e PARAM_FUNC_CLIENT_NAME_WRITE(reg) (bpm_client_t *self,     \
            char *service, uint32_t param1, uint32_t param2)

/* Read function declaration */
#define PARAM_FUNC_CLIENT_READ(reg)                                             \
    bpm_client_err_e PARAM_FUNC_CLIENT_NAME_READ(reg) (bpm_client_t *self,      \
            char *service, uint32_t *reg)

#define PARAM_FUNC_CLIENT_READ2(reg, param1, param2)                            \
    bpm_client_err_e PARAM_FUNC_CLIENT_NAME_READ(reg) (bpm_client_t *self,      \
            char *service, uint32_t *param1, uint32_t *param2)

struct _bpm_client_t;

bpm_client_err_e param_client_send_rw (struct _bpm_client_t *self, char *service,
    uint32_t operation, uint32_t rw, uint32_t param);
bpm_client_err_e param_client_recv_rw (struct _bpm_client_t *self, char *service,
    zmsg_t **report);
bpm_client_err_e param_client_write (struct _bpm_client_t *self, char *service,
        uint32_t operation, uint32_t param);
bpm_client_err_e param_client_read (struct _bpm_client_t *self, char *service,
        uint32_t operation, uint32_t *param_out);

#endif

