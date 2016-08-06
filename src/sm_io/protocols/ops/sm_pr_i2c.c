/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

/* This file is based on the work of Andrzej Wojenski for the FCS project,
 * available, to my knowledge, only in https://github.com/lerwys/fcs
 *
 * Description : Software driver for Wishbone I2C IP core
 *               (standard I2C Master Controller core from OpenCores)
 */

#include "bpm_server.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, SM_PR, "[sm_pr:i2c]",         \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)   \
    ASSERT_HAL_ALLOC(ptr, SM_PR, "[sm_pr:i2c]",                 \
            smpr_err_str(SMPR_ERR_ALLOC),                       \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                \
    CHECK_HAL_ERR(err, SM_PR, "[sm_pr:i2c]",                    \
            smpr_err_str (err_type))

#define SM_PR_I2C_MAX_TRIES                 10
#define SM_PR_I2C_USLEEP                    1000

/* Device endpoint */
typedef struct {
    uint64_t base;              /* Core base address */
    uint32_t sys_freq;          /* System clock [Hz] */
    uint32_t i2c_freq;          /* I2C clock [Hz] */
    uint32_t init_config;       /* I2C initial config register */
    i2c_mode_e mode;            /* I2C mode */
} smpr_proto_i2c_t;

/* Protocol object specification */
struct _smpr_i2c_t {
    /* Must be located first */
    smpr_proto_ops_t proto_ops;         /* I2C protocol operations */
    uint32_t rep_start;                 /* I2C repetitive start */
    uint32_t trans_size;                /* I2C transaction size */
    uint32_t addr;                      /* I2C transaction size */
};

static smpr_err_e _i2c_init (smpr_t *self);
static ssize_t _i2c_check_transfer (smpr_t *self, bool ack_check);
static smpr_err_e _i2c_set_mode (smpr_t *self);
static ssize_t _i2c_read_write_header (smpr_t *self, bool rw);
static ssize_t _i2c_read_generic (smpr_t *self, uint8_t *data,
        size_t size);
static ssize_t _i2c_write_generic (smpr_t *self, uint8_t *data,
        size_t size);

/************ Our methods implementation **********/

/* Creates a new instance of the proto_i2c */
static smpr_proto_i2c_t * smpr_proto_i2c_new (uint64_t base)
{
    smpr_proto_i2c_t *self = (smpr_proto_i2c_t *) zmalloc (sizeof *self);
    ASSERT_ALLOC (self, err_smpr_proto_i2c_alloc);

    self->base = base;

    return self;

err_smpr_proto_i2c_alloc:
    return NULL;
}

/* Destroy an instance of the proto_i2c */
static smpr_err_e smpr_proto_i2c_destroy (smpr_proto_i2c_t **self_p)
{
    assert (self_p);

    if (*self_p) {
        smpr_proto_i2c_t *self = *self_p;

        free (self);
        self_p = NULL;
    }

    return SMPR_SUCCESS;
}

/************ smpr_proto_ops_i2c Implementation **********/

/* Open I2C protocol */
static int i2c_open (smpr_t *self, uint64_t base, void *args)
{
    assert (self);

    /* Create new i2c */
    smpr_proto_i2c_t *i2c_proto = smpr_proto_i2c_new (base);
    ASSERT_TEST(i2c_proto != NULL, "Could not allocate proto_handler",
            err_proto_handler_alloc);

    /* Initialize the I2C protocol with the options passed */
    i2c_proto_args_t *i2c_proto_args = NULL;
    if (args != NULL) {
        i2c_proto_args = (i2c_proto_args_t *) args;
        i2c_proto->sys_freq = i2c_proto_args->sys_freq;
        i2c_proto->i2c_freq = i2c_proto_args->i2c_freq;
        i2c_proto->init_config = i2c_proto_args->init_config;
    }
    else {
        DBE_DEBUG (DBG_SM_PR | DBG_LVL_INFO,
                "[sm_pr:i2c] I2C arguments not specified. Using default values ...\n");
        i2c_proto->sys_freq = SM_PR_I2C_DFLT_SYS_FREQ;
        i2c_proto->i2c_freq = SM_PR_I2C_DFLT_I2C_FREQ;
        i2c_proto->init_config = SM_PR_I2C_DFLT_I2C_CONFIG;
    }

    DBE_DEBUG (DBG_SM_PR | DBG_LVL_INFO,
            "[sm_pr:i2c] Using system clock = %u Hz, i2c clock = %u Hz\n"
            "\tconfig register = 0x%08X\n",
            i2c_proto->sys_freq, i2c_proto->i2c_freq, i2c_proto->init_config);

    /* Attach specific protocol handler to generic one */
    smpr_err_e err = smpr_set_handler (self, i2c_proto);
    ASSERT_TEST(err == SMPR_SUCCESS, "Could not set protocol handler",
            err_proto_handler_set);

    DBE_DEBUG (DBG_SM_PR | DBG_LVL_INFO, "[sm_pr:i2c] Initializing I2C protocol\n");
    err = _i2c_init (self);
    ASSERT_TEST(err == SMPR_SUCCESS, "Could not initialize I2C protocol handler",
            err_proto_handler_init);

    return 0;

err_proto_handler_init:
    smpr_unset_handler (self);
err_proto_handler_set:
    smpr_proto_i2c_destroy (&i2c_proto);
err_proto_handler_alloc:
    return -1;
}

/* Release I2C protocol device */
static int i2c_release (smpr_t *self)
{
    assert (self);

    /* Deattach specific protocol handler to generic one */
    smpr_proto_i2c_t *i2c_proto = (smpr_proto_i2c_t *) smpr_unset_handler (self);
    ASSERT_TEST (i2c_proto != NULL, "Could not unset protocol handler",
            err_proto_handler_unset);
    smpr_err_e err = smpr_proto_i2c_destroy (&i2c_proto);
    ASSERT_TEST (err==SMPR_SUCCESS, "Could not close device appropriately", err_dealloc);
    DBE_DEBUG (DBG_SM_PR | DBG_LVL_INFO, "[smpr:i2c] Closed I2C protocol handler\n");

    return 0;

err_dealloc:
err_proto_handler_unset:
    return -1;
}

/* Read 16-bit data from I2C */
static ssize_t i2c_read_16 (smpr_t *self, uint64_t offs, uint16_t *data)
{
    (void) offs;
    /* We want to request a read command from some off-FPGA chip. So, we
     * always use WRITE_READ mode */
    return _i2c_read_generic (self, (uint8_t *) data, sizeof(*data));
}

/* Write 16-bit data to PCIe device */
static ssize_t i2c_write_16 (smpr_t *self, uint64_t offs, const uint16_t *data)
{
    (void) offs;
    return _i2c_write_generic (self, (uint8_t *) data, sizeof(*data));
}

/* Read 32-bit data from I2C */
static ssize_t i2c_read_32 (smpr_t *self, uint64_t offs, uint32_t *data)
{
    (void) offs;
    /* We want to request a read command from some off-FPGA chip. So, we
     * always use WRITE_READ mode */
    return _i2c_read_generic (self, (uint8_t *) data, sizeof(*data));
}

/* Write 32-bit data to PCIe device */
static ssize_t i2c_write_32 (smpr_t *self, uint64_t offs, const uint32_t *data)
{
    (void) offs;
    return _i2c_write_generic (self, (uint8_t *) data, sizeof(*data));
}

/* Read 64-bit data from I2C */
static ssize_t i2c_read_64 (smpr_t *self, uint64_t offs, uint64_t *data)
{
    (void) offs;
    /* We want to request a read command from some off-FPGA chip. So, we
     * always use WRITE_READ mode */
    return _i2c_read_generic (self, (uint8_t *) data, sizeof(*data));
}

/* Write 64-bit data to PCIe device */
static ssize_t i2c_write_64 (smpr_t *self, uint64_t offs, const uint64_t *data)
{
    (void) offs;
    return _i2c_write_generic (self, (uint8_t *) data, sizeof(*data));
}

/* Read data block from PCIe device, size in bytes */
static ssize_t i2c_read_block (smpr_t *self, uint64_t offs, size_t size, uint32_t *data)
{
    (void) offs;
    /* We want to request a read command from some off-FPGA chip. So, we
     * always use WRITE_READ mode */
    return _i2c_read_generic (self, (uint8_t *) data, size);
}

/* Write data block from PCIe device, size in bytes */
static ssize_t i2c_write_block (smpr_t *self, uint64_t offs, size_t size, const uint32_t *data)
{
    (void) offs;
    return _i2c_write_generic (self, (uint8_t *) data, size);
}

/************ Static functions **********/
static smpr_err_e _i2c_init (smpr_t *self)
{
    assert (self);

    DBE_DEBUG (DBG_SM_PR | DBG_LVL_INFO, "[sm_pr:i2c] Inside _i2c_init\n");
    smpr_err_e err = SMPR_SUCCESS;
    smio_t *parent = smpr_get_parent (self);
    smpr_proto_i2c_t *i2c_proto = smpr_get_handler (self);
    ASSERT_TEST(i2c_proto != NULL, "Could not get SMPR protocol handler",
            err_proto_handler, -1);

    DBE_DEBUG (DBG_SM_PR | DBG_LVL_INFO, "[sm_pr:i2c] Calculating I2C frequency\n");
    /* Set I2C clock */
    float f_freq = (float) i2c_proto->sys_freq/(5.0 * (float) i2c_proto->i2c_freq) - 1.0;
    uint32_t freq_lo = ((uint16_t) f_freq) & 0xFF;
    uint32_t freq_hi = (((uint16_t) f_freq) & 0xFF00) >> 8;

    /* Configure I2C clock register */
    DBE_DEBUG (DBG_SM_PR | DBG_LVL_TRACE,
            "[sm_pr:i2c] I2C frequency LO = %u, I2C frequency HI = %u\n", freq_lo,
            freq_hi);

    /* Write Clock Low Part */
    RW_REPLY_TYPE rw_err = SET_PARAM(parent, sm_pr_i2c, i2c_proto->base, I2C_PROTO,
            PRER_LO, /* field = NULL */, MULT_BIT_PARAM,
            /* value */ freq_lo, /* min */, /* max */, NO_CHK_FUNC, SET_FIELD);
    ASSERT_TEST(rw_err == RW_OK, "Could not set freq parameter", err_exit,
            SMPR_ERR_RW_SMIO);

#ifdef SM_PR_READBACK
    /* Readback test */
    uint32_t freq_lo_rb;
    smio_thsafe_client_read_32 (parent, i2c_proto->base | I2C_PROTO_REG_PRER_LO,
            &freq_lo_rb);
    DBE_DEBUG (DBG_SM_PR | DBG_LVL_TRACE,
            "[sm_pr:i2c] I2C frequency low readback value = %u\n",
            I2C_PROTO_PRER_LO_R(freq_lo_rb));
    ASSERT_TEST(I2C_PROTO_PRER_LO_R(freq_lo_rb) == freq_lo,
            "[sm_pr:i2c] I2C frequency low readback failed",
            err_exit, SMPR_ERR_RW_SMIO);
#endif

    /* Write Clock High Part */
    rw_err = SET_PARAM(parent, sm_pr_i2c, i2c_proto->base, I2C_PROTO,
            PRER_HI, /* field = NULL */, MULT_BIT_PARAM,
            /* value */ freq_hi, /* min */, /* max */, NO_CHK_FUNC, SET_FIELD);
    ASSERT_TEST(rw_err == RW_OK, "Could not set freq parameter", err_exit,
            SMPR_ERR_RW_SMIO);

#ifdef SM_PR_READBACK
    /* Readback test */
    uint32_t freq_hi_rb;
    smio_thsafe_client_read_32 (parent, i2c_proto->base | I2C_PROTO_REG_PRER_HI,
            &freq_hi_rb);
    DBE_DEBUG (DBG_SM_PR | DBG_LVL_TRACE,
            "[sm_pr:i2c] I2C frequency high readback value = %u\n",
            I2C_PROTO_PRER_HI_R(freq_hi_rb));
    ASSERT_TEST(I2C_PROTO_PRER_HI_R(freq_hi_rb) == freq_hi,
            "[sm_pr:i2c] I2C frequency high readback failed",
            err_exit, SMPR_ERR_RW_SMIO);
#endif

    /* Enable Core */
    uint32_t en = 1;
    rw_err = SET_PARAM(parent, sm_pr_i2c, i2c_proto->base, I2C_PROTO,
            CTR, EN /* field = NULL */, SINGLE_BIT_PARAM,
            /* value */ en, /* min */, /* max */, NO_CHK_FUNC, SET_FIELD);
    ASSERT_TEST(rw_err == RW_OK, "Could enable core", err_exit,
            SMPR_ERR_RW_SMIO);

#ifdef SM_PR_READBACK
    /* Readback test */
    uint32_t en_rb;
    smio_thsafe_client_read_32 (parent, i2c_proto->base | I2C_PROTO_REG_CTR,
            &en_rb);
    DBE_DEBUG (DBG_SM_PR | DBG_LVL_TRACE,
            "[sm_pr:i2c] I2C enable readback value = %u\n",
            I2C_PROTO_CTR_EN & en_rb);
    ASSERT_TEST(I2C_PROTO_CTR_EN & en_rb,
            "[sm_pr:i2c] I2C enable readback failed",
            err_exit, SMPR_ERR_RW_SMIO);
#endif

    i2c_proto->mode = I2C_MODE_NORMAL;

err_exit:
err_proto_handler:
    return err;
}

static ssize_t _i2c_check_transfer (smpr_t *self, bool ack_check)
{
    assert (self);

    ssize_t err = 0;
    smio_t *parent = smpr_get_parent (self);
    smpr_proto_i2c_t *i2c_proto = smpr_get_handler (self);
    ASSERT_TEST(i2c_proto != NULL, "Could not get SMPR protocol handler",
            err_proto_handler, -1);

    /* Check for completion */
    uint32_t tip = 0;
    uint32_t tries = 0;
    RW_REPLY_TYPE rw_err = RW_OK;

    while (1) {
        rw_err = GET_PARAM(parent, sm_pr_i2c, i2c_proto->base, I2C_PROTO,
                SR, TIP, SINGLE_BIT_PARAM, tip, NO_FMT_FUNC);
        ASSERT_TEST(rw_err == RW_OK, "Could not get TIP parameter", err_exit, -1);

        if (!tip) {
            break;
        }

        usleep (SM_PR_I2C_USLEEP);
        ++tries;
        ASSERT_TEST(tries < SM_PR_I2C_MAX_TRIES, "Transfer timeout", err_exit, -1);
    }

    DBE_DEBUG (DBG_SM_PR | DBG_LVL_TRACE,
            "[sm_pr:i2c] _i2c_check_transfer: Wait completed successfully\n");

    if (ack_check == 0) { /* not checking if core is in reading mode */
        goto err_exit;
    }

    /* Check RX ACK (should be 0) */
    uint32_t rxack = 0;
    rw_err = GET_PARAM(parent, sm_pr_i2c, i2c_proto->base, I2C_PROTO,
            SR, RXACK, SINGLE_BIT_PARAM, rxack, NO_FMT_FUNC);
    ASSERT_TEST(rw_err == RW_OK, "Could not get RX Ack parameter", err_exit, -1);

    ASSERT_TEST(rxack == 0, "RX Ack should be 0", err_exit, -1);

err_exit:
err_proto_handler:
    return err;
}

static smpr_err_e _i2c_set_mode (smpr_t *self)
{
    assert (self);

    smpr_err_e err = SMPR_SUCCESS;
    smpr_proto_i2c_t *i2c_proto = smpr_get_handler (self);
    ASSERT_TEST(i2c_proto != NULL, "Could not get SMPR protocol handler",
            err_proto_handler, SMPR_ERR_PROTO_INFO);

    /* Get specific parameters */
    smpr_i2c_t *smpr_i2c = (smpr_i2c_t *) smpr_get_ops (self);

    /* Check if we must send a stop after the last byte */
    if (smpr_i2c_get_rep_start (smpr_i2c)) {
        i2c_proto->mode = I2C_MODE_REP_START;
    }
    else {
        i2c_proto->mode = I2C_MODE_NORMAL;
    }

err_proto_handler:
    return err;
}

static ssize_t _i2c_read_write_header (smpr_t *self, bool rw)
{
    assert (self);

    ssize_t err = 0;
    RW_REPLY_TYPE rw_err = RW_OK;

    smio_t *parent = smpr_get_parent (self);
    smpr_proto_i2c_t *i2c_proto = smpr_get_handler (self);
    ASSERT_TEST(i2c_proto != NULL, "Could not get SMPR protocol handler",
            err_proto_handler, -1);

    /* Get specific parameters */
    smpr_i2c_t *smpr_i2c = (smpr_i2c_t *) smpr_get_ops (self);
    uint32_t i2c_addr = smpr_i2c_get_addr (smpr_i2c);
    DBE_DEBUG (DBG_SM_PR | DBG_LVL_TRACE,
            "[sm_pr:i2c] _i2c_read_write_header: I2C addr 0x%08X\n", i2c_addr);

    /* Send address */
    uint32_t i2c_data = I2C_PROTO_TXR_ADDR_W(i2c_addr);
    DBE_DEBUG (DBG_SM_PR | DBG_LVL_TRACE,
            "[sm_pr:i2c] _i2c_read_write_header: I2C data 0x%08X\n", i2c_data);

    /* Read or Write mode*/
    if (rw) {
        i2c_data |= I2C_PROTO_TXR_RW;
    }
    else {
        i2c_data &= ~I2C_PROTO_TXR_RW;
    }
    DBE_DEBUG (DBG_SM_PR | DBG_LVL_TRACE,
            "[sm_pr:i2c] _i2c_read_write_header: I2C address w/ rw bit= 0x%02X\n", i2c_data);

    rw_err = SET_PARAM(parent, sm_pr_i2c, i2c_proto->base, I2C_PROTO, TXR,
            /* field = NULL */, MULT_BIT_PARAM, i2c_data /* value */ , /* min */, /* max */,
            NO_CHK_FUNC, SET_FIELD);
    ASSERT_TEST(rw_err == RW_OK, "Could not set I2C address parameter", err_exit, -1);
    DBE_DEBUG (DBG_SM_PR | DBG_LVL_TRACE,
            "[sm_pr:i2c] _i2c_read_write_header: TXR register = 0x%08X\n", i2c_data);

    /* Start transfer in Write mode */
    i2c_data = I2C_PROTO_CR_STA | I2C_PROTO_CR_WR;

    rw_err = SET_PARAM(parent, sm_pr_i2c, i2c_proto->base, I2C_PROTO, CR,
            /* field = NULL */, MULT_BIT_PARAM, i2c_data /* value */ , /* min */, /* max */,
            NO_CHK_FUNC, SET_FIELD);
    ASSERT_TEST(rw_err == RW_OK, "Could not start I2C transfer", err_exit, -1);
    DBE_DEBUG (DBG_SM_PR | DBG_LVL_TRACE,
            "[sm_pr:i2c] _i2c_read_write_header: CR register = 0x%08X\n", i2c_data);

    /* Check transfer */
    err = _i2c_check_transfer (self, true);
    ASSERT_TEST(err == 0, "Could not check I2C transfer", err_exit, -1);

    err = sizeof (uint8_t);

err_exit:
err_proto_handler:
    return err;
}

/* Generic write to I2C */
static ssize_t _i2c_write_generic (smpr_t *self, uint8_t *data,
        size_t size)
{
    assert (self);

    ssize_t err = 0;
    ssize_t num_bytes = 0;
    RW_REPLY_TYPE rw_err = RW_OK;
    ASSERT_TEST(size*SMPR_BYTE_2_BIT /* bits */ <
            SMPR_PROTO_I2C_TRANS_SIZE_FLAGS_MAX+1, "Invalid size for I2C transfer",
            err_inv_size, -1);

    smio_t *parent = smpr_get_parent (self);
    smpr_proto_i2c_t *i2c_proto = smpr_get_handler (self);
    ASSERT_TEST(i2c_proto != NULL, "Could not get SMPR protocol handler",
            err_proto_handler, -1);

    err = _i2c_read_write_header (self, false /* write mode*/);
    ASSERT_TEST(err > 0, "Could not write I2C header", err_exit, -1);

    /* Set I2C mode: NORMAL, REP_START */
    _i2c_set_mode (self);

    /* Get specific parameters */
    smpr_i2c_t *smpr_i2c = (smpr_i2c_t *) smpr_get_ops (self);
    uint32_t trans_size = smpr_i2c_get_trans_size (smpr_i2c)/SMPR_BYTE_2_BIT; /* in bytes */
    ASSERT_TEST(trans_size*SMPR_BYTE_2_BIT /* bits */ <
            SMPR_PROTO_I2C_TRANS_SIZE_FLAGS_MAX+1, "Invalid transfer size for I2C",
            err_inv_size, -1);

    if (trans_size != size) {
        DBE_DEBUG (DBG_SM_PR | DBG_LVL_WARN,
                "[sm_pr:i2c] _i2c_write_generic: Data size differs from Transfer size.\n"
                "\tChoosing the smallest value between trans_size (%u) and size (%zu)\n", trans_size, size);
    }

    /* Choose the smallest one */
    trans_size = (trans_size > size) ? size : trans_size;
    DBE_DEBUG (DBG_SM_PR | DBG_LVL_TRACE,
            "[sm_pr:i2c] _i2c_write_generic: Transmission size = %u bytes\n", trans_size);

    /* Send actual data, byte by byte*/
    uint32_t i;
    for (i = 0; i < trans_size; ++i) {
        /* Write data to transmit register */
        rw_err = SET_PARAM(parent, sm_pr_i2c, i2c_proto->base, I2C_PROTO, TXR,
                /* field = NULL */, MULT_BIT_PARAM, *(data + i) /* value */,
                /* min */, /* max */, NO_CHK_FUNC, SET_FIELD);
        ASSERT_TEST(rw_err == RW_OK, "Could not send I2C data", err_exit, -1);
        DBE_DEBUG (DBG_SM_PR | DBG_LVL_TRACE,
                "[sm_pr:i2c] _i2c_write_generic: TXR data register #%u = 0x%02X\n", i, *(data+i));

        uint32_t i2c_data;
        /* if this is the last byte, then stop transfer */
        if (i == (trans_size - 1) && i2c_proto->mode == I2C_MODE_NORMAL) {    /* used for repeated start */
            i2c_data = I2C_PROTO_CR_STO | I2C_PROTO_CR_WR;
        }
        else {
            i2c_data = I2C_PROTO_CR_WR;
        }

        rw_err = SET_PARAM(parent, sm_pr_i2c, i2c_proto->base, I2C_PROTO, CR,
                /* field = NULL */, MULT_BIT_PARAM, i2c_data /* value */,
                /* min */, /* max */, NO_CHK_FUNC, SET_FIELD);
        ASSERT_TEST(rw_err == RW_OK, "Could not set I2C CR register", err_exit, -1);
        DBE_DEBUG (DBG_SM_PR | DBG_LVL_TRACE,
                "[sm_pr:i2c] _i2c_write_generic: CR register #%u = 0x%08X\n", i, i2c_data);

        /* Check transfer */
        err = _i2c_check_transfer (self, true);
        ASSERT_TEST(err == 0, "Could not check I2C transfer", err_exit, -1);
        num_bytes++;
    }

    /* Transmission done */
    DBE_DEBUG (DBG_SM_PR | DBG_LVL_TRACE,
            "[sm_pr:i2c] _i2c_write_generic: Transmission done\n");

    err = num_bytes;

err_inv_size:
err_exit:
err_proto_handler:
    return err;
}

/* Generic read from I2C */
static ssize_t _i2c_read_generic (smpr_t *self, uint8_t *data,
        size_t size)
{
    assert (self);

    ssize_t err = 0;
    ssize_t num_bytes = 0;
    RW_REPLY_TYPE rw_err = RW_OK;
    ASSERT_TEST(size*SMPR_BYTE_2_BIT /* bits */ <
            SMPR_PROTO_I2C_TRANS_SIZE_FLAGS_MAX+1, "Invalid size for I2C transfer",
            err_inv_size, -1);

    smio_t *parent = smpr_get_parent (self);
    smpr_proto_i2c_t *i2c_proto = smpr_get_handler (self);
    ASSERT_TEST(i2c_proto != NULL, "Could not get SMPR protocol handler",
            err_proto_handler, -1);

    err = _i2c_read_write_header (self, true /* read mode*/);
    ASSERT_TEST(err > 0, "Could not write I2C header", err_exit, -1);

    /* Set I2C mode: NORMAL, REP_START */
    _i2c_set_mode (self);

    /* Get specific parameters */
    smpr_i2c_t *smpr_i2c = (smpr_i2c_t *) smpr_get_ops (self);
    uint32_t trans_size = smpr_i2c_get_trans_size (smpr_i2c)/SMPR_BYTE_2_BIT; /* in bytes */
    ASSERT_TEST(trans_size*SMPR_BYTE_2_BIT /* bits */ <
            SMPR_PROTO_I2C_TRANS_SIZE_FLAGS_MAX+1, "Invalid transfer size for I2C",
            err_inv_size, -1);

    if (trans_size != size) {
        DBE_DEBUG (DBG_SM_PR | DBG_LVL_WARN,
                "[sm_pr:i2c] _i2c_read_generic: Data size differs from Transfer size.\n"
                "\tChoosing the smallest value between trans_size (%u) and size (%zu)\n", trans_size, size);
    }

    /* Choose the smallest one */
    trans_size = (trans_size > size) ? size : trans_size;
    DBE_DEBUG (DBG_SM_PR | DBG_LVL_TRACE,
            "[sm_pr:i2c] _i2c_read_generic: Transmission size = %u bytes\n", trans_size);

    /* Receive data, byte by byte*/
    uint32_t i;
    for (i = 0; i < trans_size; ++i) {
        /* if this is the last byte, then stop transfer */
        uint32_t i2c_data;
        if (i == trans_size - 1) {
            i2c_data = I2C_PROTO_CR_STO | I2C_PROTO_CR_RD | I2C_PROTO_CR_ACK;
        }
        else {
            i2c_data = I2C_PROTO_CR_RD & ~I2C_PROTO_CR_ACK;
        }

        rw_err = SET_PARAM(parent, sm_pr_i2c, i2c_proto->base, I2C_PROTO, CR,
                /* field = NULL */, MULT_BIT_PARAM, i2c_data /* value */,
                /* min */, /* max */, NO_CHK_FUNC, SET_FIELD);
        ASSERT_TEST(rw_err == RW_OK, "Could not set I2C CR register", err_exit, -1);
        DBE_DEBUG (DBG_SM_PR | DBG_LVL_TRACE,
                "[sm_pr:i2c] _i2c_read_generic: CR register #%u = 0x%08X\n", i, i2c_data);

        /* Check transfer */
        err = _i2c_check_transfer (self, false /* read mode */);
        ASSERT_TEST(err == 0, "Could not check I2C transfer", err_exit, -1);

        /* Copy data to user */
        rw_err = GET_PARAM(parent, sm_pr_i2c, i2c_proto->base, I2C_PROTO, RXR,
                /* field = NULL */, MULT_BIT_PARAM, *(data + i) /* value */,
                NO_FMT_FUNC);
        ASSERT_TEST(rw_err == RW_OK, "Could not rget I2C RXR register", err_exit, -1);
        DBE_DEBUG (DBG_SM_PR | DBG_LVL_TRACE,
                "[sm_pr:i2c] _i2c_read_generic: RXR register #%u = 0x%08X\n", i, *(data+i));

        num_bytes++;
    }

    /* Transmission done */
    DBE_DEBUG (DBG_SM_PR | DBG_LVL_TRACE,
            "[sm_pr:i2c] _i2c_read_generic: Transmission done\n");

    err = num_bytes;

err_inv_size:
err_exit:
err_proto_handler:
    return err;
}

#if 0
/* Generic write / read from I2C */
static ssize_t _i2c_write_read_generic (smpr_t *self, uint8_t *data,
        size_t size)
{
    int err = 0;
    smpr_proto_i2c_t *i2c_proto = SMPR_PROTO_I2C(self);

    i2c_proto->mode = I2C_MODE_REP_START;

    /* Get specific parameters */
    smpr_i2c_t *smpr_i2c = (smpr_i2c_t *) smpr_get_ops (self);
    uint32_t trans_size = smpr_i2c_get_trans_size(smpr_i2c)/SMPR_BYTE_2_BIT; /* in bytes */

    ssize_t smpr_err = _i2c_write_generic (self, data, size);
    ASSERT_TEST(smpr_err == smpr_i2c->trans_size/SMPR_BYTE_2_BIT /* in bytes*/,
            "Could not write data to I2C", err_exit, -1);

    smpr_err = _i2c_read_generic (self, data, size);
    ASSERT_TEST(smpr_err == smpr_i2c_get_trans_size(smpr_i2c)/SMPR_BYTE_2_BIT /* in bytes*/,
            "Could not read data to I2C", err_exit, -1);

    i2c_proto->mode = I2C_MODE_NORMAL;

err_exit:
    return err;
}
#endif

static const smpr_proto_ops_t smpr_proto_ops_i2c = {
    .proto_name           = "I2C",              /* Protocol name */
    .proto_open           = i2c_open,           /* Open device */
    .proto_release        = i2c_release,        /* Release device */
    .proto_read_16        = i2c_read_16,        /* Read 16-bit data */
    .proto_read_32        = i2c_read_32,        /* Read 32-bit data */
    .proto_read_64        = i2c_read_64,        /* Read 64-bit data */
    .proto_write_16       = i2c_write_16,       /* Write 16-bit data */
    .proto_write_32       = i2c_write_32,       /* Write 32-bit data */
    .proto_write_64       = i2c_write_64,       /* Write 64-bit data */
    .proto_read_block     = i2c_read_block,     /* Read arbitrary block size data,
                                                    parameter size in bytes */
    .proto_write_block    = i2c_write_block,    /* Write arbitrary block size data,
                                                    parameter size in bytes */
    .proto_read_dma       = NULL,               /* Read arbitrary block size data via DMA,
                                                    parameter size in bytes */
    .proto_write_dma      = NULL                /* Write arbitrary block size data via DMA,
                                                    parameter size in bytes */
};

/************ Our methods implementation **********/

/* Creates a new instance of the proto_i2c */
smpr_i2c_t *smpr_i2c_new (uint32_t rep_start, uint32_t trans_size, uint32_t addr)
{
    smpr_i2c_t *self = (smpr_i2c_t *) zmalloc (sizeof *self);
    ASSERT_ALLOC (self, err_smpr_i2c_alloc);

    /* copy I2C operations */
    self->proto_ops = smpr_proto_ops_i2c;

    self->rep_start = rep_start;
    self->trans_size = trans_size;
    self->addr = addr;

    return self;

err_smpr_i2c_alloc:
    return NULL;
}

/* Destroy an instance of the i2c */
smpr_err_e smpr_i2c_destroy (smpr_i2c_t **self_p)
{
    assert (self_p);

    if (*self_p) {
        smpr_i2c_t *self = *self_p;

        free (self);
        self_p = NULL;
    }

    return SMPR_SUCCESS;
}

smpr_err_e smpr_i2c_set_rep_start (smpr_i2c_t *self, uint32_t rep_start)
{
    assert (self);
    self->rep_start = rep_start;

    return SMPR_SUCCESS;
}

uint32_t smpr_i2c_get_rep_start (smpr_i2c_t *self)
{
    assert (self);
    return self->rep_start;
}

smpr_err_e smpr_i2c_set_trans_size (smpr_i2c_t *self, uint32_t trans_size)
{
    assert (self);
    self->trans_size = trans_size;

    return SMPR_SUCCESS;
}

uint32_t smpr_i2c_get_trans_size (smpr_i2c_t *self)
{
    assert (self);
    return self->trans_size;
}

smpr_err_e smpr_i2c_set_addr (smpr_i2c_t *self, uint32_t addr)
{
    assert (self);
    self->addr = addr;

    return SMPR_SUCCESS;
}

uint32_t smpr_i2c_get_addr (smpr_i2c_t *self)
{
    assert (self);
    return self->addr;
}

const smpr_proto_ops_t *smpr_i2c_get_ops (smpr_i2c_t *self)
{
    assert (self);
    return &self->proto_ops;
}

