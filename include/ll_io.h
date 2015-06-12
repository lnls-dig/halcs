/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _LL_IO_H_
#define _LL_IO_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Open device function pointer */
typedef int (*open_fp)(llio_t *self, llio_endpoint_t *endpoint);
/* Release device function pointer */
typedef int (*release_fp)(llio_t *self, llio_endpoint_t *endpoint);
/* Read data from device function pointers */
typedef ssize_t (*read_16_fp)(llio_t *self, uint64_t offs, uint16_t *data);
typedef ssize_t (*read_32_fp)(llio_t *self, uint64_t offs, uint32_t *data);
typedef ssize_t (*read_64_fp)(llio_t *self, uint64_t offs, uint64_t *data);
/* Write data to device function pointers */
typedef ssize_t (*write_16_fp)(llio_t *self, uint64_t offs, const uint16_t *data);
typedef ssize_t (*write_32_fp)(llio_t *self, uint64_t offs, const uint32_t *data);
typedef ssize_t (*write_64_fp)(llio_t *self, uint64_t offs, const uint64_t *data);
/* Read data block from device function pointer, size in bytes */
typedef ssize_t (*read_block_fp)(llio_t *self, uint64_t offs, size_t size, uint32_t *data);
/* Write data block from device function pointer, size in bytes */
typedef ssize_t (*write_block_fp)(llio_t *self, uint64_t offs, size_t size, uint32_t *data);
/* Read data block via DMA from device function pointer, size in bytes */
typedef ssize_t (*read_dma_fp)(llio_t *self, uint64_t offs, size_t size, uint32_t *data);
/* Write data block via DMA from device function pointer, size in bytes */
typedef ssize_t (*write_dma_fp)(llio_t *self, uint64_t offs, size_t size, uint32_t *data);
/* Read device information function pointer */
/* typedef int (*read_info_fp)(struct _llio_t *self, struct _llio_dev_info_t *dev_info); moved to dev_io */

typedef struct {
    open_fp open;                   /* Open device */
    release_fp release;             /* Release device */
    read_16_fp read_16;             /* Read 16-bit data */
    read_32_fp read_32;             /* Read 32-bit data */
    read_64_fp read_64;             /* Read 64-bit data */
    write_16_fp write_16;           /* Write 16-bit data */
    write_32_fp write_32;           /* Write 32-bit data */
    write_64_fp write_64;           /* Write 64-bit data */
    read_block_fp read_block;       /* Read arbitrary block size data,
                                       parameter size in bytes */
    write_block_fp write_block;     /* Write arbitrary block size data,
                                       parameter size in bytes */
    read_dma_fp read_dma;           /* Read arbitrary block size data via DMA,
                                       parameter size in bytes */
    write_dma_fp write_dma;         /* Write arbitrary block size data via DMA,
                                       parameter size in bytes */
    /*read_info_fp read_info; Moved to dev_io */         /* Read device information data */
} llio_ops_t;

/************************************************************/
/************************* Our methods **********************/
/************************************************************/

/* Creates a new instance of the Low-level I/O */
llio_t * llio_new (char *name, char *endpoint, llio_type_e type, int verbose);
/* Destroy an instance of the Low-level I/O */
llio_err_e llio_destroy (llio_t **self_p);

/* Set endpoint */
llio_err_e llio_set_endpoint (llio_t *self, llio_endpoint_t *endpoint);
/* Get endpoint */
const llio_endpoint_t *llio_get_endpoint (llio_t *self);
/* Set endpoint open status */
llio_err_e llio_set_endpoint_open (llio_t *self, bool open);
/* Get endpoint open status */
bool llio_get_endpoint_open (llio_t *self);
/* Sets an endpoint in case it was not done in llio_new() */
llio_err_e llio_set_endpoint_name (llio_t *self, const char *name);
/* Gets endpoint from name from instance of llio_endpoint */
const char *llio_get_endpoint_name (llio_t *self);
/* Clone endpoint from name from instance of llio_endpoint */
char *llio_clone_endpoint_name (llio_t *self);
/* Set dev handler */
llio_err_e llio_set_dev_handler (llio_t *self, void *dev_handler);
/* Get dev handler */
void *llio_get_dev_handler (llio_t *self);

/************************************************************/
/**************** Low Level generic methods API *************/
/************************************************************/

/* Open device */
int llio_open (llio_t *self, llio_endpoint_t *endpoint);
/* Release device */
int llio_release (llio_t *self, llio_endpoint_t *endpoint);
/* Read data from device */
ssize_t llio_read_16 (llio_t *self, uint64_t offs, uint16_t *data);
ssize_t llio_read_32 (llio_t *self, uint64_t offs, uint32_t *data);
ssize_t llio_read_64 (llio_t *self, uint64_t offs, uint64_t *data);
/* Write data to device */
ssize_t llio_write_16 (llio_t *self, uint64_t offs, const uint16_t *data);
ssize_t llio_write_32 (llio_t *self, uint64_t offs, const uint32_t *data);
ssize_t llio_write_64 (llio_t *self, uint64_t offs, const uint64_t *data);
/* Read data block from device, size in bytes */
ssize_t llio_read_block (llio_t *self, uint64_t offs, size_t size, uint32_t *data);
/* Write data block from device, size in bytes */
ssize_t llio_write_block (llio_t *self, uint64_t offs, size_t size, uint32_t *data);
/* Read data block via DMA from device, size in bytes */
ssize_t llio_read_dma (llio_t *self, uint64_t offs, size_t size, uint32_t *data);
/* Write data block via DMA from device, size in bytes */
ssize_t llio_write_dma (llio_t *self, uint64_t offs, size_t size, uint32_t *data);
/* Read device information */
/* int llio_read_info (llio_t *self, llio_dev_info_t *dev_info); Moved to dev_io */

#ifdef __cplusplus
}
#endif

#endif
