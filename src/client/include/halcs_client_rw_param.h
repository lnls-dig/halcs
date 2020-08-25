/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _HALCS_CLIENT_RW_PARAM_H_
#define _HALCS_CLIENT_RW_PARAM_H_

#ifdef __cplusplus
extern "C" {
#endif

#define READ_MODE                   1
#define WRITE_MODE                  0

#define MSG_FULL_SIZE               3       /* 3 frames */
#define MSG_ERR_CODE_SIZE           1       /* 1 frame */

/* Write function name */
#define PARAM_FUNC_CLIENT_NAME_WRITE(reg)                                       \
    halcs_set ## _ ## reg

/* Read function name */
#define PARAM_FUNC_CLIENT_NAME_READ(reg)                                        \
    halcs_get ## _ ## reg

/* Write function name */
#define PARAM_FUNC_CLIENT_NAME_WRITE_MOD(module, reg)                           \
    module ## _ ## set ## _ ## reg

/* Read function name */
#define PARAM_FUNC_CLIENT_NAME_READ_MOD(module, reg)                            \
	module ## _ ## get ## _ ## reg

/* Write function declaration */
#define PARAM_FUNC_CLIENT_WRITE(reg)                                            \
    halcs_client_err_e PARAM_FUNC_CLIENT_NAME_WRITE(reg) (halcs_client_t *self,     \
            char *service, uint32_t reg)

#define PARAM_FUNC_CLIENT_WRITE_MOD(module, reg)                                        \
    halcs_client_err_e PARAM_FUNC_CLIENT_NAME_WRITE_MOD(module, reg) (halcs_client_t *self, \
            char *service, uint32_t reg)

#define PARAM_FUNC_CLIENT_WRITE_BYTE(reg)                                       \
    halcs_client_err_e PARAM_FUNC_CLIENT_NAME_WRITE(reg) (halcs_client_t *self,     \
            char *service, uint8_t reg)

#define PARAM_FUNC_CLIENT_WRITE_SIGNED(reg)                                     \
    halcs_client_err_e PARAM_FUNC_CLIENT_NAME_WRITE(reg) (halcs_client_t *self,     \
            char *service, int32_t reg)

#define PARAM_FUNC_CLIENT_WRITE_DOUBLE(reg)                                     \
    halcs_client_err_e PARAM_FUNC_CLIENT_NAME_WRITE(reg) (halcs_client_t *self,     \
            char *service, double reg)

#define PARAM_FUNC_CLIENT_WRITE2(reg, param1, param2)                           \
    halcs_client_err_e PARAM_FUNC_CLIENT_NAME_WRITE(reg) (halcs_client_t *self,     \
            char *service, uint32_t param1, uint32_t param2)

#define PARAM_FUNC_CLIENT_WRITE3(reg, param1, param2, param3)                   \
    halcs_client_err_e PARAM_FUNC_CLIENT_NAME_WRITE(reg) (halcs_client_t *self,     \
            char *service, uint32_t param1, uint32_t param2, uint32_t param3)

#define PARAM_FUNC_CLIENT_WRITE_GEN(param)                                      \
    halcs_client_err_e PARAM_FUNC_CLIENT_NAME_WRITE(param) (halcs_client_t *self,     \
            char *service, void *param, size_t size)

/* Read function declaration */
#define PARAM_FUNC_CLIENT_READ(reg)                                             \
    halcs_client_err_e PARAM_FUNC_CLIENT_NAME_READ(reg) (halcs_client_t *self,      \
            char *service, uint32_t *reg)

#define PARAM_FUNC_CLIENT_READ_MOD(module, reg)                                         \
    halcs_client_err_e PARAM_FUNC_CLIENT_NAME_READ_MOD(module, reg) (halcs_client_t *self,  \
            char *service, uint32_t *reg)

#define PARAM_FUNC_CLIENT_READ_BYTE(reg)                                        \
    halcs_client_err_e PARAM_FUNC_CLIENT_NAME_READ(reg) (halcs_client_t *self,      \
            char *service, uint8_t *reg)

#define PARAM_FUNC_CLIENT_READ_SIGNED(reg)                                      \
    halcs_client_err_e PARAM_FUNC_CLIENT_NAME_READ(reg) (halcs_client_t *self,      \
            char *service, int32_t *reg)

#define PARAM_FUNC_CLIENT_READ_DOUBLE(reg)                                      \
    halcs_client_err_e PARAM_FUNC_CLIENT_NAME_READ(reg) (halcs_client_t *self,      \
            char *service, double *reg)

#define PARAM_FUNC_CLIENT_READ2(reg, param1, param2)                            \
    halcs_client_err_e PARAM_FUNC_CLIENT_NAME_READ(reg) (halcs_client_t *self,      \
            char *service, uint32_t *param1, uint32_t *param2)

#define PARAM_FUNC_CLIENT_WRITE_READ(reg, param1, param_out)                    \
    halcs_client_err_e PARAM_FUNC_CLIENT_NAME_READ(reg) (halcs_client_t *self,      \
            char *service, uint32_t param1, uint32_t *param_out)

#define PARAM_FUNC_CLIENT_WRITE2_READ(reg, param1, param2, param_out)           \
    halcs_client_err_e PARAM_FUNC_CLIENT_NAME_READ(reg) (halcs_client_t *self,      \
            char *service, uint32_t param1, uint32_t param2, uint32_t *param_out)

#define PARAM_FUNC_CLIENT_READ_GEN(param)                                       \
    halcs_client_err_e PARAM_FUNC_CLIENT_NAME_READ(param) (halcs_client_t *self,    \
            char *service, void *param, size_t size)

/* Low-level protocol functions */
halcs_client_err_e param_client_send_gen_rw (halcs_client_t *self, char *service,
        uint32_t operation, uint32_t rw, void *param1, size_t size1,
        void *param2, size_t size2, void *param3, size_t size3);
halcs_client_err_e param_client_recv_rw (halcs_client_t *self, char *service,
    zmsg_t **report);

/* Write functions */
halcs_client_err_e param_client_write_gen (halcs_client_t *self, char *service,
        uint32_t operation, uint32_t rw, void *param1, size_t size1,
        void *param2, size_t size2, void *param3, size_t size3);
halcs_client_err_e param_client_write_raw (halcs_client_t *self, char *service,
        uint32_t operation, uint32_t param1, uint32_t param2);

halcs_client_err_e param_client_write (halcs_client_t *self, char *service,
        uint32_t operation, uint32_t param1);
halcs_client_err_e param_client_write2 (halcs_client_t *self, char *service,
        uint32_t operation, uint32_t param1, uint32_t param2);
halcs_client_err_e param_client_write3 (halcs_client_t *self, char *service,
        uint32_t operation, uint32_t param1, uint32_t param2, uint32_t param3);
halcs_client_err_e param_client_write_byte (halcs_client_t *self, char *service,
        uint32_t operation, uint8_t param);
halcs_client_err_e param_client_write_signed (halcs_client_t *self, char *service,
        uint32_t operation, int32_t param);
halcs_client_err_e param_client_write_double (halcs_client_t *self, char *service,
        uint32_t operation, double param1);
halcs_client_err_e param_client_write_double2 (halcs_client_t *self, char *service,
        uint32_t operation, double param1, double param2);

/* Read functions */
halcs_client_err_e param_client_read_gen (halcs_client_t *self, char *service,
        uint32_t operation, uint32_t rw, void *param1, size_t size1,
        void *param2, size_t size2, void *param3, size_t size3,
        void *param_out, size_t size_out);

halcs_client_err_e param_client_read (halcs_client_t *self, char *service,
        uint32_t operation, uint32_t *param_out);
halcs_client_err_e param_client_write_read (halcs_client_t *self, char *service,
        uint32_t operation, uint32_t param1, uint32_t *param_out);
halcs_client_err_e param_client_write2_read (halcs_client_t *self, char *service,
        uint32_t operation, uint32_t param1, uint32_t param2, uint32_t *param_out);
halcs_client_err_e param_client_read_byte (halcs_client_t *self, char *service,
        uint32_t operation, uint8_t *param_out);
halcs_client_err_e param_client_read_signed (halcs_client_t *self, char *service,
        uint32_t operation, int32_t *param_out);
halcs_client_err_e param_client_read_double (halcs_client_t *self, char *service,
        uint32_t operation, double *param_out);
halcs_client_err_e param_client_write_read_double (halcs_client_t *self, char *service,
        uint32_t operation, double param1, double *param_out);

/* Utility functions */
zmsg_t *param_client_recv_timeout (halcs_client_t *self, char *service);

#ifdef __cplusplus
}
#endif

#endif
