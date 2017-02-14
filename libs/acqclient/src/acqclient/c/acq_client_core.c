/*
 * Copyright (C) 2017 LNLS (www.lnls.br)
 * Authors: Lucas Russo <lucas.russo@lnls.br>
 *          Janito Vaqueiro Ferreira Filho <janito.filho@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "acq_client.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, LIB_CLIENT, "[libacqclient]",\
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_ALLOC(ptr, LIB_CLIENT, "[libacqclient]",        \
            halcs_client_err_str(HALCS_CLIENT_ERR_ALLOC),       \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                            \
    CHECK_HAL_ERR(err, LIB_CLIENT, "[libacqclient]",           \
            halcs_client_err_str (err_type))

/********************************************************/
/************************ Our API ***********************/
/********************************************************/

/****************** ACQ SMIO Functions ****************/

static halcs_client_err_e _acq_start (halcs_client_t *self, char *service,
        acq_req_t *acq_req);
static halcs_client_err_e _acq_check (halcs_client_t *self, char *service);
static halcs_client_err_e _acq_check_timed (halcs_client_t *self, char *service,
        int timeout);
static halcs_client_err_e _acq_get_data_block (halcs_client_t *self,
        char *service, acq_trans_t *acq_trans);
static halcs_client_err_e _acq_get_curve (halcs_client_t *self, char *service,
        acq_trans_t *acq_trans);
static halcs_client_err_e _acq_full (halcs_client_t *self, char *service,
        acq_trans_t *acq_trans, int timeout);
static halcs_client_err_e _acq_full_compat (halcs_client_t *self, char *service,
        acq_trans_t *acq_trans, int timeout, bool new_acq);

halcs_client_err_e acq_start (halcs_client_t *self, char *service, acq_req_t *acq_req)
{
    return _acq_start (self, service, acq_req);
}

halcs_client_err_e acq_check (halcs_client_t *self, char *service)
{
    return _acq_check (self, service);
}

halcs_client_err_e acq_check_timed (halcs_client_t *self, char *service,
        int timeout)
{
    return _acq_check_timed (self, service, timeout);
}

halcs_client_err_e acq_get_data_block (halcs_client_t *self, char *service,
        acq_trans_t *acq_trans)
{
    return _acq_get_data_block (self, service, acq_trans);
}

halcs_client_err_e acq_get_curve (halcs_client_t *self, char *service,
        acq_trans_t *acq_trans)
{
    return _acq_get_curve (self, service, acq_trans);
}

halcs_client_err_e acq_full (halcs_client_t *self, char *service,
        acq_trans_t *acq_trans, int timeout)
{
    return _acq_full (self, service, acq_trans, timeout);
}

halcs_client_err_e acq_full_compat (halcs_client_t *self, char *service,
        acq_trans_t *acq_trans, int timeout, bool new_acq)
{
    return _acq_full_compat (self, service, acq_trans, timeout, new_acq);
}

static halcs_client_err_e _acq_check_timed (halcs_client_t *self, char *service,
        int timeout)
{
    return func_polling (self, ACQ_NAME_CHECK_DATA_ACQUIRE, service, NULL,
            NULL, timeout);
}

halcs_client_err_e acq_set_trig (halcs_client_t *self, char *service,
        uint32_t trig)
{
    return param_client_write (self, service, ACQ_OPCODE_CFG_TRIG, trig);
}

halcs_client_err_e acq_get_trig (halcs_client_t *self, char *service,
        uint32_t *trig)
{
    return param_client_read (self, service, ACQ_OPCODE_CFG_TRIG, trig);
}

halcs_client_err_e acq_set_data_trig_pol (halcs_client_t *self, char *service,
        uint32_t data_trig_pol)
{
    return param_client_write (self, service, ACQ_OPCODE_HW_DATA_TRIG_POL,
            data_trig_pol);
}

halcs_client_err_e acq_get_data_trig_pol (halcs_client_t *self, char *service,
        uint32_t *data_trig_pol)
{
    return param_client_read (self, service, ACQ_OPCODE_HW_DATA_TRIG_POL,
            data_trig_pol);
}

halcs_client_err_e acq_set_data_trig_sel (halcs_client_t *self, char *service,
        uint32_t data_trig_sel)
{
    return param_client_write (self, service, ACQ_OPCODE_HW_DATA_TRIG_SEL,
            data_trig_sel);
}

halcs_client_err_e acq_get_data_trig_sel (halcs_client_t *self, char *service,
        uint32_t *data_trig_sel)
{
    return param_client_read (self, service, ACQ_OPCODE_HW_DATA_TRIG_SEL,
            data_trig_sel);
}

halcs_client_err_e acq_set_data_trig_filt (halcs_client_t *self, char *service,
        uint32_t data_trig_filt)
{
    return param_client_write (self, service, ACQ_OPCODE_HW_DATA_TRIG_FILT,
            data_trig_filt);
}
halcs_client_err_e acq_get_data_trig_filt (halcs_client_t *self, char *service,
        uint32_t *data_trig_filt)
{
    return param_client_read (self, service, ACQ_OPCODE_HW_DATA_TRIG_FILT,
            data_trig_filt);
}

halcs_client_err_e acq_set_data_trig_thres (halcs_client_t *self, char *service,
        uint32_t data_trig_thres)
{
    return param_client_write (self, service, ACQ_OPCODE_HW_DATA_TRIG_THRES,
            data_trig_thres);
}

halcs_client_err_e acq_get_data_trig_thres (halcs_client_t *self, char *service,
        uint32_t *data_trig_thres)
{
    return param_client_read (self, service, ACQ_OPCODE_HW_DATA_TRIG_THRES,
            data_trig_thres);
}

halcs_client_err_e acq_set_hw_trig_dly (halcs_client_t *self, char *service,
        uint32_t hw_trig_dly)
{
    return param_client_write (self, service, ACQ_OPCODE_HW_TRIG_DLY,
            hw_trig_dly);
}

halcs_client_err_e acq_get_hw_trig_dly (halcs_client_t *self, char *service,
        uint32_t *hw_trig_dly)
{
    return param_client_read (self, service, ACQ_OPCODE_HW_TRIG_DLY,
            hw_trig_dly);
}

halcs_client_err_e acq_set_sw_trig (halcs_client_t *self, char *service,
        uint32_t sw_trig)
{
    return param_client_write (self, service, ACQ_OPCODE_SW_TRIG,
            sw_trig);
}

halcs_client_err_e acq_get_sw_trig (halcs_client_t *self, char *service,
        uint32_t *sw_trig)
{
    return param_client_read (self, service, ACQ_OPCODE_SW_TRIG,
            sw_trig);
}

halcs_client_err_e acq_set_fsm_stop (halcs_client_t *self, char *service,
        uint32_t fsm_stop)
{
    return param_client_write (self, service, ACQ_OPCODE_FSM_STOP,
            fsm_stop);
}

halcs_client_err_e acq_get_fsm_stop (halcs_client_t *self, char *service,
        uint32_t *fsm_stop)
{
    return param_client_read (self, service, ACQ_OPCODE_FSM_STOP,
            fsm_stop);
}

halcs_client_err_e acq_set_data_trig_chan (halcs_client_t *self, char *service,
        uint32_t data_trig_chan)
{
    return param_client_write (self, service, ACQ_OPCODE_HW_DATA_TRIG_CHAN,
            data_trig_chan);
}

halcs_client_err_e acq_get_data_trig_chan (halcs_client_t *self, char *service,
        uint32_t *data_trig_chan)
{
    return param_client_read (self, service, ACQ_OPCODE_HW_DATA_TRIG_CHAN,
            data_trig_chan);
}

static halcs_client_err_e _acq_start (halcs_client_t *self, char *service, acq_req_t *acq_req)
{
    assert (self);
    assert (service);
    assert (acq_req);

    uint32_t write_val[4] = {0};
    write_val[0] = acq_req->num_samples_pre;
    write_val[1] = acq_req->num_samples_post;
    write_val[2] = acq_req->num_shots;
    write_val[3] = acq_req->chan;

    const disp_op_t* func = halcs_func_translate(ACQ_NAME_DATA_ACQUIRE);
    halcs_client_err_e err = halcs_func_exec(self, func, service, write_val, NULL);

    /* Check if any error occurred */
    ASSERT_TEST(err == HALCS_CLIENT_SUCCESS, "acq_data_acquire: Data acquire was "
            "not requested correctly", err_data_acquire, HALCS_CLIENT_ERR_AGAIN);

    /* If we are here, then the request was successfully acquired*/
    DBE_DEBUG (DBG_LIB_CLIENT | DBG_LVL_TRACE, "[libacqclient] acq_data_acquire: "
            "Data acquire was successfully required\n");

err_data_acquire:
    return err;
}

static halcs_client_err_e _acq_check (halcs_client_t *self, char *service)
{
    assert (self);
    assert (service);

    const disp_op_t* func = halcs_func_translate(ACQ_NAME_CHECK_DATA_ACQUIRE);
    halcs_client_err_e err = halcs_func_exec(self, func, service, NULL, NULL);

    if (err != HALCS_CLIENT_SUCCESS) {
        DBE_DEBUG (DBG_LIB_CLIENT | DBG_LVL_TRACE, "[libacqclient] data acquisition "
                "was not completed\n");
    }
    else {
        DBE_DEBUG (DBG_LIB_CLIENT | DBG_LVL_TRACE, "[libacqclient] data acquisition "
                "was completed\n");
    }

    return err;
}

static halcs_client_err_e _acq_get_data_block (halcs_client_t *self, char *service, acq_trans_t *acq_trans)
{
    assert (self);
    assert (service);
    assert (acq_trans);
    assert (acq_trans->block.data);

    halcs_client_err_e err = HALCS_CLIENT_SUCCESS;

    uint32_t write_val[2] = {0};
    write_val[0] = acq_trans->req.chan;
    write_val[1] = acq_trans->block.idx;

    smio_acq_data_block_t read_val[1];

    /* Sent Message is:
     * frame 0: operation code
     * frame 1: channel
     * frame 2: block required */

    const disp_op_t* func = halcs_func_translate(ACQ_NAME_GET_DATA_BLOCK);
    err = halcs_func_exec(self, func, service, write_val, (uint32_t *) read_val);

    /* Message is:
     * frame 0: error code
     * frame 1: number of bytes read (optional)
     * frame 2: data read (optional) */

    /* Check if any error occurred */
    ASSERT_TEST(err == HALCS_CLIENT_SUCCESS,
            "acq_get_data_block: Data block was not acquired",
            err_get_data_block, HALCS_CLIENT_ERR_SERVER);

    /* Data size effectively returned */
    uint32_t read_size = (acq_trans->block.data_size < read_val->valid_bytes) ?
        acq_trans->block.data_size : read_val->valid_bytes;

    /* Copy message contents to user */
    memcpy (acq_trans->block.data, read_val->data, read_size);

    /* Inform user about the number of bytes effectively copied */
    acq_trans->block.bytes_read = read_size;

    /* Print some debug messages */
    DBE_DEBUG (DBG_LIB_CLIENT | DBG_LVL_TRACE, "[libacqclient] acq_get_data_block: "
            "read_size: %u\n", read_size);
    DBE_DEBUG (DBG_LIB_CLIENT | DBG_LVL_TRACE, "[libacqclient] acq_get_data_block: "
            "acq_trans->block.data: %p\n", acq_trans->block.data);

err_get_data_block:
    return err;
}

static halcs_client_err_e _acq_get_curve (halcs_client_t *self, char *service, acq_trans_t *acq_trans)
{
    assert (self);
    assert (service);
    assert (acq_trans);
    assert (acq_trans->block.data);

    halcs_client_err_e err = HALCS_CLIENT_SUCCESS;
    const acq_chan_t *acq_chan = acq_get_chan (self);

    uint32_t num_samples_shot = acq_trans->req.num_samples_pre +
        acq_trans->req.num_samples_post;
    uint32_t num_samples_multishot = num_samples_shot*acq_trans->req.num_shots;
    uint32_t n_max_samples = BLOCK_SIZE/acq_chan[acq_trans->req.chan].sample_size;
    uint32_t block_n_valid = num_samples_multishot / n_max_samples;
    DBE_DEBUG (DBG_LIB_CLIENT | DBG_LVL_TRACE, "[libacqclient] acq_get_curve: "
            "block_n_valid = %u\n", block_n_valid);

    /* Total bytes read */
    uint32_t total_bread = 0;
    /* Save the original buffer size for later */
    uint32_t data_size = acq_trans->block.data_size;
    uint32_t *original_data_pt = acq_trans->block.data;

    /* Fill all blocks */
    for (uint32_t block_n = 0; block_n <= block_n_valid; block_n++) {
        if (zsys_interrupted) {
            err = HALCS_CLIENT_INT;
            goto halcs_zsys_interrupted;
        }

        acq_trans->block.idx = block_n;
        err = _acq_get_data_block (self, service, acq_trans);

        /* Check for return code */
        ASSERT_TEST(err == HALCS_CLIENT_SUCCESS,
                "_acq_get_data_block failed. block_n is probably out of range",
                err_acq_get_data_block);

        total_bread += acq_trans->block.bytes_read;
        acq_trans->block.data = (uint32_t *)((uint8_t *)acq_trans->block.data + acq_trans->block.bytes_read);
        acq_trans->block.data_size -= acq_trans->block.bytes_read;

        /* Print some debug messages */
        DBE_DEBUG (DBG_LIB_CLIENT | DBG_LVL_TRACE, "[libacqclient] acq_get_curve: "
                "Total bytes read up to now: %u\n", total_bread);
        DBE_DEBUG (DBG_LIB_CLIENT | DBG_LVL_TRACE, "[libacqclient] acq_get_curve: "
                "Data pointer addr: %p\n", acq_trans->block.data);
        DBE_DEBUG (DBG_LIB_CLIENT | DBG_LVL_TRACE, "[libacqclient] acq_get_curve: "
                "Data buffer size: %u\n", acq_trans->block.data_size);
    }

    /* Return to client the total number of bytes read */
    acq_trans->block.bytes_read = total_bread;
    acq_trans->block.data_size = data_size;
    acq_trans->block.data = original_data_pt;

    DBE_DEBUG (DBG_LIB_CLIENT | DBG_LVL_TRACE, "[libacqclient] acq_get_curve: "
            "Data curve of %u bytes was successfully acquired\n", total_bread);

halcs_zsys_interrupted:
err_acq_get_data_block:
    return err;
}

static halcs_client_err_e _acq_full (halcs_client_t *self, char *service,
        acq_trans_t *acq_trans, int timeout)
{
    assert (self);
    assert (service);
    assert (acq_trans);
    assert (acq_trans->block.data);

    /* Send Acquisition Request */
    _acq_start (self, service, &acq_trans->req);

    /* Wait until the acquisition is finished */
    halcs_client_err_e err = func_polling (self, ACQ_NAME_CHECK_DATA_ACQUIRE,
            service, NULL, NULL, timeout);

    ASSERT_TEST(err == HALCS_CLIENT_SUCCESS,
            "Data acquisition was not completed",
            err_check_data_acquire, HALCS_CLIENT_ERR_SERVER);

    /* If we are here, then the acquisition was successfully completed*/
    DBE_DEBUG (DBG_LIB_CLIENT | DBG_LVL_TRACE, "[libacqclient] "
            "Data acquisition was successfully completed\n");

    err = _acq_get_curve (self, service, acq_trans);
    ASSERT_TEST(err == HALCS_CLIENT_SUCCESS, "Could not get requested curve",
            err_get_curve, HALCS_CLIENT_ERR_SERVER);

err_get_curve:
err_check_data_acquire:
    return err;
}

/* Wrapper to be compatible with old function behavior */
static halcs_client_err_e _acq_full_compat (halcs_client_t *self, char *service,
        acq_trans_t *acq_trans, int timeout, bool new_acq)
{
    if (new_acq) {
        return _acq_full (self, service, acq_trans, timeout);
    }
    else {
        return _acq_get_curve (self, service, acq_trans);
    }
}
