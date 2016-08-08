/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "bpm_server.h"
/* Private headers */
#include "sm_io_fmc130m_4ch_defaults.h"
#include "sm_io_fmc130m_4ch_core.h"
#include "chips_addr.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, SM_IO, "[sm_io_fmc130m_4ch_core]", \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)   \
    ASSERT_HAL_ALLOC(ptr, SM_IO, "[sm_io_fmc130m_4ch_core]",    \
            smio_err_str(SMIO_ERR_ALLOC),                       \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                \
    CHECK_HAL_ERR(err, SM_IO, "[sm_io_fmc130m_4ch_core]",       \
            smio_err_str (err_type))

static smio_err_e _smio_fmc130m_4ch_set_type (smio_fmc130m_4ch_t *self,
    uint32_t type_code);

/* Creates a new instance of Device Information */
smio_fmc130m_4ch_t * smio_fmc130m_4ch_new (smio_t *parent)
{
    assert (parent);
    smio_fmc130m_4ch_t *self = (smio_fmc130m_4ch_t *) zmalloc (sizeof *self);
    ASSERT_ALLOC(self, err_self_alloc);
    uint32_t inst_id = smio_get_inst_id (parent);

    /* Check if Instance ID is within our expected limits */
    ASSERT_TEST(inst_id < NUM_FMC130M_4CH_SMIOS, "Number of FMC130M_4CH SMIOs instances exceeded",
            err_num_fmc130m_4ch_smios);

    /* FMC130M_4CH isntance 0 is the one controlling this CI */
    /* FIXME: This breaks generality for this class */
    if (inst_id == 0) {
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fmc130m_4ch_core] PCA9547 initializing, "
                " addr: 0x%08X, Inst ID: %u\n", fmc130m_4ch_pca9547_addr[inst_id],
                inst_id);

        /* Create I2C protocol for pca9547 chip */
        self->smpr_i2c_pca9547 = smpr_i2c_new (0, fmc130m_4ch_pca9547_addr[inst_id]);
        ASSERT_ALLOC(self->smpr_i2c_pca9547, err_smpr_i2c_pca9547_alloc);

        /* FPGA I2C Switch */
        self->smch_pca9547 = smch_pca9547_new (parent, FMC_130M_EEPROM_I2C_OFFS,
                smpr_i2c_get_ops (self->smpr_i2c_pca9547), 0);
        ASSERT_ALLOC(self->smch_pca9547, err_smch_pca9547_alloc);

        /* Enable default I2C channel */
        smch_pca9547_en_chan (self->smch_pca9547, FMC130M_4CH_DFLT_PCA9547_CFG);
    }
    else {
        self->smpr_i2c_pca9547 = NULL;
        self->smch_pca9547 = NULL;
    }

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fmc130m_4ch_core] 24AA64 initializing, "
            "addr: 0x%08X, Inst ID: %u\n", fmc130m_4ch_24aa64_addr[inst_id],
            inst_id);

    /* Create I2C protocol for 24aa64 chip */
    self->smpr_i2c_24aa64 = smpr_i2c_new (0, fmc130m_4ch_24aa64_addr[inst_id]);
    ASSERT_ALLOC(self->smpr_i2c_24aa64, err_smpr_i2c_24aa64_alloc);

    /* EEPROM  is on the same I2C bus as the LM75A */
    self->smch_24aa64 = smch_24aa64_new (parent, FMC_130M_LM75A_I2C_OFFS,
            smpr_i2c_get_ops (self->smpr_i2c_24aa64), 0);
    ASSERT_ALLOC(self->smch_24aa64, err_smch_24aa64_alloc);

    uint32_t data_24aa64;
#ifdef __FMC130M_4CH_EEPROM_PROGRAM__
#if __FMC130M_4CH_EEPROM_PROGRAM__==1
    data_24aa64 = FMC130M_4CH_ACTIVE_MD5;
#elif __FMC130M_4CH_EEPROM_PROGRAM__==2
    data_24aa64 = FMC130M_4CH_PASSIVE_MD5;
#else
    #error "Unsupported FMC130M_4CH program value. Valid values are \"1\" and\"2\""
#endif
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO, "[sm_io:fmc130m_4ch_core] Writing 0x%08X to EEPROM\n",
            data_24aa64);
    smch_24aa64_write_block (self->smch_24aa64, FMC130M_4CH_EEPROM_START_ADDR,
            &data_24aa64, sizeof (data_24aa64));

    /* Readback test */
    uint32_t data_24aa64_rb;
    smch_24aa64_read_block (self->smch_24aa64, FMC130M_4CH_EEPROM_START_ADDR,
            &data_24aa64_rb, sizeof (data_24aa64_rb));
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO,
            "[sm_io:fmc130m_4ch_core] 24AA64 readback: 0x%08X\n", data_24aa64_rb);
    ASSERT_TEST(data_24aa64_rb == data_24aa64, "[sm_io:fmc130m_4ch_core] EEPROM 24AA64 readback failed",
            err_smch_ad9510_alloc);
#endif

    /* Read EEPROM */
    data_24aa64 = 0x0;
    smch_24aa64_read_block (self->smch_24aa64, FMC130M_4CH_EEPROM_START_ADDR,
            &data_24aa64, sizeof (data_24aa64));
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO,
            "[sm_io:fmc130m_4ch_core] EEPROM 24AA64 data: 0x%08X\n", data_24aa64);

    /* Determine the type of the FMC130M_4CH board */
    _smio_fmc130m_4ch_set_type (self, data_24aa64);

    return self;

#ifdef __FMC130M_4CH_EEPROM_PROGRAM__
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
err_num_fmc130m_4ch_smios:
    free (self);
err_self_alloc:
    return NULL;
}

/* Destroy an instance of the Device Information */
smio_err_e smio_fmc130m_4ch_destroy (smio_fmc130m_4ch_t **self_p)
{
    assert (self_p);

    if (*self_p) {
        smio_fmc130m_4ch_t *self = *self_p;

        smch_24aa64_destroy (&self->smch_24aa64);
        smpr_i2c_destroy (&self->smpr_i2c_24aa64);

        if (self->smch_pca9547 != NULL) {
            smch_pca9547_destroy (&self->smch_pca9547);
        }
        if (self->smpr_i2c_pca9547 != NULL) {
            smpr_i2c_destroy (&self->smpr_i2c_pca9547);
        }

        free (self);
        *self_p = NULL;
    }

    return SMIO_SUCCESS;
}

static smio_err_e _smio_fmc130m_4ch_set_type (smio_fmc130m_4ch_t *self,
    uint32_t type_code)
{
    assert (self);
    smio_err_e err = SMIO_SUCCESS;
    fmc130m_4ch_type_e type = TYPE_FMC130M_4CH_UNDEF;

    switch (type_code) {
        case FMC130M_4CH_ACTIVE_MD5:
            DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO,
                "[sm_io:fmc130m_4ch_core] Found Active FMC130M_4CH board\n");
            type = TYPE_FMC130M_4CH_ACTIVE;
            break;

        case FMC130M_4CH_PASSIVE_MD5:
            DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO,
                "[sm_io:fmc130m_4ch_core] Found Passive FMC130M_4CH board\n");
            type = TYPE_FMC130M_4CH_PASSIVE;
            break;

        default:
            DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO,
                "[sm_io:fmc130m_4ch_core] Found Undefined FMC130M_4CH board\n");
            err = SMIO_ERR_WRONG_PARAM;
            type = TYPE_FMC130M_4CH_UNDEF;
    }

    self->type = type;
    return err;
}


