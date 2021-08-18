/*
 * Copyright (C) 2021 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _SM_IO_FOFB_CTRL_EXPORTS_H_
#define _SM_IO_FOFB_CTRL_EXPORTS_H_

#include "disptable.h"

extern disp_op_t fofb_ctrl_set_get_act_part_exp;
extern disp_op_t fofb_ctrl_set_get_err_clr_exp;
extern disp_op_t fofb_ctrl_set_get_cc_enable_exp;
extern disp_op_t fofb_ctrl_set_get_tfs_override_exp;
extern disp_op_t fofb_ctrl_set_get_bpm_id_exp;
extern disp_op_t fofb_ctrl_set_get_time_frame_len_exp;
extern disp_op_t fofb_ctrl_set_get_mgt_powerdown_exp;
extern disp_op_t fofb_ctrl_set_get_mgt_loopback_exp;
extern disp_op_t fofb_ctrl_set_get_time_frame_dly_exp;
extern disp_op_t fofb_ctrl_set_get_golden_orb_x_exp;
extern disp_op_t fofb_ctrl_set_get_golden_orb_y_exp;
extern disp_op_t fofb_ctrl_set_get_cust_feature_exp;
extern disp_op_t fofb_ctrl_set_get_rx_polarity_exp;
extern disp_op_t fofb_ctrl_set_get_payloadsel_exp;
extern disp_op_t fofb_ctrl_set_get_fofbdatasel_exp;
extern disp_op_t fofb_ctrl_set_get_firmware_ver_exp;
extern disp_op_t fofb_ctrl_set_get_sys_status_exp;
extern disp_op_t fofb_ctrl_set_get_link_partner_exp;
extern disp_op_t fofb_ctrl_set_get_link_up_exp;
extern disp_op_t fofb_ctrl_set_get_time_frame_count_exp;
extern disp_op_t fofb_ctrl_set_get_hard_err_cnt_exp;
extern disp_op_t fofb_ctrl_set_get_soft_err_cnt_exp;
extern disp_op_t fofb_ctrl_set_get_frame_err_cnt_exp;
extern disp_op_t fofb_ctrl_set_get_rx_pck_cnt_exp;
extern disp_op_t fofb_ctrl_set_get_tx_pck_cnt_exp;
extern disp_op_t fofb_ctrl_set_get_fod_process_time_exp;
extern disp_op_t fofb_ctrl_set_get_bpm_count_exp;
extern disp_op_t fofb_ctrl_set_get_bpm_id_rdback_exp;
extern disp_op_t fofb_ctrl_set_get_tf_length_rdback_exp;
extern disp_op_t fofb_ctrl_set_get_powerdown_rdback_exp;
extern disp_op_t fofb_ctrl_set_get_loopback_rdback_exp;
extern disp_op_t fofb_ctrl_set_get_faival_rdback_exp;
extern disp_op_t fofb_ctrl_set_get_toa_rd_en_exp;
extern disp_op_t fofb_ctrl_set_get_toa_rd_str_exp;
extern disp_op_t fofb_ctrl_set_get_toa_data_exp;
extern disp_op_t fofb_ctrl_set_get_rcb_rd_en_exp;
extern disp_op_t fofb_ctrl_set_get_rcb_rd_str_exp;
extern disp_op_t fofb_ctrl_set_get_rcb_data_exp;

extern const disp_op_t *fofb_ctrl_exp_ops [];

#endif
