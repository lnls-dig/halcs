/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _DEV_IO_CORE_H_
#define _DEV_IO_CORE_H_

#ifdef __cplusplus
extern "C" {
#endif

/* SMIO hash key length in chars */
#define SMIO_HKEY_LEN                   8
#define NODES_MAX_LEN                   100

/* Node of sig_ops list */
typedef struct {
    int signal;         /* Signal identifier, e.g., SIGINT, SIGKILL, etc... */
    sig_handler_fp devio_sig_h;
} devio_sig_handler_t;

typedef struct {
    wait_chld_handler_fp devio_wait_chld;                   /* Called to wait a all child process */
    wait_chld_timed_handler_fp devio_wait_chld_timed;       /* Called to wait a all child process */
    spawn_chld_handler_fp devio_spawn_chld;                 /* Called to spawn a new process to handle device */

    /* List of devio_sig_handler_t */
    zlistx_t *sig_ops;
} devio_ops_t;

typedef struct {
    disp_table_func_fp thsafe_server_open;              /* Open device */
    disp_table_func_fp thsafe_server_release;           /* Release device */
    disp_table_func_fp thsafe_server_read_16;           /* Read 16-bit data */
    disp_table_func_fp thsafe_server_read_32;           /* Read 32-bit data */
    disp_table_func_fp thsafe_server_read_64;           /* Read 64-bit data */
    disp_table_func_fp thsafe_server_write_16;          /* Write 16-bit data */
    disp_table_func_fp thsafe_server_write_32;          /* Write 32-bit data */
    disp_table_func_fp thsafe_server_write_64;          /* Write 64-bit data */
    disp_table_func_fp thsafe_server_read_block;        /* Read arbitrary block size data,
                                                            parameter size in bytes */
    disp_table_func_fp thsafe_server_write_block;       /* Write arbitrary block size data,
                                                            parameter size in bytes */
    disp_table_func_fp thsafe_server_read_dma;          /* Read arbitrary block size data via DMA,
                                                            parameter size in bytes */
    disp_table_func_fp thsafe_server_write_dma;         /* Write arbitrary block size data via DMA,
                                                            parameter size in bytes */
    /*disp_table_func_fp_read_info_fp thsafe_server_read_info; Moved to dev_io */
    /* Read device information data */
} smio_thsafe_server_ops_t;

/***************** Our methods *****************/

/* Creates a new instance of Device Information */
devio_t * devio_new (char *name, uint32_t id, char *endpoint_dev,
        const llio_ops_t *reg_ops, char *endpoint_broker, int verbose,
        const char *log_file_name);
/* Destroy an instance of the Device Information */
devio_err_e devio_destroy (devio_t **self_p);

/* Read specific information about the device. Typically,
 * this is stored in the SDB structure inside the device */
devio_err_e devio_print_info (devio_t *self);
/* Register an specific sm_io module to this device */
devio_err_e devio_register_sm (void *pipe, uint32_t smio_id,
        uint64_t base, uint32_t inst_id);
/* Register a specific sm_io module to this device */
devio_err_e devio_register_sm_by_id (void *pipe, uint32_t smio_id);
/* Register all sm_io module that this device can handle,
 * according to the device information stored in the SDB */
devio_err_e devio_register_all_sm (void *pipe);
devio_err_e devio_unregister_sm (void *pipe, const char *smio_key);
devio_err_e devio_unregister_all_sm (void *pipe);
/* Poll all PIPE sockets */
void devio_loop (zsock_t *pipe, void *args);
/* Router for all the opcodes registered for this dev_io */
/* devio_err_e devio_do_op (devio_t *self, uint32_t opcode, int nargs, ...); */
/* Router for all of the low-level operations for this dev_io */
devio_err_e devio_do_smio_op (devio_t *self, void *msg);
/* Set LLIO instance to DEVIO, if not open */
devio_err_e devio_set_llio (devio_t *self, llio_t *llio);
/* Get LLIO instance from DEVIO */
llio_t *devio_get_llio (devio_t *self);

/* Register signals to Device Manager instance */
devio_err_e devio_set_sig_handler (devio_t *self, devio_sig_handler_t *sig_handler);
/* Register all signal handlers previously set */
devio_err_e devio_register_sig_handlers (devio_t *self);
/* Register function to wait a all child process */
devio_err_e devio_set_wait_clhd_handler (devio_t *self, wait_chld_handler_fp fp);
/* Register function to wait with timeout a all child process */
devio_err_e devio_set_wait_clhd_timed_handler (devio_t *self, wait_chld_timed_handler_fp fp);
/* Execute function to wait a all child process */
devio_err_e devio_wait_chld (devio_t *self);
/* Execute function to wait with timeout all child process */
devio_err_e devio_wait_chld_timed (devio_t *self, int timeout);
/* Register function to spawn a all child process */
devio_err_e devio_set_spawn_clhd_handler (devio_t *self, spawn_chld_handler_fp fp);
/* Execute function to spawn a all child process */
devio_err_e devio_spawn_chld (devio_t *self, const char *program, char *const argv[]);

/* Setting all operations at once */
devio_err_e devio_set_ops (devio_t *self, devio_ops_t *devio_ops);

/********* Low-level generic methods API *********/

/* Open device */
int smio_thsafe_server_open (void *owner, void *args, void *ret);
/* Release device */
int smio_thsafe_server_release (void *owner, void *args, void *ret);
/* Read data from device */
int smio_thsafe_server_read_16 (void *owner, void *args, void *ret);
int smio_thsafe_server_read_32 (void *owner, void *args, void *ret);
int smio_thsafe_server_read_64 (void *owner, void *args, void *ret);
/* Write data to device */
int smio_thsafe_server_write_16 (void *owner, void *args, void *ret);
int smio_thsafe_server_write_32 (void *owner, void *args, void *ret);
int smio_thsafe_server_write_64 (void *owner, void *args, void *ret);
/* Read data block from device, size in bytes */
int smio_thsafe_server_read_block (void *owner, void *args, void *ret);
/* Write data block from device, size in bytes */
int smio_thsafe_server_write_block (void *owner, void *args, void *ret);
/* Read data block via DMA from device, size in bytes */
int smio_thsafe_server_read_dma (void *owner, void *args, void *ret);
/* Write data block via DMA from device, size in bytes */
int smio_thsafe_server_write_dma (void *owner, void *args, void *ret);
/* Read device information */
/* int smio_thsafe_server_read_info (void *owner, void *args, void *ret); */

#ifdef __cplusplus
}
#endif

#endif
