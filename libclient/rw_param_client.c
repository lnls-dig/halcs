/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include "hal_assert.h"

#include "bpm_client.h"
#include "rw_param_client.h"
#include "rw_param_codes.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, LIB_CLIENT, "[libclient:rw_param_client]", \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)       \
    ASSERT_HAL_ALLOC(ptr, LIB_CLIENT, "[libclient:rw_param_client]", \
            bpm_client_err_str(BPM_CLIENT_ERR_ALLOC),               \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                    \
    CHECK_HAL_ERR(err, LIB_CLIENT, "[libclient:rw_param_client]",   \
            bpm_client_err_str (err_type))

bpm_client_err_e param_client_send_rw (bpm_client_t *self, char *service,
        uint32_t operation, uint32_t rw, uint32_t param)
{
    bpm_client_err_e err = BPM_CLIENT_SUCCESS;

    zmsg_t *request = zmsg_new ();
    ASSERT_ALLOC(request, err_send_msg_alloc, BPM_CLIENT_ERR_ALLOC);
    zmsg_addmem (request, &operation, sizeof (operation));
    zmsg_addmem (request, &rw, sizeof (rw));
    zmsg_addmem (request, &param, sizeof (param));

    mdp_client_send (self->mdp_client, service, &request);

err_send_msg_alloc:
    return err;
}

bpm_client_err_e param_client_recv_rw (bpm_client_t *self, char *service,
        zmsg_t **report)
{
    (void) service;
    assert (report);

    bpm_client_err_e err = BPM_CLIENT_SUCCESS;

    /* Receive report */
    *report = mdp_client_recv (self->mdp_client, NULL, NULL);
    ASSERT_TEST(*report != NULL, "Could not receive message", err_null_msg,
            BPM_CLIENT_ERR_SERVER);

err_null_msg:
    return err;
}

bpm_client_err_e param_client_write_gen (bpm_client_t *self, char *service,
        uint32_t operation, uint32_t param1, uint32_t param2)
{
    assert (self);
    assert (service);

    bpm_client_err_e err = BPM_CLIENT_SUCCESS;
    zmsg_t *report;

    err = param_client_send_rw (self, service, operation, param1, param2);
    ASSERT_TEST(err == BPM_CLIENT_SUCCESS, "Could not send message", err_send_msg);
    err = param_client_recv_rw (self, service, &report);
    ASSERT_TEST(err == BPM_CLIENT_SUCCESS, "Could not receive message", err_recv_msg);

    /* Message is:
     * frame 0: error code      */

    /* Handling malformed messages */
    size_t msg_size = zmsg_size (report);
    ASSERT_TEST(msg_size == MSG_ERR_CODE_SIZE, "Unexpected message received", err_msg);

    /* Get message contents */
    zframe_t *err_code = zmsg_pop (report);
    ASSERT_TEST(err_code != NULL, "Could not receive error code", err_null_code,
            BPM_CLIENT_ERR_SERVER);
    /* Check for return code from server */
    ASSERT_TEST(*(RW_REPLY_TYPE *) zframe_data(err_code) == RW_OK,
            "rw_param_client: parameter SET error, try again",
            err_set_param, BPM_CLIENT_ERR_AGAIN);

err_set_param:
    zframe_destroy (&err_code);
err_null_code:
err_msg:
    zmsg_destroy (&report);
err_recv_msg:
err_send_msg:
    return err;
}

bpm_client_err_e param_client_write (bpm_client_t *self, char *service,
        uint32_t operation, uint32_t param)
{
    uint32_t rw = WRITE_MODE;
    return param_client_write_gen (self, service,operation, rw, param);
}

/* TODO: improve error handling */
bpm_client_err_e param_client_read (bpm_client_t *self, char *service,
        uint32_t operation, uint32_t *param_out)
{
    assert (self);
    assert (service);

    bpm_client_err_e err = BPM_CLIENT_SUCCESS;
    uint32_t param1 = READ_MODE;
    zmsg_t *report;

    err = param_client_send_rw (self, service, operation, param1,
            0 /* in read mode this value will be ignored */);
    ASSERT_TEST(err == BPM_CLIENT_SUCCESS, "Could not send message", err_send_msg);
    err = param_client_recv_rw (self, service, &report);
    ASSERT_TEST(err == BPM_CLIENT_SUCCESS, "Could not receive message", err_recv_msg);

    /* Message is:
     * frame 0: error code
     * frame 1: number of bytes read (optional)
     * frame 2: data read (optional) */

    /* Handling malformed messages */
    size_t msg_size = zmsg_size (report);
    ASSERT_TEST(msg_size == MSG_ERR_CODE_SIZE || msg_size == MSG_FULL_SIZE,
            "Unexpected message received", err_msg);

    /* Get message contents */
    zframe_t *err_code = zmsg_pop(report);
    ASSERT_TEST(err_code != NULL, "Could not receive error code", err_null_code);

    /* Check for return code from server */
    ASSERT_TEST(*(RW_REPLY_TYPE *) zframe_data(err_code) == RW_OK,
            "rw_param_client: parameter GET error, try again",
            err_error_code, BPM_CLIENT_ERR_AGAIN);

    zframe_t *bytes_read_frm = NULL;
    zframe_t *data_out_frm = NULL;
    if (msg_size == MSG_FULL_SIZE) {
        bytes_read_frm = zmsg_pop(report);
        ASSERT_TEST(bytes_read_frm != NULL, "Could not receive number of bytes read", err_null_bytes_read);
        data_out_frm = zmsg_pop(report);
        ASSERT_TEST(data_out_frm != NULL, "Could not receive parameter", err_null_param);

        ASSERT_TEST(zframe_size (bytes_read_frm) == RW_REPLY_SIZE,
                "Wrong <number of payload bytes> parameter size", err_msg_fmt);

        /* Size in the second frame must match the frame size of the third */
        ASSERT_TEST(*(RW_REPLY_TYPE *) zframe_data (bytes_read_frm) == zframe_size (data_out_frm),
                "<payload> parameter size does not match size in <number of payload bytes> parameter",
                err_msg_fmt);

        /* We only accept one RW_REPLY_SIZE bytes of payload for now */
        ASSERT_TEST(zframe_size (data_out_frm) == RW_REPLY_SIZE,
                "Wrong <payload> parameter size", err_msg_fmt);

        /* Copy the message contents to the user */
        *param_out = *(uint32_t *) zframe_data (data_out_frm);
    }

err_msg_fmt:
    zframe_destroy (&data_out_frm);
err_null_param:
    zframe_destroy (&bytes_read_frm);
err_null_bytes_read:
err_error_code:
    zframe_destroy (&err_code);
err_null_code:
err_msg:
    zmsg_destroy (&report);
err_recv_msg:
err_send_msg:
    return err;
}

