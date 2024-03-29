/*
 *  * Copyright (C) 2020 LNLS (www.lnls.br)
 *   * Author: Melissa Aguiar <melissa.aguiar@cnpem.br>
 *    *
 *     * Released according to the GNU GPL, version 3 or any later version.
 *      */

#ifndef _SM_IO_FOFB_PROCESSING_CODES_H_
#define _SM_IO_FOFB_PROCESSING_CODES_H_

/* NOTE: This value should be FOFB_PROCESSING_REGS_COEFFS_RAM_BANK_SIZE/sizeof(uint32_t),
 *       where FOFB_PROCESSING_REGS_COEFFS_RAM_BANK_SIZE is defined on hw/wb_fofb_processing_regs.h
 */
#define FOFB_PROCESSING_DATA_BLOCK_MAX_PARAMS                   512
struct _smio_fofb_processing_data_block_t {
    uint32_t data[FOFB_PROCESSING_DATA_BLOCK_MAX_PARAMS];
};

/* Messaging OPCODES */
#define FOFB_PROCESSING_OPCODE_TYPE                             uint32_t
#define FOFB_PROCESSING_OPCODE_SIZE                             (sizeof (FOFB_PROCESSING_OPCODE_TYPE))

#define FOFB_PROCESSING_OPCODE_SET_GET_COEFFS_FIXED_POINT_POS    0
#define FOFB_PROCESSING_NAME_SET_GET_COEFFS_FIXED_POINT_POS      "fofb_processing_set_get_coeffs_fixed_point_pos"
#define FOFB_PROCESSING_OPCODE_SET_GET_ACC_GAINS_FIXED_POINT_POS 1
#define FOFB_PROCESSING_NAME_SET_GET_ACC_GAINS_FIXED_POINT_POS   "fofb_processing_set_get_acc_gains_fixed_point_pos"
#define FOFB_PROCESSING_OPCODE_SET_GET_ACC_GAIN                  2
#define FOFB_PROCESSING_NAME_SET_GET_ACC_GAIN                    "fofb_processing_set_get_acc_gain"
#define FOFB_PROCESSING_OPCODE_SET_GET_ACC_CTL_CLEAR             3
#define FOFB_PROCESSING_NAME_SET_GET_ACC_CTL_CLEAR               "fofb_processing_set_get_acc_ctl_clear"
#define FOFB_PROCESSING_OPCODE_SET_GET_ACC_CTL_FREEZE            4
#define FOFB_PROCESSING_NAME_SET_GET_ACC_CTL_FREEZE              "fofb_processing_set_get_acc_ctl_freeze"
#define FOFB_PROCESSING_OPCODE_SET_GET_SP_MAX                    5
#define FOFB_PROCESSING_NAME_SET_GET_SP_MAX                      "_fofb_processing_set_get_sp_max"
#define FOFB_PROCESSING_OPCODE_SET_GET_SP_MIN                    6
#define FOFB_PROCESSING_NAME_SET_GET_SP_MIN                      "_fofb_processing_set_get_sp_min"
#define FOFB_PROCESSING_OPCODE_SET_GET_LOOP_INTLK_ORB_EN         7
#define FOFB_PROCESSING_NAME_SET_GET_LOOP_INTLK_ORB_EN           "_fofb_processing_set_get_intlk_orb_en"
#define FOFB_PROCESSING_OPCODE_SET_GET_LOOP_INTLK_PACKET_EN      8
#define FOFB_PROCESSING_NAME_SET_GET_LOOP_INTLK_PACKET_EN        "_fofb_processing_set_get_intlk_packet_en"
#define FOFB_PROCESSING_OPCODE_SET_GET_LOOP_INTLK_CTL_CLR        9
#define FOFB_PROCESSING_NAME_SET_GET_LOOP_INTLK_CTL_CLR          "_fofb_processing_set_get_intlk_clr"
#define FOFB_PROCESSING_OPCODE_SET_GET_LOOP_INTLK_STA            10
#define FOFB_PROCESSING_NAME_SET_GET_LOOP_INTLK_STA              "_fofb_processing_set_get_intlk_sta"
#define FOFB_PROCESSING_OPCODE_SET_GET_LOOP_ORB_DISTORT_LIMIT    11
#define FOFB_PROCESSING_NAME_SET_GET_LOOP_ORB_DISTORT_LIMIT      "_fofb_processing_set_get_loop_orb_distort_limit"
#define FOFB_PROCESSING_OPCODE_SET_GET_MIN_NUM_PKTS              12
#define FOFB_PROCESSING_NAME_SET_GET_MIN_NUM_PKTS                "_fofb_processing_set_get_min_num_pkts"
#define FOFB_PROCESSING_OPCODE_COEFFS_RAM_BANK_READ              13
#define FOFB_PROCESSING_NAME_COEFFS_RAM_BANK_READ                "_fofb_processing_coeffs_ram_bank_read"
#define FOFB_PROCESSING_OPCODE_COEFFS_RAM_BANK_WRITE             14
#define FOFB_PROCESSING_NAME_COEFFS_RAM_BANK_WRITE               "_fofb_processing_coeffs_ram_bank_write"
#define FOFB_PROCESSING_OPCODE_SETPOINTS_RAM_BANK_READ           15
#define FOFB_PROCESSING_NAME_SETPOINTS_RAM_BANK_READ             "_fofb_processing_setpoints_ram_bank_read"
#define FOFB_PROCESSING_OPCODE_SETPOINTS_RAM_BANK_WRITE          16
#define FOFB_PROCESSING_NAME_SETPOINTS_RAM_BANK_WRITE            "_fofb_processing_setpoints_ram_bank_write"

/* Messaging Reply OPCODES */
#define FOFB_PROCESSING_REPLY_TYPE                              uint32_t
#define FOFB_PROCESSING_REPLY_SIZE                              (sizeof (FOFB_PROCESSING_REPLY_TYPE))

#define FOFB_PROCESSING_OK                                      0   /* Operation was successful */
#define FOFB_PROCESSING_ERR                                     1   /* Generic error */

#endif

