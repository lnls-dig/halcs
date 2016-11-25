/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "halcs_server.h"
/* Private headers */
#include "sm_io_mod_dispatch.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...)  \
    ASSERT_HAL_TEST(test_boolean, DEV_IO, "[dev_io_core]",  \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_ALLOC(ptr, DEV_IO, "[dev_io_core]",          \
            devio_err_str(DEVIO_ERR_ALLOC),                 \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                            \
    CHECK_HAL_ERR(err, DEV_IO, "[dev_io_core]",             \
            devio_err_str (err_type))

#define LLIO_STR                            ":LLIO\0"
#define DEVIO_POLLER_TIMEOUT                100000     /* in msec */
#define DEVIO_POLLER_NTIMES                 0          /* 0 for infinte */
#define DEVIO_POLLER_CONFIG_TIMEOUT         0          /* in msec */
#define DEVIO_DFLT_LOG_MODE                 "w"

#define DEVIO_MAX_DESTRUCT_MSG_TRIES        10
#define DEVIO_LINGER_TIME                   100         /* in ms */

struct _devio_t {
    /* General information */
    zactor_t **pipes_mgmt;              /* Address nodes using this array of actors (Management PIPES) */
    zsock_t **pipes_msg;                /* Address nodes using this array of actors (Message PIPES) */
    zactor_t **pipes_config;            /* Address config actors using this array of actors (Config PIPES) */
    zactor_t *pipe_payload;             /* Address DEVIO payload thread (Payload PIPE) */
    zsock_t *pipe;                      /* Address the DEVIO instance using this sock */
    zsock_t *pipe_user;                 /* Address the DEVIO instance using this sock */
    zsock_t *pipe_frontend;             /* Force zloop to interrupt and rebuild poll set. This is used to send messages */
    zsock_t *pipe_backend;              /* Force zloop to interrupt and rebuild poll set. This is used to receive messages */
    zsock_t *pipe_frontend_user;        /* Force zloop to interrupt and rebuild poll set. This is used to send messages */
    zsock_t *pipe_backend_user;         /* Force zloop to interrupt and rebuild poll set. This is used to receive messages */
    zloop_t *loop;                      /* Reactor for server sockets */
    zloop_t *loop_user;             	/* Reactor for user configuration */
    unsigned int nnodes;                /* Number of actual nodes */
    char *name;                         /* Identification of this worker instance */
    uint32_t id;                        /* ID number of this instance */
    char *log_file;                     /* Log filename for tracing and debugging */
    char *endpoint_broker;              /* Broker location to connect to */
    int verbose;                        /* Print activity to stdout */
    int timer_id;                       /* Timer ID */
    struct sdbfs *sdbfs;                /* SDB information */

    /* General management operations */
    devio_ops_t *ops;
    /* ll_io instance for Low-Level operations*/
    llio_t *llio;
    /* Server part of the llio operations. This is the bridge between the
     * smio client part of the llio operations and the de-facto
     * llio operations */
    const disp_op_t **thsafe_server_ops;
    /* Hash containing all the sm_io objects that
     * this dev_io can handle. It is composed
     * of key (10-char ID) / value (sm_io instance) */
    zhashx_t *sm_io_h;
    /* Hash containing all the Config sm_io objects that
     * this dev_io can handle. It is composed
     * of key (10-char ID) / value (sm_io instance) */
    zhashx_t *sm_io_cfg_h;
    /* Dispatch table containing all the sm_io thsafe operations
     * that we need to handle. It is composed
     * of key (4-char ID) / value (pointer to function) */
    disp_table_t *disp_table_thsafe_ops;
};


/* DEVIO dispatch table operations */
const disp_table_ops_t devio_disp_table_ops;

/* Dispatch table message check handler */
static disp_table_err_e _devio_check_msg_args (disp_table_t *disp_table,
        const disp_op_t *disp_op, void *args);

/* SMIO key generators */
static char *_devio_gen_smio_key (devio_t *self,
        const volatile smio_mod_dispatch_t *smio_mod_handler,
        uint32_t inst_id);
static char *_devio_gen_smio_key_auto (devio_t *self,
        const volatile smio_mod_dispatch_t *smio_mod_handler, uint32_t inst_id,
        bool auto_inst_id, uint32_t *used_inst_id);

/* Do the SMIO operation */
static devio_err_e _devio_do_smio_op (devio_t *self, void *msg);
static devio_err_e _devio_destroy_actor (devio_t *self, zactor_t **actor);
static devio_err_e _devio_destroy_smio (devio_t *self, zhashx_t *smio_h, const char *smio_key);
static devio_err_e _devio_destroy_smio_all (devio_t *self, zhashx_t *smio_h);

/* General operations set handlers */
static devio_err_e _devio_set_wait_clhd_handler (devio_t *self, wait_chld_handler_fp fp);
static devio_err_e _devio_set_wait_clhd_timed_handler (devio_t *self, wait_chld_timed_handler_fp fp);
static devio_err_e _devio_set_spawn_clhd_handler (devio_t *self, spawn_chld_handler_fp fp);
static devio_err_e _devio_register_sig_handlers (devio_t *self);

/* Handle socket events */
static volatile const smio_mod_dispatch_t *_devio_search_sm_by_id (devio_t *self,
        uint32_t smio_id);
#if 0
static devio_err_e _devio_engine_handle_socket (devio_t *devio, void *sock,
        zloop_reader_fn handler);
#endif
static devio_err_e _devio_engine_handle_socket (devio_t *devio, zloop_t *loop, 
        void *pipe_frontend, void *sock, zloop_reader_fn handler);
static int _devio_handle_timer (zloop_t *loop, int timer_id, void *arg);
static int _devio_handle_pipe_backend (zloop_t *loop, zsock_t *reader, void *args);

static devio_err_e _devio_register_sm_raw (devio_t *self, uint32_t smio_id, uint64_t base,
        uint32_t inst_id, bool auto_inst_id);
static devio_err_e _devio_send_smio_mgmt_msg_raw (devio_t *self, uint32_t smio_id, 
        uint64_t base, uint32_t inst_id, uint32_t dest_smio_id, uint32_t dest_inst_id, 
        char *msg);
static devio_err_e _devio_register_sm_by_id_raw (devio_t *self, uint32_t smio_id);
static devio_err_e _devio_register_all_sm_raw (devio_t *self);
static devio_err_e _devio_unregister_sm_raw (devio_t *self, const char *smio_key);
static devio_err_e _devio_unregister_all_sm_raw (devio_t *self);

/* Static loops */
static void _devio_payload_loop (zsock_t *pipe, void *args);

/* FIXME: Only valid for PCIe devices */
static int _devio_read_llio_block (struct sdbfs *fs, int offset, void *buf,
        int count)
{
    devio_t *devio = (devio_t *)fs->drvdata;
    llio_t *llio = devio->llio;
    uint64_t llio_sdb_prefix_addr = llio_get_sdb_prefix_addr (llio);

    return llio_read_block (llio, llio_sdb_prefix_addr |
            (offset), count, (uint32_t *) buf);
}

/* Default signal handlers */
void devio_sigchld_h (int sig, siginfo_t *siginfo, void *context)
{
    (void) sig;
    (void) siginfo;
    (void) context;
    while (hutils_wait_chld () > 0);
}

/* SIGCHLD default handler */
static devio_sig_handler_t devio_sigchld_handler =
{
    .signal = SIGCHLD,
    .devio_sig_h = devio_sigchld_h
};

/* Creates a new instance of Device Information */
devio_t * devio_new (char *name, uint32_t id, char *endpoint_dev,
        const llio_ops_t *reg_ops, char *endpoint_broker, int verbose,
        const char *log_file_name)
{
    assert (name);
    assert (endpoint_dev);
    assert (reg_ops);
    assert (endpoint_broker);

    /* Set logfile available for all dev_mngr and dev_io instances.
     * We accept NULL as a parameter, meaning to suppress all messages */
    errhand_set_log (log_file_name, DEVIO_DFLT_LOG_MODE);

    DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO, "[dev_io_core] Spawing DEVIO worker"
            " with exported service %s, for a %s device \n\tlocated on %s,"
            " broker address %s, with logfile on %s ...\n", name,
            (reg_ops->name == NULL) ? "NULL" : reg_ops->name,
            endpoint_dev, endpoint_broker, (log_file_name == NULL) ? "NULL" : log_file_name);

    /* Print Software info */
    DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO, "[dev_io_core] HALCS Device I/O version %s,"
            " Build by: %s, %s\n",
            revision_get_build_version (),
            revision_get_build_user_name (),
            revision_get_build_date ());

    devio_t *self = (devio_t *) zmalloc (sizeof *self);
    ASSERT_ALLOC(self, err_self_alloc);

    if (log_file_name != NULL) {
        self->log_file = strdup (log_file_name);
    }

    ASSERT_TEST((self->log_file == NULL && log_file_name == NULL)
            || (self->log_file != NULL && log_file_name != NULL),
            "Error setting log file!", err_log_file);

    /* Initialize the sockets structure to talk to nodes */
    self->pipes_mgmt = zmalloc (sizeof (*self->pipes_mgmt) * NODES_MAX_LEN);
    ASSERT_ALLOC(self->pipes_mgmt, err_pipes_mgmt_alloc);
    self->pipes_msg = zmalloc (sizeof (*self->pipes_msg) * NODES_MAX_LEN);
    ASSERT_ALLOC(self->pipes_msg, err_pipes_msg_alloc);
    self->pipes_config = zmalloc (sizeof (*self->pipes_config) * NODES_MAX_LEN);
    ASSERT_ALLOC(self->pipes_config, err_pipes_config_alloc);
    self->pipe_payload = zmalloc (sizeof (self->pipe_payload));
    ASSERT_ALLOC(self->pipe_payload, err_pipe_payload_alloc);
    self->pipe = NULL;
    self->pipe_user = NULL;
    /* 0 nodes for now... */
    self->nnodes = 0;

    /* Setup pipes for zloop interrupting */
    self->pipe_frontend = zsys_create_pipe (&self->pipe_backend);
    ASSERT_ALLOC(self->pipe_frontend, err_pipe_frontend_alloc);
    self->pipe_frontend_user = zsys_create_pipe (&self->pipe_backend_user);
    ASSERT_ALLOC(self->pipe_frontend_user, err_pipe_frontend_user_alloc);

    /* Setup loop */
    self->loop = zloop_new ();
    ASSERT_ALLOC(self->loop, err_loop_alloc);

    /* Set loop timeout. This is needed to ensure zloop will
     * frequently check for rebuilding its poll set */
    self->timer_id = zloop_timer (self->loop, DEVIO_POLLER_TIMEOUT, DEVIO_POLLER_NTIMES,
            _devio_handle_timer, NULL);
    ASSERT_TEST(self->timer_id != -1, "Could not create zloop timer", err_timer_alloc);

    /* Setup loop frontend, for user requests */
    self->loop_user = zloop_new ();
    ASSERT_ALLOC(self->loop_user, err_loop_user_alloc);

    /* Set-up backend handler for forcing interrupting the zloop and rebuild
     * the poll set. This avoids having to setup a short timer to periodically
     * interrupting the loop to check for rebuilds */
    _devio_engine_handle_socket (self, self->loop, self->pipe_frontend, 
        self->pipe_backend, _devio_handle_pipe_backend);
    _devio_engine_handle_socket (self, self->loop_user, self->pipe_frontend_user, 
        self->pipe_backend_user, _devio_handle_pipe_backend);

    /* Setup strings/options */
    self->name = strdup (name);
    ASSERT_ALLOC(self->name, err_name_alloc);

    /* Setup ID number */
    self->id = id;

    /* Setup Broker endpoint */
    self->endpoint_broker = strdup (endpoint_broker);
    ASSERT_ALLOC(self->endpoint_broker, err_endp_broker_alloc);
    self->verbose = verbose;

    /* Create General operations structures */
    self->ops = (devio_ops_t *) zmalloc (sizeof *self->ops);
    ASSERT_ALLOC(self->ops, err_ops_alloc);
    self->ops->sig_ops = zlistx_new ();
    ASSERT_ALLOC(self->ops->sig_ops, err_list_alloc);

    /* Setup default general operations */
    _devio_set_wait_clhd_handler (self, &hutils_wait_chld);
    _devio_set_wait_clhd_timed_handler (self, &hutils_wait_chld_timed);
    _devio_set_spawn_clhd_handler (self, &hutils_spawn_chld);

    devio_err_e derr = devio_set_sig_handler (self, &devio_sigchld_handler);
    ASSERT_TEST(derr==DEVIO_SUCCESS, "Error setting signal handlers", err_set_sig_handlers);

    derr = _devio_register_sig_handlers (self);
    ASSERT_TEST(derr==DEVIO_SUCCESS, "Error registering setting up signal handlers", err_sig_handlers);

    /* Concatenate recv'ed name with a llio identifier */
    char *llio_name = zmalloc (sizeof (char)*(strlen(name)+strlen(LLIO_STR)+1));
    ASSERT_ALLOC(llio_name, err_llio_name_alloc);
    strcat (llio_name, name);
    strcat (llio_name, LLIO_STR);
    self->llio = llio_new (llio_name, endpoint_dev, reg_ops,
            verbose);
    ASSERT_ALLOC(self->llio, err_llio_alloc);

    /* We try to open the device */
    int err = llio_open (self->llio, NULL);
    ASSERT_TEST(err==0, "Error opening device!", err_llio_open);

    /* We can free llio_name now, as llio copies the string */
    free (llio_name);
    llio_name = NULL; /* Avoid double free error */

    /* Alloc SDB structure */
    self->sdbfs = zmalloc (sizeof *self->sdbfs);
    ASSERT_ALLOC(self->sdbfs, err_sdbfs_alloc);

    /* Initialize the sdb filesystem itself */
    self->sdbfs->name = "sdb-area";
    self->sdbfs->drvdata = (void *) self;
    self->sdbfs->blocksize = 1; /* Not currently used */
    self->sdbfs->entrypoint = SDB_ADDRESS;
    self->sdbfs->data = 0;
    self->sdbfs->flags = 0;
    self->sdbfs->read = _devio_read_llio_block;

    /* Create SDB. If the device does not support SDB, this will fail.
     * So, avoid creating SDB in this case, for now, as some unsupported
     * endpoints do not have timeout implemented just yet */
    if (streq (llio_get_ops_name (self->llio), "PCIE")) {
        err = sdbfs_dev_create (self->sdbfs);
        ASSERT_TEST (err == 0, "Could not create SDBFS",
                err_sdbfs_create, DEVIO_ERR_SMIO_DO_OP);
    }

    /* Init sm_io_thsafe_server_ops_h. For now, we assume we want zmq
     * for exchanging messages between smio and devio instances */
    self->thsafe_server_ops = smio_thsafe_zmq_server_ops;

    /* Init sm_io_h hash */
    self->sm_io_h = zhashx_new ();
    ASSERT_ALLOC(self->sm_io_h, err_sm_io_h_alloc);

    /* Init sm_io_cfg_h hash */
    self->sm_io_cfg_h = zhashx_new ();
    ASSERT_ALLOC(self->sm_io_cfg_h, err_sm_io_cfg_h_alloc);

    /* Init sm_io_thsafe_ops_h dispatch table */
    self->disp_table_thsafe_ops = disp_table_new (&devio_disp_table_ops);
    ASSERT_ALLOC(self->disp_table_thsafe_ops, err_disp_table_thsafe_ops_alloc);

    disp_table_err_e disp_err = disp_table_insert_all (self->disp_table_thsafe_ops,
            self->thsafe_server_ops);
    ASSERT_TEST(disp_err==DISP_TABLE_SUCCESS, "Could not initialize dispatch table",
            err_disp_table_init);

    /* Adjust linger time for our sockets */
    /* A non-zero linger value is required for DISCONNECT to be sent
     * when the worker is destroyed. 100 is arbitrary but chosen to be
     * sufficient for common cases without significant delay in broken ones. */
    zsys_set_linger (DEVIO_LINGER_TIME);

    return self;

err_disp_table_init:
    disp_table_destroy (&self->disp_table_thsafe_ops);
err_disp_table_thsafe_ops_alloc:
    zhashx_destroy (&self->sm_io_cfg_h);
err_sm_io_cfg_h_alloc:
    zhashx_destroy (&self->sm_io_h);
err_sm_io_h_alloc:
    if (streq (llio_get_ops_name (self->llio), "PCIE")) {
        sdbfs_dev_destroy (self->sdbfs);
    }
err_sdbfs_create:
    free (self->sdbfs);
err_sdbfs_alloc:
    llio_release (self->llio, NULL);
err_llio_open:
    llio_destroy (&self->llio);
err_llio_alloc:
    free (llio_name);
err_llio_name_alloc:
    /* Nothing to undo */
err_sig_handlers:
    /* Nothing to undo */
err_set_sig_handlers:
    zlistx_destroy (&self->ops->sig_ops);
err_list_alloc:
    free (self->ops);
err_ops_alloc:
    free (self->endpoint_broker);
err_endp_broker_alloc:
    free (self->name);
err_name_alloc:
    zloop_destroy (&self->loop_user);
err_loop_user_alloc:
    zloop_timer_end (self->loop, self->timer_id);
err_timer_alloc:
    zloop_destroy (&self->loop);
err_loop_alloc:
    zsock_destroy (&self->pipe_backend_user);
    zsock_destroy (&self->pipe_frontend_user);
err_pipe_frontend_user_alloc:
    zsock_destroy (&self->pipe_backend);
    zsock_destroy (&self->pipe_frontend);
err_pipe_frontend_alloc:
    free (self->pipe_payload);
err_pipe_payload_alloc:
    free (self->pipes_config);
err_pipes_config_alloc:
    free (self->pipes_msg);
err_pipes_msg_alloc:
    free (self->pipes_mgmt);
err_pipes_mgmt_alloc:
    free (self->log_file);
err_log_file:
    free (self);
err_self_alloc:
    return NULL;
}

/* Destroy an instance of the Device Information */
devio_err_e devio_destroy (devio_t **self_p)
{
    assert (self_p);

    if (*self_p) {
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO,
                "[dev_io_core:destroy] Destroying DEVIO instance\n");
        devio_t *self = *self_p;

        /* Destroy children threads before proceeding */
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO,
                "[dev_io_core:destroy] Destroying sm_io_cfg_h\n");
        _devio_destroy_smio_all (self, self->sm_io_cfg_h);
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO,
                "[dev_io_core:destroy] Destroying sm_io_h\n");
        _devio_destroy_smio_all (self, self->sm_io_h);
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO,
                "[dev_io_core:destroy] All SMIOs destroyed\n");

        /* Starting destructing by the last resource */
        /* Notice that we destroy the worker first, as to
         * unregister from broker as soon as possible to avoid
         * loosing requests from clients */
        disp_table_destroy (&self->disp_table_thsafe_ops);
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO,
                "[dev_io_core:destroy] Destroying sm_io_cfg_h hash\n");
        zhashx_destroy (&self->sm_io_cfg_h);
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO,
                "[dev_io_core:destroy] Destroying sm_io_h hash\n");
        zhashx_destroy (&self->sm_io_h);
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO,
                "[dev_io_core:destroy] All hashes destroyed\n");

        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO,
                "[dev_io_core:destroy] Destroying SDBFS\n");
        sdbfs_dev_destroy (self->sdbfs);
        free (self->sdbfs);

        self->thsafe_server_ops = NULL;
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO,
                "[dev_io_core:destroy] Releasing LLIO\n");
        llio_release (self->llio, NULL);
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO,
                "[dev_io_core:destroy] Destroying LLIO\n");
        llio_destroy (&self->llio);
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO,
                "[dev_io_core:destroy] LLIO destroyed\n");

        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO,
                "[dev_io_core:destroy] Destroying general operation handlers\n");
        zlistx_destroy (&self->ops->sig_ops);
        free (self->ops);
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO,
                "[dev_io_core:destroy] General operation handlers destroyed\n");

        free (self->endpoint_broker);
        free (self->name);
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO,
                "[dev_io_core:destroy] Destroying loops\n");
        zloop_destroy (&self->loop_user);
        zloop_timer_end (self->loop, self->timer_id);
        zloop_destroy (&self->loop);

        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO,
                "[dev_io_core:destroy] Destroying front/back end PIPEs\n");
        zsock_destroy (&self->pipe_backend);
        zsock_destroy (&self->pipe_frontend);

        /* Do not destroy PIPE as CZMQ actor thread will do it.
         * See github issue #116 (https://github.com/lnls-dig/halcs/issues/116)
         *
         *  zsock_destroy(&self->pipe);
         *  zsock_destroy(&self->pipe_user);
         * */

        /* Destroy all remamining sockets if any */
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO,
                "[dev_io_core:destroy] Destroying all actors\n");
	zactor_destroy (&self->pipe_payload);
        uint32_t i;
        for (i = 0; i < NODES_MAX_LEN; ++i) {
            DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO,
                    "[dev_io_core:destroy] Destroying possible remaining actors, instance #%u\n", i);
            zactor_destroy (&self->pipes_config [i]);
            zsock_destroy (&self->pipes_msg [i]);
            zactor_destroy (&self->pipes_mgmt [i]);
        }

        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO,
                "[dev_io_core:destroy] All actors destroyed\n");
    	free (self->pipe_payload);
        free (self->pipes_config);
        free (self->pipes_msg);
        free (self->pipes_mgmt);
        free (self->log_file);
        free (self);
        *self_p = NULL;
    }

    return DEVIO_SUCCESS;
}

/* Read specific information about the device. Typically,
 * this is stored in the SDB structure inside the device */
devio_err_e devio_print_info (devio_t *self)
{
    assert (self);
    devio_err_e err = DEVIO_SUCCESS;

    /* FIXME: Only valid for PCIe devices */
    ASSERT_TEST (streq (llio_get_ops_name (self->llio), "PCIE"),
            "SDB is only supported for PCIe devices",
            err_sdb_not_supp, DEVIO_ERR_FUNC_NOT_IMPL);

    /* Print SDB */
    DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO,
            "[devio_print_info] SDB information:\n");
    sdbutils_do_list (self->sdbfs, 1);
    return err;

err_sdb_not_supp:
    return err;
}

static volatile const smio_mod_dispatch_t *_devio_search_sm_by_id (devio_t *self,
        uint32_t smio_id)
{
    (void) self;

    const smio_mod_dispatch_t *smio_mod_handler;

    for_each_smio(smio_mod_handler) {
        if (smio_mod_handler->id == smio_id) {
            return smio_mod_handler;
        }
    }

    return NULL;
}

/* From Malamute https://github.com/zeromq/malamute/blob/master/src/mlm_server_engine.inc
 *
 * Poll actor or zsock for activity, invoke handler on any received
 * message. Handler must be a CZMQ zloop_fn function; receives server
 * as arg. */

static devio_err_e _devio_engine_handle_socket (devio_t *devio, zloop_t *loop, 
        void *pipe_frontend, void *sock, zloop_reader_fn handler)
{
    devio_err_e err = DEVIO_SUCCESS;

    if (devio) {
        devio_t *self = devio;

        /*  Resolve zactor_t -> zsock_t */
        if (zactor_is (sock)) {
            sock = zactor_sock ((zactor_t *) sock);
        }
        else {
            /* Socket reference must be of type zsock */
            ASSERT_TEST(zsock_is (sock), "Invalid socket reference",
                    err_zsock_is, DEVIO_ERR_INV_SOCKET);
        }

        if (handler != NULL) {
            /* Register handler "handler "to socket "sock" and pass argument
             * "self" to the handler */
            int rc = zloop_reader (loop, (zsock_t *) sock, handler, self);
            ASSERT_TEST(rc == 0, "Could not register zloop_reader",
                    err_zloop_reader, DEVIO_ERR_ALLOC);
            zloop_reader_set_tolerant (loop, (zsock_t *) sock);

            /* Send message to pipe_backend to force zloop to rebuild poll_set */
            zstr_sendx (pipe_frontend, "$REBUILD_POLL", NULL);
        }
        else {
            zloop_reader_end (loop, (zsock_t *) sock);
        }
    }

err_zloop_reader:
err_zsock_is:
    return err;
}

/************************************************************/
/********************** zloop handlers **********************/
/************************************************************/

/* zloop handler for timer */
static int _devio_handle_timer (zloop_t *loop, int timer_id, void *arg)
{
    (void) loop;
    (void) timer_id;
    (void) arg;

    return 0;
}

/* zloop handler for MSG PIPE */
static int _devio_handle_pipe_msg (zloop_t *loop, zsock_t *reader, void *args)
{
    (void) loop;
    /* We expect a devio instance e as reference */
    devio_t *devio = (devio_t *) args;
    (void) devio;

    /* We process as many messages as we can, to reduce the overhead
     * of polling and the reactor */
    while (zsock_events (reader) & ZMQ_POLLIN) {
        /* Receive message */
        zmsg_t *recv_msg = zmsg_recv (reader);
        if (recv_msg == NULL) {
            return -1; /* Interrupted */
        }

        /* Prepare the args structure */
        zmq_server_args_t server_args = {
            .tag = ZMQ_SERVER_ARGS_TAG,
            .msg = &recv_msg,
            .reply_to = reader};
        /* Do the actual work */
        _devio_do_smio_op (devio, &server_args);

        /* Cleanup */
        zmsg_destroy (&recv_msg);
    }

    return 0;
}

static int _devio_handle_pipe_mgmt (zloop_t *loop, zsock_t *reader, void *args)
{
    (void) loop;

    /* We expect a devio instance e as reference */
    devio_t *devio = (devio_t *) args;
    /* Arguments for command */
    char *command = NULL;
    uint32_t smio_id;
    uint64_t base;
    uint32_t inst_id;
    uint32_t dest_smio_id;
    uint32_t dest_inst_id;
    char *msg = NULL;

    /* This command expects the following or less (depending on command) */
    /* Command: (string)   $REGISTER_SMIO | $MGMT_MSG_SMIO
     * Arg1:    (uint32_t) smio_id
     * Arg2:    (uint64_t) base
     * Arg3:    (uint32_t) inst_id
     * Arg4:    (uint32_t) dest_smio_id
     * Arg5:    (uint32_t) dest_inst_id
     * Arg6:    (string)   message
     * */
    int zerr = zsock_recv (reader, "s48444s", &command, &smio_id, &base, &inst_id,
        &dest_smio_id, &dest_inst_id, &msg);
    if (zerr == -1) {
        return 0; /* Malformed message */
    }

    if (streq (command, "$REGISTER_SMIO")) {
        /* Register new SMIO */
        _devio_register_sm_raw (devio, smio_id, base, inst_id, false);
    }
    else if (streq (command, "$MGMT_MSG_SMIO")) {
        /* Register new SMIO */
        _devio_send_smio_mgmt_msg_raw (devio, smio_id, base, inst_id, dest_smio_id,
            dest_inst_id, msg);
    }
    else {
        /* Invalid message received. Discard message and continue normally */
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_WARN, "[dev_io_core:_devio_handle_pipe_mgmt] PIPE "
                "received an invalid command\n");
    }

    free (command);
    free (msg);
    return 0;
}

/* zloop handler for CFG PIPE */
static int _devio_handle_pipe_cfg (zloop_t *loop, zsock_t *reader, void *args)
{
    (void) loop;

    int err = 0;
    char *service_id = NULL;
    char *command = NULL;
    /* We expect a devio instance e as reference */
    devio_t *devio = (devio_t *) args;

    /* We process as many messages as we can, to reduce the overhead
     * of polling and the reactor */
    /* Receive message */
    zmsg_t *recv_msg = zmsg_recv (reader);
    if (recv_msg == NULL) {
        return -1; /* Interrupted */
    }

    service_id = zmsg_popstr (recv_msg);
    ASSERT_TEST(service_id != NULL, "devio_loop: received NULL service_id string",
            err_poller_config_null_service, -1);
    command = zmsg_popstr (recv_msg);
    ASSERT_TEST(command != NULL, "devio_loop: poller_config received NULL command string",
            err_poller_config_null_command, -1);

    /* CONFIG DONE means the config thread is finished and should
     * be destroyed */
    if (streq (command, "CONFIG DONE")) {
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO,
                "[dev_io_core:poll_all_sm] Config thread signalled "
                "CONFIG DONE. Terminating thread\n");
        /* Terminate config thread */
        zstr_sendx (reader, "$TERM", NULL);
        /* Lastly, destroy the actor */
        err = _devio_destroy_smio (devio, devio->sm_io_cfg_h, service_id);
        ASSERT_TEST(err == DEVIO_SUCCESS, "devio_loop: Could not destroy SMIO",
                err_poller_destroy_cfg_smio, -1);
    }

err_poller_destroy_cfg_smio:
err_poller_config_null_command:
err_poller_config_null_service:
    free (command);
    command = NULL;
    free (service_id);
    service_id = NULL;
    zmsg_destroy (&recv_msg);

    /* TODO. Do we really need to exit on error? */
    if (err != 0) {
        return err;
    }

    return err;
}

/* zloop handler for PIPE. */
static int _devio_handle_pipe (zloop_t *loop, zsock_t *reader, void *args)
{
    (void) loop;

    /* We expect a devio instance e as reference */
    devio_t *devio = (devio_t *) args;
    char *command = NULL;
    uint32_t smio_id;
    uint64_t base;
    uint32_t inst_id;

    /* This command expects one of the following */
    /* Command: (string) $REGISTER_SMIO
     * Arg1:    (uint32_t) smio_id
     * Arg2:    (uint64_t) base
     * Arg3:    (uint32_t) inst_id
     *
     * Command: (string) $TERM
     *
     * Either way, the following zsock_recv is able to handle both cases. In
     * case of the received message is shorter than the first command, the
     * additional pointers are zeroed.
     * */

    int zerr = zsock_recv (reader, "s484", &command, &smio_id, &base, &inst_id);
    if (zerr == -1) {
        return 0; /* Malformed message */
    }

    if (streq (command, "$TERM")) {
        /* Shutdown the engine */
        free (command);
        return -1;
    }
    else if (streq (command, "$REGISTER_SMIO_ALL")) {
        /* Register all SMIOs */
        _devio_register_all_sm_raw (devio);
    }
    else if (streq (command, "$REGISTER_SMIO_BY_ID")) {
        /* Register new SMIO */
        _devio_register_sm_by_id_raw (devio, smio_id);
    }
    else if (streq (command, "$REGISTER_SMIO")) {
        /* Register new SMIO */
        _devio_register_sm_raw (devio, smio_id, base, inst_id, false);
    }
    else if (streq (command, "$UNREGISTER_SMIO_ALL")) {
        /* Unregister all SMIOs */
        _devio_unregister_all_sm_raw (devio);
    }
    /* FIXME: Will not work, as the second parameter is a string and we expect
     * something different */
    else if (streq (command, "$UNREGISTER_SMIO")) {
        /* Unregister SMIO */
        _devio_unregister_sm_raw (devio, NULL);
    }
    else {
        /* Invalid message received. Discard message and continue normally */
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_WARN, "[dev_io_core:_devio_handle_pipe] PIPE "
                "received an invalid command\n");
    }

    free (command);
    return 0;
}

/* zloop handler for PIPE backend */
static int _devio_handle_pipe_backend (zloop_t *loop, zsock_t *reader, void *args)
{
    (void) loop;

    char *command = NULL;
    /* We expect a devio instance e as reference */
    devio_t *devio = (devio_t *) args;
    (void) devio;

    /* Receive message */
    zmsg_t *recv_msg = zmsg_recv (reader);
    if (recv_msg == NULL) {
        return -1; /* Interrupted */
    }

    command = zmsg_popstr (recv_msg);
    if (streq (command, "$REBUILD_POLL")) {
        /* If we are executing this is because zloop interrupted and will rebuild the poll
         * set as soon as this handler exits. So, we don't actually need to do anything
         * here */
        free (command);
        zmsg_destroy (&recv_msg);
        return 0;
    }
    else {
        /* Invalid message received. Discard message and continue normally */
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_WARN, "[dev_io_core:_devio_handle_pipe_backend] PIPE "
                "received an invalid command\n");
    }

    zmsg_destroy (&recv_msg);
    return 0;
}

/************************************************************/
/*********************** API methods ************************/
/************************************************************/

/* Register a specific sm_io module to this device */
static devio_err_e _devio_register_sm_by_id_raw (devio_t *self, uint32_t smio_id)
{
    uint64_t base = sdbfs_find_id(self->sdbfs, LNLS_VENDOR_ID, smio_id);

    /* Register the sm_io module using the retrieved base address, and
     * automatically assign an instance id to it */
    return _devio_register_sm_raw(self, smio_id, base, 0, true);
}

static devio_err_e _devio_send_smio_mgmt_msg_raw (devio_t *self, uint32_t smio_id, 
        uint64_t base, uint32_t inst_id, uint32_t dest_smio_id, uint32_t dest_inst_id, 
        char *msg)
{
    assert (self);
    devio_err_e err = DEVIO_SUCCESS;

    /* Search for specified PIPE by using the dest_smio_id and dest_inst_id */
    const volatile smio_mod_dispatch_t *smio_mod_handler = _devio_search_sm_by_id (self, 
        dest_smio_id);
    ASSERT_TEST (smio_mod_handler != NULL, "Could not find specified SMIO "
            "for sending MGMT message", err_search_smio, DEVIO_ERR_NO_SMIO_ID);

    /* Get key to search in hash */
    char *dest_smio_key = _devio_gen_smio_key (self, smio_mod_handler, 
        dest_inst_id);
    ASSERT_ALLOC (dest_smio_key, err_key_alloc);

    /* Finally, do the lookup */
    zactor_t *dest_smio_actor = zhashx_lookup (self->sm_io_h, dest_smio_key);
    ASSERT_TEST (dest_smio_actor != NULL,
            "Could not find destination SMIO PIPE for sending MGMT message",
            err_inv_key);

    /* Send message */
    int zerr = zsock_send (dest_smio_actor, "s48444s", "$MGMT_MSG_SMIO", smio_id,
        base, inst_id, dest_smio_id, dest_inst_id, msg);
    ASSERT_TEST(zerr == 0, "Could not send MGMT message", err_send_mgmt_msg,
            DEVIO_ERR_INV_SOCKET /* TODO: improve error handling? */);

err_send_mgmt_msg:
err_inv_key:
    free (dest_smio_key);
err_key_alloc:
err_search_smio:
    return err;
}

devio_err_e devio_register_sm_by_id (void *pipe, uint32_t smio_id)
{
    assert (pipe);
    devio_err_e err = DEVIO_SUCCESS;

    int zerr = zsock_send (pipe, "s4", "$REGISTER_SMIO_BY_ID", smio_id);
    ASSERT_TEST(zerr == 0, "Could not register SMIO", err_register_sm,
            DEVIO_ERR_INV_SOCKET /* TODO: improve error handling? */);

err_register_sm:
    return err;
}

/* Register an specific sm_io modules to this device */
static devio_err_e _devio_register_sm_raw (devio_t *self, uint32_t smio_id, uint64_t base,
        uint32_t inst_id, bool auto_inst_id)
{
    assert (self);

    devio_err_e err = DEVIO_ERR_ALLOC;
    ASSERT_TEST (self->nnodes <= NODES_MAX_LEN, "Maximum number of SMIOs reached",
            err_max_smios_reached, DEVIO_ERR_MAX_SMIOS);

    /* Search the sm_io_mod_dsapatch table for the smio_id and,
     * if found, call the correspondent bootstrap code to initilize
     * the sm_io module */
    uint32_t pipe_mgmt_idx = 0;
    uint32_t pipe_msg_idx = 0;
    uint32_t pipe_config_idx = 0;
    volatile const smio_mod_dispatch_t *smio_mod_handler = NULL;

    DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE,
            "[dev_io_core:register_sm] searching for SMIO ID match\n");

    /* Search for the SMIO */
    smio_mod_handler = _devio_search_sm_by_id (self, smio_id);
    ASSERT_TEST (smio_mod_handler != NULL, "Could not find specified SMIO",
            err_search_smio);

    /* Found! Call bootstrap code and insert in
     * hash table */

    /* Try to generate unique key for the SMIO. */
    uint32_t used_inst_id = 0;
    char *key = _devio_gen_smio_key_auto (self, smio_mod_handler, inst_id,
            auto_inst_id, &used_inst_id);
    ASSERT_ALLOC (key, err_key_alloc);

    /* Check if this genrated key is valid */
    ASSERT_TEST (zhashx_lookup (self->sm_io_h, key) == NULL,
            "Invalid generated key. Possible duplicated value",
            err_inv_key);

    DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE,
            "[dev_io_core:register_sm] Allocating thread args\n");

    /* Increment PIPE indexes */
    pipe_mgmt_idx = self->nnodes++;
    pipe_msg_idx = pipe_mgmt_idx;
    pipe_config_idx = pipe_mgmt_idx;

    /* Create PIPE message to talk to SMIO */
    zsock_t *pipe_msg_backend;
    self->pipes_msg [pipe_msg_idx] = zsys_create_pipe (&pipe_msg_backend);
    ASSERT_TEST (self->pipes_msg [pipe_msg_idx] != NULL, "Could not create message PIPE",
            err_create_pipe_msg);

    /* Register socket handlers */
    err = _devio_engine_handle_socket (self, self->loop, self->pipe_frontend,
            self->pipes_msg [pipe_msg_idx], _devio_handle_pipe_msg);
    ASSERT_TEST (err == DEVIO_SUCCESS, "Could not register message socket handler",
            err_pipes_msg_handle);

    /* Alloacate thread arguments struct and pass it to the
     * thread. It is the responsability of the calling thread
     * to clear this structure after using it! */
    th_boot_args_t *th_args = zmalloc (sizeof *th_args);
    ASSERT_ALLOC (th_args, err_th_args_alloc);
    th_args->parent = self;
    th_args->smio_handler = smio_mod_handler;
    th_args->pipe_msg = pipe_msg_backend;
    th_args->broker = self->endpoint_broker;
    th_args->service = self->name;
    th_args->verbose = self->verbose;
    th_args->base = base;
    th_args->inst_id = used_inst_id;

    DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE,
            "[dev_io_core:register_sm] Calling boot func for SMIO \"%s\" @ %016"PRIX64", instance %u\n",
            smio_mod_handler->name, base, used_inst_id);

    self->pipes_mgmt [pipe_mgmt_idx] = zactor_new (smio_startup, th_args);
    ASSERT_TEST (self->pipes_mgmt [pipe_mgmt_idx] != NULL, "Could not spawn SMIO thread",
            err_spawn_smio_thread);

    err = _devio_engine_handle_socket (self, self->loop, self->pipe_frontend, 
            self->pipes_mgmt [pipe_mgmt_idx], _devio_handle_pipe_mgmt);
    ASSERT_TEST (err == DEVIO_SUCCESS, "Could not register management socket handler",
            err_pipes_mgmt_handle);

    DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE,
            "[dev_io_core:register_sm] Inserting hash with key: %s\n", key);
    int zerr = zhashx_insert (self->sm_io_h, key, &self->pipes_mgmt [pipe_mgmt_idx]);
    /* We must not fail here, as we will loose our reference to the SMIO
     * thread otherwise */
    ASSERT_TEST (zerr == 0, "Could not insert PIPE hash key. Duplicated value?",
            err_pipe_hash_insert);

    /* Configure default values of the recently created SMIO using the
     * bootstrap registered function config_defaults () */

    /* Now, we create a short lived thread just to configure our SMIO */
    /* Allocate config thread arguments struct and pass it to the
     * thread. It is the responsability of the calling thread
     * to clear this structure after using it! */
    th_config_args_t *th_config_args = zmalloc (sizeof *th_config_args);
    ASSERT_ALLOC (th_config_args, err_th_config_args_alloc);

    th_config_args->broker = self->endpoint_broker;
    th_config_args->smio_handler = smio_mod_handler;
    th_config_args->service = self->name;
    th_config_args->log_file = self->log_file;
    th_config_args->inst_id = used_inst_id;

    /* Create actor just for configuring the new recently created SMIO. We will
       check for its end later on poll_all_sm function */
    self->pipes_config [pipe_config_idx] = zactor_new (smio_config_defaults,
            th_config_args);
    ASSERT_TEST (self->pipes_config [pipe_config_idx] != NULL,
            "Could not spawn config thread", err_spawn_config_thread);

    DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE,
            "[dev_io_core:register_sm] Inserting config hash with key: %s\n", key);
    zerr = zhashx_insert (self->sm_io_cfg_h, key, &self->pipes_config [pipe_config_idx]);
    /* We must not fail here, as we will loose our reference to the SMIO
     * thread otherwise */
    ASSERT_TEST (zerr == 0, "Could not insert Config PIPE hash key. Duplicated value?",
            err_cfg_pipe_hash_insert);

    /* key is not needed anymore, as all the hashes have taken a copy of it */
    free (key);
    key = NULL;

    /* Register socket handlers */
    err = _devio_engine_handle_socket (self, self->loop, self->pipe_frontend,
            self->pipes_config [pipe_config_idx], _devio_handle_pipe_cfg);
    ASSERT_TEST (err == DEVIO_SUCCESS, "Could not register message socket handler",
            err_pipes_cfg_handle);

    return DEVIO_SUCCESS;

err_pipes_cfg_handle:
    zhashx_delete (self->sm_io_cfg_h, key);
err_cfg_pipe_hash_insert:
    /* If we can't insert the SMIO thread key in hash,
     * destroy it as we won't have a reference to it later! */
    _devio_destroy_actor (self, &self->pipes_config [pipe_config_idx]);
err_spawn_config_thread:
    /* FIXME: Destroy SMIO thread as we could configure it? */
    free (th_config_args);
err_th_config_args_alloc:
    zhashx_delete (self->sm_io_h, key);
err_pipe_hash_insert:
    _devio_engine_handle_socket (self, self->loop, self->pipe_frontend, 
        self->pipes_mgmt [pipe_mgmt_idx], NULL);
err_pipes_mgmt_handle:
    /* If we can't insert the SMIO thread key in hash,
     * destroy it as we won't have a reference to it later! */
    _devio_destroy_actor (self, &self->pipes_mgmt [pipe_mgmt_idx]);
err_spawn_smio_thread:
    free (th_args);
err_th_args_alloc:
    _devio_engine_handle_socket (self, self->loop, self->pipe_frontend, 
        self->pipes_msg [pipe_msg_idx], NULL);
err_pipes_msg_handle:
    zsock_destroy (&self->pipes_msg [pipe_msg_idx]);
    zsock_destroy (&pipe_msg_backend);
err_create_pipe_msg:
err_inv_key:
    free (key);
err_key_alloc:
err_search_smio:
err_max_smios_reached:
    return err;
}

devio_err_e devio_register_sm (void *pipe, uint32_t smio_id, uint64_t base,
        uint32_t inst_id)
{
    assert (pipe);
    devio_err_e err = DEVIO_SUCCESS;

    int zerr = zsock_send (pipe, "s484", "$REGISTER_SMIO", smio_id, base,
            inst_id);
    ASSERT_TEST(zerr == 0, "Could not register SMIO", err_register_sm,
            DEVIO_ERR_INV_SOCKET /* TODO: improve error handling? */);

err_register_sm:
    return err;
}

/* Register all sm_io module that this device can handle,
 * according to the device information stored in the SDB */
static devio_err_e _devio_register_all_sm_raw (devio_t *self)
{
    assert (self);
    devio_err_e err = DEVIO_SUCCESS;
    struct sdb_device *d;
    struct sdb_product *p;
    struct sdb_component *c;
    uint32_t smio_id = 0;

    /* FIXME: Only valid for PCIe devices */
    ASSERT_TEST (streq (llio_get_ops_name (self->llio), "PCIE"),
            "SDB is only supported for PCIe devices",
            err_sdb_not_supp, DEVIO_ERR_FUNC_NOT_IMPL);

    /* New SDBFS scan: get the interconnect and ignore it */
    sdbfs_scan (self->sdbfs, 1);
    /* Iterate over all SDB devices */
    while ((d = sdbutils_next_device (self->sdbfs)) != NULL) {
        c = &d->sdb_component;
        p = &c->product;
        smio_id = ntohl(p->device_id);

        /* Try to register SMIO. If not found, nothing is done. Also,
         * alloc the next available inst_id for this SMIO (if already present) */
        uint64_t llio_sdb_prefix_addr = llio_get_sdb_prefix_addr (self->llio);
        uint64_t smio_base_addr = (long long) self->sdbfs->base[self->sdbfs->depth] + ntohll(c->addr_first);
        uint64_t smio_full_base_addr = llio_sdb_prefix_addr | smio_base_addr;
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE,
                "[dev_io_core:register_all_sm_raw] Calling register_sm_raw () for smio_id %u @ %016"PRIX64"\n",
                smio_id, smio_full_base_addr);
        _devio_register_sm_raw (self, smio_id, llio_sdb_prefix_addr |
                smio_full_base_addr, 0, true);
    }

err_sdb_not_supp:
    return err;
}

devio_err_e devio_register_all_sm (void *pipe)
{
    assert (pipe);
    devio_err_e err = DEVIO_SUCCESS;

    int zerr = zsock_send (pipe, "s", "$REGISTER_SMIO_ALL");
    ASSERT_TEST(zerr == 0, "Could not register all SMIOs", err_register_sm_all,
            DEVIO_ERR_INV_SOCKET /* TODO: improve error handling? */);

err_register_sm_all:
    return err;
}

static devio_err_e _devio_unregister_sm_raw (devio_t *self, const char *smio_key)
{
    /* Don't care for errors here, as the Config actor is probably already
     * gone */
    _devio_destroy_smio (self, self->sm_io_cfg_h, smio_key);
    devio_err_e err = _devio_destroy_smio (self, self->sm_io_h, smio_key);
    ASSERT_TEST(err == DEVIO_SUCCESS, "Could not destroy SMIO",
            err_destroy_smio, DEVIO_ERR_SMIO_DESTROY);

err_destroy_smio:
    return err;
}

devio_err_e devio_unregister_sm (void *pipe, const char *smio_key)
{
    assert (pipe);
    devio_err_e err = DEVIO_SUCCESS;

    int zerr = zsock_send (pipe, "ss", "$UNREGISTER_SMIO", smio_key);
    ASSERT_TEST(zerr == 0, "Could not unregister SMIOs", err_unregister_sm,
            DEVIO_ERR_INV_SOCKET /* TODO: improve error handling? */);

err_unregister_sm:
    return err;
}

static devio_err_e _devio_unregister_all_sm_raw (devio_t *self)
{
    devio_err_e err = _devio_destroy_smio_all (self, self->sm_io_cfg_h);
    ASSERT_TEST(err == DEVIO_SUCCESS, "Could not destroy Config SMIOs",
            err_destroy_cfg_smios, DEVIO_ERR_SMIO_DESTROY);
    err = _devio_destroy_smio_all (self, self->sm_io_h);
    ASSERT_TEST(err == DEVIO_SUCCESS, "Could not destroy SMIOs",
            err_destroy_smios, DEVIO_ERR_SMIO_DESTROY);

err_destroy_cfg_smios:
err_destroy_smios:
    return err;
}

devio_err_e devio_unregister_all_sm (void *pipe)
{
    assert (pipe);
    devio_err_e err = DEVIO_SUCCESS;

    int zerr = zsock_send (pipe, "s", "$UNREGISTER_SMIO_ALL");
    ASSERT_TEST(zerr == 0, "Could not unregister all SMIOs", err_unregister_sm_all,
            DEVIO_ERR_INV_SOCKET /* TODO: improve error handling? */);

err_unregister_sm_all:
    return err;
}

/* Main devio loop implemented as actor */
void devio_loop (zsock_t *pipe, void *args)
{
    assert (args);

    /* Initialize */
    devio_t *self = (devio_t *) args;
    self->pipe_user = pipe;

    /* 
     * Here we create the backend loop called "loop" to take
     * care of payload messages and SMIO configuration. Apart 
     * from that, and running on a separate thread, we create 
     * a loop called "loop_user" to take care of user commands.
     * This avoids a potential deadlock on that the user sent a
     * command to spawn a new SMIO, but the SMIO itself sends
     * a message this loop. As the loop is synchronous, processing 
     * one message at a time, this will deadlock.
     *
     * Also, there is no race-conditions here, as no data is
     * shared between these two actors.
     */
    self->pipe_payload = zactor_new (_devio_payload_loop, self);
    ASSERT_TEST (self->pipe_payload != NULL, "Could not spawn DEVIO payload thread",
            err_spawn_devio_payload_thread);

    /* Set-up server register commands handler */
    _devio_engine_handle_socket (self, self->loop_user, self->pipe_frontend_user, 
        self->pipe_user, _devio_handle_pipe);

    /* Tell parent we are ready */
    zsock_signal (self->pipe_user, 0);

    /* Run reactor until there's a termination signal */
    zloop_start (self->loop_user);

    /* Cleanup */
    zactor_destroy (&self->pipe_payload);

err_spawn_devio_payload_thread:
    return;
}

/* Payload loop implemented as actor */
static void _devio_payload_loop (zsock_t *pipe, void *args)
{
    assert (args);

    /* Initialize */
    devio_t *self = (devio_t *) args;
    self->pipe = pipe;

    /* Tell parent we are ready */
    zsock_signal (self->pipe, 0);

    /* Set-up server register commands handler */
    _devio_engine_handle_socket (self, self->loop, self->pipe_frontend, 
        self->pipe, _devio_handle_pipe);

    /* Run reactor until there's a termination signal */
    zloop_start (self->loop);
}

devio_err_e devio_do_smio_op (devio_t *self, void *msg)
{
    return _devio_do_smio_op (self, msg);
}

devio_err_e devio_set_llio (devio_t *self, llio_t *llio)
{
    assert (self);
    devio_err_e err = DEVIO_SUCCESS;

    /* Check if the LLIO is already opened. If it is, we can't change the
     * LLIO */
    ASSERT_TEST(llio_get_endpoint_open (self->llio), "Could not set LLIO. "
            "LLIO is open", err_llio_set, DEVIO_ERR_MOD_LLIO);

    llio_err_e lerr = llio_destroy (&self->llio);
    ASSERT_TEST(lerr == LLIO_SUCCESS, "Could not close LLIO",
            err_llio_close, DEVIO_ERR_MOD_LLIO);
    self->llio = llio;

err_llio_close:
err_llio_set:
    return err;
}

llio_t *devio_get_llio (devio_t *self)
{
    assert (self);
    return self->llio;
}

devio_err_e devio_set_sig_handler (devio_t *self, devio_sig_handler_t *sig_handler)
{
    assert (self);
    void *handle = zlistx_add_end (self->ops->sig_ops, sig_handler);

    return (handle == NULL) ? DEVIO_ERR_ALLOC : DEVIO_SUCCESS;
}

static devio_err_e _devio_register_sig_handlers (devio_t *self)
{
    assert (self);
    devio_sig_handler_t *sig_handler =
        (devio_sig_handler_t *) zlistx_first (self->ops->sig_ops);

    /* Register all signal handlers in list*/
    while (sig_handler) {
        struct sigaction act;

        memset (&act, 0, sizeof(act));
        act.sa_sigaction = sig_handler->devio_sig_h;
        act.sa_flags = SA_SIGINFO;

        int err = sigaction (sig_handler->signal, &act, NULL);
        CHECK_ERR(err, DEVIO_ERR_SIGACTION);

        DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_INFO, "[dev_mngr_core] registered signal %d\n",
                sig_handler->signal);

        sig_handler = (devio_sig_handler_t *)
            zlistx_next (self->ops->sig_ops);
    }

    return DEVIO_SUCCESS;
}

devio_err_e devio_register_sig_handlers (devio_t *self)
{
    return _devio_register_sig_handlers (self);
}

/* Declare wrapper for all DEVIO functions API */
#define DEVIO_FUNC_WRAPPER(func_name, ops_err, ...)     \
{                                                       \
    assert (self);                                      \
    CHECK_ERR(((self->ops->func_name == NULL) ? -1 : 0),\
        DEVIO_ERR_FUNC_NOT_IMPL);                       \
    int err = self->ops->func_name (__VA_ARGS__);       \
    CHECK_ERR (err, ops_err);                           \
    return DEVIO_SUCCESS;                               \
}

static devio_err_e _devio_set_wait_clhd_handler (devio_t *self, wait_chld_handler_fp fp)
{
    assert (self);
    self->ops->devio_wait_chld = fp;

    return DEVIO_SUCCESS;
}

devio_err_e devio_set_wait_clhd_handler (devio_t *self, wait_chld_handler_fp fp)
{
    return _devio_set_wait_clhd_handler (self, fp);
}

static devio_err_e _devio_set_wait_clhd_timed_handler (devio_t *self, wait_chld_timed_handler_fp fp)
{
    assert (self);
    self->ops->devio_wait_chld_timed = fp;

    return DEVIO_SUCCESS;
}

devio_err_e devio_set_wait_clhd_timed_handler (devio_t *self, wait_chld_timed_handler_fp fp)
{
    return _devio_set_wait_clhd_timed_handler (self, fp);
}

static devio_err_e _devio_wait_chld (devio_t *self)
    DEVIO_FUNC_WRAPPER(devio_wait_chld, DEVIO_ERR_WAITCHLD)

devio_err_e devio_wait_chld (devio_t *self)
{
    return _devio_wait_chld (self);
}

static devio_err_e _devio_wait_chld_timed (devio_t *self, int timeout)
    DEVIO_FUNC_WRAPPER(devio_wait_chld_timed, DEVIO_ERR_WAITCHLD, timeout)

devio_err_e devio_wait_chld_timed (devio_t *self, int timeout)
{
    return _devio_wait_chld_timed (self, timeout);
}

static devio_err_e _devio_set_spawn_clhd_handler (devio_t *self, spawn_chld_handler_fp fp)
{
    assert (self);
    self->ops->devio_spawn_chld = fp;

    return DEVIO_SUCCESS;
}

devio_err_e devio_set_spawn_clhd_handler (devio_t *self, spawn_chld_handler_fp fp)
{
    return _devio_set_spawn_clhd_handler (self, fp);
}

static devio_err_e _devio_spawn_chld (devio_t *self, const char *program,
        char *const argv[])
    DEVIO_FUNC_WRAPPER(devio_spawn_chld, DEVIO_ERR_SPAWNCHLD, program, argv)

devio_err_e devio_spawn_chld (devio_t *self, const char *program,
        char *const argv[])
{
    return _devio_spawn_chld (self, program, argv);
}

devio_err_e devio_set_ops (devio_t *self, devio_ops_t *devio_ops)
{
    assert (self);
    assert (devio_ops);

    self->ops = devio_ops;

    return DEVIO_SUCCESS;
}

/************************************************************/
/***************** Dispatch table callbacks *****************/
/************************************************************/

static disp_table_err_e _devio_check_msg_args (disp_table_t *disp_table,
        const disp_op_t *disp_op, void *args)
{
    assert (disp_table);
    assert (disp_op);
    assert (args);

    disp_table_err_e err = DISP_TABLE_SUCCESS;

    /* Check if the message is the correct one */
    ASSERT_TEST (msg_guess_type (args) == MSG_THSAFE_ZMQ, "Invalid message tag",
            err_inv_msg, DISP_TABLE_ERR_BAD_MSG);
    msg_err_e merr = msg_check_gen_zmq_args (disp_op, THSAFE_MSG_ZMQ(args));
    ASSERT_TEST (merr == MSG_SUCCESS, "Unrecognized message. Message arguments "
            "checking failed", err_msg_args_check, DISP_TABLE_ERR_BAD_MSG);

err_msg_args_check:
err_inv_msg:
    return err;
}

const disp_table_ops_t devio_disp_table_ops = {
    .check_msg_args = _devio_check_msg_args
};

/************************************************************/
/********************* Helper Functions *********************/
/************************************************************/

static char *_devio_gen_smio_key (devio_t *self,
        const volatile smio_mod_dispatch_t *smio_mod_handler,
        uint32_t inst_id)
{
    (void) self;

    char *key = NULL;
    char *inst_id_str = hutils_stringify_dec_key (inst_id);
    ASSERT_ALLOC(inst_id_str, err_inst_id_str_alloc);

    key = hutils_concat_strings_no_sep (smio_mod_handler->name,
            inst_id_str);
    /* We don't need this anymore */
    free (inst_id_str);
    inst_id_str = NULL;
    /* Return the key nevertheless (succeeding or not) */
    /* ASSERT_ALLOC (key, err_key_alloc); */

err_inst_id_str_alloc:
    return key;
}

static char *_devio_gen_smio_key_auto (devio_t *self,
        const volatile smio_mod_dispatch_t *smio_mod_handler, uint32_t inst_id,
        bool auto_inst_id, uint32_t *used_inst_id)
{
    DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE,
            "[dev_io_core:_devio_gen_smio_key] Generating new SMIO key\n");
    uint32_t avail_inst_id = inst_id;
    char *key = _devio_gen_smio_key (self, smio_mod_handler, avail_inst_id);
    ASSERT_ALLOC (key, err_key_alloc);

    /* If auto_inst_id is set, lookup the current SMIO key to see if it's already
     * there. If it is get the next instance ID available */
    if (auto_inst_id) {
        while (zhashx_lookup (self->sm_io_h, key) != NULL) {
            DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE,
                    "[dev_io_core:_devio_gen_smio_key] Duplicated key %s found. trying the next one\n", key);
            /* Try the next inst_id*/
            avail_inst_id++;
            free (key);
            key = _devio_gen_smio_key (self, smio_mod_handler, avail_inst_id);
            DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE,
                    "[dev_io_core:_devio_gen_smio_key] Next key %s will be tested\n", key);
            ASSERT_ALLOC (key, err_key_alloc);
        }
    }

    *used_inst_id = avail_inst_id;

err_key_alloc:
    return key;
}

static devio_err_e _devio_do_smio_op (devio_t *self, void *msg)
{
    assert (self);
    assert (msg);

    devio_err_e err = DEVIO_SUCCESS;

    disp_table_t *disp_table = self->disp_table_thsafe_ops;
    msg_err_e merr = msg_handle_sock_request (self, msg, disp_table);
    ASSERT_TEST (merr == MSG_SUCCESS, "Error handling request", err_hand_req,
            DEVIO_ERR_SMIO_DO_OP /* returning a more meaningful error? */);

err_hand_req:
    return err;
}

static devio_err_e _devio_destroy_smio_all (devio_t *self, zhashx_t *smio_h)
{
    assert (self);

    devio_err_e err = DEVIO_SUCCESS;
    /* Get all hash keys */
    zlistx_t *hash_keys = zhashx_keys (smio_h);
    ASSERT_ALLOC (hash_keys, err_hash_keys_alloc, DEVIO_ERR_ALLOC);
    char *hash_item = zlistx_first (hash_keys);

    /* Iterate over all keys removing each of one */
    for (; hash_item != NULL; hash_item = zlistx_next (hash_keys)) {
        err = _devio_destroy_smio (self, smio_h, hash_item);
        ASSERT_TEST (err == DEVIO_SUCCESS, "Could not destroy SMIO "
                "instance", err_smio_destroy, DEVIO_ERR_SMIO_DESTROY);
    }

err_smio_destroy:
    zlistx_destroy (&hash_keys);
err_hash_keys_alloc:
    return err;
}

static devio_err_e _devio_destroy_actor (devio_t *self, zactor_t **actor)
{
    assert (self);
    assert (actor);
    assert (*actor);

    devio_err_e err = DEVIO_SUCCESS;
    DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO, "[dev_io_core] Destroying actor %p\n", *actor);

    /* Resolve actor into sock_t */
    zsock_t *sock = zactor_sock (*actor);
    /* Remove sock from loop */
    _devio_engine_handle_socket (self, self->loop, self->pipe_frontend, 
        sock, NULL);
    /* Destroy actor */
    zactor_destroy (actor);

    return err;
}

/* smio_key is the name of the SMIO + instance number, e.g.,
 * FMC130M_4CH0*/
static devio_err_e _devio_destroy_smio (devio_t *self, zhashx_t *smio_h, const char *smio_key)
{
    assert (self);

    devio_err_e err = DEVIO_SUCCESS;
    /* Lookup SMIO reference in hash table */
    zactor_t **actor = (zactor_t **) zhashx_lookup (smio_h, smio_key);
    ASSERT_TEST (actor != NULL, "Could not find SMIO registered with this ID",
            err_hash_lookup, DEVIO_ERR_SMIO_DESTROY);

    err = _devio_destroy_actor (self, actor);
    ASSERT_TEST (err == DEVIO_SUCCESS, "Could not send self-destruct message to "
            "PIPE management", err_send_msg, DEVIO_ERR_SMIO_DESTROY);

    /* Finally, remove the pipe from hash. FIXME: What if the SMIO does not
     * exit? We will loose its reference ...*/
    zhashx_delete (smio_h, smio_key);

err_send_msg:
err_hash_lookup:
    return err;
}

