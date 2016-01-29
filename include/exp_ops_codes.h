/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _EXP_OPS_CODES_H_
#define _EXP_OPS_CODES_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Messaging OPCODES */
#define EXP_OPS_OPCODE_SIZE          (sizeof (uint32_t))
#define EXP_OPS_OPCODE_TYPE          uint32_t

/* Messaging Reply OPCODES */
#define EXP_OPS_REPLY_SIZE           (sizeof(uint32_t))
#define EXP_OPS_REPLY_TYPE           uint32_t

#define EXP_OPS_REPLY_OK             0
#define EXP_OPS_REPLY_ERR            1

/* Messaging Return Codes */
#define EXP_OPS_RETURN_SIZE           (sizeof(int32_t))
#define EXP_OPS_RETURN_TYPE           int32_t

#ifdef __cplusplus
}
#endif

#endif
