/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include <stdlib.h>

#include "sm_io_rffe_exp.h"
#include "sm_io_rffe_codes.h"
#include "sm_io.h"
#include "dev_io_core.h"
#include "errhand.h"
#include "board.h"
#include "rw_param.h"
#include "rw_param_codes.h"
#include "sm_io_rffe_defaults.h"
#include "sm_io_rffe_exports.h"
#include "hal_stddef.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, SM_IO, "[sm_io:rffe_exp]",            \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)           \
    ASSERT_HAL_ALLOC(ptr, SM_IO, "[sm_io:rffe_exp]",                    \
            smio_err_str(SMIO_ERR_ALLOC),                               \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                        \
    CHECK_HAL_ERR(err, SM_IO, "[sm_io:rffe_exp]",                       \
            smio_err_str (err_type))

/************************************************************/
/*****************  Specific RFFE Operations *****************/
/************************************************************/

/* Macros to avoid repetition of the function body */
typedef smch_err_e (*smch_rffe_func_fp) (smch_rffe_t *self, uint32_t id,
        uint8_t *data, size_t size);

#define RFFE_FUNC_NAME(func_name)                                       \
    _rffe_set_get_ ## func_name

#define RFFE_FUNC_NAME_HEADER(func_name)                                \
    static int RFFE_FUNC_NAME(func_name) (void *owner, void *args, void *ret)

static int _rffe_var_rw (void *owner, void *args, void *ret,
        smch_rffe_func_fp read_func, smch_rffe_func_fp write_func,
        RFFE_OPCODE_TYPE id, const char *error_msg)
{
    assert (owner);
    assert (args);

    int err = -RFFE_OK;
    SMIO_OWNER_TYPE *self = SMIO_EXP_OWNER(owner);
    smch_rffe_t *smch_rffe = SMIO_CTL_HANDLER(self);
    uint32_t rw = *(uint32_t *) EXP_MSG_ZMQ_FIRST_ARG(args);

    EXP_MSG_ZMQ_ARG_TYPE param_zmq = EXP_MSG_ZMQ_PEEK_NEXT_ARG(args);
    size_t param_size = EXP_MSG_ZMQ_ARG_SIZE(param_zmq);
    void *param = EXP_MSG_ZMQ_ARG_DATA(param_zmq);
    uint32_t ret_size = DISP_GET_ASIZE(rffe_exp_ops [id]->retval);

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:rffe_exp] Calling "
            "RFFE function ID %u\n", id);

    smch_err_e serr = SMCH_SUCCESS;
    /* Call specific function */
    if (rw) {
        serr = (read_func) (smch_rffe, id, (uint8_t *) ret, ret_size);
        if (serr != SMCH_SUCCESS) {
            err = -RFFE_ERR;
        }
        else {
            err = ret_size;
        }
    }
    else {
        serr = (write_func) (smch_rffe, id, (uint8_t *) param, param_size);
        if (serr != SMCH_SUCCESS) {
            err = -RFFE_ERR;
        }
        else {
            err = -RFFE_OK;
        }
    }
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE,
            "[sm_io:rffe_exp] Function %s %s\n",
            rffe_exp_ops [id]->name,
            (err == -RFFE_ERR)? error_msg : "successfully executed");

    return err;
}

RFFE_FUNC_NAME_HEADER(sw)
{
    return _rffe_var_rw(owner, args, ret, smch_rffe_read_var,
            smch_rffe_write_var, RFFE_OPCODE_SET_GET_SW,
            "Could not set/get RFFE switching");
}


RFFE_FUNC_NAME_HEADER(att1)
{
    return _rffe_var_rw(owner, args, ret, smch_rffe_read_var,
            smch_rffe_write_var, RFFE_OPCODE_SET_GET_ATT1,
            "Could not set/get RFFE attenuator 1");
}

RFFE_FUNC_NAME_HEADER(att2)
{
    return _rffe_var_rw(owner, args, ret, smch_rffe_read_var,
            smch_rffe_write_var, RFFE_OPCODE_SET_GET_ATT2,
            "Could not set/get RFFE attenuator 2");
}

RFFE_FUNC_NAME_HEADER(temp1)
{
    return _rffe_var_rw(owner, args, ret, smch_rffe_read_var,
            smch_rffe_write_var, RFFE_OPCODE_SET_GET_TEMP1,
            "Could not set/get RFFE temperature 1");
}

RFFE_FUNC_NAME_HEADER(temp2)
{
    return _rffe_var_rw(owner, args, ret, smch_rffe_read_var,
            smch_rffe_write_var, RFFE_OPCODE_SET_GET_TEMP2,
            "Could not set/get RFFE temperature 2");
}

RFFE_FUNC_NAME_HEADER(temp3)
{
    return _rffe_var_rw(owner, args, ret, smch_rffe_read_var,
            smch_rffe_write_var, RFFE_OPCODE_SET_GET_TEMP3,
            "Could not set/get RFFE temperature 3");
}


RFFE_FUNC_NAME_HEADER(temp4)
{
    return _rffe_var_rw(owner, args, ret, smch_rffe_read_var,
            smch_rffe_write_var, RFFE_OPCODE_SET_GET_TEMP4,
            "Could not set/get RFFE temperature 4");
}

RFFE_FUNC_NAME_HEADER(set_point1)
{
    return _rffe_var_rw(owner, args, ret, smch_rffe_read_var,
            smch_rffe_write_var, RFFE_OPCODE_SET_GET_SET_POINT1,
            "Could not set/get RFFE set point 1");
}

RFFE_FUNC_NAME_HEADER(set_point2)
{
    return _rffe_var_rw(owner, args, ret, smch_rffe_read_var,
            smch_rffe_write_var, RFFE_OPCODE_SET_GET_SET_POINT2,
            "Could not set/get RFFE set point 2");
}

RFFE_FUNC_NAME_HEADER(temp_control)
{
    return _rffe_var_rw(owner, args, ret, smch_rffe_read_var,
            smch_rffe_write_var, RFFE_OPCODE_SET_GET_TEMP_CONTROL,
            "Could not set/get RFFE temperature control");
}

RFFE_FUNC_NAME_HEADER(output1)
{
    return _rffe_var_rw(owner, args, ret, smch_rffe_read_var,
            smch_rffe_write_var, RFFE_OPCODE_SET_GET_OUTPUT1,
            "Could not set/get RFFE output 1");
}


RFFE_FUNC_NAME_HEADER(output2)
{
    return _rffe_var_rw(owner, args, ret, smch_rffe_read_var,
            smch_rffe_write_var, RFFE_OPCODE_SET_GET_OUTPUT2,
            "Could not set/get RFFE output 2");
}

RFFE_FUNC_NAME_HEADER(reset)
{
    return _rffe_var_rw(owner, args, ret, smch_rffe_read_var,
            smch_rffe_write_var, RFFE_OPCODE_SET_GET_RESET,
            "Could not set/get RFFE reset");
}

RFFE_FUNC_NAME_HEADER(reprog)
{
    return _rffe_var_rw(owner, args, ret, smch_rffe_read_var,
            smch_rffe_write_var, RFFE_OPCODE_SET_GET_REPROG,
            "Could not set/get RFFE reprogramming");
}

RFFE_FUNC_NAME_HEADER(data)
{
    return _rffe_var_rw(owner, args, ret, smch_rffe_read_var,
            smch_rffe_write_var, RFFE_OPCODE_SET_GET_DATA,
            "Could not set/get RFFE data");
}

RFFE_FUNC_NAME_HEADER(version)
{
    return _rffe_var_rw(owner, args, ret, smch_rffe_read_var,
            smch_rffe_write_var, RFFE_OPCODE_SET_GET_VERSION,
            "Could not set/get RFFE version");
}


RFFE_FUNC_NAME_HEADER(sw_lvl)
{
    return _rffe_var_rw(owner, args, ret, smch_rffe_read_var,
            smch_rffe_write_var, RFFE_OPCODE_SET_GET_SW_LVL,
            "Could not set/get RFFE switching level");
}

/* Exported function pointers */
const disp_table_func_fp rffe_exp_fp [] = {
    RFFE_FUNC_NAME(sw),
    RFFE_FUNC_NAME(att1),
    RFFE_FUNC_NAME(att2),
    RFFE_FUNC_NAME(temp1),
    RFFE_FUNC_NAME(temp2),
    RFFE_FUNC_NAME(temp3),
    RFFE_FUNC_NAME(temp4),
    RFFE_FUNC_NAME(set_point1),
    RFFE_FUNC_NAME(set_point2),
    RFFE_FUNC_NAME(temp_control),
    RFFE_FUNC_NAME(output1),
    RFFE_FUNC_NAME(output2),
    RFFE_FUNC_NAME(reset),
    RFFE_FUNC_NAME(reprog),
    RFFE_FUNC_NAME(data),
    RFFE_FUNC_NAME(version),
    RFFE_FUNC_NAME(sw_lvl),
    NULL
};

/************************************************************/
/***************** Export methods functions *****************/
/************************************************************/

static smio_err_e _rffe_do_op (void *owner, void *msg);

/* Attach an instance of sm_io to dev_io function pointer */
smio_err_e rffe_attach (smio_t *self, devio_t *parent)
{
    (void) self;
    (void) parent;
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Deattach an instance of sm_io to dev_io function pointer */
smio_err_e rffe_deattach (smio_t *self)
{
    (void) self;
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Export (register) sm_io to handle operations function pointer */
smio_err_e rffe_export_ops (smio_t *self,
        const disp_op_t **smio_exp_ops)
{
    (void) self;
    (void) smio_exp_ops;
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Unexport (unregister) sm_io to handle operations function pointer */
smio_err_e rffe_unexport_ops (smio_t *self)
{
    (void) self;
    return SMIO_ERR_FUNC_NOT_IMPL;
}

smio_err_e _rffe_do_op (void *owner, void *msg)
{
    (void) owner;
    (void) msg;
    return SMIO_ERR_FUNC_NOT_IMPL;
}

smio_err_e rffe_do_op (void *self, void *msg)
{
    return _rffe_do_op (self, msg);
}

const smio_ops_t rffe_ops = {
    .attach             = rffe_attach,          /* Attach sm_io instance to dev_io */
    .deattach           = rffe_deattach,        /* Deattach sm_io instance to dev_io */
    .export_ops         = rffe_export_ops,      /* Export sm_io operations to dev_io */
    .unexport_ops       = rffe_unexport_ops,    /* Unexport sm_io operations to dev_io */
    .do_op              = rffe_do_op            /* Generic wrapper for handling specific operations */
};

/************************************************************/
/****************** Bootstrap Operations ********************/
/************************************************************/

smio_err_e rffe_init (smio_t * self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:rffe_exp] Initializing rffe\n");

    smio_err_e err = SMIO_SUCCESS;

    self->id = RFFE_DEVID;
    self->name = strdup (RFFE_NAME);
    ASSERT_ALLOC(self->name, err_name_alloc, SMIO_SUCCESS);

    /* Set SMIO ops pointers */
    self->ops = &rffe_ops;
    self->thsafe_client_ops = &smio_thsafe_client_zmq_ops;

    /* disp_op_t structure is const and all of the functions performing on it
     * obviously receives a const argument, but here (and only on the SMIO
     * initialization) we need to make an exception if we want to keep the
     * functions' description and the function pointers separate */
    err = smio_init_exp_ops (self, (disp_op_t **) rffe_exp_ops, rffe_exp_fp);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not fill SMIO "
            "function descriptors with the callbacks", err_fill_desc);

    self->exp_ops = rffe_exp_ops;

    /* Initialize specific structure */
    self->smio_handler = smio_rffe_new (self);
    ASSERT_ALLOC(self->smio_handler, err_smio_handler_alloc, SMIO_SUCCESS);

    return err;

err_smio_handler_alloc:
err_fill_desc:
    free (self->name);
err_name_alloc:
    return err;
}

/* Destroy sm_io instance of rffe */
smio_err_e rffe_shutdown (smio_t *self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:rffe_exp] Shutting down rffe\n");

    smio_rffe_destroy ((smio_rffe_t **)&self->smio_handler);
    self->exp_ops = NULL;
    self->thsafe_client_ops = NULL;
    self->ops = NULL;
    free (self->name);

    return SMIO_SUCCESS;
}

const smio_bootstrap_ops_t rffe_bootstrap_ops = {
    .init = rffe_init,
    .shutdown = rffe_shutdown,
    .config_defaults = rffe_config_defaults
};
