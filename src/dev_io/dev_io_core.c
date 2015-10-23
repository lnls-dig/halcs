/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "bpm_server.h"
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
#define DEVIO_POLLER_TIMEOUT                100        /* in msec */
#define DEVIO_POLLER_CONFIG_TIMEOUT         0          /* in msec */
#define DEVIO_DFLT_LOG_MODE                 "w"

#define DEVIO_MAX_DESTRUCT_MSG_TRIES        10
#define DEVIO_LINGER_TIME                   100         /* in ms */

struct _devio_t {
    /* General information */
    zactor_t **pipes_mgmt;              /* Address nodes using this array of actors (Management PIPES) */
    zsock_t **pipes_msg;                /* Address nodes using this array of actors (Message PIPES) */
    zactor_t **pipes_config;            /* Address config actors using this array of actors (Config PIPES) */
    zpoller_t *poller;                  /* Poller structure to multiplex threads messages */
    zpoller_t *poller_config;           /* Poller structure to multiplex config threads messages*/
    unsigned int nnodes;                /* Number of actual nodes */
    char *name;                         /* Identification of this worker instance */
    uint32_t id;                        /* ID number of this instance */
    char *log_file;                     /* Log filename for tracing and debugging */
    char *endpoint_broker;              /* Broker location to connect to */
    int verbose;                        /* Print activity to stdout */

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

/* Do the SMIO operation */
static devio_err_e _devio_do_smio_op (devio_t *self, void *msg);
static devio_err_e _devio_send_destruct_msg (devio_t *self, zactor_t **actor);
static devio_err_e _devio_destroy_smio (devio_t *self, zhashx_t *smio_h, const char *smio_key);
static devio_err_e _devio_destroy_smio_all (devio_t *self, zhashx_t *smio_h);

/* Creates a new instance of Device Information */
devio_t * devio_new (char *name, uint32_t id, char *endpoint_dev,
        llio_type_e type, char *endpoint_broker, int verbose,
        const char *log_file_name)
{
    assert (name);
    assert (endpoint_dev);
    assert (endpoint_broker);

    /* Set logfile available for all dev_mngr and dev_io instances.
     * We accept NULL as a parameter, meaning to suppress all messages */
    errhand_set_log (log_file_name, DEVIO_DFLT_LOG_MODE);

    char *dev_type_c = llio_type_to_str (type);
    DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO, "[dev_io_core] Spawing DEVIO worker"
            " with exported service %s, for a %s device \n\tlocated on %s,"
            " broker address %s, with logfile on %s ...\n", name, dev_type_c,
            endpoint_dev, endpoint_broker, (log_file_name == NULL) ? "NULL" : log_file_name);
    free (dev_type_c);

    /* Print Software info */
    DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO, "[dev_io_core] BPM Device I/O version %s,"
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
    /* 0 nodes for now... */
    self->nnodes = 0;

    /* Setup pollers */
    self->poller = zpoller_new (NULL);
    ASSERT_ALLOC(self->poller, err_poller_alloc);
    self->poller_config = zpoller_new (NULL);
    ASSERT_ALLOC(self->poller_config, err_poller_config_alloc);

    /* Setup strings/options */
    self->name = strdup (name);
    ASSERT_ALLOC(self->name, err_name_alloc);

    /* Setup ID number */
    self->id = id;

    /* Setup Broker endpoint */
    self->endpoint_broker = strdup (endpoint_broker);
    ASSERT_ALLOC(self->endpoint_broker, err_endp_broker_alloc);
    self->verbose = verbose;

    /* Concatenate recv'ed name with a llio identifier */
    char *llio_name = zmalloc (sizeof (char)*(strlen(name)+strlen(LLIO_STR)+1));
    ASSERT_ALLOC(llio_name, err_llio_name_alloc);
    strcat (llio_name, name);
    strcat (llio_name, LLIO_STR);
    self->llio = llio_new (llio_name, endpoint_dev, type,
            verbose);
    ASSERT_ALLOC(self->llio, err_llio_alloc);

    /* We try to open the device */
    int err = llio_open (self->llio, NULL);
    ASSERT_TEST(err==0, "Error opening device!", err_llio_open);

    /* We can free llio_name now, as llio copies the string */
    free (llio_name);
    llio_name = NULL; /* Avoid double free error */

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
    llio_release (self->llio, NULL);
err_llio_open:
    llio_destroy (&self->llio);
err_llio_alloc:
    free (llio_name);
err_llio_name_alloc:
    free (self->endpoint_broker);
err_endp_broker_alloc:
    free (self->name);
err_name_alloc:
    zpoller_destroy (&self->poller_config);
err_poller_config_alloc:
    zpoller_destroy (&self->poller);
err_poller_alloc:
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
        self->thsafe_server_ops = NULL;
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO,
                "[dev_io_core:destroy] Releasing LLIO\n");
        llio_release (self->llio, NULL);
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO,
                "[dev_io_core:destroy] Destroying LLIO\n");
        llio_destroy (&self->llio);
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO,
                "[dev_io_core:destroy] LLIO destroyed\n");
        free (self->endpoint_broker);
        free (self->name);
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO,
                "[dev_io_core:destroy] Destroying poller_config\n");
        zpoller_destroy (&self->poller_config);
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO,
                "[dev_io_core:destroy] Derstroying poller\n");
        zpoller_destroy (&self->poller);
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO,
                "[dev_io_core:destroy] All pollers destroyed\n");
        /* Destroy all remamining sockets if any */
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO,
                "[dev_io_core:destroy] Destroying all actors\n");
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
        free (self->pipes_config);
        free (self->pipes_msg);
        free (self->pipes_mgmt);
        free (self->log_file);
        free (self);
        *self_p = NULL;
    }

    return DEVIO_SUCCESS;
}

/* FIXME: Only valid for PCIe devices */
#if 0
static int _devio_read_llio_block (struct sdbfs *fs, int offset, void *buf,
        int count)
{
    return llio_read_block (((devio_t *)fs->drvdata)->llio,
            BAR4_ADDR | (offset), count, (uint32_t *) buf);
}
#endif

/* Read specific information about the device. Typically,
 * this is stored in the SDB structure inside the device */
devio_err_e devio_print_info (devio_t *self)
{
    (void) self;
    devio_err_e err = DEVIO_SUCCESS;
/* FIXME: Only valid for PCIe devices */
#if 0
    /* FIXME: Hardcoded non-default SDB address */
#define SDB_ADDRESS             0x00300000UL
    /* The sdb filesystem itself */
    struct sdbfs bpm_fpga_sdb = {
        .name = "fpga-area",
        .drvdata = (void *) self,
        .blocksize = 1, /* Not currently used */
        .entrypoint = SDB_ADDRESS,
        .data = 0,
        .flags = 0,
        .read = _devio_read_llio_block
    };
    struct sdb_device *d;
    int new = 1;

    int serr = sdbfs_dev_create(&bpm_fpga_sdb);
    ASSERT_TEST (serr == 0, "Could not create SDBFS",
            err_sdbfs_create, DEVIO_ERR_SMIO_DO_OP /* FIXME: temporary*/);

    while ( (d = sdbfs_scan(&bpm_fpga_sdb, new)) != NULL) {
        /*
         * "%.19s" is not working for XINT printf, and zeroing
         * d->sdb_component.product.record_type won't work, as
         * the device is read straight from fpga ROM registers
         */
        const int namesize = sizeof(d->sdb_component.product.name);
        char name[namesize + 1];

        memcpy(name, d->sdb_component.product.name, sizeof(name));
        name[namesize] = '\0';
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO, "dev  0x%08lx @ %06lx, %s\n",
                (long)(d->sdb_component.product.device_id),
                bpm_fpga_sdb.f_offset, name);
        new = 0;
    }

    sdbfs_dev_destroy(&bpm_fpga_sdb);
err_sdbfs_create:
#endif
    return err;
}

/* Register an specific sm_io modules to this device */
devio_err_e devio_register_sm (devio_t *self, uint32_t smio_id, uint64_t base,
        uint32_t inst_id)
{
    assert (self);

    devio_err_e err = DEVIO_ERR_ALLOC;
    ASSERT_TEST (self->nnodes <= NODES_MAX_LEN, "Maximum number of SMIOs reached",
            err_max_smios_reached, DEVIO_ERR_MAX_SMIOS);

    /* Search the sm_io_mod_dsapatch table for the smio_id and,
     * if found, call the correspondent bootstrap code to initilize
     * the sm_io module */
    th_boot_args_t *th_args = NULL;
    th_config_args_t *th_config_args = NULL;
    char *key = NULL;
    uint32_t pipe_mgmt_idx = 0;
    uint32_t pipe_msg_idx = 0;
    uint32_t pipe_config_idx = 0;

    DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE,
            "[dev_io_core:register_sm] searching for SMIO ID match\n");

    /* For now, just do a simple linear search. We can afford this, as
     * we don't expect to insert new sm_io modules often */
    unsigned int i;
    for (i = 0; smio_mod_dispatch[i].id != SMIO_DISPATCH_END_ID; ++i) {
        if (smio_mod_dispatch[i].id != smio_id) {
            continue;
        }

        /* Found! Call bootstrap code and insert in
         * hash table */

        /* Stringify ID. We do it before spawning a new thread as
         * alloc can fail */
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE,
                "[dev_io_core:register_sm] Stringify hash ID\n");
        char *inst_id_str = hutils_stringify_dec_key (inst_id);
        ASSERT_ALLOC(inst_id_str, err_inst_id_str_alloc);
        char *key = hutils_concat_strings_no_sep (smio_mod_dispatch[i].name,
                inst_id_str);
        /* We don't need this anymore */
        free (inst_id_str);
        inst_id_str = NULL;
        ASSERT_ALLOC (key, err_key_alloc);

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

        /* Alloacate thread arguments struct and pass it to the
         * thread. It is the responsability of the calling thread
         * to clear this structure after using it! */
        th_boot_args_t *th_args = zmalloc (sizeof *th_args);
        ASSERT_ALLOC (th_args, err_th_args_alloc);
        th_args->parent = self;
        /* FIXME: weak identifier */
        th_args->smio_id = i;
        th_args->pipe_msg = pipe_msg_backend;
        th_args->broker = self->endpoint_broker;
        th_args->service = self->name;
        th_args->verbose = self->verbose;
        th_args->base = base;
        th_args->inst_id = inst_id;

        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE,
                "[dev_io_core:register_sm] Calling boot func\n");

        self->pipes_mgmt [pipe_mgmt_idx] = zactor_new (smio_startup, th_args);
        ASSERT_TEST (self->pipes_mgmt [pipe_mgmt_idx] != NULL, "Could not spawn SMIO thread",
                err_spawn_smio_thread);

        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE,
                "[dev_io_core:register_sm] Inserting hash with key: %s\n", key);
        int zerr = zhashx_insert (self->sm_io_h, key, &self->pipes_mgmt [pipe_mgmt_idx]);
        /* We must not fail here, as we will loose our reference to the SMIO
         * thread otherwise */
        ASSERT_TEST (zerr == 0, "Could not insert PIPE hash key. Duplicated value?",
                err_pipe_hash_insert);

        /* Add actor to message poll */
        zerr = zpoller_add (self->poller, self->pipes_msg [pipe_msg_idx]);
        ASSERT_TEST (zerr == 0, "Could not insert PIPE into poller",
                err_poller_insert);

        /* Configure default values of the recently created SMIO using the
         * bootstrap registered function config_defaults () */

        /* Now, we create a short lived thread just to configure our SMIO */
        /* Allocate config thread arguments struct and pass it to the
         * thread. It is the responsability of the calling thread
         * to clear this structure after using it! */
        th_config_args = zmalloc (sizeof *th_config_args);
        ASSERT_ALLOC (th_config_args, err_th_config_args_alloc);

        th_config_args->broker = self->endpoint_broker;
        /* FIXME: weak identifier */
        th_config_args->smio_id = i;
        th_config_args->service = self->name;
        th_config_args->log_file = self->log_file;
        th_config_args->inst_id = inst_id;

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

        /* Add actor to config poll */
        zerr = zpoller_add (self->poller_config, self->pipes_config [pipe_config_idx]);
        ASSERT_TEST (zerr == 0, "Could not insert PIPE into Config poller",
                err_poller_config_insert);

        /* key is not needed anymore, as all the hashes have taken a copy of it */
        free (key);
        key = NULL;

        /* stop on first match */
        break;
    }

    return DEVIO_SUCCESS;

err_poller_config_insert:
    zhashx_delete (self->sm_io_cfg_h, key);
err_cfg_pipe_hash_insert:
    /* If we can't insert the SMIO thread key in hash,
     * destroy it as we won't have a reference to it later! */
    _devio_send_destruct_msg (self, &self->pipes_config [pipe_config_idx]);
err_spawn_config_thread:
    /* FIXME: Destroy SMIO thread as we could configure it? */
    free (th_config_args);
err_th_config_args_alloc:
    zpoller_remove (self->poller, self->pipes_msg [pipe_msg_idx]);
err_poller_insert:
    zhashx_delete (self->sm_io_h, key);
err_pipe_hash_insert:
    /* If we can't insert the SMIO thread key in hash,
     * destroy it as we won't have a reference to it later! */
    _devio_send_destruct_msg (self, &self->pipes_mgmt [pipe_mgmt_idx]);
err_spawn_smio_thread:
    zsock_destroy (&self->pipes_msg [pipe_msg_idx]);
err_create_pipe_msg:
    free (th_args);
err_th_args_alloc:
    free (key);
err_key_alloc:
err_inst_id_str_alloc:
err_max_smios_reached:
    return err;
}

/* Register all sm_io module that this device can handle,
 * according to the device information stored in the SDB */
devio_err_e devio_register_all_sm (devio_t *self)
{
    (void) self;
    return DEVIO_ERR_FUNC_NOT_IMPL;
}

devio_err_e devio_unregister_sm (devio_t *self, const char *smio_key)
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

devio_err_e devio_unregister_all_sm (devio_t *self)
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

devio_err_e devio_loop (devio_t *self)
{
    assert (self);

    devio_err_e err = DEVIO_SUCCESS;
    char *service_id = NULL;
    char *command = NULL;

    ASSERT_TEST(self->poller, "Unitialized poller!",
            err_uninitialized_poller, DEVIO_ERR_UNINIT_POLLER);
    ASSERT_TEST(self->poller_config, "Unitialized config poller!",
            err_uninitialized_poller_config, DEVIO_ERR_UNINIT_POLLER);

    while (!zsys_interrupted) {
        /* Poll Message sockets */
        zsock_t *which = zpoller_wait (self->poller, DEVIO_POLLER_TIMEOUT);
        ASSERT_TEST(which != NULL || zpoller_expired (self->poller),
                "devio_loop: poller interrupted", err_poller_interrupted,
                DEVIO_ERR_INTERRUPTED_POLLER);

        /* Activity on socket */
        if (which != NULL) {
            /* Receive message */
            zmsg_t *recv_msg = zmsg_recv (which);
            if (recv_msg == NULL) {
                break; /* Interrupted */
            }
            /* Prepare the args structure */
            zmq_server_args_t server_args = {
                .tag = ZMQ_SERVER_ARGS_TAG,
                .msg = &recv_msg,
                .reply_to = which};
            err = _devio_do_smio_op (self, &server_args);

            /* Cleanup */
            zmsg_destroy (&recv_msg);
        }

        /* Poll Config sockets */
        zactor_t *which_config = zpoller_wait (self->poller_config, DEVIO_POLLER_CONFIG_TIMEOUT);
        ASSERT_TEST(which_config != NULL || zpoller_expired (self->poller_config),
                "devio_loop: poller_config interrupted",
                err_poller_config_interrupted, DEVIO_ERR_INTERRUPTED_POLLER);

        /* Activity on socket */
        if (which_config != NULL) {
            /* Remove config actror from poller */
            zpoller_remove (self->poller_config, which_config);
            /* Receive message */
            zmsg_t *recv_msg = zmsg_recv (which_config);
            if (recv_msg == NULL) {
                break; /* Interrupted */
            }

            service_id = zmsg_popstr (recv_msg);
            ASSERT_TEST(service_id != NULL, "devio_loop: received NULL service_id string",
                    err_poller_config_null_service, DEVIO_ERR_BAD_MSG);
            command = zmsg_popstr (recv_msg);
            ASSERT_TEST(command != NULL, "devio_loop: poller_config received NULL command string",
                    err_poller_config_null_command, DEVIO_ERR_BAD_MSG);

            /* CONFIG DONE means the config thread is finished and should
             * be destroyed */
            if (streq (command, "CONFIG DONE")) {
                DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO,
                        "[dev_io_core:poll_all_sm] Config thread signalled "
                        "CONFIG DONE. Terminating thread\n");
                /* Terminate config thread */
                zstr_sendx (which_config, "$TERM", NULL);
                /* Lastly, destroy the actor */
                err = _devio_destroy_smio (self, self->sm_io_cfg_h, service_id);
                ASSERT_TEST(err == DEVIO_SUCCESS, "devio_loop: Could not destroy SMIO",
                        err_poller_destroy_cfg_smio, DEVIO_ERR_SMIO_DESTROY);
            }

err_poller_destroy_cfg_smio:
err_poller_config_null_command:
err_poller_config_null_service:
            free (command);
            command = NULL;
            free (service_id);
            service_id = NULL;
            zmsg_destroy (&recv_msg);
        }
    }

err_poller_config_interrupted:
err_poller_interrupted:
err_uninitialized_poller_config:
err_uninitialized_poller:
    return err;
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

static devio_err_e _devio_send_destruct_msg (devio_t *self, zactor_t **actor)
{
    assert (self);
    assert (actor);
    assert (*actor);

    devio_err_e err = DEVIO_SUCCESS;
    DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO, "[dev_io_core] Destroying actor %p\n", *actor);
    zactor_destroy (actor);

err_msg_alloc:
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

    err = _devio_send_destruct_msg (self, actor);
    ASSERT_TEST (err == DEVIO_SUCCESS, "Could not send self-destruct message to "
            "PIPE management", err_send_msg, DEVIO_ERR_SMIO_DESTROY);

    /* Finally, remove the pipe from hash. FIXME: What if the SMIO does not
     * exit? We will loose its reference ...*/
    zhashx_delete (smio_h, smio_key);

err_send_msg:
err_hash_lookup:
    return err;
}

