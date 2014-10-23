/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

/* This file is based on the work of Andrzej Wojenski for the FCS project,
 * available, to my knowledge, only in https://github.com/lerwys/fcs
 *
 * Description: Software driver for AD9510 chip (clock distribution)
 */

#include "sm_ch_ad9510.h"
#include "sm_pr.h"
#include "hal_assert.h"
#include "ad9510_regs.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, SM_CH, "[sm_ch:ad9510]",          \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)       \
    ASSERT_HAL_ALLOC(ptr, SM_CH, "[sm_ch:ad9510]",                  \
            smch_err_str(SMCH_ERR_ALLOC),                           \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                    \
    CHECK_HAL_ERR(err, SM_CH, "[sm_ch:ad9510]",                     \
            smch_err_str (err_type))

#define SMCH_AD9510_WAIT_TRIES              10
#define SMCH_AD9510_NAME                    "SPI_AD9510"
#define SMCH_AD9519_USECS_WAIT              1000
#define SMCH_AD9510_WAIT(usecs)             usleep(usecs)
#define SMCH_AD9510_WAIT_DFLT               SMCH_AD9510_WAIT(SMCH_AD9519_USECS_WAIT)

static ssize_t _smch_ad9510_write_8 (smch_ad9510_t *self, uint8_t addr,
        const uint8_t *data);
static ssize_t _smch_ad9510_read_8 (smch_ad9510_t *self, uint8_t addr,
        uint8_t *data);
static bool _smch_ad9510_wait_completion (smch_ad9510_t *self, unsigned int tries);
static smch_err_e _smch_ad9510_reg_update (smch_ad9510_t *self);

/* Creates a new instance of the SMCH AD9510 */
smch_ad9510_t * smch_ad9510_new (smio_t *parent, uint32_t base, uint32_t ss,
        int verbose)
{
    (void) verbose;
    assert (parent);

    smch_ad9510_t *self = (smch_ad9510_t *) zmalloc (sizeof *self);
    ASSERT_ALLOC(self, err_self_alloc);

    self->spi = smpr_new (SMCH_AD9510_NAME, parent, SMPR_SPI, verbose);
    ASSERT_ALLOC(self->spi, err_spi_alloc);

    /* Initalize the SPI protocol */
    int smpr_err = smpr_open (self->spi, base, NULL /* Default parameters are fine */);
    ASSERT_TEST(smpr_err == 0, "Could not initialize SMPR protocol", err_smpr_init);

    self->ss = ss;

    DBE_DEBUG (DBG_SM_CH | DBG_LVL_INFO, "[sm_ch:ad9510] Created instance of SMCH\n");
    return self;

err_smpr_init:
    smpr_destroy (&self->spi);
err_spi_alloc:
    free (self);
err_self_alloc:
    return NULL;
}

/* Destroy an instance of the SMCH AD9510 */
smch_err_e smch_ad9510_destroy (smch_ad9510_t **self_p)
{
    assert (self_p);

    if (*self_p) {
        smch_ad9510_t *self = *self_p;

        smpr_release (self->spi);
        smpr_destroy (&self->spi);
        free (self);
        *self_p = NULL;
    }

    return SMCH_SUCCESS;
}

smch_err_e smch_ad9510_write_8 (smch_ad9510_t *self, uint8_t addr,
        const uint8_t *data)
{
    return (_smch_ad9510_write_8 (self, addr, data) == sizeof(uint8_t))?
        SMCH_SUCCESS : SMCH_ERR_RW_SMPR;
}

smch_err_e smch_ad9510_read_8 (smch_ad9510_t *self, uint8_t addr,
        uint8_t *data)
{
    return (_smch_ad9510_read_8 (self, addr, data) == sizeof(uint8_t))?
        SMCH_SUCCESS : SMCH_ERR_RW_SMPR;
}

smch_err_e smch_ad9510_reg_update (smch_ad9510_t *self)
{
    return _smch_ad9510_reg_update (self);
}

smch_err_e smch_ad9510_config_test (smch_ad9510_t *self)
{
    smch_err_e err = SMCH_SUCCESS;
    ssize_t rw_err = -1;

    /* Reset registers,
     * Turn on Long Instruction bit */
    uint8_t data = AD9510_CFG_SERIAL_SOFT_RST | AD9510_CFG_SERIAL_LONG_INST;
    DBE_DEBUG (DBG_SM_CH | DBG_LVL_INFO,
            "[sm_ch:ad9510] Writing 0x%02X to addr 0x%02X\n", data, AD9510_REG_CFG_SERIAL);
    rw_err = _smch_ad9510_write_8 (self, AD9510_REG_CFG_SERIAL, &data);
    ASSERT_TEST(rw_err == sizeof(uint8_t), "Could not write to AD9510_REG_CFG_SERIAL",
            err_smpr_write, SMCH_ERR_RW_SMPR);

    _smch_ad9510_reg_update (self);
    /* Wait for reset to complete */
    SMCH_AD9510_WAIT_DFLT;

    /* Clear reset */
    data &= ~AD9510_CFG_SERIAL_SOFT_RST;
    rw_err = _smch_ad9510_write_8 (self, AD9510_REG_CFG_SERIAL, &data);
    ASSERT_TEST(rw_err == sizeof(uint8_t), "Could not write to AD9510_REG_CFG_SERIAL",
            err_smpr_write, SMCH_ERR_RW_SMPR);
    _smch_ad9510_reg_update (self);

    /* Wait for reset to complete */
    SMCH_AD9510_WAIT_DFLT;

    /* Power-up LVPECL outputs */
    DBE_DEBUG (DBG_SM_CH | DBG_LVL_INFO,
            "[sm_ch:ad9510] Powering up LVPECL outputs\n");
    data = AD9510_LVPECL_OUT_LVL_W(0x02) /* 810 mV output */ |
        AD9510_LVPECL_OUT_PDOWN_W(0x0) /* Do not power down */;
    _smch_ad9510_write_8 (self, AD9510_REG_LVPECL_OUT0, &data);
    _smch_ad9510_write_8 (self, AD9510_REG_LVPECL_OUT1, &data);
    _smch_ad9510_write_8 (self, AD9510_REG_LVPECL_OUT2, &data);
    _smch_ad9510_write_8 (self, AD9510_REG_LVPECL_OUT3, &data);

    /* Power-up LVCMOS/LVDS outputs */
    DBE_DEBUG (DBG_SM_CH | DBG_LVL_INFO,
            "[sm_ch:ad9510] Powering up LVDS/CMOS output 4\n");
    data = AD9510_LVDS_CMOS_CURR_W(0x1) /* 3.5 mA, 100 Ohm */ & (
        ~AD9510_LVDS_CMOS_PDOWN /* Do not power down */);
    _smch_ad9510_write_8 (self, AD9510_REG_LVDS_CMOS_OUT4, &data);
    data = AD9510_LVDS_CMOS_CURR_W(0x1) /* 3.5 mA, 100 Ohm */ | (
        AD9510_LVDS_CMOS_PDOWN /* Power down */);
    _smch_ad9510_write_8 (self, AD9510_REG_LVDS_CMOS_OUT5, &data);
    _smch_ad9510_write_8 (self, AD9510_REG_LVDS_CMOS_OUT6, &data);
    _smch_ad9510_write_8 (self, AD9510_REG_LVDS_CMOS_OUT7, &data);

    /* Set-up clock selection (distribution mode)
       CLK1 - power on
       CLK2 - power down
       Clock select = CLK1
       Prescaler Clock -  Power-Down
       REFIN - Power-Down */
    data = 0x01;
    _smch_ad9510_write_8 (self, 0x45, &data);

    rw_err = _smch_ad9510_reg_update (self);

err_smpr_write:
    return err;
}

smch_err_e smch_ad9510_div_out0 (smch_ad9510_t *self, uint32_t div)
{
    (void) div;
    smch_err_e err = SMCH_SUCCESS;
    ssize_t rw_err = -1;

    /* Reset registers,
     * Turn on Long Instruction bit */
    uint8_t data = 0x11;
    DBE_DEBUG (DBG_SM_CH | DBG_LVL_INFO,
            "[sm_ch:ad9510] Writing 0x%02X to addr 0x%02X\n", data, 0x50);
    rw_err = _smch_ad9510_write_8 (self, 0x50, &data);
    ASSERT_TEST(rw_err == sizeof(uint8_t), "Could not write to 0x50",
            err_smpr_write, SMCH_ERR_RW_SMPR);

    _smch_ad9510_reg_update (self);
    /* Wait for reset to complete */
    SMCH_AD9510_WAIT_DFLT;

err_smpr_write:
    return err;
}
/***************** Static functions *****************/
static ssize_t _smch_ad9510_write_8 (smch_ad9510_t *self, uint8_t addr,
        const uint8_t *data)
{
    ssize_t err = sizeof(uint8_t);

    /* We do only 24-bit cycles composed of:
     *
     * 16-bit instruction bits + 8-bit data bits
     *
     * Source: AD9510 REV. B datasheet, page 43
     * */

    /* We transmit a WRITE operation, with 1 byte transfer, with address as "addr"
     * and data as "data" */
    uint32_t __data = ~AD9510_HDR_RW & (
                AD9510_HDR_BT_W(0x0) |
                AD9510_HDR_ADDR_W(addr) |
                AD9510_DATA_W(*data)
            );
    uint32_t flags = SMPR_PROTO_SPI_SS_FLAGS_W(self->ss) |
            SMPR_PROTO_SPI_CHARLEN_FLAGS_W(AD9510_TRASNS_SIZE);
    ssize_t smpr_err = smpr_write_32 (self->spi, 0, &__data, flags);
    ASSERT_TEST(smpr_err == sizeof(uint32_t), "Could not write to SMPR",
            err_smpr_write, -1);

err_smpr_write:
    return err;
}

static ssize_t _smch_ad9510_read_8 (smch_ad9510_t *self, uint8_t addr,
        uint8_t *data)
{
    ssize_t err = sizeof(uint8_t);

    /* We do only 24-bit cycles composed of:
     *
     * 16-bit instruction bits + 8-bit data bits
     *
     * Source: AD9510 REV. B datasheet, page 43
     * */

    /* We transmit a READ operation, with 1 byte transfer, with address as "addr"  */
    uint32_t __data = AD9510_HDR_RW | (
                AD9510_HDR_BT_W(0x0) |
                AD9510_HDR_ADDR_W(addr)
            );
    uint32_t flags = SMPR_PROTO_SPI_SS_FLAGS_W(self->ss) |
            SMPR_PROTO_SPI_CHARLEN_FLAGS_W(AD9510_TRASNS_SIZE);
    ssize_t smpr_err = smpr_read_32 (self->spi, 0, &__data, flags);
    ASSERT_TEST(smpr_err == sizeof(uint32_t), "Could not write to SMPR",
            err_smpr_write, -1);

    /* Only the 8 LSB are valid for one byte reading (AD9510_HDR_BT_W(0x0)) */
    memcpy(data, &__data, sizeof(uint8_t));

err_smpr_write:
    return err;
}

static bool _smch_ad9510_wait_completion (smch_ad9510_t *self, unsigned int tries)
{
    assert (self);

    bool finished = false;
    uint8_t data = 0;

    unsigned int i;
    for (i = 0; i < tries; ++i) {
        smch_err_e err = _smch_ad9510_read_8 (self, AD9510_REG_UPDATE_REGS, &data);
        ASSERT_TEST(err == sizeof(uint8_t), "Could not read AD9510_REG_UPDATE_REGS",
                err_read_8);

        /* This register clear itself once the operation is completed */
        if ((data & AD9510_UPDATE_REGS_VAL) == 0) {
            finished = true;
            break;
        }

        SMCH_AD9510_WAIT_DFLT;
        DBE_DEBUG (DBG_SM_CH | DBG_LVL_TRACE,
                "[sm_ch:ad9510] register loop %u\n", i);
    }

    if (i >= tries) {
        DBE_DEBUG (DBG_SM_CH | DBG_LVL_ERR,
                "[sm_ch:ad9510] update register took too long\n");
    }

err_read_8:
    return finished;
}

static smch_err_e _smch_ad9510_reg_update (smch_ad9510_t *self)
{
    smch_err_e err = SMCH_SUCCESS;

    uint8_t data = AD9510_UPDATE_REGS_VAL;
    ssize_t rw_err = _smch_ad9510_write_8 (self, AD9510_REG_UPDATE_REGS, &data);
    ASSERT_TEST(rw_err == sizeof(uint8_t), "Could not write AD9510_REG_UPDATE_REGS",
            err_write_8, SMCH_ERR_RW_SMPR);

    bool finished = _smch_ad9510_wait_completion (self, SMCH_AD9510_WAIT_TRIES);
    ASSERT_TEST(finished == true, "Wait completion took too long",
            err_write_8, SMCH_ERR_TIMEOUT);

err_write_8:
    return err;
}
