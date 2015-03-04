/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _SM_IO_H_
#define _SM_IO_H_

#include <inttypes.h>
#include <sys/types.h>
#include <stdbool.h>

/* #include "dev_io_core.h" */
#include "ll_io.h"
#include "sm_io_err.h"
#include "sm_io_bootstrap.h"
#include "sm_io_mod_dispatch.h"
#include "sm_io_exports.h"
#include "msg.h"
#include "dispatch_table.h"
#include "mdp.h"

/* SMIO sockets IDs */
#define SMIO_WORKER_SOCK            0
#define SMIO_PIPE_SOCK              1
#define SMIO_END_SOCK               2
#define SMIO_SOCKS_NUM              SMIO_END_SOCK

struct _devio_t;
struct _smio_ops_t;
struct _smio_thsafe_client_ops_t;
struct _disp_op_t;

/* Main class object that every sm_io must implement */
struct _smio_t {
    uint32_t id;                        /* Unique identifier for this sm_io type. This must be
                                           the same from the SDB ID */
    uint32_t inst_id;                   /* Instance ID. This differentiate SMIOs
                                           of the same type */
    uint32_t base;                      /* Base SMIO address */
    char *name;                         /* Identification of this sm_io instance */
    char *service;                      /* Exported service name */
    /* int verbose; */                  /* Print activity to stdout */
    mdp_worker_t *worker;               /* zeroMQ Majordomo worker */
    struct _devio_t *parent;            /* Pointer back to parent dev_io */
    void *smio_handler;                 /* Generic pointer to a device handler. This
                                            must be cast to a specific type by the
                                            devices functions */
    zctx_t *ctx;                        /* Our context */
    void *pipe;                         /* Pipe back to parent to exchange messages */

    /* Specific SMIO operations dispatch table for exported operations */
    disp_table_t *exp_ops_dtable;
    /* Specific SMIO instance functions. This will be exported on SMIO startup */
    const struct _disp_op_t **exp_ops;
    /* Minimum set of methods that every sm_io instance must implement.
         * This is thought as the interface to the world */
    const struct _smio_ops_t *ops;
    /* These operations must be used to access LLIO methods, as they are
     * thread safe. The method's signatures are almost the same as the
     * ones available in llio, changing the llio_t self pointer to a void
     * pointer (socket to parent thread) */
    const struct _smio_thsafe_client_ops_t *thsafe_client_ops;
};

/* Attach an instance of sm_io to dev_io function pointer */
typedef enum _smio_err_e (*attach_fp)(struct _smio_t *self, struct _devio_t *parent);
/* Deattach an instance of sm_io to dev_io function pointer */
typedef enum _smio_err_e (*deattach_fp)(struct _smio_t *self);
/* Export (register) sm_io to handle operations function pointer */
typedef enum _smio_err_e (*export_ops_fp)(struct _smio_t *self,
        const struct _disp_op_t** smio_exp_ops);
/* Unexport (unregister) sm_io to handle operations function pointer */
typedef enum _smio_err_e (*unexport_ops_fp)(struct _smio_t *self);
/* Generic wrapper for receiving opcodes and arguments to specific funtions function pointer */
typedef enum _smio_err_e (*do_op_fp)(void *owner, void *msg);

struct _smio_ops_t {
    attach_fp attach;                   /* Attach sm_io instance to dev_io */
    deattach_fp deattach;               /* Deattach sm_io instance to dev_io */
    export_ops_fp export_ops;           /* Export sm_io operations to dev_io */
    unexport_ops_fp unexport_ops;       /* Unexport sm_io operations to dev_io */
    do_op_fp do_op;                     /* Generic wrapper for handling specific operations */
};

/* Open device */
typedef int (*thsafe_client_open_fp) (struct _smio_t *self, llio_endpoint_t *endpoint);
/* Release device */
typedef int (*thsafe_client_release_fp) (struct _smio_t *self, llio_endpoint_t *endpoint);
/* Read data from device */
typedef ssize_t (*thsafe_client_read_16_fp) (struct _smio_t *self, loff_t offs, uint16_t *data);
typedef ssize_t (*thsafe_client_read_32_fp) (struct _smio_t *self, loff_t offs, uint32_t *data);
typedef ssize_t (*thsafe_client_read_64_fp) (struct _smio_t *self, loff_t offs, uint64_t *data);
/* Write data to device */
typedef ssize_t (*thsafe_client_write_16_fp) (struct _smio_t *self, loff_t offs, const uint16_t *data);
typedef ssize_t (*thsafe_client_write_32_fp) (struct _smio_t *self, loff_t offs, const uint32_t *data);
typedef ssize_t (*thsafe_client_write_64_fp) (struct _smio_t *self, loff_t offs, const uint64_t *data);
/* Read data block from device, size in bytes */
typedef ssize_t (*thsafe_client_read_block_fp) (struct _smio_t *self, loff_t offs, size_t size, uint32_t *data);
/* Write data block from device, size in bytes */
typedef ssize_t (*thsafe_client_write_block_fp) (struct _smio_t *self, loff_t offs, size_t size, const uint32_t *data);
/* Read data block via DMA from device, size in bytes */
typedef ssize_t (*thsafe_client_read_dma_fp) (struct _smio_t *self, loff_t offs, size_t size, uint32_t *data);
/* Write data block via DMA from device, size in bytes */
typedef ssize_t (*thsafe_client_write_dma_fp) (struct _smio_t *self, loff_t offs, size_t size, const uint32_t *data);
/* Read device information */
/* typedef int (*thsafe_client_read_info_fp) (struct _smio_t *self, llio_dev_info_t *dev_info); Moved to dev_io */

struct _smio_thsafe_client_ops_t {
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
};

/* Opaque class structure */
typedef struct _smio_t smio_t;
/* Opaque smio_ops structure */
typedef struct _smio_ops_t smio_ops_t;
/* Opaque llio_th_safe_ops structure */
typedef struct _smio_thsafe_client_ops_t smio_thsafe_client_ops_t;

/***************** Our methods *****************/

/* Creates a new instance of the Low-level I/O */
/* smio_t * smio_new (uint32_t id, char *name, devio_t *parent); */
/* Destroy an instance of the Low-level I/O */
/* smio_err_e smio_destroy (smio_t **self_p); */

smio_err_e smio_init_exp_ops (smio_t *self, disp_op_t** smio_exp_ops,
        const disp_table_func_fp *func_fps);

/************************************************************/
/**************** Smio OPS generic methods API **************/
/************************************************************/

/* Attach an instance of sm_io to dev_io function pointer */
smio_err_e smio_attach (smio_t *self, struct _devio_t *parent);
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
ssize_t smio_thsafe_client_read_16 (smio_t *self, loff_t offs, uint16_t *data);
ssize_t smio_thsafe_client_read_32 (smio_t *self, loff_t offs, uint32_t *data);
ssize_t smio_thsafe_client_read_64 (smio_t *self, loff_t offs, uint64_t *data);
/* Read data from device with raw address (no base address mangling) */
ssize_t smio_thsafe_raw_client_read_16 (smio_t *self, loff_t offs, uint16_t *data);
ssize_t smio_thsafe_raw_client_read_32 (smio_t *self, loff_t offs, uint32_t *data);
ssize_t smio_thsafe_raw_client_read_64 (smio_t *self, loff_t offs, uint64_t *data);

/* Write data to device */
ssize_t smio_thsafe_client_write_16 (smio_t *self, loff_t offs, const uint16_t *data);
ssize_t smio_thsafe_client_write_32 (smio_t *self, loff_t offs, const uint32_t *data);
ssize_t smio_thsafe_client_write_64 (smio_t *self, loff_t offs, const uint64_t *data);
/* Write data to device with raw address (no base address mangling) */
ssize_t smio_thsafe_raw_client_write_16 (smio_t *self, loff_t offs, const uint16_t *data);
ssize_t smio_thsafe_raw_client_write_32 (smio_t *self, loff_t offs, const uint32_t *data);
ssize_t smio_thsafe_raw_client_write_64 (smio_t *self, loff_t offs, const uint64_t *data);

/* Read data block from device, size in bytes */
ssize_t smio_thsafe_client_read_block (smio_t *self, loff_t offs, size_t size, uint32_t *data);
/* Read data block from device, size in bytes, with raw address (no base address mangling) */
ssize_t smio_thsafe_raw_client_read_block (smio_t *self, loff_t offs, size_t size, uint32_t *data);

/* Write data block from device, size in bytes */
ssize_t smio_thsafe_client_write_block (smio_t *self, loff_t offs, size_t size, const uint32_t *data);
/* Write data block from device, size in bytes, with raw address (no base address mangling) */
ssize_t smio_thsafe_raw_client_write_block (smio_t *self, loff_t offs, size_t size, const uint32_t *data);

/* read data block via dma from device, size in bytes */
ssize_t smio_thsafe_client_read_dma (smio_t *self, loff_t offs, size_t size, uint32_t *data);
/* read data block via dma from device, size in bytes, with raw address (no base address mangling) */
ssize_t smio_thsafe_raw_client_read_dma (smio_t *self, loff_t offs, size_t size, uint32_t *data);

/* Write data block via DMA from device, size in bytes */
ssize_t smio_thsafe_client_write_dma (smio_t *self, loff_t offs, size_t size, const uint32_t *data);
/* Write data block via DMA from device, size in bytes, with raw address (no base address mangling) */
ssize_t smio_thsafe_raw_client_write_dma (smio_t *self, loff_t offs, size_t size, const uint32_t *data);

/* Read device information */
/* int smio_thsafe_client_read_info (smio_t *self, llio_dev_info_t *dev_info) */

#endif
