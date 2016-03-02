/*
 * Copyright (C) 2015 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "bpm_server.h"

#define DEVIO_SERVICE_LEN       50

static devio_err_e _spawn_platform_smios (devio_t *devio, devio_type_e devio_type,
        uint32_t smio_inst_id);
static devio_err_e _spawn_be_platform_smios (devio_t *devio);

static struct option long_options[] =
{
    {"help",                no_argument,         NULL, 'h'},
    {"brokerendp",          required_argument,   NULL, 'b'},
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

static const char* shortopt = "hb:dw:vn:t:e:i:l:";

void print_help (char *program_name)
{
    fprintf (stdout, "EBPM Device Config I/O\n"
            "Usage: %s [options]\n"
            "Version %s\n, Build by: %s, %s\n"
            "\n"
            "  -h  --help                           Display this usage information\n"
            "  -b  --brokerendp <Broker endpoint>   Broker endpoint\n"
            "  -d  --daemon                         Run as system daemon.\n"
            "  -w  --daemonworkdir <Work Directory> Daemon working directory.\n"
            "  -v  --verbose                        Verbose output\n"
            "  -n  --deviotype <[be|fe]>            Devio type\n"
            "  -t  --devicetype <[eth|pcie]>        Device type\n"
            "  -e  --deviceentry <[ip_addr|/dev entry]>\n"
            "                                       Device entry\n"
            "  -i  --deviceid <Device ID>           Device ID\n"
            "  -l  --logfilename <Log filename>     Log filename\n",
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
    char *log_filename = NULL;
    int opt;

    while ((opt = getopt_long (argc, argv, shortopt, long_options, NULL)) != -1) {
        /* Get the user selected options */
        switch (opt) {
            /* Display Help */
            case 'h':
                print_help (argv [0]);
                exit (1);
                break;

            case 'b':
                DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE, "[ebpm_cfg] Will set broker_endp parameter\n");
                broker_endp = strdup (optarg);
                break;

            case 'd':
                DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_TRACE, "[ebpm_cfg] Will set daemon parameter\n");
                devio_daemonize = 1;
                break;

            case 'w':
                DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE, "[ebpm_cfg] Will set devio_work_dir parameter\n");
                devio_work_dir = strdup (optarg);
                break;

            case 'v':
                DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_TRACE, "[ebpm_cfg] Will set verbose parameter\n");
                verbose = 1;
                break;

            case 'n':
                DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE, "[ebpm_cfg] Will set devio_type parameter\n");
                devio_type_str = strdup (optarg);
                break;

            case 't':
                DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE, "[ebpm_cfg] Will set dev_type parameter\n");
                dev_type = strdup (optarg);
                break;

            case 'e':
                DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE, "[ebpm_cfg] Will set dev_type parameter\n");
                dev_entry = strdup (optarg);
                break;

            case 'i':
                DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE, "[ebpm_cfg] Will set dev_id_str parameter\n");
                dev_id_str = strdup (optarg);
                break;

            case 'l':
                DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE, "[ebpm_cfg] Will set log filename\n");
                log_filename = strdup (optarg);
                break;

            case '?':
                DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_FATAL, "[ebpm_cfg] Option not recognized or missing argument\n");
                print_help (argv [0]);
                exit (1);
                break;

            default:
                DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_FATAL, "[ebpm_cfg] Could not parse options\n");
                print_help (argv [0]);
                exit (1);
        }
    }

    /* Daemonize dev_io */
    if (devio_daemonize != 0) {
        if (devio_work_dir == NULL) {
            DBE_DEBUG (DBG_DEV_IO | DBG_LVL_FATAL, "[ebpm_cfg] Daemon working directory not specified\n");
            goto err_exit;
        }

        int rc = zsys_daemonize (devio_work_dir);
        if (rc != 0) {
            DBE_DEBUG (DBG_DEV_IO | DBG_LVL_FATAL, "[ebpm_cfg] Fail to daemonize\n");
            goto err_exit;
        }
    }

    llio_type_e llio_type = llio_str_to_type (dev_type);
    /* Parse command-line options */
    if (llio_type == INVALID_DEV) {
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_FATAL, "[ebpm_cfg] Dev_type parameter is invalid\n");
        goto err_exit;
    }

    devio_type_e devio_type = devio_str_to_type (devio_type_str);
    /* Parse command-line options */
    if (devio_type == INVALID_DEVIO) {
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_FATAL, "[ebpm_cfg] Devio_type parameter is invalid\n");
        goto err_exit;
    }

    /* Check for device entry */
    if (dev_entry == NULL) {
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO, "[ebpm_cfg] Dev_entry parameter was not set. Exiting ...\n");
        goto err_exit;
    }

    uint32_t dev_id = 0;
    /* Check for device ID */
    if (dev_id_str == NULL) {
        switch (llio_type) {
            case ETH_DEV:
                DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO, "[ebpm_cfg] Dev_id parameter was not set. Exiting ...\n");
                goto err_exit;
            break;

            case PCIE_DEV:
                DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO, "[ebpm_cfg] Dev_id parameter was not set.\n"
                    "\tDefaulting it to the /dev file number ...\n");

                /* Our device follows the convention of having the ID in hexadecimal
                 * code. For instance, /dev/fpga-0c00 would be a valid ID */
                int matches = sscanf (dev_entry, "/dev/fpga-%u", &dev_id);
                if (matches == 0) {
                    DBE_DEBUG (DBG_DEV_IO | DBG_LVL_FATAL, "[ebpm_cfg] Dev_entry parameter is invalid.\n"
                            "\tIt must be in the format \"/dev/fpga-<device_number>\". Exiting ...\n");
                    goto err_exit;
                }
                break;

            default:
                DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO, "[ebpm_cfg] Dev_id parameter was not set. Exiting ...\n");
                goto err_exit;
        }
    }
    /* Use the passed ID, interpret it as hexadecimal number */
    else {
        dev_id = strtoul (dev_id_str, NULL, 10);
    }

    DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO, "[ebpm_cfg] Dev_id parameter was set to %u.\n",
            dev_id);

    /* We don't need it anymore */
    free (dev_type);
    dev_type = NULL;
    free (dev_id_str);
    dev_id_str = NULL;

    /* Initilialize dev_io */
    DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE, "[ebpm_cfg] Creating DEVIO instance ...\n");

    char devio_service_str [DEVIO_SERVICE_LEN];
    snprintf (devio_service_str, DEVIO_SERVICE_LEN-1, "BPM%u:DEVIO_CFG", dev_id);
    devio_service_str [DEVIO_SERVICE_LEN-1] = '\0'; /* Just in case ... */
    devio_t *devio = devio_new (devio_service_str, dev_id, dev_entry, llio_type,
            broker_endp, verbose, log_filename);

    if (devio == NULL) {
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_FATAL, "[ebpm_cfg] devio_new error!\n");
        goto err_exit;
    }

    /* We don't need it anymore */
    free (dev_entry);
    dev_entry = NULL;
    free (broker_endp);
    broker_endp = NULL;

    devio_err_e err = _spawn_platform_smios (devio, devio_type, 0);
    if (err != DEVIO_SUCCESS) {
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_FATAL, "[ebpm_cfg] _spawn_platform_smios error!\n");
        goto err_devio;
    }

    /* Step 1: Loop though all the SDB records and intialize (boot) the
     * smio modules*/
    /* Step 2: Optionally, register the necessary smio modules specifying
     * its ID and calling devio_register_sm */
    /* Step 3: Poll all PIPE's sockets to determine if we have something to
     * handle, like messages from smios */
    /*      Step 3.5: If we do, call devio_handle_smio () and treat its
     *      request as appropriate */
    err = devio_loop (devio);
    if (err != DEVIO_SUCCESS) {
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_FATAL, "[ebpm_cfg] devio_loop error: %s\n",
                devio_err_str (err));
        goto err_devio;
    }

err_devio:
    devio_destroy (&devio);
err_exit:
    free (log_filename);
    free (broker_endp);
    free (dev_id_str);
    free (dev_entry);
    free (dev_type);
    free (devio_work_dir);
    free (devio_type_str);
    DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO, "[ebpm_cfg] Exiting ...\n");
    return 0;
}

static devio_err_e _spawn_platform_smios (devio_t *devio, devio_type_e devio_type,
        uint32_t smio_inst_id)
{
    (void) smio_inst_id;

    assert (devio);

    devio_err_e err = DEVIO_SUCCESS;

    switch (devio_type) {
        case BE_DEVIO:
            err = _spawn_be_platform_smios (devio);
            break;

        default:
            /* FIXME: increase the error clarity? */
            err = DEVIO_ERR_NO_SMIO_ID;
    }

    if (err != DEVIO_SUCCESS) {
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_FATAL, "[ebpm_cfg] devio_register_sm error!\n");
        goto err_register_sm;
    }

err_register_sm:
    return err;
}

static devio_err_e _spawn_be_platform_smios (devio_t *devio)
{
    devio_err_e err = DEVIO_SUCCESS;

    /* ML605 specific */
#if defined (__BOARD_ML605__)
    (void) devio;
    goto err_register_sm;

    /* AFCv3 spefific */
#elif defined (__BOARD_AFCV3__)
    uint32_t afc_diag_id = 0x51954750;

    DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO, "[ebpm_cfg] Spawning AFCv3 specific SMIOs ...\n");

    err = devio_register_sm (devio, afc_diag_id, WB_AFC_DIAG_BASE_ADDR, 0);
    if (err != DEVIO_SUCCESS) {
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_FATAL, "[ebpm_cfg] devio_register_sm error!\n");
        goto err_register_sm;
    }

#else
#error "BE FPGA Board not supported!"
#endif

err_register_sm:
    return err;
}

