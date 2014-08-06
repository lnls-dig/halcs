/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include <stdlib.h>

#include "sm_io_fmc130m_4ch_exp.h"
#include "sm_io_fmc130m_4ch_codes.h"
#include "sm_io.h"
#include "dev_io.h"
#include "board.h"
#include "hal_assert.h"
#include "wb_fmc130m_4ch_regs.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...)  \
    ASSERT_HAL_TEST(test_boolean, SM_IO, "[sm_io:fmc130m_4ch_exp]", \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_ALLOC(ptr, SM_IO, "[sm_io:fmc130m_4ch_exp]", \
            smio_err_str(SMIO_ERR_ALLOC),                   \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                            \
    CHECK_HAL_ERR(err, SM_IO, "[sm_io:fmc130m_4ch_exp]",    \
            smio_err_str (err_type))

/************************************************************/
/************ Specific FMC_130M_4CH Operations **************/
/************************************************************/

static void *_fmc130m_4ch_leds (void *owner, void *args)
{
    assert (owner);
    assert (args);

    smio_t *self = (smio_t *) owner;
    exp_msg_zmq_t *exp_msg = (exp_msg_zmq_t *) args;
    assert (zmsg_size (*exp_msg->msg) > 0);

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fmc130m_4ch_exp] Calling _fmc130m_4ch_leds\n");
    uint32_t leds = *(uint32_t *) zframe_data (zmsg_pop (*exp_msg->msg));
    smio_thsafe_client_write_32 (self, FMC_130M_CTRL_REGS_OFFS |
            WB_FMC_130M_4CH_CSR_REG_MONITOR , &leds);
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fmc130m_4ch_exp] Led write: 0x%08x\n",
            leds);

    /* Don't send any response, for now... */

    return NULL;
}

const smio_exp_ops_t fmc130m_exp_ops [] = {
    {.name = FMC130M_4CH_NAME_LEDS, .opcode = FMC130M_4CH_OPCODE_LEDS, .func_fp = _fmc130m_4ch_leds},
    {.name = NULL, .opcode = 0, .func_fp = NULL} /* Must end with this NULL pattern */
};

/************************************************************/
/***************** Export methods functions *****************/
/************************************************************/

static smio_err_e _fmc130m_4ch_do_op (void *owner, void *msg);

/* Attach an instance of sm_io to dev_io function pointer */
smio_err_e fmc130m_4ch_attach (smio_t *self, devio_t *parent)
{
    (void) self;
    (void) parent;
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Deattach an instance of sm_io to dev_io function pointer */
smio_err_e fmc130m_4ch_deattach (smio_t *self)
{
    (void) self;
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Export (register) sm_io to handle operations function pointer */
smio_err_e fmc130m_4ch_export_ops (smio_t *self,
        const smio_exp_ops_t* smio_exp_ops)
{
    (void) self;
    (void) smio_exp_ops;
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Unexport (unregister) sm_io to handle operations function pointer */
smio_err_e fmc130m_4ch_unexport_ops (smio_t *self)
{
    (void) self;
    return SMIO_ERR_FUNC_NOT_IMPL;
}


/* Generic wrapper for receiving opcodes and arguments to specific funtions function pointer */
/* FIXME: Code repetition! _devio_do_smio_op () function does almost the same!!! */
smio_err_e _fmc130m_4ch_do_op (void *owner, void *msg)
{
    (void) owner;
    (void) msg;
    return SMIO_ERR_FUNC_NOT_IMPL;
}

smio_err_e fmc130m_4ch_do_op (void *self, void *msg)
{
    return _fmc130m_4ch_do_op (self, msg);
}

const smio_ops_t fmc130m_4ch_ops = {
    .attach             = fmc130m_4ch_attach,          /* Attach sm_io instance to dev_io */
    .deattach           = fmc130m_4ch_deattach,        /* Deattach sm_io instance to dev_io */
    .export_ops         = fmc130m_4ch_export_ops,      /* Export sm_io operations to dev_io */
    .unexport_ops       = fmc130m_4ch_unexport_ops,    /* Unexport sm_io operations to dev_io */
    .do_op              = fmc130m_4ch_do_op            /* Generic wrapper for handling specific operations */
};

/************************************************************/
/****************** Bootstrap Operations ********************/
/************************************************************/

smio_err_e fmc130m_4ch_init (smio_t * self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fmc130m_4ch_exp] Initializing fmc130m_4ch\n");

    smio_err_e err = SMIO_SUCCESS;

    self->id = FMC130M_4CH_SDB_DEVID;
    self->name = strdup (FMC130M_4CH_SDB_NAME);
    ASSERT_ALLOC(self->name, err_name_alloc, SMIO_ERR_ALLOC);

    /* Set SMIO ops pointers */
    self->ops = &fmc130m_4ch_ops;
    self->thsafe_client_ops = &smio_thsafe_client_zmq_ops;
    self->exp_ops = fmc130m_exp_ops;

    /* Initialize specific structure */
    self->smio_handler = smio_fmc130m_4ch_new (0);
    ASSERT_ALLOC(self->smio_handler, err_smio_handler_alloc, SMIO_ERR_ALLOC);

    return err;

err_smio_handler_alloc:
    free (self->name);
err_name_alloc:
    return err;
}

/* Destroy sm_io instance of fmc130m_4ch */
smio_err_e fmc130m_4ch_shutdown (smio_t *self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fmc130m_4ch_exp] Shutting down fmc130m_4ch\n");

    smio_fmc130m_4ch_destroy ((smio_fmc130m_4ch_t **) &self->smio_handler);
    self->exp_ops = NULL;
    self->thsafe_client_ops = NULL;
    self->ops = NULL;
    free (self->name);

    return SMIO_SUCCESS;
}

const smio_bootstrap_ops_t fmc130m_4ch_bootstrap_ops = {
    .init = fmc130m_4ch_init,
    .shutdown = fmc130m_4ch_shutdown
};
