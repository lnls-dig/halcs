/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include <inttypes.h>
#include <mdp.h>
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

/********************** Detecting NULL passed to MACRO ************************/
/* based on from "Detect empty macro arguments" from Jens Gustedt, published
 * June 8, 2010 on https://gustedt.wordpress.com/2010/06/08/detect-empty-macro-arguments/
 */
#define _ARG16(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, ...) _15
#define HAS_COMMA(...) _ARG16(__VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0)
/* following macro that just transforms the following parenthesis and contents into a comma */
#define _TRIGGER_PARENTHESIS_(...) ,

#define ISEMPTY(...)                                                    \
_ISEMPTY(                                                               \
          /* test if there is just one argument, eventually an empty    \
             one */                                                     \
          HAS_COMMA(__VA_ARGS__),                                       \
          /* test if _TRIGGER_PARENTHESIS_ together with the argument   \
             adds a comma */                                            \
          HAS_COMMA(_TRIGGER_PARENTHESIS_ __VA_ARGS__),                 \
          /* test if the argument together with a parenthesis           \
             adds a comma */                                            \
          HAS_COMMA(__VA_ARGS__ (/*empty*/)),                           \
          /* test if placing it between _TRIGGER_PARENTHESIS_ and the   \
             parenthesis adds a comma */                                \
          HAS_COMMA(_TRIGGER_PARENTHESIS_ __VA_ARGS__ (/*empty*/))      \
          )

#define PASTE5(_0, _1, _2, _3, _4) _0 ## _1 ## _2 ## _3 ## _4
#define _ISEMPTY(_0, _1, _2, _3) HAS_COMMA(PASTE5(_IS_EMPTY_CASE_, _0, _1, _2, _3))
#define _IS_EMPTY_CASE_0001 ,

/**************** MACROS to "eat" caracter if parameter is empty **************/
/* from user pfultz2, published in github repository Cloak
 * https://github.com/pfultz2/Cloak/wiki/C-Preprocessor-tricks,-tips,-and-idioms*/

/* Simple MACRO concatenation */
#define CAT(a, ...) PRIMITIVE_CAT(a  __VA_ARGS__)
#define PRIMITIVE_CAT(a, ...) a ## __VA_ARGS__

#define CAT4(a, b, c, ...) PRIMITIVE_CAT4(a, b, c, __VA_ARGS__)
#define PRIMITIVE_CAT4(a, b, c, ...) a ## b ## c ## __VA_ARGS__

#define CAT6(a, b, c, d, e, ...) PRIMITIVE_CAT6(a, b, c, d, e,  __VA_ARGS__)
#define PRIMITIVE_CAT6(a, b, c, d, e, ...) a ## b ## c ## d ## e ## __VA_ARGS__

#define CAT8(a, b, c, d, e, f, g, ...) PRIMITIVE_CAT8(a, b, c, d, e, f, g, __VA_ARGS__)
#define PRIMITIVE_CAT8(a, b, c, d, e, f, g, ...) a ## b ## c ## d ## e ## f ## g ## __VA_ARGS__

/* Simple IF macro that returns the parameter passed itself, when expanded
 * to IFF_1 and the other parameters if it's expanded to IFF_0.
 * This is the basis for a generalized IF MACRO */
#define IIF(c) PRIMITIVE_CAT(IIF_, c)
#define IIF_0(t, ...) __VA_ARGS__
#define IIF_1(t, ...) t

/* Complement of 0 or 1 */
#define COMPL(b) PRIMITIVE_CAT(COMPL_, b)
#define COMPL_0 1
#define COMPL_1 0

/* Core macros for detecting patterns */
#define CHECK_N(x, n, ...) n
#define CHECK(...) CHECK_N(__VA_ARGS__, 0,)
#define PROBE(x) x, 1,

/* NOT(x) always expands to 0, except when the parameter passed is 0 */
#define NOT(x) CHECK(PRIMITIVE_CAT(NOT_, x))
#define NOT_0 PROBE(~)

/* Generalized IF MACRO */
#define BOOL(x) COMPL(NOT(x))
#define IF(c) IIF(BOOL(c))

/* Finally, the conditonal expansion MACRO, that only expands if the condition
 * is true. Otherwise it eats the parameter */
#define EAT(...)
#define EXPAND(...) __VA_ARGS__
#define WHEN(c) IF(c)(EXPAND, EAT)
#define WHENNOT(c) IF(c)(EAT, EXPAND)

#define EXPAND_FIELD_NE(field) WHENNOT(ISEMPTY(field))(_ ## field)

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

/* TODO: add error checking for reading */
#define GET_PARAM(module, base_addr, prefix, reg, field, single_bit, fmt_funcp) \
    ({                                                                          \
        uint32_t value;                                                         \
        uint32_t addr = BAR4_ADDR | base_addr | CONCAT_NAME3(prefix, REG, reg); \
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:"#module"] "              \
				"GET_PARAM_" #reg "_" #field ": reading from address 0x%08x\n", addr); \
		smio_thsafe_client_read_32 (self, addr, &value);                        \
        value = WHEN(single_bit)(  										        \
                    (value & CONCAT_NAME3(prefix, reg, field)) ?                \
                    BIT_SET : BIT_CLR                                           \
                )                                                               \
                WHENNOT(single_bit)(                                            \
                     CONCAT_NAME4_RW(prefix, reg, field, R(value))              \
                )                                                               \
        ;                                                                       \
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:"#module"] "              \
				"GET_PARAM_" #reg "_" #field " = 0x%08x\n", value);             \
        if (fmt_funcp != NULL) {                                                \
            ((rw_param_format_fp) fmt_funcp) (&value);                          \
        }                                                                       \
        value;                                                                  \
     })

/* SET or CLEAR parameter based on the last macro parameter "clr_field" */
#define SET_PARAM(module, base_addr, prefix, reg, field, single_bit, value, min, max, chk_funcp, clr_field) \
	({                                                                          \
        RW_REPLY_TYPE set_param_return;                                         \
        uint32_t addr = BAR4_ADDR | base_addr | CONCAT_NAME3(prefix, REG, reg); \
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:"#module"] "              \
				"SET_PARAM_" #reg "_" #field ": writing 0x%08x to address 0x%08x\n", \
                value, addr);                                                   \
		if ((check_param_limits(value, min, max) == PARAM_OK) &&                \
            ((chk_funcp == NULL) || ((rw_param_check_fp) chk_funcp) (value) == PARAM_OK)) { \
            uint32_t write_value;                                               \
            smio_thsafe_client_read_32 (self, addr, &write_value); 	            \
            write_value =                                                       \
                    WHEN(single_bit)(                                           \
                        WHENNOT(clr_field)(                                     \
                           (value) ? (write_value | CONCAT_NAME3(prefix, reg, field)) : \
                           (write_value & ~ CONCAT_NAME3(prefix, reg, field))   \
                        )                                                       \
                        WHEN(clr_field)(                                        \
                            (write_value ~ CONCAT_NAME3(prefix, reg, field)))   \
                    )                                                           \
                    WHENNOT(single_bit)(                                        \
                        (write_value & ~ CONCAT_NAME4_RW(prefix, reg, field, MASK)) \
                        WHENNOT(clr_field)(                                     \
                            | CONCAT_NAME4_RW(prefix, reg, field, W(value))     \
                        )                                                       \
                    )                                                           \
            ;                                                                   \
            smio_thsafe_client_write_32 (self,  addr, &write_value);            \
            DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:"#module"] "          \
                    "SET_PARAM_" #reg "_" #field ": updated 0x%08x to address 0x%08x\n", \
                    write_value, addr);                                         \
            set_param_return = RW_WRITE_OK;                                     \
        }                                                                       \
        else {                                                                  \
            DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:"#module"] "          \
                    "SET_PARAM_" #reg "_" #field ": invalid parameter: 0x%08x\n", \
                    value);                                                     \
            set_param_return = RW_USR_ERR;                                      \
        }                                                                       \
        set_param_return;                                                       \
	})

/* zmq message in SET_GET_PARAM macro is:
 * frame 0: operation code
 * frame 1: rw		R /W	1 = read mode, 0 = write mode
 * frame 2: value to be written (only when rw = 0)
 * */
#define SET_GET_PARAM(module, base_addr, prefix, reg, field, single_bit, min,   \
        max, chk_funcp, fmt_funcp, clr_field)                                   \
	do {                                                                        \
		assert (owner);                                                         \
		assert (args);                                                          \
		DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:"#module"] "              \
				"Calling SET_GET_PARAM_"#reg"\n");                              \
		smio_t *self = (smio_t *) owner;                                        \
		exp_msg_zmq_t *exp_msg = (exp_msg_zmq_t *) args;                        \
		assert (zmsg_size (*exp_msg->msg) >= 1);                                \
        zframe_t *rw_frame = zmsg_pop (*exp_msg->msg);                          \
		uint32_t rw = *(uint32_t *) zframe_data (rw_frame);                     \
        zframe_destroy (&rw_frame);                                             \
		DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:"#module"] "              \
				"SET_GET_PARAM_"#reg": rw = %u\n", rw);                         \
		if (rw)	{                                                               \
            uint32_t value = GET_PARAM(module, base_addr, prefix, reg, field,   \
                    single_bit, fmt_funcp);                                     \
            rw_param_send_client_response (RW_READ_OK, value, true,             \
                    self->worker, exp_msg->reply_to);                           \
        }                                                                       \
        else {                                                                  \
            zframe_t *value_frame = zmsg_pop (*exp_msg->msg);                   \
            uint32_t value = *(uint32_t *)                                      \
                             zframe_data (value_frame);                         \
            zframe_destroy (&value_frame);                                      \
            RW_REPLY_TYPE set_param_return = SET_PARAM(module, base_addr,       \
                    prefix, reg, field, single_bit, value, min, max, chk_funcp, clr_field); \
            rw_param_send_client_response (set_param_return, 0, false,          \
                    self->worker, exp_msg->reply_to);                           \
        }                                                                       \
		zmsg_destroy (exp_msg->msg);                                            \
		return NULL;                                                            \
	} while (0)

uint32_t check_param_limits (uint32_t value, uint32_t min, uint32_t max);
void rw_param_send_client_response ( RW_REPLY_TYPE reply_code,
			uint32_t data_out, bool with_data_frame,
			mdp_worker_t *worker, zframe_t *reply_to);

#endif

