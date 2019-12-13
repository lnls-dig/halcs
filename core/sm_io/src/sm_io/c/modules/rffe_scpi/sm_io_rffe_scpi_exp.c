/****************************************************************************
 * core/sm_io/src/sm_io/c/modules/rffe_scpi/sm_io_rffe_scpi_exp.c
 *
 *   Copyright (C) 2014 LNLS (www.lnls.br). All rights reserved.
 *   Copyright (C) 2019 Augusto Fraga Giachero. All rights reserved.
 *   Author: Augusto Fraga Giachero <afg@augustofg.net>
 *
 * This file is part of HALCS.
 *
 * HALCS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * HALCS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with HALCS. If not, see <https://www.gnu.org/licenses/>.
 *
 ****************************************************************************/

#include "halcs_server.h"
/* Private headers */
#include "sm_io_rffe_scpi_defaults.h"
#include "sm_io_rffe_scpi_exports.h"
#include "sm_io_rffe_scpi_core.h"
#include "sm_io_rffe_scpi_exp.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, SM_IO, "[sm_io:rffe_scpi_exp]",            \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)           \
    ASSERT_HAL_ALLOC(ptr, SM_IO, "[sm_io:rffe_scpi_exp]",                    \
            smio_err_str(SMIO_ERR_ALLOC),                               \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                        \
    CHECK_HAL_ERR(err, SM_IO, "[sm_io:rffe_scpi_exp]",                       \
            smio_err_str (err_type))

/************************************************************/
/*****************  Specific RFFE Operations *****************/
/************************************************************/

/* Macros to avoid repetition of the function body */
typedef smch_err_e (*smch_rffe_scpi_func_fp) (smch_rffe_scpi_t *self, uint32_t id,
        uint8_t *data, size_t size);

#define RFFE_SCPI_FUNC_NAME(func_name)                                       \
    _rffe_scpi_set_get_ ## func_name

#define RFFE_SCPI_FUNC_NAME_HEADER(func_name)                                \
    static int RFFE_SCPI_FUNC_NAME(func_name) (void *owner, void *args, void *ret)

enum scpi_req_type {
    scpi_double,
    scpi_int,
    scpi_bool,
    scpi_none,
};

static int _rffe_scpi_var_rw (void *owner, void *args, const char* req,
       enum scpi_req_type req_type, const char *error_msg, void* ret)
{
    assert (owner);
    assert (args);
    UNUSED(error_msg);

    int err = -RFFE_OK;
    SMIO_OWNER_TYPE *self = SMIO_EXP_OWNER(owner);
    smio_rffe_scpi_t *rffe = smio_get_handler (self);
    ASSERT_TEST(rffe != NULL, "Could not get SMIO RFFE handler",
            err_get_rffe_scpi_handler, -RFFE_ERR);
    smch_rffe_scpi_t *smch_rffe = SMIO_CTL_HANDLER(rffe);
    uint32_t rw = *(uint32_t *) EXP_MSG_ZMQ_FIRST_ARG(args);

    EXP_MSG_ZMQ_ARG_TYPE param_zmq = EXP_MSG_ZMQ_PEEK_NEXT_ARG(args);
    void *param = EXP_MSG_ZMQ_ARG_DATA(param_zmq);

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:rffe_scpi_exp] Making "
            "RFFE scpi request %s\n", req);

    /* Call specific function */

    if (rw) {
        char ans[128];
        smch_scpi_write_line(smch_rffe, req);
        smch_scpi_read_line(smch_rffe, ans, sizeof(ans));

        switch (req_type) {
        case scpi_double:
            sscanf(ans, "%lf", (double*)ret);
            err = 8;
            break;

        case scpi_int:
        case scpi_bool:
            sscanf(ans, "%d", (int*)ret);
            err = 4;
            break;

        default:
            break;
        }
    }
    else {
        char req_str[128];
        switch (req_type) {
        case scpi_double:
            snprintf(req_str, sizeof(req_str), "%s %lf", req, *((double *) param));
            smch_scpi_write_line(smch_rffe, req_str);
            break;

        case scpi_int:
        case scpi_bool:
            snprintf(req_str, sizeof(req_str), "%s %lf", req, *((double *) param));
            smch_scpi_write_line(smch_rffe, req_str);
            break;

        case scpi_none:
            smch_scpi_write_line(smch_rffe, req);
            break;

        default:
            break;
        }
    }


err_get_rffe_scpi_handler:
    return err;
}

RFFE_SCPI_FUNC_NAME_HEADER(att)
{
    UNUSED(owner);
    uint32_t rw = *(uint32_t *) EXP_MSG_ZMQ_FIRST_ARG(args);
    int res = 0;

    if (rw) {
        res = _rffe_scpi_var_rw (owner, args, "GET:ATTEnuation?", scpi_double,
                            "Could not get RFFE attenuator", ret);
    }
    else {
        res = _rffe_scpi_var_rw (owner, args, "SET:ATTEnuation", scpi_double,
                            "Could not set RFFE attenuator", ret);
    }

    return res;
}

RFFE_SCPI_FUNC_NAME_HEADER(temp_ac)
{
    UNUSED(owner);
    uint32_t rw = *(uint32_t *) EXP_MSG_ZMQ_FIRST_ARG(args);
    int res = 0;

    if (rw) {
        res = _rffe_scpi_var_rw (owner, args, "MEASure:TEMPerature:AC?", scpi_double,
                            "Could not get RFFE temperature AC", ret);
    }
    else {
        res = -1;
    }

    return res;
}

RFFE_SCPI_FUNC_NAME_HEADER(temp_bd)
{
    UNUSED(owner);
    uint32_t rw = *(uint32_t *) EXP_MSG_ZMQ_FIRST_ARG(args);
    int res = 0;

    if (rw) {
        res = _rffe_scpi_var_rw (owner, args, "MEASure:TEMPerature:BD?", scpi_double,
                            "Could not get RFFE temperature BD", ret);
    }
    else {
        res = -1;
    }

    return res;
}

RFFE_SCPI_FUNC_NAME_HEADER(set_point_ac)
{
    UNUSED(owner);
    uint32_t rw = *(uint32_t *) EXP_MSG_ZMQ_FIRST_ARG(args);
    int res = 0;

    if (rw) {
        res = _rffe_scpi_var_rw (owner, args, "GET:TEMPerature:SETPoint:AC?", scpi_double,
                            "Could not get RFFE set point AC", ret);
    }
    else {
        res = _rffe_scpi_var_rw (owner, args, "SET:TEMPerature:SETPoint:AC", scpi_double,
                            "Could not set RFFE set point AC", ret);
    }

    return res;
}

RFFE_SCPI_FUNC_NAME_HEADER(set_point_bd)
{
    UNUSED(owner);
    uint32_t rw = *(uint32_t *) EXP_MSG_ZMQ_FIRST_ARG(args);
    int res = 0;

    if (rw) {
        res = _rffe_scpi_var_rw (owner, args, "GET:TEMPerature:SETPoint:BD?", scpi_double,
                            "Could not get RFFE set point BD", ret);
    }
    else {
        res = _rffe_scpi_var_rw (owner, args, "SET:TEMPerature:SETPoint:BD", scpi_double,
                            "Could not set RFFE set point BD", ret);
    }

    return res;
}

RFFE_SCPI_FUNC_NAME_HEADER(temp_control)
{
    UNUSED(owner);
    uint32_t rw = *(uint32_t *) EXP_MSG_ZMQ_FIRST_ARG(args);
    int res = 0;

    if (rw) {
        res = _rffe_scpi_var_rw (owner, args, "GET:TEMPControl:AUTOmatic?", scpi_bool,
                            "Could not get RFFE temperature control", ret);
    }
    else {
        res = _rffe_scpi_var_rw (owner, args, "SET:TEMPControl:AUTOmatic", scpi_bool,
                            "Could not set RFFE temperature control", ret);
    }

    return res;
}

RFFE_SCPI_FUNC_NAME_HEADER(heater_ac)
{
    UNUSED(owner);
    uint32_t rw = *(uint32_t *) EXP_MSG_ZMQ_FIRST_ARG(args);
    int res = 0;

    if (rw) {
        res = _rffe_scpi_var_rw (owner, args, "GET:DAC:OUTput:AC?", scpi_double,
                            "Could not get RFFE heater AC", ret);
    }
    else {
        res = _rffe_scpi_var_rw (owner, args, "SET:DAC:OUTput:AC", scpi_double,
                            "Could not set RFFE heater AC", ret);
    }

    return res;
}


RFFE_SCPI_FUNC_NAME_HEADER(heater_bd)
{
    UNUSED(owner);
    uint32_t rw = *(uint32_t *) EXP_MSG_ZMQ_FIRST_ARG(args);
    int res = 0;

    if (rw) {
        res = _rffe_scpi_var_rw (owner, args, "GET:DAC:OUTput:BD?", scpi_double,
                            "Could not get RFFE heater BD", ret);
    }
    else {
        res = _rffe_scpi_var_rw (owner, args, "SET:DAC:OUTput:BD", scpi_double,
                            "Could not set RFFE heater BD", ret);
    }

    return res;
}

RFFE_SCPI_FUNC_NAME_HEADER(reset)
{
    UNUSED(owner);
    uint32_t rw = *(uint32_t *) EXP_MSG_ZMQ_FIRST_ARG(args);
    int res = 0;

    if (rw) {
        /*
         *
         */
        res = -1;
    }
    else {
        res = _rffe_scpi_var_rw (owner, args, "SYStem:RESet", scpi_none,
                            "Could not set RFFE heater BD", ret);
    }

    return res;
}

RFFE_SCPI_FUNC_NAME_HEADER(reprog)
{
    UNUSED(owner);
    UNUSED(args);
    UNUSED(ret);

    /*
     * Not available through scpi
     */
    return -RFFE_UNINPL;
}

RFFE_SCPI_FUNC_NAME_HEADER(data)
{
    UNUSED(owner);
    UNUSED(args);
    UNUSED(ret);

    /*
     * Not implemented
     */
    return -RFFE_UNINPL;
}

RFFE_SCPI_FUNC_NAME_HEADER(version)
{
    UNUSED(owner);
    UNUSED(args);
    UNUSED(ret);

    /*
     * Not implemented yet
     */
    return -RFFE_UNINPL;
}


RFFE_SCPI_FUNC_NAME_HEADER(pid_ac_kp)
{
    UNUSED(owner);
    uint32_t rw = *(uint32_t *) EXP_MSG_ZMQ_FIRST_ARG(args);
    int res = 0;

    if (rw) {
        res = _rffe_scpi_var_rw (owner, args, "GET:PID:Kc:AC?", scpi_double,
                            "Could not get RFFE AC PID KP parameter", ret);
    }
    else {
        res = _rffe_scpi_var_rw (owner, args, "SET:PID:Kc:AC", scpi_double,
                            "Could not set RFFE AC PID KP parameter", ret);
    }

    return res;
}

RFFE_SCPI_FUNC_NAME_HEADER(pid_ac_ti)
{
    UNUSED(owner);
    uint32_t rw = *(uint32_t *) EXP_MSG_ZMQ_FIRST_ARG(args);
    int res = 0;

    if (rw) {
        res = _rffe_scpi_var_rw (owner, args, "GET:PID:Ti:AC?", scpi_double,
                            "Could not get RFFE AC PID TI parameter", ret);
    }
    else {
        res = _rffe_scpi_var_rw (owner, args, "SET:PID:Ti:AC", scpi_double,
                            "Could not set RFFE AC PID TI parameter", ret);
    }

    return res;
}

RFFE_SCPI_FUNC_NAME_HEADER(pid_ac_td)
{
    UNUSED(owner);
    uint32_t rw = *(uint32_t *) EXP_MSG_ZMQ_FIRST_ARG(args);
    int res = 0;

    if (rw) {
        res = _rffe_scpi_var_rw (owner, args, "GET:PID:Td:AC?", scpi_double,
                            "Could not get RFFE AC PID TD parameter", ret);
    }
    else {
        res = _rffe_scpi_var_rw (owner, args, "SET:PID:Td:AC", scpi_double,
                            "Could not set RFFE AC PID TD parameter", ret);
    }

    return res;
}

RFFE_SCPI_FUNC_NAME_HEADER(pid_bd_kp)
{
    UNUSED(owner);
    uint32_t rw = *(uint32_t *) EXP_MSG_ZMQ_FIRST_ARG(args);
    int res = 0;

    if (rw) {
        res = _rffe_scpi_var_rw (owner, args, "GET:PID:Kc:BD?", scpi_double,
                            "Could not get RFFE BD PID KP parameter", ret);
    }
    else {
        res = _rffe_scpi_var_rw (owner, args, "SET:PID:Kc:BD", scpi_double,
                            "Could not set RFFE BD PID KP parameter", ret);
    }

    return res;
}

RFFE_SCPI_FUNC_NAME_HEADER(pid_bd_ti)
{
    UNUSED(owner);
    uint32_t rw = *(uint32_t *) EXP_MSG_ZMQ_FIRST_ARG(args);
    int res = 0;

    if (rw) {
        res = _rffe_scpi_var_rw (owner, args, "GET:PID:Ti:BD?", scpi_double,
                            "Could not get RFFE BD PID TI parameter", ret);
    }
    else {
        res = _rffe_scpi_var_rw (owner, args, "SET:PID:Ti:BD", scpi_double,
                            "Could not set RFFE BD PID TI parameter", ret);
    }

    return res;
}

RFFE_SCPI_FUNC_NAME_HEADER(pid_bd_td)
{
    UNUSED(owner);
    uint32_t rw = *(uint32_t *) EXP_MSG_ZMQ_FIRST_ARG(args);
    int res = 0;

    if (rw) {
        res = _rffe_scpi_var_rw (owner, args, "GET:PID:Ti:BD?", scpi_double,
                            "Could not get RFFE BD PID TI parameter", ret);
    }
    else {
        res = _rffe_scpi_var_rw (owner, args, "SET:PID:Ti:BD", scpi_double,
                            "Could not set RFFE BD PID TI parameter", ret);
    }

    return res;
}

/* Exported function pointers */
const disp_table_func_fp rffe_scpi_exp_fp [] = {
    RFFE_SCPI_FUNC_NAME(att),
    RFFE_SCPI_FUNC_NAME(temp_ac),
    RFFE_SCPI_FUNC_NAME(temp_bd),
    RFFE_SCPI_FUNC_NAME(set_point_ac),
    RFFE_SCPI_FUNC_NAME(set_point_bd),
    RFFE_SCPI_FUNC_NAME(temp_control),
    RFFE_SCPI_FUNC_NAME(heater_ac),
    RFFE_SCPI_FUNC_NAME(heater_bd),
    RFFE_SCPI_FUNC_NAME(reset),
    RFFE_SCPI_FUNC_NAME(reprog),
    RFFE_SCPI_FUNC_NAME(data),
    RFFE_SCPI_FUNC_NAME(version),
    RFFE_SCPI_FUNC_NAME(pid_ac_kp),
    RFFE_SCPI_FUNC_NAME(pid_ac_ti),
    RFFE_SCPI_FUNC_NAME(pid_ac_td),
    RFFE_SCPI_FUNC_NAME(pid_bd_kp),
    RFFE_SCPI_FUNC_NAME(pid_bd_ti),
    RFFE_SCPI_FUNC_NAME(pid_bd_td),
    NULL
};

/************************************************************/
/***************** Export methods functions *****************/
/************************************************************/

static smio_err_e _rffe_scpi_do_op (void *owner, void *msg);

/* Attach an instance of sm_io to dev_io function pointer */
smio_err_e rffe_scpi_attach (smio_t *self, void *args)
{
    UNUSED(self);
    UNUSED(args);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Deattach an instance of sm_io to dev_io function pointer */
smio_err_e rffe_scpi_deattach (smio_t *self)
{
    UNUSED(self);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Export (register) sm_io to handle operations function pointer */
smio_err_e rffe_scpi_export_ops (smio_t *self,
        const disp_op_t **smio_exp_ops)
{
    UNUSED(self);
    UNUSED(smio_exp_ops);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Unexport (unregister) sm_io to handle operations function pointer */
smio_err_e rffe_scpi_unexport_ops (smio_t *self)
{
    UNUSED(self);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

smio_err_e _rffe_scpi_do_op (void *owner, void *msg)
{
    UNUSED(owner);
    UNUSED(msg);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

smio_err_e rffe_scpi_do_op (void *self, void *msg)
{
    return _rffe_scpi_do_op (self, msg);
}

const smio_ops_t rffe_scpi_ops = {
    .attach             = &rffe_scpi_attach,          /* Attach sm_io instance to dev_io */
    .deattach           = &rffe_scpi_deattach,        /* Deattach sm_io instance to dev_io */
    .export_ops         = &rffe_scpi_export_ops,      /* Export sm_io operations to dev_io */
    .unexport_ops       = &rffe_scpi_unexport_ops,    /* Unexport sm_io operations to dev_io */
    .do_op              = &rffe_scpi_do_op            /* Generic wrapper for handling specific operations */
};

/************************************************************/
/****************** Bootstrap Operations ********************/
/************************************************************/

smio_err_e rffe_scpi_init (smio_t * self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:rffe_scpi_exp] Initializing rffe\n");

    smio_err_e err = SMIO_SUCCESS;

    err = smio_set_id (self, RFFE_SCPI_DEVID);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO id", err_set_id);
    err = smio_set_name (self, RFFE_SCPI_NAME);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO name", err_set_name);

    /* Set SMIO ops pointers */
    err = smio_set_ops (self, &rffe_scpi_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO operations",
            err_smio_set_ops);
    err = smio_set_thsafe_client_ops (self, &smio_thsafe_client_zmq_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO thsafe operations",
            err_smio_set_thsafe_ops);

    /* disp_op_t structure is const and all of the functions performing on it
     * obviously receives a const argument, but here (and only on the SMIO
     * initialization) we need to make an exception if we want to keep the
     * functions' description and the function pointers separate */
    err = smio_init_exp_ops (self, (disp_op_t **) rffe_scpi_exp_ops, rffe_scpi_exp_fp);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not fill SMIO "
            "function descriptors with the callbacks", err_fill_desc);

    err = smio_set_exp_ops (self, rffe_scpi_exp_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO exported operations",
            err_smio_set_exp_ops);

    /* Initialize specific structure */
    smio_rffe_scpi_t *smio_handler = smio_rffe_scpi_new (self);
    ASSERT_ALLOC(smio_handler, err_smio_handler_alloc, SMIO_ERR_ALLOC);
    err = smio_set_handler (self, smio_handler);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO handler",
            err_smio_set_handler);

    return err;

err_smio_set_handler:
    smio_rffe_scpi_destroy (&smio_handler);
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
smio_err_e rffe_scpi_shutdown (smio_t *self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:rffe_scpi_exp] Shutting down rffe\n");

    smio_err_e err = SMIO_SUCCESS;
    smio_rffe_scpi_t *rffe = smio_get_handler (self);
    ASSERT_TEST(rffe != NULL, "Could not get RFFE handler",
            err_rffe_scpi_handler, SMIO_ERR_ALLOC /* FIXME: improve return code */);

    /* Destroy SMIO instance */
    smio_rffe_scpi_destroy (&rffe);
    /* Nullify operation pointers */
    smio_set_exp_ops (self, NULL);
    smio_set_thsafe_client_ops (self, NULL);
    smio_set_ops (self, NULL);

err_rffe_scpi_handler:
    return err;
}

const smio_bootstrap_ops_t rffe_scpi_bootstrap_ops = {
    .init            = &rffe_scpi_init,
    .shutdown        = &rffe_scpi_shutdown,
    .config_defaults = &rffe_scpi_config_defaults
};

SMIO_MOD_DECLARE(RFFE_SCPI_DEVID, RFFE_SCPI_NAME, rffe_scpi_bootstrap_ops)
