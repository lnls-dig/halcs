/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _SM_IO_H_
#define _SM_IO_H_

#ifdef __cplusplus
extern "C" {
#endif

/* SMIO sockets IDs */
#define SMIO_PIPE_MGMT_SOCK         0
#define SMIO_MLM_SOCK               1
#define SMIO_END_SOCK               2
#define SMIO_SOCKS_NUM              SMIO_END_SOCK

#define SMIO_FUNC_OPS_NOFAIL_WRAPPER(err, func_name, ...)   \
    ({                                                      \
        smio_err_e local_err = SMIO_ERR_FUNC_NOT_IMPL;      \
        const smio_ops_t *smio_ops = smio_get_ops (self);   \
        if (smio_ops && smio_ops->func_name) {              \
            local_err = smio_ops->func_name (self, ##__VA_ARGS__);  \
        }                                                   \
        (local_err != SMIO_ERR_FUNC_NOT_IMPL) ? local_err : SMIO_SUCCESS; \
    })

#define SMIO_DISPATCH_FUNC_WRAPPER_GEN(func_name, smio_mod_handler, ...)      \
    ({                                                      \
        smio_err_e local_err = SMIO_ERR_FUNC_NOT_IMPL;      \
        if (smio_mod_handler->bootstrap_ops && \
                smio_mod_handler->bootstrap_ops->func_name) { \
            local_err = smio_mod_handler->bootstrap_ops->func_name (__VA_ARGS__);  \
        }                                                   \
        local_err;                                          \
    })

#define SMIO_DISPATCH_FUNC_WRAPPER(func_name, smio_mod_handler, ...)          \
    SMIO_DISPATCH_FUNC_WRAPPER_GEN(func_name, smio_mod_handler, self, ## __VA_ARGS__)

/* Attach an instance of sm_io to dev_io function pointer */
typedef smio_err_e (*attach_fp)(smio_t *self, void *args);
/* Deattach an instance of sm_io to dev_io function pointer */
typedef smio_err_e (*deattach_fp)(smio_t *self);
/* Export (register) sm_io to handle operations function pointer */
typedef smio_err_e (*export_ops_fp)(smio_t *self, const disp_op_t** smio_exp_ops);
/* Unexport (unregister) sm_io to handle operations function pointer */
typedef smio_err_e (*unexport_ops_fp)(smio_t *self);
/* Generic wrapper for receiving opcodes and arguments to specific funtions function pointer */
typedef smio_err_e (*do_op_fp)(void *owner, void *msg);

typedef struct {
    attach_fp attach;                   /* Attach sm_io instance to dev_io */
    deattach_fp deattach;               /* Deattach sm_io instance to dev_io */
    export_ops_fp export_ops;           /* Export sm_io operations to dev_io */
    unexport_ops_fp unexport_ops;       /* Unexport sm_io operations to dev_io */
    do_op_fp do_op;                     /* Generic wrapper for handling specific operations */
    do_op_fp do_mgmt_op;                /* Generic wrapper for handling management messages.
                                           For communcation between SMIOs, not external use */
} smio_ops_t;

/* Open device */
typedef int (*thsafe_client_open_fp) (smio_t *self, llio_endpoint_t *endpoint);
/* Release device */
typedef int (*thsafe_client_release_fp) (smio_t *self, llio_endpoint_t *endpoint);
/* Read data from device */
typedef ssize_t (*thsafe_client_read_16_fp) (smio_t *self, uint64_t offs, uint16_t *data);
typedef ssize_t (*thsafe_client_read_32_fp) (smio_t *self, uint64_t offs, uint32_t *data);
typedef ssize_t (*thsafe_client_read_64_fp) (smio_t *self, uint64_t offs, uint64_t *data);
/* Write data to device */
typedef ssize_t (*thsafe_client_write_16_fp) (smio_t *self, uint64_t offs, const uint16_t *data);
typedef ssize_t (*thsafe_client_write_32_fp) (smio_t *self, uint64_t offs, const uint32_t *data);
typedef ssize_t (*thsafe_client_write_64_fp) (smio_t *self, uint64_t offs, const uint64_t *data);
/* Read data block from device, size in bytes */
typedef ssize_t (*thsafe_client_read_block_fp) (smio_t *self, uint64_t offs, size_t size, uint32_t *data);
/* Write data block from device, size in bytes */
typedef ssize_t (*thsafe_client_write_block_fp) (smio_t *self, uint64_t offs, size_t size, const uint32_t *data);
/* Read data block via DMA from device, size in bytes */
typedef ssize_t (*thsafe_client_read_dma_fp) (smio_t *self, uint64_t offs, size_t size, uint32_t *data);
/* Write data block via DMA from device, size in bytes */
typedef ssize_t (*thsafe_client_write_dma_fp) (smio_t *self, uint64_t offs, size_t size, const uint32_t *data);
/* Read device information */
/* typedef int (*thsafe_client_read_info_fp) (smio_t *self, llio_dev_info_t *dev_info); Moved to dev_io */

typedef struct {
    thsafe_client_open_fp thsafe_client_open;                   /* Open device */
    thsafe_client_release_fp thsafe_client_release;             /* Release device */
    thsafe_client_read_16_fp thsafe_client_read_16;             /* Read 16-bit data */
    thsafe_client_read_32_fp thsafe_client_read_32;             /* Read 32-bit data */
    thsafe_client_read_64_fp thsafe_client_read_64;             /* Read 64-bit data */
    thsafe_client_write_16_fp thsafe_client_write_16;           /* Write 16-bit data */
    thsafe_client_write_32_fp thsafe_client_write_32;           /* Write 32-bit data */
    thsafe_client_write_64_fp thsafe_client_write_64;           /* Write 64-bit data */
    thsafe_client_read_block_fp thsafe_client_read_block;       /* Read arbitrary block size data,
                                                     parameter size in bytes */
    thsafe_client_write_block_fp thsafe_client_write_block;     /* Write arbitrary block size data,
                                                     parameter size in bytes */
    thsafe_client_read_dma_fp thsafe_client_read_dma;           /* Read arbitrary block size data via DMA,
                                                     parameter size in bytes */
    thsafe_client_write_dma_fp thsafe_client_write_dma;         /* Write arbitrary block size data via DMA,
                                                     parameter size in bytes */
    /*thsafe_client_read_info_fp thsafe_client_read_info; Moved to dev_io */         /* Read device information data */
} smio_thsafe_client_ops_t;

/* Thread boot args structure */
typedef struct {
    void *args;                                                 /* Generic arguments for SMIO */
    volatile const smio_mod_dispatch_t *smio_handler;           /* SMIO table handler */
    zsock_t *pipe_msg;                                          /* Message PIPE to actor */
    char *broker;                                               /* Endpoint to connect to broker */
    char *service;                                              /* (part of) the service name to be exported */
    int verbose;                                                /* Print trace information to stdout*/
    uint64_t base;                                              /* SMIO base address */
    uint32_t inst_id;                                           /* SMIO instance ID */
} th_boot_args_t;

/***************** Our methods *****************/

/* Creates a new instance of the SMIO */
smio_t *smio_new (th_boot_args_t *args, zsock_t *pipe_mgmt, zsock_t *pipe_msg,
        char *service);
/* Destroy an instance of the Low-level I/O */
smio_err_e smio_destroy (smio_t **self_p);
/* Loop through all interface sockets */
smio_err_e smio_loop (smio_t *self);
/* Register SMIO */
smio_err_e smio_register_sm (smio_t *self, uint32_t smio_id, uint64_t base,
        uint32_t inst_id);
/* Send MGMT message */
smio_err_e smio_send_mgmt_msg (smio_t *self, uint32_t dest_smio_id,
    uint32_t dest_smio_inst, char *msg);

smio_err_e smio_init_exp_ops (smio_t *self, disp_op_t** smio_exp_ops,
        const disp_table_func_fp *func_fps);
/* Set SMIO base address */
smio_err_e smio_set_base (smio_t *self, uint64_t base);
/* Get SMIO base address */
uint64_t smio_get_base (smio_t *self);
/* Set SMIO id */
smio_err_e smio_set_id (smio_t *self, uint32_t id);
/* Get SMIO id */
uint32_t smio_get_id (smio_t *self);
/* Set SMIO instance id */
smio_err_e smio_set_inst_id (smio_t *self, uint32_t inst_id);
/* Get SMIO instance id */
uint32_t smio_get_inst_id (smio_t *self);
/* Set SMIO name */
smio_err_e smio_set_name (smio_t *self, const char *name);
/* Get SMIO name */
const char *smio_get_name (smio_t *self);
/* Clone SMIO name */
char *smio_clone_name (smio_t *self);
/* Set SMIO exported operations */
smio_err_e smio_set_exp_ops (smio_t *self, const disp_op_t **exp_ops);
/* Get SMIO exported operation */
const disp_op_t **smio_get_exp_ops (smio_t *self);
/* Set SMIO operations */
smio_err_e smio_set_ops (smio_t *self, const smio_ops_t *ops);
/* Get SMIO operations */
const smio_ops_t *smio_get_ops (smio_t *self);
/* Set SMIO thsafe client operations */
smio_err_e smio_set_thsafe_client_ops (smio_t *self,
        const smio_thsafe_client_ops_t *thsafe_client_ops);
/* Get SMIO thsafe client operations */
const smio_thsafe_client_ops_t *smio_get_thsafe_client_ops (smio_t *self);
/* Set SMIO handler */
smio_err_e smio_set_handler (smio_t *self, void *smio_handler);
/* Get SMIO handler */
void *smio_get_handler (smio_t *self);
/* Get SMIO Worker */
mlm_client_t *smio_get_worker (smio_t *self);
/* Get SMIO PIPE Message */
zsock_t *smio_get_pipe_msg (smio_t *self);
/* Get SMIO PIPE Management */
zsock_t *smio_get_pipe_mgmt (smio_t *self);
/* Get poller */
zpoller_t *smio_get_poller (smio_t *self);

/************************************************************/
/**************** Smio OPS generic methods API **************/
/************************************************************/

/* Attach an instance of sm_io to dev_io function pointer */
smio_err_e smio_attach (smio_t *self, void *args);
/* Deattach an instance of sm_io to dev_io function pointer */
smio_err_e smio_deattach (smio_t *self);
/* Export (Register) sm_io to handle specific operations */
smio_err_e smio_export_ops (smio_t *self, const disp_op_t** smio_exp_ops);
/* Unexport (unregister) sm_io to handle specific operations */
smio_err_e smio_unexport_ops (smio_t *self);
/* Handle the operation */
smio_err_e smio_do_op (void *owner, void *msg);

/************************************************************/
/***************** Thsafe generic methods API ***************/
/************************************************************/

/* Open device */
int smio_thsafe_client_open (smio_t *self, llio_endpoint_t *endpoint);
/* Release device */
int smio_thsafe_client_release (smio_t *self, llio_endpoint_t *endpoint);

/* Read data from device */
ssize_t smio_thsafe_client_read_16 (smio_t *self, uint64_t offs, uint16_t *data);
ssize_t smio_thsafe_client_read_32 (smio_t *self, uint64_t offs, uint32_t *data);
ssize_t smio_thsafe_client_read_64 (smio_t *self, uint64_t offs, uint64_t *data);
/* Read data from device with raw address (no base address mangling) */
ssize_t smio_thsafe_raw_client_read_16 (smio_t *self, uint64_t offs, uint16_t *data);
ssize_t smio_thsafe_raw_client_read_32 (smio_t *self, uint64_t offs, uint32_t *data);
ssize_t smio_thsafe_raw_client_read_64 (smio_t *self, uint64_t offs, uint64_t *data);

/* Write data to device */
ssize_t smio_thsafe_client_write_16 (smio_t *self, uint64_t offs, const uint16_t *data);
ssize_t smio_thsafe_client_write_32 (smio_t *self, uint64_t offs, const uint32_t *data);
ssize_t smio_thsafe_client_write_64 (smio_t *self, uint64_t offs, const uint64_t *data);
/* Write data to device with raw address (no base address mangling) */
ssize_t smio_thsafe_raw_client_write_16 (smio_t *self, uint64_t offs, const uint16_t *data);
ssize_t smio_thsafe_raw_client_write_32 (smio_t *self, uint64_t offs, const uint32_t *data);
ssize_t smio_thsafe_raw_client_write_64 (smio_t *self, uint64_t offs, const uint64_t *data);

/* Read data block from device, size in bytes */
ssize_t smio_thsafe_client_read_block (smio_t *self, uint64_t offs, size_t size, uint32_t *data);
/* Read data block from device, size in bytes, with raw address (no base address mangling) */
ssize_t smio_thsafe_raw_client_read_block (smio_t *self, uint64_t offs, size_t size, uint32_t *data);

/* Write data block from device, size in bytes */
ssize_t smio_thsafe_client_write_block (smio_t *self, uint64_t offs, size_t size, const uint32_t *data);
/* Write data block from device, size in bytes, with raw address (no base address mangling) */
ssize_t smio_thsafe_raw_client_write_block (smio_t *self, uint64_t offs, size_t size, const uint32_t *data);

/* read data block via dma from device, size in bytes */
ssize_t smio_thsafe_client_read_dma (smio_t *self, uint64_t offs, size_t size, uint32_t *data);
/* read data block via dma from device, size in bytes, with raw address (no base address mangling) */
ssize_t smio_thsafe_raw_client_read_dma (smio_t *self, uint64_t offs, size_t size, uint32_t *data);

/* Write data block via DMA from device, size in bytes */
ssize_t smio_thsafe_client_write_dma (smio_t *self, uint64_t offs, size_t size, const uint32_t *data);
/* Write data block via DMA from device, size in bytes, with raw address (no base address mangling) */
ssize_t smio_thsafe_raw_client_write_dma (smio_t *self, uint64_t offs, size_t size, const uint32_t *data);

/* Read device information */
/* int smio_thsafe_client_read_info (smio_t *self, llio_dev_info_t *dev_info) */

#ifdef __cplusplus
}
#endif

#endif
