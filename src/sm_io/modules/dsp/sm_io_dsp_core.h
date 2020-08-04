/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
*/

#ifndef _SM_IO_DSP_CORE_H_
#define _SM_IO_DSP_CORE_H_

typedef struct {
    zactor_t *actor;                    /*  Client actor */
} smio_dsp_t;

typedef struct {
    smio_t *parent;                     /* SMIO parent */
    mlm_client_t *producer;             /* MLM client */
    zsock_t *dealer;                    /* Pipe back to parent to exchange Payload messages */
    zsock_t *pipe_mgmt;                 /* Pipe back to parent to exchange Management messages */
    int timer_id;                       /* Timer ID */
    zloop_t *loop;                      /* Reactor for server sockets */
    uint32_t monit_poll_time;           /* poll time for polled operations */
} smio_dsp_monit_t;

/***************** Our methods *****************/

/* Creates a new instance of the smio realization */
smio_dsp_t * smio_dsp_new (smio_t *parent);
/* Destroys the smio realizationn */
smio_err_e smio_dsp_destroy (smio_dsp_t **self_p);
/* Returns monit actor */
zactor_t *smio_dsp_get_actor (smio_dsp_t *self);

/************** Our Monit Actor methods  ****************/

/* Creates a new instance of the smio_dsp_monit realization */
smio_dsp_monit_t* smio_dsp_monit_new (zsock_t *pipe_mgmt, zsock_t *dealer,
    smio_t *parent, const char *endpoint, const char *service);
/* Destroy an instance of the smio_dsp_monit */
smio_err_e smio_dsp_monit_destroy (smio_dsp_monit_t **self_p);
/* Returns pipe_mgmt */
zsock_t *smio_dsp_monit_get_pipe_mgmt (smio_dsp_monit_t *self);
/* Returns dealer */
zsock_t *smio_dsp_monit_get_dealer (smio_dsp_monit_t *self);
/* Send message to DSP MONIT MGMT PIPE*/
smio_err_e smio_send_dsp_monit_mgmt_msg (zactor_t *actor, 
    const char *picture, ...);
/* Recv message from DSP MONIT MGMT PIPE */
smio_err_e smio_recv_dsp_monit_mgmt_msg (zactor_t *actor, 
    const char *picture, ...);

#endif
