/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _DEV_MNGR_CORE_H_
#define _DEV_MNGR_CORE_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Configuration variables. To be filled by dev_mngr */
extern const char *dmngr_log_filename;
extern char *dmngr_log_dir;
extern char *dmngr_broker_endp;
extern char *dmngr_verbose_str;
extern int dmngr_verbose;
extern char *dmngr_daemonize_str;
extern int dmngr_daemonize;
extern char *dmngr_work_dir;
extern char *dmngr_spawn_broker_cfg_str;
extern int dmngr_spawn_broker_cfg;

/* Signal handler function pointer */
typedef void (*sig_handler_fp)(int sig, siginfo_t *siginfo, void *context);
/* Wait child handler function pointer */
typedef int (*wait_chld_handler_fp)(void);
/* Spawn child handler function pointer */
typedef int (*spawn_chld_handler_fp)(const char *program, char *const argv[]);
/* Spawn broker handler function pointer */
typedef int (*spawn_broker_handler_fp)(const char *program, char *const argv[]);

/* Node of sig_ops list */
typedef struct {
    int signal;         /* Signal identifier, e.g., SIGINT, SIGKILL, etc... */
    sig_handler_fp dmngr_sig_h;
} dmngr_sig_handler_t;

typedef struct {
    wait_chld_handler_fp dmngr_wait_chld;       /* Called to wait a all child process */
    spawn_chld_handler_fp dmngr_spawn_chld;     /* Called to spawn a new process to handle device */
    spawn_broker_handler_fp dmngr_spawn_broker; /* Called to spawn (or respawn a zeroMQ broker */

    /* List of dmngr_sig_handler_t */
    zlist_t *sig_ops;
} dmngr_ops_t;

/***************** Our methods *****************/

/* Creates a new instance of the Device Manager */
dmngr_t * dmngr_new (char *name, char *endpoint, int verbose,
        const char *log_prefix, zhash_t *hints_h);
/* Destroy an instance of the Device Manager */
dmngr_err_e dmngr_destroy (dmngr_t **self_p);

/* Register signals to Device Manager instance */
dmngr_err_e dmngr_set_sig_handler (dmngr_t *self, dmngr_sig_handler_t *sig_handler);
/* Register all signal handlers previously set */
dmngr_err_e dmngr_register_sig_handlers (dmngr_t *self);
/* Register function to wait a all child process */
dmngr_err_e dmngr_set_wait_clhd_handler (dmngr_t *self, wait_chld_handler_fp fp);
/* Execute function to wait a all child process */
dmngr_err_e dmngr_wait_chld (dmngr_t *self);
/* Register function to spawn a all child process */
dmngr_err_e dmngr_set_spawn_clhd_handler (dmngr_t *self, spawn_chld_handler_fp fp);
/* Execute function to spawn a all child process */
dmngr_err_e dmngr_spawn_chld (dmngr_t *self, const char *program, char *const argv[]);

/* Setting all operations at once */
dmngr_err_e dmngr_set_ops (dmngr_t *self, dmngr_ops_t *dmngr_ops);
/* Set configuration filename */
dmngr_err_e dmngr_set_cfg_file (dmngr_t *self, char *cfg_file);
/* Get const reference to configuration filename */
const char * dmngr_get_cfg_file (dmngr_t *self);
/* Clone configuration filename */
char * dmngr_clone_cfg_file (dmngr_t *self);
/* Is broker Running? */
bool dmngr_is_broker_running (dmngr_t *self);
/* Spawn broker if not running */
dmngr_err_e dmngr_spawn_broker (dmngr_t *self, char *broker_endp);
/* Scan for Devices to control */
dmngr_err_e dmngr_scan_devs (dmngr_t *self, uint32_t *num_devs_found);
/* Spwan all devices previously found by dmngr_scan_devs () */
dmngr_err_e dmngr_spawn_all_devios (dmngr_t *self, char *broker_endp,
        char *devio_log_filename, bool respawn_killed_devio);

#ifdef __cplusplus
}
#endif

#endif
