/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include "smio_thsafe_zmq_server.h"
#include "hal_assert.h"
#include "msg_err.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label)  \
    ASSERT_HAL_TEST(test_boolean, MSG, "[smio_thsafe_server:zmq]", \
            err_str, err_goto_label)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label)                   \
    ASSERT_HAL_ALLOC(ptr, MSG, "[smio_thsafe_server:zmq]",  \
            msg_err_str(MSG_ERR_ALLOC),                     \
            err_goto_label)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                            \
    CHECK_HAL_ERR(err, MSG, "[smio_thsafe_server:zmq]",     \
            msg_err_str (err_type))

static int _thsafe_zmq_server_send_read (int32_t *llio_ret, uint8_t *data,
        uint32_t size, void *reply_to);
static int _thsafe_zmq_server_recv_read (zmsg_t *msg, loff_t *offset);
static int _thsafe_zmq_server_send_write (int32_t *llio_ret, void *reply_to);
static int _thsafe_zmq_server_recv_write (zmsg_t *msg, loff_t *offset,
        zframe_t **data_write_frame);

/**** Open device ****/
void *thsafe_zmq_server_open (void *owner, void *args)
{
    assert (owner);
    assert (args);

    devio_t *self = (devio_t *) owner;
    zmq_server_args_t *server_args = (zmq_server_args_t *) args;

    DBE_DEBUG (DBG_MSG | DBG_LVL_TRACE, "[smio_thsafe_server:zmq] Calling thsafe_open\n");
    DBE_DEBUG (DBG_MSG | DBG_LVL_TRACE, "[smio_thsafe_server:zmq] Received message:\n");
#ifdef LOCAL_MSG_DBG
    zmsg_print (*server_args->msg);
#endif
    /* Message is:
     * frame null: OPEN opcode (removed by dev_io)
     * frame 0: endpopint struct (FIXME?) */
    zframe_t *endpoint = zmsg_pop (*server_args->msg);
    ASSERT_ALLOC(endpoint, err_endpoint_alloc);

    /* Call llio to actually perform the
     * operation */
    int32_t ret = llio_open (self->llio,
            (llio_endpoint_t *) zframe_data (endpoint));

    /* Send reply back to client */
    zmsg_t *send_msg = zmsg_new ();
    ASSERT_ALLOC(send_msg, err_send_msg_alloc);

    THSAFE_REPLY_TYPE reply_code = THSAFE_OK;
    if (ret != 0) {
        reply_code = THSAFE_ERR;
    }

    int zerr = zmsg_addmem (send_msg, &reply_code, sizeof (reply_code));
    ASSERT_TEST(zerr == 0, "Could not add reply code in message", err_reply_code);
    zerr = zmsg_addmem (send_msg, &ret, sizeof (ret));
    ASSERT_TEST(zerr == 0, "Could not add return code in message", err_ret_code);

    DBE_DEBUG (DBG_MSG | DBG_LVL_TRACE, "[smio_thsafe_server:zmq] Sending message:\n");
#ifdef LOCAL_MSG_DBG
    zmsg_print (send_msg);
#endif
    zerr = zmsg_send (&send_msg, server_args->reply_to);
    ASSERT_TEST(zerr == 0, "Could not send message", err_send_msg);

err_send_msg:
err_ret_code:
err_reply_code:
    zmsg_destroy (&send_msg);
err_send_msg_alloc:
    zframe_destroy (&endpoint);
err_endpoint_alloc:
    /* Might not be safe to do this if we fail */
    zmsg_destroy (server_args->msg);
    return NULL;
}

/**** Release device ****/
void *thsafe_zmq_server_release (void *owner, void *args)
{
    assert (owner);
    assert (args);

    devio_t *self = (devio_t *) owner;
    zmq_server_args_t *server_args = (zmq_server_args_t *) args;

    DBE_DEBUG (DBG_MSG | DBG_LVL_TRACE, "[smio_thsafe_server:zmq] Calling thsafe_release\n");
    DBE_DEBUG (DBG_MSG | DBG_LVL_TRACE, "[smio_thsafe_server:zmq] Received message:\n");
#ifdef LOCAL_MSG_DBG
    zmsg_print (*server_args->msg);
#endif
    /* Message is:
     * frame null: OPEN opcode (removed by dev_io)
     * frame 0: endpopint struct (FIXME?) */
    zframe_t *endpoint = zmsg_pop (*server_args->msg);
    ASSERT_ALLOC(endpoint, err_endpoint_alloc);

    /* Call llio to actually perform the
     * operation */
    int32_t ret = llio_release (self->llio,
            (llio_endpoint_t *) zframe_data (endpoint));

    /* Send reply back to client */
    zmsg_t *send_msg = zmsg_new ();
    ASSERT_ALLOC(send_msg, err_send_msg_alloc);

    THSAFE_REPLY_TYPE reply_code = THSAFE_OK;
    if (ret != 0) {
        reply_code = THSAFE_ERR;
    }

    int zerr = zmsg_addmem (send_msg, &reply_code, sizeof (reply_code));
    ASSERT_TEST(zerr == 0, "Could not add reply code in message", err_reply_code);
    zerr = zmsg_addmem (send_msg, &ret, sizeof (ret));
    ASSERT_TEST(zerr == 0, "Could not add return code in message", err_ret_code);
    zerr = zmsg_send (&send_msg, server_args->reply_to);
    ASSERT_TEST(zerr == 0, "Could not send message", err_send_msg);

err_send_msg:
err_ret_code:
err_reply_code:
    zmsg_destroy (&send_msg);
err_send_msg_alloc:
    zframe_destroy (&endpoint);
err_endpoint_alloc:
    /* Might not be safe to do this if we fail */
    zmsg_destroy (server_args->msg);
    return NULL;
}

/**** Read data from device ****/

void *thsafe_zmq_server_read_16 (void *owner, void *args)
{
    assert (owner);
    assert (args);

    devio_t *self = (devio_t *) owner;
    zmq_server_args_t *server_args = (zmq_server_args_t *) args;

    DBE_DEBUG (DBG_MSG | DBG_LVL_TRACE, "[smio_thsafe_server:zmq] Calling thsafe_read_16\n");
    loff_t offset = 0;
    int zerr = _thsafe_zmq_server_recv_read (*server_args->msg, &offset);
    ASSERT_TEST(zerr == 0, "Could receive message", err_recv_msg);

    /* Alloc space for the data read */
    uint16_t data = 0;
    /* Call llio to perform the actual operation */
    int32_t llio_ret = llio_read_16 (self->llio, offset, &data);

    /* Send reply back to client */
    _thsafe_zmq_server_send_read (&llio_ret, (uint8_t *) &data, sizeof (data),
            server_args->reply_to);

err_recv_msg:
    /* Might not be safe to do this if we fail */
    zmsg_destroy (server_args->msg);
    return NULL;
}

/* TODO: Reduce code repetition */
/* Changed only the necessary parameters out of laziness */
void *thsafe_zmq_server_read_32 (void *owner, void *args)
{
    assert (owner);
    assert (args);

    devio_t *self = (devio_t *) owner;
    zmq_server_args_t *server_args = (zmq_server_args_t *) args;

    DBE_DEBUG (DBG_MSG | DBG_LVL_TRACE, "[smio_thsafe_server:zmq] Calling thsafe_read_32\n");
    loff_t offset = 0;
    int zerr = _thsafe_zmq_server_recv_read (*server_args->msg, &offset);
    ASSERT_TEST(zerr == 0, "Could receive message", err_recv_msg);

    /* Alloc space for the data read */
    uint32_t data = 0;
    /* Call llio to perform the actual operation */
    int32_t llio_ret = llio_read_32 (self->llio, offset, &data);

    /* Send message back to client */
    _thsafe_zmq_server_send_read (&llio_ret, (uint8_t *) &data, sizeof (data),
            server_args->reply_to);

err_recv_msg:
    /* Might not be safe to do this if we fail */
    zmsg_destroy (server_args->msg);
    return NULL;
}

/* TODO: Reduce code repetition */
/* Changed only the necessary parameters out of laziness */
void *thsafe_zmq_server_read_64 (void *owner, void *args)
{
    assert (owner);
    assert (args);

    devio_t *self = (devio_t *) owner;
    zmq_server_args_t *server_args = (zmq_server_args_t *) args;

    DBE_DEBUG (DBG_MSG | DBG_LVL_TRACE, "[smio_thsafe_server:zmq] Calling thsafe_read_64\n");
    loff_t offset = 0;
    int zerr = _thsafe_zmq_server_recv_read (*server_args->msg, &offset);
    ASSERT_TEST(zerr == 0, "Could receive message", err_recv_msg);

    /* Alloc space for the data read */
    uint64_t data = 0;
    /* Call llio to perform the actual operation */
    int32_t llio_ret = llio_read_64 (self->llio, offset, &data);

    /* Send message back to client */
    _thsafe_zmq_server_send_read (&llio_ret, (uint8_t *) &data, sizeof (data),
            server_args->reply_to);

err_recv_msg:
    /* Might not be safe to do this if we fail */
    zmsg_destroy (server_args->msg);
    return NULL;
}

/**** Write data to device ****/

void *thsafe_zmq_server_write_16 (void *owner, void *args)
{
    assert (owner);
    assert (args);

    devio_t *self = (devio_t *) owner;
    zmq_server_args_t *server_args = (zmq_server_args_t *) args;

    DBE_DEBUG (DBG_MSG | DBG_LVL_TRACE, "[smio_thsafe_server:zmq] Calling thsafe_write_16\n");

    zframe_t *data_write_frame;
    loff_t offset = 0;
    int zerr = _thsafe_zmq_server_recv_write (*server_args->msg, &offset,
            &data_write_frame);
    ASSERT_TEST(zerr == 0, "Could receive message", err_recv_msg);

    if (zframe_size (data_write_frame) != THSAFE_WRITE_16_DSIZE) {
        DBE_DEBUG (DBG_MSG | DBG_LVL_ERR, "[smio_thsafe_server:zmq] Wrong received data size\n");
        goto err_wrong_data_write_size;
    }

    /* Call llio to perform the actual operation */
    int32_t llio_ret = llio_write_16 (self->llio, offset, (uint16_t *) zframe_data (data_write_frame));

    /* Send message back to client */
    _thsafe_zmq_server_send_write (&llio_ret, server_args->reply_to);

err_wrong_data_write_size:
    zframe_destroy (&data_write_frame);
err_recv_msg:
    /* Might not be safe to do this if we fail */
    zmsg_destroy (server_args->msg);
    return NULL;
}

void *thsafe_zmq_server_write_32 (void *owner, void *args)
{
    assert (owner);
    assert (args);

    devio_t *self = (devio_t *) owner;
    zmq_server_args_t *server_args = (zmq_server_args_t *) args;

    DBE_DEBUG (DBG_MSG | DBG_LVL_TRACE, "[smio_thsafe_server:zmq] Calling thsafe_write_32\n");

    zframe_t *data_write_frame;
    loff_t offset = 0;
    int zerr = _thsafe_zmq_server_recv_write (*server_args->msg, &offset,
            &data_write_frame);
    ASSERT_TEST(zerr == 0, "Could receive message", err_recv_msg);

    if (zframe_size (data_write_frame) != THSAFE_WRITE_32_DSIZE) {
        DBE_DEBUG (DBG_MSG | DBG_LVL_ERR, "[smio_thsafe_server:zmq] Wrong received data size\n");
        goto err_wrong_data_write_size;
    }

    /* Call llio to perform the actual operation */
    int32_t llio_ret = llio_write_32 (self->llio, offset, (uint32_t *) zframe_data (data_write_frame));

    /* Send message back to client */
    _thsafe_zmq_server_send_write (&llio_ret, server_args->reply_to);

err_wrong_data_write_size:
    zframe_destroy (&data_write_frame);
err_recv_msg:
    /* Might not be safe to do this if we fail */
    zmsg_destroy (server_args->msg);
    return NULL;
}

void *thsafe_zmq_server_write_64 (void *owner, void *args)
{
    assert (owner);
    assert (args);

    devio_t *self = (devio_t *) owner;
    zmq_server_args_t *server_args = (zmq_server_args_t *) args;

    DBE_DEBUG (DBG_MSG | DBG_LVL_TRACE, "[smio_thsafe_server:zmq] Calling thsafe_write_64\n");

    zframe_t *data_write_frame;
    loff_t offset = 0;
    int zerr = _thsafe_zmq_server_recv_write (*server_args->msg, &offset,
            &data_write_frame);
    ASSERT_TEST(zerr == 0, "Could receive message", err_recv_msg);

    if (zframe_size (data_write_frame) != THSAFE_WRITE_64_DSIZE) {
        DBE_DEBUG (DBG_MSG | DBG_LVL_ERR, "[smio_thsafe_server:zmq] Wrong received data size\n");
        goto err_wrong_data_write_size;
    }

    /* Call llio to perform the actual operation */
    int32_t llio_ret = llio_write_64 (self->llio, offset, (uint64_t *) zframe_data (data_write_frame));

    /* Send message back to client */
    _thsafe_zmq_server_send_write (&llio_ret, server_args->reply_to);

err_wrong_data_write_size:
    zframe_destroy (&data_write_frame);
err_recv_msg:
    /* Might not be safe to do this if we fail */
    zmsg_destroy (server_args->msg);
    return NULL;
}

/**** Read data block from device function pointer, size in bytes ****/
void *thsafe_zmq_server_read_block (void *owner, void *args)
{
    (void) owner;
    (void) args;
    return NULL;
}

/**** Write data block from device function pointer, size in bytes ****/
void *thsafe_zmq_server_write_block (void *owner, void *args)
{
    (void) owner;
    (void) args;
    return NULL;
}

/**** Read data block via DMA from device, size in bytes ****/
void *thsafe_zmq_server_read_dma (void *owner, void *args)
{
    (void) owner;
    (void) args;
    return NULL;
}

/**** Write data block via DMA from device, size in bytes ****/
void *thsafe_zmq_server_write_dma (void *owner, void *args)
{
    (void) owner;
    (void) args;
    return NULL;
}

/**** Read device information function pointer ****/
/* void *thsafe_zmq_server_read_info (void *owner, void *args)
 *{
 *  (void) owner;
 *  (void) args;
 *  return NULL;
 *} */

/*************** Helper functions **************/

static int _thsafe_zmq_server_send_read (int32_t *llio_ret, uint8_t *data, uint32_t size, void *reply_to)
{
    assert (llio_ret);
    int ret = 0;    /* success */

    /* Send reply back to client */
    zmsg_t *send_msg = zmsg_new ();
    ret = (send_msg == NULL) ? -1 : ret;
    ASSERT_ALLOC(send_msg, err_send_msg_alloc);

    THSAFE_REPLY_TYPE reply_code = THSAFE_OK;
    if (*llio_ret < 0) {
        reply_code = THSAFE_ERR;
    }

    /* Message is:
     * frame 0: reply code
     * frame 1: return code
     * frame 2: data read */
    int zerr = zmsg_addmem (send_msg, &reply_code, sizeof (reply_code));
    ASSERT_TEST(zerr == 0, "Could not add reply code in message", err_reply_code);
    zerr = zmsg_addmem (send_msg, llio_ret, sizeof (*llio_ret));
    ASSERT_TEST(zerr == 0, "Could not add number of bytes read in message", err_ret_code);
    zerr = zmsg_addmem (send_msg, data, size);
    ASSERT_TEST(zerr == 0, "Could not add read data in message", err_data_send);

    DBE_DEBUG (DBG_MSG | DBG_LVL_TRACE, "[smio_thsafe_server:zmq] Sending message:\n");
#ifdef LOCAL_MSG_DBG
    zmsg_print (send_msg);
#endif
    zerr = zmsg_send (&send_msg, reply_to);
    ASSERT_TEST(zerr == 0, "Could not send message", err_send_msg);

err_send_msg:
err_data_send:
err_ret_code:
err_reply_code:
    zmsg_destroy (&send_msg);
err_send_msg_alloc:
    return ret;
}

static int _thsafe_zmq_server_recv_read (zmsg_t *msg, loff_t *offset)
{
    assert (offset);
    DBE_DEBUG (DBG_MSG | DBG_LVL_TRACE, "[smio_thsafe_server:zmq:read] Received message:\n");
#ifdef LOCAL_MSG_DBG
    zmsg_print (msg);
#endif

    /* Message is:
     * frame null: READ opcode
     * frame 0: offset */
    zframe_t *offset_frame = zmsg_pop (msg);
    ASSERT_ALLOC(offset, err_offset_alloc);

    if (zframe_size (offset_frame) != sizeof (loff_t)) {
        goto err_wrong_offset_size;
    }

    *offset = *(loff_t *) zframe_data (offset_frame);
    zframe_destroy (&offset_frame);
    return 0;

err_wrong_offset_size:
err_offset_alloc:
    zframe_destroy (&offset_frame);
    return -1;
}

static int _thsafe_zmq_server_send_write (int32_t *llio_ret, void *reply_to)
{
    assert (llio_ret);
    int ret = 0;    /* success */

    /* Send reply back to client */
    zmsg_t *send_msg = zmsg_new ();
    ret = (send_msg == NULL) ? -1 : ret;
    ASSERT_ALLOC(send_msg, err_send_msg_alloc);

    THSAFE_REPLY_TYPE reply_code = THSAFE_OK;
    if (*llio_ret < 0) {
        reply_code = THSAFE_ERR;
    }

    /* Message is:
     * frame 0: reply code
     * frame 1: return code */
    int zerr = zmsg_addmem (send_msg, &reply_code, sizeof (reply_code));
    ASSERT_TEST(zerr == 0, "Could not add reply code in message", err_reply_code);
    zerr = zmsg_addmem (send_msg, llio_ret, sizeof (*llio_ret));
    ASSERT_TEST(zerr == 0, "Could not add number of bytes read in message", err_ret_code);

    DBE_DEBUG (DBG_MSG | DBG_LVL_TRACE, "[smio_thsafe_server:zmq] Sent message:\n");
#ifdef LOCAL_MSG_DBG
    zmsg_print (send_msg);
#endif
    zerr = zmsg_send (&send_msg, reply_to);
    ASSERT_TEST(zerr == 0, "Could not send message", err_send_msg);

err_send_msg:
err_ret_code:
err_reply_code:
    zmsg_destroy (&send_msg);
err_send_msg_alloc:
    return ret;
}

static int _thsafe_zmq_server_recv_write (zmsg_t *msg, loff_t *offset,
        zframe_t **data_write_frame)
{
    assert (offset);
    assert (data_write_frame);
    assert (*data_write_frame);

    DBE_DEBUG (DBG_MSG | DBG_LVL_TRACE, "[smio_thsafe_server:zmq] Received message:\n");
#ifdef LOCAL_MSG_DBG
    zmsg_print (msg);
#endif
    /* Message is:
     * frame null: WRITE opcode
     * frame 0: offset
     * frame 1: data to be written */
    zframe_t *offset_frame = zmsg_pop (msg);
    ASSERT_ALLOC(offset, err_offset_alloc);
    *data_write_frame = zmsg_pop (msg);
    ASSERT_ALLOC(*data_write_frame, err_data_write_alloc);

    *offset = *(loff_t *) zframe_data (offset_frame);
    zframe_destroy (&offset_frame);
    return 0;

err_data_write_alloc:
    zframe_destroy (&offset_frame);
err_offset_alloc:
    return -1;
}

/*************** Our constant structure **************/
const smio_thsafe_server_ops_t smio_thsafe_zmq_server_ops = {
    .thsafe_server_open           = thsafe_zmq_server_open,        /* Open device */
    .thsafe_server_release        = thsafe_zmq_server_release,     /* Release device */
    .thsafe_server_read_16        = thsafe_zmq_server_read_16,     /* Read 16-bit data */
    .thsafe_server_read_32        = thsafe_zmq_server_read_32,     /* Read 32-bit data */
    .thsafe_server_read_64        = thsafe_zmq_server_read_64,     /* Read 64-bit data */
    .thsafe_server_write_16       = thsafe_zmq_server_write_16,    /* Write 16-bit data */
    .thsafe_server_write_32       = thsafe_zmq_server_write_32,    /* Write 32-bit data */
    .thsafe_server_write_64       = thsafe_zmq_server_write_64,    /* Write 64-bit data */
    .thsafe_server_read_block     = thsafe_zmq_server_read_block,  /* Read arbitrary block size data,
                                           parameter size in bytes */
    .thsafe_server_write_block    = thsafe_zmq_server_write_block, /* Write arbitrary block size data,
                                           parameter size in bytes */
    .thsafe_server_read_dma       = thsafe_zmq_server_read_dma,    /* Read arbitrary block size data via DMA,
     _                                       parameter size in bytes */
    .thsafe_server_write_dma      = thsafe_zmq_server_write_dma    /* Write arbitrary block size data via DMA,
                                            parameter size in bytes */
    /*.thsafe_server_read_info      = thsafe_zmq_server_read_info */   /* Read device information data */
};
