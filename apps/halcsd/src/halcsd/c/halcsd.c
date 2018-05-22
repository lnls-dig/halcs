/*
 * Copyright (C) 2015 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include <libgen.h>
#include "halcs_server.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...)  \
    ASSERT_HAL_TEST(test_boolean, DEV_IO, "[halcsd]",               \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)       \
    ASSERT_HAL_ALLOC(ptr, DEV_IO, "[halcsd]",                       \
            devio_err_str (DEVIO_ERR_ALLOC),                        \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                    \
    CHECK_HAL_ERR(err, DEV_IO, "[halcsd]",                          \
            devio_err_str (err_type))

#define LOG_FILENAME_LEN            50
/* This composes the log filename as "halcsd%u_fe%u.log" or
 * "halcsd%u_be%u.log" */
#define DEVIO_LOG_PREFIX_PATTERN    "%s/"
#define DEVIO_LOG_RADICAL_PATTERN   "halcsd%u"
#define DEVIO_LOG_DEVIO_MODEL_TYPE  "%s"
#define DEVIO_LOG_INST_TYPE         "%u"
#define DEVIO_LOG_SUFFIX            "log"
#define DEVIO_LOG_FILENAME_PATTERN  \
                                    DEVIO_LOG_PREFIX_PATTERN \
                                    DEVIO_LOG_RADICAL_PATTERN "_" \
                                    DEVIO_LOG_DEVIO_MODEL_TYPE \
                                    DEVIO_LOG_INST_TYPE "." \
                                    DEVIO_LOG_SUFFIX

/* This composes the INFO filename as "halcsd%u_fe%u_info.log" or
 * "halcsd%u_be%u_info.log" */
#define DEVIO_LOG_INFO_TYPE         "info"
#define DEVIO_LOG_INFO_FILENAME_PATTERN  \
                                    DEVIO_LOG_PREFIX_PATTERN \
                                    DEVIO_LOG_RADICAL_PATTERN "_" \
                                    DEVIO_LOG_DEVIO_MODEL_TYPE \
                                    DEVIO_LOG_INST_TYPE "_" \
                                    DEVIO_LOG_INFO_TYPE "." \
                                    DEVIO_LOG_SUFFIX

/* Arbitrary hard limit for the maximum number of AFE DEVIOs
 * for each DBE DEVIO */
#define DEVIO_MAX_FE_DEVIOS             16

#define DEVIO_SERVICE_LEN               50
#define DEVIO_ENTRY_LEN                 50
#define DEVIO_NAME                      "/usr/local/bin/halcsd"
#define DEVIO_CFG_NAME                  "/usr/local/bin/halcsd_cfg"
#define DEVIO_CFG_TIMEOUT               5000       /* in ms */
#define EPICS_PROCSERV_NAME             "/usr/local/bin/procServ"
#define EPICS_HALCS_RUN_SCRIPT_NAME       "./run.sh"

#define DEVIO_LIBHALCSCLIENT_LOG_MODE   "a"
#define DEVIO_KILL_CFG_SIGNAL           SIGINT
#define FRONTEND_ENDPOINT_PREFIX        "ipc:///tmp/halcs-pipe"

static devio_err_e _rffe_get_dev_entry (uint32_t dev_id, uint32_t fe_smio_id,
        zhashx_t *hints, char **dev_entry);
static char *_create_log_filename (char *log_prefix, char *log_filename_pattern, 
        uint32_t dev_id, const char *devio_type, uint32_t smio_inst_id);
static devio_err_e _spawn_platform_smios (void *pipe, devio_type_e devio_type,
        uint32_t smio_inst_id, zhashx_t *hints, uint32_t dev_id);
static devio_err_e _spawn_fe_platform_smios (void *pipe, uint32_t smio_inst_id);

static struct option long_options[] =
{
    {"help",                no_argument,         NULL, 'h'},
    {"brokerendp",          required_argument,   NULL, 'b'},
    {"cfgfile",             required_argument,   NULL, 'f'},
    {"daemon",              no_argument,         NULL, 'd'},
    {"daemonworkdir",       required_argument,   NULL, 'w'},
    {"verbose",             no_argument,         NULL, 'v'},
    {"deviotype",           required_argument,   NULL, 'n'},
    {"devicetype",          required_argument,   NULL, 't'},
    {"deviceentry",         required_argument,   NULL, 'e'},
    {"deviceid",            required_argument,   NULL, 'i'},
    {"logprefix",           required_argument,   NULL, 'l'},
    {NULL, 0, NULL, 0}
};

static const char* shortopt = "hb:f:dw:vn:t:e:i:l:";

void print_help (char *program_name)
{
    fprintf (stdout, "HALCSD Device I/O\n"
            "Usage: %s [options]\n"
            "Version %s\n, Build by: %s, %s\n"
            "\n"
            "  -h  --help                           Display this usage information\n"
            "  -b  --brokerendp <Broker endpoint>   Broker endpoint\n"
            "  -f  --cfgfile <Configuration File>   Specify configuration file\n"
            "  -d  --daemon                         Run as system daemon.\n"
            "  -w  --daemonworkdir <Work Directory> Daemon working directory.\n"
            "  -v  --verbose                        Verbose output\n"
            "  -n  --deviotype <[be|fe]>            Devio type\n"
            "  -t  --devicetype <[eth|pcie]>        Device type\n"
            "  -e  --deviceentry <[ip_addr|/dev entry]>\n"
            "                                       Device entry\n"
            "  -i  --deviceid <Device ID>           Device ID\n"
            "  -l  --logprefix <Log prefix>         Log prefix filename\n",
            program_name,
            revision_get_build_version (),
            revision_get_build_user_name (), revision_get_build_date ());
}

int main (int argc, char *argv[])
{
    int verbose = 0;
    int devio_daemonize = 0;
    char *devio_work_dir = NULL;
    char *devio_type_str = NULL;
    char *dev_type = NULL;
    char *dev_entry = NULL;
    char *dev_id_str = NULL;
    char *broker_endp = NULL;
    char *log_prefix = NULL;
    char *cfg_file = NULL;
    int opt;

    /* Block signals in all of the threads, if not otherwise stated */
    static sigset_t signal_mask;
    sigemptyset (&signal_mask);
    sigaddset (&signal_mask, SIGUSR1);
    sigaddset (&signal_mask, SIGUSR2);
    sigaddset (&signal_mask, SIGHUP);
    int rc = pthread_sigmask (SIG_BLOCK, &signal_mask, NULL);
    if (rc != 0) {
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_FATAL, "[halcsd] Could not block sigmask: %d\n", rc); 
        goto err_sigmask;
    }

    while ((opt = getopt_long (argc, argv, shortopt, long_options, NULL)) != -1) {
        /* Get the user selected options */
        switch (opt) {
            /* Display Help */
            case 'h':
                print_help (argv [0]);
                exit (1);
                break;

            case 'b':
                DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE, "[halcsd] Will set broker_endp parameter\n");
                broker_endp = strdup (optarg);
                break;

            case 'f':
                DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_TRACE, "[halcsd] Will set cfg_file parameter\n");
                cfg_file = strdup (optarg);
                break;

            case 'd':
                DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_TRACE, "[halcsd] Will set daemon parameter\n");
                devio_daemonize = 1;
                break;

            case 'w':
                DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE, "[halcsd] Will set devio_work_dir parameter\n");
                devio_work_dir = strdup (optarg);
                break;

            case 'v':
                DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_TRACE, "[halcsd] Will set verbose parameter\n");
                verbose = 1;
                break;

            case 'n':
                DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE, "[halcsd] Will set devio_type parameter\n");
                devio_type_str = strdup (optarg);
                break;

            case 't':
                DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE, "[halcsd] Will set dev_type parameter\n");
                dev_type = strdup (optarg);
                break;

            case 'e':
                DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE, "[halcsd] Will set dev_entry parameter\n");
                dev_entry = strdup (optarg);
                break;

            case 'i':
                DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE, "[halcsd] Will set dev_id_str parameter\n");
                dev_id_str = strdup (optarg);
                break;

            case 'l':
                DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE, "[halcsd] Will set log filename\n");
                log_prefix = strdup (optarg);
                break;

            case '?':
                DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_FATAL, "[halcsd] Option not recognized or missing argument\n");
                print_help (argv [0]);
                exit (1);
                break;

            default:
                DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_FATAL, "[halcsd] Could not parse options\n");
                print_help (argv [0]);
                exit (1);
        }
    }

    /**************************************************************************/
    /************ Read configuration variables from config file ***************/
    /**************************************************************************/

    /* Check for field not found */
    if (cfg_file == NULL) {
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_FATAL, "[halcsd] Configuration file not set\n");
        goto err_cfg_not_found;
    }

    zhashx_t *devio_hints = zhashx_new ();
    if (devio_hints == NULL) {
        DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_FATAL, "[halcsd] Could allocate "
                "hints hash table\n");
        goto err_devio_hints_alloc;
    }

    zconfig_t *root_cfg = zconfig_load (cfg_file);
    if (root_cfg == NULL) {
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_FATAL, "[halcsd] Could not load "
                "configuration file\n");
        goto err_cfg_load;
    }

    /* Read DEVIO suggested bind endpoints and fill the hash table with
     * the corresponding keys */
    hutils_err_e herr = hutils_get_hints (root_cfg, devio_hints);
    if (herr != HUTILS_SUCCESS) {
        DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_FATAL, "[halcsd] Could not get hints "
                "from configuration file\n");
        goto err_cfg_get_hints;
    }

    /**************************************************************************/
    /*********************** Command-line parse options ***********************/
    /**************************************************************************/

    /* Daemonize dev_io */
    if (devio_daemonize != 0) {
        if (devio_work_dir == NULL) {
            DBE_DEBUG (DBG_DEV_IO | DBG_LVL_FATAL, "[halcsd] Daemon working directory not specified\n");
            goto err_exit;
        }

        int rc = zsys_daemonize (devio_work_dir);
        if (rc != 0) {
            DBE_DEBUG (DBG_DEV_IO | DBG_LVL_FATAL, "[halcsd] Fail to daemonize\n");
            goto err_exit;
        }
    }

    llio_type_e llio_type = llio_str_to_type (dev_type);
    /* Parse command-line options */
    if (llio_type == INVALID_DEV) {
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_FATAL, "[halcsd] Dev_type parameter is invalid\n");
        goto err_exit;
    }

    devio_type_e devio_type = devio_str_to_type (devio_type_str);
    /* Parse command-line options */
    if (devio_type == INVALID_DEVIO) {
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_FATAL, "[halcsd] Devio_type parameter is invalid\n");
        goto err_exit;
    }

    /* At least one ID must be set */
    if (dev_entry == NULL && dev_id_str == NULL) {
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO, "[halcsd] Dev_entry and Dev_id parameters "
                "were not set. Exiting ...\n");
        goto err_exit;
    }

    uint32_t full_dev_id = 0;
    uint32_t dev_id = 0;
    uint32_t fe_smio_id = 0;
    /* Check for device ID */
    if (dev_id_str != NULL) {
        /* Use the passed ID, interpret it as decimal number */
        full_dev_id = strtoul (dev_id_str, NULL, 10);
        /* Check if we are withing range */
        ASSERT_TEST (full_dev_id > 0 && full_dev_id < NUM_MAX_HALCSS+1,
                "Device ID is out of range", err_exit);

        /* Extract device and smio IDs */
        dev_id = board_epics_map [full_dev_id].dev_id;
        fe_smio_id = board_epics_map [full_dev_id].smio_id;
    }
    else {
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO, "[halcsd] Dev_id parameter "
                "is not set. Trying to get the Dev_id parameter from Dev_entry\n");
    }

    /* Check for device entry */
    if (dev_entry != NULL && dev_id_str != NULL) {
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO, "[halcsd] Dev_id and Dev_entry parameters were set\n"
                "\tUsing the passed parameters\n");
    }

    devio_err_e err = DEVIO_SUCCESS;
    const llio_ops_t *llio_ops = NULL;
    /* Check Dev_type */
    switch (llio_type) {
        case ETH_DEV:
            /* Dev_id is not set but Dev_entry is. In this case, we can't
             * extract the Dev_id, just yet
             */
            if (dev_entry != NULL && dev_id_str == NULL) {
                DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO, "[halcsd] Dev_id parameter was not set, but Dev_entry was.\n"
                        "\tThis is not supported for Devio_type = ETH_DEV. Exiting ...\n");
                goto err_exit;
            }

            if (dev_entry == NULL && dev_id_str != NULL) {
                DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO, "[halcsd] Dev_id parameter was set, but Dev_entry was not.\n"
                        "\tTrying to get Dev_entry from config file...\n");
                err = _rffe_get_dev_entry (dev_id, fe_smio_id, devio_hints, &dev_entry);
                ASSERT_TEST (err == DEVIO_SUCCESS, "Could not get dev_entry from config file",
                        err_exit);
            }

            /* Get LLIO operations */
            llio_ops = &llio_ops_eth;
            break;

        case PCIE_DEV:
            if (dev_entry != NULL && dev_id_str == NULL) {
                DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO, "[halcsd] Dev_id parameter was not set, but Dev_entry was.\n"
                        "\tExtracting Dev_id from /dev file number\n");

                /* Our device follows the convention of having the ID in decimal
                 * code. For instance, /dev/fpga-10 would be a valid ID */
                int matches = sscanf (dev_entry, "/dev/fpga-%u", &full_dev_id);
                if (matches == 0) {
                    DBE_DEBUG (DBG_DEV_IO | DBG_LVL_FATAL, "[halcsd] Dev_entry parameter is invalid.\n"
                            "\tIt must be in the format \"/dev/fpga-<device_number>\". Exiting ...\n");
                    goto err_exit;
                }

                /* Check if we are withing range */
                ASSERT_TEST (full_dev_id > 0 && full_dev_id < NUM_MAX_HALCSS+1,
                        "Device ID is out of range", err_exit);
                /* Extract device and smio IDs */
                dev_id = board_epics_map [full_dev_id].dev_id;
                fe_smio_id = board_epics_map [full_dev_id].smio_id;
            }

            if (dev_entry == NULL && dev_id_str != NULL) {
                char dev_entry_tmp [DEVIO_ENTRY_LEN];
                DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO, "[halcsd] Dev_id parameter was set, but Dev_entry was not.\n"
                        "\tDefaulting Dev_entry to /dev/fpga-<dev_id> \n");
                int errs = snprintf (dev_entry_tmp, DEVIO_SERVICE_LEN-1, "/dev/fpga-%u", dev_id);
                /* Only when the number of characters written is less than the whole buffer,
                 * it is guaranteed that the string was written successfully */
                ASSERT_TEST (errs >= 0 && (size_t) errs < sizeof (dev_entry_tmp),
                        "Could not generate default Dev_entry", err_exit, DEVIO_ERR_CFG);
                dev_entry = strdup (dev_entry_tmp);
            }

            /* As we are spawning a DEVIO for each passed device ID, we must take care
             * of invalid dev_id for PCIE_DEV. For instance, even device IDs,
             * like 0, 2, 4, ... are invalid and we only accept odd device IDs
             * for PCIE_DEV. In this case we exit with success as this is expected
             * on a larger system relying on systemd spawning, for instance */
            ASSERT_TEST (fe_smio_id == 0, "Invalid Dev_id for PCIE_DEV. Only "
                    "odd device IDs are available", err_exit, 0);

            llio_ops = &llio_ops_pcie;
            break;

        default:
            DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO, "[halcsd] Invalid Dev_type. Exiting ...\n");
            llio_ops = NULL;
            goto err_exit;
    }

    DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO, "[halcsd] Dev_entry parameter was set to %s.\n",
            dev_entry);

    /* Spawn the Configure DEVIO to get the uTCA slot number. This is only
     * available in AFCv3 */
#if defined (__BOARD_AFCV3__) && (__WITH_APP_CFG__)
    int child_devio_cfg_pid = 0;
    if (llio_type == PCIE_DEV) {
        /* Argument options are "process name", "device type" and
         *"dev entry" */
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO, "[halcsd] Spawing DEVIO Config\n");
        char *argv_exec [] = {DEVIO_CFG_NAME, "-n", devio_type_str,"-t", dev_type,
            "-i", dev_id_str, "-e", dev_entry, "-b", broker_endp, NULL};
        /* Spawn Config DEVIO. */
        /* We can't use devio_spawn_chld as DEVIO does not exist
         * just yet. So, we stick with "hutils" implementation */
        child_devio_cfg_pid = hutils_spawn_chld (DEVIO_CFG_NAME, argv_exec);

        if (child_devio_cfg_pid < 0) {
            DBE_DEBUG (DBG_DEV_IO | DBG_LVL_FATAL, "[halcsd] Could not create "
                    "DEVIO Config instance\n");
            goto err_exit;
        }

        /* FIXME: Wait for DEVIO CFG initialization. If we try to create a client instance
         * prior to DEVIO CFG creation, Malamute does not transmit the message (To be investigated) */
        sleep (2);
    }

    /* FE DEVIO is expected to have a correct dev_id. So, we don't need to get it
     * from Hardware */
    halcs_client_t *client_cfg = NULL;
    if (devio_type == BE_DEVIO) {
        /* At this point, the Config DEVIO is ready to receive our commands */
        char devio_config_service_str [DEVIO_SERVICE_LEN];
        snprintf (devio_config_service_str, DEVIO_SERVICE_LEN-1, "HALCS%u:DEVIO_CFG:AFC_DIAG%u",
                dev_id, 0);
        devio_config_service_str [DEVIO_SERVICE_LEN-1] = '\0'; /* Just in case ... */

        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO, "[halcsd] Creating libclient for DEVIO config\n");
        halcs_client_err_e client_err = HALCS_CLIENT_SUCCESS;

        /* Give DEVIO CFG some time more to answer (DEVIO_CFG_TIMEOUT) as it was just spawned... */
        client_cfg = halcs_client_new_log_mode_time (broker_endp, 0,
                "stdout", DEVIO_LIBHALCSCLIENT_LOG_MODE, DEVIO_CFG_TIMEOUT);

        if (client_cfg == NULL) {
            DBE_DEBUG (DBG_DEV_IO | DBG_LVL_FATAL, "[halcsd] Could not create "
                    "DEVIO Config libclient instance\n");
            goto err_client_cfg;
        }

        /* Get uTCA card slot number */
        client_err = halcs_get_afc_diag_card_slot (client_cfg, devio_config_service_str,
                &dev_id);

        if (client_err != HALCS_CLIENT_SUCCESS) {
            DBE_DEBUG (DBG_DEV_IO | DBG_LVL_FATAL, "[halcsd] Could not retrieve "
                    "slot number. Unsupported board?\n");
            goto err_card_slot;
        }

/* We could just leave DEVIO CFG arounf and not kill it. We do it just
 * for the sake not having unnecessary things running, as the regular
 * DEVIO already spwan the same service (i.e., AFC DIAG) as DEVIO CFG */
#if 1
        if (llio_type == PCIE_DEV) {
            /* Kill DEVIO cfg as we've already got our slot number */
            if (child_devio_cfg_pid > 0) {
                kill (child_devio_cfg_pid, DEVIO_KILL_CFG_SIGNAL);
            }
        }
        /* Destroy libclient */
        halcs_client_destroy (&client_cfg);
    }
#endif
#endif

    DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO, "[halcsd] Dev_id parameter was set to %u.\n",
            dev_id);
    DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO, "[halcsd] Smio_id parameter was set to %u.\n",
            fe_smio_id);

    /* We don't need it anymore */
    free (dev_type);
    dev_type = NULL;
    free (dev_id_str);
    dev_id_str = NULL;

    /* Initilialize dev_io */
    DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE, "[halcsd] Creating DEVIO instance ...\n");

    /* Create LOG filename path */
    char *devio_log_filename = _create_log_filename (log_prefix, 
            DEVIO_LOG_FILENAME_PATTERN, dev_id, devio_type_str, 
            fe_smio_id);
    ASSERT_ALLOC (devio_log_filename, err_devio_log_filename_alloc);

    /* Create LOG DEVIO info path */
    char *devio_log_info_filename = _create_log_filename (log_prefix, 
            DEVIO_LOG_INFO_FILENAME_PATTERN, dev_id, devio_type_str, 
            fe_smio_id);
    ASSERT_ALLOC (devio_log_info_filename, err_devio_log_info_filename_alloc);

    char devio_service_str [DEVIO_SERVICE_LEN];
    snprintf (devio_service_str, DEVIO_SERVICE_LEN-1, "HALCS%u:DEVIO", dev_id);
    devio_service_str [DEVIO_SERVICE_LEN-1] = '\0'; /* Just in case ... */
    devio_t *devio = devio_new (devio_service_str, dev_id, dev_entry, llio_ops,
            broker_endp, verbose, devio_log_filename, devio_log_info_filename);
    ASSERT_ALLOC (devio, err_devio_alloc);

    /*  Start DEVIO loop */

    /* Step 1: Loop though all the SDB records and intialize (boot) the
     * smio modules*/
    /* Step 2: Optionally, register the necessary smio modules specifying
     * its ID and calling devio_register_sm */
    /* Step 3: Poll all PIPE's sockets to determine if we have something to
     * handle, like messages from smios */
    /*      Step 3.5: If we do, call devio_handle_smio () and treat its
     *      request as appropriate */
    zactor_t *server = zactor_new (devio_loop, devio);
    if (server == NULL) {
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_FATAL, "[halcsd] Could not spawn "
                "server\n");
        goto err_server;
    }

    /* Print SDB devices into generic log */
    devio_print_info (server);
    /* Print SDB devices into log_info */
    devio_print_info_log (server);

    /* Spawn platform SMIOSs */
    err = _spawn_platform_smios (server, devio_type, fe_smio_id, devio_hints,
            dev_id);
    if (err != DEVIO_SUCCESS) {
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_FATAL, "[halcsd] _spawn_platform_smios error!\n");
        goto err_plat_devio;
    }

    /* FIXME. Currently we have an issue with some SMIOs
     * initialization.
     *
     * If the SMIOs are somewhat dependant, we might need
     * to reconfigure them again once they are all up and ready.
     * That's because the default configuration of some of them
     * (called in smio_config_defaults) might depend on another
     * SMIO initialization.
     *
     * Even worse, in FPGAs, some SMIOs might need an stable clock
     * to be able to configure themselves, but some projects
     * employ a dynamic clock configured on-thr-fly by the SMIO.
     * So, some SMIOs might not even be able to initialize before
     * this other SMIO configure the FPGA clock.
     *
     * Using a more brute-force solution we used to do the following:
     *
     * 1) We reset the endpoint here (not done here as most of 
     *   our endpoints need to recreate its SMCH instances to be 
     *   able to communicate and we don't do that on reconfigure,
     *   only on create)
     * 2) Tell the SMIOs to reconfigure themselves
     *
     * But this posed complications as we could reset the endpoint
     * without re-creating the SMIOs. So, to simplify this we just 
     * destroy the DEVIO and recreate it.
     *
     * */

    /* Wait until all SMIO configuration is executed */
    while (true) {
        char *message = zstr_recv (server);
        if (message == NULL) {
            DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE, "[halcsd] Interrupted while waiting for $SMIO_CFG_DONE\n");
            goto smio_cfg_done_err;
        }

        if (streq (message, "$SMIO_CFG_DONE")) {
            free (message);
            break;
        }
        free (message);
    }

    /* Now that everything is ready spawn INIT SMIO so clients can 
     * be sure HALCS is ready to go */
    devio_register_sm (server, 0xdc64e778, 0, 0);

#if 0
    /* Expose our PIPE port to IPC so other clients can query the server */
    zactor_t *proxy = zactor_new (zproxy, NULL);
    ASSERT_ALLOC (proxy, err_proxy_alloc, DEVIO_ERR_ALLOC);

    const char *backend_endpoint = zsock_endpoint (devio_get_pipe (devio));
    char frontend_endpoint [50];
    snprintf (frontend_endpoint, sizeof (frontend_endpoint), "%s%u", 
        FRONTEND_ENDPOINT_PREFIX, full_dev_id);
    DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO, "[halcsd] Backend (server) endpoint: %s\n", 
        backend_endpoint);
    printf ("[halcsd] Backend (server) endpoint: %s\n", backend_endpoint);
    DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO, "[halcsd] Frontend (clients) endpoint: %s\n", 
        frontend_endpoint);
    printf ("[halcsd] Frontend (server) endpoint: %s\n", frontend_endpoint);
    zstr_sendx (proxy, "FRONTEND", "ROUTER", frontend_endpoint, NULL);
    zsock_wait (proxy);
    zstr_sendx (proxy, "BACKEND", "PAIR", backend_endpoint, NULL);
    zsock_wait (proxy);
#endif

#if 1
    /*  Accept and print any message back from server */
    while (true) {
        char *message = zstr_recv (server);
        if (message) {
            DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE, "[halcsd] %s\n", message);
            free (message);
        }
        else {
            DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE, "[halcsd] Interrupted\n");
            break;
        }
    }
#endif

#if 0
err_proxy_alloc:
#endif
smio_cfg_done_err:
err_plat_devio:
    zactor_destroy (&server);
err_server:
    devio_destroy (&devio);
err_devio_alloc:
    free (devio_log_info_filename);
err_devio_log_info_filename_alloc:
    free (devio_log_filename);
err_devio_log_filename_alloc:
#if defined (__BOARD_AFCV3__) && (__WITH_APP_CFG__)
err_card_slot:
    if (client_cfg != NULL) {
        halcs_client_destroy (&client_cfg);
    }
err_client_cfg:
    /* Kill DEVIO Config process */
    kill (child_devio_cfg_pid, DEVIO_KILL_CFG_SIGNAL);
#endif
err_exit:
    free (log_prefix);
    free (broker_endp);
    free (dev_id_str);
    free (dev_entry);
    free (dev_type);
    free (devio_type_str);
    free (devio_work_dir);
    free (cfg_file);
err_cfg_get_hints:
    zconfig_destroy (&root_cfg);
err_cfg_load:
    zhashx_destroy (&devio_hints);
err_devio_hints_alloc:
err_cfg_not_found:
err_sigmask:
    DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO, "[halcsd] Exiting ...\n");
    return 0;
}

static devio_err_e _rffe_get_dev_entry (uint32_t dev_id, uint32_t fe_smio_id,
        zhashx_t *hints, char **dev_entry)
{
    assert (dev_entry);
    assert (hints);
    devio_err_e err = DEVIO_SUCCESS;

    char hints_key [HUTILS_CFG_HASH_KEY_MAX_LEN];
    int errs = snprintf (hints_key, sizeof (hints_key),
            HUTILS_CFG_HASH_KEY_PATTERN_COMPL, dev_id, fe_smio_id);

    /* Only when the number of characters written is less than the whole buffer,
     * it is guaranteed that the string was written successfully */
    ASSERT_TEST (errs >= 0 && (size_t) errs < sizeof (hints_key),
            "Could not generate configuration hash key for configuration "
            "file", err_cfg_exit, DEVIO_ERR_CFG);

    hutils_hints_t *cfg_item = zhashx_lookup (hints, hints_key);
    /* If key is not found, assume we don't have any more AFE to
     * prepare */
    ASSERT_TEST (cfg_item != NULL && cfg_item->bind != NULL && !streq (cfg_item->bind, ""),
            "Bind address not specified in config file", err_cfg_exit, DEVIO_ERR_CFG);

    *dev_entry = strdup (cfg_item->bind);
    ASSERT_ALLOC (*dev_entry, err_alloc_entry, DEVIO_ERR_ALLOC);

err_alloc_entry:
err_cfg_exit:
    return err;
}

static char *_create_log_filename (char *log_prefix, char *log_filename_pattern, 
        uint32_t dev_id, const char *devio_type, uint32_t smio_inst_id)
{
    /* Set up logdir */
    char *devio_log_filename = zmalloc (LOG_FILENAME_LEN * sizeof (char));
    ASSERT_ALLOC (devio_log_filename, err_devio_log_alloc);

    /* TODO: Check for the validity of the log filename */
    int errs = snprintf (devio_log_filename, LOG_FILENAME_LEN,
            log_filename_pattern, log_prefix,
            dev_id, devio_type, smio_inst_id);
    ASSERT_TEST (errs >= 0 && errs < LOG_FILENAME_LEN,
            "Could not generate DEVIO LOG filename", err_devio_log_gen);

    return devio_log_filename;

err_devio_log_gen:
    free (devio_log_filename);
err_devio_log_alloc:
    return NULL;
}

static devio_err_e _spawn_platform_smios (void *pipe, devio_type_e devio_type,
        uint32_t smio_inst_id, zhashx_t *hints, uint32_t dev_id)
{
    assert (pipe);
    UNUSED(hints);
    UNUSED(dev_id);

    devio_err_e err = DEVIO_SUCCESS;

    switch (devio_type) {
        case BE_DEVIO:
            err = devio_register_all_sm (pipe);
            break;

        case FE_DEVIO:
            err = _spawn_fe_platform_smios (pipe, smio_inst_id);
            break;

        default:
            /* FIXME: increase the error clarity? */
            err = DEVIO_ERR_NO_SMIO_ID;
    }

    if (err != DEVIO_SUCCESS) {
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_FATAL, "[halcsd] devio_register_sm error!\n");
        goto err_register_sm;
    }

err_register_sm:
    return err;
}

static devio_err_e _spawn_fe_platform_smios (void *pipe, uint32_t smio_inst_id)
{
    uint32_t rffe_id = 0x7af21909;
    devio_err_e err = DEVIO_SUCCESS;

    /* RFFE V2 only */
    /* #if defined (__AFE_RFFE_V1__) */
#if defined (__AFE_RFFE_V2__)
    DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO, "[dev_io_fe] Spawning default SMIOs ...\n");
    err = devio_register_sm (pipe, rffe_id, 0, smio_inst_id);
    if (err != DEVIO_SUCCESS) {
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_FATAL, "[dev_io_fe] devio_register_sm error!\n");
    }
#else
#error "FE RFFE Board not supported!"
#endif

    return err;
}
