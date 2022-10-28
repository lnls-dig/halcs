/*
 *  * Copyright (C) 2020 LNLS (www.lnls.br)
 *   * Author: Melissa Aguiar <melissa.aguiar@cnpem.br>
 *    *
 *     * Released according to the GNU GPL, version 3 or any later version.
 *      */

#include "halcs_server.h"
/* Private headers */
#include "sm_io_fofb_processing_codes.h"
#include "sm_io_fofb_processing_exports.h"
#include "sm_io_fofb_processing_core.h"
#include "sm_io_fofb_processing_exp.h"
#include "hw/wb_fofb_processing_regs.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, SM_IO, "[sm_io:fofb_processing_exp]",            \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)                  \
    ASSERT_HAL_ALLOC(ptr, SM_IO, "[sm_io:fofb_processing_exp]",                    \
            smio_err_str(SMIO_ERR_ALLOC),                                      \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                               \
    CHECK_HAL_ERR(err, SM_IO, "[sm_io:fofb_processing_exp]",                       \
            smio_err_str (err_type))

/************************************************************/
/***********  Specific FOFB_PROCESSING Operations ***********/
/************************************************************/
RW_PARAM_FUNC(fofb_processing, coeffs_fixed_point_pos) {
    SET_GET_PARAM(fofb_processing, 0x0, FOFB_PROCESSING_REGS, COEFFS_FIXED_POINT_POS,
        /* No field */, MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
        NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(fofb_processing, accs_gains_fixed_point_pos) {
    SET_GET_PARAM(fofb_processing, 0x0, FOFB_PROCESSING_REGS, ACCS_GAINS_FIXED_POINT_POS,
        /* No field */, MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
        NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(fofb_processing, acc_gain) {
    SET_GET_PARAM_CHANNEL(fofb_processing, 0x0, FOFB_PROCESSING_REGS, ACC_GAIN,
        VAL, FOFB_PROCESSING_REGS_CHANNEL_DISTANCE, FOFB_PROCESSING_CHANNEL_COUNT,
        MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
        NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(fofb_processing, acc_clear) {
    SET_GET_PARAM_CHANNEL(fofb_processing, 0x0, FOFB_PROCESSING_REGS, ACC_CTL,
        CLEAR, FOFB_PROCESSING_REGS_CHANNEL_DISTANCE, FOFB_PROCESSING_CHANNEL_COUNT,
        SINGLE_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
        NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(fofb_processing, acc_freeze) {
    SET_GET_PARAM_CHANNEL(fofb_processing, 0x0, FOFB_PROCESSING_REGS, ACC_CTL,
        FREEZE, FOFB_PROCESSING_REGS_CHANNEL_DISTANCE, FOFB_PROCESSING_CHANNEL_COUNT,
        SINGLE_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
        NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(fofb_processing, sp_max) {
    SET_GET_PARAM_CHANNEL(fofb_processing, 0x0, FOFB_PROCESSING_REGS, SP_MAX,
        VAL, FOFB_PROCESSING_REGS_CHANNEL_SP_DISTANCE, FOFB_PROCESSING_CHANNEL_COUNT,
        MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
        NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}
RW_PARAM_FUNC(fofb_processing, sp_min) {
    SET_GET_PARAM_CHANNEL(fofb_processing, 0x0, FOFB_PROCESSING_REGS, SP_MIN,
        VAL, FOFB_PROCESSING_REGS_CHANNEL_SP_DISTANCE, FOFB_PROCESSING_CHANNEL_COUNT,
        MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
        NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

RW_PARAM_FUNC(fofb_processing, intlk_en) {
    SET_GET_PARAM(fofb_processing, 0x0, FOFB_PROCESSING_REGS, LOOP_INTLK_CTL,
        EN, SINGLE_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
        NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}
RW_PARAM_FUNC(fofb_processing, intlk_clr) {
    SET_GET_PARAM(fofb_processing, 0x0, FOFB_PROCESSING_REGS, LOOP_INTLK_CTL,
        CLR, SINGLE_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
        NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}
RW_PARAM_FUNC(fofb_processing, intlk_sta) {
    SET_GET_PARAM(fofb_processing, 0x0, FOFB_PROCESSING_REGS, LOOP_INTLK_STA,
        /* No field */, MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
        NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}
RW_PARAM_FUNC(fofb_processing, loop_orb_distort_limit) {
    SET_GET_PARAM(fofb_processing, 0x0, FOFB_PROCESSING_REGS, LOOP_ORB_DISTORT_LIMIT,
        /* No field */, MULT_BIT_PARAM, /* No minimum check*/, /* No maximum check */,
        NO_CHK_FUNC, NO_FMT_FUNC, SET_FIELD);
}

static int _fofb_processing_coeffs_ram_bank_read(void *owner, void *args,
    void *ret) {
    assert(owner);
    assert(args);
    assert(ret);

    int err;

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fofb_processing] "
        "Calling _fofb_processing_coeffs_ram_bank_read\n");
    SMIO_OWNER_TYPE *self = SMIO_EXP_OWNER(owner);

    const uint32_t chan = *(uint32_t *) EXP_MSG_ZMQ_FIRST_ARG(args);

    ssize_t size;
    size = smio_thsafe_client_read_block(self,
        FOFB_PROCESSING_REGS_COEFFS_RAM_BANK_OFFS(chan),
        FOFB_PROCESSING_REGS_COEFFS_RAM_BANK_SIZE, (uint32_t *)ret);

    err = (size == FOFB_PROCESSING_REGS_COEFFS_RAM_BANK_SIZE)?
        size : -FOFB_PROCESSING_ERR;

    return err;
}

static int _fofb_processing_coeffs_ram_bank_write(void *owner, void *args,
    void *ret) {
    assert (owner);
    assert (args);
    UNUSED(ret);

    int err;

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fofb_processing] "
        "Calling _fofb_processing_coeffs_ram_bank_write\n");
    SMIO_OWNER_TYPE *self = SMIO_EXP_OWNER(owner);

    const uint32_t chan = *(uint32_t *) EXP_MSG_ZMQ_FIRST_ARG(args);

    const struct _smio_fofb_processing_data_block_t coeffs =
        *(struct _smio_fofb_processing_data_block_t *) EXP_MSG_ZMQ_NEXT_ARG(args);

    ssize_t size;
    size = smio_thsafe_client_write_block(self,
        FOFB_PROCESSING_REGS_COEFFS_RAM_BANK_OFFS(chan),
        FOFB_PROCESSING_REGS_COEFFS_RAM_BANK_SIZE, coeffs.data);

    err = (size == FOFB_PROCESSING_REGS_COEFFS_RAM_BANK_SIZE)?
        FOFB_PROCESSING_OK : -FOFB_PROCESSING_ERR;

    return err;
}

static int _fofb_processing_setpoints_ram_bank_read(void *owner, void *args,
    void *ret) {
    assert(owner);
    UNUSED(args);
    assert(ret);

    int err;

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fofb_processing] "
        "Calling _fofb_processing_setpoints_ram_bank_read\n");
    SMIO_OWNER_TYPE *self = SMIO_EXP_OWNER(owner);

    ssize_t size;
    size = smio_thsafe_client_read_block(self,
        FOFB_PROCESSING_REGS_SETPOINTS_RAM_BANK_OFFS,
        FOFB_PROCESSING_REGS_SETPOINTS_RAM_BANK_SIZE, (uint32_t *)ret);

    err = (size == FOFB_PROCESSING_REGS_SETPOINTS_RAM_BANK_SIZE)?
        size : -FOFB_PROCESSING_ERR;

    return err;
}

static int _fofb_processing_setpoints_ram_bank_write(void *owner, void *args,
    void *ret) {
    assert(owner);
    assert(args);
    UNUSED(ret);

    int err;

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fofb_processing] "
        "Calling _fofb_processing_setpoints_ram_bank_write\n");
    SMIO_OWNER_TYPE *self = SMIO_EXP_OWNER(owner);

    const struct _smio_fofb_processing_data_block_t setpoints =
        *(struct _smio_fofb_processing_data_block_t *)
        EXP_MSG_ZMQ_FIRST_ARG(args);

    ssize_t size;
    size = smio_thsafe_client_write_block(self,
        FOFB_PROCESSING_REGS_SETPOINTS_RAM_BANK_OFFS,
        FOFB_PROCESSING_REGS_SETPOINTS_RAM_BANK_SIZE, setpoints.data);

    err = (size == FOFB_PROCESSING_REGS_SETPOINTS_RAM_BANK_SIZE)?
        FOFB_PROCESSING_OK : -FOFB_PROCESSING_ERR;

    return err;
}

/* Exported function pointers */
const disp_table_func_fp fofb_processing_exp_fp [] = {
    RW_PARAM_FUNC_NAME(fofb_processing, coeffs_fixed_point_pos),
    RW_PARAM_FUNC_NAME(fofb_processing, accs_gains_fixed_point_pos),
    RW_PARAM_FUNC_NAME(fofb_processing, acc_gain),
    RW_PARAM_FUNC_NAME(fofb_processing, acc_clear),
    RW_PARAM_FUNC_NAME(fofb_processing, acc_freeze),
    RW_PARAM_FUNC_NAME(fofb_processing, sp_max),
    RW_PARAM_FUNC_NAME(fofb_processing, sp_min),
    RW_PARAM_FUNC_NAME(fofb_processing, intlk_en),
    RW_PARAM_FUNC_NAME(fofb_processing, intlk_clr),
    RW_PARAM_FUNC_NAME(fofb_processing, intlk_sta),
    RW_PARAM_FUNC_NAME(fofb_processing, loop_orb_distort_limit),
    _fofb_processing_coeffs_ram_bank_read,
    _fofb_processing_coeffs_ram_bank_write,
    _fofb_processing_setpoints_ram_bank_read,
    _fofb_processing_setpoints_ram_bank_write,
    NULL
};

/************************************************************/
/***************** Export methods functions *****************/
/************************************************************/

static smio_err_e _fofb_processing_do_op (void *owner, void *msg);

/* Attach an instance of sm_io to dev_io function pointer */
smio_err_e fofb_processing_attach (smio_t *self, void *args)
{
    UNUSED(self);
    UNUSED(args);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Deattach an instance of sm_io to dev_io function pointer */
smio_err_e fofb_processing_deattach (smio_t *self)
{
    UNUSED(self);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Export (register) sm_io to handle operations function pointer */
smio_err_e fofb_processing_export_ops (smio_t *self,
        const disp_op_t **smio_exp_ops)
{
    UNUSED(self);
    UNUSED(smio_exp_ops);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Unexport (unregister) sm_io to handle operations function pointer */
smio_err_e fofb_processing_unexport_ops (smio_t *self)
{
    UNUSED(self);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Generic wrapper for receiving opcodes and arguments to specific funtions function pointer */
/* FIXME: Code repetition! _devio_do_smio_op () function does almost the same!!! */
smio_err_e _fofb_processing_do_op (void *owner, void *msg)
{
    UNUSED(owner);
    UNUSED(msg);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

smio_err_e fofb_processing_do_op (void *self, void *msg)
{
    return _fofb_processing_do_op (self, msg);
}

const smio_ops_t fofb_processing_ops = {
    .attach             = &fofb_processing_attach,          /* Attach sm_io instance to dev_io */
    .deattach           = &fofb_processing_deattach,        /* Deattach sm_io instance to dev_io */
    .export_ops         = &fofb_processing_export_ops,      /* Export sm_io operations to dev_io */
    .unexport_ops       = &fofb_processing_unexport_ops,    /* Unexport sm_io operations to dev_io */
    .do_op              = &fofb_processing_do_op            /* Generic wrapper for handling specific operations */
};

/************************************************************/
/****************** Bootstrap Operations ********************/
/************************************************************/

smio_err_e fofb_processing_init (smio_t * self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fofb_processing_exp] Initializing fofb_processing\n");

    smio_err_e err = SMIO_SUCCESS;

    err = smio_set_id (self, FOFB_PROCESSING_SDB_DEVID);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO id", err_set_id);
    err = smio_set_name (self, FOFB_PROCESSING_SDB_NAME);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO name", err_set_name);

    /* Set SMIO ops pointers */
    err = smio_set_ops (self, &fofb_processing_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO operations",
            err_smio_set_ops);
    err = smio_set_thsafe_client_ops (self, &smio_thsafe_client_zmq_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO thsafe operations",
            err_smio_set_thsafe_ops);

    /* disp_op_t structure is const and all of the functions performing on it
 *      * obviously receives a const argument, but here (and only on the SMIO
 *           * initialization) we need to make an exception if we want to keep the
 *                * functions' description and the function pointers separate */
    err = smio_init_exp_ops (self, (disp_op_t **) fofb_processing_exp_ops, fofb_processing_exp_fp);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not fill SMIO "
            "function descriptors with the callbacks", err_fill_desc);

    err = smio_set_exp_ops (self, fofb_processing_exp_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO exported operations",
            err_smio_set_exp_ops);

    /* Initialize specific structure */
    smio_fofb_processing_t *smio_handler = smio_fofb_processing_new (self);
    ASSERT_ALLOC(smio_handler, err_smio_handler_alloc, SMIO_ERR_ALLOC);
    err = smio_set_handler (self, smio_handler);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO handler",
            err_smio_set_handler);

    return err;

err_smio_set_handler:
    smio_fofb_processing_destroy (&smio_handler);
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

/* Destroy sm_io instance of fofb_processing */
smio_err_e fofb_processing_shutdown (smio_t *self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fofb_processing_exp] Shutting down fofb_processing\n");

    smio_err_e err = SMIO_SUCCESS;
    smio_fofb_processing_t *fofb_processing = smio_get_handler (self);
    ASSERT_TEST(fofb_processing != NULL, "Could not get FOFB_PROCESSING handler",
            err_fofb_processing_handler, SMIO_ERR_ALLOC /* FIXME: improve return code */);

    /* Destroy SMIO instance */
    smio_fofb_processing_destroy (&fofb_processing);
    /* Nullify operation pointers */
    smio_set_exp_ops (self, NULL);
    smio_set_thsafe_client_ops (self, NULL);
    smio_set_ops (self, NULL);

err_fofb_processing_handler:
    return err;
}

const smio_bootstrap_ops_t fofb_processing_bootstrap_ops = {
    .init            = &fofb_processing_init,
    .shutdown        = &fofb_processing_shutdown
};

SMIO_MOD_DECLARE(FOFB_PROCESSING_SDB_DEVID, FOFB_PROCESSING_SDB_NAME, fofb_processing_bootstrap_ops)

