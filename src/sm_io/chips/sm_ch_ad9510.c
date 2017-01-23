/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

/* This file is based on the work of Andrzej Wojenski for the FCS project,
 * available, to my knowledge, only in https://github.com/lerwys/fcs
 *
 * Description: Software driver for AD9510 chip (clock distribution)
 */

#include "halcs_server.h"

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
#define SMCH_AD9510_USECS_WAIT              1000
#define SMCH_AD9510_WAIT(usecs)             usleep(usecs)
#define SMCH_AD9510_WAIT_DFLT               SMCH_AD9510_WAIT(SMCH_AD9510_USECS_WAIT)

struct _smch_ad9510_t {
    smpr_t *spi;                    /* SPI protocol object */
};

static ssize_t _smch_ad9510_write_8 (smch_ad9510_t *self, uint8_t addr,
        const uint8_t *data);
static ssize_t _smch_ad9510_read_8 (smch_ad9510_t *self, uint8_t addr,
        uint8_t *data);
static smch_err_e _smch_ad9510_init (smch_ad9510_t *self);
static bool _smch_ad9510_wait_completion (smch_ad9510_t *self, unsigned int tries);
static smch_err_e _smch_ad9510_reg_update (smch_ad9510_t *self);

/* Creates a new instance of the SMCH AD9510 */
smch_ad9510_t * smch_ad9510_new (smio_t *parent, uint64_t base,
        const smpr_proto_ops_t *reg_ops, int verbose)
{
    (void) verbose;
    assert (parent);

    smch_ad9510_t *self = (smch_ad9510_t *) zmalloc (sizeof *self);
    ASSERT_ALLOC(self, err_self_alloc);

    self->spi = smpr_new (SMCH_AD9510_NAME, parent, reg_ops, verbose);
    ASSERT_ALLOC(self->spi, err_spi_alloc);

    /* Initalize the SPI protocol */
    int smpr_err = smpr_open (self->spi, base, NULL /* Default parameters are fine */);
    ASSERT_TEST(smpr_err == 0, "Could not initialize SMPR protocol", err_smpr_init);

    DBE_DEBUG (DBG_SM_CH | DBG_LVL_INFO, "[sm_ch:ad9510] Created instance of SMCH\n");

    smch_err_e err = _smch_ad9510_init (self);
    ASSERT_TEST(err == SMCH_SUCCESS, "Could not initialize AD9510",
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

smch_err_e smch_ad9510_write_8_update (smch_ad9510_t *self, uint8_t addr,
        const uint8_t *data)
{
    smch_err_e err = (_smch_ad9510_write_8 (self, addr, data) == sizeof(uint8_t))?
        SMCH_SUCCESS : SMCH_ERR_RW_SMPR;

    if (err != SMCH_SUCCESS) {
        return err;
    }

    return _smch_ad9510_reg_update (self);
}

smch_err_e smch_ad9510_read_8 (smch_ad9510_t *self, uint8_t addr,
        uint8_t *data)
{
    return (_smch_ad9510_read_8 (self, addr, data) == sizeof(uint8_t))?
        SMCH_SUCCESS : SMCH_ERR_RW_SMPR;
}

smch_err_e smch_ad9510_read_8_update (smch_ad9510_t *self, uint8_t addr,
        uint8_t *data)
{
    smch_err_e err = (_smch_ad9510_read_8 (self, addr, data) == sizeof(uint8_t))?
        SMCH_SUCCESS : SMCH_ERR_RW_SMPR;

    if (err != SMCH_SUCCESS) {
        return err;
    }

    return _smch_ad9510_reg_update (self);
}

smch_err_e smch_ad9510_reg_update (smch_ad9510_t *self)
{
    return _smch_ad9510_reg_update (self);
}

smch_err_e smch_ad9510_cfg_defaults (smch_ad9510_t *self)
{
    smch_err_e err = SMCH_SUCCESS;

    err = _smch_ad9510_init (self);
    ASSERT_TEST(err == SMCH_SUCCESS, "Could not initialize AD9510",
            err_smpr_write, SMCH_ERR_RW_SMPR);

    /* Setup A and B PLL divider */
    uint8_t data = AD9510_PLL_A_COUNTER_W(0);
    _smch_ad9510_write_8 (self, AD9510_REG_PLL_A_COUNTER, &data);

    /* Extract MSB part of the divider */
    data = AD9510_PLL_B_MSB_COUNTER_W(SMCH_AD9510_DFLT_PLL_B_COUNTER >>
            AD9510_PLL_B_LSB_COUNTER_SIZE);
    _smch_ad9510_write_8 (self, AD9510_REG_PLL_B_MSB_COUNTER, &data);

        /* Extract LSB part of the divider */
    data = AD9510_PLL_B_LSB_COUNTER_W(SMCH_AD9510_DFLT_PLL_B_COUNTER);
    _smch_ad9510_write_8 (self, AD9510_REG_PLL_B_LSB_COUNTER, &data);

    /* Setup MUX status pin */
    data = AD9510_PLL_2_CP_MODE_W(0x03 /* CP normal operation*/) |
        AD9510_PLL_2_MUX_SEL_W(0x01 /* Digital Lock Detect */) |
        AD9510_PLL_2_PFD_POL_POS; /* PFD positive polarity */
    _smch_ad9510_write_8 (self, AD9510_REG_PLL_2, &data);

    /* Setup Prescaler and Power PLL Up*/
    data = AD9510_PLL_4_PRESCALER_P_W(0 /* Divide by 1 */) |
        AD9510_PLL_4_PLL_PDOWN_W(0x0);
    _smch_ad9510_write_8 (self, AD9510_REG_PLL_4, &data);

    /* Setup R divider */
    data = AD9510_PLL_R_MSB_COUNTER_W(SMCH_AD9510_DFLT_PLL_R_COUNTER >>
            AD9510_PLL_R_LSB_COUNTER_SIZE);
    _smch_ad9510_write_8 (self, AD9510_REG_PLL_R_MSB_COUNTER, &data);
    data = AD9510_PLL_R_LSB_COUNTER_W(SMCH_AD9510_DFLT_PLL_R_COUNTER);
    _smch_ad9510_write_8 (self, AD9510_REG_PLL_R_LSB_COUNTER, &data);

    /* Power-up LVPECL outputs */
    DBE_DEBUG (DBG_SM_CH | DBG_LVL_INFO,
            "[sm_ch:ad9510] Powering up LVPECL outputs 0-3\n");
    data = AD9510_LVPECL_OUT_LVL_W(0x02) /* 810 mV output */ |
        AD9510_LVPECL_OUT_PDOWN_W(0x0) /* Do not power down */;
    _smch_ad9510_write_8 (self, AD9510_REG_LVPECL_OUT0, &data);
    _smch_ad9510_write_8 (self, AD9510_REG_LVPECL_OUT1, &data);
    _smch_ad9510_write_8 (self, AD9510_REG_LVPECL_OUT2, &data);
    _smch_ad9510_write_8 (self, AD9510_REG_LVPECL_OUT3, &data);

    /* Power-up LVCMOS/LVDS output 4 (DEBUG) */
    DBE_DEBUG (DBG_SM_CH | DBG_LVL_INFO,
            "[sm_ch:ad9510] Powering up LVDS/CMOS output 4\n");
    data = AD9510_LVDS_CMOS_CURR_W(0x1) /* 3.5 mA, 100 Ohm */ & (
        ~AD9510_LVDS_CMOS_PDOWN /* Do not power down */);
    _smch_ad9510_write_8 (self, AD9510_REG_LVDS_CMOS_OUT4, &data);

    /* Power-down LVCMOS/LVDS outputs 5-7*/
    DBE_DEBUG (DBG_SM_CH | DBG_LVL_INFO,
            "[sm_ch:ad9510] Powering down LVDS/CMOS outputs 5-7\n");
    data = AD9510_LVDS_CMOS_CURR_W(0x1) /* 3.5 mA, 100 Ohm */ | (
        AD9510_LVDS_CMOS_PDOWN /* Power down */);
    _smch_ad9510_write_8 (self, AD9510_REG_LVDS_CMOS_OUT5, &data);
    _smch_ad9510_write_8 (self, AD9510_REG_LVDS_CMOS_OUT6, &data);
    _smch_ad9510_write_8 (self, AD9510_REG_LVDS_CMOS_OUT7, &data);

    /* Set-up clock selection (distribution mode)
     * CLK1 - power off
     * CLK2 - power on
     * Clock select = CLK2
     * Prescaler Clock -  Power-Up
     * REFIN - Power-Up
     */
    data = AD9510_CLK_OPT_CLK1_PD /* Power CLK1 down */ & (
                ~AD9510_CLK_OPT_REFIN_PD /* Power Reference In Up*/ &
                ~AD9510_CLK_OPT_PS_PD /* Power Prescaler Up*/ &
                ~AD9510_CLK_OPT_SEL_CLK1 /* Select CLK2*/);
    _smch_ad9510_write_8 (self, AD9510_REG_CLK_OPT, &data);

    /* Update registers */
    _smch_ad9510_reg_update (self);
    SMCH_AD9510_WAIT_DFLT;

    /* Clock dividers OUT0 - OUT7
     * divide = off (bypassed, ratio 1)
     * duty cycle 50%
     * lo-hi 0x00
     */
    data = AD9510_DIV_DCYCLE_LOW_W(0x0) | AD9510_DIV_DCYCLE_HIGH_W(0x0);

    _smch_ad9510_write_8 (self, AD9510_REG_DIV0_DCYCLE, &data);
    _smch_ad9510_write_8 (self, AD9510_REG_DIV1_DCYCLE, &data);
    _smch_ad9510_write_8 (self, AD9510_REG_DIV2_DCYCLE, &data);
    _smch_ad9510_write_8 (self, AD9510_REG_DIV3_DCYCLE, &data);
    _smch_ad9510_write_8 (self, AD9510_REG_DIV4_DCYCLE, &data);
    _smch_ad9510_write_8 (self, AD9510_REG_DIV5_DCYCLE, &data);
    _smch_ad9510_write_8 (self, AD9510_REG_DIV6_DCYCLE, &data);
    _smch_ad9510_write_8 (self, AD9510_REG_DIV7_DCYCLE, &data);

    /* Clock dividers OUT0 - OUT7
     * phase offset = 0
     * start high
     * bypass
     */
    data = AD9510_DIV_BYPASS | AD9510_DIV_START_HIGH | AD9510_DIV_OPT_PHASE_W(0x0);

    _smch_ad9510_write_8 (self, AD9510_REG_DIV0_OPT, &data);
    _smch_ad9510_write_8 (self, AD9510_REG_DIV1_OPT, &data);
    _smch_ad9510_write_8 (self, AD9510_REG_DIV2_OPT, &data);
    _smch_ad9510_write_8 (self, AD9510_REG_DIV3_OPT, &data);
    _smch_ad9510_write_8 (self, AD9510_REG_DIV4_OPT, &data);
    _smch_ad9510_write_8 (self, AD9510_REG_DIV5_OPT, &data);
    _smch_ad9510_write_8 (self, AD9510_REG_DIV6_OPT, &data);
    _smch_ad9510_write_8 (self, AD9510_REG_DIV7_OPT, &data);

    /* Function pin is SYNCB */
    data = AD9510_FUNCTION_FUNC_SEL_W(0x1);
    _smch_ad9510_write_8 (self, AD9510_REG_FUNCTION, &data);

    /* Update registers */
    _smch_ad9510_reg_update (self);
    SMCH_AD9510_WAIT_DFLT;

    /* Software sync */
    data |= AD9510_FUNCTION_SYNC_REG;
    _smch_ad9510_write_8 (self, AD9510_REG_FUNCTION, &data);

    /* Update registers */
    _smch_ad9510_reg_update (self);
    SMCH_AD9510_WAIT_DFLT;

    data &= ~AD9510_FUNCTION_SYNC_REG;
    _smch_ad9510_write_8 (self, AD9510_REG_FUNCTION, &data);

    _smch_ad9510_reg_update (self);

err_smpr_write:
    return err;
}

smch_err_e smch_ad9510_set_pll_a_div (smch_ad9510_t *self, uint32_t *div)
{
    smch_err_e err = SMCH_SUCCESS;
    uint32_t __div = *div;

    ASSERT_TEST(__div < AD9510_PLL_A_COUNTER_MASK+1 ,
            "PLL A divider is out of range", err_smpr_write,
            SMCH_ERR_INV_FUNC_PARAM);

    uint8_t data = AD9510_PLL_A_COUNTER_W(__div);
    _smch_ad9510_write_8 (self, AD9510_REG_PLL_A_COUNTER, &data);

    _smch_ad9510_reg_update (self);
    /* Wait for reset to complete */
    SMCH_AD9510_WAIT_DFLT;

err_smpr_write:
    return err;
}

smch_err_e smch_ad9510_get_pll_a_div (smch_ad9510_t *self, uint32_t *div)
{
    smch_err_e err = SMCH_SUCCESS;

    _smch_ad9510_read_8 (self, AD9510_REG_PLL_A_COUNTER, (uint8_t *) div);
    *div = AD9510_PLL_A_COUNTER_R(*div);

    return err;
}

smch_err_e smch_ad9510_set_pll_b_div (smch_ad9510_t *self, uint32_t *div)
{
    smch_err_e err = SMCH_SUCCESS;
    uint32_t __div = *div;

    /* Divider can by 0 (bypass mode) or between the specified limits */
    ASSERT_TEST(__div == 0 || (__div > AD9510_PLL_B_MIN_VALUE-1 &&
            __div < AD9510_PLL_B_COUNTER_MASK+1),
            "PLL B divider is out of range", err_smpr_write,
            SMCH_ERR_INV_FUNC_PARAM);

    uint8_t data = 0;
    if (__div == 0) {
        _smch_ad9510_read_8 (self, AD9510_REG_PLL_4, &data);

        data |= AD9510_PLL_4_B_BYPASS;
        _smch_ad9510_write_8 (self, AD9510_REG_PLL_4, &data);
    }
    else {
        /* Extract MSB part of the divider */
        data = AD9510_PLL_B_MSB_COUNTER_W(__div >>
                AD9510_PLL_B_LSB_COUNTER_SIZE);
        _smch_ad9510_write_8 (self, AD9510_REG_PLL_B_MSB_COUNTER, &data);

        /* Extract LSB part of the divider */
        data = AD9510_PLL_B_LSB_COUNTER_W(__div);
        _smch_ad9510_write_8 (self, AD9510_REG_PLL_B_LSB_COUNTER, &data);
    }

    _smch_ad9510_reg_update (self);
    /* Wait for reset to complete */
    SMCH_AD9510_WAIT_DFLT;

err_smpr_write:
    return err;
}

smch_err_e smch_ad9510_get_pll_b_div (smch_ad9510_t *self, uint32_t *div)
{
    smch_err_e err = SMCH_SUCCESS;

    /* Check if divider is in bypass mode */
    uint8_t data = 0;
    _smch_ad9510_read_8 (self, AD9510_REG_PLL_4, &data);

    if (data & AD9510_PLL_4_B_BYPASS) {
        *div = 1; /* No division */
    }
    else {
        _smch_ad9510_read_8 (self, AD9510_REG_PLL_B_MSB_COUNTER, &data);
        /* Extract MSB part of the divider */
        *div = AD9510_PLL_B_MSB_COUNTER_R(data) << AD9510_PLL_B_LSB_COUNTER_SIZE;

        _smch_ad9510_read_8 (self, AD9510_REG_PLL_B_LSB_COUNTER, &data);
        /* Extract LSB part of the divider */
        *div |= AD9510_PLL_B_LSB_COUNTER_R(data);
    }

    return err;
}

smch_err_e smch_ad9510_set_pll_prescaler (smch_ad9510_t *self, uint32_t *pre)
{
    smch_err_e err = SMCH_SUCCESS;
    uint32_t __pre = *pre;

    uint8_t data = 0;
    _smch_ad9510_read_8 (self, AD9510_REG_PLL_4, &data);

    data = (data & ~AD9510_PLL_4_PRESCALER_P_MASK) |
        AD9510_PLL_4_PRESCALER_P_W(__pre);
    _smch_ad9510_write_8 (self, AD9510_REG_PLL_4, &data);

    _smch_ad9510_reg_update (self);
    /* Wait for reset to complete */
    SMCH_AD9510_WAIT_DFLT;

    return err;
}

smch_err_e smch_ad9510_get_pll_prescaler (smch_ad9510_t *self, uint32_t *pre)
{
    smch_err_e err = SMCH_SUCCESS;

    _smch_ad9510_read_8 (self, AD9510_REG_PLL_4, (uint8_t *) pre);
    *pre = AD9510_PLL_4_PRESCALER_P_R(*pre);

    return err;
}

/* PLL power-down:
 * ctl = 0 -> Normal operation
 * ctl = 1 -> Asynchronous power-down
 * ctl = 2 -> Normal operation
 * ctl = 3 -> Synchronous power-down
 * */
smch_err_e smch_ad9510_set_pll_pdown (smch_ad9510_t *self, uint32_t *pdown)
{
    smch_err_e err = SMCH_SUCCESS;
    uint32_t __pdown = *pdown;

    ASSERT_TEST(__pdown < AD9510_PLL_PDOWN_SYNC+1,
            "PLL Power Down command is invalid", err_smpr_write,
            SMCH_ERR_INV_FUNC_PARAM);

    uint8_t data = 0;
    _smch_ad9510_read_8 (self, AD9510_REG_PLL_4, &data);

    data = (data & ~AD9510_PLL_4_PLL_PDOWN_MASK) |
        AD9510_PLL_4_PLL_PDOWN_W(__pdown);
    _smch_ad9510_write_8 (self, AD9510_REG_PLL_4, &data);

    _smch_ad9510_reg_update (self);
    /* Wait for reset to complete */
    SMCH_AD9510_WAIT_DFLT;

err_smpr_write:
    return err;
}

smch_err_e smch_ad9510_get_pll_pdown (smch_ad9510_t *self, uint32_t *pdown)
{
    smch_err_e err = SMCH_SUCCESS;

    _smch_ad9510_read_8 (self, AD9510_REG_PLL_4, (uint8_t *) pdown);
    *pdown = AD9510_PLL_4_PLL_PDOWN_R(*pdown);

    return err;
}

smch_err_e smch_ad9510_set_mux_status (smch_ad9510_t *self, uint32_t *mux)
{
    smch_err_e err = SMCH_SUCCESS;
    uint32_t __mux = *mux;

    ASSERT_TEST(/*__mux >= AD9510_PLL_2_MUX_SEL_MIN && */
            __mux <= AD9510_PLL_2_MUX_SEL_MAX,
            "Mux status is out of range", err_smpr_write,
            SMCH_ERR_INV_FUNC_PARAM);

    uint8_t data = 0;
    _smch_ad9510_read_8 (self, AD9510_REG_PLL_2, &data);

    data = (data & ~AD9510_PLL_2_MUX_SEL_MASK) |
        AD9510_PLL_2_MUX_SEL_W(__mux);
    _smch_ad9510_write_8 (self, AD9510_REG_PLL_2, &data);

    _smch_ad9510_reg_update (self);
    /* Wait for reset to complete */
    SMCH_AD9510_WAIT_DFLT;

err_smpr_write:
    return err;
}

smch_err_e smch_ad9510_get_mux_status (smch_ad9510_t *self, uint32_t *mux)
{
    smch_err_e err = SMCH_SUCCESS;

    uint8_t data = 0;
    _smch_ad9510_read_8 (self, AD9510_REG_PLL_2, &data);
    *mux = AD9510_PLL_2_MUX_SEL_R(data);

    return err;
}

smch_err_e smch_ad9510_set_r_div (smch_ad9510_t *self, uint32_t *div)
{
    smch_err_e err = SMCH_SUCCESS;
    uint32_t __div = *div;

    ASSERT_TEST(__div < AD9510_PLL_R_COUNTER_MASK+1 ,
            "PLL R divider is out of range", err_smpr_write,
            SMCH_ERR_INV_FUNC_PARAM);

    uint8_t data = AD9510_PLL_R_MSB_COUNTER_W(__div >>
            AD9510_PLL_R_LSB_COUNTER_SIZE);
    _smch_ad9510_write_8 (self, AD9510_REG_PLL_R_MSB_COUNTER, &data);
    data = AD9510_PLL_R_LSB_COUNTER_W(__div);
    _smch_ad9510_write_8 (self, AD9510_REG_PLL_R_LSB_COUNTER, &data);

    _smch_ad9510_reg_update (self);
    /* Wait for reset to complete */
    SMCH_AD9510_WAIT_DFLT;

err_smpr_write:
    return err;
}

smch_err_e smch_ad9510_get_r_div (smch_ad9510_t *self, uint32_t *div)
{
    smch_err_e err = SMCH_SUCCESS;

    uint8_t data = 0;
    _smch_ad9510_read_8 (self, AD9510_REG_PLL_R_MSB_COUNTER, &data);
    /* Extract MSB part of the divider */
    *div = AD9510_PLL_R_MSB_COUNTER_R(data) << AD9510_PLL_R_LSB_COUNTER_SIZE;

    _smch_ad9510_read_8 (self, AD9510_REG_PLL_R_LSB_COUNTER, &data);
    /* Extract LSB part of the divider */
    *div |= AD9510_PLL_R_LSB_COUNTER_R(data);

    return err;
}

/* Charge Pump Current.
 * cp_current between 600 uA and 4800 uA*/
smch_err_e smch_ad9510_set_cp_current (smch_ad9510_t *self, uint32_t *cp_current)
{
    smch_err_e err = SMCH_SUCCESS;
    uint32_t __cp_current = *cp_current;

    ASSERT_TEST(__cp_current > AD9510_PLL3_CP_CURRENT_MIN-1 &&
            __cp_current < AD9510_PLL3_CP_CURRENT_MAX+1 &&
            __cp_current % AD9510_PLL3_CP_CURRENT_MIN == 0,
            "PLL Charge Pump current is invalid or out of range",
            err_smpr_write, SMCH_ERR_INV_FUNC_PARAM);

    /* If we are here, cp_current has one of the possible CP current
     * values. Just read the register and write the new CP value */
    uint8_t data = 0;
    _smch_ad9510_read_8 (self, AD9510_REG_PLL_3, &data);

    data = (data & ~AD9510_PLL_3_CP_CURRENT_MASK) |
        /* Get the respective code to be written in the register */
        AD9510_PLL_3_CP_CURRENT_W(
            __cp_current/AD9510_PLL3_CP_CURRENT_MIN - 1);
    _smch_ad9510_write_8 (self, AD9510_REG_PLL_3, &data);

    _smch_ad9510_reg_update (self);
    /* Wait for reset to complete */
    SMCH_AD9510_WAIT_DFLT;

err_smpr_write:
    return err;
}

smch_err_e smch_ad9510_get_cp_current (smch_ad9510_t *self, uint32_t *cp_current)
{
    smch_err_e err = SMCH_SUCCESS;

    _smch_ad9510_read_8 (self, AD9510_REG_PLL_3, (uint8_t *) cp_current);
    /* Get the CP current in uA */
    *cp_current = (AD9510_PLL_3_CP_CURRENT_R(*cp_current) + 1) *
        AD9510_PLL3_CP_CURRENT_MIN;

    return err;
}

/* Enable or disable an AD9510 output.
 * out_en parameter is a bitmask with the following meaning:
 * bit 1 = output enable
 * bit 0 = output disabled*/
smch_err_e smch_ad9510_set_outputs (smch_ad9510_t *self, uint32_t *out_en)
{
    smch_err_e err = SMCH_SUCCESS;
    uint32_t __out_en = AD9510_OUTPUT_EN_R(*out_en);

    ASSERT_TEST(*out_en < AD9510_OUTPUT_EN_MASK+1 ,
            "Output enable selection is out of range", err_smpr_write,
            SMCH_ERR_INV_FUNC_PARAM);

    uint8_t data = 0;

    DBE_DEBUG (DBG_SM_CH | DBG_LVL_TRACE,
            "[sm_ch:ad9510] Output control enable word: 0x%02X\n", __out_en);

    uint32_t i;
    /* LVPECL outputs */
    for (i = 0; i < AD9510_NUM_LVPECL_OUTPUTS; ++i, __out_en >>=
            AD9510_OUTPUT_EN_LSB_SIZE) {

        _smch_ad9510_read_8 (self, AD9510_REG_OUTPUT_START+i, &data);

        /* Output disabled */
        if ((__out_en & AD9510_OUTPUT_EN_LSB_MASK) == 0) {
            DBE_DEBUG (DBG_SM_CH | DBG_LVL_INFO,
                    "[sm_ch:ad9510] Output #%u is going to be disabled\n", i);
            data = (data & ~AD9510_LVPECL_OUT_PDOWN_MASK) |
                AD9510_LVPECL_OUT_PDOWN_W(0x02); /* Safe power down */
        }
        /* Output enabled */
        else {
            DBE_DEBUG (DBG_SM_CH | DBG_LVL_INFO,
                    "[sm_ch:ad9510] Output #%u is going to be enabled\n", i);
            data = (data & ~AD9510_LVPECL_OUT_PDOWN_MASK) |
                AD9510_LVPECL_OUT_PDOWN_W(0x00); /* Power up */
        }
        _smch_ad9510_write_8 (self, AD9510_REG_OUTPUT_START+i, &data);
    }

    /* LVDS/CMOS Outputs */
    for ( ; i < AD9510_NUM_OUTPUTS; ++i, __out_en >>=
            AD9510_OUTPUT_EN_LSB_SIZE) {
        data = 0;
        _smch_ad9510_read_8 (self, AD9510_REG_OUTPUT_START+i, &data);

        /* Output disabled */
        if ((__out_en & AD9510_OUTPUT_EN_LSB_MASK) == 0) {
            DBE_DEBUG (DBG_SM_CH | DBG_LVL_INFO,
                    "[sm_ch:ad9510] Output #%u is going to be enabled\n", i);
            data |= AD9510_LVDS_CMOS_PDOWN;
        }
        /* Output enabled */
        else {
            DBE_DEBUG (DBG_SM_CH | DBG_LVL_INFO,
                    "[sm_ch:ad9510] Output #%u is going to be enabled\n", i);
            data &= ~AD9510_LVDS_CMOS_PDOWN;
        }
        _smch_ad9510_write_8 (self, AD9510_REG_OUTPUT_START+i, &data);
    }

    _smch_ad9510_reg_update (self);
    /* Wait for reset to complete */
    SMCH_AD9510_WAIT_DFLT;

err_smpr_write:
    return err;
}

smch_err_e smch_ad9510_get_outputs (smch_ad9510_t *self, uint32_t *out_en)
{
    smch_err_e err = SMCH_SUCCESS;
    *out_en = 0;

    uint8_t data = 0;
    uint32_t i;
    /* LVPECL outputs */
    for (i = 0; i < AD9510_NUM_LVPECL_OUTPUTS; ++i) {
        _smch_ad9510_read_8 (self, AD9510_REG_OUTPUT_START+i, &data);

        /* Output enable */
        if (AD9510_LVPECL_OUT_PDOWN_R(data) == 0x0 /* Enabled */) {
            *out_en |= (1 << i); /* Output is enabled */
        }
    }

    /* LVDS/CMOS Outputs */
    for ( ; i < AD9510_NUM_OUTPUTS; ++i) {
        _smch_ad9510_read_8 (self, AD9510_REG_OUTPUT_START+i, &data);

        /* Output enabled */
        if ((data & AD9510_LVDS_CMOS_PDOWN) == 0) {
            *out_en |= (1 << i); /* Output is enabled */
        }
    }

    return err;
}

smch_err_e smch_ad9510_set_pll_clk_sel (smch_ad9510_t *self, uint32_t *clk_num)
{
    smch_err_e err = SMCH_SUCCESS;
    uint32_t __clk_num = *clk_num;

    ASSERT_TEST(__clk_num > AD9510_PLL_CLK_MIN_SEL-1 &&
            __clk_num < AD9510_PLL_CLK_MAX_SEL+1,
            "Clock number is out of range", err_smpr_write,
            SMCH_ERR_INV_FUNC_PARAM);

    uint8_t data = 0;
    _smch_ad9510_read_8 (self, AD9510_REG_CLK_OPT, &data);

    /* Mask Power Down bits */
    data &= ~AD9510_CLK_OPT_CLK_PD_MASK;
    /* Select clock and power-down unused clock */
    switch (__clk_num) {
        case AD9510_PLL_CLK1_SEL:
            data |= AD9510_CLK_OPT_SEL_CLK1;
            /* Power down other clock */
            data |= AD9510_CLK_OPT_CLK2_PD;
            break;
        case AD9510_PLL_CLK2_SEL:
            data &= ~AD9510_CLK_OPT_SEL_CLK1;
            /* Power down other clock */
            data |= AD9510_CLK_OPT_CLK1_PD;
            break;
        default:
            data |= AD9510_CLK_OPT_SEL_CLK1;
            /* Power down other clock */
            data |= AD9510_CLK_OPT_CLK2_PD;
    }

    _smch_ad9510_write_8 (self, AD9510_REG_CLK_OPT, &data);

    _smch_ad9510_reg_update (self);
    /* Wait for reset to complete */
    SMCH_AD9510_WAIT_DFLT;

err_smpr_write:
    return err;
}

smch_err_e smch_ad9510_get_pll_clk_sel (smch_ad9510_t *self, uint32_t *clk_num)
{
    smch_err_e err = SMCH_SUCCESS;

    uint8_t data = 0;
    _smch_ad9510_read_8 (self, AD9510_REG_CLK_OPT, &data);

    if (data & AD9510_CLK_OPT_SEL_CLK1) {
        *clk_num = AD9510_PLL_CLK1_SEL;
    }
    else {
        *clk_num = AD9510_PLL_CLK2_SEL;
    }

    return err;
}

/***************** Static functions *****************/

static smch_err_e _smch_ad9510_init (smch_ad9510_t *self)
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

    /* According to AD9510 datasheet Rev. B, page 46, table 25, regester 0x00 (CFG_SERIAL),
     * does not need to be updated by setting the "update registers" bit. We still wait our default
     * ammount of time to be sure the chip is indeed reset */
    SMCH_AD9510_WAIT_DFLT;

    /* Clear reset */
    data &= ~AD9510_CFG_SERIAL_SOFT_RST;
    rw_err = _smch_ad9510_write_8 (self, AD9510_REG_CFG_SERIAL, &data);
    ASSERT_TEST(rw_err == sizeof(uint8_t), "Could not write to AD9510_REG_CFG_SERIAL",
            err_smpr_write, SMCH_ERR_RW_SMPR);

    /* According to AD9510 datasheet Rev. B, page 46, table 25, regester 0x00 (CFG_SERIAL),
     * does not need to be updated by setting the "update registers" bit. We still wait our default
     * ammount of time to be sure the chip is indeed reset */

    /* Wait for reset to complete */
    SMCH_AD9510_WAIT_DFLT;

err_smpr_write:
    return err;
}

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
    uint64_t __addr = ~AD9510_HDR_RW & (
                AD9510_HDR_BT_W(0x0) |
                AD9510_HDR_ADDR_W(addr)
            );
    size_t __addr_size = AD9510_INSTADDR_SIZE/SMPR_BYTE_2_BIT;
    uint32_t __data = AD9510_DATA_W(*data);
    size_t __data_size = AD9510_DATA_SIZE/SMPR_BYTE_2_BIT;

    ssize_t smpr_err = smpr_write_block (self->spi, __addr_size, __addr,
            __data_size, &__data);
    ASSERT_TEST(smpr_err > 0 && (size_t) smpr_err == __data_size,
            "Could not write to SMPR", err_smpr_write, -1);

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
    uint64_t __addr = AD9510_HDR_RW | (
                AD9510_HDR_BT_W(0x0) |
                AD9510_HDR_ADDR_W(addr)
            );
    size_t __addr_size = AD9510_INSTADDR_SIZE/SMPR_BYTE_2_BIT;
    size_t __data_size = AD9510_DATA_SIZE/SMPR_BYTE_2_BIT;

    ssize_t smpr_err = smpr_read_block (self->spi, __addr_size, __addr,
            __data_size, (uint32_t *) data);
    ASSERT_TEST(smpr_err > 0 && (size_t) smpr_err == __data_size,
            "Could not read to SMPR", err_read_write, -1);

err_read_write:
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
