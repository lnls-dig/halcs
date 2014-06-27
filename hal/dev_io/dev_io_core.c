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
#include "smio_thsafe_zmq_server.h"
#include "sm_io_thsafe_codes.h"
#include "sm_io_bootstrap.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label)  \
    ASSERT_HAL_TEST(test_boolean, DEV_IO, "[dev_io_core]",  \
            err_str, err_goto_label)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label)                   \
    ASSERT_HAL_ALLOC(ptr, DEV_IO, "[dev_io_core]",          \
            devio_err_str(DEVIO_ERR_ALLOC),                 \
            err_goto_label)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                            \
    CHECK_HAL_ERR(err, DEV_IO, "[dev_io_core]",             \
            devio_err_str (err_type))

#define LLIO_STR ":LLIO\0"
#define DEVIO_POLLER_TIMEOUT 100        /* in msec */

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

/* Do the SMIO operation */
static devio_err_e _devio_do_smio_op (devio_t *self, void *msg);
static void _devio_destroy_smio (devio_t *self, uint32_t smio_id);
static void _devio_destroy_smio_all (devio_t *self);

/* Creates a new instance of Device Information */
devio_t * devio_new (char *name, char *endpoint_dev,
        llio_type_e type, char *endpoint_broker, int verbose)
{
    devio_t *self = (devio_t *) zmalloc (sizeof *self);
    ASSERT_ALLOC(self, err_self_alloc);

    /* Initialize the sockets structure to talk to nodes */
    self->pipes = zmalloc (sizeof (*self->pipes) * NODES_MAX_LEN);
    ASSERT_ALLOC(self->pipes, err_pipes_alloc);
    /* 0 nodes for now... */
    self->nnodes = 0;

    /* Nullify poller */
    self->poller = zpoller_new (NULL);
    ASSERT_ALLOC(self->poller, err_poller_alloc);

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
    self->thsafe_server_ops = &smio_thsafe_zmq_server_ops;

    /* Init sm_io_h hash */
    self->sm_io_h = zhash_new ();
    ASSERT_ALLOC(self->sm_io_h, err_sm_io_h_alloc);

    /* Init sm_io_thsafe_ops_h dispatch table */
    self->disp_table_thsafe_ops = disp_table_new ();
    ASSERT_ALLOC(self->disp_table_thsafe_ops, err_disp_table_thsafe_ops_alloc);

    disp_table_func_fp *thsafe_server_fp = (disp_table_func_fp *) (self->thsafe_server_ops);
    const uint32_t *thsafe_opcodes_p = thsafe_opcodes;
    halutils_err_e halutils_err = disp_table_insert_all (self->disp_table_thsafe_ops,
            thsafe_server_fp, thsafe_opcodes_p, THSAFE_OPCODE_END);
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
        free (self->pipes);
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
devio_err_e devio_register_sm (devio_t *self, uint32_t smio_id, void *priv)
{
    assert (self);

    /* Search the sm_io_mod_dsapatch table for the smio_id and,
     * if found, call the correspondent bootstrap code to initilize
     * the sm_io module */
    th_boot_args_t *th_args = NULL;

    /* For now, just do a simple linear search. We can afford this, as
     * we don't expect to insert new sm_io modules often */
    unsigned int i;

    DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE,
            "[dev_io_core:register_sm] smio_mod_dispatch table size = %ld\n",
            ARRAY_SIZE(smio_mod_dispatch));

    for (i = 0; i < ARRAY_SIZE(smio_mod_dispatch); ++i) {
        if (smio_mod_dispatch[i].id == smio_id) {
            /* Found! Call bootstrap code and insert in
             * hash table */
            /* FIXME: Why do I need this? smio always gets initilized
             * after smio_mod_dispatch[i].bootstrap_ops->smio_boot (self); */
            /* smio_t *smio = NULL; */
            /* It is expected tha after the boot () call the operations
             * this sm_io inscate can handle are already registered! */
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
            th_args->priv = priv;

            DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE,
                    "[dev_io_core:register_sm] Calling boot func\n");

            uint32_t pipe_idx = self->nnodes++;
            self->pipes [pipe_idx] = zthread_fork (self->ctx, smio_startup,
                    th_args);
            /* self->pipes [pipe_idx] = zthread_fork (self->ctx,
                   smio_mod_dispatch[i].bootstrap_ops->thread_boot, th_args); */
            /*smio = smio_mod_dispatch[i].bootstrap_ops->boot (self);*/
            /*ASSERT_ALLOC (smio, err_smio_alloc); */

            /* Stringify ID */
            DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE,
                    "[dev_io_core:register_sm] Stringify hash ID\n");
            char *key = halutils_stringify_key (smio_mod_dispatch[i].id);
            ASSERT_ALLOC (key, err_key_alloc);

            DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE,
                    "[dev_io_core:register_sm] Inserting hash with key: %s\n", key);
            zhash_insert (self->sm_io_h, key, self->pipes [pipe_idx]);
            free (key);

            /* stop on first match */
            break;
        }
    }

    //free (th_args);
    return DEVIO_SUCCESS;

err_key_alloc:
    free (th_args);
err_th_args_alloc:
    return DEVIO_ERR_ALLOC;
}

/* Register all sm_io module that this device can handle,
 * according to the device information stored in the SDB */
devio_err_e devio_register_all_sm (devio_t *self)
{
    (void) self;
    return DEVIO_ERR_FUNC_NOT_IMPL;
}

devio_err_e devio_unregister_sm (devio_t *self, uint32_t smio_id)
{
    (void) self;
    (void) smio_id;
    return DEVIO_ERR_FUNC_NOT_IMPL;

}

devio_err_e devio_unregister_all_sm (devio_t *self)
{
    (void) self;
    return DEVIO_ERR_FUNC_NOT_IMPL;
}

devio_err_e devio_init_poller_sm (devio_t *self)
{
    devio_err_e err = DEVIO_SUCCESS;
    DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE,
            "[dev_io_core:poll_all_sm] Calling init_poller_sm\n");

    /*  Set-up poller */
    if (self->nnodes == 0) {
        err = DEVIO_ERR_NO_NODES;
        goto err_no_nodes;
    }

    /* FIXME: From CZMQ sources: If you need a balanced poll, use
     * the low level zmq_poll method directly
     */
    unsigned int i;
    for (i = 0; i < self->nnodes; ++i) {
        int zerr = zpoller_add (self->poller, self->pipes[i]);
        if (zerr < 0) {
            err = DEVIO_ERR_ALLOC;
            break;
        }
    }

err_no_nodes:
    return err;
}

devio_err_e devio_poll_all_sm (devio_t *self)
{
    devio_err_e err = DEVIO_SUCCESS;

    if (!self->poller) {
        err = DEVIO_ERR_UNINIT_POLLER;
        goto err_uninitialized_poller;
    }

    /* Wait up to 100 ms */
    void *which = zpoller_wait (self->poller, DEVIO_POLLER_TIMEOUT);

    if (zpoller_expired (self->poller)) {
        /*DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE,
                "[dev_io_core:poll_all_sm] poller expired\n");*/
        goto err_poller_expired;
    }

    if (zpoller_terminated (self->poller)) {
        /*DBE_DEBUG (DBG_DEV_IO | DBG_LVL_TRACE,
                "[dev_io_core:poll_all_sm] poller terminated\n");*/
        err = DEVIO_ERR_TERMINATED;
        goto err_poller_terminated;
    }

    zmsg_t *recv_msg = zmsg_recv (which);
    /* Prepare the args structure */
    zmq_server_args_t server_args = {.msg = &recv_msg, .reply_to = which};
    err = _devio_do_smio_op (self, &server_args);

err_poller_expired:
err_poller_terminated:
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
    zmq_server_args_t *server_args = (zmq_server_args_t *) msg;
    /* Message is:
     * frame 0: opcode
     * frame 1: payload */
    /* Extract the first frame and determine the opcode */
    zframe_t *opcode = zmsg_pop (*server_args->msg);
    devio_err_e err = (opcode == NULL) ? DEVIO_ERR_BAD_MSG : DEVIO_SUCCESS;
    ASSERT_TEST(opcode != NULL, "Could not receive opcode", err_null_opcode);

    if (zframe_size (opcode) != THSAFE_OPCODE_SIZE) {
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_ERR,
                "[dev_io_core:poll_all_sm] Invalid opcode size received\n");
        err = DEVIO_ERR_BAD_MSG;
        goto err_wrong_opcode_size;
    }

    uint32_t opcode_data = *(uint32_t *) zframe_data (opcode);
    if (opcode_data > THSAFE_OPCODE_END-1) {
        DBE_DEBUG (DBG_DEV_IO | DBG_LVL_ERR,
                "[dev_io_core:poll_all_sm] Invalid opcode received\n");
        err = DEVIO_ERR_BAD_MSG;
        goto err_invalid_opcode;
    }

    /* Do the actual work... */
    disp_table_call (self->disp_table_thsafe_ops, opcode_data, self, server_args);

err_invalid_opcode:
err_wrong_opcode_size:
    zframe_destroy (&opcode);
err_null_opcode:
    return err;
}

static void _devio_destroy_smio_all (devio_t *self)
{
#if 0
    unsigned i;
    for (i = 0; i < self->nnodes; ++i) {
        /* This cannot fail at this point... but it can */
        zmsg_t *msg = zmsg_new ();
        /* An empty message means to selfdestruct */
        zmsg_pushstr (msg, "");
        zmsg_send (&msg, self->pipes [i]);
    }
#endif
    /* Get all hash keys */
    zlist_t *hash_keys = zhash_keys (self->sm_io_h);
    ASSERT_ALLOC (hash_keys, err_hash_keys_alloc);
    char *hash_item = zlist_first (hash_keys);

    /* Iterate over all keys removing each of one */
    for (; hash_item != NULL; hash_item = zlist_next (hash_keys)) {
        /* FIXME: Usage of stroul fucntion for reconverting the string
         * into a uint32_t */
        _devio_destroy_smio (self, (uint32_t) strtoul (hash_item,
                    (char **) NULL, 16));
    }

    zlist_destroy (&hash_keys);

err_hash_keys_alloc:
    return;
}

static void _devio_destroy_smio (devio_t *self, uint32_t smio_id)
{
    assert (self);

    /* Stringify ID */
    char *key_c = halutils_stringify_key (smio_id);
    ASSERT_ALLOC (key_c, err_key_alloc);

    /* Lookup SMIO reference in hash table */
    void *pipe = zhash_lookup (self->sm_io_h, key_c);
    ASSERT_TEST (pipe != NULL, "Could not find SMIO registered with this ID",
            err_hash_lookup);

    /* Send message to SMIO informing it to destroy itself */
    /* This cannot fail at this point... but it can */
    zmsg_t *send_msg = zmsg_new ();
    ASSERT_ALLOC (send_msg, err_msg_alloc);
    /* An empty message means to selfdestruct */
    zmsg_pushstr (send_msg, "");
    int zerr = zmsg_send (&send_msg, pipe);
    ASSERT_TEST (zerr == 0, "Could not send self-destruct message to SMIO instance",
            err_send_msg);

    /* Finally, remove the pipe from hash */
    zhash_delete (self->sm_io_h, key_c);

err_send_msg:
    zmsg_destroy (&send_msg);
err_msg_alloc:
err_hash_lookup:
    free (key_c);
err_key_alloc:
    return;
}

/********* Low-level generic methods API *********/

#define CHECK_FUNC(func_p)                              \
    do {                                                \
        if(func_p == NULL) {                            \
            DBE_DEBUG (DBG_DEV_IO | DBG_LVL_ERR,        \
                    "[sm_io] %s\n",                     \
                    devio_err_str (DEVIO_ERR_FUNC_NOT_IMPL)); \
            return NULL;                                \
        }                                               \
    } while(0)

/* Declare wrapper for all DEVIO functions API */
#define DEVIO_FUNC_WRAPPER(func_name, ...)               \
{                                                        \
    assert (owner);                                      \
    CHECK_FUNC (((devio_t *)owner)->thsafe_server_ops->func_name);\
        return ((devio_t *)owner)->thsafe_server_ops->func_name (owner, ##__VA_ARGS__);  \
}

/**** Open device ****/
void *smio_thsafe_server_open (void *owner, void *args)
    DEVIO_FUNC_WRAPPER (thsafe_server_open, args)

/**** Release device ****/
void *smio_thsafe_server_release (void *owner, void *args)
    DEVIO_FUNC_WRAPPER (thsafe_server_release, args)

/**** Read data from device ****/
void *smio_thsafe_server_read_16 (void *owner, void *args)
    DEVIO_FUNC_WRAPPER (thsafe_server_read_16, args)
void *smio_thsafe_server_read_32 (void *owner, void *args)
    DEVIO_FUNC_WRAPPER (thsafe_server_read_32, args)
void *smio_thsafe_server_read_64 (void *owner, void *args)
    DEVIO_FUNC_WRAPPER (thsafe_server_read_64, args)

/**** Write data to device ****/
void *smio_thsafe_server_write_16 (void *owner, void *args)
    DEVIO_FUNC_WRAPPER (thsafe_server_write_16, args)
void *smio_thsafe_server_write_32 (void *owner, void *args)
    DEVIO_FUNC_WRAPPER (thsafe_server_write_32, args)
void *smio_thsafe_server_write_64 (void *owner, void *args)
    DEVIO_FUNC_WRAPPER (thsafe_server_write_64, args)

/**** Read data block from device function pointer, size in bytes ****/
void *smio_thsafe_server_read_block (void *owner, void *args)
    DEVIO_FUNC_WRAPPER (thsafe_server_read_block, args)

/**** Write data block from device function pointer, size in bytes ****/
void *smio_thsafe_server_write_block (void *owner, void *args)
    DEVIO_FUNC_WRAPPER (thsafe_server_write_block, args)

/**** Read data block via DMA from device, size in bytes ****/
void *smio_thsafe_server_read_dma (void *owner, void *args)
    DEVIO_FUNC_WRAPPER (thsafe_server_read_dma, args)

/**** Write data block via DMA from device, size in bytes ****/
void *smio_thsafe_server_write_dma (void *owner, void *args)
    DEVIO_FUNC_WRAPPER (thsafe_server_write_dma, args)

/**** Read device information function pointer ****/
/* int smio_thsafe_server_read_info (void *owner, void *args)
    DEVIO_FUNC_WRAPPER (thsafe_server_read_info, args) Moved to dev_io */
