/*
 * Copyright (C) 2016 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "bpm_server.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, SM_CH, "[sm_ch:isla216p]",            \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)           \
    ASSERT_HAL_ALLOC(ptr, SM_CH, "[sm_ch:isla216p]",                    \
            smch_err_str(SMCH_ERR_ALLOC),                               \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                        \
    CHECK_HAL_ERR(err, SM_CH, "[sm_ch:isla216p]",                       \
            smch_err_str (err_type))

#define SMCH_ISLA216P_WAIT_TRIES            10
#define SMCH_ISLA216P_NAME                  "ISLA216P"
#define SMCH_ISLA216P_USECS_WAIT            1000
#define SMCH_ISLA216P_WAIT(usecs)           usleep(usecs)
#define SMCH_ISLA216P_WAIT_DFLT             SMCH_ISLA216P_WAIT(SMCH_ISLA216P_USECS_WAIT)

struct _smch_isla216p_t {
    smpr_t *proto;                    /* PROTO protocol object */
};

static smch_err_e _smch_isla216p_init (smch_isla216p_t *self);
/* Read/Write 1-byte functions */
static ssize_t _smch_isla216p_write_8 (smch_isla216p_t *self, uint8_t addr,
        const uint8_t *data);
static ssize_t _smch_isla216p_read_8 (smch_isla216p_t *self, uint8_t addr,
        uint8_t *data);

/* Creates a new instance of the SMCH ISLA216P */
smch_isla216p_t * smch_isla216p_new (smio_t *parent, uint64_t base,
        const smpr_proto_ops_t *reg_ops, int verbose)
{
    (void) verbose;
    assert (parent);

    smch_isla216p_t *self = (smch_isla216p_t *) zmalloc (sizeof *self);
    ASSERT_ALLOC(self, err_self_alloc);

    self->proto = smpr_new (SMCH_ISLA216P_NAME, parent, reg_ops, verbose);
    ASSERT_ALLOC(self->proto, err_proto_alloc);

    /* Initalize the PROTO protocol */
    int smpr_err = smpr_open (self->proto, base, NULL /* Default parameters are fine */);
    ASSERT_TEST(smpr_err == 0, "Could not initialize SMPR protocol", err_smpr_init);

    DBE_DEBUG (DBG_SM_CH | DBG_LVL_INFO, "[sm_ch:isla216p] Created instance of SMCH\n");

    smch_err_e err =  _smch_isla216p_init (self);
    ASSERT_TEST(err == SMCH_SUCCESS, "Could not initialize ISLA216P", err_smch_init);

    return self;

err_smch_init:
    smpr_release (self->proto);
err_smpr_init:
    smpr_destroy (&self->proto);
err_proto_alloc:
    free (self);
err_self_alloc:
    return NULL;
}

/* Destroy an instance of the SMCH ISLA216P */
smch_err_e smch_isla216p_destroy (smch_isla216p_t **self_p)
{
    assert (self_p);

    if (*self_p) {
        smch_isla216p_t *self = *self_p;

        smpr_release (self->proto);
        smpr_destroy (&self->proto);
        free (self);
        *self_p = NULL;
    }

    return SMCH_SUCCESS;
}

smch_err_e smch_isla216p_write_8 (smch_isla216p_t *self, uint8_t addr,
        const uint8_t *data)
{
    return (_smch_isla216p_write_8 (self, addr, data) == sizeof(uint8_t))?
        SMCH_SUCCESS : SMCH_ERR_RW_SMPR;
}

smch_err_e smch_isla216p_read_8 (smch_isla216p_t *self, uint8_t addr,
        uint8_t *data)
{
    return (_smch_isla216p_read_8 (self, addr, data) == sizeof(uint8_t))?
        SMCH_SUCCESS : SMCH_ERR_RW_SMPR;
}

smch_err_e smch_isla216p_set_test_mode (smch_isla216p_t *self, uint8_t mode)
{
    smch_err_e err = SMCH_SUCCESS;
    ssize_t rw_err = -1;

    uint8_t data = 0;
    rw_err = _smch_isla216p_read_8 (self, ISLA216P_REG_TESTIO, &data);
    ASSERT_TEST(rw_err == sizeof(uint8_t), "Could not read from TESTIO register",
            err_smpr_read, SMCH_ERR_RW_SMPR);

    data = (data & ~ISLA216P_TESTIO_OUTMODE_MASK) | ISLA216P_TESTIO_OUTMODE_W(mode);
    rw_err = _smch_isla216p_write_8 (self, ISLA216P_REG_TESTIO, &data);
    ASSERT_TEST(rw_err == sizeof(uint8_t), "Could not write to TESTIO register",
            err_smpr_write, SMCH_ERR_RW_SMPR);

    SMCH_ISLA216P_WAIT_DFLT;

err_smpr_write:
err_smpr_read:
    return err;
}

smch_err_e smch_isla216p_get_chipid (smch_isla216p_t *self, uint8_t *chipid)
{
    smch_err_e err = SMCH_SUCCESS;
    ssize_t rw_err = -1;

    rw_err = _smch_isla216p_read_8 (self, ISLA216P_REG_CHIPID, chipid);
    ASSERT_TEST(rw_err == sizeof(uint8_t), "Could not read from CHIPID register",
            err_smpr_read, SMCH_ERR_RW_SMPR);

    SMCH_ISLA216P_WAIT_DFLT;

err_smpr_read:
    return err;
}

smch_err_e smch_isla216p_get_chipver (smch_isla216p_t *self, uint8_t *chipver)
{
    smch_err_e err = SMCH_SUCCESS;
    ssize_t rw_err = -1;

    rw_err = _smch_isla216p_read_8 (self, ISLA216P_REG_CHIPVER, chipver);
    ASSERT_TEST(rw_err == sizeof(uint8_t), "Could not read from CHIPVER register",
            err_smpr_read, SMCH_ERR_RW_SMPR);

    SMCH_ISLA216P_WAIT_DFLT;

err_smpr_read:
    return err;
}

/***************** Static functions *****************/

static smch_err_e _smch_isla216p_init (smch_isla216p_t *self)
{
    smch_err_e err = SMCH_SUCCESS;
    ssize_t rw_err = -1;

    /* Turn on Bidirectional PROTO */
    uint8_t data = ISLA216P_PORTCONFIG_SDO_ACTIVE;
    DBE_DEBUG (DBG_SM_CH | DBG_LVL_INFO,
            "[sm_ch:isla216p] Writing 0x%02X to addr 0x%02X\n", data, ISLA216P_REG_PORTCONFIG);
    rw_err = _smch_isla216p_write_8 (self, ISLA216P_REG_PORTCONFIG, &data);
    ASSERT_TEST(rw_err == sizeof(uint8_t), "Could not write to ISLA216P_REG_PORTCONFIG",
            err_smpr_write, SMCH_ERR_RW_SMPR);

    SMCH_ISLA216P_WAIT_DFLT;

#if 0
    /* Reset registers */
    data |= ISLA216P_PORTCONFIG_SOFT_RESET;
    DBE_DEBUG (DBG_SM_CH | DBG_LVL_INFO,
            "[sm_ch:isla216p] Writing 0x%02X to addr 0x%02X\n", data, ISLA216P_REG_PORTCONFIG);
    rw_err = _smch_isla216p_write_8 (self, ISLA216P_REG_PORTCONFIG, &data);
    ASSERT_TEST(rw_err == sizeof(uint8_t), "Could not write to ISLA216P_REG_PORTCONFIG",
            err_smpr_write, SMCH_ERR_RW_SMPR);

    /* Clear reset */
    data &= ~ISLA216P_PORTCONFIG_SOFT_RESET;
    rw_err = _smch_isla216p_write_8 (self, ISLA216P_REG_PORTCONFIG, &data);
    ASSERT_TEST(rw_err == sizeof(uint8_t), "Could not write to ISLA216P_REG_PORTCONFIG",
            err_smpr_write, SMCH_ERR_RW_SMPR);
#endif

    data = ISLA216P_NAPSLP_W(ISLA216P_NAPSLP_NORMAL_OPERATION);
    rw_err = _smch_isla216p_write_8 (self, ISLA216P_REG_NAPSLP, &data);
    ASSERT_TEST(rw_err == sizeof(uint8_t), "Could not write to ISLA216P_REG_NAPSLP",
            err_smpr_write, SMCH_ERR_RW_SMPR);

    SMCH_ISLA216P_WAIT_DFLT;

err_smpr_write:
    return err;
}

static ssize_t _smch_isla216p_write_8 (smch_isla216p_t *self, uint8_t addr,
        const uint8_t *data)
{
    ssize_t err = sizeof(uint8_t);

    /* We do only 24-bit cycles composed of:
     *
     * 16-bit instruction bits + 8-bit data bits
     *
     * Source: ISLA216P datasheet, pages 22,23,24
     * */

    /* We transmit a WRITE operation, with 1 byte transfer, with address as "addr"
     * and data as "data" */
    uint32_t __addr = ~ISLA216P_HDR_RW & (
                ISLA216P_HDR_BT_W(0x0) |
                ISLA216P_HDR_ADDR_W(addr)
            );
    size_t __addr_size = ISLA216P_INSTADDR_SIZE/SMPR_BYTE_2_BIT;
    uint32_t __data = ISLA216P_DATA_W(*data);
    size_t __data_size = ISLA216P_DATA_SIZE/SMPR_BYTE_2_BIT;

    ssize_t smpr_err = smpr_write_block (self->proto, __addr_size, 
            __addr, __data_size, &__data);
    ASSERT_TEST(smpr_err == __data_size, "Could not write to SMPR",
            err_smpr_write, -1);

err_smpr_write:
    return err;
}

static ssize_t _smch_isla216p_read_8 (smch_isla216p_t *self, uint8_t addr,
        uint8_t *data)
{
    ssize_t err = sizeof(uint8_t);

    /* We do Instruction + Data transactions composed of:
     *
     * 16-bit instruction bits + 8-bit data bits (up to 4 beats)
     *
     * Source: ISLA216P datasheet, pages 22,23,24
     * */

    /* We transmit a READ operation, with 1 byte transfer, with address as "addr"  */
    uint32_t __addr = ISLA216P_HDR_RW | (
                ISLA216P_HDR_BT_W(0x0) |
                ISLA216P_HDR_ADDR_W(addr)
            );
    size_t __addr_size = ISLA216P_INSTADDR_SIZE/SMPR_BYTE_2_BIT;
    uint32_t __data = 0;
    size_t __data_size = ISLA216P_DATA_SIZE/SMPR_BYTE_2_BIT;

    ssize_t smpr_err = smpr_read_block (self->proto, __addr_size,
            __addr, __data_size, &__data);
    ASSERT_TEST(smpr_err == __data_size, "Could not write to SMPR",
            err_smpr_write, -1);

    /* Only the 8 LSB are valid for one byte reading (ISLA216P_HDR_BT_W(0x0)) */
    memcpy(data, &__data, sizeof(uint8_t));

err_smpr_write:
    return err;
}
