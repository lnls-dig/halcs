/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "bpm_server.h"

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

/************************************************************/
/****************** SMIO Thread entry-point  ****************/
/************************************************************/
/* FIXME: Do some sanity check before calling functions from smio_mod_dispatch*/
void smio_startup (zsock_t *pipe, void *args)
{
    /* FIXME: priv pointer is unused for now! We should use it to differentiate
     * between multiple smio instances of the same type controlling multiple
     * modules of the same type */
    th_boot_args_t *th_args = (th_boot_args_t *) args;
    zsock_t *pipe_mgmt = pipe;
    zsock_t *pipe_msg = th_args->pipe_msg;
    volatile const smio_mod_dispatch_t *smio_mod_dispatch = th_args->smio_handler;
    /* Signal parent we are initializing */
    zsock_signal (pipe_mgmt, 0);

    /* We must export our service as the combination of the
     * devio name (coming from devio parent) and our own name ID
     * followed by an optional parameter coming from priv pointer */
    char *inst_id_str = hutils_stringify_dec_key (th_args->inst_id);
    ASSERT_ALLOC(inst_id_str, err_inst_id_str_alloc);
    char *smio_service = hutils_concat_strings3 (th_args->service,
            smio_mod_dispatch->name, inst_id_str, ':');
    ASSERT_ALLOC(smio_service, err_smio_service_alloc);

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO, "[sm_io_bootstrap] SMIO Thread %s "
            "starting ...\n", smio_service);
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO, "[sm_io_bootstrap] SMIO Thread %s "
            "allocating resources ...\n", smio_service);

    smio_t *self = smio_new (th_args, pipe_mgmt, pipe_msg, smio_service);
    ASSERT_ALLOC(self, err_self_alloc);

    /* Atach this SMIO instance to its parent */
    smio_err_e err = smio_attach (self, th_args->parent);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not attach SMIO", err_call_attach);

    /* Call SMIO init function to finish initializing its internal strucutres */
    err = SMIO_DISPATCH_FUNC_WRAPPER (init, smio_mod_dispatch);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not initialize SMIO", err_call_init);

    /* Export SMIO specific operations */
    const disp_op_t **smio_exp_ops = smio_get_exp_ops (self);
    ASSERT_TEST (smio_exp_ops != NULL, "Could not get SMIO exported operations",
            err_smio_get_exp_ops);

    err = smio_export_ops (self, smio_exp_ops);
    ASSERT_TEST (err == SMIO_SUCCESS, "Could not export specific SMIO operations",
            err_smio_export);

    /* Main loop request-action */
    err = smio_loop (self);
    ASSERT_TEST (err == SMIO_SUCCESS, "Could not loop the SMIO messages",
            err_smio_loop);

err_smio_loop:
    /* Unexport SMIO specific operations */
    smio_unexport_ops (self);
err_smio_export:
    /* Nullify exp ops */
    smio_set_exp_ops (self, NULL);
err_smio_get_exp_ops:
    /* FIXME: Poll PIPE sockets and on receiving any message calls shutdown () */
    SMIO_DISPATCH_FUNC_WRAPPER (shutdown, smio_mod_dispatch);
err_call_init:
    smio_deattach (self);
err_call_attach:
    /* Destroy what we did in _smio_new */
    smio_destroy (&self);
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
}

/************************************************************/
/*************** SMIO Config Thread entry-point  ************/
/************************************************************/

void smio_config_defaults (zsock_t *pipe, void *args)
{
    th_config_args_t *th_args = (th_config_args_t *) args;
    volatile const smio_mod_dispatch_t *smio_mod_dispatch = th_args->smio_handler;
    /* Signal parent we are initializing */
    zsock_signal (pipe, 0);

    /* We must export our service as the combination of the
     * devio name (coming from devio parent) and our own name ID
     * followed by an optional parameter coming from priv pointer */
    char *inst_id_str = hutils_stringify_dec_key (th_args->inst_id);
    ASSERT_ALLOC(inst_id_str, err_inst_id_str_alloc);
    char *smio_service = hutils_concat_strings3 (th_args->service,
            smio_mod_dispatch->name, inst_id_str, ':');
    ASSERT_ALLOC(smio_service, err_smio_service_alloc);

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO, "[sm_io_bootstrap] Config Thread %s "
            "starting ...\n", smio_service);
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO, "[sm_io_bootstrap] Config Thread %s "
            "allocating resources ...\n", smio_service);

    SMIO_DISPATCH_FUNC_WRAPPER_GEN(config_defaults, smio_mod_dispatch,
            th_args->broker, smio_service, th_args->log_file);

    /* We've finished configuring the SMIO. Tell DEVIO we are done */
    char *smio_service_suffix = hutils_concat_strings_no_sep (
            smio_mod_dispatch->name, inst_id_str);
    ASSERT_ALLOC(smio_service_suffix, err_smio_service_suffix_alloc);

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO, "[sm_io_bootstrap] Sending CONFIG DONE message over PIPE\n");
    int zerr = zstr_sendx (pipe, smio_service_suffix, "CONFIG DONE", NULL);
    ASSERT_TEST (zerr >= 0, "Config thread could not send CONFIG DONE message "
            "over PIPE. Destroying ourselves", err_send_config_done);
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO, "[sm_io_bootstrap] Config Thread %s "
            "sent CONFIG DONE over PIPE\n", smio_service);

    /* Wait for $TERM message from DEVIO to end */
    bool terminated = false;
    while (!terminated) {
        zmsg_t *msg = zmsg_recv (pipe);
        if (msg == NULL) {
            break; /* Interrupted */
        }

        char *command = zmsg_popstr (msg);
        if (streq (command, "$TERM")) {
            terminated = true;
        }
        /* Invalid message received. Log the error, but continue normally */
        else {
            DBE_DEBUG (DBG_SM_IO | DBG_LVL_WARN, "[sm_io_bootstrap] Config Thread %s "
                    "received an invalid command over PIPE\n", smio_service);
            goto err_pipe_mgmt_bad_msg;
        }

err_pipe_mgmt_bad_msg:
        free (command);
        zmsg_destroy (&msg);
    }

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO, "[sm_io_bootstrap] Config Thread %s "
            "terminating with %s\n", smio_service, (terminated)? "success" : "error");

err_smio_service_suffix_alloc:
    free (smio_service_suffix);
err_send_config_done:
    free (smio_service);
err_smio_service_alloc:
    free (inst_id_str);
err_inst_id_str_alloc:
    free (th_args);
}
