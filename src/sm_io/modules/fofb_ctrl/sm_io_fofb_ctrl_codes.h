/*
 * Copyright (C) 2021 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _SM_IO_FOFB_CTRL_CODES_H_
#define _SM_IO_FOFB_CTRL_CODES_H_

/* Messaging OPCODES */
#define FOFB_CTRL_OPCODE_TYPE                     uint32_t
#define FOFB_CTRL_OPCODE_SIZE                     (sizeof (FOFB_CTRL_OPCODE_TYPE))

/* Regular registers */
#define FOFB_CTRL_OPCODE_SET_GET_ACT_PART                       0
#define FOFB_CTRL_NAME_SET_GET_ACT_PART                         "fofb_ctrl_set_get_act_part"
#define FOFB_CTRL_OPCODE_SET_GET_ERR_CLR                        1
#define FOFB_CTRL_NAME_SET_GET_ERR_CLR                          "fofb_ctrl_set_get_err_clr"
#define FOFB_CTRL_OPCODE_SET_GET_CC_ENABLE                      2
#define FOFB_CTRL_NAME_SET_GET_CC_ENABLE                        "fofb_ctrl_set_get_cc_enable"
#define FOFB_CTRL_OPCODE_SET_GET_TFS_OVERRIDE                   3
#define FOFB_CTRL_NAME_SET_GET_TFS_OVERRIDE                     "fofb_ctrl_set_get_tfs_override"

/* RAM-based register */
#define FOFB_CTRL_OPCODE_SET_GET_BPM_ID                         200
#define FOFB_CTRL_NAME_SET_GET_BPM_ID                           "fofb_ctrl_set_get_bpm_id"
#define FOFB_CTRL_OPCODE_SET_GET_TIME_FRAME_LEN                 201
#define FOFB_CTRL_NAME_SET_GET_TIME_FRAME_LEN                   "fofb_ctrl_set_get_time_frame_len"
#define FOFB_CTRL_OPCODE_SET_GET_MGT_POWERDOWN                  202
#define FOFB_CTRL_NAME_SET_GET_MGT_POWERDOWN                    "fofb_ctrl_set_get_mgt_powerdown"
#define FOFB_CTRL_OPCODE_SET_GET_MGT_LOOPBACK                   203
#define FOFB_CTRL_NAME_SET_GET_MGT_LOOPBACK                     "fofb_ctrl_set_get_mgt_loopback"
#define FOFB_CTRL_OPCODE_SET_GET_TIME_FRAME_DLY                 204
#define FOFB_CTRL_NAME_SET_GET_TIME_FRAME_DLY                   "fofb_ctrl_set_get_time_frame_dly"
#define FOFB_CTRL_OPCODE_SET_GET_GOLDEN_ORB_X                   205
#define FOFB_CTRL_NAME_SET_GET_GOLDEN_ORB_X                     "fofb_ctrl_set_get_golden_orb_x"
#define FOFB_CTRL_OPCODE_SET_GET_GOLDEN_ORB_Y                   206
#define FOFB_CTRL_NAME_SET_GET_GOLDEN_ORB_Y                     "fofb_ctrl_set_get_golden_orb_y"
#define FOFB_CTRL_OPCODE_SET_GET_CUST_FEATURE                   207
#define FOFB_CTRL_NAME_SET_GET_CUST_FEATURE                     "fofb_ctrl_set_get_cust_feature"
#define FOFB_CTRL_OPCODE_SET_GET_RX_POLARITY                    208
#define FOFB_CTRL_NAME_SET_GET_RX_POLARITY                      "fofb_ctrl_set_get_rx_polarity"
#define FOFB_CTRL_OPCODE_SET_GET_PAYLOADSEL                     209
#define FOFB_CTRL_NAME_SET_GET_PAYLOADSEL                       "fofb_ctrl_set_get_payloadsel"
#define FOFB_CTRL_OPCODE_SET_GET_FOFBDATASEL                    210
#define FOFB_CTRL_NAME_SET_GET_FOFBDATASEL                      "fofb_ctrl_set_get_fofbdatasel"
#define FOFB_CTRL_OPCODE_SET_GET_FIRMWARE_VER                   211
#define FOFB_CTRL_NAME_SET_GET_FIRMWARE_VER                     "fofb_ctrl_set_get_firmware_ver"
#define FOFB_CTRL_OPCODE_SET_GET_SYS_STATUS                     212
#define FOFB_CTRL_NAME_SET_GET_SYS_STATUS                       "fofb_ctrl_set_get_sys_status"
#define FOFB_CTRL_OPCODE_SET_GET_LINK_PARTNER                   213
#define FOFB_CTRL_NAME_SET_GET_LINK_PARTNER                     "fofb_ctrl_set_get_link_partner"
#define FOFB_CTRL_OPCODE_SET_GET_LINK_UP                        217
#define FOFB_CTRL_NAME_SET_GET_LINK_UP                          "fofb_ctrl_set_get_link_up"
#define FOFB_CTRL_OPCODE_SET_GET_TIME_FRAME_COUNT               218
#define FOFB_CTRL_NAME_SET_GET_TIME_FRAME_COUNT                 "fofb_ctrl_set_get_time_frame_count"
#define FOFB_CTRL_OPCODE_SET_GET_HARD_ERR_CNT                   219
#define FOFB_CTRL_NAME_SET_GET_HARD_ERR_CNT                     "fofb_ctrl_set_get_hard_err_cnt"
#define FOFB_CTRL_OPCODE_SET_GET_SOFT_ERR_CNT                   223
#define FOFB_CTRL_NAME_SET_GET_SOFT_ERR_CNT                     "fofb_ctrl_set_get_soft_err_cnt"
#define FOFB_CTRL_OPCODE_SET_GET_FRAME_ERR_CNT                  227
#define FOFB_CTRL_NAME_SET_GET_FRAME_ERR_CNT                    "fofb_ctrl_set_get_frame_err_cnt"
#define FOFB_CTRL_OPCODE_SET_GET_RX_PCK_CNT                     235
#define FOFB_CTRL_NAME_SET_GET_RX_PCK_CNT                       "fofb_ctrl_set_get_rx_pck_cnt"
#define FOFB_CTRL_OPCODE_SET_GET_TX_PCK_CNT                     239
#define FOFB_CTRL_NAME_SET_GET_TX_PCK_CNT                       "fofb_ctrl_set_get_tx_pck_cnt"
#define FOFB_CTRL_OPCODE_SET_GET_FOD_PROCESS_TIME               243
#define FOFB_CTRL_NAME_SET_GET_FOD_PROCESS_TIME                 "fofb_ctrl_set_get_fod_process_time"
#define FOFB_CTRL_OPCODE_SET_GET_BPM_COUNT                      244
#define FOFB_CTRL_NAME_SET_GET_BPM_COUNT                        "fofb_ctrl_set_get_bpm_count"
#define FOFB_CTRL_OPCODE_SET_GET_BPM_ID_RDBACK                  245
#define FOFB_CTRL_NAME_SET_GET_BPM_ID_RDBACK                    "fofb_ctrl_set_get_bpm_id_rdback"
#define FOFB_CTRL_OPCODE_SET_GET_TF_LENGTH_RDBACK               246
#define FOFB_CTRL_NAME_SET_GET_TF_LENGTH_RDBACK                 "fofb_ctrl_set_get_tf_length_rdback"
#define FOFB_CTRL_OPCODE_SET_GET_POWERDOWN_RDBACK               247
#define FOFB_CTRL_NAME_SET_GET_POWERDOWN_RDBACK                 "fofb_ctrl_set_get_powerdown_rdback"
#define FOFB_CTRL_OPCODE_SET_GET_LOOPBACK_RDBACK                248
#define FOFB_CTRL_NAME_SET_GET_LOOPBACK_RDBACK                  "fofb_ctrl_set_get_loopback_rdback"
#define FOFB_CTRL_OPCODE_SET_GET_FAIVAL_RDBACK                  249
#define FOFB_CTRL_NAME_SET_GET_FAIVAL_RDBACK                    "fofb_ctrl_set_get_faival_rdback"
#define FOFB_CTRL_OPCODE_END                                    250

/* Messaging Reply OPCODES */
#define FOFB_CTRL_REPLY_TYPE                      uint32_t
#define FOFB_CTRL_REPLY_SIZE                      (sizeof (FOFB_CTRL_REPLY_TYPE))

#define FOFB_CTRL_OK                              0   /* Operation was successful */
#define FOFB_CTRL_ERR                             1   /* Generic error */

#endif
