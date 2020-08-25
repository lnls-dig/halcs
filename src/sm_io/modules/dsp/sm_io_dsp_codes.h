/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _SM_IO_DSP_CODES_H_
#define _SM_IO_DSP_CODES_H_

struct _smio_dsp_data_t {
    int32_t amp_ch0;                     /* Amplitude CH0 */
    int32_t amp_ch1;                     /* Amplitude CH1 */
    int32_t amp_ch2;                     /* Amplitude CH2 */
    int32_t amp_ch3;                     /* Amplitude CH3 */
    uint32_t new_amp_data;               /* Do we have new AMP data? */
    int32_t pos_x;                       /* Position X */
    int32_t pos_y;                       /* Position Y */
    int32_t pos_q;                       /* Position Q */
    uint32_t pos_sum;                    /* Position Sum */
    uint32_t new_pos_data;               /* Do we have new POS data? */
};

struct _smio_dsp_monit_data_t {
    int32_t amp_ch0;                     /* Amplitude CH0 */
    int32_t amp_ch1;                     /* Amplitude CH1 */
    int32_t amp_ch2;                     /* Amplitude CH2 */
    int32_t amp_ch3;                     /* Amplitude CH3 */
};

/* Messaging OPCODES */
#define DSP_OPCODE_TYPE                     uint32_t
#define DSP_OPCODE_SIZE                     (sizeof (DSP_OPCODE_TYPE))

#define DSP_OPCODE_SET_GET_KX               0
#define DSP_NAME_SET_GET_KX                 "dsp_set_get_kx"
#define DSP_OPCODE_SET_GET_KY               1
#define DSP_NAME_SET_GET_KY                 "dsp_set_get_ky"
#define DSP_OPCODE_SET_GET_KSUM             2
#define DSP_NAME_SET_GET_KSUM               "dsp_set_get_ksum"
#define DSP_OPCODE_SET_GET_DS_TBT_THRES     3
#define DSP_NAME_SET_GET_DS_TBT_THRES       "dsp_set_get_tbt_thres"
#define DSP_OPCODE_SET_GET_DS_FOFB_THRES    4
#define DSP_NAME_SET_GET_DS_FOFB_THRES      "dsp_set_get_fofb_thres"
#define DSP_OPCODE_SET_GET_DS_MONIT_THRES   5
#define DSP_NAME_SET_GET_DS_MONIT_THRES     "dsp_set_get_monit_thres"
#define DSP_OPCODE_SET_GET_CFG_TEST_DATA    6
#define DSP_NAME_SET_GET_CFG_TEST_DATA      "dsp_set_get_cfg_test_data"
#define DSP_OPCODE_SET_GET_MONIT_AMP_CH0    7
#define DSP_NAME_SET_GET_MONIT_AMP_CH0      "dsp_set_get_monit_amp_ch0"
#define DSP_OPCODE_SET_GET_MONIT_AMP_CH1    8
#define DSP_NAME_SET_GET_MONIT_AMP_CH1      "dsp_set_get_monit_amp_ch1"
#define DSP_OPCODE_SET_GET_MONIT_AMP_CH2    9
#define DSP_NAME_SET_GET_MONIT_AMP_CH2      "dsp_set_get_monit_amp_ch2"
#define DSP_OPCODE_SET_GET_MONIT_AMP_CH3    10
#define DSP_NAME_SET_GET_MONIT_AMP_CH3      "dsp_set_get_monit_amp_ch3"
#define DSP_OPCODE_SET_GET_MONIT_POS_X      11
#define DSP_NAME_SET_GET_MONIT_POS_X        "dsp_set_get_monit_pos_x"
#define DSP_OPCODE_SET_GET_MONIT_POS_Y      12
#define DSP_NAME_SET_GET_MONIT_POS_Y        "dsp_set_get_monit_pos_y"
#define DSP_OPCODE_SET_GET_MONIT_POS_Q      13
#define DSP_NAME_SET_GET_MONIT_POS_Q        "dsp_set_get_monit_pos_q"
#define DSP_OPCODE_SET_GET_MONIT_POS_SUM    14
#define DSP_NAME_SET_GET_MONIT_POS_SUM      "dsp_set_get_monit_pos_sum"
#define DSP_OPCODE_SET_GET_MONIT_UPDT       15
#define DSP_NAME_SET_GET_MONIT_UPDT         "dsp_set_get_monit_updt"
#define DSP_OPCODE_SET_GET_MONIT_AMP_POS    16
#define DSP_NAME_SET_GET_MONIT_AMP_POS      "dsp_set_get_monit_amp_pos"
#define DSP_OPCODE_SET_GET_MONIT1_AMP_CH0   17
#define DSP_NAME_SET_GET_MONIT1_AMP_CH0     "dsp_set_get_monit1_amp_ch0"
#define DSP_OPCODE_SET_GET_MONIT1_AMP_CH1   18
#define DSP_NAME_SET_GET_MONIT1_AMP_CH1     "dsp_set_get_monit1_amp_ch1"
#define DSP_OPCODE_SET_GET_MONIT1_AMP_CH2   19
#define DSP_NAME_SET_GET_MONIT1_AMP_CH2     "dsp_set_get_monit1_amp_ch2"
#define DSP_OPCODE_SET_GET_MONIT1_AMP_CH3   20
#define DSP_NAME_SET_GET_MONIT1_AMP_CH3     "dsp_set_get_monit1_amp_ch3"
#define DSP_OPCODE_SET_GET_MONIT1_POS_X     21
#define DSP_NAME_SET_GET_MONIT1_POS_X       "dsp_set_get_monit1_pos_x"
#define DSP_OPCODE_SET_GET_MONIT1_POS_Y     22
#define DSP_NAME_SET_GET_MONIT1_POS_Y       "dsp_set_get_monit1_pos_y"
#define DSP_OPCODE_SET_GET_MONIT1_POS_Q     23
#define DSP_NAME_SET_GET_MONIT1_POS_Q       "dsp_set_get_monit1_pos_q"
#define DSP_OPCODE_SET_GET_MONIT1_POS_SUM   24
#define DSP_NAME_SET_GET_MONIT1_POS_SUM     "dsp_set_get_monit1_pos_sum"
#define DSP_OPCODE_SET_GET_MONIT1_UPDT      25
#define DSP_NAME_SET_GET_MONIT1_UPDT        "dsp_set_get_monit1_updt"
#define DSP_OPCODE_SET_GET_MONIT1_AMP_POS   26
#define DSP_NAME_SET_GET_MONIT1_AMP_POS     "dsp_set_get_monit1_amp_pos"
#define DSP_OPCODE_SET_GET_SW_TAG_EN        27
#define DSP_NAME_SET_GET_SW_TAG_EN          "dsp_set_get_sw_tag_en"
#define DSP_OPCODE_SET_GET_SW_DATA_MASK_EN  28
#define DSP_NAME_SET_GET_SW_DATA_MASK_EN    "dsp_set_get_sw_data_mask_en"
#define DSP_OPCODE_SET_GET_SW_DATA_MASK_SAMPLES  29
#define DSP_NAME_SET_GET_SW_DATA_MASK_SAMPLES    "dsp_set_get_sw_data_mask_samples"
#define DSP_OPCODE_SET_GET_TBT_TAG_EN       30
#define DSP_NAME_SET_GET_TBT_TAG_EN         "dsp_set_get_tbt_tag_en"
#define DSP_OPCODE_SET_GET_TBT_TAG_DLY      31
#define DSP_NAME_SET_GET_TBT_TAG_DLY        "dsp_set_get_tbt_tag_dly"
#define DSP_OPCODE_SET_GET_TBT_DATA_MASK_EN 32
#define DSP_NAME_SET_GET_TBT_DATA_MASK_EN   "dsp_set_get_tbt_data_mask_en"
#define DSP_OPCODE_SET_GET_TBT_DATA_MASK_SAMPLES_BEG  \
                                            33
#define DSP_NAME_SET_GET_TBT_DATA_MASK_SAMPLES_BEG    \
                                            "dsp_set_get_tbt_data_mask_samples_beg"
#define DSP_OPCODE_SET_GET_TBT_DATA_MASK_SAMPLES_END  \
                                            34
#define DSP_NAME_SET_GET_TBT_DATA_MASK_SAMPLES_END    \
                                            "dsp_set_get_tbt_data_mask_samples_end"
#define DSP_OPCODE_SET_GET_SW_TAG_DESYNC_CNT_RST \
                                            35
#define DSP_NAME_SET_GET_SW_TAG_DESYNC_CNT_RST \
                                            "dsp_set_get_sw_tag_desync_cnt_rst"
#define DSP_OPCODE_SET_GET_SW_TAG_DESYNC_CNT \
                                            36
#define DSP_NAME_SET_GET_SW_TAG_DESYNC_CNT   "dsp_set_get_sw_tag_desync_cnt"
#define DSP_OPCODE_SET_GET_TBT_TAG_DESYNC_CNT_RST \
                                            37
#define DSP_NAME_SET_GET_TBT_TAG_DESYNC_CNT_RST \
                                            "dsp_set_get_tbt_tag_desync_cnt_rst"
#define DSP_OPCODE_SET_GET_TBT_TAG_DESYNC_CNT \
                                            38
#define DSP_NAME_SET_GET_TBT_TAG_DESYNC_CNT   "dsp_set_get_tbt_tag_desync_cnt"
#define DSP_OPCODE_SET_GET_MONIT_POLL_TIME  39
#define DSP_NAME_SET_GET_MONIT_POLL_TIME    "dsp_set_get_monit_poll_time"
#define DSP_OPCODE_SET_GET_MONIT1_TAG_EN    40
#define DSP_NAME_SET_GET_MONIT1_TAG_EN      "dsp_set_get_monit1_tag_en"
#define DSP_OPCODE_SET_GET_MONIT1_TAG_DLY   41
#define DSP_NAME_SET_GET_MONIT1_TAG_DLY     "dsp_set_get_monit1_tag_dly"
#define DSP_OPCODE_SET_GET_MONIT1_DATA_MASK_EN \
                                            42
#define DSP_NAME_SET_GET_MONIT1_DATA_MASK_EN \
                                            "dsp_set_get_monit1_data_mask_en"
#define DSP_OPCODE_SET_GET_MONIT1_DATA_MASK_SAMPLES_BEG  \
                                            43
#define DSP_NAME_SET_GET_MONIT1_DATA_MASK_SAMPLES_BEG    \
                                            "dsp_set_get_monit1_data_mask_samples_beg"
#define DSP_OPCODE_SET_GET_MONIT1_DATA_MASK_SAMPLES_END  \
                                            44
#define DSP_NAME_SET_GET_MONIT1_DATA_MASK_SAMPLES_END    \
                                            "dsp_set_get_monit1_data_mask_samples_end"
#define DSP_OPCODE_SET_GET_MONIT1_TAG_DESYNC_CNT_RST \
                                            45
#define DSP_NAME_SET_GET_MONIT1_TAG_DESYNC_CNT_RST \
                                            "dsp_set_get_monit1_tag_desync_cnt_rst"
#define DSP_OPCODE_SET_GET_MONIT1_TAG_DESYNC_CNT \
                                            46
#define DSP_NAME_SET_GET_MONIT1_TAG_DESYNC_CNT \
                                            "dsp_set_get_monit1_tag_desync_cnt"
#define DSP_OPCODE_SET_GET_MONIT_TAG_EN     47
#define DSP_NAME_SET_GET_MONIT_TAG_EN       "dsp_set_get_monit_tag_en"
#define DSP_OPCODE_SET_GET_MONIT_TAG_DLY    48
#define DSP_NAME_SET_GET_MONIT_TAG_DLY      "dsp_set_get_monit_tag_dly"
#define DSP_OPCODE_SET_GET_MONIT_DATA_MASK_EN \
                                            49
#define DSP_NAME_SET_GET_MONIT_DATA_MASK_EN \
                                            "dsp_set_get_monit_data_mask_en"
#define DSP_OPCODE_SET_GET_MONIT_DATA_MASK_SAMPLES_BEG  \
                                            50
#define DSP_NAME_SET_GET_MONIT_DATA_MASK_SAMPLES_BEG    \
                                            "dsp_set_get_monit_data_mask_samples_beg"
#define DSP_OPCODE_SET_GET_MONIT_DATA_MASK_SAMPLES_END  \
                                            51
#define DSP_NAME_SET_GET_MONIT_DATA_MASK_SAMPLES_END    \
                                            "dsp_set_get_monit_data_mask_samples_end"
#define DSP_OPCODE_SET_GET_MONIT_TAG_DESYNC_CNT_RST \
                                            52
#define DSP_NAME_SET_GET_MONIT_TAG_DESYNC_CNT_RST \
                                            "dsp_set_get_monit_tag_desync_cnt_rst"
#define DSP_OPCODE_SET_GET_MONIT_TAG_DESYNC_CNT \
                                            53
#define DSP_NAME_SET_GET_MONIT_TAG_DESYNC_CNT \
                                            "dsp_set_get_monit_tag_desync_cnt"
#define DSP_OPCODE_SET_GET_OFFSET_X         54
#define DSP_NAME_SET_GET_OFFSET_X           "dsp_set_get_offset_x"
#define DSP_OPCODE_SET_GET_OFFSET_Y         55
#define DSP_NAME_SET_GET_OFFSET_Y           "dsp_set_get_offset_y"
#define DSP_OPCODE_END                      56

/* Messaging Reply OPCODES */
#define DSP_REPLY_TYPE                      uint32_t
#define DSP_REPLY_SIZE                      (sizeof (DSP_REPLY_TYPE))

#define DSP_OK                              0   /* Operation was successful */
#define DSP_ERR                             1   /* Generic error */

#endif
