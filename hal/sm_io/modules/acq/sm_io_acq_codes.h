/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _SM_IO_ACQ_CODES_H_
#define _SM_IO_ACQ_CODES_H_

#include <inttypes.h>

/* Messaging Reply OPCODES */
#define ACQ_REPLY_SIZE           (sizeof(uint32_t))
#define ACQ_REPLY_TYPE           uint32_t

#define ACQ_REQ_OK              0
#define ACQ_REQ_ERR             1

#define ACQ_CHECK_OK            2
#define ACQ_CHECK_ERR           3

#define ACQ_BLOCK_OK            4
#define ACQ_BLOCK_ERR           5

#define ACQ_REPLY_END           6

extern const ACQ_REPLY_TYPE acq_reply_ops [ACQ_REPLY_END];

#endif
