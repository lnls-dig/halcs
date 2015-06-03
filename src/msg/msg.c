/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include "bpm_server.h"
/* Private headers */
#include "sm_io_exports.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, MSG, "[msg]",                     \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)       \
    ASSERT_HAL_ALLOC(ptr, MSG, "[msg]",                             \
            msg_err_str(MSG_ERR_ALLOC),                             \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                    \
    CHECK_HAL_ERR(err, MSG, "[msg]",                                \
            msg_err_str (err_type))

static msg_type_e _msg_guess_type (void *msg);
static msg_err_e _msg_validate (void *msg, msg_type_e expected_msg_type);
static msg_err_e _msg_exp_zmq_get_opcode (exp_msg_zmq_t *msg, uint32_t *opcode);
static msg_err_e _msg_thsafe_zmq_get_opcode (zmq_server_args_t *msg, uint32_t *opcode);
static msg_err_e _msg_gen_get_opcode (zmsg_t *zmq_msg, uint32_t *opcode);
static msg_err_e _msg_format_client_response (int disp_table_ret,
        RW_REPLY_TYPE *reply_code, bool *with_data_frame);

/* Helper functions */
static RW_REPLY_TYPE _msg_format_reply_code (int reply_code);
static zmsg_t * _msg_create_client_response (RW_REPLY_TYPE reply_code, uint32_t reply_size,
        uint32_t *data_out, bool with_data_frame);
static void _msg_send_client_response_mlm (RW_REPLY_TYPE reply_code, uint32_t reply_size,
        uint32_t *data_out, bool with_data_frame, mlm_client_t *worker,
        zframe_t *reply_to);
static void _msg_send_client_response_sock (RW_REPLY_TYPE reply_code, uint32_t reply_size,
        uint32_t *data_out, bool with_data_frame, zframe_t *reply_to);

msg_type_e msg_guess_type (void *msg)
{
    return _msg_guess_type (msg);
}

msg_err_e msg_validate (void *msg, msg_type_e expected_msg_type)
{
    return _msg_validate (msg, expected_msg_type);
}

/* Handle MLM protocol (used by SMIOs, for instance) request */
msg_err_e msg_handle_mlm_request (void *owner, void *args,
        disp_table_t *disp_table)
{
    msg_err_e err = MSG_SUCCESS;
    uint32_t opcode_data = 0;

    /* Our simple packet is composed of:
     * frame 0: operation
     * frame n: arguments*/
    err = _msg_validate (args, MSG_EXP_ZMQ); /* Only EXP ZMQ messages */
    /* FIXME. Improve error codes */
    /* Sanity checks */
    ASSERT_TEST(err == MSG_SUCCESS, "Could not receive opcode", err_inv_msg);

    /* If we are here, this is the only option */
    SMIO_OWNER_TYPE *self = SMIO_EXP_OWNER(owner);
    mlm_client_t *worker = smio_get_worker (self);
    ASSERT_TEST(worker != NULL, "Could not get SMIO worker", err_get_smio_worker,
            MSG_ERR_ALLOC);

    exp_msg_zmq_t *msg = (exp_msg_zmq_t *) args;
    /* Get opcode */
    err = _msg_exp_zmq_get_opcode (msg, &opcode_data);
    ASSERT_TEST(err == MSG_SUCCESS, "Could not get message opcode", err_get_opcode);

    /* Check registered function arguments */
    void *ret = NULL;
    int disp_table_ret = disp_table_check_call (disp_table, opcode_data, owner,
            args, &ret);

    RW_REPLY_TYPE reply_code = PARAM_ERR;
    bool with_data_frame = false;
    err = _msg_format_client_response (disp_table_ret, &reply_code, &with_data_frame);
    ASSERT_TEST(err == MSG_SUCCESS, "Could not format client response",
            err_format_response);

    /* Send response back to client */
    _msg_send_client_response_mlm (reply_code, disp_table_ret, ret, with_data_frame,
           worker, msg->reply_to);

    return err;

err_format_response:
err_get_opcode:
    _msg_send_client_response_mlm (PARAM_ERR, 0, NULL, false, worker,
            msg->reply_to);
err_get_smio_worker:
err_inv_msg:
    return err;
}

/* Handle regular protocol (used by DEVIOs, for instance) request */
msg_err_e msg_handle_sock_request (void *owner, void *args,
        disp_table_t *disp_table)
{
    msg_err_e err = MSG_SUCCESS;
    uint32_t opcode_data = 0;

    /* Our simple packet is composed of:
     * frame 0: operation
     * frame n: arguments*/
    err = _msg_validate (args, MSG_THSAFE_ZMQ); /* Only THSAFE ZMQ messages */
    /* FIXME. Improve error codes */
    /* Sanity checks */
    ASSERT_TEST(err == MSG_SUCCESS, "Could not receive opcode", err_inv_msg);

    /* If we are here, this is the only option */
    zmq_server_args_t *msg = (zmq_server_args_t *) args;
    /* Get opcode */
    err = _msg_thsafe_zmq_get_opcode (msg, &opcode_data);
    ASSERT_TEST(err == MSG_SUCCESS, "Could not get message opcode", err_get_opcode);

    /* Check registered function arguments */
    void *ret = NULL;
    int disp_table_ret = disp_table_check_call (disp_table, opcode_data, owner,
            args, &ret);

    RW_REPLY_TYPE reply_code = PARAM_ERR;
    bool with_data_frame = false;
    err = _msg_format_client_response (disp_table_ret, &reply_code, &with_data_frame);
    ASSERT_TEST(err == MSG_SUCCESS, "Could not format client response",
            err_format_response);

    /* Send response back to client */
    _msg_send_client_response_sock (reply_code, disp_table_ret, ret, with_data_frame,
           msg->reply_to);

    return err;

err_format_response:
err_get_opcode:
    _msg_send_client_response_sock (PARAM_ERR, 0, NULL, false, msg->reply_to);
err_inv_msg:
    return err;
}

msg_err_e msg_check_gen_zmq_args (const disp_op_t *disp_op, zmsg_t *zmq_msg)
{
    msg_err_e err = MSG_SUCCESS;
    GEN_MSG_ZMQ_ARG_TYPE zmq_arg = GEN_MSG_ZMQ_PEEK_FIRST(zmq_msg);

    /* Iterate over all arguments and check if they match in size with the
     * specified disp_op parameters */
    const uint32_t *args_it = disp_op->args;
    unsigned i;
    for (i = 0 ; *args_it != DISP_ARG_END; ++args_it, ++i) {
        DBE_DEBUG (DBG_MSG | DBG_LVL_TRACE,
                "[msg] Checking argument #%u for function \"%s\"\n",
                i, disp_op->name);
        /* We have argument to check according to disp_op->args */
        if (zmq_arg == NULL) {
            DBE_DEBUG (DBG_MSG | DBG_LVL_ERR,
                    "[msg] Missing arguments in message"
                    " received for function \"%s\"\n", disp_op->name);
            err = MSG_ERR_INV_LESS_ARGS;
            goto err_inv_less_args;
        }

        /* We have received something and will check for (byte) size
         * correctness */
        if ((GEN_MSG_ZMQ_ARG_SIZE(zmq_arg) > DISP_GET_ASIZE(*args_it)) ||
                (DISP_GET_ATYPE(*args_it) != DISP_ATYPE_VAR &&
                 GEN_MSG_ZMQ_ARG_SIZE(zmq_arg) != DISP_GET_ASIZE(*args_it))) {
            DBE_DEBUG (DBG_MSG | DBG_LVL_ERR,
                    "[msg] Invalid size of argument #%u"
                    " received for function \"%s\"\n", i, disp_op->name);
            err = MSG_ERR_INV_SIZE_ARG;
            goto err_inv_size_args;
        }

        zmq_arg = GEN_MSG_ZMQ_PEEK_NEXT_ARG(zmq_msg);
    }

    /* According to disp_op->args we are done. So, check if the message received
     * has any more arguments */
    if (zmq_arg != NULL) {
        DBE_DEBUG (DBG_MSG | DBG_LVL_ERR,
                "[msg] Extra arguments in message"
                " received for function \"%s\"\n", disp_op->name);
        err = MSG_ERR_INV_MORE_ARGS;
        goto err_inv_more_args;
    }

    DBE_DEBUG (DBG_MSG | DBG_LVL_TRACE,
            "[msg] No errors detected on the received arguments "
            "for function \"%s\"\n", disp_op->name);

err_inv_more_args:
err_inv_size_args:
err_inv_less_args:
    GEN_MSG_ZMQ_PEEK_EXIT(zmq_msg);
    return err;
}

/************************************************************/
/********************* Static Functions *********************/
/************************************************************/

static msg_type_e _msg_guess_type (void *msg)
{
    msg_type_e msg_type = MSG_NOT_SUPPORTED;

    if (exp_msg_zmq_is (msg)) {
        msg_type = MSG_EXP_ZMQ;
    }
    else if (thsafe_msg_zmq_is (msg)) {
        msg_type = MSG_THSAFE_ZMQ;
    }

    return msg_type;
}

static msg_err_e _msg_validate (void *msg, msg_type_e expected_msg_type)
{
    assert (msg);
    msg_err_e err = MSG_SUCCESS;

    /* Try to guess which type of message we are dealing with */
    switch (_msg_guess_type (msg)) {
        case MSG_EXP_ZMQ:
            ASSERT_TEST(expected_msg_type == MSG_EXP_ZMQ,
                    "Expected messge type does not match received message type",
                    err_unexpected_msg, MSG_ERR_UNEX_MSG);
            break;
        case MSG_THSAFE_ZMQ:
            ASSERT_TEST(expected_msg_type == MSG_THSAFE_ZMQ,
                    "Expected messge type does not match received message type",
                    err_unexpected_msg, MSG_ERR_UNEX_MSG);
            break;
        default:
            DBE_DEBUG (DBG_MSG | DBG_LVL_ERR, "[msg] Unexpected message type\n");
            err = MSG_ERR_UNEX_MSG;
            break;
    }

err_unexpected_msg:
    return err;
}

static msg_err_e _msg_exp_zmq_get_opcode (exp_msg_zmq_t *msg, uint32_t *opcode)
{
    return _msg_gen_get_opcode (EXP_MSG_ZMQ(msg), opcode);
}

static msg_err_e _msg_thsafe_zmq_get_opcode (zmq_server_args_t *msg, uint32_t *opcode)
{
    return _msg_gen_get_opcode (THSAFE_MSG_ZMQ(msg), opcode);
}

static msg_err_e _msg_gen_get_opcode (zmsg_t *zmq_msg, uint32_t *opcode)
{
    msg_err_e err = MSG_SUCCESS;

    /* Our simple packet is composed of:
     * frame 0: operation
     * frame n: arguments*/
    zframe_t *opcode_frm = zmsg_pop (zmq_msg);

    /* FIXME. Improve error codes */
    /* Sanity checks */
    ASSERT_TEST(opcode_frm != NULL, "Could not receive opcode", err_null_opcode,
            MSG_ERR_WRONG_ARGS);
    ASSERT_TEST(zframe_size (opcode_frm) == MSG_OPCODE_SIZE,
            "Invalid opcode size received", err_wrong_opcode_size,
            MSG_ERR_WRONG_ARGS);

    *opcode = *(uint32_t *) zframe_data (opcode_frm);
    ASSERT_TEST(*opcode < MSG_OPCODE_MAX, "Invalid opcode received",
            err_invalid_opcode, MSG_ERR_WRONG_ARGS);

    zframe_destroy (&opcode_frm);
    return err;

err_invalid_opcode:
err_wrong_opcode_size:
    zframe_destroy (&opcode_frm);
err_null_opcode:
    return err;
}

static msg_err_e _msg_format_client_response (int disp_table_ret,
        RW_REPLY_TYPE *reply_code, bool *with_data_frame)
{
    /* Log only error message */
    if (disp_table_ret < 0) {
        DBE_DEBUG (DBG_MSG | DBG_LVL_ERR, "[msg] disp_table_check_call returned "
                "status %d\n", disp_table_ret);
    }

    /* Error returned from registered function */
    if (disp_table_ret <= 0) {
        /* Encode error to send to client */
        *reply_code = _msg_format_reply_code (disp_table_ret);
        *with_data_frame = false;
    }
    else {
        *reply_code = PARAM_OK;
        *with_data_frame = true;
    }

    return MSG_SUCCESS;
}

/************************************************************/
/********************* Helper Functions *********************/
/************************************************************/

static RW_REPLY_TYPE _msg_format_reply_code (int reply_code)
{
    return (reply_code < 0) ? -reply_code : reply_code;
}

static void _msg_send_client_response_mlm (RW_REPLY_TYPE reply_code, uint32_t reply_size,
        uint32_t *data_out, bool with_data_frame, mlm_client_t *worker,
        zframe_t *reply_to)
{
    (void) reply_to;
    zmsg_t *msg = _msg_create_client_response (reply_code, reply_size, data_out,
            with_data_frame);
    ASSERT_TEST(msg != NULL, "Could format client message",
            err_fmt_client_message);

    mlm_client_sendto (worker, mlm_client_sender (worker), NULL, NULL, 0, &msg);
err_fmt_client_message:
    return;
}

static void _msg_send_client_response_sock (RW_REPLY_TYPE reply_code, uint32_t reply_size,
        uint32_t *data_out, bool with_data_frame, zframe_t *reply_to)
{
    zmsg_t *msg = _msg_create_client_response (reply_code, reply_size, data_out,
            with_data_frame);
    ASSERT_TEST(msg != NULL, "Could format client message",
            err_fmt_client_message);

    zmsg_send (&msg, reply_to);
err_fmt_client_message:
    return;
}

static zmsg_t * _msg_create_client_response (RW_REPLY_TYPE reply_code, uint32_t reply_size,
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
