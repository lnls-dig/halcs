/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _EXP_MSG_H_
#define _EXP_MSG_H_

/* We add more message types here */
enum _exp_msg_type_e {
    EXP_MSG_NOT_SUPPORTED = 0xFFFF,
    EXP_MSG_ZMQ = 0
};

typedef enum _exp_msg_type_e exp_msg_type_e;

exp_msg_type_e exp_msg_guess_type (void *msg);

#endif
