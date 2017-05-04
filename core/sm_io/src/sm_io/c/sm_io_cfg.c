/*
 * Copyright (C) 2017 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "halcs_server.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, SM_IO, "[sm_io_cfg_defaults]",               \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)                  \
    ASSERT_HAL_ALLOC(ptr, SM_IO, "[sm_io_cfg_defaults]",                       \
            smio_err_str(SMIO_ERR_ALLOC),                                      \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                               \
    CHECK_HAL_ERR(err, SM_IO, "[sm_io_cfg_defaults]",                          \
            smio_err_str (err_type))

#define SMIO_CFG_DEFAULTS_POLLER_TIMEOUT                100000     /* in msec */
#define SMIO_CFG_DEFAULTS_POLLER_NTIMES                 1          /* execute and exit */

/* Main class object that every sm_io_cfg_defaults must implement */
struct _smio_cfg_t {
    char *service;                      /* Exported service name */
    char *inst_id_str;                  /* Instance ID string */
    zloop_t *loop;                      /* Reactor for server sockets */
    zsock_t *pipe;                      /* Pipe back to parent to exchange Management messages */
    int timer_id;                       /* Timer ID */
    char *broker;                       /* Broker endpoint */
    char *log_file;                     /* Logfile */
    volatile const smio_mod_dispatch_t
        *smio_mod_dispatch;             /* SMIO handler table entry */
};

static const char *_smio_cfg_get_service (smio_cfg_t *self);
static char *_smio_cfg_clone_service (smio_cfg_t *self);

static smio_err_e _smio_cfg_engine_handle_socket (smio_cfg_t *smio_cfg, void *sock,
        zloop_reader_fn handler);
static int _smio_cfg_handle_timer (zloop_t *loop, int timer_id, void *arg);
static int _smio_cfg_handle_pipe (zloop_t *loop, zsock_t *reader, void *args);

/* Boot new SMIO instance. Better used as a thread (CZMQ actor) init function */
smio_cfg_t *smio_cfg_new (th_config_args_t *args,
        volatile const smio_mod_dispatch_t *smio_mod_dispatch,
        zsock_t *pipe, char *service, char *inst_id_str)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io_cfg_defaults_bootstrap] "
            "Initializing CFG SMIO\n");
    smio_cfg_t *self = (smio_cfg_t *) zmalloc (sizeof *self);
    ASSERT_ALLOC(self, err_self_alloc);

    /* Setup exported service name */
    self->service = strdup (service);
    ASSERT_ALLOC(self->service, err_service_alloc);
    self->inst_id_str = strdup (inst_id_str);
    ASSERT_ALLOC(self->inst_id_str, err_inst_id_str_alloc);
    self->broker = strdup (args->broker);
    ASSERT_ALLOC(self->broker, err_broker_alloc);
    self->log_file = strdup (args->log_file);
    ASSERT_ALLOC(self->log_file, err_log_file_alloc);

    self->smio_mod_dispatch = smio_mod_dispatch;
    self->pipe = pipe;

    /* Setup loop */
    self->loop = zloop_new ();
    ASSERT_ALLOC(self->loop, err_loop_alloc);

    /* Execute config defaults once on constructor */
    SMIO_DISPATCH_FUNC_WRAPPER_GEN(config_defaults, self->smio_mod_dispatch,
            self->broker, self->service, self->log_file);

    /* Set loop timeout. We exit the config_defaults actor if no message
     * incomes before the timer expires */
    self->timer_id = zloop_timer (self->loop, SMIO_CFG_DEFAULTS_POLLER_TIMEOUT,
            SMIO_CFG_DEFAULTS_POLLER_NTIMES, _smio_cfg_handle_timer, self);
    ASSERT_TEST(self->timer_id != -1, "Could not create zloop timer", err_timer_alloc);

    return self;

err_timer_alloc:
    zloop_destroy (&self->loop);
err_loop_alloc:
    free (self->log_file);
err_log_file_alloc:
    free (self->broker);
err_broker_alloc:
    free (self->inst_id_str);
err_inst_id_str_alloc:
    free (self->service);
err_service_alloc:
    free (self);
err_self_alloc:
    return NULL;
}

smio_err_e smio_cfg_destroy (smio_cfg_t **self_p)
{
    assert (self_p);

    if (*self_p) {
        smio_cfg_t *self = *self_p;

        zloop_timer_end (self->loop, self->timer_id);
        zloop_destroy (&self->loop);
        /* Don't destroy pipe as this is taken care of by the
         * zactor infrastructure, s_thread_shim (void *args) on CZMQ
         * 3.0.2 src/zactor.c
         * zsock_destroy (&self->pipe);
         */
        free (self->log_file);
        free (self->broker);
        free (self->inst_id_str);
        free (self->service);

        free (self);
        *self_p = NULL;
    }

    return SMIO_SUCCESS;
}

/* From Malamute https://github.com/zeromq/malamute/blob/master/src/mlm_server_engine.inc
 *
 * Poll actor or zsock for activity, invoke handler on any received
 * message. Handler must be a CZMQ zloop_fn function; receives server
 * as arg. */

static smio_err_e _smio_cfg_engine_handle_socket (smio_cfg_t *smio_cfg, void *sock,
        zloop_reader_fn handler)
{
    smio_err_e err = SMIO_SUCCESS;

    if (smio_cfg) {
        smio_cfg_t *self = (smio_cfg_t *) smio_cfg;

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

/* zloop handler for timer when it expires */
static int _smio_cfg_handle_timer (zloop_t *loop, int timer_id, void *arg)
{
    UNUSED(loop);
    UNUSED(timer_id);

    /* We expect a smio instance as reference */
    smio_cfg_t *smio_cfg = (smio_cfg_t *) arg;

    /* No activity on socket means we have finished configuring and
     * no one wants to reconfigure the SMIO. Tell DEVIO we are done */
    char *smio_service_suffix = hutils_concat_strings_no_sep (
            smio_cfg->smio_mod_dispatch->name, smio_cfg->inst_id_str);
    ASSERT_ALLOC(smio_service_suffix, err_smio_service_suffix_alloc);

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO, "[sm_io_bootstrap] Sending CONFIG DONE "
            "message over PIPE due to socket inactivity\n");

    int zerr = zstr_sendx (smio_cfg->pipe, smio_service_suffix, "CONFIG DONE", NULL);
    ASSERT_TEST (zerr >= 0, "Config thread could not send CONFIG DONE message "
            "over PIPE. Destroying ourselves", err_send_config_done);
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO, "[sm_io_bootstrap] Config Thread %s "
            "sent CONFIG DONE over PIPE\n", smio_cfg->service);

err_send_config_done:
    free (smio_service_suffix);
err_smio_service_suffix_alloc:
    return 0;
}

/* zloop handler for PIPE */
static int _smio_cfg_handle_pipe (zloop_t *loop, zsock_t *reader, void *args)
{
    UNUSED(loop);

    /* Arguments for command */
    char *command = NULL;

    /* We expect a smio instance as reference */
    smio_cfg_t *smio_cfg = (smio_cfg_t *) args;

    /* Receive message */
    zmsg_t *recv_msg = zmsg_recv (reader);
    if (recv_msg == NULL) {
        return -1; /* Interrupted */
    }

    command = zmsg_popstr (recv_msg);
    if (streq (command, "$TERM")) {
        /* Shutdown the engine */
        free (command);
        zmsg_destroy (&recv_msg);
        return -1;
    }
    else if (streq (command, "RECONFIGURE SMIO")) {
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO, "[sm_io_bootstrap] "
                "Reconfiguring SMIO %s as it was requested by command "
                "\"RECONFIGURE SMIO\"\n", smio_cfg->service);
        SMIO_DISPATCH_FUNC_WRAPPER_GEN(config_defaults, smio_cfg->smio_mod_dispatch,
                smio_cfg->broker, smio_cfg->service, smio_cfg->log_file);
    }
    /* Invalid message received. Log the error, but continue normally */
    else {
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_WARN, "[sm_io_bootstrap] Config Thread %s "
                "received an invalid command over PIPE\n", smio_cfg->service);
    }

    free (command);
    zmsg_destroy (&recv_msg);
    return 0;
}

smio_err_e smio_cfg_loop (smio_cfg_t *self)
{
    assert (self);
    smio_err_e err = SMIO_SUCCESS;

    /* Set-up server register commands handler */
    _smio_cfg_engine_handle_socket (self, self->pipe, _smio_cfg_handle_pipe);

    /* Run reactor until there's a termination signal */
    zloop_start (self->loop);

    return err;
}

/************************************************************/
/********************* Accessor methods *********************/
/************************************************************/

/* Get SMIO service */
const char *smio_cfg_get_service (smio_cfg_t *self)
{
    assert (self);
    return _smio_cfg_get_service (self);
}

/* Clone SMIO service */
char *smio_cfg_clone_service (smio_cfg_t *self)
{
    assert (self);
    return _smio_cfg_clone_service (self);
}

zsock_t *smio_cfg_get_pipe (smio_cfg_t *self)
{
    return self->pipe;
}

/**************** Static Functions ***************/

/* Get SMIO service */
static const char *_smio_cfg_get_service (smio_cfg_t *self)
{
    assert (self);
    return self->service;
}

/* Clone SMIO service */
static char *_smio_cfg_clone_service (smio_cfg_t *self)
{
    assert (self);

    char *service = strdup (self->service);
    ASSERT_ALLOC(service, err_alloc);

err_alloc:
    return service;
}

