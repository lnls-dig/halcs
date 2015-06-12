/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _RW_PARAM_CODES_
#define _RW_PARAM_CODES_

/* Messaging Reply OPCODES */
#define RW_REPLY_SIZE                  (sizeof(uint32_t))
#define RW_REPLY_TYPE                  uint32_t

#define RW_OK                           0   /* Read operation was successful */
#define RW_OOR                          1   /* Parameter is out of range */
#define RW_USR_ERR                      2   /* Parameter unmet user conditions */
#define RW_INV                          3   /* Parameter is invalid */
#define RW_READ_EAGAIN                  4   /* Read operation was incomplete */
#define RW_WRITE_EAGAIN                 5   /* Write operation was incomplete */
#define RW_REPLY_END                    6   /* End marker */

#define PARAM_OK                        0
#define PARAM_ERR                       1

#endif
