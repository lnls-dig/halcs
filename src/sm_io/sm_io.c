/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
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
    ASSERT_HAL_TEST(test_boolean, SM_IO, "[sm_io]",         \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_ALLOC(ptr, SM_IO, "[sm_io]",                 \
            smio_err_str(SMIO_ERR_ALLOC),                   \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                            \
    CHECK_HAL_ERR(err, SM_IO, "[sm_io]",                    \
            smio_err_str (err_type))

#define SMIO_RCV_TIMEOUT                   5000       /* ms */

/* Main class object that every sm_io must implement */
struct _smio_t {
    uint32_t id;                        /* Unique identifier for this sm_io type. This must be
                                           the same from the SDB ID */
    uint32_t inst_id;                   /* Instance ID. This differentiate SMIOs
                                           of the same type */
    uint64_t base;                      /* Base SMIO address */
    char *name;                         /* Identification of this sm_io instance */
    char *service;                      /* Exported service name */
    char *endpoint;                     /* Broker endpoint */
    /* int verbose; */                  /* Print activity to stdout */
    mlm_client_t *worker;               /* zeroMQ Malamute client (worker) */
    void *smio_handler;                 /* Generic pointer to a device handler. This
                                            must be cast to a specific type by the
                                            devices functions */
    zloop_t *loop;                      /* Reactor for server sockets */
    zsock_t *pipe_mgmt;                 /* Pipe back to parent to exchange Management messages */
    zsock_t *dealer;                    /* Socket to talk to DEVIO */
    zsock_t *dealer2;                   /* Socket 2 to talk to DEVIO */
    zsock_t *pipe_frontend;             /* Force zloop to interrupt and rebuild poll set. This is used to send messages */
    zsock_t *pipe_backend;              /* Force zloop to interrupt and rebuild poll set. This is used to receive messages */
    int timer_id;                       /* Timer ID */

    /* Specific SMIO operations dispatch table for exported operations */
    disp_table_t *exp_ops_dtable;
    /* Specific SMIO instance functions. This will be exported on SMIO startup */
    const disp_op_t **exp_ops;
    /* Minimum set of methods that every sm_io instance must implement.
         * This is thought as the interface to the world */
    const smio_ops_t *ops;
    /* These operations must be used to access LLIO methods, as they are
     * thread safe. The method's signatures are almost the same as the
     * ones available in llio, changing the llio_t self pointer to a void
     * pointer (socket to parent thread) */
    const smio_thsafe_client_ops_t *thsafe_client_ops;
};

/* SMIO dispatch table operations */
const disp_table_ops_t smio_disp_table_ops;

static smio_err_e _smio_do_op (void *owner, void *msg);
static smio_err_e _smio_do_mgmt_msg (void *owner, void *msg);
/* Dispatch table message check handler */
static disp_table_err_e _smio_check_msg_args (disp_table_t *disp_table,
        const disp_op_t *disp_op, void *args);
static smio_err_e _smio_set_name (smio_t *self, const char *name);
static const char *_smio_get_name (smio_t *self);
static char *_smio_clone_name (smio_t *self);
static const char *_smio_get_endpoint (smio_t *self);
static char *_smio_clone_endpoint (smio_t *self);
static const char *_smio_get_service (smio_t *self);
static char *_smio_clone_service (smio_t *self);

static smio_err_e _smio_engine_handle_socket (smio_t *smio, void *sock,
        zloop_reader_fn handler);
static int _smio_handle_pipe_backend (zloop_t *loop, zsock_t *reader, void *args);

/* Boot new SMIO instance. Better used as a thread (CZMQ actor) init function */
smio_t *smio_new (th_boot_args_t *args, zsock_t *pipe_mgmt,
        const char *devio_endpoint, char *service)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io] Initializing SMIO\n");
    smio_t *self = (smio_t *) zmalloc (sizeof *self);
    ASSERT_ALLOC(self, err_self_alloc);

    /* Will be filled by the SMIO service init function */
    self->name = NULL;

    /* Setup exported service name */
    self->service = strdup (service);
    ASSERT_ALLOC(self->service, err_service_alloc);

    /* Setup endpoint name */
    self->endpoint = strdup (args->broker);
    ASSERT_ALLOC(self->endpoint, err_endpoint_alloc);

    /* Setup Dispatch table */
    self->exp_ops_dtable = disp_table_new (&smio_disp_table_ops);
    ASSERT_ALLOC(self->exp_ops_dtable, err_exp_ops_dtable_alloc);

    self->smio_handler = NULL;      /* This is set by the device functions */
    self->pipe_mgmt = pipe_mgmt;
    self->dealer = zsock_new (ZMQ_DEALER);
    ASSERT_ALLOC(self->dealer, err_dealer_alloc);
    self->dealer2 = zsock_new (ZMQ_DEALER);
    ASSERT_ALLOC(self->dealer2, err_dealer2_alloc);
    int zerr = zsock_connect (self->dealer, "%s", devio_endpoint);
    ASSERT_TEST(zerr == 0, "Could not connect DEALER socket to DEVIO endpoint", err_dealer_connect);
    zerr = zsock_connect (self->dealer2, "%s", devio_endpoint);
    ASSERT_TEST(zerr == 0, "Could not connect DEALER 2 docker to DEVIO endpoint", err_dealer2_connect);

    self->inst_id = args->inst_id;

    /* Set maximum receive wait for PIPE MSG sockets. This is important
     * so as to not block DEVIO <-> SMIO communication if something is
     * wrong */
    zsock_set_rcvtimeo (self->dealer, SMIO_RCV_TIMEOUT);
    zsock_set_rcvtimeo (self->dealer2, SMIO_RCV_TIMEOUT);

    /* Setup pipes for zloop interrupting */
    self->pipe_frontend = zsys_create_pipe (&self->pipe_backend);
    ASSERT_ALLOC(self->pipe_frontend, err_pipe_frontend_alloc);

    /* Setup loop */
    self->loop = zloop_new ();
    ASSERT_ALLOC(self->loop, err_loop_alloc);

    /* Set-up backend handler for forcing interrupting the zloop and rebuild
     * the poll set. This avoids having to setup a short timer to periodically
     * interrupting the loop to check for rebuilds */
    _smio_engine_handle_socket (self, self->pipe_backend, _smio_handle_pipe_backend);

    /* Initialize SMIO base address */
    self->base = args->base;

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io] Creating worker\n");
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "\tbroker = %s, service = %s, verbose = %d\n",
            args->broker, service, args->verbose);
    self->worker = mlm_client_new ();
    ASSERT_ALLOC(self->worker, err_worker_alloc);
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io] Worker created\n");

    int rc = mlm_client_connect (self->worker, args->broker, 1000, service);
    ASSERT_TEST(rc >= 0, "Could not connect MLM worker to broker", err_mlm_worker_connect);

    return self;

err_mlm_worker_connect:
    mlm_client_destroy (&self->worker);
err_worker_alloc:
    zloop_destroy (&self->loop);
err_loop_alloc:
    zsock_destroy (&self->pipe_backend);
    zsock_destroy (&self->pipe_frontend);
err_pipe_frontend_alloc:
err_dealer2_connect:
err_dealer_connect:
    zsock_destroy (&self->dealer2);
err_dealer2_alloc:
    zsock_destroy (&self->dealer);
err_dealer_alloc:
    disp_table_destroy (&self->exp_ops_dtable);
err_exp_ops_dtable_alloc:
    free (self->endpoint);
err_endpoint_alloc:
    free (self->service);
err_service_alloc:
    free (self);
err_self_alloc:
    return NULL;
}

smio_err_e smio_destroy (smio_t **self_p)
{
    assert (self_p);

    if (*self_p) {
        smio_t *self = *self_p;

        mlm_client_destroy (&self->worker);
        zloop_timer_end (self->loop, self->timer_id);
        zloop_destroy (&self->loop);
        zsock_destroy (&self->pipe_backend);
        zsock_destroy (&self->pipe_frontend);
        zsock_destroy (&self->dealer2);
        zsock_destroy (&self->dealer);
        /* Don't destroy pipe_mgmt as this is taken care of by the
         * zactor infrastructure, s_thread_shim (void *args) on CZMQ
         * 3.0.2 src/zactor.c
         * zsock_destroy (&self->pipe_mgmt);
         */
        disp_table_destroy (&self->exp_ops_dtable);
        self->thsafe_client_ops = NULL;
        self->ops = NULL;
        free (self->endpoint);
        free (self->service);
        free (self->name);

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

static smio_err_e _smio_engine_handle_socket (smio_t *smio, void *sock,
        zloop_reader_fn handler)
{
    smio_err_e err = SMIO_SUCCESS;

    if (smio) {
        smio_t *self = (smio_t *) smio;

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

        /* Send message to pipe_backend to force zloop to rebuild poll_set */
        zstr_sendx (smio->pipe_frontend, "$REBUILD_POLL", NULL);
    }

err_zloop_reader:
err_zsock_is:
    return err;
}

/* zloop handler for CFG PIPE */
static int _smio_handle_pipe_mgmt (zloop_t *loop, zsock_t *reader, void *args)
{
    UNUSED(loop);

    smio_err_e err = SMIO_SUCCESS;
    /* We expect a smio instance e as reference */
    smio_t *smio = (smio_t *) args;
    UNUSED(smio);

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
    else if (streq (command, "$MGMT_MSG_SMIO")) {
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:_smio_handle_pipe_mgmt] PIPE "
               "received MGMT_MSG_SMIO command.\n");

        zmsg_t *recv_msg = zmsg_recv (reader);
        /* Call SMIO MGMT callback, passing a zmsg_t argument */
        err = _smio_do_mgmt_msg (smio, recv_msg);

        /* What can I do in case of error ?*/
        if (err != SMIO_SUCCESS) {
            DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE,
                    "_smio_handle_pipe_mgmt smio_do_mgmt_msg: %s\n",
                    smio_err_str (err));
        }
        zmsg_destroy (&recv_msg);
    }

    /*  Cleanup pipe if any argument frames are still waiting to be eaten */
    if (zsock_rcvmore (reader)) {
        zmsg_t *more = zmsg_recv (reader);
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_WARN, "[sm_io:_smio_handle_pipe_mgmt] PIPE "
               "received invalid frames, consuming them.\n");
        zmsg_destroy (&more);
    }

    zstr_free (&command);
    return terminated? -1: 0;
}

/* zloop handler for MSG PIPE */
static int _smio_handle_worker (zloop_t *loop, zsock_t *reader, void *args)
{
    UNUSED(loop);
    smio_err_e err = SMIO_SUCCESS;
    /* We expect a smio instance e as reference */
    smio_t *smio = (smio_t *) args;

    /* We process as many messages as we can, to reduce the overhead
     * of polling and the reactor */
    while (zsock_events (reader) & ZMQ_POLLIN) {
        zmsg_t *recv_msg = mlm_client_recv (smio->worker);
        if (recv_msg == NULL) {
            return -1; /* Interrupted */
        }

        exp_msg_zmq_t smio_args = {
            .tag = EXP_MSG_ZMQ_TAG,
            .msg = &recv_msg,
            .reply_to = NULL /* Unused field in MLM protocol */
        };
        err = smio_do_op (smio, &smio_args);

        /* What can I do in case of error ?*/
        if (err != SMIO_SUCCESS) {
            DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE,
                    "[sm_io] smio_do_op: %s\n",
                    smio_err_str (err));
        }

        /* Cleanup */
        zmsg_destroy (&recv_msg);
    }

    return 0;
}

/* zloop handler for PIPE backend */
static int _smio_handle_pipe_backend (zloop_t *loop, zsock_t *reader, void *args)
{
    UNUSED(loop);

    /* We expect a smio instance e as reference */
    smio_t *smio = (smio_t *) args;
    UNUSED(smio);

    /* Receive message */
    char *command = zstr_recv (reader);
    if (command == NULL) {
        return -1; /* Interrupted */
    }

    if (streq (command, "$REBUILD_POLL")) {
        /* If we are executing this is because zloop interrupted and will rebuild the poll
         * set as soon as this handler exits. So, we don't actually need to do anything
         * here */
        zstr_free (&command);
    }
    else {
        /* Invalid message received. Discard message and continue normally */
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_WARN, "[sm_io:_smio_handle_pipe_backend] PIPE "
                "received an invalid command\n");
    }

    /*  Cleanup pipe if any argument frames are still waiting to be eaten */
    if (zsock_rcvmore (reader)) {
        zmsg_t *more = zmsg_recv (reader);
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_WARN, "[sm_io:_smio_handle_pipe_backend] PIPE "
               "received invalid frames, consuming them.\n");
        zmsg_destroy (&more);
    }

    zstr_free (&command);
    return 0;
}

void smio_loop (zsock_t *pipe, void *args)
{
    /* FIXME: priv pointer is unused for now! We should use it to differentiate
     * between multiple smio instances of the same type controlling multiple
     * modules of the same type */
    th_boot_args_t *th_args = (th_boot_args_t *) args;
    const char *devio_endpoint = th_args->devio_endpoint;
    volatile const smio_mod_dispatch_t *smio_mod_dispatch = th_args->smio_handler;

    /* We must export our service as the combination of the
     * devio name (coming from devio parent) and our own name ID
     * followed by an optional parameter coming from priv pointer */
    char *inst_id_str = hutils_stringify_dec_key (th_args->inst_id);
    ASSERT_ALLOC(inst_id_str, err_inst_id_str_alloc);
    char *smio_service = hutils_concat_strings3 (th_args->service,
            smio_mod_dispatch->name, inst_id_str, ':');
    ASSERT_ALLOC(smio_service, err_smio_service_alloc);

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO, "[sm_io] SMIO Thread %s "
            "starting ...\n", smio_service);
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO, "[sm_io] SMIO Thread %s "
            "allocating resources ...\n", smio_service);

    smio_t *self = smio_new (th_args, pipe, devio_endpoint, smio_service);
    if (self) {
        /* Signal parent we are ready. It's important to signal the parent only
         * after we have register to the broker, as we must expire a possible
         * existing client with the same address before the config_defaults
         * actor send the messages to the old client reference.
         * However, we can't signal the parent after SMIO init function, as
         * it probably send messages to DEVIO/LLIO and we won't be able to poll
         * them if we are waiting here. */
        zsock_signal (pipe, 0);

        /* Atach this SMIO instance to its parent */
        smio_err_e err = smio_attach (self, th_args->args);
        ASSERT_TEST(err == SMIO_SUCCESS, "Could not attach SMIO", err_call_attach);

        /* Call SMIO init function to finish initializing its internal strucutres */
        err = SMIO_DISPATCH_FUNC_WRAPPER (init, smio_mod_dispatch);
        ASSERT_TEST(err == SMIO_SUCCESS, "Could not initialize SMIO", err_call_init);

        /* Export SMIO specific operations */
        const disp_op_t **smio_exp_ops = smio_get_exp_ops (self);
        ASSERT_TEST (smio_exp_ops != NULL, "Could not get SMIO exported operations",
                err_smio_get_exp_ops);

        err = smio_export_ops (self, smio_exp_ops);
        ASSERT_TEST (err == SMIO_SUCCESS, "Could not export specific SMIO operations",
                err_smio_export);

        /* Set-up server register commands handler */
        _smio_engine_handle_socket (self, self->pipe_mgmt, _smio_handle_pipe_mgmt);
        _smio_engine_handle_socket (self, mlm_client_msgpipe (self->worker), _smio_handle_worker);

        /* Run reactor until there's a termination signal */
        zloop_start (self->loop);

        /*  Reactor has ended */
        /* Unexport SMIO specific operations */
        smio_unexport_ops (self);
err_smio_export:
        /* Nullify exp ops */
        smio_set_exp_ops (self, NULL);
err_smio_get_exp_ops:
        /* FIXME: Poll PIPE sockets and on receiving any message calls shutdown () */
        SMIO_DISPATCH_FUNC_WRAPPER (shutdown, smio_mod_dispatch);
err_call_init:
        smio_deattach (self);
err_call_attach:
        /* Destroy what we did in _smio_new */
        smio_destroy (&self);
        /* We can't output this message at a later time as we depend on the smio_service
         * variable. This is not so bad, though, as most of the time we will not fail
         * in hutils_concat_strings () function */
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_WARN, "[sm_io] SMIO Thread %s exiting\n",
                smio_service);
    }
    else {
        zsock_signal (pipe, -1);
    }

    zstr_free (&smio_service);
err_smio_service_alloc:
    zstr_free (&inst_id_str);
err_inst_id_str_alloc:
    free (th_args);
}

smio_err_e smio_register_sm (smio_t *self, uint32_t smio_id, uint64_t base,
        uint32_t inst_id)
{
    assert (self);
    smio_err_e err = SMIO_SUCCESS;

    int zerr = zsock_send (self->pipe_mgmt, "s484", "$REGISTER_SMIO", smio_id, base,
            inst_id);
    ASSERT_TEST(zerr == 0, "Could not register SMIO", err_register_sm,
           SMIO_ERR_REGISTER_SM);

err_register_sm:
    return err;
}

smio_err_e smio_send_mgmt_msg (smio_t *self, uint32_t dest_smio_id,
    uint32_t dest_smio_inst, char *msg)
{
    assert (self);
    smio_err_e err = SMIO_SUCCESS;

    int zerr = zsock_send (self->pipe_mgmt, "s48444s", "$MGMT_MSG_SMIO",
            self->id, self->base, self->inst_id, dest_smio_id, dest_smio_inst,
            msg);
    ASSERT_TEST(zerr == 0, "Could not send MGMT message", err_send_mgmt_msg,
           SMIO_ERR_REGISTER_SM);

err_send_mgmt_msg:
    return err;
}

/* Only safe to use before SMIO initialization (inside SMIO init routine) */
smio_err_e smio_get_sdb_info (smio_t *self, uint32_t smio_id,
    uint32_t smio_inst, uint64_t vid, uint32_t did,
    sdbutils_info_t *sdbutils_info)
{
    assert (self);
    smio_err_e err = SMIO_SUCCESS;

    int zerr = zsock_send (self->pipe_mgmt, "s48444s84", "$SDB_DEVICE_INFO",
            smio_id, self->base, smio_inst, 0x0, 0x0,
            "", vid, did);
    ASSERT_TEST(zerr == 0, "Could not send SDB_DEVICE_INFO message", err_send_sdb_info_msg,
            SMIO_ERR_REGISTER_SM);

    /* Wait for reply. Note here we expect the reply over the same socket,
     * event though the receiving channel was registered in a zloop engine.
     * The zloop reader was canceled before and will be reinserted in the engine
     * in after receving the message */
    zerr = zsock_recv (self->pipe_mgmt, "s2114", NULL, &sdbutils_info->abi_class,
        &sdbutils_info->abi_ver_major, &sdbutils_info->abi_ver_minor, &sdbutils_info->bus_specific);
    ASSERT_TEST(zerr == 0, "Could not receive SDB_DEVICE_INFO message", err_recv_sdb_info_msg,
            SMIO_ERR_REGISTER_SM);

err_recv_sdb_info_msg:
err_send_sdb_info_msg:
    return err;
}

/* Only safe to use before SMIO initialization (inside SMIO init routine) */
smio_err_e smio_get_board_type (smio_t *self, char **board_type)
{
    assert (self);
    smio_err_e err = SMIO_SUCCESS;

    int zerr = zsock_send (self->pipe_mgmt, "s", "$BOARD_TYPE");
    ASSERT_TEST(zerr == 0, "Could not send BOARD_TYPE message", err_send_board_type_msg,
            SMIO_ERR_REGISTER_SM);

    /* Wait for reply. Note here we expect the reply over the same socket,
     * event though the receiving channel was registered in a zloop engine.
     * The zloop reader was canceled before and will be reinserted in the engine
     * in after receving the message */
    zerr = zsock_recv (self->pipe_mgmt, "ss", NULL, board_type);
    ASSERT_TEST(zerr == 0, "Could not receive BOARD_TYPE message", err_recv_board_type_msg,
            SMIO_ERR_REGISTER_SM);

err_recv_board_type_msg:
err_send_board_type_msg:
    return err;
}

/************************************************************/
/***************** Dispatch table callbacks *****************/
/************************************************************/

static disp_table_err_e _smio_check_msg_args (disp_table_t *disp_table,
        const disp_op_t *disp_op, void *args)
{
    assert (disp_table);
    assert (disp_op);
    assert (args);

    disp_table_err_e err = DISP_TABLE_SUCCESS;

    /* Check if the message tis the correct one */
    ASSERT_TEST (msg_guess_type (args) == MSG_EXP_ZMQ, "Invalid message tag",
            err_inv_msg, DISP_TABLE_ERR_BAD_MSG);
    msg_err_e merr = msg_check_gen_zmq_args (disp_op, EXP_MSG_ZMQ(args));
    ASSERT_TEST (merr == MSG_SUCCESS, "Unrecognized message. Message arguments "
            "checking failed", err_msg_args_check, DISP_TABLE_ERR_BAD_MSG);

err_msg_args_check:
err_inv_msg:
    return err;
}

const disp_table_ops_t smio_disp_table_ops = {
    .check_msg_args = &_smio_check_msg_args
};

/************************************************************/
/**************** SMIO Ops wrapper functions ****************/
/************************************************************/

/* Attach an instance of sm_io to dev_io function pointer */
smio_err_e smio_attach (smio_t *self, void *args)
{
    assert (self);
    smio_err_e err = SMIO_SUCCESS;

    err = SMIO_FUNC_OPS_NOFAIL_WRAPPER(err, attach, args);
    ASSERT_TEST(err == SMIO_SUCCESS, "Registered SMIO \"attach\" function error",
        err_func);

err_func:
    return err;
}

/* Deattach an instance of sm_io to dev_io function pointer */
smio_err_e smio_deattach (smio_t *self)
{
    assert (self);
    smio_err_e err = SMIO_SUCCESS;

    err = SMIO_FUNC_OPS_NOFAIL_WRAPPER(err, deattach);
    ASSERT_TEST(err == SMIO_SUCCESS, "Registered SMIO \"deattach\" function error",
        err_func);

err_func:
    return err;
}

/* Export (Register) sm_io to handle specific operations */
smio_err_e smio_export_ops (smio_t *self, const disp_op_t** smio_exp_ops)
{
    assert (self);
    assert (smio_exp_ops);

    smio_err_e err = SMIO_SUCCESS;

    disp_table_err_e derr = disp_table_insert_all (self->exp_ops_dtable, smio_exp_ops);
    ASSERT_TEST(derr == DISP_TABLE_SUCCESS, "smio_export_ops: Could not export"
            " SMIO ops", err_export_op, SMIO_ERR_EXPORT_OP);

    err = SMIO_FUNC_OPS_NOFAIL_WRAPPER(err, export_ops, smio_exp_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Registered SMIO \"export_ops\" function error",
        err_func);

err_func:
err_export_op:
    return err;
}

/* Unexport (unregister) sm_io to handle specific operations */
smio_err_e smio_unexport_ops (smio_t *self)
{
    assert (self);

    smio_err_e err = SMIO_SUCCESS;
    disp_table_err_e derr = disp_table_remove_all (self->exp_ops_dtable);

    ASSERT_TEST(derr == DISP_TABLE_SUCCESS, "smio_export_ops: Could not unexport SMIO ops",
            err_unexport_op, SMIO_ERR_EXPORT_OP);

    err = SMIO_FUNC_OPS_NOFAIL_WRAPPER(err, unexport_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Registered SMIO \"unexport_ops\" function error",
        err_func);

err_func:
err_unexport_op:
    return err;
}

smio_err_e smio_do_op (void *owner, void *msg)
{
    return _smio_do_op (owner, msg);
}

smio_err_e smio_init_exp_ops (smio_t *self, disp_op_t** smio_exp_ops,
        const disp_table_func_fp *func_fps)
{
    assert(self);

    disp_table_err_e derr = DISP_TABLE_SUCCESS;
    smio_err_e err = SMIO_SUCCESS;
    derr = disp_table_fill_desc (self->exp_ops_dtable, smio_exp_ops, func_fps);
    ASSERT_TEST(derr == DISP_TABLE_SUCCESS, "smio_export_ops: Could not export"
            " fill SMIO ops description", err_fill_desc_ops, SMIO_ERR_EXPORT_OP);

err_fill_desc_ops:
    return err;
}

/************************************************************/
/********************* Accessor methods *********************/
/************************************************************/

smio_err_e smio_set_base (smio_t *self, uint64_t base)
{
    assert (self);

    smio_err_e err = SMIO_SUCCESS;
    self->base = base;

    return err;
}

uint64_t smio_get_base (smio_t *self)
{
    assert (self);
    return self->base;
}

smio_err_e smio_set_id (smio_t *self, uint32_t id)
{
    assert (self);

    smio_err_e err = SMIO_SUCCESS;
    self->id = id;

    return err;
}

uint32_t smio_get_id (smio_t *self)
{
    assert (self);
    return self->id;
}

smio_err_e smio_set_inst_id (smio_t *self, uint32_t inst_id)
{
    assert (self);

    smio_err_e err = SMIO_SUCCESS;
    self->inst_id = inst_id;

    return err;
}

uint32_t smio_get_inst_id (smio_t *self)
{
    assert (self);
    return self->inst_id;
}

smio_err_e smio_set_name (smio_t *self, const char *name)
{
    assert (self);
    return _smio_set_name (self, name);
}

/* Get SMIO name */
const char *smio_get_name (smio_t *self)
{
    assert (self);
    return _smio_get_name (self);
}

/* Clone SMIO name */
char *smio_clone_name (smio_t *self)
{
    assert (self);
    return _smio_clone_name (self);
}

/* Get SMIO endpoint */
const char *smio_get_endpoint (smio_t *self)
{
    assert (self);
    return _smio_get_endpoint (self);
}

/* Clone SMIO endpoint */
char *smio_clone_endpoint (smio_t *self)
{
    assert (self);
    return _smio_clone_endpoint (self);
}

/* Get SMIO service */
const char *smio_get_service (smio_t *self)
{
    assert (self);
    return _smio_get_service (self);
}

/* Clone SMIO service */
char *smio_clone_service (smio_t *self)
{
    assert (self);
    return _smio_clone_service (self);
}

smio_err_e smio_set_exp_ops (smio_t *self, const disp_op_t **exp_ops)
{
    assert (self);
    smio_err_e err = SMIO_SUCCESS;

    self->exp_ops = exp_ops;

    return err;
}

const disp_op_t **smio_get_exp_ops (smio_t *self)
{
    assert (self);
    return self->exp_ops;
}

smio_err_e smio_set_ops (smio_t *self, const smio_ops_t *ops)
{
    assert (self);
    smio_err_e err = SMIO_SUCCESS;

    self->ops = ops;

    return err;
}

const smio_ops_t *smio_get_ops (smio_t *self)
{
    assert (self);
    return self->ops;
}

smio_err_e smio_set_thsafe_client_ops (smio_t *self,
        const smio_thsafe_client_ops_t *thsafe_client_ops)
{
    assert (self);
    smio_err_e err = SMIO_SUCCESS;

    self->thsafe_client_ops = thsafe_client_ops;

    return err;
}

const smio_thsafe_client_ops_t *smio_get_thsafe_client_ops (smio_t *self)
{
    assert (self);
    return self->thsafe_client_ops;
}

/**************** Static Functions ***************/

static smio_err_e _smio_do_op (void *owner, void *msg)
{
    assert (owner);
    assert (msg);

    SMIO_OWNER_TYPE *self = SMIO_EXP_OWNER(owner);
    smio_err_e err = SMIO_SUCCESS;

    /* TODO: The SMIO do_op must not modify the packet! We could pass a copy of the
     * message to it, but we this is in the critical path! Evaluate the impact
     * of doing this */
    SMIO_FUNC_OPS_NOFAIL_WRAPPER(err, do_op, msg);
    ASSERT_TEST (err == SMIO_SUCCESS, "Registered SMIO \"do_op\" function error",
            err_do_op);

    disp_table_t *disp_table = self->exp_ops_dtable;
    msg_err_e merr = msg_handle_mlm_request (owner, msg, disp_table);
    ASSERT_TEST (merr == MSG_SUCCESS, "Error handling request", err_hand_req,
           SMIO_ERR_MSG_NOT_SUPP /* returning a more meaningful error? */);

err_hand_req:
err_do_op:
    return err;
}

static smio_err_e _smio_do_mgmt_msg (void *owner, void *msg)
{
    assert (owner);
    assert (msg);

    SMIO_OWNER_TYPE *self = SMIO_EXP_OWNER(owner);
    smio_err_e err = SMIO_SUCCESS;

    /* TODO: The SMIO do_op must not modify the packet! We could pass a copy of the
     * message to it, but we this is in the critical path! Evaluate the impact
     * of doing this */
    SMIO_FUNC_OPS_NOFAIL_WRAPPER(err, do_mgmt_op, msg);
    ASSERT_TEST (err == SMIO_SUCCESS, "Registered SMIO \"do_mgmt_op\" function error",
            err_do_op);

err_do_op:
    return err;
}

/* Set SMIO name */
static smio_err_e _smio_set_name (smio_t *self, const char *name)
{
    assert (self);
    smio_err_e err = SMIO_SUCCESS;

    if (name) {
        if (self->name) {
            free (self->name);
        }
        self->name = strdup (name);
        ASSERT_ALLOC(self->name, err_name_alloc, SMIO_ERR_ALLOC);
    }

err_name_alloc:
    return err;
}

/* Get SMIO name */
static const char *_smio_get_name (smio_t *self)
{
    assert (self);
    return self->name;
}

/* Clone SMIO name */
static char *_smio_clone_name (smio_t *self)
{
    assert (self);

    char *name = strdup (self->name);
    ASSERT_ALLOC(name, err_alloc);

err_alloc:
    return name;
}

/* Get SMIO endpoint */
static const char *_smio_get_endpoint (smio_t *self)
{
    assert (self);
    return self->endpoint;
}

/* Clone SMIO endpoint */
static char *_smio_clone_endpoint (smio_t *self)
{
    assert (self);

    char *endpoint = strdup (self->endpoint);
    ASSERT_ALLOC(endpoint, err_alloc);

err_alloc:
    return endpoint;
}

/* Get SMIO service */
static const char *_smio_get_service (smio_t *self)
{
    assert (self);
    return self->service;
}

/* Clone SMIO service */
static char *_smio_clone_service (smio_t *self)
{
    assert (self);

    char *service = strdup (self->service);
    ASSERT_ALLOC(service, err_alloc);

err_alloc:
    return service;
}

smio_err_e smio_set_handler (smio_t *self, void *smio_handler)
{
    assert (self);
    self->smio_handler = smio_handler;
    return SMIO_SUCCESS;
}

void *smio_get_handler (smio_t *self)
{
    assert (self);
    return self->smio_handler;
}

mlm_client_t *smio_get_worker (smio_t *self)
{
    assert (self);
    return self->worker;
}

zsock_t *smio_get_dealer (smio_t *self)
{
    assert (self);
    return self->dealer;
}

zsock_t *smio_get_dealer2 (smio_t *self)
{
    assert (self);
    return self->dealer2;
}

zsock_t *smio_get_pipe_mgmt (smio_t *self)
{
    assert (self);
    return self->pipe_mgmt;
}

/************************************************************/
/************* SMIO thsafe wrapper functions   **************/
/************************************************************/

#define CHECK_FUNC(func_p)                                  \
    do {                                                    \
        if(func_p == NULL) {                                \
            DBE_DEBUG (DBG_SM_IO | DBG_LVL_ERR,             \
                    "[sm_io] %s\n",                         \
                    smio_err_str (SMIO_ERR_FUNC_NOT_IMPL)); \
            return -SMIO_ERR_FUNC_NOT_IMPL;                 \
        }                                                   \
    } while(0)

#define ASSERT_FUNC(func_name)                              \
    do {                                                    \
        assert (self);                                      \
        assert (self->thsafe_client_ops);                   \
        CHECK_FUNC (self->thsafe_client_ops->func_name);    \
    } while(0)

/* Declare wrapper for all SMIO functions API */
#define SMIO_FUNC_WRAPPER(func_name, sock, ...)             \
{                                                           \
    ASSERT_FUNC(func_name);                                 \
    return self->thsafe_client_ops->func_name (sock, ##__VA_ARGS__);  \
}

/**** Open device ****/
int smio_thsafe_client_open (smio_t *self, llio_endpoint_t *endpoint)
    SMIO_FUNC_WRAPPER (thsafe_client_open, self->dealer, endpoint)

int smio_thsafe_client_open_gen (smio_t *self, void *sock, llio_endpoint_t *endpoint)
    SMIO_FUNC_WRAPPER (thsafe_client_open, sock, endpoint)

/**** Release device ****/
int smio_thsafe_client_release (smio_t *self, llio_endpoint_t *endpoint)
    SMIO_FUNC_WRAPPER (thsafe_client_release, self->dealer, endpoint)

int smio_thsafe_client_release_gen (smio_t *self, void *sock, llio_endpoint_t *endpoint)
    SMIO_FUNC_WRAPPER (thsafe_client_release, sock, endpoint)

/**** Read data from device ****/
ssize_t smio_thsafe_client_read_16 (smio_t *self, uint64_t offs, uint16_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_read_16, self->dealer, self->base | offs, data)
ssize_t smio_thsafe_client_read_32 (smio_t *self, uint64_t offs, uint32_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_read_32, self->dealer, self->base | offs, data)
ssize_t smio_thsafe_client_read_64 (smio_t *self, uint64_t offs, uint64_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_read_64, self->dealer, self->base | offs, data)

ssize_t smio_thsafe_raw_client_read_16 (smio_t *self, uint64_t offs, uint16_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_read_16, self->dealer, offs, data)
ssize_t smio_thsafe_raw_client_read_32 (smio_t *self, uint64_t offs, uint32_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_read_32, self->dealer, offs, data)
ssize_t smio_thsafe_raw_client_read_64 (smio_t *self, uint64_t offs, uint64_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_read_64, self->dealer, offs, data)

ssize_t smio_thsafe_client_read_16_gen (smio_t *self, void *sock, uint64_t offs, uint16_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_read_16, sock, self->base | offs, data)
ssize_t smio_thsafe_client_read_32_gen (smio_t *self, void *sock, uint64_t offs, uint32_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_read_32, sock, self->base | offs, data)
ssize_t smio_thsafe_client_read_64_gen (smio_t *self, void *sock, uint64_t offs, uint64_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_read_64, sock, self->base | offs, data)

ssize_t smio_thsafe_raw_client_read_16_gen (smio_t *self, void *sock, uint64_t offs, uint16_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_read_16, sock, offs, data)
ssize_t smio_thsafe_raw_client_read_32_gen (smio_t *self, void *sock, uint64_t offs, uint32_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_read_32, sock, offs, data)
ssize_t smio_thsafe_raw_client_read_64_gen (smio_t *self, void *sock, uint64_t offs, uint64_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_read_64, sock, offs, data)

/**** Write data to device ****/
ssize_t smio_thsafe_client_write_16 (smio_t *self, uint64_t offs, const uint16_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_write_16, self->dealer, self->base | offs, data)
ssize_t smio_thsafe_client_write_32 (smio_t *self, uint64_t offs, const uint32_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_write_32, self->dealer, self->base | offs, data)
ssize_t smio_thsafe_client_write_64 (smio_t *self, uint64_t offs, const uint64_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_write_64, self->dealer, self->base | offs, data)

ssize_t smio_thsafe_raw_client_write_16 (smio_t *self, uint64_t offs, const uint16_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_write_16, self->dealer, offs, data)
ssize_t smio_thsafe_raw_client_write_32 (smio_t *self, uint64_t offs, const uint32_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_write_32, self->dealer, offs, data)
ssize_t smio_thsafe_raw_client_write_64 (smio_t *self, uint64_t offs, const uint64_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_write_64, self->dealer, offs, data)

ssize_t smio_thsafe_client_write_16_gen (smio_t *self, void *sock, uint64_t offs, const uint16_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_write_16, sock, self->base | offs, data)
ssize_t smio_thsafe_client_write_32_gen (smio_t *self, void *sock, uint64_t offs, const uint32_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_write_32, sock, self->base | offs, data)
ssize_t smio_thsafe_client_write_64_gen (smio_t *self, void *sock, uint64_t offs, const uint64_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_write_64, sock, self->base | offs, data)

ssize_t smio_thsafe_raw_client_write_16_gen (smio_t *self, void *sock, uint64_t offs, const uint16_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_write_16, sock, offs, data)
ssize_t smio_thsafe_raw_client_write_32_gen (smio_t *self, void *sock, uint64_t offs, const uint32_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_write_32, sock, offs, data)
ssize_t smio_thsafe_raw_client_write_64_gen (smio_t *self, void *sock, uint64_t offs, const uint64_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_write_64, sock, offs, data)

/**** Read data block from device function pointer, size in bytes ****/
ssize_t smio_thsafe_client_read_block (smio_t *self, uint64_t offs, size_t size,
        uint32_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_read_block, self->dealer, self->base | offs, size, data)
ssize_t smio_thsafe_raw_client_read_block (smio_t *self, uint64_t offs, size_t size,
        uint32_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_read_block, self->dealer, offs, size, data)

ssize_t smio_thsafe_client_read_block_gen (smio_t *self, void *sock, uint64_t offs, size_t size,
        uint32_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_read_block, sock, self->base | offs, size, data)
ssize_t smio_thsafe_raw_client_read_block_gen (smio_t *self, void *sock, uint64_t offs, size_t size,
        uint32_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_read_block, sock, offs, size, data)

/**** Write data block from device function pointer, size in bytes ****/
ssize_t smio_thsafe_client_write_block (smio_t *self, uint64_t offs, size_t size,
        const uint32_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_write_block, self->dealer, self->base | offs, size, data)
ssize_t smio_thsafe_raw_client_write_block (smio_t *self, uint64_t offs, size_t size,
        const uint32_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_write_block, self->dealer, offs, size, data)

ssize_t smio_thsafe_client_write_block_gen (smio_t *self, void *sock, uint64_t offs, size_t size,
        const uint32_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_write_block, sock, self->base | offs, size, data)
ssize_t smio_thsafe_raw_client_write_block_gen (smio_t *self, void *sock, uint64_t offs, size_t size,
        const uint32_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_write_block, sock, offs, size, data)

/**** Read data block via DMA from device, size in bytes ****/
ssize_t smio_thsafe_client_read_dma (smio_t *self, uint64_t offs, size_t size,
        uint32_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_read_dma, self->dealer, self->base | offs, size, data)
ssize_t smio_thsafe_raw_client_read_dma (smio_t *self, uint64_t offs, size_t size, uint32_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_read_dma, self->dealer, offs, size, data)

ssize_t smio_thsafe_client_read_dma_gen (smio_t *self, void *sock, uint64_t offs, size_t size,
        uint32_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_read_dma, sock, self->base | offs, size, data)
ssize_t smio_thsafe_raw_client_read_dma_gen (smio_t *self, void *sock, uint64_t offs, size_t size,
        uint32_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_read_dma, sock, offs, size, data)

/**** Write data block via DMA from device, size in bytes ****/
ssize_t smio_thsafe_client_write_dma (smio_t *self, uint64_t offs, size_t size,
        const uint32_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_write_dma, self->dealer, self->base | offs, size, data)
ssize_t smio_thsafe_raw_client_write_dma (smio_t *self, uint64_t offs, size_t size,
        const uint32_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_write_dma, self->dealer, offs, size, data)

ssize_t smio_thsafe_client_write_dma_gen (smio_t *self, void *sock, uint64_t offs, size_t size,
        const uint32_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_write_dma, sock, self->base | offs, size, data)
ssize_t smio_thsafe_raw_client_write_dma_gen (smio_t *self, void *sock, uint64_t offs, size_t size,
        const uint32_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_write_dma, sock, offs, size, data)

/**** Read device information function pointer ****/
/* int smio_thsafe_raw_client_read_info (smio_t *self, llio_dev_info_t *dev_info)
    SMIO_FUNC_WRAPPER (thsafe_client_read_info, dev_info) Moved to dev_io */

