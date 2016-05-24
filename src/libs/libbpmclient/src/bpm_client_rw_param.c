/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "bpm_client.h"
/* Private headers */
#include "errhand.h"
#include "bpm_client_rw_param_codes.h"
#include "bpm_client_revision.h"

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

bpm_client_err_e param_client_send_gen_rw (bpm_client_t *self, char *service,
        uint32_t operation, uint32_t rw, void *param1, size_t size1,
        void *param2, size_t size2)
{
    bpm_client_err_e err = BPM_CLIENT_SUCCESS;

    ASSERT_TEST(param1 != NULL, "param_client_send_gen_rw (): parameter cannot be NULL",
            err_param1_null, BPM_CLIENT_ERR_INV_PARAM);

    mlm_client_t *client = bpm_get_mlm_client (self);
    ASSERT_TEST(client != NULL, "Could not get BPM client handler", err_get_handler,
            BPM_CLIENT_ERR_SERVER);

    zmsg_t *request = zmsg_new ();
    ASSERT_ALLOC(request, err_send_msg_alloc, BPM_CLIENT_ERR_ALLOC);
    zmsg_addmem (request, &operation, sizeof (operation));
    zmsg_addmem (request, &rw, sizeof (rw));
    zmsg_addmem (request, param1, size1);
    if (param2 != NULL) {
        zmsg_addmem (request, param2, size2);
    }

    /* Get poller and timeout from client */
    uint32_t timeout = bpm_client_get_timeout (self);

    int rc = mlm_client_sendto (client, service, NULL, NULL, timeout, &request);
    ASSERT_TEST(rc >= 0, "Could not send message", err_get_handler,
            BPM_CLIENT_ERR_SERVER);

err_send_msg_alloc:
err_get_handler:
err_param1_null:
    return err;
}

bpm_client_err_e param_client_recv_rw (bpm_client_t *self, char *service,
        zmsg_t **report)
{
    (void) service;
    assert (report);

    bpm_client_err_e err = BPM_CLIENT_SUCCESS;

    /* Receive report */
    mlm_client_t *client = bpm_get_mlm_client (self);
    ASSERT_TEST(client != NULL, "Could not get BPM client handler", err_get_handler,
            BPM_CLIENT_ERR_SERVER);
    *report = param_client_recv_timeout (self);
    ASSERT_TEST(*report != NULL, "Could not receive message", err_null_msg,
            BPM_CLIENT_ERR_SERVER);

err_null_msg:
err_get_handler:
    return err;
}

bpm_client_err_e param_client_write_gen (bpm_client_t *self, char *service,
        uint32_t operation, uint32_t rw, void *param1, size_t size1,
        void *param2, size_t size2)
{
    assert (self);
    assert (service);

    bpm_client_err_e err = BPM_CLIENT_SUCCESS;
    zmsg_t *report;

    err = param_client_send_gen_rw (self, service, operation, rw, param1,
            size1, param2, size2);
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

bpm_client_err_e param_client_write_raw (bpm_client_t *self, char *service,
        uint32_t operation, uint32_t param1, uint32_t param2)
{
    return param_client_write_gen (self, service, operation, param1, &param2,
            sizeof (param2), NULL, 0);
}

bpm_client_err_e param_client_write (bpm_client_t *self, char *service,
        uint32_t operation, uint32_t param)
{
    uint32_t rw = WRITE_MODE;
    return param_client_write_gen (self, service, operation, rw, &param,
            sizeof (param), NULL, 0);
}

bpm_client_err_e param_client_write_double (bpm_client_t *self, char *service,
        uint32_t operation, double param)
{
    uint32_t rw = WRITE_MODE;
    return param_client_write_gen (self, service, operation, rw, &param,
            sizeof (param), NULL, 0);
}

bpm_client_err_e param_client_write2 (bpm_client_t *self, char *service,
        uint32_t operation, uint32_t param1, uint32_t param2)
{
    uint32_t rw = WRITE_MODE;
    return param_client_write_gen (self, service, operation, rw, &param1,
            sizeof (param1), &param2, sizeof (param2));
}

bpm_client_err_e param_client_write_double2 (bpm_client_t *self, char *service,
        uint32_t operation, double param1, double param2)
{
    uint32_t rw = WRITE_MODE;
    return param_client_write_gen (self, service, operation, rw, &param1,
            sizeof (param1), &param2, sizeof (param2));
}

bpm_client_err_e param_client_read_gen (bpm_client_t *self, char *service,
        uint32_t operation, uint32_t rw, void *param1, size_t size1,
        void *param2, size_t size2, void *param_out, size_t size_out)
{
    assert (self);
    assert (service);

    bpm_client_err_e err = BPM_CLIENT_SUCCESS;
    zmsg_t *report;

    /* Even though we don't use the second parameter, we have the same
     * message strucuture and the server will check for strict consistency
     * (number of arguments and size) of all parameters. So, use the size of
     * the passed parameter here */
    err = param_client_send_gen_rw (self, service, operation, rw,
            param1, size1,
            param2, size2);
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

        /* We accept any payload that is less than the specified size */
        ASSERT_TEST(zframe_size (data_out_frm) <= size_out,
                "Wrong <payload> parameter size", err_msg_fmt);

        /* Copy the message contents to the user */
        memcpy (param_out, zframe_data (data_out_frm), size_out);
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

bpm_client_err_e param_client_read (bpm_client_t *self, char *service,
        uint32_t operation, uint32_t *param_out)
{
    uint32_t rw = READ_MODE;
    return param_client_read_gen (self, service, operation, rw, param_out,
            sizeof (*param_out), NULL, 0, param_out, sizeof (*param_out));
}

bpm_client_err_e param_client_read_double (bpm_client_t *self, char *service,
        uint32_t operation, double *param_out)
{
    uint32_t rw = READ_MODE;
    return param_client_read_gen (self, service, operation, rw, param_out,
            sizeof (*param_out), NULL, 0, param_out, sizeof (*param_out));
}

bpm_client_err_e param_client_write_read (bpm_client_t *self, char *service,
        uint32_t operation, uint32_t param1, uint32_t *param_out)
{
    uint32_t rw = READ_MODE;
    return param_client_read_gen (self, service, operation, rw, &param1,
            sizeof (param1), &param1, sizeof (param1), param_out, sizeof (*param_out));
}

bpm_client_err_e param_client_write_read_double (bpm_client_t *self, char *service,
        uint32_t operation, double param1, double *param_out)
{
    uint32_t rw = READ_MODE;
    return param_client_read_gen (self, service, operation, rw, &param1,
            sizeof (param1), &param1, sizeof (param1), param_out, sizeof (*param_out));
}

/********************* Utility functions ************************************/
/* Wait for message to arrive up to timeout msecs */
zmsg_t *param_client_recv_timeout (bpm_client_t *self)
{
    zmsg_t *msg = NULL;

    /* Get poller and timeout from client */
    uint32_t timeout = bpm_client_get_timeout (self);
    zpoller_t *poller = bpm_client_get_poller (self);

    /* Get MLM socket for use with poller */
    zsock_t *msgpipe = mlm_client_msgpipe (bpm_get_mlm_client (self));
    ASSERT_TEST (msgpipe != NULL, "Invalid MLM client socket reference",
            err_mlm_inv_client_socket);

    zsock_t *which = zpoller_wait (poller, timeout);
    /* Check if poller expired */
    ASSERT_TEST(!zpoller_expired (poller),
            "Server took too long too respond", err_poller_timeout);
    /* If not we should have a valid message */
    ASSERT_TEST(!zpoller_terminated (poller),
            "Poller terminated", err_poller_terminated);
    ASSERT_TEST(which != NULL, "Could not poll on sockets",
            err_poller_invalid);

    /* Check for activity socket */
    if (which == msgpipe) {
        msg = mlm_client_recv (bpm_get_mlm_client (self));
    }

err_poller_invalid:
err_poller_terminated:
err_poller_timeout:
err_mlm_inv_client_socket:
    return msg;
}
