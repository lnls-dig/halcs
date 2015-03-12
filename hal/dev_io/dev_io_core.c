/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include <string.h>
#include <stdio.h>

#include "dev_io_core.h"
#include "dev_io_err.h"
#include "hal_assert.h"
#include "sm_io_mod_dispatch.h"
#include "msg.h"
#include "rw_param.h"
#include "sm_io_thsafe_codes.h"
#include "sm_io_bootstrap.h"
#include "ll_io_utils.h"
#include "hal_utils.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...)  \
    ASSERT_HAL_TEST(test_boolean, DEV_IO, "[dev_io_core]",  \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_ALLOC(ptr, DEV_IO, "[dev_io_core]",          \
            devio_err_str(DEVIO_ERR_ALLOC),                 \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                            \
    CHECK_HAL_ERR(err, DEV_IO, "[dev_io_core]",             \
            devio_err_str (err_type))

#define LLIO_STR                            ":LLIO\0"
#define DEVIO_POLLER_TIMEOUT                100        /* in msec */
#define DEVIO_DFLT_LOG_MODE                 "w"

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

/* Do the SMIO operation */
static devio_err_e _devio_do_smio_op (devio_t *self, void *msg);
static devio_err_e _devio_send_destruct_msg (devio_t *self, void *pipe);
static devio_err_e _devio_destroy_smio (devio_t *self, const char *smio_key);
static devio_err_e _devio_destroy_smio_all (devio_t *self);

/* Creates a new instance of Device Information */
devio_t * devio_new (char *name, char *endpoint_dev, llio_type_e type,
        char *endpoint_broker, int verbose, const char *log_file_name)
{
    assert (name);
    assert (endpoint_dev);
    assert (endpoint_broker);

    /* Set logfile available for all dev_mngr and dev_io instances.
     * We accept NULL as a parameter, meaning to suppress all messages */
    debug_set_log (log_file_name, DEVIO_DFLT_LOG_MODE);

    char *dev_type_c = llio_type_to_str (type);
    DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_INFO, "[dev_io_core] Spawing DEVIO worker"
            " with exported service %s, for a %s device \n\tlocated on %s,"
            " broker address %s, with logfile on %s ...\n", name, dev_type_c,
            endpoint_dev, endpoint_broker, (log_file_name == NULL) ? "NULL" : log_file_name);
    free (dev_type_c);

    devio_t *self = (devio_t *) zmalloc (sizeof *self);
    ASSERT_ALLOC(self, err_self_alloc);

    self->log_file = strdup (log_file_name);
    ASSERT_TEST((self->log_file == NULL && log_file_name == NULL)
            || (self->log_file != NULL && log_file_name != NULL),
            "Error setting log file!", err_log_file);

    /* Initialize the sockets structure to talk to nodes */
    self->pipes = zmalloc (sizeof (*self->pipes) * NODES_MAX_LEN);
    ASSERT_ALLOC(self->pipes, err_pipes_alloc);
    /* 0 nodes for now... */
    self->nnodes = 0;

    /* Nullify poller */
    self->poller = zpoller_new (NULL);
    ASSERT_ALLOC(self->poller, err_poller_alloc);

    /* Setup new poller. It uses the low-level zmq_poll API */
    self->poller2 = NULL;

    /* Initilize mdp_worrker last, as we need to have everything ready
     * when we attemp to register in broker. Actually, we still need
     * to parse the SDB strucutres and register the operations in the
     * hash tables... * TODO (FIXME?): Find a better initialitazion routine before registering
     * to the broker the request from clients */
    self->name = strdup (name);
    ASSERT_ALLOC(self->name, err_name_alloc);
    self->endpoint_broker = strdup (endpoint_broker);
    ASSERT_ALLOC(self->endpoint_broker, err_endp_broker_alloc);
    self->verbose = verbose;

    /* Concatenate recv'ed name with a llio identifier */
    char *llio_name = zmalloc (sizeof (char)*(strlen(name)+strlen(LLIO_STR)+1));
    ASSERT_ALLOC(llio_name, err_llio_name_alloc);
    strcat (llio_name, name);
    strcat (llio_name, LLIO_STR);
    self->llio = llio_new (llio_name, endpoint_dev, type,
            verbose);
    ASSERT_ALLOC(self->llio, err_llio_alloc);

    /* We try to open the device */
    int err = llio_open (self->llio, NULL);
    ASSERT_TEST(err==0, "Error opening device!", err_llio_open);

    /* We can free llio_name now, as llio copies the string */
    free (llio_name);
    llio_name = NULL; /* Avoid double free error */

    /* Init sm_io_thsafe_server_ops_h. For now, we assume we want zmq
     * for exchanging messages between smio and devio instances */
    self->thsafe_server_ops = smio_thsafe_zmq_server_ops;

    /* Init sm_io_h hash */
    self->sm_io_h = zhash_new ();
    ASSERT_ALLOC(self->sm_io_h, err_sm_io_h_alloc);

    /* Init sm_io_thsafe_ops_h dispatch table */
    self->disp_table_thsafe_ops = disp_table_new ();
    ASSERT_ALLOC(self->disp_table_thsafe_ops, err_disp_table_thsafe_ops_alloc);

    halutils_err_e halutils_err = disp_table_insert_all (self->disp_table_thsafe_ops,
            self->thsafe_server_ops);
    ASSERT_TEST(halutils_err==HALUTILS_SUCCESS, "Could not initialize dispatch table",
            err_disp_table_init);

    /* Finally, initialize mdp_worker with service being the BPM<board_number> */
    /* self->worker = mdp_worker_new (endpoint_broker, name, verbose);
    ASSERT_ALLOC(self->worker, err_worker_alloc); */
    /* Finally, initialize out zeroMQ context */
    self->ctx = zctx_new ();
    ASSERT_ALLOC(self->ctx, err_ctx_alloc);

    /* Adjust linger time for Majordomo protocol (MDP) */
    /* A non-zero linger value is required for DISCONNECT to be sent
     * when the worker is destroyed.  100 is arbitrary but chosen to be
     * sufficient for common cases without significant delay in broken ones. */
    zctx_set_linger (self->ctx, 100);

    return self;

err_ctx_alloc:
err_disp_table_init:
    disp_table_destroy (&self->disp_table_thsafe_ops);
err_disp_table_thsafe_ops_alloc:
    zhash_destroy (&self->sm_io_h);
err_sm_io_h_alloc:
    llio_release (self->llio, NULL);
err_llio_open:
    llio_destroy (&self->llio);
err_llio_alloc:
    free (llio_name);
err_llio_name_alloc:
    free (self->endpoint_broker);
err_endp_broker_alloc:
    free (self->name);
err_name_alloc:
    zpoller_destroy (&self->poller);
err_poller_alloc:
    free (self->pipes);
err_pipes_alloc:
    free (self->log_file);
err_log_file:
    free (self);
err_self_alloc:
    return NULL;
}

/* Destroy an instance of the Device Information */
devio_err_e devio_destroy (devio_t **self_p)
{
    assert (self_p);

    if (*self_p) {
        devio_t *self = *self_p;

        /* Destroy children threads before proceeding */
        _devio_destroy_smio_all (self);

        /* Starting destructing by the last resource */
        /* Notice that we destroy the worker first, as to
         * unregister from broker as soon as possible to avoid
         * loosing requests from clients */
        zctx_destroy (&self->ctx);
        disp_table_destroy (&self->disp_table_thsafe_ops);
        zhash_destroy (&self->sm_io_h);
        self->thsafe_server_ops = NULL;
        llio_release (self->llio, NULL);
        llio_destroy (&self->llio);
        free (self->endpoint_broker);
        free (self->name);
        zpoller_destroy (&self->poller);
        free (self->poller2);
        free (self->pipes);
        free (self->log_file);
        free (self);
        *self_p = NULL;
    }

    return DEVIO_SUCCESS;
}

/* Read specific information about the device. Typically,
 * this is stored in the SDB structure inside the device */
devio_err_e devio_print_info (devio_t *self)
{
    (void) self;
    return DEVIO_ERR_FUNC_NOT_IMPL;
}

/* Register an specific sm_io modules to this device */
devio_err_e devio_register_sm (devio_t *self, uint32_t smio_id, uint32_t base,
        uint32_t inst_id)
{
    assert (self);

    /* Search the sm_io_mod_dsapatch table for the smio_id and,
     * if found, call the correspondent bootstrap code to initilize
     * the sm_io module */
    th_boot_args_t *th_args = NULL;
    th_config_args_t *th_config_args = NULL;
    void *config_pipe = NULL;
    char *key = NULL;
    uint32_t pipe_idx = 0;

    DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE,
            "[dev_io_core:register_sm] smio_mod_dispatch table size = %ld\n",
            ARRAY_SIZE(smio_mod_dispatch));

    /* For now, just do a simple linear search. We can afford this, as
     * we don't expect to insert new sm_io modules often */
    unsigned int i;
    for (i = 0; i < ARRAY_SIZE(smio_mod_dispatch); ++i) {
        if (smio_mod_dispatch[i].id != smio_id) {
            continue;
        }

        /* Found! Call bootstrap code and insert in
         * hash table */

        /* Stringify ID. We do it before spawning a new thread as
         * alloc can fail */
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE,
                "[dev_io_core:register_sm] Stringify hash ID\n");
        char *inst_id_str = halutils_stringify_dec_key (inst_id);
        ASSERT_ALLOC(inst_id_str, err_inst_id_str_alloc);
        char *key = halutils_concat_strings_no_sep (smio_mod_dispatch[i].name,
                inst_id_str);
        /* We don't need this anymore */
        free (inst_id_str);
        inst_id_str = NULL;
        ASSERT_ALLOC (key, err_key_alloc);

        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE,
                "[dev_io_core:register_sm] Allocating thread args\n");

        /* Alloacate thread arguments struct and pass it to the
         * thread. It is the responsability of the calling thread
         * to clear this structure after using it! */
        th_boot_args_t *th_args = zmalloc (sizeof *th_args);
        ASSERT_ALLOC (th_args, err_th_args_alloc);
        th_args->parent = self;
        /* FIXME: weak identifier */
        th_args->smio_id = i;
        th_args->broker = self->endpoint_broker;
        th_args->service = self->name;
        th_args->verbose = self->verbose;
        th_args->base = base;
        th_args->inst_id = inst_id;

        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE,
                "[dev_io_core:register_sm] Calling boot func\n");

        pipe_idx = self->nnodes++;
        self->pipes [pipe_idx] = zthread_fork (self->ctx, smio_startup,
                th_args);
        ASSERT_TEST (self->pipes [pipe_idx] != NULL, "Could not spawn SMIO thread",
                err_spawn_smio_thread);

        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE,
                "[dev_io_core:register_sm] Inserting hash with key: %s\n", key);
        int zerr = zhash_insert (self->sm_io_h, key, self->pipes [pipe_idx]);
        /* We must not fail here, as we will loose our reference to the SMIO
         * thread otherwise */
        ASSERT_TEST (zerr == 0, "Could not insert PIPE hash key. Duplicated value?",
                err_pipe_hash_insert);

        /* Configure default values of the recently created SMIO using the
         * bootstrap registered function config_defaults () */

        /* Now, we create a short lived thread just to configure our SMIO */
        /* Allocate config thread arguments struct and pass it to the
         * thread. It is the responsability of the calling thread
         * to clear this structure after using it! */
        th_config_args = zmalloc (sizeof *th_config_args);
        ASSERT_ALLOC (th_config_args, err_th_config_args_alloc);

        th_config_args->broker = self->endpoint_broker;
        /* FIXME: weak identifier */
        th_config_args->smio_id = i;
        th_config_args->service = self->name;
        th_config_args->log_file = self->log_file;
        th_config_args->inst_id = inst_id;

        config_pipe = zthread_fork (self->ctx, smio_config_defaults, th_config_args);
        ASSERT_TEST (config_pipe != NULL, "Could not spawn config thread",
                err_spawn_config_thread);

        /* stop on first match */
        break;
    }

    /* On success, just "key"" is not deallocated. All of the other
     * allocated parameters are either free'd or its ownership
     * is transfered to the calling function/thread */
    free (key);
    return DEVIO_SUCCESS;

err_spawn_config_thread:
    /* FIXME: Destroy SMIO thread as we could configure it? */
    free (th_config_args);
err_th_config_args_alloc:
    zhash_delete (self->sm_io_h, key);
err_pipe_hash_insert:
    /* If we can't insert the SMIO thread key in hash,
     * destroy it as we won't have a reference to it later! */
    _devio_send_destruct_msg (self, self->pipes [pipe_idx]);
err_spawn_smio_thread:
    free (th_args);
err_th_args_alloc:
    free (key);
err_key_alloc:
err_inst_id_str_alloc:
    return DEVIO_ERR_ALLOC;
}

/* Register all sm_io module that this device can handle,
 * according to the device information stored in the SDB */
devio_err_e devio_register_all_sm (devio_t *self)
{
    (void) self;
    return DEVIO_ERR_FUNC_NOT_IMPL;
}

devio_err_e devio_unregister_sm (devio_t *self, const char *smio_key)
{
    return _devio_destroy_smio (self, smio_key);
}

devio_err_e devio_unregister_all_sm (devio_t *self)
{
    return _devio_destroy_smio_all (self);
}

devio_err_e devio_init_poller_sm (devio_t *self)
{
    devio_err_e err = DEVIO_SUCCESS;
    DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE,
            "[dev_io_core:poll_all_sm] Calling init_poller_sm\n");

    /* Check minimum number of nodes */
    ASSERT_TEST(self->nnodes > 0, "There are no SMIOs registered!",
            err_no_nodes, DEVIO_ERR_NO_NODES);

    /* FIXME: From CZMQ sources: If you need a balanced poll, use
     * the low level zmq_poll method directly
     */
    unsigned int i;
    for (i = 0; i < self->nnodes; ++i) {
        int zerr = zpoller_add (self->poller, self->pipes[i]);
        ASSERT_TEST(zerr >= 0, "zmq: zpoller_add error!",
            err_zpoller_add, DEVIO_ERR_ALLOC);
    }

err_no_nodes:
err_zpoller_add:
    return err;
}

devio_err_e devio_init_poller2_sm (devio_t *self)
{
    devio_err_e err = DEVIO_SUCCESS;
    DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE,
            "[dev_io_core:poll_all_sm] Calling init_poller2_sm\n");

    /* Check minimum number of nodes */
    ASSERT_TEST(self->nnodes > 0, "There are no SMIOs registered!",
            err_no_nodes, DEVIO_ERR_NO_NODES);

    zmq_pollitem_t *items = zmalloc (sizeof (*items) * self->nnodes);
    ASSERT_ALLOC(items, err_alloc_items, DEVIO_ERR_ALLOC);

    unsigned int i;
    for (i = 0; i < self->nnodes; ++i) {
        items [i].socket = self->pipes [i];
        items [i].events = ZMQ_POLLIN;
    }

    /* This should be freed on dev_io exit */
    self->poller2 = items;

err_alloc_items:
err_no_nodes:
    return err;
}

devio_err_e devio_poll_all_sm (devio_t *self)
{
    devio_err_e err = DEVIO_SUCCESS;

    ASSERT_TEST(self->poller, "Unitialized poller!",
            err_uninitialized_poller, DEVIO_ERR_UNINIT_POLLER);

    /* Wait up to 100 ms */
    void *which = zpoller_wait (self->poller, DEVIO_POLLER_TIMEOUT);

    if (zpoller_expired (self->poller)) {
        /*DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE,
                "[dev_io_core:poll_all_sm] poller expired\n");*/
        goto err_poller_expired;
    }

    ASSERT_TEST(!zpoller_terminated (self->poller), "Poller terminated!",
            err_poller_terminated, DEVIO_ERR_TERMINATED);

    zmsg_t *recv_msg = zmsg_recv (which);
    /* Prepare the args structure */
    zmq_server_args_t server_args = {.msg = &recv_msg, .reply_to = which};
    err = _devio_do_smio_op (self, &server_args);

    /* Cleanup */
    zmsg_destroy (&recv_msg);

err_poller_expired:
err_poller_terminated:
err_uninitialized_poller:
    return err;
}

devio_err_e devio_poll2_all_sm (devio_t *self)
{
    devio_err_e err = DEVIO_SUCCESS;

    ASSERT_TEST(self->poller2, "Unitialized poller!",
            err_uninitialized_poller, DEVIO_ERR_UNINIT_POLLER);

    /* Wait up to 100 ms */
    int rc = zmq_poll (self->poller2, self->nnodes, DEVIO_POLLER_TIMEOUT);
    ASSERT_TEST(rc != -1, "devio_poll2_all_sm: poller interrupted", err_poller_interrupted,
            DEVIO_ERR_INTERRUPTED_POLLER);

    /* Timeout */
    if (rc == 0) {  /* Exit silently */
        /*DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE,
                "[dev_io_core:poll_all_sm] poller expired\n");*/
        goto err_poller_expired;
    }

    /* Loop once through all the available sockets */
    unsigned int i;
    for (i = 0; i < self->nnodes; ++i) {
        if (self->poller2 [i].revents & ZMQ_POLLIN) {
            zmsg_t *recv_msg = zmsg_recv (self->poller2 [i].socket);
            /* Prepare the args structure */
            zmq_server_args_t server_args = {
                .tag = ZMQ_SERVER_ARGS_TAG,
                .msg = &recv_msg,
                .reply_to = self->poller2 [i].socket};
            err = _devio_do_smio_op (self, &server_args);

            /* Cleanup */
            zmsg_destroy (&recv_msg);
        }
    }

err_poller_expired:
err_poller_interrupted:
err_uninitialized_poller:
    return err;
}

devio_err_e devio_do_smio_op (devio_t *self, void *msg)
{
    return _devio_do_smio_op (self, msg);
}

/**************** Helper Functions ***************/
static devio_err_e _devio_do_smio_op (devio_t *self, void *msg)
{
    assert (self);
    assert (msg);

    devio_err_e err = DEVIO_SUCCESS;

    disp_table_t *disp_table = self->disp_table_thsafe_ops;
    msg_err_e merr = msg_handle_sock_request (self, msg, disp_table);
    ASSERT_TEST (merr == MSG_SUCCESS, "Error handling request", err_hand_req,
           SMIO_ERR_MSG_NOT_SUPP /* returning a more meaningful error? */);

err_hand_req:
    return err;
}

static devio_err_e _devio_destroy_smio_all (devio_t *self)
{
    assert (self);

    devio_err_e err = DEVIO_SUCCESS;
    /* Get all hash keys */
    zlist_t *hash_keys = zhash_keys (self->sm_io_h);
    ASSERT_ALLOC (hash_keys, err_hash_keys_alloc, DEVIO_ERR_ALLOC);
    char *hash_item = zlist_first (hash_keys);

    /* Iterate over all keys removing each of one */
    for (; hash_item != NULL; hash_item = zlist_next (hash_keys)) {
        err = _devio_destroy_smio (self, hash_item);
        ASSERT_TEST (err == DEVIO_SUCCESS, "Could not destroy SMIO "
                "instance", err_smio_destroy, DEVIO_ERR_SMIO_DESTROY);
    }

err_smio_destroy:
    zlist_destroy (&hash_keys);
err_hash_keys_alloc:
    return err;
}

static devio_err_e _devio_send_destruct_msg (devio_t *self, void *pipe)
{
    assert (self);
    assert (pipe);

    devio_err_e err = DEVIO_SUCCESS;
    /* Send message to SMIO informing it to destroy itself */
    /* This cannot fail at this point... but it can */
    zmsg_t *send_msg = zmsg_new ();
    ASSERT_ALLOC (send_msg, err_msg_alloc, DEVIO_ERR_ALLOC);
    /* An empty message means to selfdestruct */
    zmsg_pushstr (send_msg, "");
    int zerr = zmsg_send (&send_msg, pipe);
    ASSERT_TEST (zerr == 0, "Could not send self-destruct message to SMIO instance",
            err_send_msg, DEVIO_ERR_SMIO_DESTROY);

    DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_INFO, "[dev_io_core] Self-destruct message "
            "to SMIO sent\n");

err_send_msg:
    zmsg_destroy (&send_msg);
err_msg_alloc:
    return err;
}

/* smio_key is the name of the SMIO + instance number, e.g.,
 * FMC130M_4CH0*/
static devio_err_e _devio_destroy_smio (devio_t *self, const char *smio_key)
{
    assert (self);

    devio_err_e err = DEVIO_SUCCESS;
    /* Lookup SMIO reference in hash table */
    void *pipe = zhash_lookup (self->sm_io_h, smio_key);
    ASSERT_TEST (pipe != NULL, "Could not find SMIO registered with this ID",
            err_hash_lookup, DEVIO_ERR_SMIO_DESTROY);

    err = _devio_send_destruct_msg (self, pipe);
    ASSERT_TEST (err == DEVIO_SUCCESS, "Could not send self-destruct message to "
            "PIPE", err_send_msg, DEVIO_ERR_SMIO_DESTROY);

    /* Finally, remove the pipe from hash. FIXME: What if the SMIO does not
     * exit? We will loose its reference ...*/
    zhash_delete (self->sm_io_h, smio_key);

err_send_msg:
err_hash_lookup:
    return err;
}

