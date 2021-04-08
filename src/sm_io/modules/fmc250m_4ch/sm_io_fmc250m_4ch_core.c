/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "halcs_server.h"
/* Private headers */
#include "sm_io_fmc250m_4ch_defaults.h"
#include "sm_io_fmc250m_4ch_core.h"
#include "chips_addr.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, SM_IO, "[sm_io_fmc250m_4ch_core]", \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)   \
    ASSERT_HAL_ALLOC(ptr, SM_IO, "[sm_io_fmc250m_4ch_core]",    \
            smio_err_str(SMIO_ERR_ALLOC),                       \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                \
    CHECK_HAL_ERR(err, SM_IO, "[sm_io_fmc250m_4ch_core]",       \
            smio_err_str (err_type))

static smio_err_e _smio_fmc250m_4ch_set_type (smio_fmc250m_4ch_t *self,
    uint32_t type_code);

/* Creates a new instance of Device Information */
smio_fmc250m_4ch_t * smio_fmc250m_4ch_new (smio_t *parent)
{
    assert (parent);
    smio_fmc250m_4ch_t *self = (smio_fmc250m_4ch_t *) zmalloc (sizeof *self);
    ASSERT_ALLOC(self, err_self_alloc);
    uint32_t inst_id = smio_get_inst_id (parent);

    /* Check if Instance ID is within our expected limits */
    ASSERT_TEST(inst_id < NUM_FMC250M_4CH_SMIOS, "Number of FMC250M_4CH SMIOs instances exceeded",
            err_num_fmc250m_4ch_smios);

    /* FMC250M_4CH isntance 0 is the one controlling this CI */
    /* FIXME: This breaks generality for this class */
    if (inst_id == 0) {
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fmc250m_4ch_core] PCA9547 initializing, "
                " addr: 0x%08X, Inst ID: %u\n", fmc250m_4ch_pca9547_addr[inst_id],
                inst_id);
        /* FPGA I2C Switch */
#if 0
        self->smpr_i2c_pca9547 = smpr_i2c_new (0, fmc250m_4ch_pca9547_addr[inst_id]);
        ASSERT_ALLOC(self->smpr_i2c_pca9547, err_smpr_i2c_pca9547_alloc);

        self->smch_pca9547 = smch_pca9547_new (parent, FMC_250M_EEPROM_I2C_OFFS,
                smpr_i2c_get_ops (self->smpr_i2c_pca9547), 0);
        ASSERT_ALLOC(self->smch_pca9547, err_smch_pca9547_alloc);

        /* Enable default I2C channel */
        smch_pca9547_en_chan (self->smch_pca9547, FMC250M_4CH_DFLT_PCA9547_CFG);
    }
    else {
#endif
        self->smpr_i2c_pca9547 = NULL;
        self->smch_pca9547 = NULL;
    }

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fmc250m_4ch_core] 24AA64 initializing, "
            "addr: 0x%08X, Inst ID: %u\n", fmc250m_4ch_24aa64_addr[inst_id],
            inst_id);
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO,
            "[sm_io:fmc250m_4ch_core] pre new EEPROM 24AA64 data: 0x%08X\n", 0);
#if 0

    /* Create I2C protocol for 24aa64 chip */
    self->smpr_i2c_24aa64 = smpr_i2c_new (0, fmc250m_4ch_24aa64_addr[inst_id]);
    ASSERT_ALLOC(self->smpr_i2c_24aa64, err_smpr_i2c_24aa64_alloc);

    /* EEPROM  is on the same I2C bus as the LM75A */
    self->smch_24aa64 = smch_24aa64_new (parent, FMC_250M_LM75A_I2C_OFFS,
            smpr_i2c_get_ops (self->smpr_i2c_24aa64), 0);
    ASSERT_ALLOC(self->smch_24aa64, err_smch_24aa64_alloc);
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO,
            "[sm_io:fmc250m_4ch_core] post new 24AA64 data: 0x%08X\n", 0);
    ASSERT_ALLOC(self->smch_24aa64, err_smch_24aa64_alloc);

    uint32_t data_24aa64;
#ifdef __FMC250M_4CH_EEPROM_PROGRAM__
#if __FMC250M_4CH_EEPROM_PROGRAM__==1
    data_24aa64 = FMC250M_4CH_ACTIVE_MD5;
#elif __FMC250M_4CH_EEPROM_PROGRAM__==2
    data_24aa64 = FMC250M_4CH_PASSIVE_MD5;
#else
    #error "Unsupported FMC250M_4CH program value. Valid values are \"1\" and\"2\""
#endif
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO, "[sm_io:fmc250m_4ch_core] Writing 0x%08X to EEPROM\n",
            data_24aa64);
    smch_24aa64_write_block (self->smch_24aa64, FMC250M_4CH_EEPROM_START_ADDR,
            &data_24aa64, sizeof (data_24aa64));

    /* Readback test */
    uint32_t data_24aa64_rb;
    smch_24aa64_read_block (self->smch_24aa64, FMC250M_4CH_EEPROM_START_ADDR,
            &data_24aa64_rb, sizeof (data_24aa64_rb));
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO,
            "[sm_io:fmc250m_4ch_core] 24AA64 readback: 0x%08X\n", data_24aa64_rb);
    ASSERT_TEST(data_24aa64_rb == data_24aa64, "[sm_io:fmc250m_4ch_core] EEPROM 24AA64 readback failed",
            err_smch_ad9510_alloc);
#endif
    /* Read EEPROM */
    data_24aa64 = 0x0;
    smch_24aa64_read_block (self->smch_24aa64, FMC250M_4CH_EEPROM_START_ADDR,
            &data_24aa64, sizeof (data_24aa64));
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO,
            "[sm_io:fmc250m_4ch_core] EEPROM 24AA64 data: 0x%08X\n", data_24aa64);
#endif

    /* Determine the type of the FMC250M_4CH board */
#if 0
    _smio_fmc250m_4ch_set_type (self, data_24aa64);
#endif
    _smio_fmc250m_4ch_set_type (self, 0x0);

    self->smpr_spi_isla216p_adc = zmalloc (sizeof *self->smpr_spi_isla216p_adc * NUM_FMC250M_4CH_ISLA216P);
    ASSERT_ALLOC(self->smpr_spi_isla216p_adc, err_smpr_spi_isla216p_adc_arr_alloc);
    
    self->smch_isla216p_adc = zmalloc (sizeof *self->smch_isla216p_adc * NUM_FMC250M_4CH_ISLA216P);
    ASSERT_ALLOC(self->smch_isla216p_adc, err_smch_isla216p_adc_arr_alloc);

    /* Setup ISLA216P ADC SPI communication */
    uint32_t i;
    for (i = 0; i < NUM_FMC250M_4CH_ISLA216P; ++i) {
        /* Create SPI protocol for ISLA216P chips */
        self->smpr_spi_isla216p_adc[i] = smpr_spi_new (fmc250m_4ch_isla216p_addr[inst_id][i], 1 /* addr_msb */);
        ASSERT_ALLOC(self->smpr_spi_isla216p_adc[i], err_smpr_spi_isla216p_adc_alloc);

        self->smch_isla216p_adc[i] = NULL;
        self->smch_isla216p_adc[i] = smch_isla216p_new (parent, FMC_250M_ISLA216P_SPI_OFFS,
            smpr_spi_get_ops (self->smpr_spi_isla216p_adc[i]), 0);
        ASSERT_ALLOC(self->smch_isla216p_adc[i], err_smch_isla216p_adc);

        uint8_t chipid = 0;
        uint8_t chipver = 0;

        /* Read ISLA216P ID */
        smch_isla216p_get_chipid (self->smch_isla216p_adc[i], &chipid);
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO, "[sm_io:fmc250m_4ch_core] ISLA216P0 CHIPID: 0x%02X\n", chipid);
        /* Read ISLA216P Version */
        smch_isla216p_get_chipver (self->smch_isla216p_adc[i], &chipver);
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO, "[sm_io:fmc250m_4ch_core] ISLA216P0 CHIPVER: 0x%02X\n", chipver);
        /* Read ISLA216P temperature */
        uint16_t temp = 0;
        smch_isla216p_get_temp (self->smch_isla216p_adc[i], &temp);
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO, "[sm_io:fmc250m_4ch_core] ISLA216P0 TEMP: 0x%04X\n", temp);
    }

    return self;

err_smch_isla216p_adc:
    for (i = 0; i < NUM_FMC250M_4CH_ISLA216P; ++i) {
        smch_isla216p_destroy (&self->smch_isla216p_adc[i]);
    }
err_smpr_spi_isla216p_adc_alloc:
    for (i = 0; i < NUM_FMC250M_4CH_ISLA216P; ++i) {
        smpr_spi_destroy (&self->smpr_spi_isla216p_adc[i]);
    }
    free (self->smch_isla216p_adc);
err_smch_isla216p_adc_arr_alloc:
    free (self->smpr_spi_isla216p_adc);
err_smpr_spi_isla216p_adc_arr_alloc:
#if 0
#ifdef __FMC250M_4CH_EEPROM_PROGRAM__
err_smch_ad9510_alloc:
    smch_24aa64_destroy (&self->smch_24aa64);
#endif
err_smch_24aa64_alloc:
    smpr_i2c_destroy (&self->smpr_i2c_24aa64);
err_smpr_i2c_24aa64_alloc:
    if (self->smch_pca9547 != NULL) {
        smch_pca9547_destroy (&self->smch_pca9547);
    }
err_smch_pca9547_alloc:
    if (self->smpr_i2c_pca9547 != NULL) {
        smpr_i2c_destroy (&self->smpr_i2c_pca9547);
    }
err_smpr_i2c_pca9547_alloc:
#endif
err_num_fmc250m_4ch_smios:
    free (self);
err_self_alloc:
    return NULL;
}

/* Destroy an instance of the Device Information */
smio_err_e smio_fmc250m_4ch_destroy (smio_fmc250m_4ch_t **self_p)
{
    assert (self_p);

    if (*self_p) {
        smio_fmc250m_4ch_t *self = *self_p;

        /* Destroy all ISLA216P instances */
        uint32_t i;
        for (i = 0; i < NUM_FMC250M_4CH_ISLA216P; ++i) {
            smch_isla216p_destroy (&self->smch_isla216p_adc[i]);
        }
        for (i = 0; i < NUM_FMC250M_4CH_ISLA216P; ++i) {
            smpr_spi_destroy (&self->smpr_spi_isla216p_adc[i]);
        }

        smch_24aa64_destroy (&self->smch_24aa64);
        smpr_i2c_destroy (&self->smpr_i2c_24aa64);

        if (self->smch_pca9547 != NULL) {
            smch_pca9547_destroy (&self->smch_pca9547);
        }
        if (self->smpr_i2c_pca9547 != NULL) {
            smpr_i2c_destroy (&self->smpr_i2c_pca9547);
        }

        free (self->smch_isla216p_adc);
        free (self->smpr_spi_isla216p_adc);
        free (self);
        *self_p = NULL;
    }

    return SMIO_SUCCESS;
}

static smio_err_e _smio_fmc250m_4ch_set_type (smio_fmc250m_4ch_t *self,
    uint32_t type_code)
{
    assert (self);
    smio_err_e err = SMIO_SUCCESS;
    fmc250m_4ch_type_e type = TYPE_FMC250M_4CH_UNDEF;

    switch (type_code) {
        case FMC250M_4CH_ACTIVE_MD5:
            DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO,
                "[sm_io:fmc250m_4ch_core] Found Active FMC250M_4CH board\n");
            type = TYPE_FMC250M_4CH_ACTIVE;
            break;

        case FMC250M_4CH_PASSIVE_MD5:
            DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO,
                "[sm_io:fmc250m_4ch_core] Found Passive FMC250M_4CH board\n");
            type = TYPE_FMC250M_4CH_PASSIVE;
            break;

        default:
            DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO,
                "[sm_io:fmc250m_4ch_core] Found Undefined FMC250M_4CH board\n");
            err = SMIO_ERR_WRONG_PARAM;
//            type = TYPE_FMC250M_4CH_UNDEF;
            type = TYPE_FMC250M_4CH_ACTIVE;
    }

    self->type = type;
    return err;
}


