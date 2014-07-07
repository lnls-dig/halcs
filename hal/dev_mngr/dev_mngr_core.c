/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include <string.h>

#include "dev_mngr_core.h"
#include "hal_assert.h"

/* Some global variables for use with signal handlers */
extern volatile sig_atomic_t __dev_nums;

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...)  \
    ASSERT_HAL_TEST(test_boolean, DEV_MNGR, "dev_mngr_core",\
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_ALLOC(ptr, DEV_MNGR, "dev_mngr_core",        \
            dmngr_err_str(DMNGR_ERR_ALLOC),                 \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                            \
    CHECK_HAL_ERR(err, DEV_MNGR, "dev_mngr_core",           \
            dmngr_err_str (err_type))

/* Creates a new instance of the Device Manager */
dmngr_t * dmngr_new (char *name, char *endpoint, int verbose)
{
    assert (name);
    assert (endpoint);

    dmngr_t *self = (dmngr_t *) zmalloc (sizeof *self);
    ASSERT_ALLOC(self, err_self_alloc);

    /* Initialize the Device Manager */
    self->ctx = zctx_new ();
    ASSERT_ALLOC(self->ctx, err_ctx_alloc);

    /* Create Dealer for use with zbeacon */
    self->dealer = zsocket_new (self->ctx, ZMQ_DEALER);
    ASSERT_ALLOC(self->dealer, err_dealer_alloc);
    zsocket_bind (self->dealer, "%s", endpoint);

    self->name = strdup (name);
    ASSERT_ALLOC(self->name, err_name_alloc);
    self->endpoint = strdup (endpoint);
    ASSERT_ALLOC(self->endpoint, err_endpoint_alloc);
    self->verbose = verbose;

    self->ops = (dmngr_ops_t *) zmalloc (sizeof *self->ops);
    ASSERT_ALLOC(self->ops, err_ops_alloc);
    self->ops->sig_ops = zlist_new ();
    ASSERT_ALLOC(self->ops->sig_ops, err_list_alloc);

    self->broker_running = false;

    return self;

err_list_alloc:
    free (self->ops);
err_ops_alloc:
    free (self->endpoint);
err_endpoint_alloc:
    free (self->name);
err_name_alloc:
    zsocket_destroy (self->ctx, self->dealer);
err_dealer_alloc:
    zctx_destroy (&self->ctx);
err_ctx_alloc:
    free (self);
err_self_alloc:
    return NULL;
}

/* Destroy an instance of the Device Manager */
dmngr_err_e dmngr_destroy (dmngr_t **self_p)
{
    assert (self_p);

    if (*self_p) {
        dmngr_t *self = *self_p;

        /* Starting destructing by the last resource */
        zlist_destroy (&self->ops->sig_ops);
        free (self->ops);
        free (self->endpoint);
        free (self->name);
        zsocket_destroy (self->ctx, self->dealer);
        zctx_destroy (&self->ctx);

        free (self);
        *self_p = NULL;
    }

    return DMNGR_SUCCESS;
}

dmngr_err_e dmngr_set_sig_handler (dmngr_t *self, dmngr_sig_handler_t *sig_handler)
{
    assert (self);
    int err = zlist_append (self->ops->sig_ops, sig_handler);

    return (err == -1) ? DMNGR_ERR_ALLOC : DMNGR_SUCCESS;
}

dmngr_err_e dmngr_register_sig_handlers (dmngr_t *self)
{
    assert (self);
    dmngr_sig_handler_t *sig_handler =
        (dmngr_sig_handler_t *) zlist_first (self->ops->sig_ops);

    /* Register all signal handlers in list*/
    while (sig_handler) {
        struct sigaction act;

        memset (&act, 0, sizeof(act));
        act.sa_sigaction = sig_handler->dmngr_sig_h;
        act.sa_flags = SA_SIGINFO;

        int err = sigaction (sig_handler->signal, &act, NULL);
        CHECK_ERR(err, DMNGR_ERR_SIGACTION);

        DBE_DEBUG (DBG_DEV_MNGR | DBG_LVL_INFO, "[dev_mngr_core] registered signal %d\n",
                sig_handler->signal);

        sig_handler = (dmngr_sig_handler_t *)
            zlist_next (self->ops->sig_ops);
    }

    return DMNGR_SUCCESS;
}

/* Declare wrapper for all DMNGR functions API */
#define DMNGR_FUNC_WRAPPER(func_name, ops_err, ...)     \
{                                                       \
    assert (self);                                      \
    CHECK_ERR(((self->ops->func_name == NULL) ? -1 : 0),\
        DMNGR_ERR_FUNC_NOT_IMPL);                       \
    int err = self->ops->func_name (##__VA_ARGS__);     \
    CHECK_ERR (err, ops_err);                           \
    return DMNGR_SUCCESS;                               \
}

dmngr_err_e dmngr_set_wait_clhd_handler (dmngr_t *self, wait_chld_handler_fp fp)
{
    assert (self);
    self->ops->dmngr_wait_chld = fp;

    return DMNGR_SUCCESS;
}

dmngr_err_e dmngr_wait_chld (dmngr_t *self)
    DMNGR_FUNC_WRAPPER(dmngr_wait_chld, DMNGR_ERR_WAITCHLD)

/*{
    assert (self);
    int err = self->ops->dmngr_wait_chld ();
    CHECK_ERR (err, DMNGR_ERR_WAITCHLD);

    return DMNGR_SUCCESS;
}*/

/* FIXME: Use the above wrapper!!!! */
dmngr_err_e dmngr_set_spawn_clhd_handler (dmngr_t *self, spawn_chld_handler_fp fp)
{
    assert (self);
    self->ops->dmngr_spawn_chld = fp;

    return DMNGR_SUCCESS;
}

dmngr_err_e dmngr_spawn_chld (dmngr_t *self, const char *program, char *const argv[])
{
    assert (self);
    int err = self->ops->dmngr_spawn_chld (program, argv);
    CHECK_ERR (err, DMNGR_ERR_SPAWNCHLD);

    return DMNGR_SUCCESS;
}

/* Execute function to spawn a broker process */
dmngr_err_e dmngr_spawn_broker (dmngr_t *self, const char *program, char *const argv[])
{
    assert (self);
    if (self->broker_running) {
        return DMNGR_ERR_BROK_RUNN;
    }

    int err = self->ops->dmngr_spawn_chld (program, argv);
    CHECK_ERR (err, DMNGR_ERR_SPAWNCHLD);

    return DMNGR_SUCCESS;
}

dmngr_err_e dmngr_set_ops (dmngr_t *self, dmngr_ops_t *dmngr_ops)
{
    assert (self);
    assert (dmngr_ops);

    self->ops = dmngr_ops;

    return DMNGR_SUCCESS;
}

bool dmngr_is_broker_running (dmngr_t *self)
{
    assert (self);
    return self->broker_running;
}

