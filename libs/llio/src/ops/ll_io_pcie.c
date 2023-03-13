/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "ll_io.h"

/* PCIe specifics */
#include <pciDriver/lib/pciDriver.h>
#include "hw/pcie_regs.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, LL_IO, "[ll_io:pcie]",    \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_ALLOC(ptr, LL_IO, "[ll_io:pcie]",            \
            llio_err_str(LLIO_ERR_ALLOC),                   \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                            \
    CHECK_HAL_ERR(err, LL_IO, "[ll_io:pcie]",               \
            llio_err_str (err_type))

#define READ_FROM_BAR                           1
#define WRITE_TO_BAR                            0

#define PCIE_TIMEOUT_MAX_TRIES                  32
/* Wait between reads/writes, in usecs */
#define PCIE_TIMEOUT_WAIT                       1000

/* Timeout byte pattern */
#define PCIE_TIMEOUT_PATT_INIT                  0xFF
/* Number of timeout pattern bytes in a row to detect a timeout */
#define PCIE_TIMEOUT_PATT_SIZE                  32
/* Number of kernel memory bytes to alloc on initialization */
#define PCIE_DMA_KERNEL_BUFF_SIZE               (1 * (1 << 20)) /* in bytes */

/* Device endpoint */
typedef struct {
    pd_device_t *dev;                   /* PCIe device handler */
    uint32_t *bar0;                     /* PCIe BAR0 */
    uint32_t bar0_size;                 /* PCIe BAR0 size */
    uint32_t *bar2;                     /* PCIe BAR2 */
    uint32_t bar2_size;                 /* PCIe BAR2 size */
    uint64_t *bar4;                     /* PCIe BAR4 */
    uint32_t bar4_size;                 /* PCIe BAR4 size */

    /* DMA */
    pd_kmem_t *kmem_handle;             /* Kernel memory handler */
} llio_dev_pcie_t;

typedef enum {
    PCIE_DMA_US = 0,
    PCIE_DMA_DS
} pcie_dev_dma_type_e;

static uint8_t pcie_timeout_patt [PCIE_TIMEOUT_PATT_SIZE];

static ssize_t _pcie_rw_32 (llio_t *self, uint64_t offs, uint32_t *data, int rw);
static ssize_t _pcie_rw_bar2_block_raw (llio_t *self, uint32_t pg_start, uint64_t pg_offs,
        uint32_t *data, uint32_t size, int rw);
static ssize_t _pcie_rw_bar4_block_raw (llio_t *self, uint32_t pg_start, uint64_t pg_offs,
        uint32_t *data, uint32_t size, int rw);
static ssize_t _pcie_rw_block (llio_t *self, uint64_t offs, size_t size,
        uint32_t *data, int rw);
static uint64_t _pcie_dma_base_addr (llio_t *self, pcie_dev_dma_type_e dma_type);
static int _pcie_configure_dma (llio_t *self, uint64_t device_addr,
    uint64_t next_bda, size_t size, int bar_no, pcie_dev_dma_type_e dma_type, bool block);
static int _pcie_reset_dma (llio_t *self, pcie_dev_dma_type_e dma_type);
static int _pcie_set_dma (llio_t *self, pcie_dev_dma_type_e dma_type,
        uint64_t device_addr, size_t size, int bar_no, uint64_t next_bda);
static int _pcie_wait_dma (llio_t *self, pcie_dev_dma_type_e dma_type, bool block);
static int _pcie_get_dma_status_reg (llio_t *self, pcie_dev_dma_type_e dma_type,
        uint32_t *status);
static int _pcie_check_dma_completion (llio_t *self, pcie_dev_dma_type_e dma_type);
static int _pcie_check_dma_busy (llio_t *self, pcie_dev_dma_type_e dma_type);
static int _pcie_timeout_reset (llio_t *self);
static int _pcie_reset_fpga (llio_t *self);

/************ Our methods implementation **********/

/* Creates a new instance of the dev_pcie */
static llio_dev_pcie_t * llio_dev_pcie_new (const char *dev_entry)
{
    llio_dev_pcie_t *self = (llio_dev_pcie_t *) zmalloc (sizeof *self);
    ASSERT_ALLOC (self, err_llio_dev_pcie_alloc);

    self->dev = (pd_device_t *) zmalloc (sizeof *self->dev);
    ASSERT_ALLOC (self->dev, err_dev_pcie_alloc);
    self->kmem_handle = (pd_kmem_t *) zmalloc (sizeof *self->kmem_handle);
    ASSERT_ALLOC (self->kmem_handle, err_kmem_handle);

    /* FIXME: hardcoded dev number */
    /* TODO: should we use llio_endpoint_get to get the endpoint name? */
    int err = pd_open (0, self->dev, dev_entry);

    if (err != 0) {
        perror ("pd_open");
    }
    ASSERT_TEST(err==0, "Error opening device", err_dev_pcie_open);

    /* Map all available BARs */
    self->bar0 = (uint32_t *) pd_mapBAR (self->dev, BAR0NO);
    ASSERT_TEST(self->bar0!=NULL, "Could not allocate bar0", err_bar0_alloc);
    self->bar2 = (uint32_t *) pd_mapBAR (self->dev, BAR2NO);
    ASSERT_TEST(self->bar2!=NULL, "Could not allocate bar2", err_bar2_alloc);
    DBE_DEBUG (DBG_LL_IO | DBG_LVL_TRACE, "[ll_io_pcie] BAR2 addr = %p\n",
            self->bar2);
    self->bar4 = (uint64_t *) pd_mapBAR (self->dev, BAR4NO);
    ASSERT_TEST(self->bar4!=NULL, "Could not allocate bar4", err_bar4_alloc);
    DBE_DEBUG (DBG_LL_IO | DBG_LVL_TRACE, "[ll_io_pcie] BAR4 addr = %p\n",
            self->bar4);

    self->bar0_size = pd_getBARsize (self->dev, BAR0NO);
    ASSERT_TEST(self->bar0_size > 0, "Could not get bar0 size", err_bar0_size);
    self->bar2_size = pd_getBARsize (self->dev, BAR2NO);
    ASSERT_TEST(self->bar2_size > 0, "Could not get bar2 size", err_bar2_size);
    self->bar4_size = pd_getBARsize (self->dev, BAR4NO);
    ASSERT_TEST(self->bar4_size > 0, "Could not get bar4 size", err_bar4_size);

    /* Initialize PCIE timeout pattern */
    memset (&pcie_timeout_patt, PCIE_TIMEOUT_PATT_INIT, sizeof (pcie_timeout_patt));
    DBE_DEBUG (DBG_LL_IO | DBG_LVL_TRACE, "[ll_io_pcie] Created instance of llio_dev_pcie\n");

    /* Initialize Kernel buffers */
    void *mem = pd_allocKernelMemory(self->dev, PCIE_DMA_KERNEL_BUFF_SIZE, self->kmem_handle);
    ASSERT_ALLOC(mem, err_kernel_alloc);

    return self;

err_kernel_alloc:
err_bar4_size:
err_bar2_size:
err_bar0_size:
    pd_unmapBAR (self->dev, BAR4NO, self->bar4);
err_bar4_alloc:
    pd_unmapBAR (self->dev, BAR2NO, self->bar2);
err_bar2_alloc:
    pd_unmapBAR (self->dev, BAR0NO, self->bar0);
err_bar0_alloc:
    pd_close (self->dev);
err_dev_pcie_open:
    free (self->kmem_handle);
err_kmem_handle:
    free (self->dev);
err_dev_pcie_alloc:
    free (self);
err_llio_dev_pcie_alloc:
    return NULL;
}

/* Destroy an instance of the Endpoint */
static llio_err_e llio_dev_pcie_destroy (llio_dev_pcie_t **self_p)
{
    if (*self_p) {
        llio_dev_pcie_t *self = *self_p;

        /* Free kernel memory */
        pd_freeKernelMemory (self->kmem_handle);

        /* Unmap all bars first and then destroy the remaining structures */
        pd_unmapBAR (self->dev, BAR4NO, self->bar4);
        pd_unmapBAR (self->dev, BAR2NO, self->bar2);
        pd_unmapBAR (self->dev, BAR0NO, self->bar0);
        pd_close (self->dev);

        free (self->kmem_handle);
        free (self->dev);
        free (self);

        self_p = NULL;
    }

    return LLIO_SUCCESS;
}

/************ llio_ops_pcie Implementation **********/

/* Open PCIe device */
static int pcie_open (llio_t *self, llio_endpoint_t *endpoint)
{
    if (llio_get_endpoint_open (self)) {
        /* Device is already opened. So, we return success */
        return 0;
    }

    llio_err_e lerr = LLIO_SUCCESS;
    int err = 0;
    if (endpoint != NULL) {
        lerr = llio_set_endpoint (self, endpoint);
        ASSERT_TEST(lerr == LLIO_SUCCESS, "Could not set endpoint on PCIe device",
                err_endpoint_set, -1);
    }

    /* Create new private PCIe handler */
    llio_dev_pcie_t *dev_pcie = llio_dev_pcie_new (llio_get_endpoint_name (self));
    ASSERT_TEST(dev_pcie != NULL, "Could not allocate dev_handler",
            err_dev_handler_alloc);

    /* Initialize Wishbone and SDRAM pages to 0 */
    SET_SDRAM_PG(dev_pcie->bar0, 0);
    SET_WB_PG(dev_pcie->bar0, 0);

    /* Attach this PCIe device to LLIO instance */
    llio_set_dev_handler (self, dev_pcie);

    /* Signal that the endpoint is opened and ready to work */
    llio_set_endpoint_open (self, true);
    DBE_DEBUG (DBG_LL_IO | DBG_LVL_INFO,
            "[ll_io_pcie] Opened PCIe device located at %s\n",
            llio_get_endpoint_name (self));

    /* Set SDB prefix adress */
    llio_set_sdb_prefix_addr (self, BAR4_ADDR);

    return err;

err_dev_handler_alloc:
err_endpoint_set:
    return err;
}

/* Open PCIe device */
static int pcie_reset (llio_t *self)
{
    int err = 0;
    ASSERT_TEST(llio_get_endpoint_open (self), "Could not reset LLIO. Device is not opened",
            err_endp_open, -1);

    /* Reset FPGA */
    _pcie_reset_fpga (self);
    /* Reset PCIe Timeout */
    _pcie_timeout_reset (self);

    return err;

err_endp_open:
    return err;
}

/* Release PCIe device */
static int pcie_release (llio_t *self, llio_endpoint_t *endpoint)
{
    UNUSED(endpoint);

    if (!llio_get_endpoint_open (self)) {
        /* Nothing to close */
        return 0;
    }

    llio_err_e lerr = LLIO_SUCCESS;
    int err = 0;
    llio_dev_pcie_t *dev_pcie = llio_get_dev_handler (self);
    ASSERT_TEST(dev_pcie != NULL, "Could not get PCIe handler",
            err_dev_pcie_handler, -1);

    /* Deattach specific device handler to generic one */
    lerr = llio_dev_pcie_destroy (&dev_pcie);
    ASSERT_TEST (lerr==LLIO_SUCCESS, "Could not close device appropriately",
            err_dealloc, -1);

    llio_set_dev_handler (self, NULL);
    llio_set_endpoint_open (self, false);

    DBE_DEBUG (DBG_LL_IO | DBG_LVL_INFO,
            "[ll_io_pcie] Closed PCIe device located at %s\n",
            llio_get_endpoint_name (self));

    return err;

err_dealloc:
err_dev_pcie_handler:
    return err;
}

/* NOTE: For the PCIe device, we only have up to 28 address bits, as
 * the 4 MSB are reserved for selecting the BAR to operate on */

/* Read data from PCIe device */
static ssize_t pcie_read_32 (llio_t *self, uint64_t offs, uint32_t *data)
{
    return _pcie_rw_32 (self, offs, data, READ_FROM_BAR);
}

static ssize_t pcie_read_64 (llio_t *self, uint64_t offs, uint64_t *data)
{
    return _pcie_rw_32 (self, offs,
            (uint32_t *) data, READ_FROM_BAR)
            +
            _pcie_rw_32 (self, offs,
            (uint32_t *)((uint8_t *) data + sizeof (uint32_t)), READ_FROM_BAR);
}

/* Write data to PCIe device */
static ssize_t pcie_write_32 (llio_t *self, uint64_t offs, const uint32_t *data)
{
    uint32_t _data = *data;
    return _pcie_rw_32 (self, offs, &_data, WRITE_TO_BAR);
}

static ssize_t pcie_write_64 (llio_t *self, uint64_t offs, const uint64_t *data)
{
    uint64_t _data = *data;
    return _pcie_rw_32 (self, offs,
            (uint32_t *) &_data, WRITE_TO_BAR)
            +
            _pcie_rw_32 (self, offs,
            (uint32_t *)((uint8_t *) &_data + sizeof (uint32_t)), WRITE_TO_BAR);
}

/* Read data block from PCIe device, size in bytes */
static ssize_t pcie_read_block (llio_t *self, uint64_t offs, size_t size, uint32_t *data)
{
    return _pcie_rw_block (self, offs, size, data, READ_FROM_BAR);
}

/* Write data block from PCIe device, size in bytes */
static ssize_t pcie_write_block (llio_t *self, uint64_t offs, size_t size, uint32_t *data)
{
    /* _pcie_rw_block with WRITE_TO_BAR does not modify "data" */
    return _pcie_rw_block (self, offs, size, data, WRITE_TO_BAR);
}

/* Read data block from PCIe device, size in bytes */
static ssize_t pcie_read_dma (llio_t *self, uint64_t offs, size_t size, uint32_t *data)
{
    assert (self);
    int err = sizeof (*data);
    size_t size_clipped = size;
    ASSERT_TEST(llio_get_endpoint_open (self), "Could not perform RW operation. Device is not opened",
            err_endp_open, -1);

    /* Maximum DMA block size */
    if (size > PCIE_DMA_KERNEL_BUFF_SIZE) {
        DBE_DEBUG (DBG_LL_IO | DBG_LVL_TRACE, "[pcie_read_dma] DMA size transaction is too large. "
                "Clipping it to the maximum of %u bytes\n", PCIE_DMA_KERNEL_BUFF_SIZE);
        size_clipped = PCIE_DMA_KERNEL_BUFF_SIZE;
    }

    llio_dev_pcie_t *dev_pcie = llio_get_dev_handler (self);
    ASSERT_TEST(dev_pcie != NULL, "Could not get PCIe handler",
            err_dev_pcie_handler, -1);

    /* Determine which bar to operate on */
    int bar_no = PCIE_ADDR_BAR (offs);
    uint64_t full_offs = PCIE_ADDR_GEN (offs);

    err = _pcie_configure_dma (self, full_offs, 0, size_clipped, bar_no, PCIE_DMA_US, true);
    ASSERT_TEST(err == 0, "Could not configure DMA", err_configure_dma, -1);

    /* Copy kernel memory to user buffer */
    memcpy (data, dev_pcie->kmem_handle->mem, size_clipped);
    err = size_clipped;
    return err;

err_configure_dma:
err_dev_pcie_handler:
err_endp_open:
    return -1;
}

/* Write data block from PCIe device, size in bytes */
static ssize_t pcie_write_dma (llio_t *self, uint64_t offs, size_t size, uint32_t *data)
{
    assert (self);
    int err = sizeof (*data);
    size_t size_clipped = size;
    ASSERT_TEST(llio_get_endpoint_open (self), "Could not perform RW operation. Device is not opened",
            err_endp_open, -1);

    /* Maximum DMA block size */
    if (size > PCIE_DMA_KERNEL_BUFF_SIZE) {
        DBE_DEBUG (DBG_LL_IO | DBG_LVL_TRACE, "[pcie_read_dma] DMA size transaction is too large. "
                "Clipping it to the maximum of %u bytes\n", PCIE_DMA_KERNEL_BUFF_SIZE);
        size_clipped = PCIE_DMA_KERNEL_BUFF_SIZE;
    }

    llio_dev_pcie_t *dev_pcie = llio_get_dev_handler (self);
    ASSERT_TEST(dev_pcie != NULL, "Could not get PCIe handler",
            err_dev_pcie_handler, -1);

    /* Determine which bar to operate on */
    int bar_no = PCIE_ADDR_BAR (offs);
    uint64_t full_offs = PCIE_ADDR_GEN (offs);

    /* Copy user buffer to kernel memory */
    memcpy (dev_pcie->kmem_handle->mem, data, size_clipped);

    err = _pcie_configure_dma (self, full_offs, 0, size_clipped, bar_no, PCIE_DMA_DS, true);
    ASSERT_TEST(err == 0, "Could not configure DMA", err_configure_dma, -1);
    err = size_clipped;
    return err;

err_configure_dma:
err_dev_pcie_handler:
err_endp_open:
    return -1;
}

/* Read PCIe device information */
/*static int pcie_read_info (llio_t *self, llio_dev_info_t *dev_info)
{
    UNUSED(self);
    UNUSED(dev_info);
    return -1;
}
*/

/************ Helper functions **********/
static ssize_t _pcie_rw_32 (llio_t *self, uint64_t offs, uint32_t *data, int rw)
{
    assert (self);
    int err = sizeof (*data);
    ASSERT_TEST(llio_get_endpoint_open (self), "Could not perform RW operation. Device is not opened",
            err_endp_open, -1);

    llio_dev_pcie_t *dev_pcie = llio_get_dev_handler (self);
    ASSERT_TEST(dev_pcie != NULL, "Could not get PCIe handler",
            err_dev_pcie_handler, -1);

    /* Determine which bar to operate on */
    int bar_no = PCIE_ADDR_BAR (offs);
    uint64_t full_offs = PCIE_ADDR_GEN (offs);
    int pg_num;
    uint64_t pg_offs;

    /* FIXME: This switch is in the critical path! Remove it */
    switch (bar_no) {
        /* PCIe config registers */
        case BAR0NO:
            DBE_DEBUG (DBG_LL_IO | DBG_LVL_TRACE,
                    "----------------------------------------------------------\n"
                    "[ll_io_pcie:_pcie_rw_32] Going to read/write in BAR0\n");
            DBE_DEBUG (DBG_LL_IO | DBG_LVL_TRACE,
                    "[ll_io_pcie:_pcie_rw_32] bar_no = %d, full_offs = %"PRIX64"\n"
                    "-------------------------------------------------------------------------------------\n",
                    bar_no, full_offs);
            BAR0_RW(dev_pcie->bar0, full_offs, data, rw);
            break;

        /* FPGA SDRAM */
        case BAR2NO:
            DBE_DEBUG (DBG_LL_IO | DBG_LVL_TRACE,
                    "----------------------------------------------------------\n"
                    "[ll_io_pcie:_pcie_rw_32] Going to read/write in BAR2\n");
            pg_num = PCIE_ADDR_SDRAM_PG (full_offs);
            pg_offs = PCIE_ADDR_SDRAM_PG_OFFS (full_offs);
            SET_SDRAM_PG (dev_pcie->bar0, pg_num);
            DBE_DEBUG (DBG_LL_IO | DBG_LVL_TRACE,
                    "[ll_io_pcie:_pcie_rw_32] bar_no = %d, pg_num  = %d,\n\tfull_offs = 0x%"PRIX64", pg_offs = 0x%"PRIX64"\n",
                    bar_no, pg_num, full_offs, pg_offs);
            DBE_DEBUG (DBG_LL_IO | DBG_LVL_TRACE,
                    "[ll_io_pcie:_pcie_rw_32] full_addr = 0x%p\n"
                    "-------------------------------------------------------------------------------------\n",
                    ((llio_dev_pcie_t *) llio_get_dev_handler (self))->bar2 + pg_offs);
            BAR2_RW(dev_pcie->bar2, pg_offs, data, rw);
            break;

        /* FPGA Wishbone */
        case BAR4NO:
            DBE_DEBUG (DBG_LL_IO | DBG_LVL_TRACE,
                    "----------------------------------------------------------\n"
                    "[ll_io_pcie:_pcie_rw_32] Going to read/write in BAR4\n");
            pg_num = PCIE_ADDR_WB_PG (full_offs);
            pg_offs = PCIE_ADDR_WB_PG_OFFS (full_offs);
            SET_WB_PG (dev_pcie->bar0, pg_num);
            DBE_DEBUG (DBG_LL_IO | DBG_LVL_TRACE,
                    "[ll_io_pcie:_pcie_rw_32] bar_no = %d, pg_num  = %d,\n\tfull_offs = 0x%"PRIX64", pg_offs = 0x%"PRIX64"\n",
                    bar_no, pg_num, full_offs, pg_offs);
            DBE_DEBUG (DBG_LL_IO | DBG_LVL_TRACE,
                    "[ll_io_pcie:_pcie_rw_32] full_addr = %p\n"
                    "-------------------------------------------------------------------------------------\n",
                    ((llio_dev_pcie_t *) llio_get_dev_handler (self))->bar4 + pg_offs);
            BAR4_RW(dev_pcie->bar4, pg_offs, data, rw);
            break;

        /* Invalid BAR */
        default:
            DBE_DEBUG (DBG_LL_IO | DBG_LVL_ERR,
                    "----------------------------------------------------------\n"
                    "[ll_io_pcie:_pcie_rw_32] Invalid BAR access\n");
            return -1;
    }

err_dev_pcie_handler:
err_endp_open:
    return err;
}

static ssize_t _pcie_rw_bar2_block_raw (llio_t *self, uint32_t pg_start, uint64_t pg_offs,
        uint32_t *data, uint32_t size, int rw)
{
    uint32_t offs = pg_offs;
    uint32_t num_bytes_rem = size;
    /* Number of bytes read/written, for faster comparison */
    uint32_t num_bytes_rw = 0;
    ssize_t err = 0;
    uint32_t *datap = data;

    llio_dev_pcie_t *dev_pcie = llio_get_dev_handler (self);
    ASSERT_TEST(dev_pcie != NULL, "Could not get PCIe handler",
            err_dev_pcie_handler, -1);
    uint32_t bar_size = dev_pcie->bar2_size;

    DBE_DEBUG (DBG_LL_IO | DBG_LVL_TRACE,
            "----------------------------------------------------------\n"
            "[ll_io_pcie:_pcie_rw_bar2_block_raw] pg_start = %u, pg_end = %"PRIu64"...\n",
            pg_start, pg_start + (pg_offs+size)/bar_size + 1);
    for (unsigned int pg = pg_start;
            pg < (pg_start + (pg_offs+size)/bar_size + 1);
            ++pg) {
        SET_SDRAM_PG (dev_pcie->bar0, pg);
        uint32_t num_bytes_page = (offs + num_bytes_rem > bar_size) ?
            (bar_size-offs) : (num_bytes_rem);
        num_bytes_rem -= num_bytes_page;
        num_bytes_rw += num_bytes_page;

        DBE_DEBUG (DBG_LL_IO | DBG_LVL_TRACE,
            "[ll_io_pcie:_pcie_rw_bar2_block_raw] offs = %u, num_bytes_rem = %u,\n"
            "bar_size = %u, num_byte_page = %u\n", offs, num_bytes_rem, bar_size,
            num_bytes_page);
        DBE_DEBUG (DBG_LL_IO | DBG_LVL_TRACE,
                "[ll_io_pcie:_pcie_rw_bar2_block_raw] Reading %u bytes from addr: %p\n"
                "-------------------------------------------------------------------------------------\n",
                num_bytes_page, dev_pcie->bar2);
        BAR2_RW_BLOCK(dev_pcie->bar2, offs, num_bytes_page,
                datap, rw);
        datap = (uint32_t *)((uint8_t *)datap + num_bytes_page);

        /* Always 0 after the first page */
        offs = 0;
    }

    err = num_bytes_rw;
err_dev_pcie_handler:
    return err;
}

/* Read/Write BAR2 block with timeout detection */
static ssize_t _pcie_rw_bar2_block_td (llio_t *self, uint32_t pg_start, uint64_t pg_offs,
        uint32_t *data, uint32_t size, int rw)
{
    size_t num_bytes_rw = 0;

    uint32_t i;
    for (i = 0; i < PCIE_TIMEOUT_MAX_TRIES; ++i) {
        num_bytes_rw = _pcie_rw_bar2_block_raw (self, pg_start, pg_offs, data,
                size, rw);

        /* If sufficient number of bytes read, try to detect a PCIe core
         * timeout by reading specified number of words and comparing to
         * the timeout pattern */
        if (num_bytes_rw >= PCIE_TIMEOUT_PATT_SIZE && !memcmp (data,
                    pcie_timeout_patt, PCIE_TIMEOUT_PATT_SIZE)) {
            DBE_DEBUG (DBG_LL_IO | DBG_LVL_TRACE,
                    "[ll_io_pcie:_pcie_rw_bar2_td] Timeout detected. Retrying\n");
            _pcie_timeout_reset (self);
            usleep (PCIE_TIMEOUT_WAIT);
        }
        else {
            break;
        }
    }

    if (i >= PCIE_TIMEOUT_MAX_TRIES) {
        DBE_DEBUG (DBG_LL_IO | DBG_LVL_ERR,
                "[ll_io_pcie:_pcie_rw_bar2_td] Unrecoverable timeout detected. Exceeded "
                "maximum number of tries\n");
        return -1;
    }

    return num_bytes_rw;
}

static ssize_t _pcie_rw_bar4_block_raw (llio_t *self, uint32_t pg_start, uint64_t pg_offs,
        uint32_t *data, uint32_t size, int rw)
{
    uint32_t offs = pg_offs;
    uint32_t num_bytes_rem = size;
    /* Number of bytes read/written, for faster comparison */
    uint32_t num_bytes_rw = 0;
    ssize_t err = 0;

    llio_dev_pcie_t *dev_pcie = llio_get_dev_handler (self);
    ASSERT_TEST(dev_pcie != NULL, "Could not get PCIe handler",
            err_dev_pcie_handler, -1);
    uint32_t bar_size = dev_pcie->bar4_size;

    DBE_DEBUG (DBG_LL_IO | DBG_LVL_TRACE,
            "----------------------------------------------------------\n"
            "[ll_io_pcie:_pcie_rw_bar4_block_raw] pg_start = %u, pg_end = %"PRIu64"...\n",
            pg_start, pg_start + (pg_offs+size)/bar_size + 1);
    for (unsigned int pg = pg_start;
            pg < pg_start + (pg_offs+size)/bar_size + 1;
            ++pg) {
        SET_WB_PG (((volatile uint32_t *)dev_pcie->bar0), pg);
        uint32_t num_bytes_page = (num_bytes_rem > bar_size) ?
            (bar_size-offs) : (num_bytes_rem);
        num_bytes_rem -= num_bytes_page;
        num_bytes_rw += num_bytes_page;

        DBE_DEBUG (DBG_LL_IO | DBG_LVL_TRACE,
                "[ll_io_pcie:_pcie_rw_bar4_block_raw] Reading %u bytes from addr: %p\n"
                "-------------------------------------------------------------------------------------\n",
                num_bytes_page, dev_pcie->bar4);
        BAR4_RW_BLOCK(((volatile uint64_t *)dev_pcie->bar4), offs, num_bytes_page,
                (uint32_t *)((uint8_t *)data + (pg-pg_start)*bar_size), rw);

        /* Always 0 after the first page */
        offs = 0;
    }

    err = num_bytes_rw;
err_dev_pcie_handler:
    return err;
}

/* Read/Write BAR4 block with timeout detection.
 * FIXME: Reduce code repetition. _pcie_rw_bar2_block_td () is almost the same */
static ssize_t _pcie_rw_bar4_block_td (llio_t *self, uint32_t pg_start, uint64_t pg_offs,
        uint32_t *data, uint32_t size, int rw)
{
    size_t num_bytes_rw = 0;

    uint32_t i;
    for (i = 0; i < PCIE_TIMEOUT_MAX_TRIES; ++i) {
        num_bytes_rw = _pcie_rw_bar4_block_raw (self, pg_start, pg_offs, data,
                size, rw);

        /* If sufficient number of bytes read, try to detect a PCIe core
         * timeout by reading specified number of words and comparing to
         * the timeout pattern */
        if (num_bytes_rw >= PCIE_TIMEOUT_PATT_SIZE && !memcmp (data,
                    pcie_timeout_patt, PCIE_TIMEOUT_PATT_SIZE)) {
            DBE_DEBUG (DBG_LL_IO | DBG_LVL_TRACE,
                    "[ll_io_pcie:_pcie_rw_bar4_td] Timeout detected. Retrying\n");
            _pcie_timeout_reset (self);
            usleep (PCIE_TIMEOUT_WAIT);
        }
        else {
            break;
        }
    }

    if (i >= PCIE_TIMEOUT_MAX_TRIES) {
        DBE_DEBUG (DBG_LL_IO | DBG_LVL_ERR,
                "[ll_io_pcie:_pcie_rw_bar4_td] Unrecoverable timeout detected. Exceeded "
                "maximum number of tries\n");
        return -1;
    }

    return num_bytes_rw;
}

static ssize_t _pcie_rw_block (llio_t *self, uint64_t offs, size_t size, uint32_t *data, int rw)
{
    assert (self);
    int err = 0;
    ASSERT_TEST(llio_get_endpoint_open (self), "Could not perform RW operation. Device is not opened",
            err_endp_open, -1);

    llio_dev_pcie_t *dev_pcie = llio_get_dev_handler (self);
    ASSERT_TEST(dev_pcie != NULL, "Could not get PCIe handler",
            err_dev_pcie_handler, -1);

    /* Determine which bar to operate on */
    int bar_no = PCIE_ADDR_BAR (offs);
    uint64_t full_offs = PCIE_ADDR_GEN (offs);
    int pg_start;
    uint64_t pg_offs;

    switch (bar_no) {
        /* PCIe config registers */
        case BAR0NO:
            /* Not available */
            break;

        /* FPGA SDRAM */
        case BAR2NO:
            DBE_DEBUG (DBG_LL_IO | DBG_LVL_TRACE,
                    "----------------------------------------------------------\n"
                    "[ll_io_pcie:_pcie_rw_block] Going to read/write in BAR2\n");
            pg_start = PCIE_ADDR_SDRAM_PG (full_offs);
            pg_offs = PCIE_ADDR_SDRAM_PG_OFFS (full_offs);
            DBE_DEBUG (DBG_LL_IO | DBG_LVL_TRACE,
                    "[ll_io_pcie:_pcie_rw_block] bar_no = %d, pg_start = %d,\n\tfull_offs = 0x%"PRIX64", pg_offs = 0x%"PRIX64"\n",
                    bar_no, pg_start, full_offs, pg_offs);
            DBE_DEBUG (DBG_LL_IO | DBG_LVL_TRACE,
                    "[ll_io_pcie:_pcie_rw_block] full_addr = 0x%p\n"
                    "-------------------------------------------------------------------------------------\n",
                    dev_pcie->bar2 + pg_offs);
            err = _pcie_rw_bar2_block_td (self, pg_start, pg_offs, data, size, rw);
            break;

        /* FPGA Wishbone */
        case BAR4NO:
            DBE_DEBUG (DBG_LL_IO | DBG_LVL_TRACE,
                    "----------------------------------------------------------\n"
                    "[ll_io_pcie:_pcie_rw_block] Going to read/write in BAR4\n");
            pg_start = PCIE_ADDR_WB_PG (full_offs);
            pg_offs = PCIE_ADDR_WB_PG_OFFS (full_offs);
            DBE_DEBUG (DBG_LL_IO | DBG_LVL_TRACE,
                    "[ll_io_pcie:_pcie_rw_block] bar_no = %d, pg_start  = %d,\n\tfull_offs = 0x%"PRIX64", pg_offs = 0x%"PRIX64"\n",
                    bar_no, pg_start, full_offs, pg_offs);
            DBE_DEBUG (DBG_LL_IO | DBG_LVL_TRACE,
                    "[ll_io_pcie:_pcie_rw_block] full_addr = %p\n"
                    "-------------------------------------------------------------------------------------\n",
                    dev_pcie->bar4 + pg_offs);
            err =  _pcie_rw_bar4_block_td (self, pg_start, pg_offs, data, size, rw);
            break;

        /* Invalid BAR */
        default:
            DBE_DEBUG (DBG_LL_IO | DBG_LVL_ERR,
                    "----------------------------------------------------------\n"
                    "[ll_io_pcie:_pcie_rw_32] Invalid BAR access\n");
            break;
    }

err_dev_pcie_handler:
err_endp_open:
    return err;
}

static uint64_t _pcie_dma_base_addr (llio_t *self, pcie_dev_dma_type_e dma_type)
{
    UNUSED(self);

    uint64_t dma_base_addr = 0;
    switch (dma_type) {
        case PCIE_DMA_US:
            dma_base_addr = PCIE_CFG_REG_DMA_US_BASE;
        break;

        case PCIE_DMA_DS:
            dma_base_addr = PCIE_CFG_REG_DMA_DS_BASE;
        break;

        default:
            dma_base_addr = PCIE_CFG_REG_DMA_US_BASE;
    }

    return dma_base_addr;
}


static int _pcie_configure_dma (llio_t *self, uint64_t device_addr,
    uint64_t next_bda, size_t size, int bar_no, pcie_dev_dma_type_e dma_type, bool block)
{
    DBE_DEBUG (DBG_LL_IO | DBG_LVL_TRACE,
            "[ll_io_pcie:_pcie_configure_dma] Configuring DMA\n");

    /* Before starting a new DMA transaction we need to check if
     * it's not busy */
    int err = _pcie_check_dma_busy (self, dma_type);
    ASSERT_TEST(err == 0, "DMA is busy. Aborting DMA transaction",
            err_busy_dma);

    err = _pcie_reset_dma (self, dma_type);
    ASSERT_TEST(err == 0, "Could not reset DMA", err_reset_dma);

    err = _pcie_set_dma (self, dma_type, device_addr, size, bar_no, next_bda);
    ASSERT_TEST(err == 0, "Could not set DMA", err_set_dma);

    err = _pcie_wait_dma (self, dma_type, block);
    ASSERT_TEST(err == 0, "Could not wait DMA", err_wait_dma);

err_wait_dma:
err_set_dma:
err_reset_dma:
err_busy_dma:
    return err;
}

static int _pcie_reset_dma (llio_t *self, pcie_dev_dma_type_e dma_type)
{
    DBE_DEBUG (DBG_LL_IO | DBG_LVL_TRACE,
            "[ll_io_pcie:_pcie_reset_dma] Reseting DMA engine\n");

    uint64_t dma_base_addr = BAR0_ADDR | _pcie_dma_base_addr (self, dma_type);
    uint64_t offs = dma_base_addr + PCIE_CFG_REG_DMA_CTRL;
    /* We must write the reset pattern + the valid bit */
    uint32_t data = PCIE_CFG_DMA_CTRL_VALID | PCIE_CFG_TX_CTRL_CHANNEL_RST;
    return (_pcie_rw_32 (self, offs, &data, WRITE_TO_BAR) == sizeof (uint32_t) ? 0 : 1);
}

static int _pcie_set_dma (llio_t *self, pcie_dev_dma_type_e dma_type,
        uint64_t device_addr, size_t size, int bar_no, uint64_t next_bda)
{
    DBE_DEBUG (DBG_LL_IO | DBG_LVL_TRACE,
            "[ll_io_pcie:_pcie_reset_dma] Setting DMA engine\n");

    int err = 0;
    llio_dev_pcie_t *dev_pcie = llio_get_dev_handler (self);
    ASSERT_TEST(dev_pcie != NULL, "Could not get PCIe handler",
            err_dev_pcie_handler, -1);
    uint64_t dma_base_addr = BAR0_ADDR | _pcie_dma_base_addr (self, dma_type);

    uint32_t data = PCIE_CFG_DMA_PAH_R(device_addr);
    err |= _pcie_rw_32 (self, dma_base_addr + PCIE_CFG_REG_DMA_PAH, &data, WRITE_TO_BAR)
            == sizeof (uint32_t) ? 0 : 1;
    data = PCIE_CFG_DMA_PAL_R(device_addr);
    err |= _pcie_rw_32 (self, dma_base_addr + PCIE_CFG_REG_DMA_PAL, &data, WRITE_TO_BAR)
            == sizeof (uint32_t) ? 0 : 1;

    data = PCIE_CFG_DMA_HAH_R(dev_pcie->kmem_handle->pa);
    err |= _pcie_rw_32 (self, dma_base_addr + PCIE_CFG_REG_DMA_HAH, &data, WRITE_TO_BAR)
            == sizeof (uint32_t) ? 0 : 1;
    data = PCIE_CFG_DMA_HAL_R(dev_pcie->kmem_handle->pa);
    err |= _pcie_rw_32 (self, dma_base_addr + PCIE_CFG_REG_DMA_HAL, &data, WRITE_TO_BAR)
            == sizeof (uint32_t) ? 0 : 1;

    data = PCIE_CFG_DMA_BDAH_R(next_bda);
    err |= _pcie_rw_32 (self, dma_base_addr + PCIE_CFG_REG_DMA_BDAH, &data, WRITE_TO_BAR)
            == sizeof (uint32_t) ? 0 : 1;
    data = PCIE_CFG_DMA_BDAL_R(next_bda);
    err |= _pcie_rw_32 (self, dma_base_addr + PCIE_CFG_REG_DMA_BDAL, &data, WRITE_TO_BAR)
            == sizeof (uint32_t) ? 0 : 1;

    data = PCIE_CFG_DMA_LENG_R(size);
    err |= _pcie_rw_32 (self, dma_base_addr + PCIE_CFG_REG_DMA_LENG, &data, WRITE_TO_BAR)
            == sizeof (uint32_t) ? 0 : 1;

    /* Write Control register at the end, as it starts the DMA */
    data = PCIE_CFG_DMA_CTRL_VALID | PCIE_CFG_DMA_CTRL_LAST | \
            PCIE_CFG_DMA_CTRL_AINC | PCIE_CFG_DMA_CTRL_BAR_W(bar_no);
    err |= _pcie_rw_32 (self, dma_base_addr + PCIE_CFG_REG_DMA_CTRL, &data, WRITE_TO_BAR)
            == sizeof (uint32_t) ? 0 : 1;

    ASSERT_TEST(err == 0, "Could not set DMA registers", err_set_dma_regs);
    return err;

err_set_dma_regs:
err_dev_pcie_handler:
    return err;
}

static int _pcie_get_dma_status_reg (llio_t *self, pcie_dev_dma_type_e dma_type,
        uint32_t *status)
{
    DBE_DEBUG (DBG_LL_IO | DBG_LVL_TRACE,
            "[ll_io_pcie:_pcie_get_dma_status_reg] Getting DMA status register\n");

    uint64_t dma_base_addr = BAR0_ADDR | _pcie_dma_base_addr (self, dma_type);
    int err = 0;

    uint32_t data = 0;
    err |= _pcie_rw_32 (self, dma_base_addr + PCIE_CFG_REG_DMA_STA, &data, READ_FROM_BAR)
            == sizeof (uint32_t) ? 0 : 1;
    ASSERT_TEST(err == 0, "Could not read DMA status register", err_read_dma_reg,
            -1);
    DBE_DEBUG (DBG_LL_IO | DBG_LVL_TRACE,
            "[ll_io_pcie:_pcie_get_dma_status_reg] DMA status = 0x%08X\n", data);

    *status = data;
    return err;

err_read_dma_reg:
    return err;
}

static int _pcie_check_dma_completion (llio_t *self, pcie_dev_dma_type_e dma_type)
{
    DBE_DEBUG (DBG_LL_IO | DBG_LVL_TRACE,
            "[ll_io_pcie:_pcie_check_dma_completion] Getting DMA completion status\n");

    uint32_t status = 0;
    int err = _pcie_get_dma_status_reg (self, dma_type, &status);
    ASSERT_TEST(err == 0, "Could not check DMA completion bit", err_read_dma_reg,
            -1);

    /* Check for status done bit */
    if (status & PCIE_CFG_DMA_STA_DONE) {
        DBE_DEBUG (DBG_LL_IO | DBG_LVL_TRACE,
                "[ll_io_pcie:_pcie_check_dma_completion] DMA engine done\n");
        return 0;
    }

    /* Check for timeout bit */
    if (status & PCIE_CFG_DMA_STA_TIMEOUT) {
        DBE_DEBUG (DBG_LL_IO | DBG_LVL_TRACE,
                "[ll_io_pcie:_pcie_check_dma_completion] DMA engine timeout\n");
        return -1;
    }

err_read_dma_reg:
    return err;
}

static int _pcie_check_dma_busy (llio_t *self, pcie_dev_dma_type_e dma_type)
{
    DBE_DEBUG (DBG_LL_IO | DBG_LVL_TRACE,
            "[ll_io_pcie:_pcie_check_dma_busy] Getting DMA busy status\n");

    uint32_t status = 0;
    int err = _pcie_get_dma_status_reg (self, dma_type, &status);
    ASSERT_TEST(err == 0, "Could not check DMA busy bit", err_read_dma_reg,
            -1);

    /* Check for status done bit */
    if (status & PCIE_CFG_DMA_STA_BUSY) {
        DBE_DEBUG (DBG_LL_IO | DBG_LVL_TRACE,
                "[ll_io_pcie:_pcie_check_dma_busy] DMA engine busy\n");
        return -1;
    }

    DBE_DEBUG (DBG_LL_IO | DBG_LVL_TRACE,
            "[ll_io_pcie:_pcie_check_dma_busy] DMA engine not busy\n");
    return 0;

err_read_dma_reg:
    return err;
}

static int _pcie_wait_dma (llio_t *self, pcie_dev_dma_type_e dma_type, bool block)
{
    DBE_DEBUG (DBG_LL_IO | DBG_LVL_TRACE,
            "[ll_io_pcie:_pcie_wait_dma] Waiting for DMA engine\n");
    int err = 0;
    /*FIXME. Should we insert a maximum number of wait tries? */
    /* FIXME 2: For now, we must check the DONE and BUSY bits.
     * The DONE bit is always 1, so maybe we need to check
     * FPGA reset signal */
    while ((err = (_pcie_check_dma_completion (self, dma_type) ||
                   _pcie_check_dma_busy (self, dma_type))) && block);

    return err;
}

static int _pcie_timeout_reset (llio_t *self)
{
    DBE_DEBUG (DBG_LL_IO | DBG_LVL_TRACE,
            "[ll_io_pcie:_pcie_timeout_reset] Reseting timeout\n");

    uint64_t offs = BAR0_ADDR | PCIE_CFG_REG_TX_CTRL;
    uint32_t data = PCIE_CFG_TX_CTRL_CHANNEL_RST;
    return (_pcie_rw_32 (self, offs, &data, WRITE_TO_BAR) == sizeof (uint32_t) ? 0 : 1);
}

static int _pcie_reset_fpga (llio_t *self)
{
    DBE_DEBUG (DBG_LL_IO | DBG_LVL_TRACE,
            "[ll_io_pcie:_pcie_reset_fpga] Reseting FPGA\n");

    uint64_t offs = BAR0_ADDR | PCIE_CFG_REG_EB_STACON;
    uint32_t data = PCIE_CFG_TX_CTRL_CHANNEL_RST;
    return (_pcie_rw_32 (self, offs, &data, WRITE_TO_BAR) ==  sizeof (uint32_t) ? 0 : 1);
}

const llio_ops_t llio_ops_pcie = {
    .name           = "PCIE",           /* Operations name */
    .open           = pcie_open,        /* Open device */
    .reset          = pcie_reset,       /* Reset device */
    .release        = pcie_release,     /* Release device */
    .read_16        = NULL,             /* Read 16-bit data */
    .read_32        = pcie_read_32,     /* Read 32-bit data */
    .read_64        = pcie_read_64,     /* Read 64-bit data */
    .write_16       = NULL,             /* Write 16-bit data */
    .write_32       = pcie_write_32,    /* Write 32-bit data */
    .write_64       = pcie_write_64,    /* Write 64-bit data */
    .read_block     = pcie_read_block,  /* Read arbitrary block size data,
                                           parameter size in bytes */
    .write_block    = pcie_write_block, /* Write arbitrary block size data,
                                           parameter size in bytes */
    .read_dma       = pcie_read_dma,    /* Read arbitrary block size data via DMA,
                                            parameter size in bytes */
    .write_dma      = pcie_write_dma    /* Write arbitrary block size data via DMA,
                                            parameter size in bytes */
    /*.read_info      = pcie_read_info */   /* Read device information data */
};
