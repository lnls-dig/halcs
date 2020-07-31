/*
 * Copyright (C) 2020 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "halcs_server.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...)  \
    ASSERT_HAL_TEST(test_boolean, DEV_IO, "[dev_io_proto]",                     \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_ALLOC(ptr, DEV_IO, "[dev_io_proto]",           \
            devio_err_str(DEVIO_ERR_ALLOC),                   \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                              \
    CHECK_HAL_ERR(err, DEV_IO, "[dev_io_proto]",              \
            devio_err_str (err_type))

/* Based on https://github.com/zeromq/malamute/blob/master/src/mlm_proto.c */

struct _devio_proto_t {
    zframe_t *routing_id;               /*  Routing_id from ROUTER, if any */
    zmsg_t *content;                    /*  Message body frames */
};

devio_proto_t *devio_proto_new (void)
{
    devio_proto_t *self = (devio_proto_t *) zmalloc (sizeof (devio_proto_t));
    return self;
}

void devio_proto_destroy (devio_proto_t **self_p)
{
    assert (self_p);
    if (*self_p) {
        devio_proto_t *self = *self_p;

        /* Free class properties */
        zframe_destroy (&self->routing_id);
        zmsg_destroy (&self->content);

        /* Free object itself */
        free (self);
        *self_p = NULL;
    }
}

devio_err_e devio_proto_recv (devio_proto_t *self, zsock_t *input)
{
    assert (self);
    assert (input);
    devio_err_e err = DEVIO_SUCCESS;

    if (zsock_type (input) == ZMQ_ROUTER) {
        zframe_destroy (&self->routing_id);
        self->routing_id = zframe_recv (input);
        ASSERT_TEST (self->routing_id && zsock_rcvmore (input), 
            "No routing ID", err_recv_msg, DEVIO_ERR_BAD_MSG);
    }

    //  Get zero or more remaining frames
    zmsg_destroy (&self->content);
    if (zsock_rcvmore (input)) {
        self->content = zmsg_recv (input);
    }
    else {
        self->content = zmsg_new ();
    }

    ASSERT_TEST (self->content, "Could not reveive message", 
        err_recv_msg, DEVIO_ERR_BAD_MSG);
   
    return err;

err_recv_msg:
    return err;
}

devio_err_e devio_proto_send (devio_proto_t *self, zsock_t *output)
{
    assert (self);
    assert (output);
    devio_err_e err = DEVIO_SUCCESS;
    int rc = 0;

    if (zsock_type (output) == ZMQ_ROUTER) {
        rc = zframe_send (&self->routing_id, output, ZFRAME_MORE + ZFRAME_REUSE);
        ASSERT_TEST (rc >= 0, "Could not send routing ID message", 
            err_send_msg, DEVIO_ERR_BAD_MSG);
    }

    size_t nbr_frames = self->content? zmsg_size (self->content): 0;
    if (self->content) {
        zframe_t *frame = zmsg_first (self->content);
        while (frame) {
            zframe_send (&frame, output, ZFRAME_REUSE + (--nbr_frames? ZFRAME_MORE: 0));
            frame = zmsg_next (self->content);
        }
    }
    else {
        zmq_send (zsock_resolve (output), NULL, 0, 0);
    }
   
    return err;

err_send_msg:
    return err;
}

zframe_t *devio_proto_get_routing_id (devio_proto_t *self)
{
    assert (self);
    return self->routing_id;
}

devio_err_e devio_proto_set_routing_id (devio_proto_t *self, zframe_t *routing_id)
{
    assert (self);
    devio_err_e err = DEVIO_SUCCESS;

    if (self->routing_id) {
        zframe_destroy (&self->routing_id);
    }
    self->routing_id = zframe_dup (routing_id);

    return err;
}

zmsg_t *devio_proto_get_content (devio_proto_t *self)
{
    assert (self);
    return self->content;
}

zmsg_t *devio_proto_get_content_move (devio_proto_t *self)
{
    assert (self);
    zmsg_t *content = self->content;
    self->content = NULL;
    return content;
}

devio_err_e devio_proto_set_content_move (devio_proto_t *self, zmsg_t **msg_p)
{
    assert (self);
    assert (msg_p);
    devio_err_e err = DEVIO_SUCCESS;

    zmsg_destroy (&self->content);
    self->content = *msg_p;
    *msg_p = NULL;

    return err;
}
