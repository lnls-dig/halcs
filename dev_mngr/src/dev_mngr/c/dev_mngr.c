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
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...)  \
    ASSERT_HAL_TEST(test_boolean, DEV_MNGR, "[dev_mngr]",           \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)       \
    ASSERT_HAL_ALLOC(ptr, DEV_MNGR, "[dev_mngr]",                   \
            dmngr_err_str (DMNGR_ERR_ALLOC),                        \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                    \
    CHECK_HAL_ERR(err, DEV_MNGR, "[dev_mngr]",                      \
            dmngr_err_str (err_type))

#define DFLT_BIND_FOLDER            "/tmp/halcs"
#define DFLT_BIND_ADDR              "0"
#define IPC_FILE_PERM               0777

#define DFLT_LOG_DIR                "stdout"

static struct option long_options[] =
{
    {"help",                no_argument,         NULL, 'h'},
    {"cfgfile",             required_argument,   NULL, 'f'},
    {NULL, 0, NULL, 0}
};

static const char* shortopt = "hf:";

void print_help (char *program_name)
{
    fprintf (stdout, "HALCSD Device Manager\n"
            "Usage: %s [options]\n"
            "Version %s\n, Build by: %s, %s\n"
            "\n"
            "  -h  --help                           Display this usage information\n"
            "  -f  --cfgfile <Configuration File>   Specify configuration file\n"
            "\n"
            "\n\t Most of the options resides at the halcs configuration file,\n"
            "typically located in /etc/halcs", program_name,
            revision_get_build_version (),
            revision_get_build_user_name (), revision_get_build_date ());
}

int main (int argc, char *argv[])
{
    char *cfg_file = NULL;
    int opt;

    while ((opt = getopt_long (argc, argv, shortopt, long_options, NULL)) != -1) {
        /* Get the user selected options */
        switch (opt) {
            /* Display Help */
            case 'h':
                print_help (argv [0]);
                exit (1);
                break;

            case 'f':
                DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_TRACE, "[dev_mngr] Will set cfg_file parameter\n");
                cfg_file = strdup (optarg);
                break;

            case '?':
                DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_FATAL, "[dev_mngr] Option not recognized or missing argument\n");
                print_help (argv [0]);
                exit (1);
                break;

            default:
                DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_FATAL, "[dev_mngr] Could not parse options\n");
                print_help (argv [0]);
                exit (1);
        }
    }

    /* Check command-line parse options */
    if (cfg_file == NULL) {
        DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_FATAL, "[dev_mngr] Config file was "
                "not specified\n");
        print_help (argv [0]);
        goto err_parse_cfg;
    }

    zhashx_t *dmngr_hints = zhashx_new ();
    if (dmngr_hints == NULL) {
        DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_FATAL, "[dev_mngr] Could allocate "
                "hints hash table\n");
        goto err_dmngr_hints_alloc;
    }

    /**************************************************************************/
    /************ Read configuration variables from config file ***************/
    /**************************************************************************/

    /* Check for field not found */
    zconfig_t *root_cfg = zconfig_load (cfg_file);
    if (root_cfg == NULL) {
        DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_FATAL, "[dev_mngr] Could not load "
                "configuration file\n");
        goto err_cfg_exit;
    }

    /* Read the broker address */
    dmngr_broker_endp = zconfig_resolve (root_cfg, "/dev_mngr/broker/bind", NULL);
    if (dmngr_broker_endp == NULL) {
        DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_INFO,
                "[dev_mngr] Could not find broker endpoint in configuration "
                "file. Defaulting it to %s\n",
                DFLT_BIND_FOLDER"/"DFLT_BIND_ADDR);
        dmngr_broker_endp = DFLT_BIND_FOLDER"/"DFLT_BIND_ADDR;
    }

    DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_INFO,
            "[dev_mngr] Broker endpoint set to \"%s\"\n", dmngr_broker_endp);

    /* Read the log directory */
    dmngr_log_dir = zconfig_resolve (root_cfg, "/dev_mngr/log/dir", NULL);

    /* Set default logfile. We accept NULL as stdout */
    if (dmngr_log_dir == NULL) {
        DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_INFO,
                "[dev_mngr] Could not find Log directory in configuration "
                "file. Defaulting it to %s\n", DFLT_LOG_DIR);
        dmngr_log_dir = DFLT_LOG_DIR;
    }

    DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_INFO,
            "[dev_mngr] Log directory set to \"%s\"\n", dmngr_log_dir);

    /* Read the dev_mngr log filename */
    dmngr_log_filename = zconfig_resolve (root_cfg, "/dev_mngr/log/filename", NULL);

    /* Check for field not found */
    if (dmngr_log_filename == NULL) {
        DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_FATAL, "[dev_mngr] Could not find "
                "Log filename\n");
        goto err_cfg_exit;
    }

    DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_INFO,
            "[dev_mngr] Log filename set to \"%s\"\n", dmngr_log_filename);

    /* Read the verbose parameter */
    dmngr_verbose_str = zconfig_resolve (root_cfg, "/dev_mngr/verbose", NULL);

    /* Check for field not found */
    if (dmngr_verbose_str== NULL) {
        DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_FATAL, "[dev_mngr] Could not find "
                "verbose in configuration file\n");
        goto err_cfg_exit;
    }
    else {
        dmngr_verbose = strtol (dmngr_verbose_str, NULL, 10);
    }

    DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_INFO,
            "[dev_mngr] Verbose to \"%d\"\n", dmngr_verbose);

    /* Read the daemon-mode parameter */
    dmngr_daemonize_str = zconfig_resolve (root_cfg, "/dev_mngr/daemonize", NULL);

    /* Check for field not found */
    if (dmngr_daemonize_str== NULL) {
        DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_FATAL, "[dev_mngr] Could not find "
                "daemonize in configuration file\n");
        goto err_cfg_exit;
    }
    else {
        if (streq (dmngr_daemonize_str, "yes")) {
            dmngr_daemonize = 1;
        }
        else if (streq (dmngr_daemonize_str, "no")) {
            dmngr_daemonize = 0;
        }
        else {
            DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_FATAL, "[dev_mngr] Invalid option "
                    "for daemonize configuration variable\n");
            goto err_cfg_exit;
        }
    }

    DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_INFO,
            "[dev_mngr] Daemonize set to \"%d\"\n", dmngr_daemonize);

    /* Read the work directory for daemon only */
    if (dmngr_daemonize == 1) {
        dmngr_work_dir = zconfig_resolve (root_cfg, "/dev_mngr/workdir", NULL);

        /* Set default logfile. We accept NULL as stdout */
        if (dmngr_work_dir == NULL) {
            DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_FATAL,
                    "[dev_mngr] Could not find workdir in configuration "
                    "file\n");
            goto err_cfg_exit;
        }

        DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_INFO,
                "[dev_mngr] Work directory set to \"%s\"\n", dmngr_work_dir);
    }

    /* Read spawn broker parameter */
    dmngr_spawn_broker_cfg_str = zconfig_resolve (root_cfg, "/dev_mngr/spawn_broker", NULL);

    /* Check for field not found */
    if (dmngr_spawn_broker_cfg_str== NULL) {
        DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_FATAL, "[dev_mngr] Could not find "
                "spawn_broker in configuration file\n");
        goto err_cfg_exit;
    }
    else {
        if (streq (dmngr_spawn_broker_cfg_str, "yes")) {
            dmngr_spawn_broker_cfg = 1;
        }
        else if (streq (dmngr_spawn_broker_cfg_str, "no")) {
            dmngr_spawn_broker_cfg = 0;
        }
        else {
            DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_FATAL, "[dev_mngr] Invalid option "
                    "for spawn_broker configuration variable\n");
            goto err_cfg_exit;
        }
    }

    DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_INFO,
            "[dev_mngr] spawn_broker set to \"%d\"\n", dmngr_spawn_broker_cfg);

    /* Read DEVIO suggested bind endpoints and fill the hash table with
     * the corresponding keys */
    hutils_err_e herr = hutils_get_hints (root_cfg, dmngr_hints);
    if (herr != HUTILS_SUCCESS) {
        DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_FATAL, "[dev_mngr] Could not get hints "
                "from configuration file\n");
        goto err_cfg_exit;
    }

    /**************************************************************************/
    /********************** END of configuration variables ********************/
    /**************************************************************************/

    /* Daemonize dev_mngr */
    if (dmngr_daemonize != 0) {
        int rc = zsys_daemonize (dmngr_work_dir);

        if (rc != 0) {
            DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_FATAL, "[dev_mngr] Fail to daemonize\n");
            goto err_daemonize;
        }
    }

    /* See the fake promiscuous endpoint tcp*:*. To be changed soon! */
    DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_TRACE, "[dev_mngr] Creating DEV_MNGR instance ...\n");
    dmngr_t *dmngr = dmngr_new ("dev_mngr", "tcp://*:*", dmngr_verbose, dmngr_log_dir,
            dmngr_hints);
    if (dmngr == NULL) {
        DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_FATAL, "[dev_mngr] Fail to allocate dev_mngr instance\n");
        goto err_dmngr_alloc;
    }

    dmngr_err_e err = dmngr_set_cfg_file (dmngr, cfg_file);
    if (err != DMNGR_SUCCESS) {
        DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_FATAL, "[dev_mngr] Fail set configuration file\n");
        goto err_dmngr_set_cfg_file;
    }

#if 0
    dmngr_sig_handler_t dmngr_sigkill_handler =
    {   .signal = SIGKILL,
        .dmngr_sig_h = dmngr_sigkill_handler};
    dmngr_sig_handler_t dmngr_sigterm_handler =
    {   .signal = SIGTERM,
        .dmngr_sig_h = dmngr_sigterm_handler};

    dmngr_err_e err = dmngr_set_sig_handler (dmngr, &dmngr_sigkill_handler);
    if (err != DMNGR_SUCCESS) {
        DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_FATAL, "[dev_mngr] dmngr_set_sig_handler error!\n");
        goto err_exit;
    }

    dmngr_err_e err = dmngr_set_sig_handler (dmngr, &dmngr_sigterm_handler);
    if (err != DMNGR_SUCCESS) {
        DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_FATAL, "[dev_mngr] dmngr_set_sig_handler error!\n");
        goto err_exit;
    }
#endif

    dmngr_set_wait_clhd_handler (dmngr, &hutils_wait_chld);
    dmngr_set_spawn_clhd_handler (dmngr, &hutils_spawn_chld);

    err = dmngr_register_sig_handlers (dmngr);
    if (err != DMNGR_SUCCESS) {
        DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_FATAL, "[dev_mngr] dmngr_register_sig_handler error!\n");
        goto err_sig_handlers;
    }

    /* Here we should monitor the plug of new devices, such as
     * PCIe devices, by means of a udev event (or some other) and
     * Ethernet ones, by using a discovery protocol based on zeroMQ
     * (zbeacon should provide a sufficient infrastructure for that)
     */
    DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_TRACE, "[dev_mngr] DEV_MNGR PID: %d\n", getpid());
    DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_TRACE, "[dev_mngr] Monitoring devices ...\n");

    /* Do until a C^c is pressed (daemon mode unset) or SIGTERM signal arrives */
    while (!zsys_interrupted) {

        /* DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_TRACE, "[dev_mngr] ., PID: %d\n", getpid()); */

        /* Spawn the broker if not running */
        if (dmngr_spawn_broker_cfg == 1) {
            err = dmngr_spawn_broker (dmngr, dmngr_broker_endp);
            if (err != DMNGR_SUCCESS) {
                DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_FATAL, "[dev_mngr] Could not spwan broker!\n");
                goto err_spawn_broker;
            }
        }

        /* Search for new devices */
        err = dmngr_scan_devs (dmngr, NULL);
        if (err != DMNGR_SUCCESS) {
            DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_FATAL, "[dev_mngr] Could not scan devices!\n");
            goto err_scan_devs;
        }

        /* Spawn all found Device IOs that are ready to run */
        bool respawn_killed_devio = false;
        err = dmngr_spawn_all_devios (dmngr, dmngr_broker_endp,
                dmngr_log_dir, respawn_killed_devio);
        if (err != DMNGR_SUCCESS) {
            DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_FATAL, "[dev_mngr] Could not spawn DEVIOs!\n");
            goto err_spawn_devios;
        }

        /* Check the status of child processes */
        /* DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_TRACE,
         * "[dev_mngr] PID: %d will wait for child. # = %d\n", getpid(),  __dev_nums); */
        err = dmngr_wait_chld (dmngr);
        if (err != DMNGR_SUCCESS) {
            DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_FATAL, "[dev_mngr] Could not wait for children!\n");
            goto err_wait_chld;
        }

        /* Do some monitoring activities */
        sleep(1);
    }

    DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_FATAL, "[dev_mngr] Monitoring loop interrupted!\n");

err_wait_chld:
err_spawn_devios:
err_scan_devs:
err_spawn_broker:
err_sig_handlers:
    dmngr_destroy (&dmngr);
err_dmngr_set_cfg_file:
err_dmngr_alloc:
err_daemonize:
    zconfig_destroy (&root_cfg);
err_cfg_exit:
    zhashx_destroy (&dmngr_hints);
err_dmngr_hints_alloc:
    free (cfg_file);
err_parse_cfg:
    DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_INFO, "[dev_mngr] Exiting ...\n");

    return 0;
}
