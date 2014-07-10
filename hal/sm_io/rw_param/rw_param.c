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

uint32_t check_param_limits (uint32_t value, uint32_t min, uint32_t max) {
	if (value < min || value > max) {
		return PARAM_ERR;
	}
	return PARAM_OK;
}

void rw_param_send_client_response ( RW_REPLY_TYPE reply_code,
			uint32_t data_out, bool with_data_frame,
			mdp_worker_t *worker, zframe_t *reply_to) {

    /* Send reply back to client */
    zmsg_t *report = zmsg_new ();
    ASSERT_ALLOC(report, err_send_msg_alloc);

    /* Message is:
     * frame 0: error code          -> always sent
     * frame 2: data out            -> sent only in read modes		*/
    int zerr = zmsg_addmem (report, &reply_code, sizeof(reply_code));
    ASSERT_TEST(zerr==0, "Could not add reply code in message", err_reply_code);

    if (with_data_frame) {
        zerr = zmsg_addmem (report, &data_out, sizeof(data_out));
        ASSERT_TEST(zerr==0, "Could not add reply data in message",
                err_data_code);
    }

    DBE_DEBUG (DBG_MSG | DBG_LVL_TRACE, "[sm_io:rw_param] send_client_response: "
            "Sending message:\n");
#ifdef LOCAL_MSG_DBG
    debug_log_print_zmq_msg (report);
#endif
    mdp_worker_send (worker, &report, reply_to);
    return;

err_data_code:
err_reply_code:
err_send_msg_alloc:
    zmsg_destroy (&report);
}

