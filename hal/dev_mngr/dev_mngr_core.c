/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include <string.h>
#include <glob.h>

#include "dev_mngr_core.h"
#include "hal_assert.h"
#include "debug_print.h"
#include "hal_utils.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...)  \
    ASSERT_HAL_TEST(test_boolean, DEV_MNGR, "dev_mngr_core",\
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_ALLOC(ptr, DEV_MNGR, "dev_mngr_core",        \
            dmngr_err_str(DMNGR_ERR_ALLOC),                 \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                            \
    CHECK_HAL_ERR(err, DEV_MNGR, "dev_mngr_core",           \
            dmngr_err_str (err_type))

#define LOG_FILENAME_LEN            50
#define DEVMNGR_LOG_FILENAME        "dev_mngr.log"
#define DEVMNGR_DFLT_LOG_MODE       "w"

#define DEVIO_DEV_NAME_LEN          40

#define DEVIO_BE_DEV_PATTERN        "/dev/fpga%d"
#define DEVIO_BE_DEV_GLOB           "/dev/fpga*"

/******************* Configuration file property names ************************/

#define DMNGR_FE_CFG_ENDP_MAX_PATH  256
#define DMNGR_FE_CFG_ENDP_PATH_PATTERN   \
                                    "/dev_io/board%u/bpm%u/afe/bind"

/* We don't expect our hash key to be bigger than this */
#define DMNGR_CFG_HASH_KEY_MAX_LEN  64
/* Our config hash key is composed of this pattern: board%u/bpm%u/afe
 * or board%u/bpm%u/dbe */
#define DMNGR_CFG_BOARD_TYPE        "%s"
#define DMNGR_CFG_BOARD_PATTERN     "board%u"
#define DMNGR_CFG_BPM_TYPE          "%s"
#define DMNGR_CFG_BPM_PATTERN       "bpm%u"
#define DMNGR_CFG_DEVIO_MODEL_TYPE  "%s"
#define DMNGR_CFG_AFE               "afe"
#define DMNGR_CFG_DBE               "dbe"

//#define DMNGR_CFG_HASH_KEY_PATTERN  "%s/%s/%s"
#define DMNGR_CFG_HASH_KEY_PATTERN  DMNGR_CFG_BOARD_TYPE \
                                    "/" DMNGR_CFG_BPM_TYPE "/" \
                                    DMNGR_CFG_DEVIO_MODEL_TYPE
#define DMNGR_CFG_HASH_KEY_PATTERN_COMPL \
                                    DMNGR_CFG_BOARD_PATTERN \
                                    "/" DMNGR_CFG_BPM_PATTERN "/" \
                                    DMNGR_CFG_DEVIO_MODEL_TYPE

/* Arbitrary hard limit for the maximum number of AFE DEVIOs
 * for each DBE DEVIO */
#define DEVIO_MAX_FE_DEVIOS         16

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

/* Configuration variables. To be filled by dev_mngr */
const char *dmngr_log_filename = NULL;
char *dmngr_log_dir = NULL;
char *dmngr_broker_endp = NULL;
char *dmngr_verbose_str = NULL;
int dmngr_verbose = 0;
char *dmngr_daemonize_str = NULL;
int dmngr_daemonize = 0;

static void _devio_hash_free_item (void *data);
static dmngr_err_e _dmngr_scan_devs (dmngr_t *self, uint32_t *num_devs_found);
static dmngr_err_e _dmngr_prepare_devio (dmngr_t *self, const char *key,
        char *dev_pathname, uint32_t id, llio_type_e type,
        devio_type_e devio_type, uint32_t smio_inst_id, devio_state_e state);

/* Creates a new instance of the Device Manager */
dmngr_t * dmngr_new (char *name, char *endpoint, int verbose,
        const char *log_prefix, zhash_t *hints_h)
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
    debug_set_log (log_filename, DEVMNGR_DFLT_LOG_MODE);

    DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_INFO, "[dev_mngr_core] Spawing DEV_MNGR worker"
            " with \n\tdiscovery endpoint (ignored for now) %s, with logfile %s"
            " ...\n", endpoint, (log_filename == NULL) ? "NULL" : log_filename);

    dmngr_t *self = (dmngr_t *) zmalloc (sizeof *self);
    ASSERT_ALLOC(self, err_self_alloc);

    /* Initialize the Device Manager */
    self->ctx = zctx_new ();
    ASSERT_ALLOC(self->ctx, err_ctx_alloc);

    /* Create Dealer for use with zbeacon */
    self->dealer = zsocket_new (self->ctx, ZMQ_DEALER);
    ASSERT_ALLOC(self->dealer, err_dealer_alloc);
    zsocket_bind (self->dealer, "%s", endpoint);

    self->name = strdup (name);
    ASSERT_ALLOC(self->name, err_name_alloc);
    self->endpoint = strdup (endpoint);
    ASSERT_ALLOC(self->endpoint, err_endpoint_alloc);
    self->verbose = verbose;

    self->ops = (dmngr_ops_t *) zmalloc (sizeof *self->ops);
    ASSERT_ALLOC(self->ops, err_ops_alloc);
    self->ops->sig_ops = zlist_new ();
    ASSERT_ALLOC(self->ops->sig_ops, err_list_alloc);

    /* Init devio_info hash */
    self->devio_info_h = zhash_new ();
    ASSERT_ALLOC(self->devio_info_h, err_devio_info_h_alloc);

    self->hints_h = zhash_dup (hints_h);
    ASSERT_ALLOC(self->hints_h, err_hints_h_alloc);

    self->broker_running = false;

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
err_hints_h_alloc:
        zhash_destroy (&self->devio_info_h);
err_devio_info_h_alloc:
    zlist_destroy (&self->ops->sig_ops);
err_list_alloc:
    free (self->ops);
err_ops_alloc:
    free (self->endpoint);
err_endpoint_alloc:
    free (self->name);
err_name_alloc:
    zsocket_destroy (self->ctx, self->dealer);
err_dealer_alloc:
    zctx_destroy (&self->ctx);
err_ctx_alloc:
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
        zhash_destroy (&self->hints_h);
        zhash_destroy (&self->devio_info_h);
        zlist_destroy (&self->ops->sig_ops);
        free (self->ops);
        free (self->endpoint);
        free (self->name);
        zsocket_destroy (self->ctx, self->dealer);
        zctx_destroy (&self->ctx);

        free (self);
        *self_p = NULL;
    }

    return DMNGR_SUCCESS;
}

dmngr_err_e dmngr_set_sig_handler (dmngr_t *self, dmngr_sig_handler_t *sig_handler)
{
    assert (self);
    int err = zlist_append (self->ops->sig_ops, sig_handler);

    return (err == -1) ? DMNGR_ERR_ALLOC : DMNGR_SUCCESS;
}

dmngr_err_e dmngr_register_sig_handlers (dmngr_t *self)
{
    assert (self);
    dmngr_sig_handler_t *sig_handler =
        (dmngr_sig_handler_t *) zlist_first (self->ops->sig_ops);

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
            zlist_next (self->ops->sig_ops);
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
    char *argv_exec[] = {"mdp_broker", broker_endp, NULL};
    /* char *argv_exec[] = {"mdp_broker", "-v", NULL}; */
    int spawn_err = _dmngr_spawn_chld (self, "mdp_broker", argv_exec);

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
    dmngr_err_e err = DMNGR_SUCCESS;
    char *dev_type_c = NULL;
    char *devio_type_c = NULL;
    char *dev_id_c = NULL;
    char *smio_inst_id_c = NULL;

    /* DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_TRACE, "[dev_mngr_core] Spawing all DEVIO workers\n");*/

    /* Get all hash keys*/
    zlist_t *devio_info_key_list = zhash_keys (self->devio_info_h);
    ASSERT_ALLOC (devio_info_key_list, err_hash_keys_alloc, DMNGR_ERR_ALLOC);

    char *devio_info_key = zlist_first (devio_info_key_list);

    /* Iterate over all keys spawning the DEVIOs */
    for (; devio_info_key != NULL; devio_info_key = zlist_next (devio_info_key_list)) {
        /* FIXME: Usage of stroul function for reconverting the string
         * into a uint32_t */
        devio_info_t *devio_info = zhash_lookup (self->devio_info_h,
                devio_info_key);

        /* If device is already running or is stopped or inactive, don't do
         * anything */
        if (devio_info->state != READY_TO_RUN &&
                devio_info->state != KILLED) {
            /* DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_TRACE, "[dev_mngr_core] Device located in"
                    " %s is running, stopped or inactive. Skipping device...\n",
                    devio_info->dev_pathname); */
            continue;
        }

        /* If device was killed but the respawn option of off, don't do
         * anything */
        if (devio_info->state == KILLED && !respawn_killed_devio){
            /* DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_TRACE, "[dev_mngr_core] Device located in"
                    " %s is dead, but the option \n\trespawn_killed_devio is off. Skipping device...\n",
                    devio_info->dev_pathname); */
            continue;
        }

        /* Get DEVIO type to set-up correct log filename */
        devio_type_c = devio_type_to_str (devio_info->devio_type);
        ASSERT_ALLOC (devio_type_c, err_devio_type_c_alloc, DMNGR_ERR_ALLOC);

        /* Set up logdir. Defaulting it to stdout */
        char devio_log_filename[LOG_FILENAME_LEN] = "stdout";

        /* TODO: Check for the validity of the log filename */
        if (devio_log_prefix != NULL) {
            snprintf (devio_log_filename, LOG_FILENAME_LEN,
                    "%s/"DEVIO_LOG_FILENAME_PATTERN, devio_log_prefix,
                    devio_info->id, devio_type_c, devio_info->smio_inst_id);
        }

        /* Alloc and convert types */
        dev_type_c = llio_type_to_str (devio_info->type);
        ASSERT_ALLOC (dev_type_c, err_dev_type_c_alloc, DMNGR_ERR_ALLOC);
        dev_id_c = halutils_stringify_dec_key (devio_info->id);
        ASSERT_ALLOC (dev_id_c, err_dev_id_c_alloc, DMNGR_ERR_ALLOC);
        smio_inst_id_c = halutils_stringify_dec_key (devio_info->smio_inst_id);
        ASSERT_ALLOC (smio_inst_id_c, err_smio_inst_id_c_alloc, DMNGR_ERR_ALLOC);

        /* Argument options are "process name", "device type" and
         *"dev entry" */
        DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_INFO, "[dev_mngr_core] Spawing DEVIO worker"
                " for a %s device \n\tlocated on %s, ID %u, broker address %s, with "
                "logfile on %s ...\n", dev_type_c, devio_info->dev_pathname, devio_info->id,
                broker_endp, devio_log_filename);
        char *argv_exec [] = {DEVIO_NAME, "-n", devio_type_c,"-t", dev_type_c,
            "-i", dev_id_c, "-e", devio_info->dev_pathname, "-s", smio_inst_id_c,
            "-b", broker_endp, "-l", devio_log_filename, NULL};
        int spawn_err = _dmngr_spawn_chld (self, DEVIO_NAME, argv_exec);

        free (dev_type_c);
        dev_type_c = NULL;
        free (devio_type_c);
        devio_type_c = NULL;
        free (dev_id_c);
        dev_id_c = NULL;
        free (smio_inst_id_c);
        smio_inst_id_c = NULL;
        /* Just fail miserably, for now */
        ASSERT_TEST(spawn_err == 0, "Could not spawn DEVIO instance",
                err_spawn, DMNGR_ERR_SPAWNCHLD);

        devio_info->state = RUNNING;
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
    zlist_destroy (&devio_info_key_list);
err_hash_keys_alloc:
    return err;
}

/************************ Local helper functions ******************/
/* Hash free function */
static void _devio_hash_free_item (void *data)
{
    devio_info_destroy ((devio_info_t **) &data);
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
        char key [DMNGR_CFG_HASH_KEY_MAX_LEN];

        /* This follows the hierarchy found in the config file */
        int errs = snprintf (key, sizeof (key), DMNGR_CFG_HASH_KEY_PATTERN_COMPL,
                devio_info_id, /* BPM ID does not matter for DBE DEVIOs */ 0,
                DMNGR_CFG_DBE);

        /* Only when the number of characters written is less than the whole buffer,
         * it is guaranteed that the string was written successfully */
        ASSERT_TEST (errs >= 0 && (size_t) errs < sizeof (key),
                "[dev_mngr] Could not generate DBE config path\n", err_cfg_key,
                DMNGR_ERR_CFG);

        const devio_info_t *devio_info_lookup = zhash_lookup (self->devio_info_h,
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

        /* For each PCIE device find, register Ethernet devices to control
         * our RFFEs. Do a lookup in our hints_h hash to look for endpoints
         * to bind to */
        uint32_t j;
        for (j = 0; j < DEVIO_MAX_FE_DEVIOS; ++j) {
            char hints_key [DMNGR_CFG_HASH_KEY_MAX_LEN];
            int errs = snprintf (hints_key, sizeof (hints_key),
                    DMNGR_CFG_HASH_KEY_PATTERN_COMPL, devio_info_id, j,
                    DMNGR_CFG_AFE);

            /* Only when the number of characters written is less than the whole buffer,
             * it is guaranteed that the string was written successfully */
            ASSERT_TEST (errs >= 0 && (size_t) errs < sizeof (hints_key),
                    "[dev_mngr] Could not generate AFE bind address from "
                    "configuration file\n", err_cfg_exit, DMNGR_ERR_CFG);

            char *endpoint_fe = zhash_lookup (self->hints_h, hints_key);
            /* If key is not found, assume we don't have any more AFE to
             * prepare */
            if (endpoint_fe == NULL) {
                /* DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_INFO,
                        "[dev_mngr_core:scan_devs] Could not find any more endpoint
                        hints for FEs\n"); */
                continue;
            }

            /* Prepare respective DEVIO structure */
            err = _dmngr_prepare_devio (self, hints_key, endpoint_fe,
                    devio_info_id, ETH_DEV, DEVIO_FE_TYPE, j,
                    READY_TO_RUN);
            ASSERT_TEST (err == DMNGR_SUCCESS, "Could not prepare associated FE DEVIO",
                    err_devio_fe_insert_alloc);
        }
    }

    /* devio_info_destroy (&devio_info); */
err_devio_fe_insert_alloc:
err_cfg_exit:
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
    zhash_insert (self->devio_info_h, key, devio_info);

    /* Setup free function */
    zhash_freefn (self->devio_info_h, key, _devio_hash_free_item);

err_devio_info_alloc:
    return err;
}

dmngr_err_e dmngr_get_hints (zconfig_t *root_cfg, zhash_t *hints_h)
{
    assert (root_cfg);
    assert (hints_h);

    dmngr_err_e err = DMNGR_SUCCESS;

    /* Read DEVIO suggested bind endpoints and fill the hash table with
     * the corresponding keys */

    /* First find the dev_io property */
    zconfig_t *devio_cfg = zconfig_locate (root_cfg, "/dev_io");
    ASSERT_TEST (devio_cfg != NULL, "[dev_mngr] Could not find "
            "dev_io property in configuration file\n", err_cfg_exit,
            DMNGR_ERR_CFG);

    /* Now, find all of our child */
    zconfig_t *board_cfg = zconfig_child (devio_cfg);
    ASSERT_TEST (board_cfg != NULL, "[dev_mngr] Could not find "
            "board* property in configuration file\n", err_cfg_exit,
            DMNGR_ERR_CFG);

    /* Navigate through all of our board siblings */
    for (; board_cfg != NULL; board_cfg = zconfig_next (board_cfg)) {
        DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_FATAL, "[dev_mngr] Config file: "
                "board_cfg name: %s\n", zconfig_name (board_cfg));

        zconfig_t *bpm_cfg = zconfig_child (board_cfg);
        ASSERT_TEST (bpm_cfg != NULL, "[dev_mngr] Could not find "
                "bpm* property in configuration file\n", err_cfg_exit,
                DMNGR_ERR_CFG);

        /* Navigate through all of our bpm siblings and fill the hash table */
        for (; bpm_cfg != NULL; bpm_cfg = zconfig_next (bpm_cfg)) {
            DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_TRACE, "[dev_mngr] Config file: "
                    "bpm_cfg name: %s\n", zconfig_name (bpm_cfg));

            /* Now, we expect to find the bind address of this bpm/board instance
             * in the configuration file */
            char *hints_value = zconfig_resolve (bpm_cfg, "/afe/bind",
                    NULL);
            ASSERT_TEST (hints_value != NULL, "[dev_mngr] Could not find "
                    "AFE bind address in configuration file\n", err_cfg_exit,
                    DMNGR_ERR_CFG);

            /* Now, we only need to generate a valid key to insert in the hash.
             * we choose the combination of the type "board%u/bpm%u/afe" or
             * board%u/bpm%u/dbe */
            char hints_key [DMNGR_CFG_HASH_KEY_MAX_LEN];
            int errs = snprintf (hints_key, sizeof (hints_key),
                    DMNGR_CFG_HASH_KEY_PATTERN, zconfig_name (board_cfg),
                    zconfig_name (bpm_cfg), DMNGR_CFG_AFE);

            /* Only when the number of characters written is less than the whole buffer,
             * it is guaranteed that the string was written successfully */
            ASSERT_TEST (errs >= 0 && (size_t) errs < sizeof (hints_key),
                    "[dev_mngr] Could not generate AFE bind address from "
                    "configuration file\n", err_cfg_exit, DMNGR_ERR_CFG);

            DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_INFO, "[dev_mngr] AFE hint endpoint "
                    "hash key: \"%s\", value: \"%s\"\n", hints_key, hints_value);

            /* Insert this value in the hash table */
            errs = zhash_insert (hints_h, hints_key, hints_value);
            ASSERT_TEST (errs == 0, "[dev_mngr] Could not find "
                    "insert AFE endpoint to hash table\n", err_cfg_exit,
                    DMNGR_ERR_CFG);
        }
    }

err_cfg_exit:
    return err;
}

