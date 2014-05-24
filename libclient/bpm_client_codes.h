/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _BPM_CLIENT_CODES_H_
#define _BPM_CLIENT_CODES_H_

#include <inttypes.h>

/* Messaging OPCODES */
#define BPM_CLIENT_OPCODE_LEDS_SIZE        (sizeof (uint32_t))
#define BPM_CLIENT_OPCODE_TYPE             uint32_t

#define BPM_CLIENT_OPCODE_LEDS             0
#define BPM_CLIENT_OPCODE_END              1

typedef BPM_CLIENT_OPCODE_TYPE bpm_client_opcode_t;

extern const bpm_client_opcode_t bpm_client_opcodes [BPM_CLIENT_OPCODE_END];

/* Messaging Reply CODES */
#define BPM_CLIENT_RECODE_SIZE              (sizeof(uint32_t))
#define BPM_CLIENT_RECODE_TYPE              uint32_t

#define BPM_CLIENT_RECODE_OK               0
#define BPM_CLIENT_RECODE_ERR              1
#define BPM_CLIENT_RECODE_END              2

typedef BPM_CLIENT_RECODE_TYPE bpm_client_recode_t;

extern const bpm_client_recode_t bpm_client_recodes [BPM_CLIENT_RECODE_END];

#endif
