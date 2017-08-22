/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "halcs_server.h"
/* Private headers */
#include "ddr3_map.h"
#include "sm_io_acq_codes.h"
#include "sm_io_acq_exports.h"
#include "sm_io_acq_core.h"
#include "sm_io_acq_exp.h"
#include "hw/wb_acq_core_regs.h"

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

/* FIXME: Usage of reserved, undocumented bits */
/* Acquisition can start if FSM is in IDLE state, if Flow Control FIFO
 * is not FULL, dbg_source_pl_stall (RESERVED BIT, UNDOCUMENTED) is not asserted
 * and there is no data in the FIFOs (ACQ_CORE_STA_RESERVED3_MASK)*/
#define ACQ_CORE_IDLE_MASK      (ACQ_CORE_STA_FSM_STATE_MASK | ACQ_CORE_STA_FC_FULL | \
                                    ACQ_CORE_STA_RESERVED2_W(1 << 3) | ACQ_CORE_STA_RESERVED3_MASK)

#define ACQ_CORE_IDLE_VALUE     ACQ_CORE_STA_FSM_STATE_W(0x1)

/* Acquisition is completed when FSM is in IDLE state, FSM is with DONE
 * flag asserted, Flow Control FIFO is with DOME flag asserted and DDR3 has
 * its DONE flag asserted */
#define ACQ_CORE_COMPLETE_MASK  (ACQ_CORE_STA_FSM_STATE_MASK | ACQ_CORE_STA_FSM_ACQ_DONE | \
                                    ACQ_CORE_STA_FC_TRANS_DONE | ACQ_CORE_STA_DDR3_TRANS_DONE)

#define ACQ_CORE_COMPLETE_VALUE (ACQ_CORE_STA_FSM_STATE_W(0x1) | ACQ_CORE_STA_FSM_ACQ_DONE | \
                                    ACQ_CORE_STA_FC_TRANS_DONE | ACQ_CORE_STA_DDR3_TRANS_DONE)

static int _acq_check_status (SMIO_OWNER_TYPE *self, uint32_t status_mask,
        uint32_t status_value);
static int _acq_set_trigger_type (SMIO_OWNER_TYPE *self, uint32_t trigger_type);
static int _acq_get_trigger_type (SMIO_OWNER_TYPE *self, uint32_t *trigger_type);
static uint64_t _acq_get_start_address (uint64_t acq_core_trig_addr,
        uint64_t acq_size_bytes, uint64_t start_mem_space_addr,
        uint64_t end_mem_space_addr);
static uint64_t _acq_get_read_block_addr_start (uint64_t start_addr, uint64_t offset,
        uint64_t channel_start_addr, uint64_t end_mem_space_addr);

/************************************************************/
/***************** Specific ACQ Operations ******************/
/************************************************************/

static int _acq_data_acquire (void *owner, void *args, void *ret)
{
    UNUSED(ret);
    assert (owner);
    assert (args);
    int err = -ACQ_OK;

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] "
            "Calling _acq_data_acquire\n");
    SMIO_OWNER_TYPE *self = SMIO_EXP_OWNER(owner);
    smio_acq_t *acq = smio_get_handler (self);
    ASSERT_TEST(acq != NULL, "Could not get SMIO ACQ handler",
            err_get_acq_handler, -ACQ_ERR);

    /* First step is to check if the FPGA is already doing an acquisition. If it
     * is, then return an error. Otherwise proceed normally. */
    err = _acq_check_status (self, ACQ_CORE_IDLE_MASK, ACQ_CORE_IDLE_VALUE);
    ASSERT_TEST(err == -ACQ_OK, "Previous acquisition in progress. "
            "New acquisition not started", err_acq_not_completed);

    /* Message is:
     * frame 0: operation code
     * frame 1: number of pre-trigger samples
     * frame 2: number of post-trigger samples
     * frame 3: number of shots
     * frame 4: channel                 */
    uint32_t num_samples_pre = *(uint32_t *) EXP_MSG_ZMQ_FIRST_ARG(args);
    uint32_t num_samples_post = *(uint32_t *) EXP_MSG_ZMQ_NEXT_ARG(args);
    uint32_t num_shots = *(uint32_t *) EXP_MSG_ZMQ_NEXT_ARG(args);
    uint32_t chan = *(uint32_t *) EXP_MSG_ZMQ_NEXT_ARG(args);

    /* channel required is out of the limit */
    if (chan > acq->num_chan-1) {
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_WARN, "[sm_io:acq] data_acquire: "
                "Channel required is out of the maximum limit\n");
        return -ACQ_NUM_CHAN_OOR;
    }

    /* channel required is out of the limit */
    if (num_shots < ACQ_CORE_MIN_NUM_SHOTS) {
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_WARN, "[sm_io:acq] data_acquire: "
                "Number of shots required is below the minimum limit\n");
        return -ACQ_NUM_CHAN_OOR;
    }

    /* number of samples required is out of the maximum limit. Maixmum number of samples
     * in multishot mode is simply the maximum number of samples of the DPRAM. The DPRAM
     * size is calculated to fit the largest sample in the design, so we are safe. */
    uint32_t max_samples_multishot = ACQ_CORE_MULTISHOT_MEM_SIZE;
    if (((num_shots == ACQ_CORE_MIN_NUM_SHOTS) &&
            (num_samples_pre + num_samples_post > acq->acq_buf[chan].max_samples)) ||
            ((num_shots > ACQ_CORE_MIN_NUM_SHOTS) &&
            (num_samples_pre + num_samples_post > max_samples_multishot))) {
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_WARN, "[sm_io:acq] data_acquire: "
                "Number of samples required (%u) is out of the maximum limit (%u)\n"
                 "for shots = %u\n",
                 num_samples_pre + num_samples_post, (num_shots == ACQ_CORE_MIN_NUM_SHOTS) ?
                 acq->acq_buf[chan].max_samples : max_samples_multishot, num_shots);
        return -ACQ_NUM_SAMPLES_OOR;
    }

    /* If skip trigger is set, we must set post_trigger_samples to 0 */
    uint32_t trigger_type = 0;
    err = _acq_get_trigger_type (self, &trigger_type);
    ASSERT_TEST(err == -ACQ_OK, "Could not check for trigger type",
            err_acq_get_trig);
    if (trigger_type == TYPE_ACQ_CORE_SKIP && num_samples_post > 0) {
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_WARN, "[sm_io:acq] data_acquire: "
                "Incompatible trigger type. Post trigger samples is greater than 0\n");
        return -ACQ_TRIG_TYPE;
    }

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] data_acquire:\n"
            "\tCurrent acq params for channel #%u: number of pre-trigger samples = %u\n"
            "\tnumber of post-trigger samples = %u, number of shots = %u\n",
            chan, num_samples_pre, num_samples_post, num_shots);

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] data_acquire:\n"
            "\tPrevious acq params for channel #%u: number of pre-trigger samples = %u\n"
            "\tnumber of post-trigger samples = %u, number of shots = %u\n",
            chan, acq->acq_params[chan].num_samples_pre,
            acq->acq_params[chan].num_samples_post,
            acq->acq_params[chan].num_shots);

    /* Setting the number of shots */
    uint32_t acq_core_shots = ACQ_CORE_SHOTS_NB_W(num_shots);
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] data_acquire: "
            "Number of shots = %u\n", acq_core_shots);
    smio_thsafe_client_write_32 (self, ACQ_CORE_REG_SHOTS, &acq_core_shots);

    /* FIXME FPGA Firmware requires number of samples to be divisible by
     * acquisition channel sample size */
    uint32_t channel_sample_size = acq->acq_buf[chan].sample_size;
    uint32_t samples_alignment =
        DDR3_PAYLOAD_SIZE/channel_sample_size;
    uint32_t num_samples_pre_aligned = hutils_align_value (num_samples_pre,
            samples_alignment);
    /* FIXME. Curently, the FPGA gateware does not support triggered acquisitions with
     * post_samples = 0. There must be at more than DDR3_PAYLOAD_SIZE bytes in post_samples.
     * See github lnls-bpm/bpm-gw#62 */
    uint32_t num_samples_post_aligned = (num_samples_post == 0 && trigger_type != TYPE_ACQ_CORE_SKIP) ?
            samples_alignment*channel_sample_size + 1 : hutils_align_value (num_samples_post, samples_alignment);

    /* Set the parameters: number of samples of this channel */
    acq->acq_params[chan].num_samples_pre = num_samples_pre_aligned;
    acq->acq_params[chan].num_samples_post = num_samples_post_aligned;
    acq->acq_params[chan].num_shots = num_shots;

    /* Pre trigger samples */
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] data_acquire: "
            "Number of pre-trigger samples (aligned to sample size) = %u\n",
            num_samples_pre_aligned);
    smio_thsafe_client_write_32 (self, ACQ_CORE_REG_PRE_SAMPLES, &num_samples_pre_aligned);

    /* Post trigger samples */
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] data_acquire: "
            "Number of post-trigger samples = %u\n",
            num_samples_post_aligned);
    smio_thsafe_client_write_32 (self, ACQ_CORE_REG_POST_SAMPLES, &num_samples_post_aligned);

    /* DDR3 start address. Byte addressed */
    uint32_t start_addr = (uint32_t) acq->acq_buf[chan].start_addr;
    uint32_t end_addr = (uint32_t) acq->acq_buf[chan].end_addr;

    /* Start address */
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] data_acquire: "
            "DDR3 start address: 0x%08x\n", start_addr);
    smio_thsafe_client_write_32 (self, ACQ_CORE_REG_DDR3_START_ADDR, &start_addr);

    /* End address */
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] data_acquire: "
            "DDR3 end address: 0x%08x\n", end_addr);
    smio_thsafe_client_write_32 (self, ACQ_CORE_REG_DDR3_END_ADDR, &end_addr);

    /* Prepare acquisition channel control */
    uint32_t acq_chan_ctl = 0;
    smio_thsafe_client_read_32 (self, ACQ_CORE_REG_ACQ_CHAN_CTL, &acq_chan_ctl);
    acq_chan_ctl = (acq_chan_ctl & ~ACQ_CORE_ACQ_CHAN_CTL_WHICH_MASK) |
         ACQ_CORE_ACQ_CHAN_CTL_WHICH_W(chan);
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] data_acquire: "
            "Channel control register is: 0x%08x\n",
            acq_chan_ctl);
    smio_thsafe_client_write_32 (self, ACQ_CORE_REG_ACQ_CHAN_CTL, &acq_chan_ctl);

    /* Starting acquisition... */
    uint32_t acq_core_ctl_reg = 0;
    smio_thsafe_client_read_32 (self, ACQ_CORE_REG_CTL, &acq_core_ctl_reg);
    acq_core_ctl_reg |= ACQ_CORE_CTL_FSM_START_ACQ;
    smio_thsafe_client_write_32 (self, ACQ_CORE_REG_CTL, &acq_core_ctl_reg);

    /* If we are here, the FPGA is acquiring samples from the
     * specified channel. Set current channel field */
    acq->curr_chan = chan;

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] data_acquire: "
            "Acquisition Started!\n");

    return -ACQ_OK;

err_acq_get_trig:
err_acq_not_completed:
err_get_acq_handler:
    return err;
}

static int _acq_check_data_acquire (void *owner, void *args, void *ret)
{
    UNUSED(ret);
    assert (owner);
    assert (args);
    int err = -ACQ_OK;

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] "
            "Calling _acq_check_data_acquire\n");

    /* Message is:
     * frame 0: operation code
     */
    SMIO_OWNER_TYPE *self = SMIO_EXP_OWNER(owner);
    smio_acq_t *acq = smio_get_handler (self);
    ASSERT_TEST(acq != NULL, "Could not get SMIO ACQ handler",
            err_get_acq_handler, -ACQ_ERR);

    uint32_t chan = acq->curr_chan;

    err = _acq_check_status (self, ACQ_CORE_COMPLETE_MASK, ACQ_CORE_COMPLETE_VALUE);
    if (err != -ACQ_OK) {
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] acq_check_data_acquire: "
                "Acquisition is not done for channel %u\n", chan);
    }
    else {
        uint32_t acq_core_trig_addr;
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] acq_check_data_acquire: "
                "Acquisition is done for channel %u\n", chan);
        smio_thsafe_client_read_32 (self, ACQ_CORE_REG_TRIG_POS, &acq_core_trig_addr);
        acq->acq_params[chan].trig_addr = acq_core_trig_addr;
    }

err_get_acq_handler:
    return err;
}

static int _acq_check_status (SMIO_OWNER_TYPE *self, uint32_t status_mask,
        uint32_t status_value)
{
    int err = -ACQ_OK;
    uint32_t status_done = 0;

    /* Check for completion */
    smio_thsafe_client_read_32 (self, ACQ_CORE_REG_STA, &status_done);
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] data_acquire: "
            "Status done = 0x%08x\n", status_done);

    if ((status_done & status_mask) != status_value) {
        err = -ACQ_NOT_COMPLETED;
        goto err_not_completed;
    }

    err = -ACQ_OK;

err_not_completed:
    return err;
}

static int _acq_get_data_block (void *owner, void *args, void *ret)
{
    assert (owner);
    assert (args);

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] "
            "Calling _acq_get_data_block\n");

    int err = -ACQ_OK;
    SMIO_OWNER_TYPE *self = SMIO_EXP_OWNER(owner);
    smio_acq_t *acq = smio_get_handler (self);
    ASSERT_TEST(acq != NULL, "Could not get SMIO ACQ handler",
            err_get_acq_handler, -ACQ_ERR);

    /* Message is:
     * frame 0: channel
     * frame 1: block required      */
    uint32_t chan = *(uint32_t *) EXP_MSG_ZMQ_FIRST_ARG(args);
    uint32_t block_n = *(uint32_t *) EXP_MSG_ZMQ_NEXT_ARG(args);
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] get_data_block: "
            "chan = %u, block_n = %u\n", chan, block_n);

    /* channel required is out of the limit */
    if (chan > acq->num_chan-1) {
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_WARN, "[sm_io:acq] data_acquire: "
                "Channel required is out of the maximum limit\n");

        return -ACQ_NUM_CHAN_OOR;
    }

    /* Channel features */
    uint32_t channel_sample_size = acq->acq_buf[chan].sample_size;
    uint32_t channel_start_addr = acq->acq_buf[chan].start_addr;
    uint32_t channel_end_addr = acq->acq_buf[chan].end_addr;
    uint32_t channel_max_samples = acq->acq_buf[chan].max_samples;
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] get_data_block: "
            "\t[channel = %u], id = %u, start addr = 0x%08x\n"
            "\tend addr = 0x%08x, max samples = %u, sample size = %u\n",
            chan,
            acq->acq_buf[chan].id,
            channel_start_addr,
            channel_end_addr,
            channel_max_samples,
            channel_sample_size);

    uint32_t block_n_max = (channel_max_samples*channel_sample_size) /
        BLOCK_SIZE;
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] get_data_block: "
            "block_n_max = %u\n", block_n_max);

    if (block_n > block_n_max) {    /* block required out of the limits */
        /* TODO error level in this case */
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_ERR, "[sm_io:acq] get_data_block: "
                "Block %u of channel %u is out of range\n", block_n, chan);
        return -ACQ_BLOCK_OOR;
    }

    /* Get number of samples and shots */
    uint32_t num_samples_pre =
        acq->acq_params[chan].num_samples_pre;
    uint32_t num_samples_post =
        acq->acq_params[chan].num_samples_post;
    uint32_t num_samples_shot = num_samples_pre + num_samples_post;
    uint32_t num_shots =
        acq->acq_params[chan].num_shots;
    uint32_t num_samples_multishot = num_samples_shot*num_shots;
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] get_data_block: "
            "last num_samples_pre = %u\n", num_samples_pre);
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] get_data_block: "
            "last num_samples_post = %u\n", num_samples_post);
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] get_data_block: "
            "last num_shots = %u\n", num_shots);

    uint32_t n_max_samples = BLOCK_SIZE/channel_sample_size;
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] get_data_block: "
            "n_max_samples = %u\n", n_max_samples);

    uint32_t over_samples = num_samples_multishot % n_max_samples;
    uint32_t block_n_valid = num_samples_multishot / n_max_samples;
    /* When the last block is full 'block_n_valid' exceeds by one */
    if (block_n_valid != 0 && over_samples == 0) {
        block_n_valid--;
    }
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] get_data_block: "
            "block_n_valid= %u, over_samples= %u\n",
            block_n_valid, over_samples);

    /* check if block required is valid and if it is full or not */
    if (block_n > block_n_valid) {
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_ERR, "[sm_io:acq] get_data_block: "
                "Block %u of channel %u is not valid\n", block_n, chan);
        return -ACQ_BLOCK_OOR;
    }   /* Last valid data conditions check done */

    uint32_t reply_size_full;
    if (block_n == block_n_valid && over_samples > 0){
        reply_size_full = over_samples*channel_sample_size;
    }
    else { /* if block_n < block_n_valid */
        reply_size_full = BLOCK_SIZE;
    }

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] get_data_block: "
            "Reading block %u of channel %u with %u valid samples\n",
            block_n, chan, reply_size_full);

    /* For all modes the start valid address is given by:
     * start_addr = trigger_addr -
     * ((num_samples_pre+num_samples_post)*(num_shots-1) + num_samples_pre)*
     * sample_size
     * */

    /* First step if to get the trigger address from the channel.
     * Even on skip trigger mode, this will contain the address after
     * the last valid sample (end of acquisition address) */
    uint32_t acq_core_trig_addr = acq->acq_params[chan].trig_addr;

    /* Second step is to calculate the size of the whole acquisition in bytes */
    uint32_t acq_size_bytes = (num_samples_shot*(num_shots-1) +
            num_samples_pre)*channel_sample_size;
    /* Our "end address" is the start of the last valid address available for a
     * sample. So, our "end address" needs to be accounted for one sample more */
    uint32_t end_mem_space_addr = channel_end_addr + channel_sample_size;

    /* Third step is to get the absolute start address of the acquisition, taking
     * care for wraps in the beginning of the current memory space */
    uint64_t start_addr = _acq_get_start_address (acq_core_trig_addr,
            acq_size_bytes, channel_start_addr, end_mem_space_addr);

    /* Forth step is to calculate the offset from the start_addr, taking care
     * for wraps in the end of the current memory space */
    uint64_t addr_i = _acq_get_read_block_addr_start (start_addr, block_n * BLOCK_SIZE,
            channel_start_addr, end_mem_space_addr);
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] get_data_block:\n"
            "\tBlock %u of channel %u:\n"
            "\tChannel start address = 0x%08x,\n"
            "\tTrigger address = 0x%08x,\n"
            "\tAcquisition read start address\n"
            "\t\t(trig_addr - ((num_samples_pre+num_samples_post)*(num_shots-1)\n"
            "\t\t+ num_samples_pre)*sample_size = 0x%"PRIx64 ",\n"
            "\tCurrent block start address (read_start_addr + block_n*BLOCK_SIZE) = 0x%"PRIx64 "\n",
            block_n, chan,
            channel_start_addr,
            acq_core_trig_addr,
            start_addr,
            addr_i);

    smio_acq_data_block_t *data_block = (smio_acq_data_block_t *) ret;

    uint32_t num_bytes_read = 0;
    ssize_t valid_bytes = 0;
    uint64_t addr_start = addr_i;
    uint64_t rem_mem_space = end_mem_space_addr - addr_start;
    for ( ;
            num_bytes_read < reply_size_full;
            num_bytes_read += valid_bytes) {
        uint32_t bytes_to_read = (reply_size_full-num_bytes_read > rem_mem_space)?
                rem_mem_space : reply_size_full-num_bytes_read;

        DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] get_data_block: "
                "Reading %u bytes from address 0x%"PRIx64"\n"
                "\twith remaining memory space 0x%"PRIx64"\n"
                "\twith end memory space 0x%08X\n",
                bytes_to_read, addr_start,
                rem_mem_space,
                end_mem_space_addr);

        /* Here we must use the "raw" version, as we can't have
         * LARGE_MEM_ADDR mangled with the bas address of this SMIO */
        valid_bytes = smio_thsafe_raw_client_read_dma (self, LARGE_MEM_ADDR | addr_start,
                bytes_to_read, (uint32_t *)((uint8_t *) data_block->data + num_bytes_read));
        /* Try reading block-by-block if DMA fails for some reason */
        if (valid_bytes < 0) {
            valid_bytes = smio_thsafe_raw_client_read_block (self, LARGE_MEM_ADDR | addr_start,
                    bytes_to_read, (uint32_t *)((uint8_t *) data_block->data + num_bytes_read));
        }

        ASSERT_TEST(valid_bytes == bytes_to_read, "Could not read data block completely",
                err_read_data_block, -ACQ_COULD_NOT_READ);
        addr_start += valid_bytes;
        if (addr_start >= end_mem_space_addr) {
            addr_start = channel_start_addr;
        }

        rem_mem_space = end_mem_space_addr - addr_start;
    }

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] get_data_block: "
            "%u bytes read\n", num_bytes_read);

    /* Check if we could read successfully */
    int retf = 0;
    data_block->valid_bytes = (uint32_t) num_bytes_read;
    retf = num_bytes_read + (ssize_t) sizeof (data_block->valid_bytes);

    return retf;

err_read_data_block:
    data_block->valid_bytes = 0;
err_get_acq_handler:
    return err;
}

static uint64_t _acq_get_start_address (uint64_t acq_core_trig_addr,
        uint64_t acq_size_bytes, uint64_t start_mem_space_addr,
        uint64_t end_mem_space_addr)
{
    uint64_t addr = 0;

    /* Trigger address relative to the start of the memory space */
    uint64_t rel_start_trig_addr = acq_core_trig_addr - start_mem_space_addr;
    /* Check if an address wrap can occur */
    if (rel_start_trig_addr >= acq_size_bytes) {
        addr = acq_core_trig_addr - acq_size_bytes;
    }
    else {
        addr = end_mem_space_addr - (acq_size_bytes - rel_start_trig_addr);
    }

    return addr;
}

static uint64_t _acq_get_read_block_addr_start (uint64_t start_addr, uint64_t offset,
        uint64_t channel_start_addr, uint64_t end_mem_space_addr)
{
    uint64_t addr = 0;

    /* Memory space last address relative to the start address */
    uint64_t rel_end_mem_space_addr = end_mem_space_addr - start_addr;
    /* Check if an address wrap can occur */
    if (offset <= rel_end_mem_space_addr) {
        addr = start_addr + offset;
    }
    else {
        addr = channel_start_addr + (offset - rel_end_mem_space_addr);
    }

    return addr;
}

static int _acq_cfg_trigger (void *owner, void *args, void *ret)
{
    UNUSED(ret);
    assert (owner);
    assert (args);
    int err = -ACQ_OK;

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] "
            "Calling _acq_cfg_trigger\n");
    SMIO_OWNER_TYPE *self = SMIO_EXP_OWNER(owner);
    smio_acq_t *acq = smio_get_handler (self);
    ASSERT_TEST(acq != NULL, "Could not get SMIO ACQ handler",
            err_get_acq_handler, -ACQ_ERR);

    /* Message is:
     * frame 0: operation code
     * frame 1: rw
     * frame 2: trigger type (0 -> skip trigger, 1 -> external trigger,
     *                          2 -> data-driven trigger, 3 -> software trigger)
     */
    uint32_t rw = *(uint32_t *) EXP_MSG_ZMQ_FIRST_ARG(args);
    uint32_t trigger_type = *(uint32_t *) EXP_MSG_ZMQ_NEXT_ARG(args);

    /* channel required is out of the limit */
    ASSERT_TEST(trigger_type < ACQ_CORE_NUM_TRIGGERS, "Trigger type is not valid",
            err_acq_not_completed, -ACQ_NUM_CHAN_OOR);

    uint32_t trigger_type_ret = 0;
    if (rw) {
        err = _acq_get_trigger_type (self, &trigger_type_ret);
        ASSERT_TEST(err == -ACQ_OK, "Trigger type is not valid", err_acq_get_trig);

        /* Return value to caller */
        *((uint32_t *) ret) = trigger_type_ret;
        err = sizeof (trigger_type_ret);
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] "
                "Trigger type = 0x%08X\n", trigger_type_ret);
    }
    else {
        /* Only check if the FPGA is already doing an acquisition if we are about
         * to change it. If it is, then return an error. Otherwise proceed normally. */
        err = _acq_check_status (self, ACQ_CORE_IDLE_MASK, ACQ_CORE_IDLE_VALUE);
        ASSERT_TEST(err == -ACQ_OK, "Previous acquisition in progress. "
                "Cannot change trigger type", err_acq_not_completed);

        err = _acq_set_trigger_type (self, trigger_type);
        ASSERT_TEST(err == -ACQ_OK, "Trigger type is not valid", err_acq_inv_trig);
    }

    return err;

err_acq_inv_trig:
err_acq_not_completed:
err_acq_get_trig:
err_get_acq_handler:
    return err;
}

static int _acq_set_trigger_type (SMIO_OWNER_TYPE *self, uint32_t trigger_type)
{
    int err = -ACQ_OK;

    /* Read control registers so we don't overwrite previous configs */
    uint32_t acq_core_ctl_reg = 0;
    uint32_t acq_core_trig_cfg_reg = 0;
    smio_thsafe_client_read_32 (self, ACQ_CORE_REG_CTL, &acq_core_ctl_reg);
    smio_thsafe_client_read_32 (self, ACQ_CORE_REG_TRIG_CFG, &acq_core_trig_cfg_reg);

    /* Select between our trigger types */
    switch (trigger_type) {
        case TYPE_ACQ_CORE_SKIP:
            /* Enable skip trigger */
            acq_core_ctl_reg |= ACQ_CORE_CTL_FSM_ACQ_NOW;
            /* Disable hardware trigger */
            acq_core_trig_cfg_reg &= ~ACQ_CORE_TRIG_CFG_HW_TRIG_EN;
            /* Disable software trigger */
            acq_core_trig_cfg_reg &= ~ACQ_CORE_TRIG_CFG_SW_TRIG_EN;
        break;

        /* Set HW_TRIG_SEL bit (1 is HW pulse trigger) */
        case TYPE_ACQ_CORE_HW_PULSE:
            /* Disable skip trigger */
            acq_core_ctl_reg &= ~ACQ_CORE_CTL_FSM_ACQ_NOW;
            /* Enable hardware trigger */
            acq_core_trig_cfg_reg |= ACQ_CORE_TRIG_CFG_HW_TRIG_EN;
            /* Disable software trigger */
            acq_core_trig_cfg_reg &= ~ACQ_CORE_TRIG_CFG_SW_TRIG_EN;
            /* Select external pulse trigger */
            acq_core_trig_cfg_reg |= ACQ_CORE_TRIG_CFG_HW_TRIG_SEL;
        break;

        /* Clear HW_TRIG_SEL bit (0 is HW data-driven trigger) */
        case TYPE_ACQ_CORE_HW_DATA_DRIVEN:
            /* Disable skip trigger */
            acq_core_ctl_reg &= ~ACQ_CORE_CTL_FSM_ACQ_NOW;
            /* Enable hardware trigger */
            acq_core_trig_cfg_reg |= ACQ_CORE_TRIG_CFG_HW_TRIG_EN;
            /* Disable software trigger */
            acq_core_trig_cfg_reg &= ~ACQ_CORE_TRIG_CFG_SW_TRIG_EN;
            /* Enable data-driven trigger */
            acq_core_trig_cfg_reg &= ~ACQ_CORE_TRIG_CFG_HW_TRIG_SEL;
        break;

        case TYPE_ACQ_CORE_SW:
            /* Disable skip trigger */
            acq_core_ctl_reg &= ~ACQ_CORE_CTL_FSM_ACQ_NOW;
            /* Disable hardware trigger */
            acq_core_trig_cfg_reg &= ~ACQ_CORE_TRIG_CFG_HW_TRIG_EN;
            /* Enable software trigger */
            acq_core_trig_cfg_reg |= ACQ_CORE_TRIG_CFG_SW_TRIG_EN;
        break;

        default:
            return -ACQ_ERR;
    }

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] cfg_trigger: "
            "Control register is: 0x%08x\n",
            acq_core_ctl_reg);
    smio_thsafe_client_write_32 (self, ACQ_CORE_REG_CTL, &acq_core_ctl_reg);
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] cfg_trigger: "
            "Trigger config register is: 0x%08x\n",
            acq_core_trig_cfg_reg);
    smio_thsafe_client_write_32 (self, ACQ_CORE_REG_TRIG_CFG, &acq_core_trig_cfg_reg);

    return err;
}

static int _acq_get_trigger_type (SMIO_OWNER_TYPE *self, uint32_t *trigger_type)
{
    int err = -ACQ_OK;

    uint32_t acq_core_ctl_reg = 0;
    uint32_t acq_core_trig_cfg_reg = 0;
    smio_thsafe_client_read_32 (self, ACQ_CORE_REG_CTL, &acq_core_ctl_reg);
    smio_thsafe_client_read_32 (self, ACQ_CORE_REG_TRIG_CFG, &acq_core_trig_cfg_reg);

    /* Check for SKIP trigger */
    if (acq_core_ctl_reg & ACQ_CORE_CTL_FSM_ACQ_NOW) {
        *trigger_type = (int) TYPE_ACQ_CORE_SKIP;
        /* Check if other trigger types are also set */
        ASSERT_TEST(!(acq_core_trig_cfg_reg & (ACQ_CORE_TRIG_CFG_HW_TRIG_EN |
                        ACQ_CORE_TRIG_CFG_SW_TRIG_EN)), "Invalid skip trigger type",
                err_inv_skip_trig, -ACQ_ERR);
        return -ACQ_OK;
    }

    /* We don't have the SKIP trigger flag set. Check for HW trigger */
    if (acq_core_trig_cfg_reg & ACQ_CORE_TRIG_CFG_HW_TRIG_EN) {
        /* We can have SW trigger enable and it isn't an error */
        if (acq_core_trig_cfg_reg & ACQ_CORE_TRIG_CFG_HW_TRIG_SEL) {
            *trigger_type = (int) TYPE_ACQ_CORE_HW_PULSE;
        }
        else {
            *trigger_type = (int) TYPE_ACQ_CORE_HW_DATA_DRIVEN;
        }
        return -ACQ_OK;
    }

    /* We don't have SKIP nor HW flags set. Check for SW trigger */
    if (acq_core_trig_cfg_reg & ACQ_CORE_TRIG_CFG_SW_TRIG_EN) {
        /* We can have SW trigger enable and it isn't an error */
        *trigger_type = (int) TYPE_ACQ_CORE_SW;
        return -ACQ_OK;
    }

err_inv_skip_trig:
    return err;
}


#define ACQ_HW_DATA_TRIG_POL_MIN                    0       /* positive slope: 0 -> 1 */
#define ACQ_HW_DATA_TRIG_POL_MAX                    1       /* negative slope: 1 -> 0 */
RW_PARAM_FUNC(acq, hw_data_trig_pol) {
    SET_GET_PARAM(acq, 0x0, ACQ_CORE, TRIG_CFG,
            HW_TRIG_POL, SINGLE_BIT_PARAM, ACQ_HW_DATA_TRIG_POL_MIN,
            ACQ_HW_DATA_TRIG_POL_MAX, NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define ACQ_HW_DATA_TRIG_SEL_MIN                    0
#define ACQ_HW_DATA_TRIG_SEL_MAX                    ((1 << 5)-1)
RW_PARAM_FUNC(acq, hw_data_trig_sel) {
    SET_GET_PARAM(acq, 0x0, ACQ_CORE, TRIG_CFG,
            INT_TRIG_SEL, MULT_BIT_PARAM, ACQ_HW_DATA_TRIG_SEL_MIN,
            ACQ_HW_DATA_TRIG_SEL_MAX, NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define ACQ_HW_DATA_TRIG_FILT_MIN                   0
#define ACQ_HW_DATA_TRIG_FILT_MAX                   ((1 << 8)-1)
RW_PARAM_FUNC(acq, hw_data_trig_filt) {
    SET_GET_PARAM(acq, 0x0, ACQ_CORE, TRIG_DATA_CFG,
            THRES_FILT, MULT_BIT_PARAM, ACQ_HW_DATA_TRIG_FILT_MIN,
            ACQ_HW_DATA_TRIG_FILT_MAX, NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define ACQ_CORE_TRIG_DATA_THRES_R(val)             (val)
#define ACQ_CORE_TRIG_DATA_THRES_W(val)             (val)
#define ACQ_CORE_TRIG_DATA_THRES_MASK               ((1ULL<<32)-1)
RW_PARAM_FUNC(acq, hw_data_trig_thres) {
    SET_GET_PARAM(acq, 0x0, ACQ_CORE, TRIG_DATA_THRES,
            /* No field */, MULT_BIT_PARAM, /* No minimum check*/,
            /* No maximum check */, NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

/* */
#define ACQ_CORE_TRIG_DLY_R(val)                    (val)
#define ACQ_CORE_TRIG_DLY_W(val)                    (val)
#define ACQ_CORE_TRIG_DLY_MASK                      ((1ULL<<32)-1)
RW_PARAM_FUNC(acq, hw_trig_dly) {
    SET_GET_PARAM(acq, 0x0, ACQ_CORE, TRIG_DLY,
            /* No field*/, MULT_BIT_PARAM, /* No minimum check */,
            /* No maximum check */, NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define ACQ_CORE_SW_TRIG_R(val)                     (val)
#define ACQ_CORE_SW_TRIG_W(val)                     (val)
#define ACQ_CORE_SW_TRIG_MASK                       ((1ULL<<32)-1)
#define ACQ_SW_TRIG_MIN                             0
#define ACQ_SW_TRIG_MAX                             1
RW_PARAM_FUNC(acq, sw_trig) {
    SET_GET_PARAM(acq, 0x0, ACQ_CORE, SW_TRIG,
            /* No field*/, MULT_BIT_PARAM, ACQ_SW_TRIG_MIN,
            ACQ_SW_TRIG_MAX, NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define ACQ_FSM_STOP_MIN                            0
#define ACQ_FSM_STOP_MAX                            1
RW_PARAM_FUNC(acq, fsm_stop) {
    SET_GET_PARAM(acq, 0x0, ACQ_CORE, CTL,
            FSM_STOP_ACQ, SINGLE_BIT_PARAM, ACQ_FSM_STOP_MIN,
            ACQ_FSM_STOP_MAX, NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define ACQ_DATA_DRIVEN_CHAN_MIN                    0
#define ACQ_DATA_DRIVEN_CHAN_MAX                    ((1<<5)-1)
RW_PARAM_FUNC(acq, hw_data_trig_chan) {
    SET_GET_PARAM(acq, 0x0, ACQ_CORE, ACQ_CHAN_CTL,
            DTRIG_WHICH, MULT_BIT_PARAM, ACQ_DATA_DRIVEN_CHAN_MIN,
            ACQ_DATA_DRIVEN_CHAN_MAX, NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

#define ACQ_CORE_INT_WIDTH_MIN                      0
#define ACQ_CORE_INT_WIDTH_MAX                      ((1 << 16)-1)
RW_PARAM_FUNC(acq, int_width) {
    SET_GET_PARAM_CHANNEL(acq, 0x0, ACQ_CORE,
            CH0_DESC, INT_WIDTH, ACQ_CORE_CHAN_DESC_OFFSET, ACQ_CORE_NUM_CHAN_DESC,
            MULT_BIT_PARAM, ACQ_CORE_INT_WIDTH_MIN, ACQ_CORE_INT_WIDTH_MAX, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

#define ACQ_CORE_NUM_COALESCE_MIN                   0
#define ACQ_CORE_NUM_COALESCE_MAX                   ((1 << 16)-1)
RW_PARAM_FUNC(acq, num_coalesce) {
    SET_GET_PARAM_CHANNEL(acq, 0x0, ACQ_CORE,
            CH0_DESC, NUM_COALESCE, ACQ_CORE_CHAN_DESC_OFFSET, ACQ_CORE_NUM_CHAN_DESC,
            MULT_BIT_PARAM, ACQ_CORE_NUM_COALESCE_MIN, ACQ_CORE_NUM_COALESCE_MAX, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

#define ACQ_CORE_NUM_ATOMS_MIN                      0
#define ACQ_CORE_NUM_ATOMS_MAX                      ((1 << 16)-1)
RW_PARAM_FUNC(acq, num_atoms) {
    SET_GET_PARAM_CHANNEL(acq, 0x0, ACQ_CORE,
            CH0_ATOM_DESC, NUM_ATOMS, ACQ_CORE_CHAN_DESC_OFFSET, ACQ_CORE_NUM_CHAN_DESC,
            MULT_BIT_PARAM, ACQ_CORE_NUM_ATOMS_MIN, ACQ_CORE_NUM_ATOMS_MAX, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

#define ACQ_CORE_ATOM_WIDTH_MIN                     0
#define ACQ_CORE_ATOM_WIDTH_MAX                     ((1 << 16)-1)
RW_PARAM_FUNC(acq, atom_width) {
    SET_GET_PARAM_CHANNEL(acq, 0x0, ACQ_CORE,
            CH0_ATOM_DESC, ATOM_WIDTH, ACQ_CORE_CHAN_DESC_OFFSET, ACQ_CORE_NUM_CHAN_DESC,
            MULT_BIT_PARAM, ACQ_CORE_ATOM_WIDTH_MIN, ACQ_CORE_ATOM_WIDTH_MAX, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

static int _acq_sample_size (void *owner, void *args, void *ret)
{
    UNUSED(ret);
    assert (owner);
    assert (args);
    int err = -ACQ_OK;

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] "
            "Calling _acq_cfg_trigger\n");
    SMIO_OWNER_TYPE *self = SMIO_EXP_OWNER(owner);
    smio_acq_t *acq = smio_get_handler (self);
    ASSERT_TEST(acq != NULL, "Could not get SMIO ACQ handler",
            err_get_acq_handler, -ACQ_ERR);

    /* Message is:
     * frame 0: operation code
     * frame 1: rw
     * frame 2: channel number
     * frame 3: value
     */
    uint32_t rw = *(uint32_t *) EXP_MSG_ZMQ_FIRST_ARG(args);
    uint32_t channel_number = *(uint32_t *) EXP_MSG_ZMQ_NEXT_ARG(args);
    uint32_t value = *(uint32_t *) EXP_MSG_ZMQ_NEXT_ARG(args);
    UNUSED(value);

    /* channel required is out of the limit */
    ASSERT_TEST(channel_number < ACQ_CORE_NUM_CHAN_DESC, "Channel number is not valid",
            err_acq_get_chan, -ACQ_NUM_CHAN_OOR);

    uint32_t sample_size = 0;
    uint32_t int_ch_width = 0;
    uint32_t num_coalesce = 0;
    if (rw) {
        /* Get channel properties */
        GET_PARAM_CHANNEL(self, acq, 0x0, ACQ_CORE, CH0_DESC, INT_WIDTH,
                ACQ_CORE_CHAN_DESC_OFFSET, channel_number, MULT_BIT_PARAM, int_ch_width,
                NO_FMT_FUNC);
        GET_PARAM_CHANNEL(self, acq, 0x0, ACQ_CORE, CH0_DESC, NUM_COALESCE,
                ACQ_CORE_CHAN_DESC_OFFSET, channel_number, MULT_BIT_PARAM, num_coalesce,
                NO_FMT_FUNC);
        sample_size = int_ch_width/DDR3_BYTE_2_BIT * num_coalesce;

        /* Return value to caller */
        *((uint32_t *) ret) = sample_size;
        err = sizeof (sample_size);
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq] "
                "Sample size = %u\n", sample_size);
    }

    return err;

err_acq_get_chan:
err_get_acq_handler:
    return err;
}

/* Exported function pointers */
const disp_table_func_fp acq_exp_fp [] = {
    _acq_data_acquire,
    _acq_check_data_acquire,
    _acq_get_data_block,
    _acq_cfg_trigger,
    RW_PARAM_FUNC_NAME(acq, hw_data_trig_pol),
    RW_PARAM_FUNC_NAME(acq, hw_data_trig_sel),
    RW_PARAM_FUNC_NAME(acq, hw_data_trig_filt),
    RW_PARAM_FUNC_NAME(acq, hw_data_trig_thres),
    RW_PARAM_FUNC_NAME(acq, hw_trig_dly),
    RW_PARAM_FUNC_NAME(acq, sw_trig),
    RW_PARAM_FUNC_NAME(acq, fsm_stop),
    RW_PARAM_FUNC_NAME(acq, hw_data_trig_chan),
    RW_PARAM_FUNC_NAME(acq, int_width),
    RW_PARAM_FUNC_NAME(acq, num_coalesce),
    RW_PARAM_FUNC_NAME(acq, num_atoms),
    RW_PARAM_FUNC_NAME(acq, atom_width),
    _acq_sample_size,
    NULL
};

/************************************************************/
/***************** Export methods functions *****************/
/************************************************************/

static smio_err_e _acq_do_op (void *owner, void *msg);

/* Attach an instance of sm_io to dev_io function pointer */
smio_err_e acq_attach (smio_t *self, void *args)
{
    UNUSED(self);
    UNUSED(args);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Deattach an instance of sm_io to dev_io function pointer */
smio_err_e acq_deattach (smio_t *self)
{
    UNUSED(self);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Export (register) sm_io to handle operations function pointer */
smio_err_e acq_export_ops (smio_t *self,
        const disp_op_t** smio_exp_ops)
{
    UNUSED(self);
    UNUSED(smio_exp_ops);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Unexport (unregister) sm_io to handle operations function pointer */
smio_err_e acq_unexport_ops (smio_t *self)
{
    UNUSED(self);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Generic wrapper for receiving opcodes and arguments to specific funtions function pointer */
/* FIXME: Code repetition! _devio_do_smio_op () function does almost the same!!! */
smio_err_e _acq_do_op (void *owner, void *msg)
{
    UNUSED(owner);
    UNUSED(msg);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

smio_err_e acq_do_op (void *self, void *msg)
{
    return _acq_do_op (self, msg);
}

const smio_ops_t acq_ops = {
    .attach             = &acq_attach,          /* Attach sm_io instance to dev_io */
    .deattach           = &acq_deattach,        /* Deattach sm_io instance to dev_io */
    .export_ops         = &acq_export_ops,      /* Export sm_io operations to dev_io */
    .unexport_ops       = &acq_unexport_ops,    /* Unexport sm_io operations to dev_io */
    .do_op              = &acq_do_op            /* Generic wrapper for handling specific operations */
};

/************************************************************/
/****************** Bootstrap Operations ********************/
/************************************************************/

smio_err_e acq_init (smio_t * self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq_exp] Initializing acq\n");

    smio_err_e err = SMIO_SUCCESS;

    err = smio_set_id (self, ACQ_SDB_DEVID);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO id", err_set_id);
    err = smio_set_name (self, ACQ_SDB_NAME);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO name", err_set_name);

    /* Set SMIO ops pointers */
    err = smio_set_ops (self, &acq_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO operations",
            err_smio_set_ops);
    err = smio_set_thsafe_client_ops (self, &smio_thsafe_client_zmq_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO thsafe operations",
            err_smio_set_thsafe_ops);

    /* Fill the disp_op_t description structure with the callbacks. */

    /* disp_op_t structure is const and all of the functions performing on it
     * obviously receives a const argument, but here (and only on the SMIO
     * initialization) we need to make an exception if we want to keep the
     * functions' description and the function pointers separate */
    err = smio_init_exp_ops (self, (disp_op_t **) acq_exp_ops, acq_exp_fp);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not fill SMIO "
            "function descriptors with the callbacks", err_fill_desc);

    err = smio_set_exp_ops (self, acq_exp_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO exported operations",
            err_smio_set_exp_ops);

    /* Initialize specific structure */
    smio_acq_t *smio_handler = smio_acq_new (self, 0, 0, 1);
    ASSERT_ALLOC(smio_handler, err_smio_handler_alloc, SMIO_ERR_ALLOC);
    err = smio_set_handler (self, smio_handler);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO handler",
            err_smio_set_handler);

    return err;

err_smio_set_handler:
    smio_acq_destroy (&smio_handler);
err_smio_handler_alloc:
    smio_set_exp_ops (self, NULL);
err_smio_set_exp_ops:
err_fill_desc:
    smio_set_thsafe_client_ops (self, NULL);
err_smio_set_thsafe_ops:
    smio_set_ops (self, NULL);
err_smio_set_ops:
err_set_name:
err_set_id:
    return err;
}

/* Destroy sm_io instance of acq */
smio_err_e acq_shutdown (smio_t *self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq_exp] Shutting down acq\n");

    smio_err_e err = SMIO_SUCCESS;
    smio_acq_t *acq = smio_get_handler (self);
    ASSERT_TEST(acq != NULL, "Could not get ACQ handler",
            err_acq_handler, SMIO_ERR_ALLOC /* FIXME: improve return code */);

    /* Destroy SMIO instance */
    smio_acq_destroy (&acq);
    /* Nullify operation pointers */
    smio_set_exp_ops (self, NULL);
    smio_set_thsafe_client_ops (self, NULL);
    smio_set_ops (self, NULL);

err_acq_handler:
    return err;
}

const smio_bootstrap_ops_t acq_bootstrap_ops = {
    .init     = &acq_init,
    .shutdown = &acq_shutdown
};

SMIO_MOD_DECLARE(ACQ_SDB_DEVID, ACQ_SDB_NAME, acq_bootstrap_ops)
