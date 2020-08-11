/*
 * Copyright (C) 2020 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _DEV_IO_PROTO_H_
#define _DEV_IO_PROTO_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Create message instance */
devio_proto_t *devio_proto_new (void);
/* Destroy message instance */
void devio_proto_destroy (devio_proto_t **self_p);
/* Recv message */
devio_err_e devio_proto_recv (devio_proto_t *self, zsock_t *input);
/* Send message */
devio_err_e devio_proto_send (devio_proto_t *self, zsock_t *output);
/* Get routing ID */
zframe_t *devio_proto_get_routing_id (devio_proto_t *self);
/* Set routing ID */
devio_err_e devio_proto_set_routing_id (devio_proto_t *self, zframe_t *routing_id);
/* Get content */
zmsg_t *devio_proto_get_content (devio_proto_t *self);
/* Get content transferring ownership */
zmsg_t *devio_proto_get_content_move (devio_proto_t *self);
/* Set content transferring ownership */
devio_err_e devio_proto_set_content_move (devio_proto_t *self, zmsg_t **msg_p);

#ifdef __cplusplus
}
#endif

#endif
