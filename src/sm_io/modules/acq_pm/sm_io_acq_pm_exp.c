/*
 * Copyright (C) 2017 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "halcs_server.h"
/* Private headers */
#include "sm_io_acq_pm_codes.h"
#include "sm_io_acq_pm_exports.h"
#include "sm_io_acq_pm_core.h"
#include "sm_io_acq_pm_exp.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, SM_IO, "[sm_io:acq_pm_exp]", \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_ALLOC(ptr, SM_IO, "[sm_io:acq_pm_exp]", \
            smio_err_str(SMIO_ERR_ALLOC),           \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                    \
    CHECK_HAL_ERR(err, SM_IO, "[sm_io:acq_pm_exp]",    \
            smio_err_str (err_type))

/************************************************************/
/***************** Specific ACQ_PM Operations ******************/
/************************************************************/

/* Exported function pointers */
const disp_table_func_fp acq_pm_exp_fp [] = {
    NULL
};

/************************************************************/
/***************** Export methods functions *****************/
/************************************************************/

static smio_err_e _acq_pm_do_op (void *owner, void *msg);

/* Attach an instance of sm_io to dev_io function pointer */
smio_err_e acq_pm_attach (smio_t *self, void *args)
{
    UNUSED(self);
    UNUSED(args);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Deattach an instance of sm_io to dev_io function pointer */
smio_err_e acq_pm_deattach (smio_t *self)
{
    UNUSED(self);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Export (register) sm_io to handle operations function pointer */
smio_err_e acq_pm_export_ops (smio_t *self,
        const disp_op_t** smio_exp_ops)
{
    UNUSED(self);
    UNUSED(smio_exp_ops);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Unexport (unregister) sm_io to handle operations function pointer */
smio_err_e acq_pm_unexport_ops (smio_t *self)
{
    UNUSED(self);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Generic wrapper for receiving opcodes and arguments to specific funtions function pointer */
/* FIXME: Code repetition! _devio_do_smio_op () function does almost the same!!! */
smio_err_e _acq_pm_do_op (void *owner, void *msg)
{
    UNUSED(owner);
    UNUSED(msg);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

smio_err_e acq_pm_do_op (void *self, void *msg)
{
    return _acq_pm_do_op (self, msg);
}

const smio_ops_t acq_pm_ops = {
    .attach             = &acq_pm_attach,          /* Attach sm_io instance to dev_io */
    .deattach           = &acq_pm_deattach,        /* Deattach sm_io instance to dev_io */
    .export_ops         = &acq_pm_export_ops,      /* Export sm_io operations to dev_io */
    .unexport_ops       = &acq_pm_unexport_ops,    /* Unexport sm_io operations to dev_io */
    .do_op              = &acq_pm_do_op            /* Generic wrapper for handling specific operations */
};

/************************************************************/
/****************** Bootstrap Operations ********************/
/************************************************************/

smio_err_e acq_pm_init (smio_t * self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq_pm_exp] Initializing acq_pm\n");

    smio_err_e err = SMIO_SUCCESS;

    err = smio_set_id (self, ACQ_PM_SDB_DEVID);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO id", err_set_id);
    err = smio_set_name (self, ACQ_PM_SDB_NAME);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO name", err_set_name);

    /* Set SMIO ops pointers */
    err = smio_set_ops (self, &acq_pm_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO operations",
            err_smio_set_ops);
    err = smio_set_thsafe_client_ops (self, &smio_thsafe_client_zmq_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO thsafe operations",
            err_smio_set_thsafe_ops);

    /* Fill the disp_op_t description structure with the callbacks. */

    /* disp_op_t structure is const and all of the functions performing on it
     * obviously receives a const argument, but here (and only on the SMIO
     * initialization) we need to make an exception if we want to keep the
     * functions' description and the function pointers separate */
    err = smio_init_exp_ops (self, (disp_op_t **) acq_pm_exp_ops, acq_pm_exp_fp);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not fill SMIO "
            "function descriptors with the callbacks", err_fill_desc);

    err = smio_set_exp_ops (self, acq_pm_exp_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO exported operations",
            err_smio_set_exp_ops);

    /* Initialize specific structure */
    smio_acq_pm_t *smio_handler = smio_acq_pm_new (self);
    ASSERT_ALLOC(smio_handler, err_smio_handler_alloc, SMIO_ERR_ALLOC);
    err = smio_set_handler (self, smio_handler);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO handler",
            err_smio_set_handler);

    return err;

err_smio_set_handler:
    smio_acq_pm_destroy (&smio_handler);
err_smio_handler_alloc:
    smio_set_exp_ops (self, NULL);
err_smio_set_exp_ops:
err_fill_desc:
    smio_set_thsafe_client_ops (self, NULL);
err_smio_set_thsafe_ops:
    smio_set_ops (self, NULL);
err_smio_set_ops:
err_set_name:
err_set_id:
    return err;
}

/* Destroy sm_io instance of acq_pm */
smio_err_e acq_pm_shutdown (smio_t *self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:acq_pm_exp] Shutting down acq_pm\n");

    smio_err_e err = SMIO_SUCCESS;
    smio_acq_pm_t *acq_pm = smio_get_handler (self);
    ASSERT_TEST(acq_pm != NULL, "Could not get ACQ_PM handler",
            err_acq_pm_handler, SMIO_ERR_ALLOC /* FIXME: improve return code */);

    /* Destroy SMIO instance */
    smio_acq_pm_destroy (&acq_pm);
    /* Nullify operation pointers */
    smio_set_exp_ops (self, NULL);
    smio_set_thsafe_client_ops (self, NULL);
    smio_set_ops (self, NULL);

err_acq_pm_handler:
    return err;
}

const smio_bootstrap_ops_t acq_pm_bootstrap_ops = {
    .init     = &acq_pm_init,
    .shutdown = &acq_pm_shutdown
};

SMIO_MOD_DECLARE(ACQ_PM_SDB_DEVID, ACQ_PM_SDB_NAME, acq_pm_bootstrap_ops)
