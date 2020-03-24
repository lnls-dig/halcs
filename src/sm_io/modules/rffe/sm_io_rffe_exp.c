/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "halcs_server.h"
/* Private headers */
#include "sm_io_rffe_codes.h"
#include "sm_io_rffe_defaults.h"
#include "sm_io_rffe_exports.h"
#include "sm_io_rffe_core.h"
#include "sm_io_rffe_exp.h"

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
    smio_rffe_t *rffe = smio_get_handler (self);
    ASSERT_TEST(rffe != NULL, "Could not get SMIO RFFE handler",
            err_get_rffe_handler, -RFFE_ERR);
    smch_rffe_t *smch_rffe = SMIO_CTL_HANDLER(rffe);
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

err_get_rffe_handler:
    return err;
}

RFFE_FUNC_NAME_HEADER(att)
{
    return _rffe_var_rw(owner, args, ret, smch_rffe_read_var,
            smch_rffe_write_var, RFFE_OPCODE_SET_GET_ATT,
            "Could not set/get RFFE attenuator");
}

RFFE_FUNC_NAME_HEADER(temp_ac)
{
    return _rffe_var_rw(owner, args, ret, smch_rffe_read_var,
            smch_rffe_write_var, RFFE_OPCODE_SET_GET_TEMP_AC,
            "Could not set/get RFFE temperature AC");
}

RFFE_FUNC_NAME_HEADER(temp_bd)
{
    return _rffe_var_rw(owner, args, ret, smch_rffe_read_var,
            smch_rffe_write_var, RFFE_OPCODE_SET_GET_TEMP_BD,
            "Could not set/get RFFE temperature BD");
}

RFFE_FUNC_NAME_HEADER(set_point_ac)
{
    return _rffe_var_rw(owner, args, ret, smch_rffe_read_var,
            smch_rffe_write_var, RFFE_OPCODE_SET_GET_SET_POINT_AC,
            "Could not set/get RFFE set point AC");
}

RFFE_FUNC_NAME_HEADER(set_point_bd)
{
    return _rffe_var_rw(owner, args, ret, smch_rffe_read_var,
            smch_rffe_write_var, RFFE_OPCODE_SET_GET_SET_POINT_BD,
            "Could not set/get RFFE set point BD");
}

RFFE_FUNC_NAME_HEADER(temp_control)
{
    return _rffe_var_rw(owner, args, ret, smch_rffe_read_var,
            smch_rffe_write_var, RFFE_OPCODE_SET_GET_TEMP_CONTROL,
            "Could not set/get RFFE temperature control");
}

RFFE_FUNC_NAME_HEADER(heater_ac)
{
    return _rffe_var_rw(owner, args, ret, smch_rffe_read_var,
            smch_rffe_write_var, RFFE_OPCODE_SET_GET_HEATER_AC,
            "Could not set/get RFFE heater AC");
}


RFFE_FUNC_NAME_HEADER(heater_bd)
{
    return _rffe_var_rw(owner, args, ret, smch_rffe_read_var,
            smch_rffe_write_var, RFFE_OPCODE_SET_GET_HEATER_BD,
            "Could not set/get RFFE output BD");
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


RFFE_FUNC_NAME_HEADER(pid_ac_kp)
{
    return _rffe_var_rw(owner, args, ret, smch_rffe_read_var,
            smch_rffe_write_var, RFFE_OPCODE_SET_GET_PID_AC_KP,
            "Could not set/get RFFE AC PID KP parameter");
}

RFFE_FUNC_NAME_HEADER(pid_ac_ti)
{
    return _rffe_var_rw(owner, args, ret, smch_rffe_read_var,
            smch_rffe_write_var, RFFE_OPCODE_SET_GET_PID_AC_TI,
            "Could not set/get RFFE AC PID TI parameter");
}

RFFE_FUNC_NAME_HEADER(pid_ac_td)
{
    return _rffe_var_rw(owner, args, ret, smch_rffe_read_var,
            smch_rffe_write_var, RFFE_OPCODE_SET_GET_PID_AC_TD,
            "Could not set/get RFFE AC PID TD parameter");
}

RFFE_FUNC_NAME_HEADER(pid_bd_kp)
{
    return _rffe_var_rw(owner, args, ret, smch_rffe_read_var,
            smch_rffe_write_var, RFFE_OPCODE_SET_GET_PID_BD_KP,
            "Could not set/get RFFE BD PID KP parameter");
}

RFFE_FUNC_NAME_HEADER(pid_bd_ti)
{
    return _rffe_var_rw(owner, args, ret, smch_rffe_read_var,
            smch_rffe_write_var, RFFE_OPCODE_SET_GET_PID_BD_TI,
            "Could not set/get RFFE BD PID TI parameter");
}

RFFE_FUNC_NAME_HEADER(pid_bd_td)
{
    return _rffe_var_rw(owner, args, ret, smch_rffe_read_var,
            smch_rffe_write_var, RFFE_OPCODE_SET_GET_PID_BD_TD,
            "Could not set/get RFFE BD PID TD parameter");
}

/* Exported function pointers */
const disp_table_func_fp rffe_exp_fp [] = {
    RFFE_FUNC_NAME(att),
    RFFE_FUNC_NAME(temp_ac),
    RFFE_FUNC_NAME(temp_bd),
    RFFE_FUNC_NAME(set_point_ac),
    RFFE_FUNC_NAME(set_point_bd),
    RFFE_FUNC_NAME(temp_control),
    RFFE_FUNC_NAME(heater_ac),
    RFFE_FUNC_NAME(heater_bd),
    RFFE_FUNC_NAME(reset),
    RFFE_FUNC_NAME(reprog),
    RFFE_FUNC_NAME(data),
    RFFE_FUNC_NAME(version),
    RFFE_FUNC_NAME(pid_ac_kp),
    RFFE_FUNC_NAME(pid_ac_ti),
    RFFE_FUNC_NAME(pid_ac_td),
    RFFE_FUNC_NAME(pid_bd_kp),
    RFFE_FUNC_NAME(pid_bd_ti),
    RFFE_FUNC_NAME(pid_bd_td),
    NULL
};

/************************************************************/
/***************** Export methods functions *****************/
/************************************************************/

static smio_err_e _rffe_do_op (void *owner, void *msg);

/* Attach an instance of sm_io to dev_io function pointer */
smio_err_e rffe_attach (smio_t *self, void *args)
{
    UNUSED(self);
    UNUSED(args);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Deattach an instance of sm_io to dev_io function pointer */
smio_err_e rffe_deattach (smio_t *self)
{
    UNUSED(self);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Export (register) sm_io to handle operations function pointer */
smio_err_e rffe_export_ops (smio_t *self,
        const disp_op_t **smio_exp_ops)
{
    UNUSED(self);
    UNUSED(smio_exp_ops);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Unexport (unregister) sm_io to handle operations function pointer */
smio_err_e rffe_unexport_ops (smio_t *self)
{
    UNUSED(self);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

smio_err_e _rffe_do_op (void *owner, void *msg)
{
    UNUSED(owner);
    UNUSED(msg);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

smio_err_e rffe_do_op (void *self, void *msg)
{
    return _rffe_do_op (self, msg);
}

const smio_ops_t rffe_ops = {
    .attach             = &rffe_attach,          /* Attach sm_io instance to dev_io */
    .deattach           = &rffe_deattach,        /* Deattach sm_io instance to dev_io */
    .export_ops         = &rffe_export_ops,      /* Export sm_io operations to dev_io */
    .unexport_ops       = &rffe_unexport_ops,    /* Unexport sm_io operations to dev_io */
    .do_op              = &rffe_do_op            /* Generic wrapper for handling specific operations */
};

/************************************************************/
/****************** Bootstrap Operations ********************/
/************************************************************/

smio_err_e rffe_init (smio_t * self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:rffe_exp] Initializing rffe\n");

    smio_err_e err = SMIO_SUCCESS;

    err = smio_set_id (self, RFFE_DEVID);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO id", err_set_id);
    err = smio_set_name (self, RFFE_NAME);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO name", err_set_name);

    /* Set SMIO ops pointers */
    err = smio_set_ops (self, &rffe_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO operations",
            err_smio_set_ops);
    err = smio_set_thsafe_client_ops (self, &smio_thsafe_client_zmq_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO thsafe operations",
            err_smio_set_thsafe_ops);

    /* disp_op_t structure is const and all of the functions performing on it
     * obviously receives a const argument, but here (and only on the SMIO
     * initialization) we need to make an exception if we want to keep the
     * functions' description and the function pointers separate */
    err = smio_init_exp_ops (self, (disp_op_t **) rffe_exp_ops, rffe_exp_fp);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not fill SMIO "
            "function descriptors with the callbacks", err_fill_desc);

    err = smio_set_exp_ops (self, rffe_exp_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO exported operations",
            err_smio_set_exp_ops);

    /* Initialize specific structure */
    smio_rffe_t *smio_handler = smio_rffe_new (self);
    ASSERT_ALLOC(smio_handler, err_smio_handler_alloc, SMIO_ERR_ALLOC);
    err = smio_set_handler (self, smio_handler);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO handler",
            err_smio_set_handler);

    return err;

err_smio_set_handler:
    smio_rffe_destroy (&smio_handler);
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

/* Destroy sm_io instance of rffe */
smio_err_e rffe_shutdown (smio_t *self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:rffe_exp] Shutting down rffe\n");

    smio_err_e err = SMIO_SUCCESS;
    smio_rffe_t *rffe = smio_get_handler (self);
    ASSERT_TEST(rffe != NULL, "Could not get RFFE handler",
            err_rffe_handler, SMIO_ERR_ALLOC /* FIXME: improve return code */);

    /* Destroy SMIO instance */
    smio_rffe_destroy (&rffe);
    /* Nullify operation pointers */
    smio_set_exp_ops (self, NULL);
    smio_set_thsafe_client_ops (self, NULL);
    smio_set_ops (self, NULL);

err_rffe_handler:
    return err;
}

const smio_bootstrap_ops_t rffe_bootstrap_ops = {
    .init            = &rffe_init,
    .shutdown        = &rffe_shutdown,
    .config_defaults = &rffe_config_defaults
};

SMIO_MOD_DECLARE(RFFE_DEVID, RFFE_NAME, rffe_bootstrap_ops)
