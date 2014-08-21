/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include "sm_io.h"
#include "rw_param.h"
#include "hal_assert.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, SM_IO, "[sm_io:rw_param]", \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_ALLOC(ptr, SM_IO, "[sm_io:rw_param]", 		\
            smio_err_str(SMIO_ERR_ALLOC),                   \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                            \
    CHECK_HAL_ERR(err, SM_IO, "[sm_io:rw_param]",    		\
            smio_err_str (err_type))

static zmsg_t * _format_client_response (RW_REPLY_TYPE reply_code, uint32_t reply_size,
        uint32_t *data_out, bool with_data_frame);

uint32_t check_param_limits (uint32_t value, uint32_t min, uint32_t max) {
	if (value < min || value > max) {
		return PARAM_ERR;
	}
	return PARAM_OK;
}

RW_REPLY_TYPE format_reply_code (int reply_code)
{
    return (reply_code < 0) ? -reply_code : reply_code;
}

void send_client_response_mdp (RW_REPLY_TYPE reply_code, uint32_t reply_size,
        uint32_t *data_out, bool with_data_frame, mdp_worker_t *worker,
        zframe_t *reply_to)
{
    zmsg_t *msg = _format_client_response (reply_code, reply_size, data_out,
            with_data_frame);
    ASSERT_TEST(msg != NULL, "Could format client message",
            err_fmt_client_message);

    mdp_worker_send (worker, &msg, reply_to);
err_fmt_client_message:
    return;
}

void send_client_response_sock (RW_REPLY_TYPE reply_code, uint32_t reply_size,
        uint32_t *data_out, bool with_data_frame, zframe_t *reply_to)
{
    zmsg_t *msg = _format_client_response (reply_code, reply_size, data_out,
            with_data_frame);
    ASSERT_TEST(msg != NULL, "Could format client message",
            err_fmt_client_message);

    zmsg_send (&msg, reply_to);
err_fmt_client_message:
    return;
}

static zmsg_t * _format_client_response (RW_REPLY_TYPE reply_code, uint32_t reply_size,
        uint32_t *data_out, bool with_data_frame)
{
    /* Send reply back to client */
    zmsg_t *report = zmsg_new ();
    ASSERT_ALLOC(report, err_send_msg_alloc);

    /* Message is:
     * frame 0: error code
     * frame 1: size (in bytes) or return code
     * frame 2: data
     * */
    int zerr = zmsg_addmem (report, &reply_code, sizeof(reply_code));
    ASSERT_TEST(zerr==0, "Could not add reply code in message", err_reply_code);

    if (with_data_frame) {
        zerr = zmsg_addmem (report, &reply_size, sizeof(reply_size));
        ASSERT_TEST(zerr==0, "Could not add reply size or return code in message",
                err_size_ret);
        zerr = zmsg_addmem (report, data_out, reply_size);
        ASSERT_TEST(zerr==0, "Could not add reply data in message",
                err_data);
    }

    DBE_DEBUG (DBG_MSG | DBG_LVL_TRACE, "[sm_io:rw_param] send_client_response: "
            "Sending message:\n");
#ifdef LOCAL_MSG_DBG
    debug_log_print_zmq_msg (report);
#endif
    return report;

err_data:
err_size_ret:
err_reply_code:
err_send_msg_alloc:
    zmsg_destroy (&report);
    return NULL;
}
