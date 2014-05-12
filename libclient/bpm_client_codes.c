/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include "bpm_client_codes.h"

const BPM_CLI_OPCODE_TYPE cli_opcodes [BPM_CLI_OPCODE_END] = {
    [0]  = BPM_CLI_OPCODE_LEDS,
};

const BPM_CLI_RECODE_TYPE cli_recodes [BPM_CLI_RECODE_END] = {
    [0] = BPM_CLI_RECODE_OK,
    [1] = BPM_CLI_RECODE_ERR
};

