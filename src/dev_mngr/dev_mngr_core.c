/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include <glob.h>
#include "bpm_server.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...)  \
    ASSERT_HAL_TEST(test_boolean, DEV_MNGR, "[dev_mngr_core]",  \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)   \
    ASSERT_HAL_ALLOC(ptr, DEV_MNGR, "[dev_mngr_core]",          \
            dmngr_err_str(DMNGR_ERR_ALLOC),                     \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                \
    CHECK_HAL_ERR(err, DEV_MNGR, "[dev_mngr_core]",             \
            dmngr_err_str (err_type))

#define LOG_FILENAME_LEN            50
#define DEVMNGR_LOG_FILENAME        "dev_mngr.log"
#define DEVMNGR_DFLT_LOG_MODE       "w"

#define DEVIO_DEV_NAME_LEN          40

#define DEVIO_BE_DEV_PATTERN        "/dev/fpga%d"
#define DEVIO_BE_DEV_GLOB           "/dev/fpga*"

#define DEVIO_NAME                  "dev_io"

/* FPGA DEVIO */
#define DEVIO_BE_TYPE               BE_DEVIO
#define DEVIO_BE_NAME               "dev_io_be"

/* RFFE DEVIO */
#define DEVIO_FE_TYPE               FE_DEVIO
#define DEVIO_FE_NAME               "dev_io_fe"

/* This composes the log filename as "dev_io%u_fe%u.log" or
 * "dev_io%u_be%u.log" */
#define DEVIO_LOG_RADICAL_PATTERN   "dev_io%u"
#define DEVIO_LOG_DEVIO_MODEL_TYPE  "%s"
#define DEVIO_LOG_INST_TYPE         "%u"
#define DEVIO_LOG_SUFFIX            "log"
#define DEVIO_LOG_FILENAME_PATTERN \
                                    DEVIO_LOG_RADICAL_PATTERN "_" \
                                    DEVIO_LOG_DEVIO_MODEL_TYPE \
                                    DEVIO_LOG_INST_TYPE "." \
                                    DEVIO_LOG_SUFFIX

#define DEVIO_MLM_PREFIX_CFG_DIR    "/usr/local"
#define DEVIO_MLM_CFG_DIR           "/etc/malamute"
#define DEVIO_MLM_CFG_FILENAME      "malamute.cfg"

struct _dmngr_t {
    /* General information */
    zsock_t *dealer;            /* zeroMQ Dealer socket */
    char *name;                 /* Identification of this dmngr instance */
    char *endpoint;             /* Endpoint to connect to */
    char *cfg_file;             /* Configuration file location */
    int verbose;                /* Print activity to stdout */

    /* General management operations */
    dmngr_ops_t *ops;

    /* zeroMQ broker management */
    bool broker_running;        /* true if broker is already running */

    /* Device managment */
    zhashx_t *devio_info_h;
    zhashx_t *hints_h;           /* Config hints from configuration file */
};

/* Configuration variables. To be filled by dev_mngr */
const char *dmngr_log_filename = NULL;
char *dmngr_log_dir = NULL;
char *dmngr_broker_endp = NULL;
char *dmngr_verbose_str = NULL;
int dmngr_verbose = 0;
char *dmngr_daemonize_str = NULL;
int dmngr_daemonize = 0;
char *dmngr_work_dir = NULL;
char *dmngr_spawn_broker_cfg_str = NULL;
int dmngr_spawn_broker_cfg = 0;

static void _devio_hash_free_item (void **data);
static dmngr_err_e _dmngr_scan_devs (dmngr_t *self, uint32_t *num_devs_found);
static dmngr_err_e _dmngr_prepare_devio (dmngr_t *self, const char *key,
        char *dev_pathname, uint32_t id, llio_type_e type,
        devio_type_e devio_type, uint32_t smio_inst_id, devio_state_e state);

/* Creates a new instance of the Device Manager */
dmngr_t * dmngr_new (char *name, char *endpoint, int verbose,
        const char *log_prefix, zhashx_t *hints_h)
{
    assert (name);
    assert (endpoint);

    /* Set up logdir. Defaulting it to stdout */
    char log_filename [LOG_FILENAME_LEN] = "stdout";

    /* TODO: Check for the validity of the log filename */
    if (log_prefix != NULL) {
        snprintf (log_filename, LOG_FILENAME_LEN, "%s/"DEVMNGR_LOG_FILENAME,
                log_prefix);
    }

    /* Set logfile available for all dev_mngr and dev_io instances.
     * We accept NULL as a parameter, meaning to suppress all messages */
    errhand_set_log (log_filename, DEVMNGR_DFLT_LOG_MODE);

    DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_INFO, "[dev_mngr_core] Spawing DEV_MNGR worker"
            " with \n\tdiscovery endpoint (ignored for now) %s, with logfile %s"
            " ...\n", endpoint, (log_filename == NULL) ? "NULL" : log_filename);

    dmngr_t *self = (dmngr_t *) zmalloc (sizeof *self);
    ASSERT_ALLOC(self, err_self_alloc);

    self->name = strdup (name);
    ASSERT_ALLOC(self->name, err_name_alloc);
    self->endpoint = strdup (endpoint);
    ASSERT_ALLOC(self->endpoint, err_endpoint_alloc);
    self->verbose = verbose;

    self->ops = (dmngr_ops_t *) zmalloc (sizeof *self->ops);
    ASSERT_ALLOC(self->ops, err_ops_alloc);
    self->ops->sig_ops = zlistx_new ();
    ASSERT_ALLOC(self->ops->sig_ops, err_list_alloc);

    /* Init devio_info hash */
    self->devio_info_h = zhashx_new ();
    ASSERT_ALLOC(self->devio_info_h, err_devio_info_h_alloc);
    zhashx_set_destructor (self->devio_info_h, _devio_hash_free_item);

    /* FIXME: set duplicator in hints_h before calling dup method. Without a
     * duplicator method set, zhashx_dup () does not duplicate the item and
     * just points to the same reference */
    self->hints_h = zhashx_dup (hints_h);
    ASSERT_ALLOC(self->hints_h, err_hints_h_alloc);

    self->broker_running = false;

    /* Create Dealer for use with zbeacon and bind it to the endpoint */
    self->dealer = zsock_new_dealer (NULL);
    ASSERT_ALLOC(self->dealer, err_dealer_alloc);
    int rc = zsock_bind (self->dealer, "%s", self->endpoint);
    ASSERT_TEST(rc > -1, "Dealer socket could not bind to specified endpoint",
            err_dealer_bind);

    /* Scan devios for the first time */
    uint32_t num_devs_found = 0;
    dmngr_err_e err = _dmngr_scan_devs (self, &num_devs_found);
    ASSERT_TEST(err == DMNGR_SUCCESS, "Could not scan devices", err_scan_devs);

    DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_TRACE,
            "[dev_mngr_core] Found a total of %u device(s)\n", num_devs_found);
    /* Supress compiler warnings if we are not debugging */
    (void) num_devs_found;

    return self;

err_scan_devs:
    zsock_unbind (self->dealer, "%s", endpoint);
err_dealer_bind:
    zsock_destroy (&self->dealer);
err_dealer_alloc:
err_hints_h_alloc:
    zhashx_destroy (&self->devio_info_h);
err_devio_info_h_alloc:
    zlistx_destroy (&self->ops->sig_ops);
err_list_alloc:
    free (self->ops);
err_ops_alloc:
    free (self->endpoint);
err_endpoint_alloc:
    free (self->name);
err_name_alloc:
    free (self);
err_self_alloc:
    return NULL;
}

/* Destroy an instance of the Device Manager */
dmngr_err_e dmngr_destroy (dmngr_t **self_p)
{
    assert (self_p);

    if (*self_p) {
        dmngr_t *self = *self_p;

        /* Starting destructing by the last resource */
        zsock_unbind (self->dealer, "%s", self->endpoint);
        zsock_destroy (&self->dealer);
        zhashx_destroy (&self->hints_h);
        zhashx_destroy (&self->devio_info_h);
        zlistx_destroy (&self->ops->sig_ops);
        free (self->ops);
        free (self->endpoint);
        free (self->name);

        free (self);
        *self_p = NULL;
    }

    return DMNGR_SUCCESS;
}

dmngr_err_e dmngr_set_sig_handler (dmngr_t *self, dmngr_sig_handler_t *sig_handler)
{
    assert (self);
    void *handle = zlistx_add_end (self->ops->sig_ops, sig_handler);

    return (handle == NULL) ? DMNGR_ERR_ALLOC : DMNGR_SUCCESS;
}

dmngr_err_e dmngr_register_sig_handlers (dmngr_t *self)
{
    assert (self);
    dmngr_sig_handler_t *sig_handler =
        (dmngr_sig_handler_t *) zlistx_first (self->ops->sig_ops);

    /* Register all signal handlers in list*/
    while (sig_handler) {
        struct sigaction act;

        memset (&act, 0, sizeof(act));
        act.sa_sigaction = sig_handler->dmngr_sig_h;
        act.sa_flags = SA_SIGINFO;

        int err = sigaction (sig_handler->signal, &act, NULL);
        CHECK_ERR(err, DMNGR_ERR_SIGACTION);

        DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_INFO, "[dev_mngr_core] registered signal %d\n",
                sig_handler->signal);

        sig_handler = (dmngr_sig_handler_t *)
            zlistx_next (self->ops->sig_ops);
    }

    return DMNGR_SUCCESS;
}

/* Declare wrapper for all DMNGR functions API */
#define DMNGR_FUNC_WRAPPER(func_name, ops_err, ...)     \
{                                                       \
    assert (self);                                      \
    CHECK_ERR(((self->ops->func_name == NULL) ? -1 : 0),\
        DMNGR_ERR_FUNC_NOT_IMPL);                       \
    int err = self->ops->func_name (__VA_ARGS__);       \
    CHECK_ERR (err, ops_err);                           \
    return DMNGR_SUCCESS;                               \
}

dmngr_err_e dmngr_set_wait_clhd_handler (dmngr_t *self, wait_chld_handler_fp fp)
{
    assert (self);
    self->ops->dmngr_wait_chld = fp;

    return DMNGR_SUCCESS;
}

static dmngr_err_e _dmngr_wait_chld (dmngr_t *self)
    DMNGR_FUNC_WRAPPER(dmngr_wait_chld, DMNGR_ERR_WAITCHLD)

dmngr_err_e dmngr_wait_chld (dmngr_t *self)
{
    return _dmngr_wait_chld (self);
}

dmngr_err_e dmngr_set_spawn_clhd_handler (dmngr_t *self, spawn_chld_handler_fp fp)
{
    assert (self);
    self->ops->dmngr_spawn_chld = fp;

    return DMNGR_SUCCESS;
}

static dmngr_err_e _dmngr_spawn_chld (dmngr_t *self, const char *program,
        char *const argv[])
    DMNGR_FUNC_WRAPPER(dmngr_spawn_chld, DMNGR_ERR_SPAWNCHLD, program, argv)

dmngr_err_e dmngr_spawn_chld (dmngr_t *self, const char *program,
        char *const argv[])
{
    return _dmngr_spawn_chld (self, program, argv);
}

dmngr_err_e dmngr_set_ops (dmngr_t *self, dmngr_ops_t *dmngr_ops)
{
    assert (self);
    assert (dmngr_ops);

    self->ops = dmngr_ops;

    return DMNGR_SUCCESS;
}

dmngr_err_e dmngr_set_cfg_file (dmngr_t *self, char *cfg_file)
{
    assert (self);
    assert (cfg_file);

    dmngr_err_e err = DMNGR_SUCCESS;

    /* Free previously allocated file */
    if (self->cfg_file != NULL) {
        free (self->cfg_file);
        self->cfg_file = NULL;
    }

    self->cfg_file = strdup (cfg_file);
    ASSERT_ALLOC(self->cfg_file, err_set_cfg_file);

err_set_cfg_file:
    return err;
}

const char * dmngr_get_cfg_file (dmngr_t *self)
{
    assert (self);
    return self->cfg_file;
}

char * dmngr_clone_cfg_file (dmngr_t *self)
{
    assert (self);

    char *cfg_file_cpy = strdup (self->cfg_file);
    return cfg_file_cpy;
}

static bool _dmngr_is_broker_running (dmngr_t *self)
{
    assert (self);
    return self->broker_running;
}

bool dmngr_is_broker_running (dmngr_t *self)
{
    return _dmngr_is_broker_running (self);
}

dmngr_err_e dmngr_spawn_broker (dmngr_t *self, char *broker_endp)
{
    (void) broker_endp;

    assert (self);
    assert (broker_endp);

    dmngr_err_e err = DMNGR_SUCCESS;

    /* Verify if the Broker is running. If not, spawn it */
    /* ASSERT_TEST(!_dmngr_is_broker_running (self), "Broker is already running",
            err_broker_run, DMNGR_ERR_BROK_RUNN);*/
    /* This is not an error */
    if (_dmngr_is_broker_running (self)) {
        goto err_broker_run;
    }

    DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_TRACE, "[dev_mngr_core] Spawning Broker ...\n");

    /* Specify if broker is to be run in verbose mode or not */
    char *argv_exec[] = {"malamute", "-f", DEVIO_MLM_PREFIX_CFG_DIR ""
        DEVIO_MLM_CFG_DIR"/"DEVIO_MLM_CFG_FILENAME, NULL};

    int spawn_err = _dmngr_spawn_chld (self, "malamute", argv_exec);

    /* Just fail miserably, for now */
    ASSERT_TEST(spawn_err >= 0, "Could not spawn broker",
            err_spawn_broker, DMNGR_ERR_SPAWNCHLD);

    self->broker_running = true;

err_spawn_broker:
err_broker_run:
    return err;
}

dmngr_err_e dmngr_scan_devs (dmngr_t *self, uint32_t *num_devs_found)
{
    return _dmngr_scan_devs (self, num_devs_found);
}

dmngr_err_e dmngr_spawn_all_devios (dmngr_t *self, char *broker_endp,
        char *devio_log_prefix, bool respawn_killed_devio)
{
    assert (self);
    assert (broker_endp);

    dmngr_err_e err = DMNGR_SUCCESS;
    char *cfg_file = self->cfg_file;
    char *dev_type_c = NULL;
    char *devio_type_c = NULL;
    char *dev_id_c = NULL;
    char *smio_inst_id_c = NULL;
    char *dev_pathname = NULL;

    /* DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_TRACE, "[dev_mngr_core] Spawing all DEVIO workers\n");*/

    /* Get all hash keys*/
    zlistx_t *devio_info_key_list = zhashx_keys (self->devio_info_h);
    ASSERT_ALLOC (devio_info_key_list, err_hash_keys_alloc, DMNGR_ERR_ALLOC);

    char *devio_info_key = zlistx_first (devio_info_key_list);

    /* Iterate over all keys spawning the DEVIOs */
    for (; devio_info_key != NULL; devio_info_key = zlistx_next (devio_info_key_list)) {
        /* Free any possibly dev_pathname allocated but not free'd */
        free (dev_pathname);
        dev_pathname = NULL;

        /* Look for DEVIO */
        devio_info_t *devio_info = zhashx_lookup (self->devio_info_h,
                devio_info_key);

        /* Get all devio_info properties */
        uint32_t id = devio_info_get_id (devio_info);
        llio_type_e type = devio_info_get_llio_type (devio_info);
        devio_type_e devio_type = devio_info_get_devio_type (devio_info);
        uint32_t smio_inst_id = devio_info_get_smio_inst_id (devio_info);
        dev_pathname = devio_info_clone_dev_pathname (devio_info);
        ASSERT_ALLOC (dev_pathname, err_dev_pathname_alloc, DMNGR_ERR_ALLOC);
        devio_state_e state = devio_info_get_state (devio_info);

        /* If device is already running or is stopped or inactive, don't do
         * anything */
        if (state != READY_TO_RUN &&
                state != KILLED) {
            /* DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_TRACE, "[dev_mngr_core] Device located in"
                    " %s is running, stopped or inactive. Skipping device...\n",
                    dev_pathname); */
            continue;
        }

        /* If device was killed but the respawn option of off, don't do
         * anything */
        if (state == KILLED && !respawn_killed_devio){
            /* DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_TRACE, "[dev_mngr_core] Device located in"
                    " %s is dead, but the option \n\trespawn_killed_devio is off. Skipping device...\n",
                    dev_pathname); */
            continue;
        }

        /* Get DEVIO type to set-up correct log filename */
        devio_type_c = devio_type_to_str (devio_type);
        ASSERT_ALLOC (devio_type_c, err_devio_type_c_alloc, DMNGR_ERR_ALLOC);

        /* Alloc and convert types */
        dev_type_c = llio_type_to_str (type);
        ASSERT_ALLOC (dev_type_c, err_dev_type_c_alloc, DMNGR_ERR_ALLOC);
        dev_id_c = hutils_stringify_dec_key (id);
        ASSERT_ALLOC (dev_id_c, err_dev_id_c_alloc, DMNGR_ERR_ALLOC);
        smio_inst_id_c = hutils_stringify_dec_key (smio_inst_id);
        ASSERT_ALLOC (smio_inst_id_c, err_smio_inst_id_c_alloc, DMNGR_ERR_ALLOC);

        /* Argument options are "process name", "device type" and
         *"dev entry" */
        DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_INFO, "[dev_mngr_core] Spawing DEVIO worker"
                " for a %s device \n\tlocated on %s, ID %u, broker address %s, with "
                "logfile on %s ...\n", dev_type_c, dev_pathname, id,
                broker_endp, devio_log_prefix);
        char *argv_exec [] = {DEVIO_NAME, "-f", cfg_file, "-n", devio_type_c,"-t", dev_type_c,
            "-i", dev_id_c, "-e", dev_pathname, "-s", smio_inst_id_c,
            "-b", broker_endp, "-l", devio_log_prefix, NULL};
        int spawn_err = _dmngr_spawn_chld (self, DEVIO_NAME, argv_exec);

        free (dev_type_c);
        dev_type_c = NULL;
        free (devio_type_c);
        devio_type_c = NULL;
        free (dev_id_c);
        dev_id_c = NULL;
        free (smio_inst_id_c);
        smio_inst_id_c = NULL;
        free (dev_pathname);
        dev_pathname = NULL;
        /* Just fail miserably, for now */
        ASSERT_TEST(spawn_err == 0, "Could not spawn DEVIO instance",
                err_spawn, DMNGR_ERR_SPAWNCHLD);

        state = RUNNING;
        devio_info_set_state (devio_info, state);
    }

err_spawn:
    free (smio_inst_id_c);
err_smio_inst_id_c_alloc:
    free (dev_id_c);
err_dev_id_c_alloc:
    free (dev_type_c);
err_dev_type_c_alloc:
    free (devio_type_c);
err_devio_type_c_alloc:
    free (dev_pathname);
err_dev_pathname_alloc:
    zlistx_destroy (&devio_info_key_list);
err_hash_keys_alloc:
    return err;
}

/************************ Local helper functions ******************/
/* Hash free function */
static void _devio_hash_free_item (void **data)
{
    devio_info_destroy ((devio_info_t **) data);
}

static dmngr_err_e _dmngr_scan_devs (dmngr_t *self, uint32_t *num_devs_found)
{
    assert (self);

    dmngr_err_e err = DMNGR_SUCCESS;
    glob_t glob_dev;

    /* Scan just the PCIe bus for now. We expect to find devices of
     * the form: /dev/fpga0 .. /dev/fpga5 */
    glob (DEVIO_BE_DEV_GLOB, 0, NULL, &glob_dev);

    /* Initialize the devices we found */
    uint32_t i = 0;
    for (; i < glob_dev.gl_pathc; ++i) {
        /* Check if the found device is already on the hash list */

        /* Extract ID */
        uint32_t devio_info_id;
        sscanf (glob_dev.gl_pathv[i], DEVIO_BE_DEV_PATTERN, &devio_info_id);

        /* Stringify ID */
        /* DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_TRACE,
                "[dev_mngr_core:scan_devs] Stringify hash ID\n"); */
        char key [HUTILS_CFG_HASH_KEY_MAX_LEN];

        /* This follows the hierarchy found in the config file */
        int errs = snprintf (key, sizeof (key), HUTILS_CFG_HASH_KEY_PATTERN_COMPL,
                devio_info_id, /* BPM ID does not matter for DBE DEVIOs */ 0);

        /* Only when the number of characters written is less than the whole buffer,
         * it is guaranteed that the string was written successfully */
        ASSERT_TEST (errs >= 0 && (size_t) errs < sizeof (key),
                "Could not generate DBE config path", err_cfg_key,
                DMNGR_ERR_CFG);

        const devio_info_t *devio_info_lookup = zhashx_lookup (self->devio_info_h,
                key);

        /* If device is already registered, do nothing */
        if (devio_info_lookup != NULL) {
            continue;
        }

        /* Found new device */
        DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_INFO,
                "[dev_mngr_core:scan_devs] Found new device on %s\n",
                glob_dev.gl_pathv[i]);

        /* Insert found device */
        err = _dmngr_prepare_devio (self, key, glob_dev.gl_pathv[i],
                devio_info_id, PCIE_DEV, DEVIO_BE_TYPE, 0, READY_TO_RUN);
        ASSERT_TEST (err == DMNGR_SUCCESS, "Could not insert DEVIO",
                err_devio_insert_alloc);
    }

    /* devio_info_destroy (&devio_info); */
err_devio_insert_alloc:
err_cfg_key:
    globfree (&glob_dev);

    /* Number of new devices found */
    if (num_devs_found != NULL) {
        *num_devs_found = i;
    }

    return err;
}

static dmngr_err_e _dmngr_prepare_devio (dmngr_t *self, const char *key,
        char *dev_pathname, uint32_t id, llio_type_e type,
        devio_type_e devio_type, uint32_t smio_inst_id, devio_state_e state)
{
    assert (self);
    assert (dev_pathname);

    dmngr_err_e err = DMNGR_SUCCESS;

    /* Alloc DBE hash item */
    devio_info_t *devio_info = devio_info_new (dev_pathname,
            id, type, devio_type, smio_inst_id, state);
    ASSERT_ALLOC (devio_info, err_devio_info_alloc, DMNGR_ERR_ALLOC);

    DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_INFO,
            "[dev_mngr_core:prepare_devio] Inserting device with key: %s\n", key);
    zhashx_insert (self->devio_info_h, key, devio_info);

err_devio_info_alloc:
    return err;
}

