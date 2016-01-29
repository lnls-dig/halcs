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
#define SMCH_24AA64_NAME                    "I2C_24AA64"
#define SMCH_24AA64_USECS_WAIT              10000
#define SMCH_24AA64_WAIT(usecs)             usleep(usecs)
#define SMCH_24AA64_WAIT_DFLT               SMCH_24AA64_WAIT(SMCH_24AA64_USECS_WAIT)

struct _smch_24aa64_t {
    smpr_t *i2c;                    /* I2C protocol object */
    uint32_t addr;                  /* I2C address for this 24AA64 chip */
};

static ssize_t _smch_24aa64_write_generic (smch_24aa64_t *self, uint16_t addr,
        const uint8_t *data, size_t size);
static ssize_t _smch_24aa64_read_generic (smch_24aa64_t *self, uint16_t addr,
        uint8_t *data, size_t size);

/* Creates a new instance of the SMCH 24AA64 */
smch_24aa64_t * smch_24aa64_new (smio_t *parent, uint64_t base, uint32_t addr,
        int verbose)
{
    (void) verbose;
    assert (parent);

    smch_24aa64_t *self = (smch_24aa64_t *) zmalloc (sizeof *self);
    ASSERT_ALLOC(self, err_self_alloc);

    self->i2c = smpr_new (SMCH_24AA64_NAME, parent, SMPR_I2C, verbose);
    ASSERT_ALLOC(self->i2c, err_i2c_alloc);

    /* Initalize the I2C protocol */
    int smpr_err = smpr_open (self->i2c, base, NULL /* Default parameters are fine */);
    ASSERT_TEST(smpr_err == 0, "Could not initialize SMPR protocol", err_smpr_init);

    self->addr = addr;

    DBE_DEBUG (DBG_SM_CH | DBG_LVL_INFO, "[sm_ch:24aa64] Created instance of SMCH\n");
    return self;

err_smpr_init:
    smpr_destroy (&self->i2c);
err_i2c_alloc:
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

        smpr_release (self->i2c);
        smpr_destroy (&self->i2c);
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

    uint32_t trans_size = E24AA64_ADDR_TRANS_SIZE + size*E24AA64_DATA_TRANS_SIZE;
    uint32_t flags = SMPR_PROTO_I2C_TRANS_SIZE_FLAGS_W(trans_size) /* in bits */ |
        SMPR_PROTO_I2C_ADDR_FLAGS_W(self->addr);

    /* 24AA64 write byte transaction is:
     *
     * Address high byte   |  Address low byte   |    Data0    |    Data1   ...
     *      8-bit          |        8-bit        |    8-bit    |    8-bit
     * */
    DBE_DEBUG (DBG_SM_CH | DBG_LVL_TRACE, "[sm_ch:24aa64_write_generic] data =  0x%02X\n", *data);
    DBE_DEBUG (DBG_SM_CH | DBG_LVL_TRACE, "[sm_ch:24aa64_write_generic] addr =  0x%04X\n", addr);

    /* FIXME? Reduce memcpy calls? We just need this because the address must
     * come in the LSBs of data */
    uint8_t __data[E24AA64_PAGE_TRANS_SIZE_MAX/SMPR_BYTE_2_BIT];
    uint16_t __addr = E24AA64_ADDR_W(addr);
    memcpy ((uint8_t *) &__data, &__addr, E24AA64_ADDR_TRANS_SIZE/SMPR_BYTE_2_BIT);
    memcpy ((uint8_t *) &__data + E24AA64_ADDR_TRANS_SIZE/SMPR_BYTE_2_BIT, data, size);

    ssize_t smpr_err = smpr_write_block (self->i2c, 0, ARRAY_SIZE(__data),
            (uint32_t *) &__data, flags);

    /* Check if we have written everything */
    ASSERT_TEST(smpr_err == trans_size/SMPR_BYTE_2_BIT /* in bytes */,
            "Could not write to SMPR", err_smpr_write, -1);

    /* Return just the number of data bytes written */
    err = smpr_err - E24AA64_ADDR_TRANS_SIZE/SMPR_BYTE_2_BIT;

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

    uint32_t trans_size = E24AA64_ADDR_TRANS_SIZE;
    uint32_t flags = SMPR_PROTO_I2C_TRANS_SIZE_FLAGS_W(trans_size) /* in bits */ |
        SMPR_PROTO_I2C_ADDR_FLAGS_W(self->addr);

    /* 24AA64 byte read transaction is:
     *
     * Address high byte   |  Address low byte
     *      8-bit          |        8-bit
     * */
    DBE_DEBUG (DBG_SM_CH | DBG_LVL_TRACE, "[sm_ch:24aa64_read_generic] addr =  0x%04X\n", addr);

    uint32_t __data = E24AA64_ADDR_W(addr);

    ssize_t smpr_err = smpr_write_32 (self->i2c, 0, &__data, flags);
    /* Check if we have written everything */
    ASSERT_TEST(smpr_err == trans_size/SMPR_BYTE_2_BIT /* in bytes */,
            "Could not write to SMPR", err_smpr_write, -1);

    /* Now, read the data */
    trans_size = size*E24AA64_DATA_TRANS_SIZE;
    flags = SMPR_PROTO_I2C_TRANS_SIZE_FLAGS_W(trans_size) /* in bits */ |
        SMPR_PROTO_I2C_ADDR_FLAGS_W(self->addr);

    smpr_err = smpr_read_block (self->i2c, 0, size, (uint32_t *) data, flags);
    /* Check if we have written everything */
    ASSERT_TEST(smpr_err == trans_size/SMPR_BYTE_2_BIT /* in bytes */,
            "Could not READ from SMPR", err_smpr_read, -1);

    err = smpr_err;

    /* 24AA64 takes up to 2 ms to write the page */
    SMCH_24AA64_WAIT_DFLT;

err_smpr_read:
err_smpr_write:
    return err;
}

ssize_t smch_24aa64_probe_bus (smch_24aa64_t *self)
{
    ssize_t err = 0;

    DBE_DEBUG (DBG_SM_CH | DBG_LVL_TRACE, "[sm_ch:24aa64_probe_bus] Probing bus ...\n");

    uint32_t i;

    for (i = 0; i < 128; ++i) {
        uint32_t flags = SMPR_PROTO_I2C_TRANS_SIZE_FLAGS_W(8) /* in bits */ |
            SMPR_PROTO_I2C_ADDR_FLAGS_W(i);
        uint32_t __data = 0;

        ssize_t smpr_err = smpr_read_32 (self->i2c, 0, &__data, flags);

        if (smpr_err >= 0) {
            DBE_DEBUG (DBG_SM_CH | DBG_LVL_INFO, "[sm_ch:24aa64_probe_bus] "
                    "Found device at address 0x%02X\n", i);
        }

        SMCH_24AA64_WAIT_DFLT;
    }

    return err;
}
