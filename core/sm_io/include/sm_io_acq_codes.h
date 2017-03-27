/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _SM_IO_ACQ_CODES_H_
#define _SM_IO_ACQ_CODES_H_

#include "acq_chan_gen_defs.h"

struct _smio_acq_data_block_t {
    uint32_t valid_bytes;           /* how much of the BLOCK_SIZE bytes are valid */
    uint8_t data[BLOCK_SIZE];       /* data buffer */
};

/* Messaging OPCODES */
#define ACQ_OPCODE_TYPE                  uint32_t
#define ACQ_OPCODE_SIZE                  (sizeof (ACQ_OPCODE_TYPE))

#define ACQ_OPCODE_DATA_ACQUIRE         0
#define ACQ_NAME_DATA_ACQUIRE           "acq_data_acquire"
#define ACQ_OPCODE_GET_DATA_BLOCK       1
#define ACQ_NAME_GET_DATA_BLOCK         "acq_get_data_block"
#define ACQ_OPCODE_CHECK_DATA_ACQUIRE   2
#define ACQ_NAME_CHECK_DATA_ACQUIRE     "acq_check_data_acquire"
#define ACQ_OPCODE_CFG_TRIG             3
#define ACQ_NAME_CFG_TRIG               "acq_cfg_trig"
#define ACQ_OPCODE_HW_DATA_TRIG_POL     4
#define ACQ_NAME_HW_DATA_TRIG_POL       "acq_hw_data_trig_pol"
#define ACQ_OPCODE_HW_DATA_TRIG_SEL     5
#define ACQ_NAME_HW_DATA_TRIG_SEL       "acq_hw_data_trig_sel"
#define ACQ_OPCODE_HW_DATA_TRIG_FILT    6
#define ACQ_NAME_HW_DATA_TRIG_FILT      "acq_hw_data_filt"
#define ACQ_OPCODE_HW_DATA_TRIG_THRES   7
#define ACQ_NAME_HW_DATA_TRIG_THRES     "acq_hw_data_thres"
#define ACQ_OPCODE_HW_TRIG_DLY          8
#define ACQ_NAME_HW_TRIG_DLY            "acq_hw_trig_dly"
#define ACQ_OPCODE_SW_TRIG              9
#define ACQ_NAME_SW_TRIG                "acq_sw_trig"
#define ACQ_OPCODE_FSM_STOP             10
#define ACQ_NAME_FSM_STOP               "acq_fsm_stop"
#define ACQ_OPCODE_HW_DATA_TRIG_CHAN    11
#define ACQ_NAME_HW_DATA_TRIG_CHAN      "acq_hw_data_trig_chan"
#define ACQ_OPCODE_CH_INT_WIDTH         12
#define ACQ_NAME_CH_INT_WIDTH           "acq_ch_int_width"
#define ACQ_OPCODE_CH_NUM_COALESCE      13
#define ACQ_NAME_CH_NUM_COALESCE        "acq_ch_num_coalesce"
#define ACQ_OPCODE_CH_NUM_ATOMS         14
#define ACQ_NAME_CH_NUM_ATOMS           "acq_ch_num_atoms"
#define ACQ_OPCODE_CH_ATOM_WIDTH        15
#define ACQ_NAME_CH_ATOM_WIDTH          "acq_ch_atom_width"
#define ACQ_OPCODE_CH_SAMPLE_SIZE       16
#define ACQ_NAME_CH_SAMPLE_SIZE         "acq_ch_sample_size"
#define ACQ_OPCODE_END                  17

/* Messaging Reply OPCODES */
#define ACQ_REPLY_TYPE                  uint32_t
#define ACQ_REPLY_SIZE                  (sizeof (ACQ_REPLY_TYPE))

#define ACQ_OK                          0   /* Operation was successful */
#define ACQ_ERR                         1   /* Generic error */
#define ACQ_NUM_SAMPLES_OOR             2   /* Number of samples out of range */
#define ACQ_NOT_COMPLETED               3   /* Acquisition not completed */
#define ACQ_BLOCK_OOR                   4   /* Block number out of range */
#define ACQ_NUM_CHAN_OOR                5   /* Channel number out of range */
#define ACQ_COULD_NOT_READ              6   /* Could not read memory block */
#define ACQ_TRIG_TYPE                   7   /* Incompatible trigger type */
#define ACQ_REPLY_END                   8   /* End marker */

#endif
