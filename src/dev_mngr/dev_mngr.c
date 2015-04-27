/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include <unistd.h>
#include <signal.h>      /* sigaction */
//#include <asm/siginfo.h> /* siginfo_t */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdarg.h>
#include <sys/stat.h>   /* chmod */

#include "dev_mngr.h"
#include "varg_macros.h"
#include "errhand.h"
#include "hutils.h"

#define DFLT_BIND_FOLDER            "/tmp/bpm"
#define DFLT_BIND_ADDR              "0"
#define IPC_FILE_PERM               0777

#define DFLT_LOG_DIR                "stdout"

#ifdef __CFG_DIR__
#define CFG_DIR                     STRINGIFY(__CFG_DIR__)
#else
#error "Config directory not defined!"
#endif

#ifdef __CFG_FILENAME__
#define CFG_FILENAME                STRINGIFY(__CFG_FILENAME__)
#else
#error "Config filename not defined!"
#endif

void print_help (char *program_name)
{
    printf( "Usage: %s [options]\n"
            "\t-h This help message\n"
            "\n\t Most of the options resides at the bpm_sw configuration file,\n"
            "typically located in /etc/bpm_sw", program_name);
}

int main (int argc, char *argv[])
{
    /* Simple handling of command-line options. This should be done
     * with getopt, for instance*/
    int i;
    for (i = 1; i < argc; i++)
    {
        if (streq(argv[i], "-h"))
        {
            print_help (argv [0]);
            exit (1);
        }
    }

    zhash_t *dmngr_hints = zhash_new ();
    if (dmngr_hints == NULL) {
        DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_FATAL, "[dev_mngr] Could allocate "
                "hints hash table\n");
        goto err_dmngr_hints_alloc;
    }

    /**************************************************************************/
    /************ Read configuration variables from config file ***************/
    /**************************************************************************/

    /* Check for field not found */
    zconfig_t *root_cfg = zconfig_load (CFG_DIR "/" CFG_FILENAME);
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

    /* Read DEVIO suggested bind endpoints and fill the hash table with
     * the corresponding keys */
    dmngr_err_e err = dmngr_get_hints (root_cfg, dmngr_hints);
    if (err != DMNGR_SUCCESS) {
        DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_FATAL, "[dev_mngr] Could not get hints "
                "from configuration file\n");
        goto err_cfg_exit;
    }

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

    /**************************************************************************/
    /********************** END of configuration variables ********************/
    /**************************************************************************/

    /* Daemonize dev_mngr */
    if (dmngr_daemonize != 0) {
        int rc = daemon(0, 0);

        if (rc != 0) {
            perror ("[dev_mngr] daemon");
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
    while (!zctx_interrupted) {

        /* DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_TRACE, "[dev_mngr] ., PID: %d\n", getpid()); */

        /* Spawn the broker if not running */
        err = dmngr_spawn_broker (dmngr, dmngr_broker_endp);
        if (err != DMNGR_SUCCESS) {
            DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_FATAL, "[dev_mngr] Could not spwan broker!\n");
            goto err_spawn_broker;
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
err_dmngr_alloc:
err_daemonize:
    zconfig_destroy (&root_cfg);
err_cfg_exit:
    zhash_destroy (&dmngr_hints);
err_dmngr_hints_alloc:

    DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_FATAL, "[dev_mngr] Exiting ...\n");

    return 0;
}
