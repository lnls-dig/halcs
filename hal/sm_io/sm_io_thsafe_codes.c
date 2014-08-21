/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include <sm_io_thsafe_codes.h>

const uint32_t thsafe_opcodes [THSAFE_OPCODE_END] = {
    [0]  = THSAFE_OPCODE_OPEN,
    [1]  = THSAFE_OPCODE_RELEASE,
    [2]  = THSAFE_OPCODE_READ_16,
    [3]  = THSAFE_OPCODE_READ_32,
    [4]  = THSAFE_OPCODE_READ_64,
    [5]  = THSAFE_OPCODE_WRITE_16,
    [6]  = THSAFE_OPCODE_WRITE_32,
    [7]  = THSAFE_OPCODE_WRITE_64,
    [8]  = THSAFE_OPCODE_READ_BLOCK,
    [9]  = THSAFE_OPCODE_WRITE_BLOCK,
    [10] = THSAFE_OPCODE_READ_DMA,
    [11] = THSAFE_OPCODE_WRITE_DMA
//    [12] = THSAFE_OPCODE_READ_INFO
};

const uint32_t thsafe_reply_opcodes [THSAFE_REPLY_END] = {
    [0] = THSAFE_OK,
    [1] = THSAFE_ERR
};

