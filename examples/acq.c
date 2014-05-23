/*
 *  * Simple example demonstrating the communication between
 *   * a client and the FPGA device
 *    */

#include <mdp.h>
#include <czmq.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#include "hal_assert.h"
#include "debug_print.h"
#include "ddr3_map.h"
#include "sm_io_acq_codes.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label)  \
    ASSERT_HAL_TEST(test_boolean, SM_IO, "[client]",        \
            err_str, err_goto_label)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label)                   \
    ASSERT_HAL_ALLOC(ptr, SM_IO, "[client]",                \
            smio_err_str(SMIO_ERR_ALLOC),                   \
            err_goto_label)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                            \
    CHECK_HAL_ERR(err, SM_IO, "[client]",                   \
            smio_err_str (err_type))


#define DFLT_BIND_FOLDER            "/tmp/bpm"
#define DFLT_BIND_ADDR              "0"

/* FMC103M_4CH operations op code */
#define LEDS                        0

/* ACQ operations op code */
#define DATA_ACQUIRE                0
#define GET_DATA_BLOCK              1
#define CHECK_DATA_ACQUIRE          2

#define WAIT_TIME 100                           /* in ms */
#define DATA_ACQ_TIMEOUT 200000                 /* in ms = 20 sec */

/* Our structure */
typedef struct _bpm_client_t {
    mdp_client_t *mdp_client;
} bpm_client_t;

/* Our API */
bpm_client_t *bpm_client_new (char *broker_endp, int verbose)
{
    bpm_client_t *self = zmalloc (sizeof *self);
    self->mdp_client = mdp_client_new (broker_endp, verbose);

    return self;
}

void bpm_client_destroy (bpm_client_t **self_p)
{
    assert (self_p);

    if (*self_p) {
        bpm_client_t *self = *self_p;
        mdp_client_destroy (&self->mdp_client);
        *self_p = NULL;
    }
}

void print_data (uint32_t chan, uint32_t *data, uint32_t size)
{
    /* FIXME: Make it more generic */
    if (chan == 0) {
        int16_t *raw_data16 = (int16_t *) data;
        for (uint32_t i = 0; i < (size/sizeof(uint16_t)) / 4; i++) {
            if (!zctx_interrupted) {
                printf("%6u\t %8d\t %8d\t %8d\t %8d\n", i,
                        raw_data16[(i*4)],
                        raw_data16[(i*4)+1],
                        raw_data16[(i*4)+2],
                        raw_data16[(i*4)+3]);
            }
        }
    }
    else {
        int32_t *raw_data32 = (int32_t *) data;
        for (uint32_t i = 0; i < (size/sizeof(uint32_t)) / 4; i++) {
            if (!zctx_interrupted) {
                printf("%6u\t %8d\t %8d\t %8d\t %8d\n", i,
                    raw_data32[(i*4)],
                    raw_data32[(i*4)+1],
                    raw_data32[(i*4)+2],
                    raw_data32[(i*4)+3]);
            }
        }
    }
}

static int _bpm_data_acquire (bpm_client_t *self, char *service, uint32_t num_samples,
                      uint32_t chan)
{
    int err = 0;    /*TODO MACRO error codes: 1 ok, -1 error (for example) */
    uint32_t operation = DATA_ACQUIRE;

    /* Message is:
     * frame 0: operation code
     * frame 1: number of samples
     * frame 2: channel             */
    zmsg_t *request = zmsg_new ();
    zmsg_addmem (request, &operation, sizeof (operation));
    zmsg_addmem (request, &num_samples, sizeof (num_samples));
    zmsg_addmem (request, &chan, sizeof (chan));
    mdp_client_send (self->mdp_client, service, &request);

    /* Receive report */
    zmsg_t *report = mdp_client_recv (self->mdp_client, NULL, NULL);
    ASSERT_TEST(report != NULL, "Report received is NULL", err_null_report);
    assert (zmsg_size (report) == 1);

    /* Message is:
     * frame 0: error code      */
    zframe_t *err_code = zmsg_pop(report);
    ASSERT_TEST(err_code != NULL, "Could not receive error code", err_null_code);
    if ( *(uint32_t *) zframe_data(err_code) != ACQ_REQ_OK) {
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[client] bpm_data_acquire: "
                "Data acquire was not required correctly\n");
        err = -1;
        goto err_data_acquire;
    }
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[client] bpm_data_acquire: "
            "Data acquire was successfully required\n");

err_data_acquire:
    zframe_destroy (&err_code);
err_null_code:
    zmsg_destroy (&report);
err_null_report:
    return err;
}

static int _bpm_check_data_acquire (bpm_client_t *self, char *service)
{
    int err = 0;
    uint32_t operation = CHECK_DATA_ACQUIRE;

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
    if ( *(uint32_t *) zframe_data(err_code) != ACQ_CHECK_OK) {
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[client] bpm_check_data_acquire: "
            "Check fail: data acquire was not completed\n");
        err = -1;
        goto err_check_data_acquire;
    }
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[client] bpm_check_data_acquire: "
            "Check ok: data acquire was successfully completed\n");

err_check_data_acquire:
    zframe_destroy (&err_code);
err_null_code:
    zmsg_destroy (&report);
err_null_report:
    return err;
}

static int _bpm_wait_data_acquire_timed (bpm_client_t *self, char *service,
        unsigned int msec)
{
    int err = 0;
    if (msec < WAIT_TIME) {
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[client] "
                "bpm_wait_data_acquire_timed: 'msec' must be at least %d\n"
                , WAIT_TIME);
        err = -1;
        goto err_wait_data;
    }

    int max_tries = msec / WAIT_TIME;
    int tries;
    for (tries = 0; tries < max_tries; tries++) {
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[client] "
                "bpm_wait_data_acquire_timed: waiting... #%d\n", tries);
        if (_bpm_check_data_acquire(self, service) == 0) {
            break;
        }
        usleep (1000*WAIT_TIME);
    }

    if (tries == max_tries) {
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[client] "
                "bpm_wait_data_acquire_timed: number of tries was exceeded\n");
		err = -1;
        goto err_wait_data;
    }

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[client] "
            "bpm_wait_data_acquire_timed: the wait is over successfully\n");

err_wait_data:
    return err;
}

static int _bpm_get_data_block (bpm_client_t *self, char *service, uint32_t chan,
                        uint32_t block_n, uint32_t *data_out, uint32_t size)
{
    int err = 0;
    uint32_t operation = GET_DATA_BLOCK;

    /* Message is:
     * frame 0: operation code
     * frame 1: channel
     * frame 2: block required          */
    zmsg_t *request = zmsg_new ();
    zmsg_addmem (request, &operation, sizeof (operation));
    zmsg_addmem (request, &chan, sizeof (chan));
    zmsg_addmem (request, &block_n, sizeof (block_n));
    mdp_client_send (self->mdp_client, service, &request);

    /* Receive report */
    zmsg_t *report = mdp_client_recv (self->mdp_client, NULL, NULL);
    ASSERT_TEST(report != NULL, "Report received is NULL", err_null_report);
    assert (zmsg_size (report) == 3);

    /* Message is:
     * frame 0: error code
     * frame 1: data size
     * frame 2: data block              */
    zframe_t *err_code = zmsg_pop(report);
    ASSERT_TEST(err_code != NULL, "Could not receive error code", err_null_code);
    zframe_t *data_size_frm = zmsg_pop(report);
    ASSERT_TEST(data_size_frm != NULL, "Could not receive data size", err_null_data_size);
    uint32_t data_size = *(uint32_t *) zframe_data(data_size_frm);
    zframe_t *data = zmsg_pop(report);
    ASSERT_TEST(data != NULL, "Could not receive data", err_null_data);

    if ( *(uint32_t *) zframe_data(err_code) != ACQ_BLOCK_OK) {
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[client] bpm_get_data_block: "
            "Data block was not acquired\n");
        err = -1;
        goto err_get_data_block;
    }

    /* Data size effectively returned */
    uint32_t read_size = (size < data_size) ? size : data_size;
    memcpy(data_out, (uint32_t *) zframe_data(data), read_size);
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[client] bpm_get_data_block: "
        "Data block of %u bytes was successfully acquired\n", read_size);
    err = read_size;
//    print_data (chan, (uint32_t *) zframe_data(data), data_size);

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

int bpm_data_acquire (bpm_client_t *self, char *service, uint32_t num_samples,
                      uint32_t chan)
{
    return _bpm_data_acquire (self, service, num_samples, chan);
}
int bpm_wait_data_acquire_timed (bpm_client_t *self, char *service,
        unsigned int msec)
{
    return _bpm_wait_data_acquire_timed (self, service, msec);
}
int bpm_check_data_acquire (bpm_client_t *self, char *service)
{
    return _bpm_check_data_acquire (self, service);
}

int bpm_get_data_block (bpm_client_t *self, char *service, uint32_t chan,
                        uint32_t block_n, uint32_t *data_out, uint32_t size)
{
    return _bpm_get_data_block (self, service, chan, block_n, data_out, size);
}

int bpm_get_curve (bpm_client_t *self, char *service, uint32_t num_samples,
                        uint32_t chan, uint32_t *data_out, uint32_t size)
{
    int ret = 0;

    /* Client requisition: data acquire */
    int err = _bpm_data_acquire (self, service, num_samples, chan);
    if (err == -1){
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[client] bpm_get_curve: "
            "_bpm_data_acquire failed\n");
        ret = -1;
        goto err_bpm_data_acquire;
    }

    /* Client requisition: wait data acquire */
    err = bpm_wait_data_acquire_timed (self, service, DATA_ACQ_TIMEOUT);
    if (err == -1){
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[client] bpm_get_curve: "
            "_bpm_wait_data_acquire failed\n");
        ret = -1;
        goto err_bpm_wait_data_acquire;
    }

    /* FIXME When the last block is full 'block_n_valid exceeds by one */
    uint32_t block_n_valid = num_samples /
                            (BLOCK_SIZE/__acq_buf[chan].sample_size);
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[client] bpm_get_curve: "
            "block_n_valid= %u\n", block_n_valid);

    int read_size = 0;
    /* Client requisition: get data block */
    for (uint32_t block_n = 0; block_n <= block_n_valid; block_n++) {
        if (!zctx_interrupted) {
            read_size = _bpm_get_data_block (self, service, chan, block_n,
                    (uint32_t *)((uint8_t *)data_out+ret), size-ret);
            if (read_size == -1){
                DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[client] bpm_get_curve: "
                        "_bpm_get_data_block failed. block_n: %u\n", block_n);
                goto err_bpm_get_data_block;
            }
            ret += read_size;   /* return read data size */
        }
    }
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[client] bpm_get_curve: "
        "Data curve of %u bytes was successfully acquired\n", size);

err_bpm_get_data_block:
err_bpm_wait_data_acquire:
err_bpm_data_acquire:
    return ret;
}

void print_help (char *program_name)
{
    printf( "Usage: %s [options]\n"
            "\t-h This help message\n"
            "\t-v Verbose output\n"
            "\t-b <broker_endpoint> Broker endpoint\n"
            "\t-s <num_samples_str> Number of samples\n"
            "\t-ch <chan_str> Acquisition channel\n"
            , program_name);
}

int main (int argc, char *argv [])
{
    int verbose = 0;
    char *broker_endp = NULL;
    char *num_samples_str = NULL;
    char *chan_str = NULL;
    char **str_p = NULL;

    if (argc < 2) {
        print_help (argv[0]);
        exit (1);
    }

    /* FIXME: This is rather buggy! */
    /* Simple handling of command-line options. This should be done
     *      * with getopt, for instance*/
    int i;
    for (i = 1; i < argc; i++)
    {
        if (streq(argv[i], "-v")) {
            verbose = 1;
        }
        else if (streq(argv[i], "-h"))
        {
            print_help (argv [0]);
            exit (1);
        }
        else if (streq (argv[i], "-b")) {
            str_p = &broker_endp;
        }
        else if (streq (argv[i], "-s")) { /* s: samples */
            str_p = &num_samples_str;
        }
        else if (streq (argv[i], "-ch")) { /* ch: channel */
            str_p = &chan_str;
        }
        /* Fallout for options with parameters */
        else {
            *str_p = strdup (argv[i]);
        }
    }

    /* Set default broker address */
    if (broker_endp == NULL) {
        broker_endp = strdup ("ipc://"DFLT_BIND_FOLDER"/"DFLT_BIND_ADDR);
    }

    bpm_client_t *bpm_client = bpm_client_new (broker_endp, verbose);

    /* Set default number samples */
    uint32_t num_samples;
    if (num_samples_str == NULL) {
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[client] acq: "
            "Set default value to 'num_samples'\n");
        num_samples = 4096;
    }
    else {
        num_samples = (uint32_t) atoi(num_samples_str);
    }
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[client] acq: "
        "num_samples = %u\n", num_samples);

    /* Set default channel */
    uint32_t chan;
    if (chan_str == NULL) {
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[client] acq: "
            "Set default value to 'chan'\n");
        chan = 0;
    }
    else {
        chan = (uint32_t) atoi(chan_str);
    }
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[client] acq: "
        "chan = %u\n", chan);

    uint32_t size_read = num_samples*__acq_buf[chan].sample_size;
    uint32_t *data = (uint32_t *) zmalloc (size_read*sizeof (uint8_t));
    int bytes_read = bpm_get_curve (bpm_client, "BPM0:DEVIO:ACQ", num_samples, chan,
                        data, size_read);
    if (bytes_read == -1){
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[client] acq: "
                "bpm_get_curve failed\n");
        goto err_bpm_get_curve;
    }
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[client] acq: "
            "bpm_get_curve was successfully executed\n");

    print_data (chan, data, bytes_read);

err_bpm_get_curve:
    str_p = &chan_str;
    free (*str_p);
    chan_str = NULL;
    str_p = &num_samples_str;
    free (*str_p);
    num_samples_str = NULL;
    str_p = &broker_endp;
    free (*str_p);
    broker_endp = NULL;
    bpm_client_destroy (&bpm_client);

return 0;
}
