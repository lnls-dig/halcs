/*
 * Copyright (C) 2022 LNLS (www.lnls.br)
 * Author: Érico Nogueira <erico.rolim@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "halcs_server.h"
/* Private headers */
#include "sm_io_clk_cnt_codes.h"
#include "sm_io_clk_cnt_exports.h"
#include "sm_io_clk_cnt_core.h"
#include "sm_io_clk_cnt_exp.h"
#include "hw/wb_clk_cnt_regs.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, SM_IO, "[sm_io:clk_cnt_exp]",              \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)                  \
    ASSERT_HAL_ALLOC(ptr, SM_IO, "[sm_io:clk_cnt_exp]",                      \
            smio_err_str(SMIO_ERR_ALLOC),                                      \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                               \
    CHECK_HAL_ERR(err, SM_IO, "[sm_io:clk_cnt_exp]",                         \
            smio_err_str (err_type))

/******************************************************************/
/*****************  Specific CLK_CNT Operations *****************/
/******************************************************************/

/* Regular registers */

RW_PARAM_FUNC(clk_cnt, trig_act) {
    SET_GET_PARAM(clk_cnt, 0x0, CLK_CNT, CTL, TRIG_ACT, SINGLE_BIT_PARAM,
            0, 1, NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(clk_cnt, cnt_snap) {
    SET_GET_PARAM(clk_cnt, 0x0, CLK_CNT, CNT_SNAP, /* no field */, MULT_BIT_PARAM,
            /* no minimum */, /* no maximum */, NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

/* Exported function pointers */
const disp_table_func_fp clk_cnt_exp_fp [] = {
    RW_PARAM_FUNC_NAME(clk_cnt, trig_act),
    RW_PARAM_FUNC_NAME(clk_cnt, cnt_snap),
    NULL
};

/************************************************************/
/***************** Export methods functions *****************/
/************************************************************/

static smio_err_e _clk_cnt_do_op (void *owner, void *msg);

/* Attach an instance of sm_io to dev_io function pointer */
smio_err_e clk_cnt_attach (smio_t *self, void *args)
{
    UNUSED(self);
    UNUSED(args);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Deattach an instance of sm_io to dev_io function pointer */
smio_err_e clk_cnt_deattach (smio_t *self)
{
    UNUSED(self);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Export (register) sm_io to handle operations function pointer */
smio_err_e clk_cnt_export_ops (smio_t *self,
        const disp_op_t **smio_exp_ops)
{
    UNUSED(self);
    UNUSED(smio_exp_ops);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Unexport (unregister) sm_io to handle operations function pointer */
smio_err_e clk_cnt_unexport_ops (smio_t *self)
{
    UNUSED(self);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Generic wrapper for receiving opcodes and arguments to specific funtions function pointer */
/* FIXME: Code repetition! _devio_do_smio_op () function does almost the same!!! */
smio_err_e _clk_cnt_do_op (void *owner, void *msg)
{
    UNUSED(owner);
    UNUSED(msg);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

smio_err_e clk_cnt_do_op (void *self, void *msg)
{
    return _clk_cnt_do_op (self, msg);
}

const smio_ops_t clk_cnt_ops = {
    .attach             = &clk_cnt_attach,          /* Attach sm_io instance to dev_io */
    .deattach           = &clk_cnt_deattach,        /* Deattach sm_io instance to dev_io */
    .export_ops         = &clk_cnt_export_ops,      /* Export sm_io operations to dev_io */
    .unexport_ops       = &clk_cnt_unexport_ops,    /* Unexport sm_io operations to dev_io */
    .do_op              = &clk_cnt_do_op            /* Generic wrapper for handling specific operations */
};

/************************************************************/
/****************** Bootstrap Operations ********************/
/************************************************************/

smio_err_e clk_cnt_init (smio_t * self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:clk_cnt_exp] Initializing clk_cnt\n");

    smio_err_e err = SMIO_SUCCESS;

    err = smio_set_id (self, CLK_CNT_SDB_DEVID);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO id", err_set_id);
    err = smio_set_name (self, CLK_CNT_SDB_NAME);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO name", err_set_name);

    /* Set SMIO ops pointers */
    err = smio_set_ops (self, &clk_cnt_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO operations",
            err_smio_set_ops);
    err = smio_set_thsafe_client_ops (self, &smio_thsafe_client_zmq_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO thsafe operations",
            err_smio_set_thsafe_ops);

    /* disp_op_t structure is const and all of the functions performing on it
     * obviously receives a const argument, but here (and only on the SMIO
     * initialization) we need to make an exception if we want to keep the
     * functions' description and the function pointers separate */
    err = smio_init_exp_ops (self, (disp_op_t **) clk_cnt_exp_ops, clk_cnt_exp_fp);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not fill SMIO "
            "function descriptors with the callbacks", err_fill_desc);

    err = smio_set_exp_ops (self, clk_cnt_exp_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO exported operations",
            err_smio_set_exp_ops);

    /* Initialize specific structure */
    smio_clk_cnt_t *smio_handler = smio_clk_cnt_new (self);
    ASSERT_ALLOC(smio_handler, err_smio_handler_alloc, SMIO_ERR_ALLOC);
    err = smio_set_handler (self, smio_handler);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO handler",
            err_smio_set_handler);

    return err;

err_smio_set_handler:
    smio_clk_cnt_destroy (&smio_handler);
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

/* Destroy sm_io instance of clk_cnt */
smio_err_e clk_cnt_shutdown (smio_t *self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:clk_cnt_exp] Shutting down clk_cnt\n");

    smio_err_e err = SMIO_SUCCESS;
    smio_clk_cnt_t *clk_cnt = smio_get_handler (self);
    ASSERT_TEST(clk_cnt != NULL, "Could not get CLK_CNT handler",
            err_clk_cnt_handler, SMIO_ERR_ALLOC /* FIXME: improve return code */);

    /* Destroy SMIO instance */
    smio_clk_cnt_destroy (&clk_cnt);
    /* Nullify operation pointers */
    smio_set_exp_ops (self, NULL);
    smio_set_thsafe_client_ops (self, NULL);
    smio_set_ops (self, NULL);

err_clk_cnt_handler:
    return err;
}

const smio_bootstrap_ops_t clk_cnt_bootstrap_ops = {
    .init            = &clk_cnt_init,
    .shutdown        = &clk_cnt_shutdown,
};

SMIO_MOD_DECLARE(CLK_CNT_SDB_DEVID, CLK_CNT_SDB_NAME, clk_cnt_bootstrap_ops)
