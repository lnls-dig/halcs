#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>          /* getpid, getppid */
#include <unistd.h>

#include "czmq.h"
#include "dev_io.h"
#include "debug_print.h"
#include "ll_io_utils.h"
#include "board.h"

#define DEVIO_SERVICE_LEN       50

static devio_err_e spwan_platform_smios (devio_t *devio);

void print_help (char *program_name)
{
    printf( "Usage: %s [options]\n"
            "\t-h This help message\n"
            "\t-d Daemon mode.\n"
            "\t-v Verbose output\n"
            "\t-t <device_type> Device type\n"
            "\t-e <dev_entry> Device /dev entry\n"
            "\t-i <dev_id> Device ID\n"
            "\t-l <log_filename> Log filename\n"
            "\t-b <broker_endpoint> Broker endpoint\n", program_name);
}

int main (int argc, char *argv[])
{
    int verbose = 0;
    int daemonize = 0;
    char *dev_type = NULL;
    char *dev_entry = NULL;
    char *dev_id_str = NULL;
    uint32_t dev_id = 0;
    char *broker_endp = NULL;
    char *log_file_name = NULL;
    char **str_p = NULL;
    int i;

    if (argc < 5) {
        print_help (argv[0]);
        exit (1);
    }

    /* FIXME: This is rather buggy! */
    /* Simple handling of command-line options. This should be done
     * with getopt, for instance*/
    for (i = 1; i < argc; i++)
    {
        if (streq (argv[i], "-v")) {
            verbose = 1;
            DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE, "[dev_io] Verbose mode set\n");
        }
        else if (streq (argv[i], "-d")) {
            daemonize = 1;
            DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE, "[dev_io] Demonize mode set\n");
        }
        else if (streq (argv[i], "-t")) {
            str_p = &dev_type;
            DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE, "[dev_io] Will set dev_type parameter\n");
        }
        else if (streq (argv[i], "-e")) {
            str_p = &dev_entry;
            DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE, "[dev_io] Will set dev_entry parameter\n");
        }
        else if (streq (argv[i], "-i")) {
            str_p = &dev_id_str;
            DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE, "[dev_io] Will set dev_id_str parameter\n");
        }
        else if (streq (argv[i], "-b")) {
            str_p = &broker_endp;
            DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE, "[dev_io] Will set broker_endp parameter\n");
        }
        else if (streq (argv[i], "-l")) {
            str_p = &log_file_name;
            DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE, "[dev_io] Will set log filename\n");
        }
        else if (streq (argv[i], "-h")) {
            print_help (argv[0]);
            exit (1);
        }
        /* Fallout for options with parameters */
        else {
            if (str_p) {
                *str_p = strdup (argv[i]);
                DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE, "[dev_io] Parameter set to \"%s\"\n", *str_p);
            }
        }
    }

    /* Daemonize dev_io */
    if (daemonize != 0) {
        int rc = daemon(0, 0);

        if (rc != 0) {
            perror ("[dev_io] daemon");
            goto err_exit;
        }
    }

    llio_type_e llio_type = llio_str_to_type (dev_type);
    /* Parse command-line options */
    if (llio_type == INVALID_DEV) {
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_FATAL, "[dev_io] Dev_type parameter is invalid\n");
        goto err_exit;
    }

    /* Check for device entry */
    if (dev_entry == NULL) {
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO, "[dev_io] Dev_entry parameter was not set. Exiting ...\n");
        goto err_exit;
    }

    /* Check for device ID */
    if (dev_id_str == NULL) {
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO, "[dev_io] Dev_id parameter was not set.\n"
                "\tDefaulting it to the /dev file number ...\n");

        int matches = sscanf (dev_entry, "/dev/fpga%u", &dev_id);
        if (matches == 0) {
            DBE_DEBUG (DBG_DEV_IO | DBG_LVL_FATAL, "[dev_io] Dev_entry parameter is invalid.\n"
                    "\tIt must be in the format \"/dev/fpga<device_number>\". Exiting ...\n");
            goto err_exit;
        }
    }
    /* Use the passed ID */
    else {
        dev_id = strtoul (dev_id_str, NULL, 10);
    }

    DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO, "[dev_io] Dev_id parameter was set to %u.\n",
            dev_id);

    /* We don't need it anymore */
    str_p = &dev_type;
    free (*str_p);
    dev_type = NULL;
    str_p = &dev_id_str;
    free (*str_p);
    dev_id_str = NULL;

    /* Initilialize dev_io */
    DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE, "[dev_io] Creating DEVIO instance ...\n");

    char devio_service_str [DEVIO_SERVICE_LEN];
    snprintf (devio_service_str, DEVIO_SERVICE_LEN-1, "BPM%u:DEVIO", dev_id);
    devio_service_str [DEVIO_SERVICE_LEN-1] = '\0'; /* Just in case ... */
    devio_t *devio = devio_new (devio_service_str, dev_entry, llio_type,
            broker_endp, verbose, log_file_name);
    /* devio_t *devio = devio_new ("BPM0:DEVIO", *str_p, llio_type,
            "tcp://localhost:5555", verbose); */

    if (devio == NULL) {
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_FATAL, "[dev_io] devio_new error!\n");
        goto err_exit;
    }

    /* We don't need it anymore */
    str_p = &dev_entry;
    free (*str_p);
    dev_entry = NULL;
    str_p = &broker_endp;
    free (*str_p);
    broker_endp = NULL;

    devio_err_e err = spwan_platform_smios (devio);
    if (err != DEVIO_SUCCESS) {
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_FATAL, "[dev_io] spwan_platform_smios error!\n");
        goto err_devio;
    }

    /* err = devio_init_poller_sm (devio); */
    err = devio_init_poller2_sm (devio);
    if (err != DEVIO_SUCCESS) {
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_FATAL, "[dev_io] devio_init_poller_sm error!\n");
        goto err_devio;
    }

    while (!zctx_interrupted) {
        /* Step 1: Loop though all the SDB records and intialize (boot) the
         * smio modules*/
        /* Step 2: Optionally, register the necessary smio modules specifying
         * its ID and calling devio_register_sm */
        /* Step 3: Poll all PIPE's sockets to determine if we have something to
         * handle, like messages from smios */
        /*      Step 3.5: If we do, call devio_handle_smio () and treat its
         *      request as appropriate */

        /* devio_poll_all_sm (devio); */
        devio_poll2_all_sm (devio);
    }

err_devio:
    devio_destroy (&devio);
err_exit:
    str_p = &log_file_name;
    free (*str_p);
    str_p = &broker_endp;
    free (*str_p);
    str_p = &dev_id_str;
    free (*str_p);
    str_p = &dev_entry;
    free (*str_p);
    str_p = &dev_type;
    free (*str_p);
    return 0;
}

static devio_err_e spwan_platform_smios (devio_t *devio)
{
    assert (devio);

    uint32_t fmc130m_4ch_id = 0x7085ef15;
    uint32_t acq_id = 0x4519a0ad;
    uint32_t dsp_id = 0x1bafbf1e;
    uint32_t swap_id = 0x12897592;
    devio_err_e err;

    /* ML605 or AFCv3 */
#if defined (__BOARD_ML605__) || defined (__BOARD_AFCV3__)
    DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO, "[dev_io] Spawning default SMIOs ...\n");
    err = devio_register_sm (devio, fmc130m_4ch_id, FMC1_130M_BASE_ADDR, 0);
    if (err != DEVIO_SUCCESS) {
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_FATAL, "[dev_io] devio_register_sm error!\n");
        goto err_register_sm;
    }

    err = devio_register_sm (devio, acq_id, WB_ACQ1_BASE_ADDR, 0);
    if (err != DEVIO_SUCCESS) {
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_FATAL, "[dev_io] devio_register_sm error!\n");
        goto err_register_sm;
    }

    err = devio_register_sm (devio, dsp_id, DSP1_BASE_ADDR, 0);
    if (err != DEVIO_SUCCESS) {
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_FATAL, "[dev_io] devio_register_sm error!\n");
        goto err_register_sm;
    }

    err = devio_register_sm (devio, swap_id, DSP1_BASE_ADDR, 0);
    if (err != DEVIO_SUCCESS) {
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_FATAL, "[dev_io] devio_register_sm error!\n");
        goto err_register_sm;
    }
    /* AFCv3 spefific */
#if defined (__BOARD_AFCV3__)
    DBE_DEBUG (DBG_DEV_IO | DBG_LVL_INFO, "[dev_io] Spawning AFCv3 specific SMIOs ...\n");

    err = devio_register_sm (devio, fmc130m_4ch_id, FMC2_130M_BASE_ADDR, 1);
    if (err != DEVIO_SUCCESS) {
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_FATAL, "[dev_io] devio_register_sm error!\n");
        goto err_register_sm;
    }

    err = devio_register_sm (devio, acq_id, WB_ACQ2_BASE_ADDR, 1);
    if (err != DEVIO_SUCCESS) {
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_FATAL, "[dev_io] devio_register_sm error!\n");
        goto err_register_sm;
    }

    err = devio_register_sm (devio, dsp_id, DSP2_BASE_ADDR, 1);
    if (err != DEVIO_SUCCESS) {
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_FATAL, "[dev_io] devio_register_sm error!\n");
        goto err_register_sm;
    }

    err = devio_register_sm (devio, swap_id, DSP2_BASE_ADDR, 1);
    if (err != DEVIO_SUCCESS) {
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_FATAL, "[dev_io] devio_register_sm error!\n");
        goto err_register_sm;
    }
#endif
#else
#error "Board not supported!"
#endif

err_register_sm:
    return err;
}

