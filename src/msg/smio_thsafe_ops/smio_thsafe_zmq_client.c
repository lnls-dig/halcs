/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include "bpm_server.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, MSG, "[smio_thsafe_client:zmq]", \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_ALLOC(ptr, MSG, "[smio_thsafe_client:zmq]",  \
            msg_err_str(MSG_ERR_ALLOC),                     \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                            \
    CHECK_HAL_ERR(err, MSG, "[smio_thsafe_client:zmq]",     \
            msg_err_str (err_type))

static zmsg_t *_thsafe_zmq_client_recv_confirmation (smio_t *self);
int _thsafe_zmq_client_open_release (smio_t *self, llio_endpoint_t *endpoint, uint32_t opcode);
static ssize_t _thsafe_zmq_client_read_generic (smio_t *self, uint64_t offs, uint8_t *data,
        uint32_t size);
static ssize_t _thsafe_zmq_client_write_generic (smio_t *self, uint64_t offs, const uint8_t *data,
        uint32_t size);
static ssize_t _thsafe_zmq_client_recv_rw (smio_t *self, uint8_t *data,
        uint32_t size, bool accept_empty_data);

/**** Open device ****/
int thsafe_zmq_client_open (smio_t *self, llio_endpoint_t *endpoint)
{
    return _thsafe_zmq_client_open_release (self, endpoint, THSAFE_OPCODE_OPEN);
}

/**** Release device ****/

int thsafe_zmq_client_release (smio_t *self, llio_endpoint_t *endpoint)
{
    return _thsafe_zmq_client_open_release (self, endpoint, THSAFE_OPCODE_RELEASE);
}

/**** Read data from device ****/
ssize_t thsafe_zmq_client_read_16 (smio_t *self, uint64_t offs, uint16_t *data)
{
    return _thsafe_zmq_client_read_generic (self, offs, (uint8_t *) data, THSAFE_READ_16_DSIZE);
}

ssize_t thsafe_zmq_client_read_32 (smio_t *self, uint64_t offs, uint32_t *data)
{
    return _thsafe_zmq_client_read_generic (self, offs, (uint8_t *) data, THSAFE_READ_32_DSIZE);
}

ssize_t thsafe_zmq_client_read_64 (smio_t *self, uint64_t offs, uint64_t *data)
{
    return _thsafe_zmq_client_read_generic (self, offs, (uint8_t *) data, THSAFE_READ_64_DSIZE);
}

/**** Write data to device ****/
ssize_t thsafe_zmq_client_write_16 (smio_t *self, uint64_t offs, const uint16_t *data)
{
    return _thsafe_zmq_client_write_generic (self, offs, (const uint8_t *) data, THSAFE_WRITE_16_DSIZE);
}

ssize_t thsafe_zmq_client_write_32 (smio_t *self, uint64_t offs, const uint32_t *data)
{
    return _thsafe_zmq_client_write_generic (self, offs, (const uint8_t *) data, THSAFE_WRITE_32_DSIZE);
}

ssize_t thsafe_zmq_client_write_64 (smio_t *self, uint64_t offs, const uint64_t *data)
{
    return _thsafe_zmq_client_write_generic (self, offs, (const uint8_t *) data, THSAFE_WRITE_64_DSIZE);
}

/**** Read data block from device function pointer, size in bytes ****/
ssize_t thsafe_zmq_client_read_block (smio_t *self, uint64_t offs, size_t size, uint32_t *data)
{
    assert (self);
    ssize_t ret_size = -1;
    zmsg_t *send_msg = zmsg_new ();
    ASSERT_ALLOC(send_msg, err_msg_alloc);
    uint32_t opcode = THSAFE_OPCODE_READ_BLOCK;
    zsock_t *pipe_msg = smio_get_pipe_msg (self);
    ASSERT_TEST(pipe_msg != NULL, "Could not get SMIO PIPE MSG",
            err_get_pipe_msg);

    DBE_DEBUG (DBG_MSG | DBG_LVL_TRACE, "[smio_thsafe_client:zmq] Calling thsafe_read_block\n");

    /* Message is:
     * frame 0: READ_BLOCK opcode
     * frame 1: offset
     * frame 2: number of bytes to be read */
    int zerr = zmsg_addmem (send_msg, &opcode, sizeof (opcode));
    ASSERT_TEST(zerr == 0, "Could not add READ opcode in message",
            err_add_opcode);
    zerr = zmsg_addmem (send_msg, &offs, sizeof (offs));
    ASSERT_TEST(zerr == 0, "Could not add offset in message",
            err_add_offset);
    zerr = zmsg_addmem (send_msg, &size, sizeof (size));
    ASSERT_TEST(zerr == 0, "Could not add size in message",
            err_add_size);

    DBE_DEBUG (DBG_MSG | DBG_LVL_TRACE, "[smio_thsafe_client:zmq] Sending message:\n");
#ifdef LOCAL_MSG_DBG
    debug_log_print_zmq_msg (send_msg);
#endif

    zerr = zmsg_send (&send_msg, pipe_msg);
    ASSERT_TEST(zerr == 0, "Could not send message", err_send_msg);

    /* Message is:
     * frame 0: reply code
     * frame 1: return code
     * frame 2: data */
    ret_size = _thsafe_zmq_client_recv_rw (self, (uint8_t *) data, size, true);

err_send_msg:
err_add_size:
err_add_offset:
err_add_opcode:
err_get_pipe_msg:
    zmsg_destroy (&send_msg);
err_msg_alloc:
    return ret_size;
}

/**** Write data block from device function pointer, size in bytes ****/
ssize_t thsafe_zmq_client_write_block (smio_t *self, uint64_t offs, size_t size, const uint32_t *data)
{
    assert (self);
    zmsg_t *send_msg = zmsg_new ();
    ASSERT_ALLOC(send_msg, err_msg_alloc);
    uint32_t opcode = THSAFE_OPCODE_WRITE_BLOCK;
    zsock_t *pipe_msg = smio_get_pipe_msg (self);
    ASSERT_TEST(pipe_msg != NULL, "Could not get SMIO PIPE MSG",
            err_get_pipe_msg);

    DBE_DEBUG (DBG_MSG | DBG_LVL_TRACE, "[smio_thsafe_client:zmq] Calling thsafe_write_block\n");

    /* Message is:
     * frame 0: WRITE_BLOCK opcode
     * frame 1: offset
     * frame 2: data to be written
     * */
    int zerr = zmsg_addmem (send_msg, &opcode, sizeof (opcode));
    ASSERT_TEST(zerr == 0, "Could not add WRITE opcode in message",
            err_add_opcode);
    zerr = zmsg_addmem (send_msg, &offs, sizeof (offs));
    ASSERT_TEST(zerr == 0, "Could not add offset in message",
            err_add_offset);
    zerr = zmsg_addmem (send_msg, data, size);
    ASSERT_TEST(zerr == 0, "Could not add data in message",
            err_add_data);

    DBE_DEBUG (DBG_MSG | DBG_LVL_TRACE, "[smio_thsafe_client:zmq] Sending message:\n");
#ifdef LOCAL_MSG_DBG
    debug_log_print_zmq_msg (send_msg);
#endif

    zerr = zmsg_send (&send_msg, pipe_msg);
    ASSERT_TEST(zerr == 0, "Could not send message", err_send_msg);

    /* Message is:
     * frame 0: reply code
     * frame 1: return code
     * frame 2: data */
    uint32_t ret_data = 0;
    ssize_t ret_size = _thsafe_zmq_client_recv_rw (self, (uint8_t *) &ret_data,
            sizeof (ret_data), false);
    ASSERT_TEST(ret_size == sizeof (ret_data), "Data size does not match the expected",
            err_data_size);

    zmsg_destroy (&send_msg);
    return ret_data;

err_data_size:
err_send_msg:
err_add_data:
err_add_offset:
err_add_opcode:
err_get_pipe_msg:
    zmsg_destroy (&send_msg);
err_msg_alloc:
    return -1;
}

/**** Read data block via DMA from device, size in bytes ****/
ssize_t thsafe_zmq_client_read_dma (smio_t *self, uint64_t offs, size_t size, uint32_t *data)
{
    (void) self;
    (void) offs;
    (void) size;
    (void) data;
    return -1;
}

/**** Write data block via DMA from device, size in bytes ****/
ssize_t thsafe_zmq_client_write_dma (smio_t *self, uint64_t offs, size_t size, const uint32_t *data)
{
    (void) self;
    (void) offs;
    (void) size;
    (void) data;
    return -1;
}

/**** Read device information function pointer ****/
/* int thsafe_zmq_client_read_info (smio_t *self, thsafe_dev_info_t *dev_info)
 *{
 *  (void) self;
 *  (void) dev_info;
 *   return -1;
 *} */

/*************** Helper functions **************/

int _thsafe_zmq_client_open_release (smio_t *self, llio_endpoint_t *endpoint, uint32_t opcode)
{
    (void) endpoint;
    assert (self);

    int ret = -1;
    zmsg_t *send_msg = zmsg_new ();
    ASSERT_ALLOC(send_msg, err_msg_alloc);
    zsock_t *pipe_msg = smio_get_pipe_msg (self);
    ASSERT_TEST(pipe_msg != NULL, "Could not get SMIO PIPE MSG",
            err_get_pipe_msg);

    DBE_DEBUG (DBG_MSG | DBG_LVL_TRACE, "[smio_thsafe_client:zmq] Calling thsafe_release\n");
    /* Message is:
     * frame 0: RELEASE opcode
     * frame 1: endpopint struct (FIXME?) */
    int zerr = zmsg_addmem (send_msg, &opcode, sizeof (opcode));
    ASSERT_TEST(zerr == 0, "Could not add OPEN opcode in message",
            err_add_opcode);
    /* FIXME: This is wrong and unused */
    zerr = zmsg_addmem (send_msg, 0, 0);
    ASSERT_TEST(zerr == 0, "Could not add endpoint in message",
            err_add_endpoint);

    DBE_DEBUG (DBG_MSG | DBG_LVL_TRACE, "[smio_thsafe_client:zmq] Sending message:\n");
#ifdef LOCAL_MSG_DBG
    debug_log_print_zmq_msg (send_msg);
#endif
    zerr = zmsg_send (&send_msg, pipe_msg);
    ASSERT_TEST(zerr == 0, "Could not send message", err_send_msg);

    /* Message is:
     * frame 0: reply code */
    /* Returns NULL if confirmation was not OK or in case of error.
     * Returns the original message if the confirmation was OK */
    zmsg_t *recv_msg = _thsafe_zmq_client_recv_confirmation (self);
    ASSERT_TEST(recv_msg != NULL, "Could not receive confirmation code", err_null_raw_data);

    /* If we are here the message got a OK reply code.
     * Just return the reply code */
    zframe_t *ret_code_frame = zmsg_pop (recv_msg);

    /* Check for malformed message */
    ASSERT_TEST(ret_code_frame != NULL, "Interrupted or malformed message",
            err_recv_data);

    /* Check if the frame has the number of bytes requested.
     * For now, we consider a success only when the number of
     * bytes requested is the same as the actually read*/
    ASSERT_TEST(zframe_size (ret_code_frame) == THSAFE_REPLY_SIZE, "Frame size is wrong",
            err_recv_data_size);

    ret = *(THSAFE_REPLY_TYPE *) zframe_data (ret_code_frame);
    DBE_DEBUG (DBG_MSG | DBG_LVL_TRACE, "[smio_thsafe_client:zmq] Received return code: %08X\n", ret);

err_recv_data_size:
    zframe_destroy (&ret_code_frame);
err_recv_data:
err_null_raw_data:
    zmsg_destroy (&recv_msg);
err_send_msg:
err_add_endpoint:
err_add_opcode:
err_get_pipe_msg:
    zmsg_destroy (&send_msg);
err_msg_alloc:
    return ret;
}

static ssize_t _thsafe_zmq_client_read_generic (smio_t *self, uint64_t offs, uint8_t *data,
        uint32_t size)
{
    assert (self);
    ssize_t ret_size = -1;
    zmsg_t *send_msg = zmsg_new ();
    ASSERT_ALLOC(send_msg, err_msg_alloc);
    zsock_t *pipe_msg = smio_get_pipe_msg (self);
    ASSERT_TEST(pipe_msg != NULL, "Could not get SMIO PIPE MSG",
            err_get_pipe_msg);
    uint32_t opcode;

    DBE_DEBUG (DBG_MSG | DBG_LVL_TRACE, "[smio_thsafe_client:zmq] Calling _thsafe_read_generic\n");

    switch (size) {
        case THSAFE_READ_16_DSIZE:
            opcode = THSAFE_OPCODE_READ_16;
        break;

        case THSAFE_READ_32_DSIZE:
            opcode = THSAFE_OPCODE_READ_32;
        break;

        case THSAFE_READ_64_DSIZE:
            opcode = THSAFE_OPCODE_READ_64;
        break;

        default:
            opcode = THSAFE_OPCODE_READ_32;
    }

    /* Message is:
     * frame 0: READ<size> opcode
     * frame 1: offset */
    int zerr = zmsg_addmem (send_msg, &opcode, sizeof (opcode));
    ASSERT_TEST(zerr == 0, "Could not add READ opcode in message",
            err_add_opcode);
    zerr = zmsg_addmem (send_msg, &offs, sizeof (offs));
    ASSERT_TEST(zerr == 0, "Could not add offset in message",
            err_add_offset);

    DBE_DEBUG (DBG_MSG | DBG_LVL_TRACE, "[smio_thsafe_client:zmq] Sending message:\n");
#ifdef LOCAL_MSG_DBG
    debug_log_print_zmq_msg (send_msg);
#endif

    zerr = zmsg_send (&send_msg, pipe_msg);
    ASSERT_TEST(zerr == 0, "Could not send message", err_send_msg);

    /* Message is:
     * frame 0: reply code
     * frame 1: return code
     * frame 2: data */
    ret_size = _thsafe_zmq_client_recv_rw (self, data, size, true);

err_send_msg:
err_add_offset:
err_add_opcode:
err_get_pipe_msg:
    zmsg_destroy (&send_msg);
err_msg_alloc:
    return ret_size;
}

static ssize_t _thsafe_zmq_client_write_generic (smio_t *self, uint64_t offs, const uint8_t *data,
        uint32_t size)
{
    assert (self);
    zmsg_t *send_msg = zmsg_new ();
    ASSERT_ALLOC(send_msg, err_msg_alloc);
    zsock_t *pipe_msg = smio_get_pipe_msg (self);
    ASSERT_TEST(pipe_msg != NULL, "Could not get SMIO PIPE MSG",
            err_get_pipe_msg);
    uint32_t opcode;

    DBE_DEBUG (DBG_MSG | DBG_LVL_TRACE, "[smio_thsafe_client:zmq] Calling _thsafe_write_generic\n");

    switch (size) {
        case THSAFE_WRITE_16_DSIZE:
            opcode = THSAFE_OPCODE_WRITE_16;
        break;

        case THSAFE_WRITE_32_DSIZE:
            opcode = THSAFE_OPCODE_WRITE_32;
        break;

        case THSAFE_WRITE_64_DSIZE:
            opcode = THSAFE_OPCODE_WRITE_64;
        break;

        default:
            opcode = THSAFE_OPCODE_WRITE_32;
    }

    /* Message is:
     * frame 0: WRITE<size> opcode
     * frame 1: offset
     * frame 2: data to be written
     * */
    int zerr = zmsg_addmem (send_msg, &opcode, sizeof (opcode));
    ASSERT_TEST(zerr == 0, "Could not add WRITE opcode in message",
            err_add_opcode);
    zerr = zmsg_addmem (send_msg, &offs, sizeof (offs));
    ASSERT_TEST(zerr == 0, "Could not add offset in message",
            err_add_offset);
    zerr = zmsg_addmem (send_msg, data, size);
    ASSERT_TEST(zerr == 0, "Could not add READ opcode in message",
            err_add_data);

    DBE_DEBUG (DBG_MSG | DBG_LVL_TRACE, "[smio_thsafe_client:zmq] Sending message:\n");
#ifdef LOCAL_MSG_DBG
    debug_log_print_zmq_msg (send_msg);
#endif

    zerr = zmsg_send (&send_msg, pipe_msg);
    ASSERT_TEST(zerr == 0, "Could not send message",
            err_send_msg);

    /* Message is:
     * frame 0: reply code
     * frame 1: return code
     * frame 2: data */
    uint32_t ret_data = 0;
    ssize_t ret_size = _thsafe_zmq_client_recv_rw (self, (uint8_t *) &ret_data,
            sizeof (ret_data), false);
    ASSERT_TEST(ret_size == sizeof (ret_data), "Data size does not match the expected",
            err_data_size);

    zmsg_destroy (&send_msg);
    return ret_data;

err_data_size:
err_send_msg:
err_add_data:
err_add_offset:
err_add_opcode:
err_get_pipe_msg:
    zmsg_destroy (&send_msg);
err_msg_alloc:
    return -1;
}

static zmsg_t *_thsafe_zmq_client_recv_confirmation (smio_t *self)
{
    DBE_DEBUG (DBG_MSG | DBG_LVL_TRACE, "[smio_thsafe_client:zmq] Calling _thsafe_zmq_client_recv_confirmation\n");

    assert (self);
    zsock_t *pipe_msg = smio_get_pipe_msg (self);
    ASSERT_TEST(pipe_msg != NULL, "Could not get SMIO PIPE MSG",
            err_get_pipe_msg);
    /* Wait for response */
    zmsg_t *recv_msg = zmsg_recv (pipe_msg);
    /* Do not pop the message, just set a cursor to it */
    zframe_t *reply_frame = zmsg_first (recv_msg);

    DBE_DEBUG (DBG_MSG | DBG_LVL_TRACE, "[smio_thsafe_client:zmq] Receiving message:\n");
#ifdef LOCAL_MSG_DBG
    debug_log_print_zmq_msg (recv_msg);
#endif

    /* Message is:
     * frame 0: Reply code */
    /* Check for interrupted or malformed message */
    ASSERT_TEST(reply_frame != NULL, "Interrupted or malformed message",
            err_recv_data);
    /* Check if the frame has the correct number of bytes */
    ASSERT_TEST(zframe_size (reply_frame) == THSAFE_REPLY_SIZE, "Frame size is wrong",
            err_recv_data);

    uint8_t *raw_data = (uint8_t *) zframe_data (reply_frame);
    ASSERT_TEST(raw_data != NULL, "Could not receive confirmation code", err_null_raw_data);
    uint32_t reply_code = *(uint32_t *) raw_data;

    /* Check for confirmation */
    ASSERT_TEST(reply_code == THSAFE_OK, "Received reply code NOT OK",
            err_reply_code_not_ok);

    /* Caller owns the message and is its responsability to destroy it */
    return recv_msg;

/* TODO: reduce code repetition */
err_reply_code_not_ok:
err_null_raw_data:
err_recv_data:
    zmsg_destroy (&recv_msg);
err_get_pipe_msg:
    return NULL;
}

static ssize_t _thsafe_zmq_client_recv_rw (smio_t *self, uint8_t *data,
        uint32_t size, bool accept_empty_data)
{
    ssize_t ret_size = -1;

    /* Returns NULL if confirmation was not OK or in case of error.
     * Returns the original message if the confirmation was OK */
    zmsg_t *recv_msg = _thsafe_zmq_client_recv_confirmation (self);
    ASSERT_TEST(recv_msg != NULL, "Could not receive confirmation code",
            err_null_recv_msg);

    /* If we are here, confirmation code was OK. Check for second frame */
    zframe_t *reply_frame = zmsg_pop (recv_msg);
    zframe_destroy (&reply_frame); /* Don't do anything with the reply code */

    zframe_t *return_frame = zmsg_pop (recv_msg);
    zframe_t *data_frame = NULL;

    /* Some messages accept an empty data frame */
    if (return_frame == NULL && accept_empty_data) {
        DBE_DEBUG (DBG_MSG | DBG_LVL_TRACE, "[smio_thsafe_client:zmq] Empty "
                "data frame received\n");
        ret_size = 0;
    }
    else {
        ASSERT_TEST(return_frame != NULL, "Could not receive return code",
                err_null_ret_code_frame);
        /* Check for return frame size */
        ASSERT_TEST(zframe_size (return_frame) == THSAFE_RETURN_SIZE,
                "Return frame size is wrong", err_wrong_size_ret_frame);

        data_frame = zmsg_pop (recv_msg);
        ASSERT_TEST(data_frame != NULL, "Could not receive data", err_recv_data);

        /* Check if the frame has the number of bytes requested.
         * For now, we consider a success only when the number of
         * bytes requested is the same as the actually read*/
        size_t data_frame_size = zframe_size (data_frame);
        THSAFE_RETURN_TYPE ret_frame_size =
            *(THSAFE_RETURN_TYPE *) zframe_data (return_frame);

        ASSERT_TEST(ret_frame_size >= 0 &&
                (THSAFE_RETURN_UTYPE) ret_frame_size == data_frame_size,
                "Received data frame size is wrong", err_data_frame_size);
        ASSERT_TEST(data_frame_size >= size,
                "Specified buffer size is bigger than the available data",
                err_buf_size_data);

        uint8_t* raw_data = (uint8_t *) zframe_data (data_frame);
        memcpy (data, raw_data, size);
        ret_size = size;
    }

    DBE_DEBUG (DBG_MSG | DBG_LVL_TRACE, "[smio_thsafe_client:zmq] Received data "
            "successfully\n");
err_buf_size_data:
err_data_frame_size:
    zframe_destroy (&data_frame);
err_recv_data:
err_wrong_size_ret_frame:
    zframe_destroy (&return_frame);
err_null_ret_code_frame:
    zmsg_destroy (&recv_msg);
err_null_recv_msg:
    return ret_size;
}

/*************** Our constant structure **************/
const smio_thsafe_client_ops_t smio_thsafe_client_zmq_ops = {
    .thsafe_client_open           = thsafe_zmq_client_open,        /* Open device */
    .thsafe_client_release        = thsafe_zmq_client_release,     /* Release device */
    .thsafe_client_read_16        = thsafe_zmq_client_read_16,     /* Read 16-bit data */
    .thsafe_client_read_32        = thsafe_zmq_client_read_32,     /* Read 32-bit data */
    .thsafe_client_read_64        = thsafe_zmq_client_read_64,     /* Read 64-bit data */
    .thsafe_client_write_16       = thsafe_zmq_client_write_16,    /* Write 16-bit data */
    .thsafe_client_write_32       = thsafe_zmq_client_write_32,    /* Write 32-bit data */
    .thsafe_client_write_64       = thsafe_zmq_client_write_64,    /* Write 64-bit data */
    .thsafe_client_read_block     = thsafe_zmq_client_read_block,  /* Read arbitrary block size data,
                                                                        parameter size in bytes */
    .thsafe_client_write_block    = thsafe_zmq_client_write_block, /* Write arbitrary block size data,
                                                                        parameter size in bytes */
    .thsafe_client_read_dma       = thsafe_zmq_client_read_dma,    /* Read arbitrary block size data via DMA,
     _                                                                  parameter size in bytes */
    .thsafe_client_write_dma      = thsafe_zmq_client_write_dma    /* Write arbitrary block size data via DMA,
                                                                        parameter size in bytes */
    /*.thsafe_client_read_info      = thsafe_zmq_client_read_info */   /* Read device information data */
};
