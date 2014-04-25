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
#include "hal_assert.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label)  \
    ASSERT_HAL_TEST(test_boolean, SM_IO, "[sm_io_bootstrap]",         \
            err_str, err_goto_label)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label)                   \
    ASSERT_HAL_ALLOC(ptr, SM_IO, "[sm_io_bootstrap]",       \
            smio_err_str(SMIO_ERR_ALLOC),                   \
            err_goto_label)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                            \
    CHECK_HAL_ERR(err, SM_IO, "[sm_io_bootstrap]",          \
            smio_err_str (err_type))

#define SMIO_FUNC_OPS_NOFAIL_WRAPPER(err, func_name, ...)   \
    do {                                                    \
        if (self->ops && self->ops->func_name) {            \
            smio_err_e local_err = self->ops->func_name (self, ##__VA_ARGS__);  \
            err = (local_err != SMIO_ERR_FUNC_NOT_IMPL) ?   \
                local_err : err;                            \
        }                                                   \
    } while (0)

#define SMIO_DISPATCH_FUNC_WRAPPER(func_name, ...)          \
    do {                                                    \
        if (smio_mod_dispatch[th_args->smio_id].bootstrap_ops && \
                smio_mod_dispatch[th_args->smio_id].bootstrap_ops->func_name) { \
            smio_mod_dispatch[th_args->smio_id].bootstrap_ops->func_name (self, ##__VA_ARGS__);  \
        }                                                   \
    } while (0)

#define SMIO_POLLER_TIMEOUT         100        /* in msec */
/* ':'. We have parent <devio_service>:<smio_serivce>. So, we need 1
 * more byte allocated */
#define EXTRA_SMIO_SERV_BYTES       1

static struct _smio_t *_smio_new (struct _devio_t *parent, struct _zctx_t *ctx,
        void *pipe, char *broker, char *service, int verbose);
static smio_err_e _smio_destroy (struct _smio_t **self_p);
static smio_err_e _smio_loop (smio_t *self);

/************************************************************/
/****************** SMIO Thread entry-point  ****************/
/************************************************************/
/* FIXME: Do some sanity check before calling functions from smio_mod_dispatch*/
void smio_startup (void *args, zctx_t *ctx, void *pipe)
{
    /* FIXME: priv pointer is unused for now! We should use it to differentiate
     * between multiple smio instances of the same type controlling multiple
     * modules of the same type. Otherwise, we would ended up with two workers
     * for the same thing (see Majordomo protocol) */
    th_boot_args_t *th_args = (th_boot_args_t *) args;

    /* Initialize serice name concatenating the received name with
     * the smio service name */
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io_bootstrap] Thread starting\n");
    /* We must export our service as the combination of the
     * devio name (coming from devio parent) and our own name ID
     * followed by an optional parameter coming from priv pointer */
    char *smio_service = zmalloc (strlen(th_args->service)+
            strlen(smio_mod_dispatch[th_args->smio_id].name)+
            EXTRA_SMIO_SERV_BYTES+1);
    ASSERT_ALLOC(smio_service, err_smio_service_alloc);

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io_bootstrap] Thread allocating resources\n");
    sprintf (smio_service, "%s:%s", th_args->service, smio_mod_dispatch[th_args->smio_id].name);
    smio_t *self = _smio_new (th_args->parent, ctx, pipe, th_args->broker,
            smio_service, th_args->verbose);
    ASSERT_ALLOC(self, err_self_alloc);

    /* We don't need this anymore */
    free (smio_service);
    smio_service = NULL;

    /* Call SMIO init function to finish initializing its internal strucutres */
    SMIO_DISPATCH_FUNC_WRAPPER (init);
    /* Atach this SMIO instance to its parent */
    smio_attach (self, th_args->parent);

    /* Export SMIO specific operations */
    smio_err_e err = smio_export_ops (self, self->exp_ops);
    ASSERT_TEST (err == SMIO_SUCCESS, "Could not export specific SMIO operations",
            err_smio_export);

    /* Main loop request-action */
    _smio_loop (self);

    /* Unexport SMIO specific operations */
    smio_unexport_ops (self);
    /* Deattach this SMIO instance to its parent */
    smio_deattach (self);
    /* FIXME: Poll PIPE sockets and on receiving any message calls shutdown () */
    SMIO_DISPATCH_FUNC_WRAPPER (shutdown);

err_smio_export:
    /* FIXME: check for return error */
    /* Destroy what we did in _smio_new */
    _smio_destroy (&self);
err_self_alloc:
    free (smio_service);
err_smio_service_alloc:
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_ERR, "[sm_io_bootstrap] Thread exiting\n");
    free (args);
    return;
}

/************************************************************/
/************ SMIO Bootstrap wrapper functions **************/
/************************************************************/

struct _smio_t *smio_new (struct _devio_t *parent, struct _zctx_t *ctx, void *pipe,
        char *broker, char *service, int verbose)
{
    return _smio_new (parent, ctx, pipe, broker, service, verbose);
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
/****************** Local helper functions ******************/
/************************************************************/

/* Boot new sm_io instance of fmc130m_4ch */
static struct _smio_t *_smio_new (struct _devio_t *parent, struct _zctx_t *ctx, void *pipe,
        char *broker, char *service, int verbose)
{
    (void) parent;

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io_bootstrap] Initializing SMIO\n");
    smio_t *self = (smio_t *) zmalloc (sizeof *self);
    ASSERT_ALLOC(self, err_self_alloc);

    /* Setup Dispatch table */
    self->exp_ops_dtable = disp_table_new ();
    ASSERT_ALLOC(self->exp_ops_dtable, err_exp_ops_dtable_alloc);

    self->smio_handler = NULL;      /* This is set by the device functions */
    self->ctx = ctx;
    self->pipe = pipe;

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io_bootstrap] Creating worker\n");
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "\tbroker = %s, service = %s, verbose = %d\n",
            broker, service, verbose);
    self->worker = mdp_worker_new (self->ctx, broker, service, verbose);
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io_bootstrap] Worker created\n");
    ASSERT_ALLOC(self->worker, err_worker_alloc);

    return self;

err_worker_alloc:
    disp_table_destroy (&self->exp_ops_dtable);
err_exp_ops_dtable_alloc:
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
    /* Begin infinite polling on Majordomo socket
     * and exit if the parent send a message through
     * the pipe socket */
    while (!zctx_interrupted) {
        zframe_t *reply_to = NULL;
        zmsg_t *request = mdp_worker_recv (self->worker, &reply_to);

        if (request == NULL)
            break;                          /* Worker has been interrupted */

        exp_msg_zmq_t smio_args = {.msg = &request, .reply_to = reply_to};
        err = smio_do_op (self, &smio_args);

        /* What can I do in case of error ?*/
        if (err != SMIO_SUCCESS) {
            DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE,
                    "[sm_io_bootstrap] smio_do_op: %s\n",
                    smio_err_str (err));
        }
    }

    return err;
}

