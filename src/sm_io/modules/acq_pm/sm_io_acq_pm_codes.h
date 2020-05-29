/*
 * Copyright (C) 2017 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _SM_IO_ACQ_PM_CODES_H_
#define _SM_IO_ACQ_PM_CODES_H_

/* Messaging OPCODES */
#define ACQ_PM_OPCODE_TYPE                  uint32_t
#define ACQ_PM_OPCODE_SIZE                  (sizeof (ACQ_PM_OPCODE_TYPE))

#define ACQ_PM_OPCODE_DUMMY                0
#define ACQ_PM_NAME_DUMMY                  "acq_pm_dummy"
#define ACQ_PM_OPCODE_END                  1

/* Messaging Reply OPCODES */
#define ACQ_PM_REPLY_TYPE                  uint32_t
#define ACQ_PM_REPLY_SIZE                  (sizeof (ACQ_PM_REPLY_TYPE))

#define ACQ_PM_OK                          0   /* Operation was successful */
#define ACQ_PM_ERR                         1   /* Generic error */
#define ACQ_PM_REPLY_END                   2   /* End marker */

#endif
