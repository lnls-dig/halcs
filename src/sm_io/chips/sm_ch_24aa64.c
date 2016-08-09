/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

/* This file is based on the work of Andrzej Wojenski for the FCS project,
 * available, to my knowledge, only in https://github.com/lerwys/fcs
 *
 * Description: Software driver for EEPROM 24AA64 chip
 */

#include "bpm_server.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, SM_CH, "[sm_ch:24aa64]",          \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)       \
    ASSERT_HAL_ALLOC(ptr, SM_CH, "[sm_ch:24aa64]",                  \
            smch_err_str(SMCH_ERR_ALLOC),                           \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                    \
    CHECK_HAL_ERR(err, SM_CH, "[sm_ch:24aa64]",                     \
            smch_err_str (err_type))

#define SMCH_24AA64_WAIT_TRIES              10
#define SMCH_24AA64_NAME                    "24AA64"
#define SMCH_24AA64_USECS_WAIT              10000
#define SMCH_24AA64_WAIT(usecs)             usleep(usecs)
#define SMCH_24AA64_WAIT_DFLT               SMCH_24AA64_WAIT(SMCH_24AA64_USECS_WAIT)

struct _smch_24aa64_t {
    smpr_t *proto;                    /* PROTO protocol object */
};

static ssize_t _smch_24aa64_write_generic (smch_24aa64_t *self, uint16_t addr,
        const uint8_t *data, size_t size);
static ssize_t _smch_24aa64_read_generic (smch_24aa64_t *self, uint16_t addr,
        uint8_t *data, size_t size);

/* Creates a new instance of the SMCH 24AA64 */
smch_24aa64_t * smch_24aa64_new (smio_t *parent, uint64_t base,
        const smpr_proto_ops_t *reg_ops, int verbose)
{
    (void) verbose;
    assert (parent);

    smch_24aa64_t *self = (smch_24aa64_t *) zmalloc (sizeof *self);
    ASSERT_ALLOC(self, err_self_alloc);

    self->proto = smpr_new (SMCH_24AA64_NAME, parent, reg_ops, verbose);
    ASSERT_ALLOC(self->proto, err_proto_alloc);

    /* Initalize the PROTO protocol */
    int smpr_err = smpr_open (self->proto, base, NULL /* Default parameters are fine */);
    ASSERT_TEST(smpr_err == 0, "Could not initialize SMPR protocol", err_smpr_init);

    DBE_DEBUG (DBG_SM_CH | DBG_LVL_INFO, "[sm_ch:24aa64] Created instance of SMCH\n");
    return self;

err_smpr_init:
    smpr_destroy (&self->proto);
err_proto_alloc:
    free (self);
err_self_alloc:
    return NULL;
}

/* Destroy an instance of the SMCH 24AA64 */
smch_err_e smch_24aa64_destroy (smch_24aa64_t **self_p)
{
    assert (self_p);

    if (*self_p) {
        smch_24aa64_t *self = *self_p;

        smpr_release (self->proto);
        smpr_destroy (&self->proto);
        free (self);
        *self_p = NULL;
    }

    return SMCH_SUCCESS;
}

smch_err_e smch_24aa64_write_8 (smch_24aa64_t *self, uint16_t addr,
        const uint8_t *data)
{
    return (_smch_24aa64_write_generic (self, addr, data, sizeof(*data)) ==
            sizeof(*data))? SMCH_SUCCESS : SMCH_ERR_RW_SMPR;
}

smch_err_e smch_24aa64_read_8 (smch_24aa64_t *self, uint16_t addr,
        uint8_t *data)
{
    return (_smch_24aa64_read_generic (self, addr, data, sizeof(*data)) ==
            sizeof(*data))? SMCH_SUCCESS : SMCH_ERR_RW_SMPR;
}

smch_err_e smch_24aa64_write_block (smch_24aa64_t *self, uint16_t addr,
        const uint32_t *data, size_t size)
{
    ssize_t ret = _smch_24aa64_write_generic (self, addr,
            (const uint8_t *) data, size);
    return (ret >= 0 && (size_t) ret == size)? SMCH_SUCCESS : SMCH_ERR_RW_SMPR;
}

smch_err_e smch_24aa64_read_block (smch_24aa64_t *self, uint16_t addr,
        uint32_t *data, size_t size)
{
    ssize_t ret = _smch_24aa64_read_generic (self, addr, (uint8_t *) data, size);
    return (ret >= 0 && (size_t) ret == size)? SMCH_SUCCESS : SMCH_ERR_RW_SMPR;
}

/***************** Static functions *****************/

static ssize_t _smch_24aa64_write_generic (smch_24aa64_t *self, uint16_t addr,
        const uint8_t *data, size_t size)
{
    ssize_t err = 0;

    ASSERT_TEST(size < E24AA64_PAGE_DATA_BYTES_MAX/* in bytes */,
            "Transaction size too big. Maximum of 32 bytes.", err_smpr_write, -1);

    /* Single byte write with 16-bit addresses (15-13 are don't care, 12-0 are
     * valid addresses bits) and 8-bit data:
     *
     * Source: 24AA64/24LC64 DS21189F datasheet, page 8
     */

    /* 24AA64 write byte transaction is:
     *
     * Address high byte   |  Address low byte   |    Data0    |    Data1   ...
     *      8-bit          |        8-bit        |    8-bit    |    8-bit
     * */
    DBE_DEBUG (DBG_SM_CH | DBG_LVL_TRACE, "[sm_ch:24aa64_write_generic] data =  0x%02X\n", *data);
    DBE_DEBUG (DBG_SM_CH | DBG_LVL_TRACE, "[sm_ch:24aa64_write_generic] addr =  0x%04X\n", addr);

    ssize_t smpr_err = smpr_write_block (self->proto, E24AA64_ADDR_SIZE/SMPR_BYTE_2_BIT,
            E24AA64_ADDR_W(addr), size, (uint32_t *) &data);

    /* Check if we have written everything */
    ASSERT_TEST(smpr_err >= 0 && (size_t)smpr_err == size /* in bytes */,
            "Could not write to SMPR", err_smpr_write, -1);

    /* Return the number of data bytes written */
    err = smpr_err;

    /* 24AA64 takes up to 2 ms to write the page */
    SMCH_24AA64_WAIT_DFLT;

err_smpr_write:
    return err;
}

static ssize_t _smch_24aa64_read_generic (smch_24aa64_t *self, uint16_t addr,
        uint8_t *data, size_t size)
{
    ssize_t err = 0;

    /* Single byte read with 16-bit addresses (15-13 are don't care, 12-0 are
     * valid addresses bits) and 8-bit data:
     *
     * Source: 24AA64/24LC64 DS21189F datasheet, page 10-11
     */

    /* 24AA64 byte read transaction is:
     *
     * Address high byte   |  Address low byte
     *      8-bit          |        8-bit
     * */
    DBE_DEBUG (DBG_SM_CH | DBG_LVL_TRACE, "[sm_ch:24aa64_read_generic] addr = 0x%04X\n", addr);

    err = smpr_read_block (self->proto, E24AA64_ADDR_SIZE/SMPR_BYTE_2_BIT,
            E24AA64_ADDR_W(addr), size, (uint32_t *) data);
    /* Check if we have written everything */
    ASSERT_TEST(err >= 0 && (size_t) err == size /* in bytes */,
            "Could not READ from SMPR", err_smpr_read, -1);

    /* 24AA64 takes up to 2 ms to write the page */
    SMCH_24AA64_WAIT_DFLT;

err_smpr_read:
    return err;
}

