/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include "bpm_client_codes.h"

const BPM_CLIENT_OPCODE_TYPE client_opcodes [BPM_CLIENT_OPCODE_END] = {
    [0]  = BPM_CLIENT_OPCODE_LEDS,
};

const BPM_CLIENT_RECODE_TYPE client_recodes [BPM_CLIENT_RECODE_END] = {
    [0] = BPM_CLIENT_RECODE_OK,
    [1] = BPM_CLIENT_RECODE_ERR
};

