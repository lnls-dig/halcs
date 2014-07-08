#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>          /* getpid, getppid */
#include <unistd.h>

#include "czmq.h"
#include "dev_io.h"
#include "debug_print.h"

void print_help (char *program_name)
{
    printf( "Usage: %s [options]\n"
            "\t-h This help message\n"
            "\t-d Daemon mode.\n"
            "\t-v Verbose output\n"
            "\t-t <device_type> Device type\n"
            "\t-e <dev_entry> Device /dev entry\n"
            "\t-l <log_filename> Log filename\n"
            "\t-b <broker_endpoint> Broker endpoint\n", program_name);
}

int main (int argc, char *argv[])
{
    int verbose = 0;
    int daemonize = 0;
    char *dev_type = NULL;
    char *dev_entry = NULL;
    char *broker_endp = NULL;
    char *log_file_name = NULL;
    char **str_p = NULL;
    int i;

    if (argc < 4) {
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

    llio_type_e llio_type;
    /* Parse command-line options */
    if (streq (dev_type, "pcie")) {
        llio_type = PCIE_DEV;
    }
    else if (streq (dev_type, "eth")) {
        llio_type = ETH_DEV;
    }
    else {
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_FATAL, "[dev_io] Dev_type parameter is invalid\n");
        goto err_exit;
    }

    /* We don't need it anymore */
    str_p = &dev_type;
    free (*str_p);
    dev_type = NULL;

    /* Initilialize dev_io */
    DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE, "[dev_io] Creating devio instance ...\n");
    devio_t *devio = devio_new ("BPM0:DEVIO", dev_entry, llio_type,
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

    uint32_t fmc130m_4ch_id = 0x7085ef15;
    uint32_t acq_id = 0x4519a0ad;
    uint32_t dsp_id = 0x1bafbf1e;
    uint32_t swap_id = 0x12897592;
    devio_err_e err;

    err = devio_register_sm (devio, fmc130m_4ch_id, NULL);
    if (err != DEVIO_SUCCESS) {
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_FATAL, "[dev_io] devio_register_sm error!\n");
        goto err_devio;
    }

    err = devio_register_sm (devio, acq_id, NULL);
    if (err != DEVIO_SUCCESS) {
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_FATAL, "[dev_io] devio_register_sm error!\n");
        goto err_devio;
    }

    err = devio_register_sm (devio, dsp_id, NULL);
    if (err != DEVIO_SUCCESS) {
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_FATAL, "[dev_io] devio_register_sm error!\n");
        goto err_devio;
    }

    err = devio_register_sm (devio, swap_id, NULL);
    if (err != DEVIO_SUCCESS) {
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_FATAL, "[dev_io] devio_register_sm error!\n");
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
    str_p = &dev_entry;
    free (*str_p);
    str_p = &dev_type;
    free (*str_p);
    return 0;
}
