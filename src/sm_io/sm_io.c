/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include "bpm_server.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, SM_IO, "[sm_io]",         \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_ALLOC(ptr, SM_IO, "[sm_io]",                 \
            smio_err_str(SMIO_ERR_ALLOC),                   \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                            \
    CHECK_HAL_ERR(err, SM_IO, "[sm_io]",                    \
            smio_err_str (err_type))

#define SMIO_POLLER_TIMEOUT         100        /* in msec */

/* Main class object that every sm_io must implement */
struct _smio_t {
    uint32_t id;                        /* Unique identifier for this sm_io type. This must be
                                           the same from the SDB ID */
    uint32_t inst_id;                   /* Instance ID. This differentiate SMIOs
                                           of the same type */
    uint64_t base;                      /* Base SMIO address */
    char *name;                         /* Identification of this sm_io instance */
    char *service;                      /* Exported service name */
    /* int verbose; */                  /* Print activity to stdout */
    mlm_client_t *worker;               /* zeroMQ Malamute client (worker) */
    devio_t *parent;                    /* Pointer back to parent dev_io */
    void *smio_handler;                 /* Generic pointer to a device handler. This
                                            must be cast to a specific type by the
                                            devices functions */
    zsock_t *pipe_mgmt;                 /* Pipe back to parent to exchange Management messages */
    zsock_t *pipe_msg;                  /* Pipe back to parent to exchange Payload messages */

    /* Specific SMIO operations dispatch table for exported operations */
    disp_table_t *exp_ops_dtable;
    /* Specific SMIO instance functions. This will be exported on SMIO startup */
    const disp_op_t **exp_ops;
    /* Minimum set of methods that every sm_io instance must implement.
         * This is thought as the interface to the world */
    const smio_ops_t *ops;
    /* These operations must be used to access LLIO methods, as they are
     * thread safe. The method's signatures are almost the same as the
     * ones available in llio, changing the llio_t self pointer to a void
     * pointer (socket to parent thread) */
    const smio_thsafe_client_ops_t *thsafe_client_ops;
};

/* SMIO dispatch table operations */
const disp_table_ops_t smio_disp_table_ops;

static smio_err_e _smio_do_op (void *owner, void *msg);
/* Dispatch table message check handler */
static disp_table_err_e _smio_check_msg_args (disp_table_t *disp_table,
        const disp_op_t *disp_op, void *args);
static smio_err_e _smio_set_name (smio_t *self, const char *name);
static const char *_smio_get_name (smio_t *self);
static char *_smio_clone_name (smio_t *self);

/* Boot new SMIO instance. Better used as a thread (CZMQ actor) init function */
smio_t *smio_new (th_boot_args_t *args, zsock_t *pipe_mgmt,
        zsock_t *pipe_msg, char *service)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io_bootstrap] Initializing SMIO\n");
    smio_t *self = (smio_t *) zmalloc (sizeof *self);
    ASSERT_ALLOC(self, err_self_alloc);

    /* Will be filled by the SMIO service init function */
    self->name = NULL;

    /* Setup exported service name */
    self->service = strdup (service);
    ASSERT_ALLOC(self->service, err_service_alloc);

    /* Setup Dispatch table */
    self->exp_ops_dtable = disp_table_new (&smio_disp_table_ops);
    ASSERT_ALLOC(self->exp_ops_dtable, err_exp_ops_dtable_alloc);

    self->smio_handler = NULL;      /* This is set by the device functions */
    self->pipe_mgmt = pipe_mgmt;
    self->pipe_msg = pipe_msg;
    self->inst_id = args->inst_id;

    /* Initialize SMIO base address */
    self->base = args->base;

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io_bootstrap] Creating worker\n");
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "\tbroker = %s, service = %s, verbose = %d\n",
            args->broker, service, args->verbose);
    self->worker = mlm_client_new ();
    ASSERT_ALLOC(self->worker, err_worker_alloc);
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io_bootstrap] Worker created\n");

    int rc = mlm_client_connect (self->worker, args->broker, 1000, service);
    ASSERT_TEST(rc >= 0, "Could not connect MLM to broker", err_mlm_connect);

    return self;

err_mlm_connect:
    mlm_client_destroy (&self->worker);
err_worker_alloc:
    zsock_destroy (&self->pipe_msg);
    disp_table_destroy (&self->exp_ops_dtable);
err_exp_ops_dtable_alloc:
    free (self->service);
err_service_alloc:
    free (self);
err_self_alloc:
    return NULL;
}

smio_err_e smio_destroy (struct _smio_t **self_p)
{
    assert (self_p);

    if (*self_p) {
        struct _smio_t *self = *self_p;

        mlm_client_destroy (&self->worker);
        zsock_destroy (&self->pipe_msg);
        disp_table_destroy (&self->exp_ops_dtable);
        self->thsafe_client_ops = NULL;
        self->ops = NULL;
        self->parent = NULL;
        free (self->service);
        free (self->name);

        free (self);
        *self_p = NULL;
    }

    return SMIO_SUCCESS;
}

smio_err_e smio_loop (smio_t *self)
{
    assert (self);

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE,
            "[sm_io_bootstrap] Main loop starting\n");

    smio_err_e err = SMIO_SUCCESS;

    /* We send/recv messages through MLM msgpipe */
    zsock_t *worker_msgpipe = mlm_client_msgpipe (self->worker);
    ASSERT_TEST (worker_msgpipe != NULL, "Invalid WORKER socket reference",
            err_inv_worker_socket, SMIO_ERR_INV_SOCKET);
    zpoller_t *poller = zpoller_new (worker_msgpipe, self->pipe_mgmt, NULL);
    ASSERT_TEST (poller != NULL, "Could not Initialize poller",
            err_init_poller, SMIO_ERR_INV_SOCKET);

    /* Begin infinite polling on Malamute/PIPE socket
     * and exit if the parent send a message through
     * the pipe socket */
    bool terminated = false;
    while (!zsys_interrupted && !terminated) {
        /* Poll Message sockets */
        zsock_t *which = zpoller_wait (poller, SMIO_POLLER_TIMEOUT);
        ASSERT_TEST(which != NULL || zpoller_expired (poller),
                "_smio_loop: poller interrupted", err_poller_interrupted,
                SMIO_ERR_INTERRUPTED_POLLER);

            /* Check for activity on WORKER socket */
        if (which == worker_msgpipe) {
            zmsg_t *request = mlm_client_recv (self->worker);

            if (request == NULL) {
                err = SMIO_ERR_INTERRUPTED_POLLER;
                break;                          /* Worker has been interrupted */
            }

            exp_msg_zmq_t smio_args = {
                .tag = EXP_MSG_ZMQ_TAG,
                .msg = &request,
                .reply_to = NULL /* Unused field in MLM protocol */
            };
            err = smio_do_op (self, &smio_args);

            /* What can I do in case of error ?*/
            if (err != SMIO_SUCCESS) {
                DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE,
                        "[sm_io_bootstrap] smio_do_op: %s\n",
                        smio_err_str (err));
            }

            /* Cleanup */
            zmsg_destroy (&request);
        }
        /* Check for activity on PIPE socket */
        else if (which == self->pipe_mgmt) {
            zmsg_t *request = zmsg_recv (self->pipe_mgmt);

            if (request == NULL) {
                err = SMIO_ERR_INTERRUPTED_POLLER;
                break;                          /* Worker has been interrupted */
            }

            char *command = zmsg_popstr (request);
            /* A $TERM message on this means to self-destruct */
            if (streq (command, "$TERM")) {
                /* Destroy SMIO instance. As we already do this on the main
                 * smio_startup (), we just need to exit this cleanly */
                DBE_DEBUG (DBG_SM_IO | DBG_LVL_WARN,
                        "[sm_io_bootstrap] Received shutdown message on "
                        "PIPE socket. Exiting.\n");
                terminated = true;
            }
            /* Invalid message received. Log the error, but continue normally */
            else {
                DBE_DEBUG (DBG_SM_IO | DBG_LVL_WARN,
                        "[sm_io_bootstrap]  Invalid message received on PIPE "
                        "socket.\n");
                err = SMIO_ERR_BAD_MSG;
                goto err_pipe_mgmt_bad_msg;
            }

err_pipe_mgmt_bad_msg:
            free (command);
            zmsg_destroy (&request);
        }
    }

err_poller_interrupted:
err_init_poller:
err_inv_worker_socket:
    return err;
}

/************************************************************/
/***************** Dispatch table callbacks *****************/
/************************************************************/

static disp_table_err_e _smio_check_msg_args (disp_table_t *disp_table,
        const disp_op_t *disp_op, void *args)
{
    assert (disp_table);
    assert (disp_op);
    assert (args);

    devio_err_e err = SMIO_SUCCESS;

    /* Check if the message tis the correct one */
    ASSERT_TEST (msg_guess_type (args) == MSG_EXP_ZMQ, "Invalid message tag",
            err_inv_msg, SMIO_ERR_MSG_NOT_SUPP);
    msg_err_e merr = msg_check_gen_zmq_args (disp_op, EXP_MSG_ZMQ(args));
    ASSERT_TEST (merr == MSG_SUCCESS, "Unrecognized message. Message arguments "
            "checking failed", err_msg_args_check, SMIO_ERR_MSG_NOT_SUPP);

err_msg_args_check:
err_inv_msg:
    return err;
}

const disp_table_ops_t smio_disp_table_ops = {
    .check_msg_args = _smio_check_msg_args
};

/************************************************************/
/**************** SMIO Ops wrapper functions ****************/
/************************************************************/

/* Attach an instance of sm_io to dev_io function pointer */
smio_err_e smio_attach (smio_t *self, struct _devio_t *parent)
{
    assert (self);
    smio_err_e err = SMIO_SUCCESS;
    self->parent = parent;

    err = SMIO_FUNC_OPS_NOFAIL_WRAPPER(err, attach, parent);
    ASSERT_TEST(err == SMIO_SUCCESS, "Registered SMIO \"attach\" function error",
        err_func);

err_func:
    return err;
}

/* Deattach an instance of sm_io to dev_io function pointer */
smio_err_e smio_deattach (smio_t *self)
{
    assert (self);
    smio_err_e err = SMIO_SUCCESS;
    self->parent = NULL;

    err = SMIO_FUNC_OPS_NOFAIL_WRAPPER(err, deattach);
    ASSERT_TEST(err == SMIO_SUCCESS, "Registered SMIO \"deattach\" function error",
        err_func);

err_func:
    return err;
}

/* Export (Register) sm_io to handle specific operations */
smio_err_e smio_export_ops (smio_t *self, const disp_op_t** smio_exp_ops)
{
    assert (self);
    assert (smio_exp_ops);

    smio_err_e err = SMIO_SUCCESS;

    hutils_err_e herr = disp_table_insert_all (self->exp_ops_dtable, smio_exp_ops);
    ASSERT_TEST(herr == HUTILS_SUCCESS, "smio_export_ops: Could not export"
            " SMIO ops", err_export_op, SMIO_ERR_EXPORT_OP);

    err = SMIO_FUNC_OPS_NOFAIL_WRAPPER(err, export_ops, smio_exp_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Registered SMIO \"export_ops\" function error",
        err_func);

err_func:
err_export_op:
    return err;
}

/* Unexport (unregister) sm_io to handle specific operations */
smio_err_e smio_unexport_ops (smio_t *self)
{
    assert (self);

    smio_err_e err = SMIO_SUCCESS;
    hutils_err_e herr = disp_table_remove_all (self->exp_ops_dtable);

    ASSERT_TEST(herr == HUTILS_SUCCESS, "smio_export_ops: Could not unexport SMIO ops",
            err_unexport_op, SMIO_ERR_EXPORT_OP);

    err = SMIO_FUNC_OPS_NOFAIL_WRAPPER(err, unexport_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Registered SMIO \"unexport_ops\" function error",
        err_func);

err_func:
err_unexport_op:
    return err;
}

smio_err_e smio_do_op (void *owner, void *msg)
{
    return _smio_do_op (owner, msg);
}

smio_err_e smio_init_exp_ops (smio_t *self, disp_op_t** smio_exp_ops,
        const disp_table_func_fp *func_fps)
{
    assert(self);

    hutils_err_e herr = HUTILS_SUCCESS;
    smio_err_e err = SMIO_SUCCESS;
    herr = disp_table_fill_desc (self->exp_ops_dtable, smio_exp_ops, func_fps);
    ASSERT_TEST(herr == HUTILS_SUCCESS, "smio_export_ops: Could not export"
            " fill SMIO ops description", err_fill_desc_ops, SMIO_ERR_EXPORT_OP);

err_fill_desc_ops:
    return err;
}

/************************************************************/
/********************* Accessor methods *********************/
/************************************************************/

smio_err_e smio_set_base (smio_t *self, uint64_t base)
{
    assert (self);

    smio_err_e err = SMIO_SUCCESS;
    self->base = base;

    return err;
}

uint64_t smio_get_base (smio_t *self)
{
    assert (self);
    return self->base;
}

smio_err_e smio_set_id (smio_t *self, uint32_t id)
{
    assert (self);

    smio_err_e err = SMIO_SUCCESS;
    self->id = id;

    return err;
}

uint32_t smio_get_id (smio_t *self)
{
    assert (self);
    return self->id;
}

smio_err_e smio_set_inst_id (smio_t *self, uint32_t inst_id)
{
    assert (self);

    smio_err_e err = SMIO_SUCCESS;
    self->inst_id = inst_id;

    return err;
}

uint32_t smio_get_inst_id (smio_t *self)
{
    assert (self);
    return self->inst_id;
}

smio_err_e smio_set_name (smio_t *self, const char *name)
{
    assert (self);
    return _smio_set_name (self, name);
}

/* Get SMIO name */
const char *smio_get_name (smio_t *self)
{
    assert (self);
    return _smio_get_name (self);
}

/* Clone SMIO name */
char *smio_clone_name (smio_t *self)
{
    assert (self);
    return _smio_clone_name (self);
}

smio_err_e smio_set_exp_ops (smio_t *self, const disp_op_t **exp_ops)
{
    assert (self);
    smio_err_e err = SMIO_SUCCESS;

    self->exp_ops = exp_ops;

    return err;
}

const disp_op_t **smio_get_exp_ops (smio_t *self)
{
    assert (self);
    return self->exp_ops;
}

smio_err_e smio_set_ops (smio_t *self, const smio_ops_t *ops)
{
    assert (self);
    smio_err_e err = SMIO_SUCCESS;

    self->ops = ops;

    return err;
}

const smio_ops_t *smio_get_ops (smio_t *self)
{
    assert (self);
    return self->ops;
}

smio_err_e smio_set_thsafe_client_ops (smio_t *self,
        const smio_thsafe_client_ops_t *thsafe_client_ops)
{
    assert (self);
    smio_err_e err = SMIO_SUCCESS;

    self->thsafe_client_ops = thsafe_client_ops;

    return err;
}

const smio_thsafe_client_ops_t *smio_get_thsafe_client_ops (smio_t *self)
{
    assert (self);
    return self->thsafe_client_ops;
}

/**************** Static Functions ***************/

static smio_err_e _smio_do_op (void *owner, void *msg)
{
    assert (owner);
    assert (msg);

    SMIO_OWNER_TYPE *self = SMIO_EXP_OWNER(owner);
    smio_err_e err = SMIO_SUCCESS;

    /* TODO: The SMIO do_op must not modify the packet! We could pass a copy of the
     * message to it, but we this is in the critical path! Evaluate the impact
     * of doing this */
    SMIO_FUNC_OPS_NOFAIL_WRAPPER(err, do_op, msg);
    ASSERT_TEST (err == SMIO_SUCCESS, "Registered SMIO \"do_op\" function error",
            err_do_op);

    disp_table_t *disp_table = self->exp_ops_dtable;
    msg_err_e merr = msg_handle_mlm_request (owner, msg, disp_table);
    ASSERT_TEST (merr == MSG_SUCCESS, "Error handling request", err_hand_req,
           SMIO_ERR_MSG_NOT_SUPP /* returning a more meaningful error? */);

err_hand_req:
err_do_op:
    return err;
}

/* Set SMIO name */
static smio_err_e _smio_set_name (smio_t *self, const char *name)
{
    assert (self);
    smio_err_e err = SMIO_SUCCESS;

    if (name) {
        if (self->name) {
            free (self->name);
        }
        self->name = strdup (name);
        ASSERT_ALLOC(self->name, err_name_alloc, SMIO_ERR_ALLOC);
    }

err_name_alloc:
    return err;
}

/* Get SMIO name */
static const char *_smio_get_name (smio_t *self)
{
    assert (self);
    return self->name;
}

/* Clone SMIO name */
static char *_smio_clone_name (smio_t *self)
{
    assert (self);

    char *name = strdup (self->name);
    ASSERT_ALLOC(name, err_alloc);

err_alloc:
    return name;
}

smio_err_e smio_set_handler (smio_t *self, void *smio_handler)
{
    self->smio_handler = smio_handler;
    return SMIO_SUCCESS;
}

void *smio_get_handler (smio_t *self)
{
    return self->smio_handler;
}

mlm_client_t *smio_get_worker (smio_t *self)
{
    return self->worker;
}

zsock_t *smio_get_pipe_msg (smio_t *self)
{
    return self->pipe_msg;
}

zsock_t *smio_get_pipe_mgmt (smio_t *self)
{
    return self->pipe_mgmt;
}

/************************************************************/
/************* SMIO thsafe wrapper functions   **************/
/************************************************************/

#define CHECK_FUNC(func_p)                                  \
    do {                                                    \
        if(func_p == NULL) {                                \
            DBE_DEBUG (DBG_SM_IO | DBG_LVL_ERR,             \
                    "[sm_io] %s\n",                         \
                    smio_err_str (SMIO_ERR_FUNC_NOT_IMPL)); \
            return -SMIO_ERR_FUNC_NOT_IMPL;                 \
        }                                                   \
    } while(0)

#define ASSERT_FUNC(func_name)                              \
    do {                                                    \
        assert (self);                                      \
        assert (self->thsafe_client_ops);                   \
        CHECK_FUNC (self->thsafe_client_ops->func_name);    \
    } while(0)

/* Declare wrapper for all SMIO functions API */
#define SMIO_FUNC_WRAPPER(func_name, ...)                   \
{                                                           \
    ASSERT_FUNC(func_name);                                 \
    return self->thsafe_client_ops->func_name (self, ##__VA_ARGS__);  \
}

/**** Open device ****/
int smio_thsafe_client_open (smio_t *self, llio_endpoint_t *endpoint)
    SMIO_FUNC_WRAPPER (thsafe_client_open, endpoint)

/**** Release device ****/
int smio_thsafe_client_release (smio_t *self, llio_endpoint_t *endpoint)
    SMIO_FUNC_WRAPPER (thsafe_client_release, endpoint)

/**** Read data from device ****/
ssize_t smio_thsafe_client_read_16 (smio_t *self, uint64_t offs, uint16_t *data)
{
    ASSERT_FUNC(thsafe_client_read_16);
    return self->thsafe_client_ops->thsafe_client_read_16 (self, self->base | offs, data);
}

ssize_t smio_thsafe_client_read_32 (smio_t *self, uint64_t offs, uint32_t *data)
{
    ASSERT_FUNC(thsafe_client_read_32);
    return self->thsafe_client_ops->thsafe_client_read_32 (self, self->base | offs, data);
}

ssize_t smio_thsafe_client_read_64 (smio_t *self, uint64_t offs, uint64_t *data)
{
    ASSERT_FUNC(thsafe_client_read_64);
    return self->thsafe_client_ops->thsafe_client_read_64 (self, self->base | offs, data);
}

ssize_t smio_thsafe_raw_client_read_16 (smio_t *self, uint64_t offs, uint16_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_read_16, offs, data)
ssize_t smio_thsafe_raw_client_read_32 (smio_t *self, uint64_t offs, uint32_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_read_32, offs, data)
ssize_t smio_thsafe_raw_client_read_64 (smio_t *self, uint64_t offs, uint64_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_read_64, offs, data)

/**** Write data to device ****/
ssize_t smio_thsafe_client_write_16 (smio_t *self, uint64_t offs, const uint16_t *data)
{
    ASSERT_FUNC(thsafe_client_write_16);
    return self->thsafe_client_ops->thsafe_client_write_16 (self, self->base | offs, data);
}

ssize_t smio_thsafe_client_write_32 (smio_t *self, uint64_t offs, const uint32_t *data)
{
    ASSERT_FUNC(thsafe_client_write_32);
    return self->thsafe_client_ops->thsafe_client_write_32 (self, self->base | offs, data);
}

ssize_t smio_thsafe_client_write_64 (smio_t *self, uint64_t offs, const uint64_t *data)
{
    ASSERT_FUNC(thsafe_client_write_64);
    return self->thsafe_client_ops->thsafe_client_write_64 (self, self->base | offs, data);
}

ssize_t smio_thsafe_raw_client_write_16 (smio_t *self, uint64_t offs, const uint16_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_write_16, offs, data)
ssize_t smio_thsafe_raw_client_write_32 (smio_t *self, uint64_t offs, const uint32_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_write_32, offs, data)
ssize_t smio_thsafe_raw_client_write_64 (smio_t *self, uint64_t offs, const uint64_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_write_64, offs, data)

/**** Read data block from device function pointer, size in bytes ****/
ssize_t smio_thsafe_client_read_block (smio_t *self, uint64_t offs, size_t size,
        uint32_t *data)
{
    ASSERT_FUNC(thsafe_client_read_block);
    return self->thsafe_client_ops->thsafe_client_read_block (self, self->base | offs,
            size, data);
}

ssize_t smio_thsafe_raw_client_read_block (smio_t *self, uint64_t offs, size_t size, uint32_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_read_block, offs, size, data)

/**** Write data block from device function pointer, size in bytes ****/
ssize_t smio_thsafe_client_write_block (smio_t *self, uint64_t offs, size_t size,
        const uint32_t *data)
{
    ASSERT_FUNC(thsafe_client_write_block);
    return self->thsafe_client_ops->thsafe_client_write_block (self, self->base | offs,
            size, data);
}

ssize_t smio_thsafe_raw_client_write_block (smio_t *self, uint64_t offs, size_t size, const uint32_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_write_block, offs, size, data)

/**** Read data block via DMA from device, size in bytes ****/
ssize_t smio_thsafe_client_read_dma (smio_t *self, uint64_t offs, size_t size,
        uint32_t *data)
{
    ASSERT_FUNC(thsafe_client_read_dma);
    return self->thsafe_client_ops->thsafe_client_read_dma (self, self->base | offs,
            size, data);
}

ssize_t smio_thsafe_raw_client_read_dma (smio_t *self, uint64_t offs, size_t size, uint32_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_read_dma, offs, size, data)

/**** Write data block via DMA from device, size in bytes ****/
ssize_t smio_thsafe_client_write_dma (smio_t *self, uint64_t offs, size_t size,
        const uint32_t *data)
{
    ASSERT_FUNC(thsafe_client_write_dma);
    return self->thsafe_client_ops->thsafe_client_write_dma (self, self->base | offs,
            size, data);
}

ssize_t smio_thsafe_raw_client_write_dma (smio_t *self, uint64_t offs, size_t size, const uint32_t *data)
    SMIO_FUNC_WRAPPER (thsafe_client_write_dma, offs, size, data)

/**** Read device information function pointer ****/
/* int smio_thsafe_raw_client_read_info (smio_t *self, llio_dev_info_t *dev_info)
    SMIO_FUNC_WRAPPER (thsafe_client_read_info, dev_info) Moved to dev_io */

