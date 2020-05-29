/*
 * Copyright (C) 2020 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "halcs_server.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, SM_CH, "[sm_ch:ltc2174]",                    \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)                  \
    ASSERT_HAL_ALLOC(ptr, SM_CH, "[sm_ch:ltc2174]",                            \
            smch_err_str(SMCH_ERR_ALLOC),                                      \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                               \
    CHECK_HAL_ERR(err, SM_CH, "[sm_ch:ltc2174]",                               \
            smch_err_str (err_type))

#define SMCH_LTC2174_WAIT_TRIES              10
#define SMCH_LTC2174_NAME                    "SPI_LTC2174"
#define SMCH_LTC2174_USECS_WAIT              5000
#define SMCH_LTC2174_WAIT(usecs)             usleep(usecs)
#define SMCH_LTC2174_WAIT_DFLT               SMCH_LTC2174_WAIT(SMCH_LTC2174_USECS_WAIT)

struct _smch_ltc2174_t {
    smpr_t *spi;                    /* SPI protocol object */
};

static ssize_t _smch_ltc2174_write_8 (smch_ltc2174_t *self, uint8_t addr,
        const uint8_t *data);
static ssize_t _smch_ltc2174_read_8 (smch_ltc2174_t *self, uint8_t addr,
        uint8_t *data);
static smch_err_e _smch_ltc2174_init (smch_ltc2174_t *self);
static smch_err_e _smch_ltc2174_reset (smch_ltc2174_t *self);

/* Creates a new instance of the SMCH LTC2174 */
smch_ltc2174_t * smch_ltc2174_new (smio_t *parent, uint64_t base,
        const smpr_proto_ops_t *reg_ops, int verbose, void *args)
{
    UNUSED(verbose);
    assert (parent);

    smch_ltc2174_t *self = (smch_ltc2174_t *) zmalloc (sizeof *self);
    ASSERT_ALLOC(self, err_self_alloc);

    self->spi = smpr_new (SMCH_LTC2174_NAME, parent, reg_ops, verbose);
    ASSERT_ALLOC(self->spi, err_spi_alloc);

    /* Initalize the SPI protocol */
    int smpr_err = smpr_open (self->spi, base, args);
    ASSERT_TEST(smpr_err == 0, "Could not initialize SMPR protocol", err_smpr_init);

    DBE_DEBUG (DBG_SM_CH | DBG_LVL_INFO, "[sm_ch:ltc2174] Created instance of SMCH\n");

    smch_err_e err = _smch_ltc2174_init (self);
    ASSERT_TEST(err == SMCH_SUCCESS, "Could not initialize LTC2174",
            err_smch_init);

    return self;

err_smch_init:
    smpr_release (self->spi);
err_smpr_init:
    smpr_destroy (&self->spi);
err_spi_alloc:
    free (self);
err_self_alloc:
    return NULL;
}

/* Destroy an instance of the SMCH LTC2174 */
smch_err_e smch_ltc2174_destroy (smch_ltc2174_t **self_p)
{
    assert (self_p);

    if (*self_p) {
        smch_ltc2174_t *self = *self_p;

        smpr_release (self->spi);
        smpr_destroy (&self->spi);
        free (self);
        *self_p = NULL;
    }

    return SMCH_SUCCESS;
}

smch_err_e smch_ltc2174_write_8 (smch_ltc2174_t *self, uint8_t addr,
        const uint8_t *data)
{
    return (_smch_ltc2174_write_8 (self, addr, data) == sizeof(uint8_t))?
        SMCH_SUCCESS : SMCH_ERR_RW_SMPR;
}

smch_err_e smch_ltc2174_write_8_update (smch_ltc2174_t *self, uint8_t addr,
        const uint8_t *data)
{
    return (_smch_ltc2174_write_8 (self, addr, data) == sizeof(uint8_t))?
        SMCH_SUCCESS : SMCH_ERR_RW_SMPR;
}

smch_err_e smch_ltc2174_read_8 (smch_ltc2174_t *self, uint8_t addr,
        uint8_t *data)
{
    return (_smch_ltc2174_read_8 (self, addr, data) == sizeof(uint8_t))?
        SMCH_SUCCESS : SMCH_ERR_RW_SMPR;
}


smch_err_e smch_ltc2174_reset (smch_ltc2174_t *self)
{
    return _smch_ltc2174_reset (self);
}

smch_err_e smch_ltc2174_set_test_patt (smch_ltc2174_t *self, uint32_t *test_patt)
{
    smch_err_e err = SMCH_SUCCESS;
    uint8_t data = 0;
    ssize_t rw_err = -1;

    _smch_ltc2174_read_8 (self, LTC2174_REG_TEST_PATT_MSB, &data);

    /* Mask enable bit */
    data &= ~LTC2174_TEST_PATT_OUTTEST;
    data |= (*test_patt)? LTC2174_TEST_PATT_OUTTEST : 0;

    rw_err = _smch_ltc2174_write_8 (self, LTC2174_REG_TEST_PATT_MSB, &data);
    ASSERT_TEST(rw_err == sizeof(uint8_t), "Could not set test pattern to LTC2174 registers",
            err_smpr_write, SMCH_ERR_RW_SMPR);

err_smpr_write:
    return err;
}

smch_err_e smch_ltc2174_get_test_patt (smch_ltc2174_t *self, uint32_t *test_patt)
{
    smch_err_e err = SMCH_SUCCESS;
    uint8_t data = 0;
    ssize_t rw_err = -1;

    rw_err = _smch_ltc2174_read_8 (self, LTC2174_REG_TEST_PATT_MSB, &data);
    ASSERT_TEST(rw_err == sizeof(uint8_t), "Could not get test pattern from LTC2174 registers",
            err_smpr_read, SMCH_ERR_RW_SMPR);

    *test_patt = (data & LTC2174_TEST_PATT_OUTTEST) >> LTC2174_RESET_EN_SHIFT;

err_smpr_read:
    return err;
}

smch_err_e smch_ltc2174_set_test_patt_data (smch_ltc2174_t *self, uint32_t *test_patt_data)
{
    smch_err_e err = SMCH_SUCCESS;
    uint8_t data = 0;
    ssize_t rw_err = -1;

    rw_err = _smch_ltc2174_read_8 (self, LTC2174_REG_TEST_PATT_MSB, &data);
    ASSERT_TEST(rw_err == sizeof(uint8_t), "Could not get test pattern data MSB from LTC2174 registers",
            err_smpr_read, SMCH_ERR_RW_SMPR);

    /* Extract MSB part of the divider */
    data &= ~LTC2174_TEST_PATT_MSB_MASK;
    data |= LTC2174_TEST_PATT_MSB_W(*test_patt_data >>
            LTC2174_TEST_PATT_LSB_SIZE);
    _smch_ltc2174_write_8 (self, LTC2174_REG_TEST_PATT_MSB, &data);

    /* Extract LSB part of the divider */
    data = LTC2174_TEST_PATT_LSB_W(*test_patt_data);
    rw_err = _smch_ltc2174_write_8 (self, LTC2174_REG_TEST_PATT_LSB, &data);
    ASSERT_TEST(rw_err == sizeof(uint8_t), "Could not set test pattern data LSB to LTC2174 registers",
            err_smpr_write, SMCH_ERR_RW_SMPR);

err_smpr_write:
err_smpr_read:
    return err;
}

smch_err_e smch_ltc2174_get_test_patt_data (smch_ltc2174_t *self, uint32_t *test_patt_data)
{
    smch_err_e err = SMCH_SUCCESS;
    uint8_t data = 0;
    uint32_t __test_patt_data = 0;
    ssize_t rw_err = -1;

    rw_err = _smch_ltc2174_read_8 (self, LTC2174_REG_TEST_PATT_MSB, &data);
    ASSERT_TEST(rw_err == sizeof(uint8_t), "Could not get test pattern data MSB from LTC2174 registers",
            err_smpr_read, SMCH_ERR_RW_SMPR);

    __test_patt_data |= LTC2174_TEST_PATT_MSB_R(data) << LTC2174_TEST_PATT_LSB_SIZE;

    rw_err = _smch_ltc2174_read_8 (self, LTC2174_REG_TEST_PATT_LSB, &data);
    ASSERT_TEST(rw_err == sizeof(uint8_t), "Could not get test pattern data LSB from LTC2174 registers",
            err_smpr_read, SMCH_ERR_RW_SMPR);

    __test_patt_data |= LTC2174_TEST_PATT_LSB_R(data);
    *test_patt_data = __test_patt_data;

err_smpr_read:
    return err;
}

smch_err_e smch_ltc2174_set_twos_compl (smch_ltc2174_t *self, uint32_t *twos_compl)
{
    smch_err_e err = SMCH_SUCCESS;
    uint8_t data = 0;
    ssize_t rw_err = -1;

    rw_err = _smch_ltc2174_read_8 (self, LTC2174_REG_FORMAT_PDOWN, &data);
    ASSERT_TEST(rw_err == sizeof(uint8_t), "Could not get format/pdown data from LTC2174",
            err_smpr_read, SMCH_ERR_RW_SMPR);

    /* Mask TWOS_COMPL bit */
    data &= ~LTC2174_FORMAT_PDOWN_TWOS_COMPL;
    data |= (*twos_compl)? LTC2174_FORMAT_PDOWN_TWOS_COMPL : 0;
    _smch_ltc2174_write_8 (self, LTC2174_REG_FORMAT_PDOWN, &data);
    ASSERT_TEST(rw_err == sizeof(uint8_t), "Could not set twos complement to LTC2174 registers",
            err_smpr_write, SMCH_ERR_RW_SMPR);

err_smpr_write:
err_smpr_read:
    return err;
}

smch_err_e smch_ltc2174_get_twos_compl (smch_ltc2174_t *self, uint32_t *twos_compl)
{
    smch_err_e err = SMCH_SUCCESS;
    uint8_t data = 0;
    ssize_t rw_err = -1;

    rw_err = _smch_ltc2174_read_8 (self, LTC2174_REG_FORMAT_PDOWN, &data);
    ASSERT_TEST(rw_err == sizeof(uint8_t), "Could not get format/pdown from LTC2174",
            err_smpr_read, SMCH_ERR_RW_SMPR);

    *twos_compl = (data & LTC2174_FORMAT_PDOWN_TWOS_COMPL) >> LTC2174_FORMAT_PDOWN_TWOS_COMPL_SHIFT;

err_smpr_read:
    return err;
}

/***************** Static functions *****************/

static smch_err_e _smch_ltc2174_init (smch_ltc2174_t *self)
{
    smch_err_e err = SMCH_SUCCESS;

    _smch_ltc2174_reset (self);

    uint8_t data = 0;
    smch_ltc2174_read_8 (self, 0x1, &data);
    DBE_DEBUG (DBG_SM_CH | DBG_LVL_INFO, "[sm_ch:ltc2174] Register 0x1: 0x%08X\n", data);

    smch_ltc2174_read_8 (self, 0x2, &data);
    DBE_DEBUG (DBG_SM_CH | DBG_LVL_INFO, "[sm_ch:ltc2174] Register 0x2: 0x%08X\n", data);

    smch_ltc2174_read_8 (self, 0x3, &data);
    DBE_DEBUG (DBG_SM_CH | DBG_LVL_INFO, "[sm_ch:ltc2174] Register 0x3: 0x%08X\n", data);

    smch_ltc2174_read_8 (self, 0x4, &data);
    DBE_DEBUG (DBG_SM_CH | DBG_LVL_INFO, "[sm_ch:ltc2174] Register 0x4: 0x%08X\n", data);

    return err;
}

static ssize_t _smch_ltc2174_write_8 (smch_ltc2174_t *self, uint8_t addr,
        const uint8_t *data)
{
    ssize_t err = sizeof(uint8_t);

    /* We do only 16-bit cycles composed of:
     *
     * 8-bit instruction bits + 8-bit data bits
     *
     * Source: LTC2174 datasheet, page 25
     * */

    /* We transmit a WRITE operation, with 1 byte transfer, with address as "addr"
     * and data as "data" */
    uint64_t __addr = ~LTC2174_HDR_RW & (
                LTC2174_HDR_ADDR_W(addr)
            );
    size_t __addr_size = LTC2174_INSTADDR_SIZE/SMPR_BYTE_2_BIT;
    uint32_t __data = LTC2174_DATA_W(*data);
    size_t __data_size = LTC2174_DATA_SIZE/SMPR_BYTE_2_BIT;

    ssize_t smpr_err = smpr_write_block (self->spi, __addr_size, __addr,
            __data_size, &__data);
    ASSERT_TEST(smpr_err > 0 && (size_t) smpr_err == __data_size,
            "Could not write to SMPR", err_smpr_write, -1);

err_smpr_write:
    return err;
}

static ssize_t _smch_ltc2174_read_8 (smch_ltc2174_t *self, uint8_t addr,
        uint8_t *data)
{
    ssize_t err = sizeof(uint8_t);

    /* We do only 16-bit cycles composed of:
     *
     * 8-bit instruction bits + 8-bit data bits
     *
     * Source: LTC2174 datasheet, page 25
     * */

    /* We transmit a READ operation, with 1 byte transfer, with address as "addr"  */
    uint64_t __addr = LTC2174_HDR_RW | (
                LTC2174_HDR_ADDR_W(addr)
            );
    size_t __addr_size = LTC2174_INSTADDR_SIZE/SMPR_BYTE_2_BIT;
    size_t __data_size = LTC2174_DATA_SIZE/SMPR_BYTE_2_BIT;

    ssize_t smpr_err = smpr_read_block (self->spi, __addr_size, __addr,
            __data_size, (uint32_t *) data);
    ASSERT_TEST(smpr_err > 0 && (size_t) smpr_err == __data_size,
            "Could not read to SMPR", err_read_write, -1);

err_read_write:
    return err;
}

static smch_err_e _smch_ltc2174_reset (smch_ltc2174_t *self)
{
    smch_err_e err = SMCH_SUCCESS;
    uint8_t data = 0;
    ssize_t rw_err = -1;

    /* Mask enable bit */
    data = LTC2174_RESET_EN;

    rw_err = _smch_ltc2174_write_8 (self, LTC2174_REG_RESET, &data);
    ASSERT_TEST(rw_err == sizeof(uint8_t), "Could not reset LTC2174 registers",
            err_smpr_write, SMCH_ERR_RW_SMPR);

    SMCH_LTC2174_WAIT_DFLT;

err_smpr_write:
    return err;
}

