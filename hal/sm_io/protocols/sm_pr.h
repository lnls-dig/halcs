/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _SM_PR_H_
#define _SM_PR_H_

#include <inttypes.h>
#include <sys/types.h>

#include "sm_io.h"
#include "sm_pr_err.h"

/* This is used by the protocols to perform the device IO */
#define SMPR_PARENT(self) (self->parent)

enum _smpr_type_e {
    SMPR_SPI = 0,
    SMPR_I2C,
    SMPR_1WIRE,
    SMPR_GPIO,
    SMPR_BYPASS
};

struct _smpr_proto_ops_t;

struct _smpr_t {
    enum _smpr_type_e type;             /* Protocol type (SPI, I2C, 1-wire, GPIO, Bypass) */
    void *proto_handler;                /* Generic pointer to a protocol handler. This
                                            must be cast to a specific type by the
                                            specific protocol functions */
    char *name;                         /* Identification of this smpr instance */
    int verbose;                        /* Print activity to stdout */

    /* SMIO instance this SMPR belongs to. We need this, as
     * all of the low-level operations must go through it */
    smio_t *parent;
    /* Protocol operations */
    const struct _smpr_proto_ops_t *ops;
};

/* Open protocol */
typedef int (*proto_open_fp) (struct _smpr_t *self, uint32_t base, void *args);
/* Release protocol */
typedef int (*proto_release_fp) (struct _smpr_t *self);
/* Read data from protocol */
typedef ssize_t (*proto_read_16_fp) (struct _smpr_t *self, loff_t offs, uint16_t *data, uint32_t flags);
typedef ssize_t (*proto_read_32_fp) (struct _smpr_t *self, loff_t offs, uint32_t *data, uint32_t flags);
typedef ssize_t (*proto_read_64_fp) (struct _smpr_t *self, loff_t offs, uint64_t *data, uint32_t flags);
/* Write data to protocol */
typedef ssize_t (*proto_write_16_fp) (struct _smpr_t *self, loff_t offs, const uint16_t *data, uint32_t flags);
typedef ssize_t (*proto_write_32_fp) (struct _smpr_t *self, loff_t offs, const uint32_t *data, uint32_t flags);
typedef ssize_t (*proto_write_64_fp) (struct _smpr_t *self, loff_t offs, const uint64_t *data, uint32_t flags);
/* Read data block from protocol, size in bytes */
typedef ssize_t (*proto_read_block_fp) (struct _smpr_t *self, loff_t offs, size_t size, uint32_t *data, uint32_t flags);
/* Write data block from protocol, size in bytes */
typedef ssize_t (*proto_write_block_fp) (struct _smpr_t *self, loff_t offs, size_t size, const uint32_t *data, uint32_t flags);
/* Read data block via DMA from protocol, size in bytes */
typedef ssize_t (*proto_read_dma_fp) (struct _smpr_t *self, loff_t offs, size_t size, uint32_t *data, uint32_t flags);
/* Write data block via DMA from protocol, size in bytes */
typedef ssize_t (*proto_write_dma_fp) (struct _smpr_t *self, loff_t offs, size_t size, const uint32_t *data, uint32_t flags);

struct _smpr_proto_ops_t {
    proto_open_fp proto_open;                   /* Open protocol */
    proto_release_fp proto_release;             /* Release protocol */
    proto_read_16_fp proto_read_16;             /* Read 16-bit data */
    proto_read_32_fp proto_read_32;             /* Read 32-bit data */
    proto_read_64_fp proto_read_64;             /* Read 64-bit data */
    proto_write_16_fp proto_write_16;           /* Write 16-bit data */
    proto_write_32_fp proto_write_32;           /* Write 32-bit data */
    proto_write_64_fp proto_write_64;           /* Write 64-bit data */
    proto_read_block_fp proto_read_block;       /* Read arbitrary block size data,
                                                     parameter size in bytes */
    proto_write_block_fp proto_write_block;     /* Write arbitrary block size data,
                                                     parameter size in bytes */
    proto_read_dma_fp proto_read_dma;           /* Read arbitrary block size data via DMA,
                                                     parameter size in bytes */
    proto_write_dma_fp proto_write_dma;         /* Write arbitrary block size data via DMA,
                                                     parameter size in bytes */
};

/* Opaque sm_pr structure */
typedef struct _smpr_t smpr_t;
/* Opaque smpr_proto_ops structure */
typedef struct _smpr_proto_ops_t smpr_proto_ops_t;
/* smpr_type_e structure */
typedef enum _smpr_type_e smpr_type_e;

/***************** Our methods *****************/

/* Creates a new instance of the Low-level I/O */
smpr_t * smpr_new (char *name, smio_t *parent, smpr_type_e type, int verbose);
/* Destroy an instance of the Low-level I/O */
smpr_err_e smpr_destroy (smpr_t **self_p);

/************************************************************/
/***************** Thsafe generic methods API ***************/
/************************************************************/

/* Open protocol */
int smpr_open (smpr_t *self, uint32_t base, void *args);
/* Release protocol */
int smpr_release (smpr_t *self);
/* Read data from protocol */
ssize_t smpr_read_16 (smpr_t *self, loff_t offs, uint16_t *data, uint32_t flags);
ssize_t smpr_read_32 (smpr_t *self, loff_t offs, uint32_t *data, uint32_t flags);
ssize_t smpr_read_64 (smpr_t *self, loff_t offs, uint64_t *data, uint32_t flags);
/* Write data to protocol */
ssize_t smpr_write_16 (smpr_t *self, loff_t offs, const uint16_t *data, uint32_t flags);
ssize_t smpr_write_32 (smpr_t *self, loff_t offs, const uint32_t *data, uint32_t flags);
ssize_t smpr_write_64 (smpr_t *self, loff_t offs, const uint64_t *data, uint32_t flags);
/* Read data block from protocol, size in bytes */
ssize_t smpr_read_block (smpr_t *self, loff_t offs, size_t size, uint32_t *data, uint32_t flags);
/* Write data block from protocol, size in bytes */
ssize_t smpr_write_block (smpr_t *self, loff_t offs, size_t size, const uint32_t *data, uint32_t flags);
/* Read data block via DMA from protocol, size in bytes */
ssize_t smpr_read_dma (smpr_t *self, loff_t offs, size_t size, uint32_t *data, uint32_t flags);
/* Write data block via DMA from protocol, size in bytes */
ssize_t smpr_write_dma (smpr_t *self, loff_t offs, size_t size, const uint32_t *data, uint32_t flags);

#endif
