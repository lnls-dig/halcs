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
#include "sm_io_exports.h"
#include "dev_io.h"
#include "hal_assert.h"
#include "ddr3_map.h"
#include "board.h"
#include "rw_param.h"
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

#define SMIO_ACQ_HANDLER(self) ((smio_acq_t *) self->smio_handler)

/************************************************************/
/***************** Specific ACQ Operations ******************/
/************************************************************/

static int _acq_data_acquire (void *owner, void *args, void *ret)
{
    (void) ret;
    assert (owner);
    assert (args);

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] "
            "Calling _acq_data_acquire\n");
    SMIO_OWNER_TYPE *self = SMIO_EXP_OWNER(owner);

    /* Message is:
     * frame 0: operation code
     * frame 1: number of samples
     * frame 2: channel                 */
    uint32_t num_samples = *(uint32_t *) EXP_MSG_ZMQ_FIRST_ARG(args);
    uint32_t chan = *(uint32_t *) EXP_MSG_ZMQ_NEXT_ARG(args);

    /* channel required is out of the limit */
    if (chan > SMIO_ACQ_NUM_CHANNELS-1) {
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_WARN, "[sm_io:acq] data_acquire: "
                "Channel required is out of the maximum limit\n");

        return -ACQ_NUM_SAMPLES_OOR;
    }

    /* number of samples required is out of the maximum limit */
    if (num_samples > SMIO_ACQ_HANDLER(self)->acq_buf[chan].max_samples) {
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_WARN, "[sm_io:acq] data_acquire: "
                "Number of samples required is out of the maximum limit\n");
        return -ACQ_NUM_SAMPLES_OOR;
    }

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] data_acquire: "
            "\n\tCurrent acq params for channel #%u: number of samples = %u\n",
            chan, num_samples);

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] data_acquire: "
            "\n\tPrevious acq params for channel #%u: number of samples = %u\n",
            chan, SMIO_ACQ_HANDLER(self)->acq_params[chan].num_samples);

    /* Set the parameters: number of samples of this channel */
    SMIO_ACQ_HANDLER(self)->acq_params[chan].num_samples = num_samples;

    /* Default SHOTS value is 1 */
    uint32_t acq_core_shots = ACQ_CORE_SHOTS_NB_W(1);
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] data_acquire: "
            "Number of shots = %u\n", acq_core_shots);
    smio_thsafe_client_write_32 (self, ACQ_CORE_REG_SHOTS, &acq_core_shots);

    /* FIXME FPGA Firmware requires number of samples to be divisible by
     * acquisition channel sample size */
    uint32_t num_samples_div_pre =
        DDR3_PAYLOAD_SIZE/SMIO_ACQ_HANDLER(self)->acq_buf[chan].sample_size;
    uint32_t num_samples_aligned_pre = num_samples + num_samples_div_pre -
        (num_samples % num_samples_div_pre);
    /* Pre trigger samples */
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] data_acquire: "
            "Number of pre-trigger samples (aligned to sample size) = %u\n",
            num_samples_aligned_pre);
    smio_thsafe_client_write_32 (self, ACQ_CORE_REG_PRE_SAMPLES, &num_samples_aligned_pre);

    /* Post trigger samples */
    uint32_t num_samples_post = 0;
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] data_acquire: "
            "Number of post-trigger samples = %u\n",
            num_samples_post);
    smio_thsafe_client_write_32 (self, ACQ_CORE_REG_POST_SAMPLES, &num_samples_post);

    /* DDR3 start address. Convert Byte address to Word address, as we specify only
     * the start address */
    uint32_t start_addr = (uint32_t)
        SMIO_ACQ_HANDLER(self)->acq_buf[chan].start_addr/DDR3_ADDR_WORD_2_BYTE;
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] data_acquire: "
            "DDR3 start address: 0x%08x\n", start_addr);
    smio_thsafe_client_write_32 (self, ACQ_CORE_REG_DDR3_START_ADDR, &start_addr );

    /* Prepare core_ctl register */
    uint32_t acq_core_ctl_reg = ACQ_CORE_CTL_FSM_ACQ_NOW;
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] data_acquire: "
            "Control register is: 0x%08x\n",
            acq_core_ctl_reg);
    smio_thsafe_client_write_32 (self, ACQ_CORE_REG_CTL, &acq_core_ctl_reg );

    /* Prepare acquisition channel control */
    uint32_t acq_chan_ctl = ACQ_CORE_ACQ_CHAN_CTL_WHICH_W(chan);
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] data_acquire: "
            "Channel control register is: 0x%08x\n",
            acq_chan_ctl);
    smio_thsafe_client_write_32 (self, ACQ_CORE_REG_ACQ_CHAN_CTL, &acq_chan_ctl );

    /* Starting acquisition... */
    acq_core_ctl_reg |= ACQ_CORE_CTL_FSM_START_ACQ;
    smio_thsafe_client_write_32 (self, ACQ_CORE_REG_CTL, &acq_core_ctl_reg );

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] data_acquire: "
            "Acquisition Started!\n");

    return -ACQ_OK;
}

disp_op_t acq_data_acquire_exp = {
    .name = ACQ_NAME_DATA_ACQUIRE,
    .opcode = ACQ_OPCODE_DATA_ACQUIRE,
    .func_fp = _acq_data_acquire,
    .retval = DISP_ARG_END,
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

static int _acq_check_data_acquire (void *owner, void *args, void *ret)
{
    (void) ret;
    assert (owner);
    assert (args);

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] "
            "Calling _acq_check_data_acquire\n");

    SMIO_OWNER_TYPE *self = SMIO_EXP_OWNER(owner);

    uint32_t status_done = 0;
    /* Check for completion */
    smio_thsafe_client_read_32 (self, ACQ_CORE_REG_STA, &status_done );
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] data_acquire: "
            "Status done = 0x%08x\n", status_done);

    if (!(status_done & ACQ_CORE_STA_DDR3_TRANS_DONE)) {
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] acq_check_data_acquire: "
                "Acquisition is not done\n");
        return -ACQ_NOT_COMPLETED;
    }

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] acq_check_data_acquire: "
            "Acquisition is done\n");
    return -ACQ_OK;
}

disp_op_t acq_check_data_acquire_exp = {
    .name = ACQ_NAME_CHECK_DATA_ACQUIRE,
    .opcode = ACQ_OPCODE_CHECK_DATA_ACQUIRE,
    .func_fp = _acq_check_data_acquire,
    .retval = DISP_ARG_END,
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_END
    }
};

static int _acq_get_data_block (void *owner, void *args, void *ret)
{
    assert (owner);
    assert (args);

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] "
            "Calling _acq_get_data_block\n");

    SMIO_OWNER_TYPE *self = SMIO_EXP_OWNER(owner);

    /* Message is:
     * frame 0: channel
     * frame 1: block required      */
    uint32_t chan = *(uint32_t *) EXP_MSG_ZMQ_FIRST_ARG(args);
    uint32_t block_n = *(uint32_t *) EXP_MSG_ZMQ_NEXT_ARG(args);
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] get_data_block: "
            "chan = %u, block_n = %u\n",chan, block_n);

    /* Channel features */
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] get_data_block: "
            "\t[channel = %u], id = %u, start addr = 0x%08x\n"
            "\tend addr = 0x%08x, max samples = %u, sample size = %u\n",
            chan,
            SMIO_ACQ_HANDLER(self)->acq_buf[chan].id,
            SMIO_ACQ_HANDLER(self)->acq_buf[chan].start_addr,
            SMIO_ACQ_HANDLER(self)->acq_buf[chan].end_addr,
            SMIO_ACQ_HANDLER(self)->acq_buf[chan].max_samples,
            SMIO_ACQ_HANDLER(self)->acq_buf[chan].sample_size);

    uint32_t block_n_max = ( SMIO_ACQ_HANDLER(self)->acq_buf[chan].end_addr -
            SMIO_ACQ_HANDLER(self)->acq_buf[chan].start_addr +
            SMIO_ACQ_HANDLER(self)->acq_buf[chan].sample_size) / BLOCK_SIZE;
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] get_data_block: "
            "block_n_max = %u\n", block_n_max);

    if (block_n > block_n_max) {    /* block required out of the limits */
        /* TODO error level in this case */
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_ERR, "[sm_io:acq] get_data_block: "
                "Block required is out of the limit\n");
        return -ACQ_BLOCK_OOR;
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
            "block_n_valid= %u, over_samples= %u\n",
            block_n_valid, over_samples);

    /* check if block required is valid and if it is full or not */
    if (block_n > block_n_valid) {
        /* TODO error level in this case */
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_ERR, "[sm_io:acq] get_data_block: "
                "Block required is not valid\n");
        return -ACQ_BLOCK_OOR;
    }   /* Last valid data conditions check done */

    uint32_t reply_size;
    if (block_n == block_n_valid && over_samples > 0){
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO, "[sm_io:acq] get_data_block: "
                "Block required has %u valid samples\n", over_samples);
        reply_size = over_samples*SMIO_ACQ_HANDLER(self)->acq_buf[chan].sample_size;
    }
    else { /* if block_n < block_n_valid */
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO, "[sm_io:acq] get_data_block: "
                "Block required is full of valid data\n");
        reply_size = BLOCK_SIZE;
    }

    uint32_t addr_i = SMIO_ACQ_HANDLER(self)->acq_buf[chan].start_addr +
        block_n * BLOCK_SIZE;
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] get_data_block: "
            "Memory start address = 0x%08x\n", addr_i);

    smio_acq_data_block_t *data_block = (smio_acq_data_block_t *) ret;

    /* Here we must use the "raw" version, as we can't have
     * LARGE_MEM_ADDR mangled with the bas address of this SMIO */
    data_block->valid_bytes = smio_thsafe_raw_client_read_block (self, LARGE_MEM_ADDR | addr_i,
            reply_size, (uint32_t *) data_block->data);
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] get_data_block: "
            "%u bytes read\n", data_block->valid_bytes);

    return sizeof (*data_block);
}

disp_op_t acq_get_data_block_exp = {
    .name = ACQ_NAME_GET_DATA_BLOCK,
    .opcode = ACQ_OPCODE_GET_DATA_BLOCK,
    .func_fp = _acq_get_data_block,
    .retval = DISP_ARG_ENCODE(DISP_ATYPE_STRUCT, smio_acq_data_block_t),
    .retval_owner = DISP_OWNER_OTHER,
    .args = {
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_ENCODE(DISP_ATYPE_UINT32, uint32_t),
        DISP_ARG_END
    }
};

const disp_op_t *acq_exp_ops [] = {
    &acq_data_acquire_exp,
    &acq_check_data_acquire_exp,
    &acq_get_data_block_exp,
    &disp_op_end
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
        const disp_op_t** smio_exp_ops)
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

    smio_err_e err = SMIO_SUCCESS;

    self->id = ACQ_SDB_DEVID;
    self->name = strdup (ACQ_SDB_NAME);
    ASSERT_ALLOC(self->name, err_name_alloc, SMIO_ERR_ALLOC);

    /* Set SMIO ops pointers */
    self->ops = &acq_ops;
    self->thsafe_client_ops = &smio_thsafe_client_zmq_ops;
    self->exp_ops = acq_exp_ops;

    /* Initialize specific structure */
    self->smio_handler = smio_acq_new (0); /* Default: num_samples = 0 */
    ASSERT_ALLOC(self->smio_handler, err_smio_handler_alloc, SMIO_ERR_ALLOC);

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
