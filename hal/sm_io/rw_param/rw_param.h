/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include <inttypes.h>
#include <mdp.h>
#include "hal_varg.h"
#include "rw_param_codes.h"

#ifndef _RW_PARAM_H_
#define _RW_PARAM_H_

/* User parameter check function pointer. Called in SET functions to check the
 * parameter */
typedef int (*rw_param_check_fp) (uint32_t param);

/* User parameter format fucntion pointer. Called in GET fucntions to
 * format the read parameter to a specific output */
typedef int (*rw_param_format_fp) (uint32_t *param);

#define SINGLE_BIT_PARAM            1
#define MULT_BIT_PARAM              0

#define BIT_SET 					1
#define BIT_CLR	  	     			0

#define NO_CHK_FUNC                 NULL
#define NO_FMT_FUNC                 NULL

#define CLR_FIELD 					1
#define SET_FIELD  	     			0

#define EXPAND_FIELD_NE(field) WHENNOT(ISEMPTY(field))(_ ## field)
#define EXPAND_CHECK_LIM_NE(min, max) WHENNOT(ISEMPTY(min))                     \
    (WHENNOT(ISEMPTY(max)) ((check_param_limits(value, min, max) == PARAM_OK) &&))

/************************** MACRO concatenation *******************************/
/* Call CONCAT_NAMEX_ version to avoid expanding macro definitions
 * Call CONCAT_NAMEX version to expand macro definitions */

#define CONCAT_NAME2_(reg, field)                                               \
    CAT2(reg, EXPAND_FIELD_NE(field))
#define CONCAT_NAME2(reg, field) CONCAT_NAME2_(reg, field)

#define CONCAT_NAME3_(prefix, reg, field)                                       \
    CAT4(prefix, _, reg, EXPAND_FIELD_NE(field))
#define CONCAT_NAME3(prefix, reg, field) CONCAT_NAME3_(prefix, reg, field)

#define CONCAT_NAME4_(prefix1, prefix2, reg, field)                             \
    CAT6(prefix1, _, prefix2, _, reg, EXPAND_FIELD_NE(field))
#define CONCAT_NAME4(prefix1, prefix2, reg, field)                              \
    CONCAT_NAME4_(prefix1, prefix2, reg, field)

/* For generating the macros prefix ## _ ## reg ## _ ## field ## _R(_value) or
 * prefix ## _ ## reg ## _ ## field ## _W(_value) */
#define CONCAT_NAME4_RW_(prefix, reg, field, rw_suffix)                         \
    CAT6(prefix, _, reg, EXPAND_FIELD_NE(field), _, rw_suffix)
#define CONCAT_NAME4_RW(prefix, reg, field, rw_suffix)                          \
    CONCAT_NAME4_RW_(prefix, reg, field, rw_suffix)

#define CONCAT_NAME5_(prefix1, prefix2, prefix3, reg, field)                    \
    CAT8(prefix1, _, prefix2, _, prefix3, _, reg, EXPAND_FIELD_NE(field))
#define CONCAT_NAME5(prefix1, prefix2, prefix3, reg, field)                     \
    CONCAT_NAME5_(prefix1, prefix2, prefix3, reg, field)

/******************* RW MACRO function header/body definitions ****************/
#define RW_PARAM_FUNC_NAME(module, reg)                                         \
        _##module##_set_get_##reg

#define RW_PARAM_FUNC(module, reg)                                              \
        static void *RW_PARAM_FUNC_NAME(module, reg) (void *owner, void *args)

#define GET_PARAM_GEN(smio, module, base_addr, prefix, reg, field, single_bit, var, \
        fmt_funcp, read_32_fp)                                                  \
    ({                                                                          \
        RW_REPLY_TYPE err = RW_OK;                                         \
        uint32_t __value;                                                       \
        uint32_t addr = base_addr | CONCAT_NAME3(prefix, REG, reg);             \
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:rw_param:"#module"] "     \
				"GET_PARAM_" #reg "_" #field ": reading from address 0x%08x\n", \
                smio->base | addr);                                             \
        ssize_t __ret = ((thsafe_client_read_32_fp) read_32_fp)(smio, addr,     \
            &__value);                                                          \
                                                                                \
        if (__ret != sizeof(uint32_t)) {                                        \
            DBE_DEBUG (DBG_SM_IO | DBG_LVL_ERR, "[sm_io:rw_param:"#module"] "   \
                "SET_PARAM_" #reg "_" #field ": Number of bytes read (%ld)\n"   \
                "does not match the request (%lu)\n", __ret, sizeof(uint32_t)); \
            err = RW_READ_EAGAIN;                                               \
        }                                                                       \
                                                                                \
        __value = WHEN(single_bit)(  										    \
                    (__value & CONCAT_NAME3(prefix, reg, field)) ?              \
                    BIT_SET : BIT_CLR                                           \
                )                                                               \
                WHENNOT(single_bit)(                                            \
                     CONCAT_NAME4_RW(prefix, reg, field, R(__value))            \
                )                                                               \
        ;                                                                       \
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:rw_param:"#module"] "     \
				"GET_PARAM_" #reg "_" #field " = 0x%08x\n", __value);           \
        if (fmt_funcp != NULL) {                                                \
            ((rw_param_format_fp) fmt_funcp) (&__value);                        \
        }                                                                       \
        var = __value;                                                          \
        err;                                                                    \
     })

#define GET_PARAM(smio, module, base_addr, prefix, reg, field, single_bit, var, \
        fmt_funcp)                                                              \
            GET_PARAM_GEN(smio, module, base_addr, prefix, reg, field, single_bit, var, \
               fmt_funcp, smio_thsafe_client_read_32)

/* SET or CLEAR parameter based on the last macro parameter "clr_field" */
#define SET_PARAM_GEN(smio, module, base_addr, prefix, reg, field, single_bit, value, \
        min, max, chk_funcp, clr_field, read_32_fp, write_32_fp)                \
	({                                                                          \
        RW_REPLY_TYPE err = RW_OK;                                        \
        uint32_t addr = base_addr | CONCAT_NAME3(prefix, REG, reg);             \
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:rw_param:"#module"] "     \
				"SET_PARAM_" #reg "_" #field ": writing 0x%08x to address 0x%08x\n", \
                value, smio->base | addr);                                      \
		if (EXPAND_CHECK_LIM_NE(min, max)                                       \
            ((chk_funcp == NULL) || ((rw_param_check_fp) chk_funcp) (value) == PARAM_OK)) { \
            uint32_t __write_value;                                             \
            ssize_t __ret = ((thsafe_client_read_32_fp) read_32_fp)(smio,       \
                addr, &__write_value);                                          \
                                                                                \
            if (__ret != sizeof(uint32_t)) {                                    \
                DBE_DEBUG (DBG_SM_IO | DBG_LVL_ERR, "[sm_io:rw_param:"#module"] " \
                    "SET_PARAM_" #reg "_" #field ": Number of bytes read (%ld)\n" \
                    "does not match the request (%lu)\n", __ret, sizeof(uint32_t)); \
                err = RW_WRITE_EAGAIN;                                          \
            }                                                                   \
                                                                                \
            __write_value =                                                     \
                    WHEN(single_bit)(                                           \
                        WHENNOT(clr_field)(                                     \
                           (value) ? (__write_value | CONCAT_NAME3(prefix, reg, field)) : \
                           (__write_value & ~ CONCAT_NAME3(prefix, reg, field))   \
                        )                                                       \
                        WHEN(clr_field)(                                        \
                            (__write_value ~ CONCAT_NAME3(prefix, reg, field))) \
                    )                                                           \
                    WHENNOT(single_bit)(                                        \
                        (__write_value & ~ CONCAT_NAME4_RW(prefix, reg, field, MASK)) \
                        WHENNOT(clr_field)(                                     \
                            | CONCAT_NAME4_RW(prefix, reg, field, W(value))     \
                        )                                                       \
                    )                                                           \
            ;                                                                   \
            __ret = ((thsafe_client_write_32_fp) write_32_fp)(smio, addr,       \
                &__write_value);                                                \
                                                                                \
            if (__ret != sizeof(uint32_t)) {                                    \
                DBE_DEBUG (DBG_SM_IO | DBG_LVL_ERR, "[sm_io:rw_param:"#module"] " \
                        "SET_PARAM_" #reg "_" #field ": Number of bytes written (%ld)\n" \
                        "does not match the request (%lu)\n", __ret, sizeof(uint32_t)); \
                err = RW_WRITE_EAGAIN;                                          \
            }                                                                   \
            else {                                                              \
                DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:rw_param:"#module"] " \
                        "SET_PARAM_" #reg "_" #field ": updated 0x%08x to address 0x%08x\n", \
                        __write_value, smio->base | addr);                      \
            }                                                                   \
        }                                                                       \
        else {                                                                  \
            DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:rw_param:"#module"] " \
                    "SET_PARAM_" #reg "_" #field ": invalid parameter: 0x%08x\n", \
                    value);                                                     \
            err = RW_USR_ERR;                                                   \
        }                                                                       \
        err;                                                                    \
	})

#define SET_PARAM(smio, module, base_addr, prefix, reg, field, single_bit, value, \
        min, max, chk_funcp, clr_field)                                         \
            SET_PARAM_GEN(smio, module, base_addr, prefix, reg, field, single_bit, value, \
                min, max, chk_funcp, clr_field, smio_thsafe_client_read_32,     \
                    smio_thsafe_client_write_32)

/* zmq message in SET_GET_PARAM macro is:
 * frame 0: operation code
 * frame 1: rw		R /W	1 = read mode, 0 = write mode
 * frame 2: value to be written (only when rw = 0)
 * */
#define SET_GET_PARAM_GEN(module, base_addr, prefix, reg, field, single_bit, min,   \
        max, chk_funcp, fmt_funcp, clr_field, read_32_fp, write_32_fp)          \
	do {                                                                        \
		assert (owner);                                                         \
		assert (args);                                                          \
		DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:rw_param:"#module"] "     \
				"Calling SET_GET_PARAM_"#reg"\n");                              \
		smio_t *smio = (smio_t *) owner;                                        \
		exp_msg_zmq_t *exp_msg = (exp_msg_zmq_t *) args;                        \
		assert (zmsg_size (*exp_msg->msg) >= 1);                                \
        zframe_t *rw_frame = zmsg_pop (*exp_msg->msg);                          \
		uint32_t rw = *(uint32_t *) zframe_data (rw_frame);                     \
        zframe_destroy (&rw_frame);                                             \
		DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:rw_param:"#module"] "     \
				"SET_GET_PARAM_"#reg": rw = %u\n", rw);                         \
		if (rw)	{                                                               \
            uint32_t value;                                                     \
            RW_REPLY_TYPE set_param_return = GET_PARAM_GEN(smio, module, base_addr, \
                    prefix, reg, field, single_bit, value, fmt_funcp, read_32_fp); \
            rw_param_send_client_response (set_param_return, value, true,       \
                    smio->worker, exp_msg->reply_to);                           \
        }                                                                       \
        else {                                                                  \
            zframe_t *value_frame = zmsg_pop (*exp_msg->msg);                   \
            uint32_t value = *(uint32_t *)                                      \
                             zframe_data (value_frame);                         \
            zframe_destroy (&value_frame);                                      \
            RW_REPLY_TYPE set_param_return = SET_PARAM_GEN(smio, module, base_addr, \
                    prefix, reg, field, single_bit, value, min, max, chk_funcp, \
                    clr_field, read_32_fp, write_32_fp);                        \
            rw_param_send_client_response (set_param_return, 0, false,          \
                    smio->worker, exp_msg->reply_to);                           \
        }                                                                       \
		zmsg_destroy (exp_msg->msg);                                            \
		return NULL;                                                            \
	} while (0)

#define SET_GET_PARAM(module, base_addr, prefix, reg, field, single_bit, min,   \
        max, chk_funcp, fmt_funcp, clr_field)                                   \
            SET_GET_PARAM_GEN(module, base_addr, prefix, reg, field, single_bit, min,   \
                max, chk_funcp, fmt_funcp, clr_field, smio_thsafe_client_read_32, \
                    smio_thsafe_client_write_32)

uint32_t check_param_limits (uint32_t value, uint32_t min, uint32_t max);
void rw_param_send_client_response (RW_REPLY_TYPE reply_code,
        uint32_t data_out, bool with_data_frame, mdp_worker_t *worker,
        zframe_t *reply_to);
void send_client_response (RW_REPLY_TYPE reply_code, uint32_t reply_size,
        uint32_t *data_out, bool with_data_frame, mdp_worker_t *worker,
        zframe_t *reply_to);

#endif

