/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include "bpm_client.h"
#include "hal_assert.h"
#include "ddr3_map.h"

#include "sm_io_acq_codes.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label)  \
    ASSERT_HAL_TEST(test_boolean, LIB_CLIENT, "[libclient]",\
            err_str, err_goto_label)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label)                   \
    ASSERT_HAL_ALLOC(ptr, LIB_CLIENT, "[libclient]",        \
            bpm_client_err_str(BPM_CLIENT_ERR_ALLOC),       \
            err_goto_label)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                            \
    CHECK_HAL_ERR(err, LIB_CLIENT, "[libclient]",           \
            bpm_client_err_str (err_type))

/* Our structure */
struct _bpm_client_t {
    mdp_client_t *mdp_client;           /* Majordomo client instance */
    const acq_buf_t *acq_buf;           /* Acquisition buffer table */
};

/********************************************************/
/************************ Our API ***********************/
/********************************************************/

bpm_client_t *bpm_client_new (char *broker_endp, int verbose)
{
    bpm_client_t *self = zmalloc (sizeof *self);
    ASSERT_ALLOC(self, err_self_alloc);
    self->mdp_client = mdp_client_new (broker_endp, verbose);
    ASSERT_TEST(self->mdp_client!=NULL, "Could not create MDP client",
            err_mdp_client);

    /* Initialize acquisition table. Defined in hal/smio/modules/
     * acq/ddr3_map.h */
    self->acq_buf = __acq_buf;

    return self;

err_mdp_client:
    free (self);
err_self_alloc:
    return NULL;
}

void bpm_client_destroy (bpm_client_t **self_p)
{
    assert (self_p);

    if (*self_p) {
        bpm_client_t *self = *self_p;

        self->acq_buf = NULL;
        mdp_client_destroy (&self->mdp_client);
        *self_p = NULL;
    }
}

/**************** FMC130M SMIO Functions ****************/
bpm_client_err_e bpm_blink_leds (bpm_client_t *self, char *service, uint32_t leds)
{
    assert (self);
    assert (service);

    bpm_client_err_e err = BPM_CLIENT_SUCCESS;
    FMC130M_4CH_REPLY_TYPE operation = FMC130M_4CH_OPCODE_LEDS;

    zmsg_t *request = zmsg_new ();
    err = (request == NULL) ? BPM_CLIENT_ERR_ALLOC : err;
    ASSERT_ALLOC(request, err_send_msg_alloc);
    zmsg_addmem (request, &operation, sizeof (operation));
    zmsg_addmem (request, &leds, sizeof (leds));
    mdp_client_send (self->mdp_client, service, &request);

err_send_msg_alloc:
    return err;
}

/****************** ACQ SMIO Functions ****************/
#define MIN_WAIT_TIME           1                           /* in ms */
#define MSECS                   1000                        /* in seconds */

static bpm_client_err_e _bpm_data_acquire (bpm_client_t *self, char *service,
        acq_req_t *acq_req);
static bpm_client_err_e _bpm_check_data_acquire (bpm_client_t *self, char *service);
static bpm_client_err_e _bpm_wait_data_acquire_timed (bpm_client_t *self, char *service,
        int timeout);
static bpm_client_err_e _bpm_get_data_block (bpm_client_t *self, char *service,
        acq_trans_t *acq_trans);

bpm_client_err_e bpm_data_acquire (bpm_client_t *self, char *service, acq_req_t *acq_req)
{
    return _bpm_data_acquire (self, service, acq_req);
}

bpm_client_err_e bpm_check_data_acquire (bpm_client_t *self, char *service)
{
    return _bpm_check_data_acquire (self, service);
}

bpm_client_err_e bpm_wait_data_acquire_timed (bpm_client_t *self, char *service,
        int timeout)
{
    return _bpm_wait_data_acquire_timed (self, service, timeout);
}

bpm_client_err_e bpm_get_data_block (bpm_client_t *self, char *service,
        acq_trans_t *acq_trans)
{
    return _bpm_get_data_block (self, service, acq_trans);
}

static bpm_client_err_e _bpm_data_acquire (bpm_client_t *self, char *service,
        acq_req_t *acq_req)
{
    assert (self);
    assert (service);
    assert (acq_req);

    bpm_client_err_e err = BPM_CLIENT_SUCCESS;
    ACQ_OPCODE_TYPE operation = ACQ_OPCODE_DATA_ACQUIRE;

    /* Message is:
     * frame 0: operation code
     * frame 1: number of samples
     * frame 2: channel             */
    zmsg_t *request = zmsg_new ();
    zmsg_addmem (request, &operation, sizeof (operation));
    zmsg_addmem (request, &acq_req->num_samples, sizeof (acq_req->num_samples));
    zmsg_addmem (request, &acq_req->chan, sizeof (acq_req->chan));
    mdp_client_send (self->mdp_client, service, &request);

    /* Receive report */
    zmsg_t *report = mdp_client_recv (self->mdp_client, NULL, NULL);
    ASSERT_TEST(report != NULL, "Report received is NULL", err_null_report);
    assert (zmsg_size (report) == 1);

    /* Message is:
     * frame 0: error code      */
    zframe_t *err_code = zmsg_pop(report);
    ASSERT_TEST(err_code != NULL, "Could not receive error code", err_null_code);
    if ( *(ACQ_REPLY_TYPE *) zframe_data(err_code) != ACQ_OK) {
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[libclient] bpm_data_acquire: "
                "Data acquire was not required correctly\n");
        err = BPM_CLIENT_ERR_AGAIN;
        goto err_data_acquire;
    }
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[libclient] bpm_data_acquire: "
            "Data acquire was successfully required\n");

err_data_acquire:
    zframe_destroy (&err_code);
err_null_code:
    zmsg_destroy (&report);
err_null_report:
    return err;
}

static bpm_client_err_e _bpm_check_data_acquire (bpm_client_t *self, char *service)
{
    assert (self);
    assert (service);

    int err = BPM_CLIENT_SUCCESS;
    ACQ_OPCODE_TYPE operation = ACQ_OPCODE_DATA_ACQUIRE;

    /* Message is:
     * frame 0: operation code      */
    zmsg_t *request = zmsg_new ();
    zmsg_addmem (request, &operation, sizeof (operation));
    mdp_client_send (self->mdp_client, service, &request);

    /* Receive report */
    zmsg_t *report = mdp_client_recv (self->mdp_client, NULL, NULL);
    ASSERT_TEST(report != NULL, "Report received is NULL", err_null_report);
    assert (zmsg_size (report) == 1);

    /* Message is:
     * frame 0: error code          */
    zframe_t *err_code = zmsg_pop(report);
    ASSERT_TEST(err_code != NULL, "Could not receive error code", err_null_code);
    if ( *(ACQ_REPLY_TYPE *) zframe_data(err_code) != ACQ_OK) {
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[libclient] bpm_check_data_acquire: "
            "Check fail: data acquire was not completed\n");
        err = BPM_CLIENT_ERR_SERVER;
        goto err_check_data_acquire;
    }
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[libclient] bpm_check_data_acquire: "
            "Check ok: data acquire was successfully completed\n");

err_check_data_acquire:
    zframe_destroy (&err_code);
err_null_code:
    zmsg_destroy (&report);
err_null_report:
    return err;
}

static bpm_client_err_e _bpm_wait_data_acquire_timed (bpm_client_t *self, char *service,
        int timeout)
{
    assert (self);
    assert (service);

    /* timeout < 0 means "infinite" wait */
    if (timeout < 0) {
        /* FIXME! Very unportable way! We assume that time_t is at least of
         * size INT */
        timeout = INT_MAX;
    }

    bpm_client_err_e err = BPM_CLIENT_SUCCESS;
    time_t start = time (NULL);
    while (time(NULL) - start < timeout) {
        if (!zctx_interrupted) {
            if ((err = _bpm_check_data_acquire (self, service)) == BPM_CLIENT_SUCCESS) {
                DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[libclient] "
                        "bpm_wait_data_acquire_timed: finished waiting\n");
                goto exit;
            }

            usleep (MSECS*timeout);
        }
    }

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[libclient] "
            "bpm_wait_data_acquire_timed: number of tries was exceeded\n");
    /* timeout occured */
    err = BPM_CLIENT_ERR_TIMEOUT;

exit:
    return err;
}

static bpm_client_err_e _bpm_get_data_block (bpm_client_t *self, char *service,
        acq_trans_t *acq_trans)
{
    assert (self);
    assert (service);
    assert (acq_trans);
    assert (acq_trans->block.data);

    bpm_client_err_e err = BPM_CLIENT_SUCCESS;
    ACQ_OPCODE_TYPE operation = ACQ_OPCODE_CHECK_DATA_ACQUIRE;

    /* Message is:
     * frame 0: operation code
     * frame 1: channel
     * frame 2: block required          */
    zmsg_t *request = zmsg_new ();
    zmsg_addmem (request, &operation, sizeof (operation));
    zmsg_addmem (request, &acq_trans->req.chan, sizeof (acq_trans->req.chan));
    zmsg_addmem (request, &acq_trans->block.idx, sizeof (acq_trans->block.idx));
    mdp_client_send (self->mdp_client, service, &request);

    /* Receive report */
    zmsg_t *report = mdp_client_recv (self->mdp_client, NULL, NULL);
    ASSERT_TEST(report != NULL, "Report received is NULL", err_null_report);
    assert (zmsg_size (report) == 3);

    /* Message is:
     * frame 0: error code
     * frame 1: data size
     * frame 2: data block              */
    zframe_t *err_code = zmsg_pop (report);
    ASSERT_TEST(err_code != NULL, "Could not receive error code", err_null_code);
    zframe_t *data_size_frm = zmsg_pop (report);
    ASSERT_TEST(data_size_frm != NULL, "Could not receive data size", err_null_data_size);
    uint32_t data_size = *(uint32_t *) zframe_data(data_size_frm);
    zframe_t *data = zmsg_pop (report);
    ASSERT_TEST(data != NULL, "Could not receive data", err_null_data);

    if ( *(ACQ_REPLY_TYPE *) zframe_data (err_code) != ACQ_OK) {
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[libclient] bpm_get_data_block: "
            "Data block was not acquired\n");
        err = BPM_CLIENT_ERR_SERVER;
        goto err_get_data_block;
    }

    /* Data size effectively returned */
    uint32_t read_size = (acq_trans->block.data_size < data_size) ?
        acq_trans->block.data_size : data_size;
    memcpy (acq_trans->block.data, (uint32_t *) zframe_data (data), read_size);
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[libclient] bpm_get_data_block: "
        "Data block of %u bytes was successfully acquired\n", read_size);
    acq_trans->block.bytes_read = read_size;

err_get_data_block:
    zframe_destroy (&data);
err_null_data:
    zframe_destroy (&data_size_frm);
err_null_data_size:
    zframe_destroy (&err_code);
err_null_code:
    zmsg_destroy (&report);
err_null_report:
    return err;
}

bpm_client_err_e bpm_get_curve (bpm_client_t *self, char *service,
        acq_trans_t *acq_trans)
{
    assert (self);
    assert (service);
    assert (acq_trans);
    assert (acq_trans->block.data);

    /* Client requisition: data acquire */
    bpm_client_err_e err = _bpm_data_acquire (self, service, &acq_trans->req);
    ASSERT_TEST(err == BPM_CLIENT_SUCCESS, "Could not request acqusition\n",
            err_bpm_data_acquire);

    /* Client requisition: wait data acquire indefinetly */
    err = _bpm_wait_data_acquire_timed (self, service, -1);
    ASSERT_TEST(err == BPM_CLIENT_SUCCESS, "Request acquisition timed out\n",
            err_bpm_wait_data_acquire);

    /* FIXME: When the last block is full 'block_n_valid exceeds by one */
    uint32_t block_n_valid = acq_trans->req.num_samples /
        (BLOCK_SIZE/self->acq_buf[acq_trans->req.chan].sample_size);
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[libclient] bpm_get_curve: "
            "block_n_valid = %u\n", block_n_valid);

    int bytes_read = 0;
    /* Client requisition: get data block */
    for (uint32_t block_n = 0; block_n <= block_n_valid; block_n++) {
        if (!zctx_interrupted) {
            acq_trans->block.idx = block_n;
            err = _bpm_get_data_block (self, service, /*chan, block_n,
                    (uint32_t *)((uint8_t *)data_out+ret), size-ret*/
                    acq_trans);
            if (err != BPM_CLIENT_SUCCESS){
                DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[libclient] bpm_get_curve: "
                        "_bpm_get_data_block failed. block_n: %u\n", block_n);
                goto err_bpm_get_data_block;
            }

            bytes_read += acq_trans->block.bytes_read;
            acq_trans->block.data = (uint32_t *)((uint8_t *)acq_trans->block.data + bytes_read);
            acq_trans->block.data_size -= bytes_read;
        }
    }

    /* return to client the total number of bytes read */
    acq_trans->block.bytes_read = bytes_read;

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[libclient] bpm_get_curve: "
        "Data curve of %u bytes was successfully acquired\n", bytes_read);

err_bpm_get_data_block:
err_bpm_wait_data_acquire:
err_bpm_data_acquire:
    return err;
}
