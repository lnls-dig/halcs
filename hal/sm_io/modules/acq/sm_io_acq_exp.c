/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include <stdlib.h>

#include "sm_io_acq_exp.h"
#include "sm_io_acq_codes.h"
#include "sm_io.h"
#include "dev_io.h"
#include "hal_assert.h"
#include "ddr3_map.h"
#include "board.h"
#include "wb_acq_core_regs.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, SM_IO, "[sm_io:acq_exp]", \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_ALLOC(ptr, SM_IO, "[sm_io:acq_exp]", \
            smio_err_str(SMIO_ERR_ALLOC),           \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                    \
    CHECK_HAL_ERR(err, SM_IO, "[sm_io:acq_exp]",    \
            smio_err_str (err_type))

/************************************************************/
/******************  Specific Operations ********************/
/************************************************************/
/* This does not belong to any public interface. It's meant to
 * allow the do_ops() function to route the operations based on
 * the opcode  */
/* FIXME FIXME! We don't have to know our transport layer! */

#define BAR4_ADDR (4 << 28)
#define BAR2_ADDR (2 << 28)

#define SMIO_ACQ_HANDLER(self) ((smio_acq_t *) self->smio_handler)

/************************************************************/
/******************** Local functions ***********************/
/************************************************************/
static void _send_client_response ( ACQ_REPLY_TYPE reply_code,
        uint32_t reply_size, uint32_t *data_out,
        bool with_data_frame, mdp_worker_t *worker,
        zframe_t *reply_to ){

    /* Send reply back to client */
    zmsg_t *report = zmsg_new ();
    ASSERT_ALLOC(report, err_send_msg_alloc);

    /* Message is:
     * frame 0: error code          -> always sent
     * frame 1: size (in bytes)     -> sent only get_data_block
     * frame 2: data                -> sent only get_data_block          */
    int zerr = zmsg_addmem (report, &reply_code, sizeof(reply_code));
    ASSERT_TEST(zerr==0, "Could not add reply code in message", err_reply_code);

    if (with_data_frame) {
        zerr = zmsg_addmem (report, &reply_size, sizeof(reply_size));
        ASSERT_TEST(zerr==0, "Could not add reply size in message",
                err_size_code);
        zerr = zmsg_addmem (report, data_out, reply_size);
        ASSERT_TEST(zerr==0, "Could not add reply data in message",
                err_data_code);
    }

    DBE_DEBUG (DBG_MSG | DBG_LVL_TRACE, "[sm_io:acq] send_client_response: "
            "Sending message:\n");
#ifdef LOCAL_MSG_DBG
    zmsg_print (report);
#endif
    mdp_worker_send (worker, &report, reply_to);
    return;

err_data_code:
err_size_code:
err_reply_code:
err_send_msg_alloc:
    zmsg_destroy (&report);
}

/************************************************************/
/************* Specific acq functions **************/
/************************************************************/
static void *_acq_data_acquire (void *owner, void *args)
{
    assert (owner);
    assert (args);

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] "
            "Calling _acq_data_acquire\n");
    smio_t *self = (smio_t *) owner;
    exp_msg_zmq_t *exp_msg = (exp_msg_zmq_t *) args;
    assert (zmsg_size (*exp_msg->msg) == 2);

    /* Message is:
     * frame 0: operation code
     * frame 1: number of samples
     * frame 2: channel                 */
    uint32_t num_samples = *(uint32_t *) zframe_data (zmsg_pop (*exp_msg->msg));
    uint32_t chan = *(uint32_t *) zframe_data (zmsg_pop (*exp_msg->msg));

    /* number of samples required is out of the maximum limit */
    if (num_samples >= SMIO_ACQ_HANDLER(self)->acq_buf[chan].max_samples) {
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_WARN, "[sm_io:acq] data_acquire: "
                "Number of samples required is out of the maximum limit\n");

        /* Message is:
         * frame 0: error code      */
        _send_client_response (ACQ_NUM_SAMPLES_OOR, 0, NULL, false,
                self->worker, exp_msg->reply_to);
        goto err_smp_exceeded;
    }

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] data_acquire: "
            "num_samples= %u\tchannel= %u\n", num_samples, chan);

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] data_acquire: "
            "acq params: num_samples[ch=%u] = %u\n", chan,
            SMIO_ACQ_HANDLER(self)->acq_params[chan].num_samples);

    /* Set the parameters: number of samples of this channel */
    SMIO_ACQ_HANDLER(self)->acq_params[chan].num_samples = num_samples;

    /* Default SHOTS value is 1 */
    uint32_t acq_core_shots = ACQ_CORE_SHOTS_NB_W(1);
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] data_acquire: "
            "acq_core_shots = %u = 0x%08x\n", acq_core_shots, acq_core_shots);
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] data_acquire: "
            "ACQ_CORE_REG_PRE_SAMPLES = 0x%08x\n", ACQ_CORE_REG_PRE_SAMPLES);
    smio_thsafe_client_write_32 (self, BAR4_ADDR | WB_ACQ_BASE_ADDR |
            ACQ_CORE_REG_SHOTS, &acq_core_shots);

    /* FIXME FPGA Firmware requires number of samples to be divisible by
     * acquisition channel sample size */
    uint32_t num_samples_div =
        DDR3_PAYLOAD_SIZE/SMIO_ACQ_HANDLER(self)->acq_buf[chan].sample_size;
    uint32_t num_samples_aligned = num_samples + num_samples_div -
        (num_samples % num_samples_div);
    /* Pre trigger samples */
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] data_acquire: "
            "Pre Trigger offset: ACQ_CORE_REG_PRE_SAMPLES = 0x%08x\n",
            ACQ_CORE_REG_PRE_SAMPLES);
    smio_thsafe_client_write_32 (self, BAR4_ADDR | WB_ACQ_BASE_ADDR |
            ACQ_CORE_REG_PRE_SAMPLES, &num_samples_aligned);

    /* Post trigger samples */
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] data_acquire: "
            "Post Trigger offset: ACQ_CORE_REG_POST_SAMPLES = 0x%08x\n",
            ACQ_CORE_REG_POST_SAMPLES);
    smio_thsafe_client_write_32 (self, BAR4_ADDR | WB_ACQ_BASE_ADDR |
            ACQ_CORE_REG_POST_SAMPLES, 0);

    /* DDR3 start address */
    uint32_t start_addr= (uint32_t) SMIO_ACQ_HANDLER(self)->acq_buf[chan].start_addr;
    uint32_t start_addr8= start_addr/8;
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] data_acquire: "
            "DDR3 Start address: ACQ_CORE_REG_DDR3_START_ADDR = 0x%08x\n",
            ACQ_CORE_REG_DDR3_START_ADDR);
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] data_acquire: "
            "DDR3 Start value: 0x%08x\n", start_addr);
    smio_thsafe_client_write_32 (self, BAR4_ADDR | WB_ACQ_BASE_ADDR |
            ACQ_CORE_REG_DDR3_START_ADDR, &start_addr8 );

    /* Prepare core_ctl register */
    uint32_t acq_core_ctl_reg = ACQ_CORE_CTL_FSM_ACQ_NOW;
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] data_acquire: "
            "Skip Trigger address: ACQ_CORE_REG_CTL = 0x%08x\n",
            ACQ_CORE_REG_CTL);
    smio_thsafe_client_write_32 (self, BAR4_ADDR | WB_ACQ_BASE_ADDR |
            ACQ_CORE_REG_CTL, &acq_core_ctl_reg );

    /* Prepare acquisition channel control */
    uint32_t acq_chan_ctl = ACQ_CORE_ACQ_CHAN_CTL_WHICH_W(chan);
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] data_acquire: "
            "Channel address: ACQ_CORE_REG_ACQ_CHAN_CTL = 0x%08x\n",
            ACQ_CORE_REG_ACQ_CHAN_CTL);
    smio_thsafe_client_write_32 (self, BAR4_ADDR | WB_ACQ_BASE_ADDR |
            ACQ_CORE_REG_ACQ_CHAN_CTL, &acq_chan_ctl );

    /* Starting acquisition... */
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] data_acquire: "
            "Start Acquision address: ACQ_CORE_REG_CTL = 0x%08x\n",
            ACQ_CORE_REG_CTL);
    acq_core_ctl_reg |= ACQ_CORE_CTL_FSM_START_ACQ;
    smio_thsafe_client_write_32 (self, BAR4_ADDR | WB_ACQ_BASE_ADDR |
            ACQ_CORE_REG_CTL, &acq_core_ctl_reg );

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] data_acquire: "
            "Acquisition Started!\n");

    /* Message is:
     * frame 0: error code      */
    _send_client_response (ACQ_OK, 0, NULL, false, self->worker,
            exp_msg->reply_to);

err_smp_exceeded:
    zmsg_destroy (exp_msg->msg);
    return NULL;
}

static void *_acq_check_data_acquire (void *owner, void *args)
{
    assert (owner);
    assert (args);

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] "
            "Calling _acq_check_data_acquire\n");

    smio_t *self = (smio_t *) owner;
    exp_msg_zmq_t *exp_msg = (exp_msg_zmq_t *) args;

    uint32_t status_done = 0;
    /* Check for completion */
    smio_thsafe_client_read_32 (self, BAR4_ADDR | WB_ACQ_BASE_ADDR |
            ACQ_CORE_REG_STA, &status_done );
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] data_acquire: "
            "Status done = 0x%08x\n", status_done);

    if (!(status_done & ACQ_CORE_STA_DDR3_TRANS_DONE)) {
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] acq_check_data_acquire: "
                "Acquisition is not done\n");
        /* Message is:
         * frame 0: error code  */
        _send_client_response (ACQ_NOT_COMPLETED, 0, NULL, false, self->worker,
                exp_msg->reply_to);
        goto err_acq_check;
    }

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] acq_check_data_acquire: "
            "Acquisition is done\n");
    _send_client_response (ACQ_OK, 0, NULL, false, self->worker,
            exp_msg->reply_to);
err_acq_check:
    zmsg_destroy (exp_msg->msg);
    return NULL;
}

static void *_acq_get_data_block (void *owner, void *args)
{
    assert (owner);
    assert (args);

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] "
            "Calling _acq_get_data_block\n");

    smio_t *self = (smio_t *) owner;
    exp_msg_zmq_t *exp_msg = (exp_msg_zmq_t *) args;
    assert (zmsg_size (*exp_msg->msg) == 2);

    /* Message is:
     * frame 0: channel
     * frame 1: block required      */
    uint32_t chan = *(uint32_t *) zframe_data (zmsg_pop (*exp_msg->msg));
    uint32_t block_n     = *(uint32_t *) zframe_data (zmsg_pop (*exp_msg->msg));
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] get_data_block: "
            "chan = %u\t block_n = %u\n",chan, block_n);

    /* Channel features */
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] get_data_block: "
            "[channel = %u]\tid = %u\tstart_addr = %u\tend_addr = %u\t"
            "max_samples = %u\tsample_size = %u\n", chan,
            SMIO_ACQ_HANDLER(self)->acq_buf[chan].id,
            SMIO_ACQ_HANDLER(self)->acq_buf[chan].start_addr,
            SMIO_ACQ_HANDLER(self)->acq_buf[chan].end_addr,
            SMIO_ACQ_HANDLER(self)->acq_buf[chan].max_samples,
            SMIO_ACQ_HANDLER(self)->acq_buf[chan].sample_size);

    uint32_t block_n_max = ( SMIO_ACQ_HANDLER(self)->acq_buf[chan].end_addr -
            SMIO_ACQ_HANDLER(self)->acq_buf[chan].start_addr +
            SMIO_ACQ_HANDLER(self)->acq_buf[chan].sample_size) /
        BLOCK_SIZE;
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] get_data_block: "
            "block_n_max= %u\n", block_n_max);
    if (block_n > block_n_max) {	/* block required out of the limits */
        /* TODO error level in this case */
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_ERR, "[sm_io:acq] get_data_block: "
                "Block required out of the limit\n");
        /* Message is:
         * frame 0: error code
         * frame 1: size (in bytes)
         * frame 2: data            */
        _send_client_response (ACQ_BLOCK_OOR, 0, NULL, true, self->worker,
                exp_msg->reply_to);
        goto err_invalid_block;
    }

    /* Get number of samples */
    uint32_t num_samples =
        SMIO_ACQ_HANDLER(self)->acq_params[chan].num_samples;
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] get_data_block: "
            "last num_samples = %u\n", num_samples);

    uint32_t n_max_samples = BLOCK_SIZE/SMIO_ACQ_HANDLER(self)->acq_buf[chan].sample_size;
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] get_data_block: "
            "n_max_samples = %u\n", n_max_samples);

    uint32_t over_samples = num_samples % n_max_samples;
    uint32_t block_n_valid = num_samples / n_max_samples;
    /* When the last block is full 'block_n_valid' exceeds by one */
    if (block_n_valid != 0 && over_samples == 0) {
        block_n_valid--;
    }
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] get_data_block: "
            "block_n_valid= %u\tover_samples= %u\n",
            block_n_valid, over_samples);

    /* check if block required is valid and if it is full or not */
    if (block_n > block_n_valid) {
        /* TODO error level in this case */
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_ERR, "[sm_io:acq] get_data_block: "
                "Block required is not valid\n");
        /* Message is:
         * frame 0: error code
         * frame 1: size (in bytes)
         * frame 2: data            */
        _send_client_response (ACQ_BLOCK_OOR, 0, NULL, true, self->worker,
                exp_msg->reply_to);
        goto err_invalid_block;
    }	/* Last valid data conditions check done */

    uint32_t reply_size;
    if (block_n == block_n_valid && over_samples > 0){
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_ERR, "[sm_io:acq] get_data_block: "
                "Block required has %u valid samples\n", over_samples);
        reply_size = over_samples*SMIO_ACQ_HANDLER(self)->acq_buf[chan].sample_size;
    }
    else { /* if block_n < block_n_valid */
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_ERR, "[sm_io:acq] get_data_block: "
                "Block required is full of valid data\n");
        reply_size = BLOCK_SIZE;
    }

    uint32_t addr_i = SMIO_ACQ_HANDLER(self)->acq_buf[chan].start_addr +
        block_n * BLOCK_SIZE;
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] get_data_block: "
            "addr_i = %u\n", addr_i);

    /* static max allocation (32-bit words) */
    uint32_t data_out[BLOCK_SIZE/sizeof(uint32_t)];
    ssize_t bytes_read = smio_thsafe_client_read_block (self, BAR2_ADDR | addr_i,
            reply_size, data_out);
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] get_data_block: "
            "%lu bytes read\n", bytes_read);

    /* Successul case */
    /* Message is:
     * frame 0: error code
     * frame 1: size (in bytes)
     * frame 2: data            */
    _send_client_response (ACQ_OK, bytes_read, data_out, true, self->worker,
            exp_msg->reply_to);

err_invalid_block:
    zmsg_destroy (exp_msg->msg);
    return NULL;
}

const smio_exp_ops_t acq_exp_ops [] = {
    {.name 			= ACQ_NAME_DATA_ACQUIRE,
        .opcode 		= ACQ_OPCODE_DATA_ACQUIRE,
        .func_fp 		= _acq_data_acquire						},

    {.name 			= ACQ_NAME_GET_DATA_BLOCK,
        .opcode 		= ACQ_OPCODE_GET_DATA_BLOCK,
        .func_fp 		= _acq_get_data_block					},

    {.name 			= ACQ_NAME_CHECK_DATA_ACQUIRE,
        .opcode 		= ACQ_OPCODE_CHECK_DATA_ACQUIRE,
        .func_fp 		= _acq_check_data_acquire				},

    {.name 			= NULL,		/* Must end with this NULL pattern */
        .opcode 		= 0,
        .func_fp 		= NULL									}
};
/************************************************************/
/***************** Export methods functions *****************/
/************************************************************/

static smio_err_e _acq_do_op (void *owner, void *msg);

/* Attach an instance of sm_io to dev_io function pointer */
smio_err_e acq_attach (smio_t *self, devio_t *parent)
{
    (void) self;
    (void) parent;
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Deattach an instance of sm_io to dev_io function pointer */
smio_err_e acq_deattach (smio_t *self)
{
    (void) self;
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Export (register) sm_io to handle operations function pointer */
smio_err_e acq_export_ops (smio_t *self,
        const smio_exp_ops_t* smio_exp_ops)
{
    (void) self;
    (void) smio_exp_ops;
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Unexport (unregister) sm_io to handle operations function pointer */
smio_err_e acq_unexport_ops (smio_t *self)
{
    (void) self;
    return SMIO_ERR_FUNC_NOT_IMPL;
}


/* Generic wrapper for receiving opcodes and arguments to specific funtions function pointer */
/* FIXME: Code repetition! _devio_do_smio_op () function does almost the same!!! */
smio_err_e _acq_do_op (void *owner, void *msg)
{
    (void) owner;
    (void) msg;
    return SMIO_ERR_FUNC_NOT_IMPL;
}

smio_err_e acq_do_op (void *self, void *msg)
{
    return _acq_do_op (self, msg);
}

const smio_ops_t acq_ops = {
    .attach             = acq_attach,          /* Attach sm_io instance to dev_io */
    .deattach           = acq_deattach,        /* Deattach sm_io instance to dev_io */
    .export_ops         = acq_export_ops,      /* Export sm_io operations to dev_io */
    .unexport_ops       = acq_unexport_ops,    /* Unexport sm_io operations to dev_io */
    .do_op              = acq_do_op            /* Generic wrapper for handling specific operations */
};

/************************************************************/
/****************** Bootstrap Operations ********************/
/************************************************************/

smio_err_e acq_init (smio_t * self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq_exp] Initializing acq\n");

    smio_err_e err = SMIO_ERR_ALLOC;

    self->id = ACQ_SDB_DEVID;
    self->name = strdup (ACQ_SDB_NAME);
    ASSERT_ALLOC(self->name, err_name_alloc);

    /* Set SMIO ops pointers */
    self->ops = &acq_ops;
    self->thsafe_client_ops = &smio_thsafe_client_zmq_ops;
    self->exp_ops = acq_exp_ops;

    /* Initialize specific structure */
    self->smio_handler = smio_acq_new (0); /* Default: num_samples = 0 */
    ASSERT_ALLOC(self->smio_handler, err_smio_handler_alloc);

    err = SMIO_SUCCESS;
    return err;

err_smio_handler_alloc:
    free (self->name);
err_name_alloc:
    return err;
}

/* Destroy sm_io instance of acq */
smio_err_e acq_shutdown (smio_t *self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq_exp] Shutting down acq\n");

    smio_acq_destroy ((smio_acq_t **)&self->smio_handler);
    self->exp_ops = NULL;
    self->thsafe_client_ops = NULL;
    self->ops = NULL;
    free (self->name);

    return SMIO_SUCCESS;
}

const smio_bootstrap_ops_t acq_bootstrap_ops = {
    .init = acq_init,
    .shutdown = acq_shutdown
};
