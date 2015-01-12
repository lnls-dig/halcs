/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

/* This file is based on the work of Andrzej Wojenski for the FCS project,
 * available, to my knowledge, only in https://github.com/lerwys/fcs
 *
 * Description: Software driver for XTAL Si57x chip
 */

#include "sm_ch_si57x.h"
#include "sm_pr.h"
#include "hal_assert.h"
#include "hal_stddef.h"
#include "si57x_regs.h"
#include "sm_ch_si57x_defaults.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, SM_CH, "[sm_ch:si57x]",           \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)       \
    ASSERT_HAL_ALLOC(ptr, SM_CH, "[sm_ch:si57x]",                   \
            smch_err_str(SMCH_ERR_ALLOC),                           \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                    \
    CHECK_HAL_ERR(err, SM_CH, "[sm_ch:si57x]",                      \
            smch_err_str (err_type))

#define SMCH_SI57X_WAIT_TRIES               10
#define SMCH_SI57X_NAME                     "I2C_SI57X"
#define SMCH_SI57X_USECS_WAIT               10000
#define SMCH_SI57X_WAIT(usecs)              usleep(usecs)
#define SMCH_SI57X_WAIT_DFLT                SMCH_SI57X_WAIT(SMCH_SI57X_USECS_WAIT)

static smch_err_e _smch_si57x_write_8 (smch_si57x_t *self, uint8_t addr,
        const uint8_t *data);
static smch_err_e _smch_si57x_write_block (smch_si57x_t *self, uint8_t addr,
        const uint8_t *data, size_t size);
static ssize_t _smch_si57x_write_generic (smch_si57x_t *self, uint8_t addr,
        const uint8_t *data, size_t size);

static smch_err_e _smch_si57x_read_8 (smch_si57x_t *self, uint8_t addr,
        uint8_t *data);
static smch_err_e _smch_si57x_read_block (smch_si57x_t *self, uint8_t addr,
        uint8_t *data, size_t size);
static ssize_t _smch_si57x_read_generic (smch_si57x_t *self, uint8_t addr,
        uint8_t *data, size_t size);

static smch_err_e _smch_si57x_get_divs (smch_si57x_t *self, uint64_t *rfreq,
        unsigned int *n1, unsigned int *hs_div);
static smch_err_e _smch_si57x_get_defaults (smch_si57x_t *self, double fout);
static smch_err_e _smch_si57x_set_freq_raw (smch_si57x_t *self, uint8_t *data,
        size_t size);
static smch_err_e _smch_si57x_calc_divs (smch_si57x_t *self, double frequency,
        uint64_t *out_rfreq, unsigned int *out_n1, unsigned int *out_hs_div);
static smch_err_e _smch_si57x_wait_new_freq (smch_si57x_t *self);

/* Creates a new instance of the SMCH SI57X */
smch_si57x_t * smch_si57x_new (smio_t *parent, uint32_t base, uint32_t addr,
        int verbose)
{
    (void) verbose;
    assert (parent);

    smch_si57x_t *self = (smch_si57x_t *) zmalloc (sizeof *self);
    ASSERT_ALLOC(self, err_self_alloc);

    self->i2c = smpr_new (SMCH_SI57X_NAME, parent, SMPR_I2C, verbose);
    ASSERT_ALLOC(self->i2c, err_i2c_alloc);

    /* Initalize the I2C protocol */
    int smpr_err = smpr_open (self->i2c, base, NULL /* Default parameters are fine */);
    ASSERT_TEST(smpr_err == 0, "Could not initialize SMPR protocol", err_smpr_init);

    self->addr = addr;

    /* Initialize Si57X parameters */
    self->fxtal     = SMCH_SI57X_DFLT_FXTAL;
    self->n1        = SMCH_SI57X_DFLT_N1;
    self->hs_div    = SMCH_SI57X_DFLT_HSDIV;
    self->rfreq     = SMCH_SI57X_DFLT_RFREQ;
    self->frequency = SMCH_SI57X_DFLT_FREQUENCY;

    DBE_DEBUG (DBG_SM_CH | DBG_LVL_INFO, "[sm_ch:si57x] Created instance of SMCH\n");
    return self;

err_smpr_init:
    smpr_destroy (&self->i2c);
err_i2c_alloc:
    free (self);
err_self_alloc:
    return NULL;
}

/* Destroy an instance of the SMCH SI57X */
smch_err_e smch_si57x_destroy (smch_si57x_t **self_p)
{
    assert (self_p);

    if (*self_p) {
        smch_si57x_t *self = *self_p;

        smpr_release (self->i2c);
        smpr_destroy (&self->i2c);
        free (self);
        *self_p = NULL;
    }

    return SMCH_SUCCESS;
}

smch_err_e smch_si57x_write_8 (smch_si57x_t *self, uint8_t addr,
        const uint8_t *data)
{
    return _smch_si57x_write_8 (self, addr, data);
}

smch_err_e smch_si57x_write_block (smch_si57x_t *self, uint8_t addr,
        const uint8_t *data, size_t size)
{
    return _smch_si57x_write_block (self, addr, data, size);
}

smch_err_e smch_si57x_read_8 (smch_si57x_t *self, uint8_t addr,
        uint8_t *data)
{
    return _smch_si57x_read_8 (self, addr, data);
}

smch_err_e smch_si57x_read_block (smch_si57x_t *self, uint8_t addr,
        uint8_t *data, size_t size)
{
    return _smch_si57x_read_block (self, addr, data, size);
}

smch_err_e smch_si57x_get_divs (smch_si57x_t *self, uint64_t *rfreq,
        unsigned int *n1, unsigned int *hs_div)
{
    return _smch_si57x_get_divs (self, rfreq, n1, hs_div);
}

smch_err_e smch_si57x_get_defaults (smch_si57x_t *self, double fout)
{
    return _smch_si57x_get_defaults (self, fout);
}

smch_err_e smch_si57x_set_freq (smch_si57x_t *self, double frequency)
{
    assert (self);
    smch_err_e err = SMCH_SUCCESS;

    DBE_DEBUG (DBG_SM_CH | DBG_LVL_TRACE, "[sm_ch:si57x_set_freq] Configuring "
            "frequency to %f Hz\n", frequency);

    /* Get optimal divider values */
    err = _smch_si57x_calc_divs (self, frequency, &self->rfreq, &self->n1,
            &self->hs_div);
    ASSERT_TEST(err == SMCH_SUCCESS, "Could not calculate optimal divider values",
            err_exit);

    /* Format divider values */
    uint8_t divs[SI57X_NUM_DIV_REGS];
    divs[0] = SI57X_HS_N1_HS_W(self->hs_div - SI57X_HS_N1_HS_OFFSET) |
        (((self->n1 - 1) >> SI57X_HS_N1_N1_6_2_SHIFT_MSB) & SI57X_HS_N1_N1_6_2_MASK);
    divs[1] = SI57X_N1_RFREQ0_N1_1_0_W(self->n1 - 1) |
        SI57X_N1_RFREQ0_RFREQ_37_32_W(self->rfreq >> 32);
    divs[2] = SI57X_RFREQ1_RFREQ_31_24_W(self->rfreq >> 24);
    divs[3] = SI57X_RFREQ2_RFREQ_23_16_W(self->rfreq >> 16);
    divs[4] = SI57X_RFREQ3_RFREQ_15_8_W(self->rfreq >> 8);
    divs[5] = SI57X_RFREQ4_RFREQ_7_0_W(self->rfreq);

    /* Setup new frequency */
    err = _smch_si57x_set_freq_raw (self, divs, SI57X_NUM_DIV_REGS);
    ASSERT_TEST(err == SMCH_SUCCESS, "Could not set new frequency",
            err_exit);

err_exit:
    return err;
}

/***************** Static functions *****************/

static smch_err_e _smch_si57x_write_8 (smch_si57x_t *self, uint8_t addr,
        const uint8_t *data)
{
    return (_smch_si57x_write_generic (self, addr, data, sizeof(*data)) ==
            sizeof(*data))? SMCH_SUCCESS : SMCH_ERR_RW_SMPR;
}

static smch_err_e _smch_si57x_write_block (smch_si57x_t *self, uint8_t addr,
        const uint8_t *data, size_t size)
{
    ssize_t ret = _smch_si57x_write_generic (self, addr, data, size);
    return (ret >= 0 && (size_t) ret == size)? SMCH_SUCCESS : SMCH_ERR_RW_SMPR;
}

static ssize_t _smch_si57x_write_generic (smch_si57x_t *self, uint8_t addr,
        const uint8_t *data, size_t size)
{
    assert (self);
    assert (data);
    ssize_t err = -1;

    ASSERT_TEST(size < SI57X_DATA_BYTES_MAX /* in bytes */,
            "Transaction size too big. Maximum of 32 bytes.", err_smpr_write, -1);

    uint32_t trans_size = SI57X_ADDR_TRANS_SIZE + size*SI57X_DATA_TRANS_SIZE;
    uint32_t flags = SMPR_PROTO_I2C_TRANS_SIZE_FLAGS_W(trans_size) /* in bits */ |
        SMPR_PROTO_I2C_ADDR_FLAGS_W(self->addr);

    /* SI57X write byte transaction is:
     *
     *
     *  Address byte   |   Data byte
     *      8-bit      |     8-bit
     *
     * */
    DBE_DEBUG (DBG_SM_CH | DBG_LVL_TRACE, "[sm_ch:si57x_write_8] data = 0x%02X\n",
            *data);
    DBE_DEBUG (DBG_SM_CH | DBG_LVL_TRACE, "[sm_ch:si57x_write_8] addr = 0x%02X\n",
            addr);

    /* FIXME? Reduce memcpy calls? We just need this because the address must
     * come in the LSBs of data */
    uint8_t __data[SI57X_TRANS_SIZE_MAX/SMPR_BYTE_2_BIT];
    memcpy ((uint8_t *) &__data, &addr, SI57X_ADDR_TRANS_SIZE/SMPR_BYTE_2_BIT);
    memcpy ((uint8_t *) &__data + SI57X_ADDR_TRANS_SIZE/SMPR_BYTE_2_BIT, data,
            size);

    ssize_t smpr_err = smpr_write_block (self->i2c, 0, ARRAY_SIZE(__data),
            (uint32_t *) &__data, flags);
    ASSERT_TEST(smpr_err == trans_size/SMPR_BYTE_2_BIT /* in bytes*/,
            "Could not write data to I2C", err_exit, -1);

    /* Return just the number of data bytes written */
    err = smpr_err - SI57X_ADDR_TRANS_SIZE/SMPR_BYTE_2_BIT;

err_exit:
err_smpr_write:
    return err;
}

static smch_err_e _smch_si57x_read_8 (smch_si57x_t *self, uint8_t addr,
        uint8_t *data)
{
    return (_smch_si57x_read_generic (self, addr, data, sizeof(*data)) ==
            sizeof(*data))? SMCH_SUCCESS : SMCH_ERR_RW_SMPR;
}

static smch_err_e _smch_si57x_read_block (smch_si57x_t *self, uint8_t addr,
        uint8_t *data, size_t size)
{
    ssize_t ret = _smch_si57x_read_generic (self, addr, data, size);
    return (ret >= 0 && (size_t) ret == size)? SMCH_SUCCESS : SMCH_ERR_RW_SMPR;
}

static ssize_t _smch_si57x_read_generic (smch_si57x_t *self, uint8_t addr, uint8_t *data,
        size_t size)
{
    assert (self);
    assert (data);

    ssize_t err = -1;
    uint32_t trans_size = SI57X_ADDR_TRANS_SIZE;
    /* Si571 needs a repeated start between the write and read commands */
    uint32_t flags = SMPR_PROTO_I2C_REP_START  |
        SMPR_PROTO_I2C_TRANS_SIZE_FLAGS_W(trans_size) /* in bits */ |
        SMPR_PROTO_I2C_ADDR_FLAGS_W(self->addr);

    /* SI57X read byte transaction is:
     *
     *
     *  Address byte
     *      8-bit
     *
     * */
    DBE_DEBUG (DBG_SM_CH | DBG_LVL_TRACE, "[sm_ch:si57x_read_8] addr = 0x%02X\n",
            addr);

    ssize_t smpr_err = smpr_write_32 (self->i2c, 0, (uint32_t *) &addr, flags);
    /* Check if we have written everything */
    ASSERT_TEST(smpr_err == trans_size/SMPR_BYTE_2_BIT /* in bytes */,
            "Could not write data to I2C", err_exit, -1);

    /* Now, read the data */
    trans_size = size*SI57X_DATA_TRANS_SIZE;
    flags = SMPR_PROTO_I2C_TRANS_SIZE_FLAGS_W(trans_size) /* in bits */ |
        SMPR_PROTO_I2C_ADDR_FLAGS_W(self->addr);

    smpr_err = smpr_read_block (self->i2c, 0, size, (uint32_t *) data,
        flags);
    /* Check if we have written everything */
    ASSERT_TEST(smpr_err == trans_size/SMPR_BYTE_2_BIT /* in bytes */,
            "Could not read data from I2C", err_exit, -1);

    err = smpr_err;

err_exit:
    return err;
}

static smch_err_e _smch_si57x_get_divs (smch_si57x_t *self, uint64_t *rfreq,
        unsigned int *n1, unsigned int *hs_div)
{
    assert (self);
    assert (rfreq);
    assert (n1);
    assert (hs_div);

    uint8_t divs[SI57X_NUM_DIV_REGS] = {0};
    smch_err_e err = SMCH_SUCCESS;

    /* Read all divider and RFreq registers */
    ssize_t smpr_err = _smch_si57x_read_generic (self, SI57X_REG_HS_N1, divs,
            SI57X_NUM_DIV_REGS);
    ASSERT_TEST(smpr_err == SI57X_NUM_DIV_REGS, "Could not get divider values",
            err_exit, SMCH_ERR_RW_SMPR);

    /* Get divider values */
    *hs_div = SI57X_HS_N1_HS_R(divs[0]) + SI57X_HS_N1_HS_OFFSET;
    *n1 = (SI57X_HS_N1_N1_6_2_R(divs[0]) << SI57X_HS_N1_N1_6_2_SHIFT_MSB) +
                (SI57X_N1_RFREQ0_N1_1_0_R(divs[1])) + 1;
    /* Handle invalid cases */
    if (*n1 > 1) {
        *n1 &= ~1;
    }

    uint64_t tmp = SI57X_N1_RFREQ0_RFREQ_37_32_R(divs[1]);
    tmp = (tmp << 8) + SI57X_RFREQ1_RFREQ_31_24_R(divs[2]);
    tmp = (tmp << 8) + SI57X_RFREQ2_RFREQ_23_16_R(divs[3]);
    tmp = (tmp << 8) + SI57X_RFREQ3_RFREQ_15_8_R(divs[4]);
    tmp = (tmp << 8) + SI57X_RFREQ4_RFREQ_7_0_R(divs[5]);
    *rfreq = tmp;

    DBE_DEBUG (DBG_SM_CH | DBG_LVL_INFO, "[sm_ch:si57x_get_divs]\n"
            "\tRFREQ = %lu, HS_DIV = %u, N1 = %u\n", *rfreq, *hs_div, *n1);

err_exit:
    return err;
}

static smch_err_e _smch_si57x_get_defaults (smch_si57x_t *self, double fout)
{
    assert (self);
    smch_err_e err = SMCH_SUCCESS;

    DBE_DEBUG (DBG_SM_CH | DBG_LVL_TRACE, "[sm_ch:si57x_get_defaults] Returning "
            "Si57x to default configurations\n");

    /* Return Si57x to default configurations */
    uint8_t data = SI57X_CONTROL_RECALL;
    _smch_si57x_write_8 (self, SI57X_REG_CONTROL, &data);

    /* Si57x takes up to 30ms to return to initial conditions */
    SMCH_SI57X_WAIT(30000);

    /* Read dividers */
    err = _smch_si57x_get_divs (self, &self->rfreq, &self->n1, &self->hs_div);
    ASSERT_TEST(err == SMCH_SUCCESS, "Could not get divider values", err_exit);

    uint64_t fdco = SI57X_FOUT_FACTORY_DFLT * self->n1 * self->hs_div;
    self->fxtal = (double) (fdco << SI57X_RFREQ_FRAC_SIZE) / (double) (self->rfreq);

    DBE_DEBUG (DBG_SM_CH | DBG_LVL_TRACE, "[sm_ch:si57x_get_defaults] fxtal: %f, "
            "fdco: %" PRIu64 ", rfreq: %f\n", self->fxtal, fdco, self->rfreq / POW_2_28);

    self->frequency = fout;

err_exit:
    return err;
}

static smch_err_e _smch_si57x_set_freq_raw (smch_si57x_t *self, uint8_t *data,
        size_t size)
{
    assert (self);
    smch_err_e err = SMCH_SUCCESS;

    /* Freeze DCO */
    uint8_t __data = SI57X_FREEZE_DCO;
    err = _smch_si57x_write_8 (self, SI57X_REG_FREEZE_DCO, &__data);
    ASSERT_TEST(err == SMCH_SUCCESS, "Could not freeze DCO", err_exit);

    SMCH_SI57X_WAIT_DFLT;

    /* Write frequency registers to Chip (for 20ppm and 50ppm devices) */
    err = _smch_si57x_write_block (self, SI57X_REG_START, data, size);
    ASSERT_TEST(err == SMCH_SUCCESS, "Could not write frequency registers to chip",
            err_exit);

    SMCH_SI57X_WAIT_DFLT;

    /* Unfreeze DCO */
    __data &= ~SI57X_FREEZE_DCO;
    err = _smch_si57x_write_8 (self, SI57X_REG_FREEZE_DCO, &__data);
    ASSERT_TEST(err == SMCH_SUCCESS, "Could not unfreeze DCO", err_exit);

    /* Apply new frequency. Must be down within 10 ms. So, hurry up! */
    __data = SI57X_CONTROL_NEWFREQ;
    err = _smch_si57x_write_8 (self, SI57X_REG_CONTROL, &__data);
    ASSERT_TEST(err == SMCH_SUCCESS, "Could not apply new frequency", err_exit);

    SMCH_SI57X_WAIT_DFLT;

    err = _smch_si57x_wait_new_freq (self);
    ASSERT_TEST(err == SMCH_SUCCESS, "New frequency setting timeout", err_exit);

    DBE_DEBUG (DBG_SM_CH | DBG_LVL_INFO, "[sm_ch:si57x_set_freq_raw] Setup new "
            "frequency completed\n");

err_exit:
    return err;
}

/* FIXME: reuse 24AA64 probe function */
ssize_t smch_si57x_probe_bus (smch_si57x_t *self)
{
    assert (self);
    ssize_t err = 0;

    DBE_DEBUG (DBG_SM_CH | DBG_LVL_TRACE, "[sm_ch:si57x_probe_bus] Probing bus ...\n");

    uint32_t i;

    for (i = 0; i < 128; ++i) {
        uint32_t flags = SMPR_PROTO_I2C_TRANS_SIZE_FLAGS_W(8) /* in bits */ |
            SMPR_PROTO_I2C_ADDR_FLAGS_W(i);
        uint32_t __data = 0;

        ssize_t smpr_err = smpr_read_32 (self->i2c, 0, &__data, flags);

        if (smpr_err >= 0) {
            DBE_DEBUG (DBG_SM_CH | DBG_LVL_INFO, "[sm_ch:si57x_probe_bus] "
                    "Found device at address 0x%02X\n", i);
        }

        SMCH_SI57X_WAIT_DFLT;
    }

    return err;
}

/******************************* Helper Functions ****************************/

static smch_err_e _smch_si57x_wait_new_freq (smch_si57x_t *self)
{
    assert (self);

    smch_err_e err = SMCH_SUCCESS;
    uint8_t data = SI57X_CONTROL_NEWFREQ;
    uint32_t tries = 0;

    /* Check if newfreq bit is cleared (new frequency applied) */
    while (1) { /* bit is automatically cleared */
        err = _smch_si57x_read_8 (self, SI57X_REG_CONTROL, &data);
        ASSERT_TEST(err == SMCH_SUCCESS, "Could not read new frequency bit",
                err_exit);

        if ((data & SI57X_CONTROL_NEWFREQ) == 0) {
            break;
        }

        SMCH_SI57X_WAIT_DFLT;
        tries++;
        ASSERT_TEST(tries < SMCH_SI57X_WAIT_TRIES, "Frequency transfer timeout", err_exit,
                SMCH_ERR_TIMEOUT);
    }

err_exit:
    return err;
}

/* Cut from linux sources */
static smch_err_e _smch_si57x_calc_divs (smch_si57x_t *self, double frequency,
        uint64_t *out_rfreq, unsigned int *out_n1, unsigned int *out_hs_div)
{
    smch_err_e err = SMCH_SUCCESS;
    unsigned int n1, hs_div;
    double fdco, best_fdco = DBL_MAX;
    double rfreq_tmp;
    uint64_t rfreq_integer = 0;
    uint64_t rfreq_frac = 0;
    static const uint8_t si57x_hs_div_values [] = { 11, 9, 7, 6, 5, 4 };

    uint32_t i;
    for (i = 0; i < ARRAY_SIZE(si57x_hs_div_values); ++i) {
        hs_div = si57x_hs_div_values[i];

        /* Calculate lowest possible value for n1 */
        n1 = div_u64 (div_u64 (SI57X_FDCO_MIN, hs_div), (uint64_t) FLOOR(frequency));

        if (!n1 || (n1 & 1)) {
            n1++;
        }

        while (n1 <= 128) {
            fdco = (double) frequency * (double) hs_div * (double) n1;
            if (fdco > SI57X_FDCO_MAX) {
                break;
            }

            if (fdco >= SI57X_FDCO_MIN && fdco < best_fdco) {
                *out_n1 = n1;
                *out_hs_div = hs_div;

                rfreq_tmp = fdco / self->fxtal;

                /* convert new RFREQ to the binary representation
                 * separate the integer part */

                /* Integer part is a 10-bit number */
                rfreq_integer = FLOOR(rfreq_tmp);
                /* Fractional part is a 28-bit number */
                rfreq_frac = FLOOR((rfreq_tmp - rfreq_integer) * POW_2_28);

                /* Concatenate the integer and fractional parts */
                *out_rfreq = (rfreq_integer << SI57X_RFREQ_FRAC_SIZE) | rfreq_frac;
                best_fdco = fdco;
            }

            n1 += (n1 == 1 ? 1 : 2);
        }
    }

    if (best_fdco == DBL_MAX) {
        err = SMCH_ERR_INV_FUNC_PARAM;
    }

    DBE_DEBUG (DBG_SM_CH | DBG_LVL_TRACE, "[sm_ch:si57x_calc_divs] Divider values:\n"
            "\tfrequency: %f, rfreq: %" PRIu64 ", rfreq_integer: %" PRIu64 ", "
            "rfreq_frac: %" PRIu64 ", n1: %u, hs_div: %u\n", frequency, *out_rfreq,
            rfreq_integer, rfreq_frac, *out_n1, *out_hs_div);

    return err;
}

