/*
 * Copyright (C) 2017 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "halcs_server.h"
/* Private headers */
#include "sm_io_afc_mgmt_codes.h"
#include "sm_io_afc_mgmt_defaults.h"
#include "sm_io_afc_mgmt_exports.h"
#include "sm_io_afc_mgmt_core.h"
#include "sm_io_afc_mgmt_exp.h"
#include "hw/wb_afc_mgmt_regs.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...)  \
    ASSERT_HAL_TEST(test_boolean, SM_IO, "[sm_io:afc_mgmt_exp]",                \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)                   \
    ASSERT_HAL_ALLOC(ptr, SM_IO, "[sm_io:afc_mgmt_exp]",                        \
            smio_err_str(SMIO_ERR_ALLOC),                                       \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                                \
    CHECK_HAL_ERR(err, SM_IO, "[sm_io:afc_mgmt_exp]",                           \
            smio_err_str (err_type))

/************************************************************/
/*********** Specific AFC_MGMT Operations *************/
/************************************************************/

#define HALCS_AFC_MGMT_SI57X_OE_MIN             0 /* SI57X Output disable */
#define HALCS_AFC_MGMT_SI57X_OE_MAX             1 /* SI57X Output enable */

RW_PARAM_FUNC(afc_mgmt, si57x_oe) {
    SET_GET_PARAM(afc_mgmt, 0x0, AFC_MGMT,
            CLK_DISTRIB, SI57X_OE, SINGLE_BIT_PARAM,
            HALCS_AFC_MGMT_SI57X_OE_MIN, HALCS_AFC_MGMT_SI57X_OE_MAX, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

/******************************** Chips Export functions *************************/

/* Macros to avoid repetition of the function body Si57X */
typedef smch_err_e (*smch_si57x_func_fp) (smch_si57x_t *self, double *param);

#define AFC_MGMT_SI57X_FUNC_NAME(func_name)                               \
    _afc_mgmt_si57x_ ## func_name

#define AFC_MGMT_SI57X_FUNC_NAME_HEADER(func_name)                        \
    static int AFC_MGMT_SI57X_FUNC_NAME(func_name) (void *owner, void *args, void *ret)

#define AFC_MGMT_SI57X_FUNC_BODY(owner, args, ret, read_func, write_func, \
        error_msg)                                                              \
    do {                                                                        \
        UNUSED(ret);                                                             \
        assert (owner);                                                         \
        assert (args);                                                          \
                                                                                \
        int err = -AFC_MGMT_OK;                                                 \
        SMIO_OWNER_TYPE *self = SMIO_EXP_OWNER(owner);                          \
        smio_afc_mgmt_t *afcmgmt = smio_get_handler (self);                     \
        ASSERT_TEST(afcmgmt != NULL, "Could not get SMIO FMC ACTIVE CLK handler", \
                err_get_afcmgmt_handler, -AFC_MGMT_ERR);                        \
        smch_si57x_t *smch_si57x = SMIO_SI57X_HANDLER(afcmgmt);                 \
        uint32_t rw = *(uint32_t *) EXP_MSG_ZMQ_FIRST_ARG(args);                \
        double param = *(double *) EXP_MSG_ZMQ_NEXT_ARG(args);                  \
                                                                                \
        smch_err_e serr = SMCH_SUCCESS;                                         \
        /* Call specific function */                                            \
        if (rw) {                                                               \
            WHEN(ISEMPTY(read_func))(                                           \
                UNUSED(ret);                                                    \
                DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:afc_mgmt_exp] "   \
                        "SI57x read function not implemented\n");               \
                err = -AFC_MGMT_UNINPL;                                         \
                return err;                                                     \
            )                                                                   \
            WHENNOT(ISEMPTY(read_func))(                                        \
                double value = 0;                                               \
                serr = ((smch_si57x_func_fp) read_func) (smch_si57x,            \
                        &value);                                                \
                if (serr != SMCH_SUCCESS) {                                     \
                    err = -AFC_MGMT_ERR;                                        \
                }                                                               \
                else {                                                          \
                    *((double *) ret) = value;                                  \
                    err = sizeof (value);                                       \
                    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:afc_mgmt_exp] " \
                            "SI57x function read value = 0x%f\n", value);       \
                }                                                               \
            )                                                                   \
        }                                                                       \
        else {                                                                  \
            WHEN(ISEMPTY(write_func))(                                          \
                DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:afc_mgmt_exp] "   \
                        "SI57x write function not implemented\n");              \
                err = -AFC_MGMT_UNINPL;                                         \
                return err;                                                     \
            )                                                                   \
            WHENNOT(ISEMPTY(write_func))(                                       \
                serr = ((smch_si57x_func_fp) write_func) (smch_si57x,           \
                        &param);                                                \
                if (serr != SMCH_SUCCESS) {                                     \
                    err = -AFC_MGMT_ERR;                                        \
                }                                                               \
                else {                                                          \
                    err = -AFC_MGMT_OK;                                         \
                }                                                               \
            )                                                                   \
        }                                                                       \
                                                                                \
err_get_afcmgmt_handler:                                                        \
        return err;                                                             \
                                                                                \
    } while(0)

AFC_MGMT_SI57X_FUNC_NAME_HEADER(freq)
{
    AFC_MGMT_SI57X_FUNC_BODY(owner, args, ret, smch_si57x_get_freq,
            smch_si57x_set_freq, "Could not set SI57X frequency");
}

static smch_err_e smch_si57x_get_defaults_compat (smch_si57x_t *self, double *param)
{
    return smch_si57x_get_defaults (self, *param);
}

AFC_MGMT_SI57X_FUNC_NAME_HEADER(get_defaults)
{
    AFC_MGMT_SI57X_FUNC_BODY(owner, args, ret, /* No read func*/,
            smch_si57x_get_defaults_compat, "Could not restart SI57X to its defaults");
}

static smch_err_e smch_si57x_get_fstartup_compat (smch_si57x_t *self, double *param)
{
    smch_err_e err = SMCH_SUCCESS;
    double fstartup = smch_si57x_get_fstartup (self);
    *param = fstartup;
    return err;
}

static smch_err_e smch_si57x_set_fstartup_compat (smch_si57x_t *self, double *param)
{
    return smch_si57x_set_fstartup (self, *param);
}

AFC_MGMT_SI57X_FUNC_NAME_HEADER(fstartup)
{
    AFC_MGMT_SI57X_FUNC_BODY(owner, args, ret, smch_si57x_get_fstartup_compat,
            smch_si57x_set_fstartup_compat, "Could not set/get SI57X startup frequency");
}

/* Exported function pointers */
const disp_table_func_fp afc_mgmt_exp_fp [] = {
    RW_PARAM_FUNC_NAME(afc_mgmt, si57x_oe),
    AFC_MGMT_SI57X_FUNC_NAME(freq),
    AFC_MGMT_SI57X_FUNC_NAME(get_defaults),
    AFC_MGMT_SI57X_FUNC_NAME(fstartup),
    NULL
};

/************************************************************/
/***************** Export methods functions *****************/
/************************************************************/

static smio_err_e _afc_mgmt_do_op (void *owner, void *msg);

/* Attach an instance of sm_io to dev_io function pointer */
smio_err_e afc_mgmt_attach (smio_t *self, void *args)
{
    UNUSED(self);
    UNUSED(args);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Deattach an instance of sm_io to dev_io function pointer */
smio_err_e afc_mgmt_deattach (smio_t *self)
{
    UNUSED(self);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Export (register) sm_io to handle operations function pointer */
smio_err_e afc_mgmt_export_ops (smio_t *self,
        const disp_op_t** smio_exp_ops)
{
    UNUSED(self);
    UNUSED(smio_exp_ops);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Unexport (unregister) sm_io to handle operations function pointer */
smio_err_e afc_mgmt_unexport_ops (smio_t *self)
{
    UNUSED(self);
    return SMIO_ERR_FUNC_NOT_IMPL;
}


/* Generic wrapper for receiving opcodes and arguments to specific funtions function pointer */
/* FIXME: Code repetition! _devio_do_smio_op () function does almost the same!!! */
smio_err_e _afc_mgmt_do_op (void *owner, void *msg)
{
    UNUSED(owner);
    UNUSED(msg);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

smio_err_e afc_mgmt_do_op (void *self, void *msg)
{
    return _afc_mgmt_do_op (self, msg);
}

const smio_ops_t afc_mgmt_ops = {
    .attach             = &afc_mgmt_attach,          /* Attach sm_io instance to dev_io */
    .deattach           = &afc_mgmt_deattach,        /* Deattach sm_io instance to dev_io */
    .export_ops         = &afc_mgmt_export_ops,      /* Export sm_io operations to dev_io */
    .unexport_ops       = &afc_mgmt_unexport_ops,    /* Unexport sm_io operations to dev_io */
    .do_op              = &afc_mgmt_do_op            /* Generic wrapper for handling specific operations */
};

/************************************************************/
/****************** Bootstrap Operations ********************/
/************************************************************/

smio_err_e afc_mgmt_init (smio_t * self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:afc_mgmt_exp] Initializing afc_mgmt\n");

    smio_err_e err = SMIO_SUCCESS;

    err = smio_set_id (self, AFC_MGMT_SDB_DEVID);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO id", err_set_id);
    err = smio_set_name (self, AFC_MGMT_SDB_NAME);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO name", err_set_name);

    /* Set SMIO ops pointers */
    err = smio_set_ops (self, &afc_mgmt_ops);
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
    err = smio_init_exp_ops (self, (disp_op_t **) afc_mgmt_exp_ops,
            afc_mgmt_exp_fp);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not fill SMIO "
            "function descriptors with the callbacks", err_fill_desc);

    err = smio_set_exp_ops (self, afc_mgmt_exp_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO exported operations",
            err_smio_set_exp_ops);

    /* Initialize specific structure */
    smio_afc_mgmt_t *smio_handler = smio_afc_mgmt_new (self);
    ASSERT_ALLOC(smio_handler, err_smio_handler_alloc, SMIO_ERR_ALLOC);
    err = smio_set_handler (self, smio_handler);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO handler",
            err_smio_set_handler);

    return err;

err_smio_set_handler:
    smio_afc_mgmt_destroy (&smio_handler);
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

/* Destroy sm_io instance of afc_mgmt */
smio_err_e afc_mgmt_shutdown (smio_t *self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:afc_mgmt_exp] Shutting down afc_mgmt\n");

    smio_err_e err = SMIO_SUCCESS;
    smio_afc_mgmt_t *afcmgmt = smio_get_handler (self);
    ASSERT_TEST(afcmgmt != NULL, "Could not get FMC ACTIVE CLK handler",
            err_afcmgmt_handler, SMIO_ERR_ALLOC /* FIXME: improve return code */);

    /* Destroy SMIO instance */
    smio_afc_mgmt_destroy (&afcmgmt);
    /* Nullify operation pointers */
    smio_set_exp_ops (self, NULL);
    smio_set_thsafe_client_ops (self, NULL);
    smio_set_ops (self, NULL);

err_afcmgmt_handler:
    return err;
}

const smio_bootstrap_ops_t afc_mgmt_bootstrap_ops = {
    .init            = &afc_mgmt_init,
    .shutdown        = &afc_mgmt_shutdown,
    .config_defaults = &afc_mgmt_config_defaults
};

SMIO_MOD_DECLARE(AFC_MGMT_SDB_DEVID, AFC_MGMT_SDB_NAME, afc_mgmt_bootstrap_ops)
