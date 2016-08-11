/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _SM_PR_H_
#define _SM_PR_H_

#ifdef __cplusplus
extern "C" {
#endif

#define SM_PR_READBACK                      1

/* Conversion MACROS */
#define SMPR_BYTE_2_BIT                     8
#define SMPR_WB_REG_2_BYTE                  4       /* 32-bit word */
#define SMPR_WB_REG_2_BIT                   (SMPR_WB_REG_2_BYTE*SMPR_BYTE_2_BIT)

/* Open protocol */
typedef int (*proto_open_fp) (smpr_t *self, uint64_t base, void *args);
/* Release protocol */
typedef int (*proto_release_fp) (smpr_t *self);
/* Read data from protocol */
typedef ssize_t (*proto_read_16_fp) (smpr_t *self, size_t size_offs, uint64_t offs, uint16_t *data);
typedef ssize_t (*proto_read_32_fp) (smpr_t *self, size_t size_offs, uint64_t offs, uint32_t *data);
typedef ssize_t (*proto_read_64_fp) (smpr_t *self, size_t size_offs, uint64_t offs, uint64_t *data);
/* Write data to protocol */
typedef ssize_t (*proto_write_16_fp) (smpr_t *self, size_t size_offs, uint64_t offs, const uint16_t *data);
typedef ssize_t (*proto_write_32_fp) (smpr_t *self, size_t size_offs, uint64_t offs, const uint32_t *data);
typedef ssize_t (*proto_write_64_fp) (smpr_t *self, size_t size_offs, uint64_t offs, const uint64_t *data);
/* Read data block from protocol, size in bytes */
typedef ssize_t (*proto_read_block_fp) (smpr_t *self, size_t size_offs, uint64_t offs, size_t size, uint32_t *data);
/* Write data block from protocol, size in bytes */
typedef ssize_t (*proto_write_block_fp) (smpr_t *self, size_t size_offs, uint64_t offs, size_t size, const uint32_t *data);
/* Read data block via DMA from protocol, size in bytes */
typedef ssize_t (*proto_read_dma_fp) (smpr_t *self, size_t size_offs, uint64_t offs, size_t size, uint32_t *data);
/* Write data block via DMA from protocol, size in bytes */
typedef ssize_t (*proto_write_dma_fp) (smpr_t *self, size_t size_offs, uint64_t offs, size_t size, const uint32_t *data);

typedef struct {
    const char *proto_name;                     /* Protocol name */
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
} smpr_proto_ops_t;

/***************** Our methods *****************/

/* Creates a new instance of the Low-level I/O */
smpr_t * smpr_new (char *name, smio_t *parent, const smpr_proto_ops_t *reg_ops,
        int verbose);
/* Destroy an instance of the Low-level I/O */
smpr_err_e smpr_destroy (smpr_t **self_p);
/* Register Specific Protocol operations to smpr instance */
smpr_err_e smpr_set_handler (smpr_t *self, void *handler);
/* Get protocol handler */
void *smpr_get_handler (smpr_t *self);
/* Unregister Specific Protocol operations to smpr instance */
void *smpr_unset_handler (smpr_t *self);
/* Get parent handler */
smio_t *smpr_get_parent (smpr_t *self);
/* Get ops */
const smpr_proto_ops_t *smpr_get_ops (smpr_t *self);
/* Get protocol name */
const char *smpr_get_ops_name (smpr_t *self);

/************************************************************/
/***************** Thsafe generic methods API ***************/
/************************************************************/

/* Open protocol */
int smpr_open (smpr_t *self, uint64_t base, void *args);
/* Release protocol */
int smpr_release (smpr_t *self);
/* Read data from protocol */
ssize_t smpr_read_16 (smpr_t *self, size_t size_offs, uint64_t offs, uint16_t *data);
ssize_t smpr_read_32 (smpr_t *self, size_t size_offs, uint64_t offs, uint32_t *data);
ssize_t smpr_read_64 (smpr_t *self, size_t size_offs, uint64_t offs, uint64_t *data);
/* Write data to protocol */
ssize_t smpr_write_16 (smpr_t *self, size_t size_offs, uint64_t offs, const uint16_t *data);
ssize_t smpr_write_32 (smpr_t *self, size_t size_offs, uint64_t offs, const uint32_t *data);
ssize_t smpr_write_64 (smpr_t *self, size_t size_offs, uint64_t offs, const uint64_t *data);
/* Read data block from protocol, size in bytes */
ssize_t smpr_read_block (smpr_t *self, size_t size_offs, uint64_t offs, size_t size, uint32_t *data);
/* Write data block from protocol, size in bytes */
ssize_t smpr_write_block (smpr_t *self, size_t size_offs, uint64_t offs, size_t size, uint32_t *data);
/* Read data block via DMA from protocol, size in bytes */
ssize_t smpr_read_dma (smpr_t *self, size_t size_offs, uint64_t offs, size_t size, uint32_t *data);
/* Write data block via DMA from protocol, size in bytes */
ssize_t smpr_write_dma (smpr_t *self, size_t size_offs, uint64_t offs, size_t size, uint32_t *data);

#ifdef __cplusplus
}
#endif

#endif
