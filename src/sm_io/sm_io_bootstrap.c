/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include <stdio.h>

#include "sm_io_bootstrap.h"
#include "sm_io.h"
#include "exp_ops_codes.h"
#include "errhand.h"
#include "hutils.h"
#include "disp_table.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, SM_IO, "[sm_io_bootstrap]",         \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_ALLOC(ptr, SM_IO, "[sm_io_bootstrap]",       \
            smio_err_str(SMIO_ERR_ALLOC),                   \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                            \
    CHECK_HAL_ERR(err, SM_IO, "[sm_io_bootstrap]",          \
            smio_err_str (err_type))

#define SMIO_POLLER_TIMEOUT         100        /* in msec */
/* ':'. We have parent <devio_service>:<smio_serivce>. So, we need 1
 * more byte allocated */
#define EXTRA_SMIO_SERV_BYTES       1

/* SMIO dispatch table operations */
const disp_table_ops_t smio_disp_table_ops;

/* Dispatch table message check handler */
static disp_table_err_e _smio_check_msg_args (disp_table_t *disp_table,
        const disp_op_t *disp_op, void *args);

static struct _smio_t *_smio_new (th_boot_args_t *args, zsock_t *pipe,
        char *service);
static smio_err_e _smio_destroy (struct _smio_t **self_p);
static smio_err_e _smio_loop (smio_t *self);

/************************************************************/
/****************** SMIO Thread entry-point  ****************/
/************************************************************/
/* FIXME: Do some sanity check before calling functions from smio_mod_dispatch*/
void smio_startup (zsock_t *pipe, void *args)
{
    /* FIXME: priv pointer is unused for now! We should use it to differentiate
     * between multiple smio instances of the same type controlling multiple
     * modules of the same type. Otherwise, we would ended up with two workers
     * for the same thing (see Majordomo protocol) */
    th_boot_args_t *th_args = (th_boot_args_t *) args;
    /* Signal parent we are initializing */
    zsock_signal (pipe, 0);

    /* We must export our service as the combination of the
     * devio name (coming from devio parent) and our own name ID
     * followed by an optional parameter coming from priv pointer */
    char *inst_id_str = hutils_stringify_dec_key (th_args->inst_id);
    ASSERT_ALLOC(inst_id_str, err_inst_id_str_alloc);
    char *smio_service = hutils_concat_strings3 (th_args->service,
            smio_mod_dispatch[th_args->smio_id].name, inst_id_str, ':');
    ASSERT_ALLOC(smio_service, err_smio_service_alloc);

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO, "[sm_io_bootstrap] SMIO Thread %s "
            "starting ...\n", smio_service);
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO, "[sm_io_bootstrap] SMIO Thread %s "
            "allocating resources ...\n", smio_service);

    smio_t *self = _smio_new (th_args, pipe, smio_service);
    ASSERT_ALLOC(self, err_self_alloc);

    /* Call SMIO init function to finish initializing its internal strucutres */
    smio_err_e err = SMIO_DISPATCH_FUNC_WRAPPER (init);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not initialize SMIO", err_call_init);
    /* Atach this SMIO instance to its parent */
    err = smio_attach (self, th_args->parent);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not attach SMIO", err_call_attach);

    /* Export SMIO specific operations */
    err = smio_export_ops (self, self->exp_ops);
    ASSERT_TEST (err == SMIO_SUCCESS, "Could not export specific SMIO operations",
            err_smio_export);

    /* Main loop request-action */
    err = _smio_loop (self);
    ASSERT_TEST (err == SMIO_SUCCESS, "Could not loop the SMIO messages",
            err_smio_loop);

err_smio_loop:
    /* Unexport SMIO specific operations */
    smio_unexport_ops (self);
err_smio_export:
    /* Deattach this SMIO instance to its parent */
    smio_deattach (self);
err_call_attach:
    /* FIXME: Poll PIPE sockets and on receiving any message calls shutdown () */
    SMIO_DISPATCH_FUNC_WRAPPER (shutdown);
err_call_init:
    /* Destroy what we did in _smio_new */
    _smio_destroy (&self);
err_self_alloc:
    /* We can't output this message at a later time as we depend on the smio_service
     * variable. This is not so bad, though, as most of the time we will not fail
     * in hutils_concat_strings () function */
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_WARN, "[sm_io_bootstrap] SMIO Thread %s exiting\n",
            smio_service);
    free (smio_service);
err_smio_service_alloc:
    free (inst_id_str);
err_inst_id_str_alloc:
    free (th_args);
    return;
}

/************************************************************/
/*************** SMIO Config Thread entry-point  ************/
/************************************************************/
void *smio_config_defaults (void *args)
{
    th_config_args_t *th_args = (th_config_args_t *) args;

    /* We must export our service as the combination of the
     * devio name (coming from devio parent) and our own name ID
     * followed by an optional parameter coming from priv pointer */
    char *inst_id_str = hutils_stringify_dec_key (th_args->inst_id);
    ASSERT_ALLOC(inst_id_str, err_inst_id_str_alloc);
    char *smio_service = hutils_concat_strings3 (th_args->service,
            smio_mod_dispatch[th_args->smio_id].name, inst_id_str, ':');
    ASSERT_ALLOC(smio_service, err_smio_service_alloc);

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO, "[sm_io_bootstrap] Config Thread %s "
            "starting ...\n", smio_service);
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO, "[sm_io_bootstrap] Config Thread %s "
            "allocating resources ...\n", smio_service);

    SMIO_DISPATCH_FUNC_WRAPPER_GEN(config_defaults, th_args->broker,
            smio_service, th_args->log_file);

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO, "[sm_io_bootstrap] Config Thread %s "
            "exiting\n", smio_service);
    free (smio_service);
err_smio_service_alloc:
    free (inst_id_str);
err_inst_id_str_alloc:
    free (th_args);
    return NULL;
}

/************************************************************/
/************ SMIO Bootstrap wrapper functions **************/
/************************************************************/

struct _smio_t *smio_new (th_boot_args_t* args, zsock_t *pipe,
        char *service)
{
    return _smio_new (args, pipe, service);
}

smio_err_e smio_destroy (struct _smio_t **self_p)
{
    return _smio_destroy (self_p);
}

smio_err_e smio_loop (struct _smio_t *self)
{
    return _smio_loop (self);
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
/****************** Local helper functions ******************/
/************************************************************/

/* Boot new sm_io instance of fmc130m_4ch */
static struct _smio_t *_smio_new (th_boot_args_t *args, zsock_t *pipe,
        char *service)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io_bootstrap] Initializing SMIO\n");
    smio_t *self = (smio_t *) zmalloc (sizeof *self);
    ASSERT_ALLOC(self, err_self_alloc);

    /* Setup exported service name */
    self->service = strdup (service);
    ASSERT_ALLOC(self->service, err_service_alloc);

    /* Setup Dispatch table */
    self->exp_ops_dtable = disp_table_new (&smio_disp_table_ops);
    ASSERT_ALLOC(self->exp_ops_dtable, err_exp_ops_dtable_alloc);

    self->smio_handler = NULL;      /* This is set by the device functions */
    self->pipe = pipe;
    self->inst_id = args->inst_id;

    /* Initialize SMIO base address */
    self->base = args->base;

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io_bootstrap] Creating worker\n");
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "\tbroker = %s, service = %s, verbose = %d\n",
            args->broker, service, args->verbose);
    /* self->worker = mdp_worker_new (self->ctx, args->broker, service, args->verbose); */
    self->worker = NULL;
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io_bootstrap] Worker created\n");
    ASSERT_ALLOC(self->worker, err_worker_alloc);

    return self;

err_worker_alloc:
    disp_table_destroy (&self->exp_ops_dtable);
err_exp_ops_dtable_alloc:
    free (self->service);
err_service_alloc:
    free (self);
err_self_alloc:
    return NULL;
}

static smio_err_e _smio_destroy (struct _smio_t **self_p)
{
    assert (self_p);

    if (*self_p) {
        struct _smio_t *self = *self_p;

        mdp_worker_destroy (&self->worker);
        disp_table_destroy (&self->exp_ops_dtable);
        self->thsafe_client_ops = NULL;
        self->ops = NULL;
        self->parent = NULL;
        free (self->service);

        free (self);
        *self_p = NULL;
    }

    return SMIO_SUCCESS;
}

/* FIXME: Poll on PIPE socket as well and in case of any arriving message
 * destroy itself */
static smio_err_e _smio_loop (smio_t *self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE,
            "[sm_io_bootstrap] Main loop starting\n");

    smio_err_e err = SMIO_SUCCESS;
    bool terminated = false;
    /* Begin infinite polling on Majordomo/PIPE socket
     * and exit if the parent send a message through
     * the pipe socket */
    while (!terminated) {
        /* Listen to WORKER (requests from clients) and PIPE (managment) sockets */
        zmq_pollitem_t items [] = {
            [SMIO_PIPE_SOCK] = {
                .socket = self->pipe,
                .fd = 0,
                .events = ZMQ_POLLIN,
                .revents = 0
            }
        };

        /* Check for activity on WORKER socket */
        zframe_t *reply_to = NULL;
        zmsg_t *request = mdp_worker_recv (self->worker, &reply_to, true);

        if (request != NULL) {
            exp_msg_zmq_t smio_args = {
                .tag = EXP_MSG_ZMQ_TAG,
                .msg = &request,
                .reply_to = reply_to};
            err = smio_do_op (self, &smio_args);

            /* What can I do in case of error ?*/
            if (err != SMIO_SUCCESS) {
                DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE,
                        "[sm_io_bootstrap] smio_do_op: %s\n",
                        smio_err_str (err));
            }

            /* Cleanup */
            zframe_destroy (&reply_to);
            zmsg_destroy (&request);
        }

        /* Wait up to 100 ms */
        int rc = zmq_poll (items, SMIO_SOCKS_NUM, SMIO_POLLER_TIMEOUT);
        ASSERT_TEST(rc != -1, "Poller has been interrupted",
                err_loop_interrupted, SMIO_ERR_INTERRUPTED_POLLER);

        /* Check for activity on PIPE socket */
        if (items [SMIO_PIPE_SOCK].revents & ZMQ_POLLIN) {
            /* On any activity we destroy ourselves */
            zmsg_t *request = zmsg_recv (self->pipe);

            if (request == NULL) {
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
            else {
                DBE_DEBUG (DBG_SM_IO | DBG_LVL_ERR,
                        "[sm_io_bootstrap]  Invalid message received on PIPE socket.\n"
                        "This was probably supposed to go through another socket\n");
            }

            free (command);
            zmsg_destroy (&request);
        }
    }

err_loop_interrupted:
    return err;
}

