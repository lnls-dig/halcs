/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _MSG_H_
#define _MSG_H_

#ifdef __cplusplus
extern "C" {
#endif

#define MSG_OPCODE_SIZE                     (sizeof (uint32_t))
/* Arbitrary number*/
#define MSG_OPCODE_MAX                      200

/* We add more message types here */
typedef enum {
    MSG_NOT_SUPPORTED = 0xFFFF,
    MSG_EXP_ZMQ = 0,
    MSG_THSAFE_ZMQ
} msg_type_e;

/************************************************************/
/***************************** Our API **********************/
/************************************************************/

msg_type_e msg_guess_type (void *msg);
msg_err_e msg_validate (void *msg, msg_type_e expected_msg_type);

/* Utility function for helping other classes in implementing their
 * message checking */
msg_err_e msg_check_gen_zmq_args (const disp_op_t *disp_op, zmsg_t *zmq_msg);

/* Handle MLM protocol (used by SMIOs, for instance) request */
msg_err_e msg_handle_mlm_request (void *owner, void *args,
        disp_table_t *disp_table);
/* Handle regular protocol (used by DEVIOs, for instance) request */
msg_err_e msg_handle_sock_request (void *owner, void *args,
        disp_table_t *disp_table);

#ifdef __cplusplus
}
#endif

#endif
