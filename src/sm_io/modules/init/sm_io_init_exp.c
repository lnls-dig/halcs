/*
 * Copyright (C) 2017 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "halcs_server.h"
/* Private headers */
#include "sm_io_init_codes.h"
#include "sm_io_init_exports.h"
#include "sm_io_init_core.h"
#include "sm_io_init_exp.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, SM_IO, "[sm_io:init_exp]",  \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_ALLOC(ptr, SM_IO, "[sm_io:init_exp]",          \
            smio_err_str(SMIO_ERR_ALLOC),                     \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                              \
    CHECK_HAL_ERR(err, SM_IO, "[sm_io:init_exp]",             \
            smio_err_str (err_type))

/************************************************************/
/*****************  Specific INIT Operations *****************/
/************************************************************/

static int _init_check (void *owner, void *args, void *ret)
{
    assert (owner);
    assert (args);
    int err = -INIT_OK;

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:init] "
            "Calling _init_check\n");
    SMIO_OWNER_TYPE *self = SMIO_EXP_OWNER(owner);
    smio_init_t *init = smio_get_handler (self);
    ASSERT_TEST(init != NULL, "Could not get SMIO INIT handler",
            err_get_init_handler, -INIT_ERR);

    /* Message is:
     * frame 0: operation code
     * frame 1: rw
     * frame 2: arg (unused) 
     */
    uint32_t rw = *(uint32_t *) EXP_MSG_ZMQ_FIRST_ARG(args);
    uint32_t arg = *(uint32_t *) EXP_MSG_ZMQ_NEXT_ARG(args);
    UNUSED(rw);
    UNUSED(arg);

    /* Return value to caller */
    uint32_t arg_ret = 0x1;
    *((uint32_t *) ret) = arg_ret;
    err = sizeof (arg_ret);
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:init] "
            "Init = 0x%08X\n", arg_ret);

    return err;

err_get_init_handler:
    return err;
}

static int _init_get_board_type (void *owner, void *args, void *ret)
{
    assert (owner);
    assert (args);
    int err = -INIT_OK;

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:init] "
            "Calling _init_get_board_check\n");
    SMIO_OWNER_TYPE *self = SMIO_EXP_OWNER(owner);
    smio_init_t *init = smio_get_handler (self);
    ASSERT_TEST(init != NULL, "Could not get SMIO INIT handler",
            err_get_init_handler, -INIT_ERR);

    /* Message is:
     * frame 0: operation code
     * frame 1: rw
     * frame 2: arg (unused) 
     */
    uint32_t rw = *(uint32_t *) EXP_MSG_ZMQ_FIRST_ARG(args);
    uint32_t arg = *(uint32_t *) EXP_MSG_ZMQ_NEXT_ARG(args);
    UNUSED(rw);
    UNUSED(arg);

    if (rw) {
        int count = snprintf ((char *)ret, INIT_BOARD_TYPE_SIZE, "%s", init->board_type);
        /* Only when the number of characters written is less than the whole buffer,
         * it is guaranteed that the string was written successfully */
	if (count <= 0 || (size_t) count >= INIT_BOARD_TYPE_SIZE) {
            DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:init] "
                "Could not write full board name into \"ret\" = %s\n", init->board_type);
        }
        err = (count <= 0)? -INIT_ERR : count;
    }

    return err;

err_get_init_handler:
    return err;
}

/* Exported function pointers */
const disp_table_func_fp init_exp_fp [] = {
    _init_check,
    _init_get_board_type,
    NULL
};

/************************************************************/
/***************** Export methods functions *****************/
/************************************************************/

static smio_err_e _init_do_op (void *owner, void *msg);

/* Attach an instance of sm_io to dev_io function pointer */
smio_err_e init_attach (smio_t *self, void *args)
{
    UNUSED(self);
    UNUSED(args);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Deattach an instance of sm_io to dev_io function pointer */
smio_err_e init_deattach (smio_t *self)
{
    UNUSED(self);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Export (register) sm_io to handle operations function pointer */
smio_err_e init_export_ops (smio_t *self,
        const disp_op_t **smio_exp_ops)
{
    UNUSED(self);
    UNUSED(smio_exp_ops);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Unexport (unregister) sm_io to handle operations function pointer */
smio_err_e init_unexport_ops (smio_t *self)
{
    UNUSED(self);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Generic wrapper for receiving opcodes and arguments to specific funtions function pointer */
/* FIXME: Code repetition! _devio_do_smio_op () function does almost the same!!! */
smio_err_e _init_do_op (void *owner, void *msg)
{
    UNUSED(owner);
    UNUSED(msg);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

smio_err_e init_do_op (void *self, void *msg)
{
    return _init_do_op (self, msg);
}

const smio_ops_t init_ops = {
    .attach             = &init_attach,          /* Attach sm_io instance to dev_io */
    .deattach           = &init_deattach,        /* Deattach sm_io instance to dev_io */
    .export_ops         = &init_export_ops,      /* Export sm_io operations to dev_io */
    .unexport_ops       = &init_unexport_ops,    /* Unexport sm_io operations to dev_io */
    .do_op              = &init_do_op            /* Generic wrapper for handling specific operations */
};

/************************************************************/
/****************** Bootstrap Operations ********************/
/************************************************************/

smio_err_e init_init (smio_t * self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:init_exp] Initializing init\n");

    smio_err_e err = SMIO_SUCCESS;

    err = smio_set_id (self, INIT_SDB_DEVID);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO id", err_set_id);
    err = smio_set_name (self, INIT_SDB_NAME);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO name", err_set_name);

    /* Set SMIO ops pointers */
    err = smio_set_ops (self, &init_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO operations",
            err_smio_set_ops);
    err = smio_set_thsafe_client_ops (self, &smio_thsafe_client_zmq_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO thsafe operations",
            err_smio_set_thsafe_ops);

    /* disp_op_t structure is const and all of the functions performing on it
     * obviously receives a const argument, but here (and only on the SMIO
     * initialization) we need to make an exception if we want to keep the
     * functions' description and the function pointers separate */
    err = smio_init_exp_ops (self, (disp_op_t **) init_exp_ops, init_exp_fp);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not fill SMIO "
            "function descriptors with the callbacks", err_fill_desc);

    err = smio_set_exp_ops (self, init_exp_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO exported operations",
            err_smio_set_exp_ops);

    /* Initialize specific structure */
    smio_init_t *smio_handler = smio_init_new (self);
    ASSERT_ALLOC(smio_handler, err_smio_handler_alloc, SMIO_ERR_ALLOC);
    err = smio_set_handler (self, smio_handler);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO handler",
            err_smio_set_handler);

    return err;

err_smio_set_handler:
    smio_init_destroy (&smio_handler);
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

/* Destroy sm_io instance of init */
smio_err_e init_shutdown (smio_t *self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:init_exp] Shutting down init\n");

    smio_err_e err = SMIO_SUCCESS;
    smio_init_t *init = smio_get_handler (self);
    ASSERT_TEST(init != NULL, "Could not get INIT handler",
            err_init_handler, SMIO_ERR_ALLOC /* FIXME: improve return code */);

    /* Destroy SMIO instance */
    smio_init_destroy (&init);
    /* Nullify operation pointers */
    smio_set_exp_ops (self, NULL);
    smio_set_thsafe_client_ops (self, NULL);
    smio_set_ops (self, NULL);

err_init_handler:
    return err;
}

const smio_bootstrap_ops_t init_bootstrap_ops = {
    .init            = &init_init,
    .shutdown        = &init_shutdown,
    .config_defaults = NULL
};

SMIO_MOD_DECLARE(INIT_SDB_DEVID, INIT_SDB_NAME, init_bootstrap_ops)
