/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "halcs_server.h"
/* Private headers */
#include "sm_io_dsp_core.h"
#include "hw/wb_pos_calc_regs.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, SM_IO, "[sm_io_dsp_core]", \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_ALLOC(ptr, SM_IO, "[sm_io_dsp_core]", \
            smio_err_str(SMIO_ERR_ALLOC),                   \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                    \
    CHECK_HAL_ERR(err, SM_IO, "[sm_io_dsp_core]",   \
            smio_err_str (err_type))

#define SMIO_DSP_POLLER_TIMEOUT                4     /* in msec */
#define SMIO_DSP_POLLER_NTIMES                 0     /* 0 for infinte */

static smio_err_e _smio_dsp_monit_engine_handle_socket (smio_dsp_monit_t *smio_dsp_monit, void *sock,
        zloop_reader_fn handler);
static int _smio_dsp_monit_handle_timer (zloop_t *loop, int timer_id, void *arg);
static int _smio_dsp_monit_handle_pipe_mgmt (zloop_t *loop, zsock_t *reader, void *args);
static void _smio_dsp_monit (zsock_t *pipe_mgmt, void *th_boot_args);

/* Thread boot args structure */
typedef struct {
    smio_t *parent;                                         /* SMIO parent */
    zsock_t *dealer;                                        /* Aditional Message PIPE to DEVIO */
    const char *endpoint;                                   /* Endpoint to connect to broker */
    const char *service;                                    /* (part of) the service name to be exported */
} th_dsp_boot_args_t;

/************************************************************/
/****************     Our DSP structure      ****************/
/************************************************************/

/* Creates a new instance of Device Information */
smio_dsp_t * smio_dsp_new (smio_t *parent)
{
    smio_dsp_t *self = (smio_dsp_t *) zmalloc (sizeof *self);
    ASSERT_ALLOC(self, err_self_alloc);
    
    /* Alloacate thread arguments struct and pass it to the
     * thread. It is the responsability of the calling thread
     * to clear this structure after using it! */
    th_dsp_boot_args_t *th_args = zmalloc (sizeof *th_args);
    ASSERT_ALLOC (th_args, err_th_args_alloc);
    th_args->parent = parent;
    th_args->dealer = smio_get_dealer2(parent);
    th_args->endpoint = smio_get_endpoint(parent);
    th_args->service = smio_get_service(parent);

    /* Create actor */
    self->actor = zactor_new (_smio_dsp_monit, th_args);
    ASSERT_ALLOC(self->actor, err_actor_alloc);

    return self;

err_actor_alloc:
    free (th_args);
err_th_args_alloc:
    free (self);
err_self_alloc:
    return NULL;
}

/* Destroy an instance of the Device Information */
smio_err_e smio_dsp_destroy (smio_dsp_t **self_p)
{
    assert (self_p);

    if (*self_p) {
        smio_dsp_t *self = *self_p;

        zactor_destroy (&self->actor);
        free (self);
        *self_p = NULL;
    }

    return SMIO_SUCCESS;
}

zactor_t *smio_dsp_get_actor (smio_dsp_t *self)
{
    assert (self);
    return self->actor;
}


/************************************************************/
/************** Our Monit Actor structure    ****************/
/************************************************************/

/* Creates a new instance of Device Information */
smio_dsp_monit_t* smio_dsp_monit_new (zsock_t *pipe_mgmt, zsock_t *dealer,
    smio_t *parent, const char *endpoint, const char *service)
{
    smio_dsp_monit_t *self = (smio_dsp_monit_t *) zmalloc (sizeof *self);
    ASSERT_ALLOC(self, err_self_alloc);

    self->parent = parent;
    self->pipe_mgmt = pipe_mgmt;
    self->dealer = dealer;

    /* Set monit update time */
    self->monit_poll_time = SMIO_DSP_POLLER_TIMEOUT;

    /* Setup loop */
    self->loop = zloop_new ();
    ASSERT_ALLOC(self->loop, err_loop_alloc);

    /* Set loop timeout. This will poll Monit registers and send them to
     * connected clients */
    self->timer_id = zloop_timer (self->loop, self->monit_poll_time, SMIO_DSP_POLLER_NTIMES,
        _smio_dsp_monit_handle_timer, self);
    ASSERT_TEST(self->timer_id != -1, "Could not create zloop timer", err_timer_alloc);

    self->producer = mlm_client_new ();
    ASSERT_ALLOC(self->producer, err_producer_alloc);

    int rc = mlm_client_connect (self->producer, endpoint, 1000, service);
    ASSERT_TEST(rc >= 0, "Could not connect DSP MLM producer to broker", err_mlm_producer_connect);

    /* Set producer channel */
    rc = mlm_client_set_producer (self->producer, service);
    ASSERT_TEST(rc >= 0, "Could not set DSP MLM producer channel name", err_mlm_producer_channel);

    return self;

err_mlm_producer_channel:
err_mlm_producer_connect:
    mlm_client_destroy (&self->producer);
err_producer_alloc:
    zloop_timer_end (self->loop, self->timer_id);
err_timer_alloc:
    zloop_destroy (&self->loop);
err_loop_alloc:
    /* Already destroyed at sm_io.c */
    /* zsock_destroy (&self->dealer); */
    free(self);
err_self_alloc:
    return NULL;
}

/* Destroy an instance of the Device Information */
smio_err_e smio_dsp_monit_destroy (smio_dsp_monit_t **self_p)
{
    assert (self_p);

    if (*self_p) {
        smio_dsp_monit_t *self = *self_p;

        mlm_client_destroy (&self->producer);
        zloop_timer_end (self->loop, self->timer_id);
        zloop_destroy (&self->loop);
        /* Already destroyed at sm_io.c */
        /* zsock_destroy (&self->dealer); */
        /* Don't destroy pipe_mgmt as this is taken care of by the
         * zactor infrastructure, s_thread_shim (void *args) on CZMQ
         * 3.0.2 src/zactor.c
         * zsock_destroy (&self->pipe_mgmt);
         */
        free (self);
        *self_p = NULL;
    }

    return SMIO_SUCCESS;
}

zsock_t *smio_dsp_monit_get_pipe_mgmt (smio_dsp_monit_t *self)
{
    assert (self);
    return self->pipe_mgmt;
}

zsock_t *smio_dsp_monit_get_dealer (smio_dsp_monit_t *self)
{
    assert (self);
    return self->dealer;
}

/*
 *  The picture can contain any
 *  of these characters, each corresponding to one or two arguments:
 *        i = int (signed)
 *        1 = uint8_t
 *        2 = uint16_t
 *        4 = uint32_t
 *        8 = uint64_t
 *        s = char *
 *        b = byte *, int (2 arguments)
 *        c = zchunk_t *
 *        f = zframe_t *
 *        h = zhashx_t *
 *        l = zlistx_t *
 *        U = zuuid_t *
 *        p = void * (sends the pointer value, only meaningful over inproc)
 *        m = zmsg_t * (sends all frames in the zmsg)
 *        z = sends zero-sized frame (0 arguments)
 *        u = uint (deprecated)
 *    Note that s, b, c, and f are encoded the same way and the choice is
 *    offered as a convenience to the sender, which may or may not already
 *    have data in a zchunk or zframe. Does not change or take ownership of
 *    any arguments. Returns 0 if successful, -1 if sending failed for any
 *    reason.
 */
smio_err_e smio_send_dsp_monit_mgmt_msg (zactor_t *actor, 
    const char *picture, ...)
{
    assert (actor);
    smio_err_e err = SMIO_SUCCESS;

    va_list argptr;
    va_start(argptr, picture);
    int zerr = zsock_vsend (actor, picture, argptr);
    va_end(argptr);
    ASSERT_TEST(zerr == 0, "Could not send MGMT message to DSP MONIT", 
        err_send_mgmt_msg,
        SMIO_ERR_INV_SOCKET);

err_send_mgmt_msg:
    return err;
}

smio_err_e smio_recv_dsp_monit_mgmt_msg (zactor_t *actor, 
    const char *picture, ...)
{
    assert (actor);
    smio_err_e err = SMIO_SUCCESS;

    va_list argptr;
    va_start(argptr, picture);
    int zerr = zsock_vrecv (actor, picture, argptr);
    va_end(argptr);
    ASSERT_TEST(zerr == 0, "Could not recv MGMT message from DSP MONIT", 
        err_recv_mgmt_msg,
        SMIO_ERR_INV_SOCKET);

err_recv_mgmt_msg:
    return err;
}

static smio_err_e _smio_dsp_monit_engine_handle_socket (smio_dsp_monit_t *smio_dsp_monit, void *sock,
        zloop_reader_fn handler)
{
    smio_err_e err = SMIO_SUCCESS;

    if (smio_dsp_monit) {
        smio_dsp_monit_t *self = (smio_dsp_monit_t *) smio_dsp_monit;

        /*  Resolve zactor_t -> zsock_t */
        if (zactor_is (sock)) {
            sock = zactor_sock ((zactor_t *) sock);
        }
        else {
            /* Socket reference must be of type zsock */
            ASSERT_TEST(zsock_is (sock), "Invalid socket reference",
                    err_zsock_is, SMIO_ERR_INV_SOCKET);
        }

        if (handler != NULL) {
            /* Register handler "handler "to socket "sock" and pass argument
             * "self" to the handler */
            int rc = zloop_reader (self->loop, (zsock_t *) sock, handler, self);
            ASSERT_TEST(rc == 0, "Could not register zloop_reader",
                    err_zloop_reader, SMIO_ERR_ALLOC);
            zloop_reader_set_tolerant (self->loop, (zsock_t *) sock);
        }
        else {
            zloop_reader_end (self->loop, (zsock_t *) sock);
        }
    }

err_zloop_reader:
err_zsock_is:
    return err;
}

/* zloop handler for timer */
static int _smio_dsp_monit_handle_timer (zloop_t *loop, int timer_id, void *arg)
{
    UNUSED(loop);
    UNUSED(timer_id);
    smio_dsp_monit_t *self = (smio_dsp_monit_t *) arg;

    uint32_t ampfifo_empty;
    RW_REPLY_TYPE err = GET_PARAM_GEN(self->parent, dsp, 0x0, POS_CALC, AMPFIFO_MONIT_CSR, EMPTY,
        SINGLE_BIT_PARAM, ampfifo_empty, NO_FMT_FUNC,
        smio_thsafe_client_read_32_gen, self->dealer);
    ASSERT_TEST(err == RW_OK, "Could not get ampfifo empty register", err_get_monit_amp, -1);

    zmsg_t *msg = NULL;
    smio_dsp_monit_data_t monit_data;
    if (!ampfifo_empty) {
        err = GET_PARAM_GEN(self->parent, dsp, 0x0, POS_CALC, AMPFIFO_MONIT_R0, AMP_CH0,
            MULT_BIT_PARAM, monit_data.amp_ch0, NO_FMT_FUNC,
            smio_thsafe_client_read_32_gen, self->dealer);
        ASSERT_TEST(err == RW_OK, "Could not get ampfifo amp0 register", err_get_monit_amp, -1);

        err = GET_PARAM_GEN(self->parent, dsp, 0x0, POS_CALC, AMPFIFO_MONIT_R1, AMP_CH1,
            MULT_BIT_PARAM, monit_data.amp_ch1, NO_FMT_FUNC,
            smio_thsafe_client_read_32_gen, self->dealer);
        ASSERT_TEST(err == RW_OK, "Could not get ampfifo amp1 register", err_get_monit_amp, -1);

        err = GET_PARAM_GEN(self->parent, dsp, 0x0, POS_CALC, AMPFIFO_MONIT_R2, AMP_CH2,
            MULT_BIT_PARAM, monit_data.amp_ch2, NO_FMT_FUNC,
            smio_thsafe_client_read_32_gen, self->dealer);
        ASSERT_TEST(err == RW_OK, "Could not get ampfifo amp2 register", err_get_monit_amp, -1);

        err = GET_PARAM_GEN(self->parent, dsp, 0x0, POS_CALC, AMPFIFO_MONIT_R3, AMP_CH3,
            MULT_BIT_PARAM, monit_data.amp_ch3, NO_FMT_FUNC,
            smio_thsafe_client_read_32_gen, self->dealer);
        ASSERT_TEST(err == RW_OK, "Could not get ampfifo amp3 register", err_get_monit_amp, -1);

        /* Send message */
        msg = zmsg_new ();
        ASSERT_ALLOC(msg, err_send_msg_alloc);

        int zerr = zmsg_addmem (msg, &monit_data, sizeof(monit_data));
        ASSERT_TEST(zerr==0, "Could not add MONIT data in message", err_addmem);

        int rc = mlm_client_send (self->producer, "MONIT_AMP", &msg);
        ASSERT_TEST(rc==0, "Could not send MONIT data message", err_send_msg);

        DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io_dsp_core:_smio_dsp_monit_handle_timer] Data sent:\n"
            "amp0 = %u, amp1 = %u, amp2 = %u, amp3 = %u\n", monit_data.amp_ch0, monit_data.amp_ch1,
            monit_data.amp_ch2, monit_data.amp_ch3);
    }

    return err;

err_send_msg:
err_addmem:
    zmsg_destroy (&msg);
err_send_msg_alloc:
err_get_monit_amp:
    return err;
}

/* zloop handler for CFG PIPE */
static int _smio_dsp_monit_handle_pipe_mgmt (zloop_t *loop, zsock_t *reader, void *args)
{
    UNUSED(loop);

    /* We expect a smio instance e as reference */
    smio_dsp_monit_t *self = (smio_dsp_monit_t *) args;

    /* Receive message */
    char *command = zstr_recv (reader);
    if (command == NULL) {
        return -1; /* Interrupted */
    }

    bool terminated = false;
    if (streq (command, "$TERM")) {
        /* Shutdown the engine */
        terminated = true;    
    }
    else if (streq (command, "MONIT_POLL_TIME_W")) {
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io_dsp_core:_smio_dsp_monit_handle_pipe_mgmt] PIPE "
               "received MONIT_POLL_TIME_W command.\n");
        /* Updte monit poll time */
        zsock_recv (reader, "4", &self->monit_poll_time);
        zloop_timer_end (self->loop, self->timer_id);
        self->timer_id = zloop_timer (self->loop, self->monit_poll_time, SMIO_DSP_POLLER_NTIMES,
            _smio_dsp_monit_handle_timer, self);
    }
    else if (streq (command, "MONIT_POLL_TIME_R")) {
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io_dsp_core:_smio_dsp_monit_handle_pipe_mgmt] PIPE "
               "received MONIT_POLL_TIME_R command.\n");
        /* Updte monit poll time */
        zsock_send (reader, "4", self->monit_poll_time);
    }

    /*  Cleanup pipe if any argument frames are still waiting to be eaten */
    if (zsock_rcvmore (reader)) {
        zmsg_t *more = zmsg_recv (reader);
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_WARN, "[sm_io_dsp_core:_smio_dsp_monit_handle_pipe_mgmt] PIPE "
               "received invalid frames, consuming them.\n");
        zmsg_destroy (&more);
    }

    zstr_free (&command);
    return terminated? -1: 0;
}

/* DSP Monit actor. Polls on its MGMT socket */
static void _smio_dsp_monit (zsock_t *pipe_mgmt, void *th_boot_args)
{
    th_dsp_boot_args_t *th_args = (th_dsp_boot_args_t *) th_boot_args;

    /* DSP MONIT service will be composed of the received service +
     * :DATA_PRODUCER */
    char *service_producer = hutils_concat_strings (th_args->service, "DATA_PRODUCER", ':');
    ASSERT_ALLOC(service_producer, err_service_prod_alloc);

    smio_dsp_monit_t *self = smio_dsp_monit_new (pipe_mgmt, th_args->dealer, 
        th_args->parent, th_args->endpoint, service_producer);
    if (self) {
        zsock_signal (pipe_mgmt, 0);

        /* Set up handler for the sockets it uses */
        _smio_dsp_monit_engine_handle_socket (self, self->pipe_mgmt, _smio_dsp_monit_handle_pipe_mgmt);
        /* _smio_dsp_monit_engine_handle_socket (self, self->dealer, _smio_dsp_monit_handle_dealer); */

        /* Run reactor until there's a termination signal */
        zloop_start (self->loop);

        /*  Reactor has ended */
        smio_dsp_monit_destroy (&self);
    }
    else {
        zsock_signal (pipe_mgmt, -1);
    }

    free (service_producer);
err_service_prod_alloc:
    /* Our responsability to clear this up */
    free (th_args);
}
