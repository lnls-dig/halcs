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
#include <errno.h>       /* perror */
#include <sys/types.h>
#include <sys/wait.h>   /* waitpid */
#include <stdarg.h>
#include <sys/stat.h>   /* chmod */

#include "dev_mngr.h"
#include "debug_print.h"

#define DFLT_BIND_FOLDER            "/tmp/bpm"
#define DFLT_BIND_ADDR              "0"
#define IPC_FILE_PERM               0777

/* Global variable for testing an asynchronous event */
volatile sig_atomic_t __new_dev = 0;
volatile sig_atomic_t __dev_nums = 0;

/* This signal handler emulates some device being plugged */
static void dmngr_sigusr1_h (int sig, siginfo_t *siginfo, void *context)
{
    (void) sig;
    (void) context;
    (void) siginfo;

    DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_TRACE, "[dev_mngr] Recv'ed signal: %d\n",
            siginfo->si_signo);
    DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_TRACE, "[dev_mngr] New PCIe device detected!\n");

    /* Simplistic, inefficient method of handling a new_dev */
    __new_dev = 1;
    __dev_nums++;
}

int dmngr_wait_chld_f (void)
{
    int chld_status;
    pid_t chld_pid = waitpid (-1, &chld_status, WNOHANG);

    /* Error or no child exists */
    if (chld_pid == (pid_t) -1) {
        /* Not actually an error if ECHILD. Do nothing... */
        if (errno == ECHILD) {
            /* DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_INFO, "[dev_mngr] no child to wait for\n"); */
            return 0;
        }

        return -1;
    }

    /* Child exists but have not changed its state */
    if (chld_pid == (pid_t) 0) {
        /* DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_INFO, "[dev_mngr] Child has not changed its state\n"); */
        return 0;
    }

    /* Child exists and has changed its state. Check fior the return status */
    if (WIFEXITED (chld_status)) {
        DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_WARN, "[dev_mngr] Child exited%s with status %d\n",
                WCOREDUMP(chld_status) ? " and dumped core" : "",
                WEXITSTATUS(chld_status));
        __dev_nums--;
    }

    if (WIFSTOPPED (chld_status)) {
        DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_WARN, "[dev_mngr] Child stopped by signal %d\n",
                WSTOPSIG(chld_status));
    }

    if (WIFSIGNALED (chld_status)) {
        DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_WARN, "[dev_mngr] Child signalled by signal %d\n",
                WTERMSIG(chld_status));
        __dev_nums--;
    }

    return 0;
}

int dmngr_spawn_chld_f (const char *program, char *const argv[])
{
    /* Fill the devio strucutre and create a new process
     * in charge of handling it */
    pid_t child = fork ();

    if (child == -1) {
        perror ("[dev_mngr] fork");
        /* What to do in case of error? retry ? */
        return -1;
    }
    else if (child == 0) { /* Child */
        int err = execv (program, argv);

        if (err < 0) {
            perror ("[dev_mngr] execl");
            return -1;
        }
    }
    else { /* Parent */
    }

    return 0; /* Success */
}

void print_help (char *program_name)
{
    printf( "Usage: %s [options]\n"
            "\t-h This help message\n"
            "\t-d Daemon mode.\n"
            "\t-v Verbose output\n"
            "\t-l <log_filename> Log filename\n"
            "\t-b <broker_endpoint> Broker endpoint\n", program_name);
}

int main (int argc, char *argv[])
{
    int verbose = 0;
    int daemonize = 0;
    char *broker_endp = NULL;
    char *log_file_name = NULL;
    char **str_p = NULL;
    int i;

    if (argc < 2) {
        print_help (argv[0]);
        exit (1);
    }

    /* FIXME: This is rather buggy! */
    /* Simple handling of command-line options. This should be done
     * with getopt, for instance*/
    for (i = 1; i < argc; i++)
    {
        if (streq(argv[i], "-v")) {
            verbose = 1;
            DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_TRACE, "[dev_mngr] Verbose mode set\n");
        }
        else if (streq(argv[i], "-d")) {
            daemonize = 1;
            DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_TRACE, "[dev_mngr] Demonize mode set\n");
        }
        else if (streq (argv[i], "-b")) {
            str_p = &broker_endp;
            DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_TRACE, "[dev_mngr] Will set broker_endp parameter\n");
        }
        else if (streq (argv[i], "-l")) {
            str_p = &log_file_name;
            DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_TRACE, "[dev_mngr] Will set log filename\n");
        }
        else if (streq(argv[i], "-h"))
        {
            print_help (argv [0]);
            exit (1);
        }
        /* Fallout for options with parameters */
        else {
            if (str_p) {
                *str_p = strdup (argv[i]);
                DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_TRACE, "[dev_mngr] Parameter set to \"%s\"\n", *str_p);
            }
        }
    }

    /* Set default broker address */
    if (broker_endp == NULL) {
        broker_endp = DFLT_BIND_FOLDER"/"DFLT_BIND_ADDR;
    }

    /* Daemonize dev_mngr */
    if (daemonize != 0) {
        int rc = daemon(0, 0);

        if (rc != 0) {
            perror ("[dev_mngr] daemon");
            goto err_daemonize;
        }
    }

    /* See the fake promiscuous endpoint tcp*:*. To be changed soon! */
    dmngr_t *dmngr = dmngr_new ("dev_mngr", "tcp://*:*", verbose, log_file_name);
    if (dmngr == NULL) {
        DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_FATAL, "[dev_mngr] Fail to allocate dev_mngr instance\n");
        goto err_dmngr_alloc;
    }

    dmngr_sig_handler_t dmngr_sigusr1_handler =
    {.signal = SIGUSR1,
     .dmngr_sig_h = dmngr_sigusr1_h};
    /* dmngr_sigusr1_handler_t dmngr_sigkill_handler =
    {.signal = SIGKILL,
     .dmngr_sig_h = dmngr_sigusr1_handler};
    dmngr_sigusr1_handler_t dmngr_sigterm_handler =
    {.signal = SIGTERM,
     .dmngr_sig_h = dmngr_sigusr1_handler}; */

    dmngr_err_e err = dmngr_set_sig_handler (dmngr, &dmngr_sigusr1_handler);
    if (err != DMNGR_SUCCESS) {
        DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_FATAL, "[dev_mngr] dmngr_set_sig_handler error!\n");
        goto err_exit;
    }

    dmngr_set_wait_clhd_handler (dmngr, &dmngr_wait_chld_f);
    dmngr_set_spawn_clhd_handler (dmngr, &dmngr_spawn_chld_f);

    err = dmngr_register_sig_handlers (dmngr);
    if (err != DMNGR_SUCCESS) {
        DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_FATAL, "[dev_mngr] dmngr_register_sig_handler error!\n");
        goto err_exit;
    }

    /* Here we should monitor the plug of new devices, such as
     * PCIe devices, by means of a udev event (or some other) and
     * Ethernet ones, by using a discovery protocol based on zeroMQ
     * (zbeacon should provide a sufficient infrastrucutre for that)
     */
    DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_TRACE, "[dev_mngr] Waiting for new device\n");
    DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_TRACE, "[dev_mngr] PID: %d\n", getpid());

    /* Do until a C^c is pressed (daemon mode unset) or SIGTERM signal arrives */
    while (!zctx_interrupted) {

        /* DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_TRACE, "[dev_mngr] ., PID: %d\n", getpid()); */
        if (__new_dev) {

            __new_dev = 0;

            /* Verify if the Broker is running. If not, spawn it */
            if (!dmngr_is_broker_running (dmngr)) {
                DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_TRACE, "[dev_mngr] Spawing Broker\n");
                char *argv_exec[] = {"mdp_broker", broker_endp, NULL};
                /* char *argv_exec[] = {"mdp_broker", "-v", NULL}; */
                int spawn_err = dmngr_spawn_chld (dmngr, "mdp_broker", argv_exec);

                /* Just fail miserably, for now */
                if (spawn_err < 0) {
                    perror ("spwan");
                    goto err_exit;
                }
            }

            /* Argument options are "process name", "device type" and
             *"dev entry" */
            DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_TRACE, "[dev_mngr] Spawing DEVIO worker\n");
            /* FIXME: change devio.log to devio%d.log as multiple devio might exist! */
            char *argv_exec[] = {"dev_io", "-t", "pcie", "-e", "/dev/fpga0",
                "-b", broker_endp, "-l", "dev_io.log", NULL};
            int spawn_err = dmngr_spawn_chld (dmngr, "./dev_io", argv_exec);

            /* Just fail miserably, for now */
            if (spawn_err < 0) {
                perror ("spwan");
                goto err_exit;
            }
        }

        /* Check the status of child processes */
        /* DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_TRACE,
         * "[dev_mngr] PID: %d will wait for child. # = %d\n", getpid(),  __dev_nums); */
        err = dmngr_wait_chld (dmngr);

        if (err != DMNGR_SUCCESS) {
            //perror("[dev_mngr] waitpid");
            goto err_exit;
            exit (1);
        }

        /* Do some monitoring activities */
        sleep(1);
    }

err_exit:
    dmngr_destroy (&dmngr);
err_dmngr_alloc:
err_daemonize:
    str_p = &log_file_name;
    free (*str_p);
    str_p = &broker_endp;
    free (*str_p);
    broker_endp = NULL;

    return 0;
}
