/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include <stdlib.h>

#include "sm_io_afc_diag_exp.h"
#include "sm_io_afc_diag_codes.h"
#include "sm_io.h"
#include "dev_io_core.h"
#include "hal_assert.h"
#include "board.h"
#include "rw_param.h"
#include "rw_param_codes.h"
#include "wb_afc_diag_regs.h"
#include "sm_io_afc_diag_defaults.h"
#include "sm_io_afc_diag_exports.h"
#include "hal_stddef.h"
#include "revision.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, SM_IO, "[sm_io:afc_diag_exp]",        \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)           \
    ASSERT_HAL_ALLOC(ptr, SM_IO, "[sm_io:afc_diag_exp]",                \
            smio_err_str(SMIO_ERR_ALLOC),                               \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                        \
    CHECK_HAL_ERR(err, SM_IO, "[sm_io:afc_diag_exp]",                   \
            smio_err_str (err_type))

/************************************************************/
/*****************  Specific AFC_DIAG Operations *****************/
/************************************************************/

RW_PARAM_FUNC(afc_diag, card_slot) {
    SET_GET_PARAM(afc_diag, WB_AFC_DIAG_CTRL_RAW_REGS_OFFS, BPM_AFC_DIAG, GEO_ID,
            CARD_SLOT, MULT_BIT_PARAM, /* No minimum limit */,
            /* No maximum limit */, NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(afc_diag, ipmi_addr) {
    SET_GET_PARAM(afc_diag, WB_AFC_DIAG_CTRL_RAW_REGS_OFFS, BPM_AFC_DIAG, GEO_ID,
            IPMI_ADDR, MULT_BIT_PARAM, /* No minimum limit */,
            /* No maximum limit */, NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

/* Software ID functions */

/* Macros to avoid repetition of the function body */
typedef int (*smio_afc_diag_func_fp) (char *dest, size_t size);

/* Macros to avoid repetition of the function body */
#define AFC_DIAG_INFO_FUNC_NAME(func_name)                                      \
    _afc_diag_info_get_ ## func_name

#define AFC_DIAG_INFO_FUNC_NAME_HEADER(func_name)                               \
    static int AFC_DIAG_INFO_FUNC_NAME(func_name) (void *owner, void *args, void *ret)

static int _afc_diag_info_rw (void *owner, void *args, void *ret,
        smio_afc_diag_func_fp read_func, AFC_DIAG_OPCODE_TYPE id,
        const char *error_msg)
{
    assert (owner);
    assert (args);

    int err = -AFC_DIAG_OK;
    /* Unused parameter */
    uint32_t rw = *(uint32_t *) EXP_MSG_ZMQ_FIRST_ARG(args);
    (void) rw;

    /* Unused parameter */
    uint32_t param = *(uint32_t *) EXP_MSG_ZMQ_NEXT_ARG(args);
    (void) param;
    uint32_t ret_size = DISP_GET_ASIZE(afc_diag_exp_ops [id]->retval);

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:afc_diag_exp] Calling "
            "AFC_DIAG function ID %u\n", id);

    /* Call specific function */
    int herr = (read_func) (ret, ret_size);

    if (herr < 0 || (uint32_t) herr >= ret_size) {
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO,
                "[sm_io:afc_diag_exp] Could not clone string. Enconding error or "
                "truncation occured\n");
        err = -AFC_DIAG_ERR;
    }
    else {
        err = herr;
    }

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE,
            "[sm_io:afc_diag_exp] Function %s %s\n",
            afc_diag_exp_ops [id]->name,
            (err == -AFC_DIAG_ERR)? error_msg : "successfully executed");

    return err;
}

AFC_DIAG_INFO_FUNC_NAME_HEADER(build_revision)
{
    return _afc_diag_info_rw(owner, args, ret, halutils_copy_build_revision,
            AFC_DIAG_OPCODE_GET_BUILD_REVISION, "Could not get build revision");
}

AFC_DIAG_INFO_FUNC_NAME_HEADER(build_date)
{
    return _afc_diag_info_rw(owner, args, ret, halutils_copy_build_date,
            AFC_DIAG_OPCODE_GET_BUILD_DATE, "Could not get build date");
}

AFC_DIAG_INFO_FUNC_NAME_HEADER(build_user_name)
{
    return _afc_diag_info_rw(owner, args, ret, halutils_copy_build_user_name,
            AFC_DIAG_OPCODE_GET_BUILD_USER_NAME, "Could not get build user name");
}

AFC_DIAG_INFO_FUNC_NAME_HEADER(build_user_email)
{
    return _afc_diag_info_rw(owner, args, ret, halutils_copy_build_user_email,
            AFC_DIAG_OPCODE_GET_BUILD_USER_EMAIL, "Could not get build user email");
}

/* Exported function pointers */
const disp_table_func_fp afc_diag_exp_fp [] = {
    RW_PARAM_FUNC_NAME(afc_diag, card_slot),
    RW_PARAM_FUNC_NAME(afc_diag, ipmi_addr),
    AFC_DIAG_INFO_FUNC_NAME(build_revision),
    AFC_DIAG_INFO_FUNC_NAME(build_date),
    AFC_DIAG_INFO_FUNC_NAME(build_user_name),
    AFC_DIAG_INFO_FUNC_NAME(build_user_email),
    NULL
};

/************************************************************/
/***************** Export methods functions *****************/
/************************************************************/

static smio_err_e _afc_diag_do_op (void *owner, void *msg);

/* Attach an instance of sm_io to dev_io function pointer */
smio_err_e afc_diag_attach (smio_t *self, devio_t *parent)
{
    (void) self;
    (void) parent;
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Deattach an instance of sm_io to dev_io function pointer */
smio_err_e afc_diag_deattach (smio_t *self)
{
    (void) self;
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Export (register) sm_io to handle operations function pointer */
smio_err_e afc_diag_export_ops (smio_t *self,
        const disp_op_t **smio_exp_ops)
{
    (void) self;
    (void) smio_exp_ops;
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Unexport (unregister) sm_io to handle operations function pointer */
smio_err_e afc_diag_unexport_ops (smio_t *self)
{
    (void) self;
    return SMIO_ERR_FUNC_NOT_IMPL;
}

smio_err_e _afc_diag_do_op (void *owner, void *msg)
{
    (void) owner;
    (void) msg;
    return SMIO_ERR_FUNC_NOT_IMPL;
}

smio_err_e afc_diag_do_op (void *self, void *msg)
{
    return _afc_diag_do_op (self, msg);
}

const smio_ops_t afc_diag_ops = {
    .attach             = afc_diag_attach,          /* Attach sm_io instance to dev_io */
    .deattach           = afc_diag_deattach,        /* Deattach sm_io instance to dev_io */
    .export_ops         = afc_diag_export_ops,      /* Export sm_io operations to dev_io */
    .unexport_ops       = afc_diag_unexport_ops,    /* Unexport sm_io operations to dev_io */
    .do_op              = afc_diag_do_op            /* Generic wrapper for handling specific operations */
};

/************************************************************/
/****************** Bootstrap Operations ********************/
/************************************************************/

smio_err_e afc_diag_init (smio_t * self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:afc_diag_exp] Initializing afc_diag\n");

    smio_err_e err = SMIO_SUCCESS;

    self->id = AFC_DIAG_DEVID;
    self->name = strdup (AFC_DIAG_NAME);
    ASSERT_ALLOC(self->name, err_name_alloc, SMIO_SUCCESS);

    /* Set SMIO ops pointers */
    self->ops = &afc_diag_ops;
    self->thsafe_client_ops = &smio_thsafe_client_zmq_ops;

    /* disp_op_t structure is const and all of the functions performing on it
     * obviously receives a const argument, but here (and only on the SMIO
     * initialization) we need to make an exception if we want to keep the
     * functions' description and the function pointers separate */
    err = smio_init_exp_ops (self, (disp_op_t **) afc_diag_exp_ops, afc_diag_exp_fp);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not fill SMIO "
            "function descriptors with the callbacks", err_fill_desc);

    self->exp_ops = afc_diag_exp_ops;

    /* Initialize specific structure */
    self->smio_handler = smio_afc_diag_new (self);
    ASSERT_ALLOC(self->smio_handler, err_smio_handler_alloc, SMIO_SUCCESS);

    return err;

err_smio_handler_alloc:
err_fill_desc:
    free (self->name);
err_name_alloc:
    return err;
}

/* Destroy sm_io instance of afc_diag */
smio_err_e afc_diag_shutdown (smio_t *self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:afc_diag_exp] Shutting down afc_diag\n");

    smio_afc_diag_destroy ((smio_afc_diag_t **)&self->smio_handler);
    self->exp_ops = NULL;
    self->thsafe_client_ops = NULL;
    self->ops = NULL;
    free (self->name);

    return SMIO_SUCCESS;
}

const smio_bootstrap_ops_t afc_diag_bootstrap_ops = {
    .init = afc_diag_init,
    .shutdown = afc_diag_shutdown,
    .config_defaults = afc_diag_config_defaults
};
