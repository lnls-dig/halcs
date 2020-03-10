#ifndef __CHEBY__FMC_ADC_100MS_CSR__H__
#define __CHEBY__FMC_ADC_100MS_CSR__H__
#define FMC_ADC_100MS_CSR_SIZE 512

#include <inttypes.h>

#if defined( __GNUC__)
#define PACKED __attribute__ ((packed))
#else
#error "Unsupported compiler?"
#endif

#ifndef __WBGEN2_MACROS_DEFINED__
#define __WBGEN2_MACROS_DEFINED__
#define WBGEN2_GEN_MASK(offset, size) (((1<<(size))-1) << (offset))
#define WBGEN2_GEN_WRITE(value, offset, size) (((value) & ((1<<(size))-1)) << (offset))
#define WBGEN2_GEN_READ(reg, offset, size) (((reg) >> (offset)) & ((1<<(size))-1))
#define WBGEN2_SIGN_EXTEND(value, bits) (((value) & (1<<bits) ? ~((1<<(bits))-1): 0 ) | (value))
#endif

/* Control register */
#define FMC_ADC_100MS_CSR_REG_CTL 0x0UL
#define FMC_ADC_100MS_CSR_CTL_FSM_CMD_MASK 0x3UL
#define FMC_ADC_100MS_CSR_CTL_FSM_CMD_SHIFT 0
#define FMC_ADC_100MS_CSR_CTL_FMC_CLK_OE 0x4UL
#define FMC_ADC_100MS_CSR_CTL_OFFSET_DAC_CLR_N 0x8UL
#define FMC_ADC_100MS_CSR_CTL_MAN_BITSLIP 0x10UL
#define FMC_ADC_100MS_CSR_CTL_TEST_DATA_EN 0x20UL
#define FMC_ADC_100MS_CSR_CTL_TRIG_LED 0x40UL
#define FMC_ADC_100MS_CSR_CTL_ACQ_LED 0x80UL
#define FMC_ADC_100MS_CSR_CTL_CLEAR_TRIG_STAT 0x100UL

/* Status register */
#define FMC_ADC_100MS_CSR_REG_STA 0x4UL
#define FMC_ADC_100MS_CSR_STA_FSM_MASK 0x7UL
#define FMC_ADC_100MS_CSR_STA_FSM_SHIFT 0
#define FMC_ADC_100MS_CSR_STA_SERDES_PLL 0x8UL
#define FMC_ADC_100MS_CSR_STA_SERDES_SYNCED 0x10UL
#define FMC_ADC_100MS_CSR_STA_ACQ_CFG 0x20UL

/* Trigger status */
#define FMC_ADC_100MS_CSR_REG_TRIG_STAT 0x8UL
#define FMC_ADC_100MS_CSR_TRIG_STAT_EXT 0x1UL
#define FMC_ADC_100MS_CSR_TRIG_STAT_SW 0x2UL
#define FMC_ADC_100MS_CSR_TRIG_STAT_TIME 0x10UL
#define FMC_ADC_100MS_CSR_TRIG_STAT_CH1 0x100UL
#define FMC_ADC_100MS_CSR_TRIG_STAT_CH2 0x200UL
#define FMC_ADC_100MS_CSR_TRIG_STAT_CH3 0x400UL
#define FMC_ADC_100MS_CSR_TRIG_STAT_CH4 0x800UL

/* Trigger enable */
#define FMC_ADC_100MS_CSR_REG_TRIG_EN 0xcUL
#define FMC_ADC_100MS_CSR_TRIG_EN_EXT 0x1UL
#define FMC_ADC_100MS_CSR_TRIG_EN_SW 0x2UL
#define FMC_ADC_100MS_CSR_TRIG_EN_TIME 0x10UL
#define FMC_ADC_100MS_CSR_TRIG_EN_AUX_TIME 0x20UL
#define FMC_ADC_100MS_CSR_TRIG_EN_CH1 0x100UL
#define FMC_ADC_100MS_CSR_TRIG_EN_CH2 0x200UL
#define FMC_ADC_100MS_CSR_TRIG_EN_CH3 0x400UL
#define FMC_ADC_100MS_CSR_TRIG_EN_CH4 0x800UL

/* Trigger polarity */
#define FMC_ADC_100MS_CSR_REG_TRIG_POL 0x10UL
#define FMC_ADC_100MS_CSR_TRIG_POL_EXT 0x1UL
#define FMC_ADC_100MS_CSR_TRIG_POL_CH1 0x100UL
#define FMC_ADC_100MS_CSR_TRIG_POL_CH2 0x200UL
#define FMC_ADC_100MS_CSR_TRIG_POL_CH3 0x400UL
#define FMC_ADC_100MS_CSR_TRIG_POL_CH4 0x800UL

/* External trigger delay */
#define FMC_ADC_100MS_CSR_REG_EXT_TRIG_DLY 0x14UL

/* Software trigger */
#define FMC_ADC_100MS_CSR_REG_SW_TRIG 0x18UL

/* Number of shots */
#define FMC_ADC_100MS_CSR_REG_SHOTS 0x1cUL
#define FMC_ADC_100MS_CSR_SHOTS_NBR_MASK 0xffffUL
#define FMC_ADC_100MS_CSR_SHOTS_NBR_SHIFT 0
#define FMC_ADC_100MS_CSR_SHOTS_REMAIN_MASK 0xffff0000UL
#define FMC_ADC_100MS_CSR_SHOTS_REMAIN_SHIFT 16

/* Multi-shot sample depth register */
#define FMC_ADC_100MS_CSR_REG_MULTI_DEPTH 0x20UL

/* Trigger address register */
#define FMC_ADC_100MS_CSR_REG_TRIG_POS 0x24UL

/* Sampling clock frequency */
#define FMC_ADC_100MS_CSR_REG_FS_FREQ 0x28UL

/* Downsampling ratio */
#define FMC_ADC_100MS_CSR_REG_DOWNSAMPLE 0x2cUL

/* Pre-trigger samples */
#define FMC_ADC_100MS_CSR_REG_PRE_SAMPLES 0x30UL

/* Post-trigger samples */
#define FMC_ADC_100MS_CSR_REG_POST_SAMPLES 0x34UL

/* Samples counter */
#define FMC_ADC_100MS_CSR_REG_SAMPLES_CNT 0x38UL

/* Channel 1 control register */
#define FMC_ADC_100MS_CSR_REG_CH1_CTL 0x80UL
#define FMC_ADC_100MS_CSR_CH1_CTL_SSR_MASK 0x7fUL
#define FMC_ADC_100MS_CSR_CH1_CTL_SSR_SHIFT 0

/* Channel 1 status register */
#define FMC_ADC_100MS_CSR_REG_CH1_STA 0x84UL
#define FMC_ADC_100MS_CSR_CH1_STA_VAL_MASK 0xffffUL
#define FMC_ADC_100MS_CSR_CH1_STA_VAL_SHIFT 0

/* Channel 1 calibration register */
#define FMC_ADC_100MS_CSR_REG_CH1_CALIB 0x88UL
#define FMC_ADC_100MS_CSR_CH1_CALIB_GAIN_MASK 0xffffUL
#define FMC_ADC_100MS_CSR_CH1_CALIB_GAIN_SHIFT 0
#define FMC_ADC_100MS_CSR_CH1_CALIB_OFFSET_MASK 0xffff0000UL
#define FMC_ADC_100MS_CSR_CH1_CALIB_OFFSET_SHIFT 16

/* Channel 1 saturation register */
#define FMC_ADC_100MS_CSR_REG_CH1_SAT 0x8cUL
#define FMC_ADC_100MS_CSR_CH1_SAT_VAL_MASK 0x7fffUL
#define FMC_ADC_100MS_CSR_CH1_SAT_VAL_SHIFT 0

/* Channel 1 trigger threshold configuration register */
#define FMC_ADC_100MS_CSR_REG_CH1_TRIG_THRES 0x90UL
#define FMC_ADC_100MS_CSR_CH1_TRIG_THRES_VAL_MASK 0xffffUL
#define FMC_ADC_100MS_CSR_CH1_TRIG_THRES_VAL_SHIFT 0
#define FMC_ADC_100MS_CSR_CH1_TRIG_THRES_HYST_MASK 0xffff0000UL
#define FMC_ADC_100MS_CSR_CH1_TRIG_THRES_HYST_SHIFT 16

/* Channel 1 trigger delay */
#define FMC_ADC_100MS_CSR_REG_CH1_TRIG_DLY 0x94UL

/* Channel 2 control register */
#define FMC_ADC_100MS_CSR_REG_CH2_CTL 0xc0UL
#define FMC_ADC_100MS_CSR_CH2_CTL_SSR_MASK 0x7fUL
#define FMC_ADC_100MS_CSR_CH2_CTL_SSR_SHIFT 0

/* Channel 2 status register */
#define FMC_ADC_100MS_CSR_REG_CH2_STA 0xc4UL
#define FMC_ADC_100MS_CSR_CH2_STA_VAL_MASK 0xffffUL
#define FMC_ADC_100MS_CSR_CH2_STA_VAL_SHIFT 0

/* Channel 2 calibration register */
#define FMC_ADC_100MS_CSR_REG_CH2_CALIB 0xc8UL
#define FMC_ADC_100MS_CSR_CH2_CALIB_GAIN_MASK 0xffffUL
#define FMC_ADC_100MS_CSR_CH2_CALIB_GAIN_SHIFT 0
#define FMC_ADC_100MS_CSR_CH2_CALIB_OFFSET_MASK 0xffff0000UL
#define FMC_ADC_100MS_CSR_CH2_CALIB_OFFSET_SHIFT 16

/* Channel 2 saturation register */
#define FMC_ADC_100MS_CSR_REG_CH2_SAT 0xccUL
#define FMC_ADC_100MS_CSR_CH2_SAT_VAL_MASK 0x7fffUL
#define FMC_ADC_100MS_CSR_CH2_SAT_VAL_SHIFT 0

/* Channel 2 trigger threshold configuration register */
#define FMC_ADC_100MS_CSR_REG_CH2_TRIG_THRES 0xd0UL
#define FMC_ADC_100MS_CSR_CH2_TRIG_THRES_VAL_MASK 0xffffUL
#define FMC_ADC_100MS_CSR_CH2_TRIG_THRES_VAL_SHIFT 0
#define FMC_ADC_100MS_CSR_CH2_TRIG_THRES_HYST_MASK 0xffff0000UL
#define FMC_ADC_100MS_CSR_CH2_TRIG_THRES_HYST_SHIFT 16

/* Channel 2 trigger delay */
#define FMC_ADC_100MS_CSR_REG_CH2_TRIG_DLY 0xd4UL

/* Channel 3 control register */
#define FMC_ADC_100MS_CSR_REG_CH3_CTL 0x100UL
#define FMC_ADC_100MS_CSR_CH3_CTL_SSR_MASK 0x7fUL
#define FMC_ADC_100MS_CSR_CH3_CTL_SSR_SHIFT 0

/* Channel 3 status register */
#define FMC_ADC_100MS_CSR_REG_CH3_STA 0x104UL
#define FMC_ADC_100MS_CSR_CH3_STA_VAL_MASK 0xffffUL
#define FMC_ADC_100MS_CSR_CH3_STA_VAL_SHIFT 0

/* Channel 3 calibration register */
#define FMC_ADC_100MS_CSR_REG_CH3_CALIB 0x108UL
#define FMC_ADC_100MS_CSR_CH3_CALIB_GAIN_MASK 0xffffUL
#define FMC_ADC_100MS_CSR_CH3_CALIB_GAIN_SHIFT 0
#define FMC_ADC_100MS_CSR_CH3_CALIB_OFFSET_MASK 0xffff0000UL
#define FMC_ADC_100MS_CSR_CH3_CALIB_OFFSET_SHIFT 16

/* Channel 3 saturation register */
#define FMC_ADC_100MS_CSR_REG_CH3_SAT 0x10cUL
#define FMC_ADC_100MS_CSR_CH3_SAT_VAL_MASK 0x7fffUL
#define FMC_ADC_100MS_CSR_CH3_SAT_VAL_SHIFT 0

/* Channel 3 trigger threshold configuration register */
#define FMC_ADC_100MS_CSR_REG_CH3_TRIG_THRES 0x110UL
#define FMC_ADC_100MS_CSR_CH3_TRIG_THRES_VAL_MASK 0xffffUL
#define FMC_ADC_100MS_CSR_CH3_TRIG_THRES_VAL_SHIFT 0
#define FMC_ADC_100MS_CSR_CH3_TRIG_THRES_HYST_MASK 0xffff0000UL
#define FMC_ADC_100MS_CSR_CH3_TRIG_THRES_HYST_SHIFT 16

/* Channel 3 trigger delay */
#define FMC_ADC_100MS_CSR_REG_CH3_TRIG_DLY 0x114UL

/* Channel 4 control register */
#define FMC_ADC_100MS_CSR_REG_CH4_CTL 0x140UL
#define FMC_ADC_100MS_CSR_CH4_CTL_SSR_MASK 0x7fUL
#define FMC_ADC_100MS_CSR_CH4_CTL_SSR_SHIFT 0

/* Channel 4 status register */
#define FMC_ADC_100MS_CSR_REG_CH4_STA 0x144UL
#define FMC_ADC_100MS_CSR_CH4_STA_VAL_MASK 0xffffUL
#define FMC_ADC_100MS_CSR_CH4_STA_VAL_SHIFT 0

/* Channel 4 gain calibration register */
#define FMC_ADC_100MS_CSR_REG_CH4_CALIB 0x148UL
#define FMC_ADC_100MS_CSR_CH4_CALIB_GAIN_MASK 0xffffUL
#define FMC_ADC_100MS_CSR_CH4_CALIB_GAIN_SHIFT 0
#define FMC_ADC_100MS_CSR_CH4_CALIB_OFFSET_MASK 0xffff0000UL
#define FMC_ADC_100MS_CSR_CH4_CALIB_OFFSET_SHIFT 16

/* Channel 4 saturation register */
#define FMC_ADC_100MS_CSR_REG_CH4_SAT 0x14cUL
#define FMC_ADC_100MS_CSR_CH4_SAT_VAL_MASK 0x7fffUL
#define FMC_ADC_100MS_CSR_CH4_SAT_VAL_SHIFT 0

/* Channel 4 trigger threshold configuration register */
#define FMC_ADC_100MS_CSR_REG_CH4_TRIG_THRES 0x150UL
#define FMC_ADC_100MS_CSR_CH4_TRIG_THRES_VAL_MASK 0xffffUL
#define FMC_ADC_100MS_CSR_CH4_TRIG_THRES_VAL_SHIFT 0
#define FMC_ADC_100MS_CSR_CH4_TRIG_THRES_HYST_MASK 0xffff0000UL
#define FMC_ADC_100MS_CSR_CH4_TRIG_THRES_HYST_SHIFT 16

/* Channel 4 trigger delay */
#define FMC_ADC_100MS_CSR_REG_CH4_TRIG_DLY 0x154UL

struct fmc_adc_100ms_csr {
  /* [0x0]: REG (rw) Control register */
  uint32_t ctl;

  /* [0x4]: REG (ro) Status register */
  uint32_t sta;

  /* [0x8]: REG (ro) Trigger status */
  uint32_t trig_stat;

  /* [0xc]: REG (rw) Trigger enable */
  uint32_t trig_en;

  /* [0x10]: REG (rw) Trigger polarity */
  uint32_t trig_pol;

  /* [0x14]: REG (rw) External trigger delay */
  uint32_t ext_trig_dly;

  /* [0x18]: REG (wo) Software trigger */
  uint32_t sw_trig;

  /* [0x1c]: REG (rw) Number of shots */
  uint32_t shots;

  /* [0x20]: REG (ro) Multi-shot sample depth register */
  uint32_t multi_depth;

  /* [0x24]: REG (ro) Trigger address register */
  uint32_t trig_pos;

  /* [0x28]: REG (ro) Sampling clock frequency */
  uint32_t fs_freq;

  /* [0x2c]: REG (rw) Downsampling ratio */
  uint32_t downsample;

  /* [0x30]: REG (rw) Pre-trigger samples */
  uint32_t pre_samples;

  /* [0x34]: REG (rw) Post-trigger samples */
  uint32_t post_samples;

  /* [0x38]: REG (ro) Samples counter */
  uint32_t samples_cnt;

  /* padding to: 32 words */
  uint32_t __padding_0[17];

  /* [0x80]: REG (rw) Channel 1 control register */
  uint32_t ch1_ctl;

  /* [0x84]: REG (ro) Channel 1 status register */
  uint32_t ch1_sta;

  /* [0x88]: REG (rw) Channel 1 calibration register */
  uint32_t ch1_calib;

  /* [0x8c]: REG (rw) Channel 1 saturation register */
  uint32_t ch1_sat;

  /* [0x90]: REG (rw) Channel 1 trigger threshold configuration register */
  uint32_t ch1_trig_thres;

  /* [0x94]: REG (rw) Channel 1 trigger delay */
  uint32_t ch1_trig_dly;

  /* padding to: 48 words */
  uint32_t __padding_1[10];

  /* [0xc0]: REG (rw) Channel 2 control register */
  uint32_t ch2_ctl;

  /* [0xc4]: REG (ro) Channel 2 status register */
  uint32_t ch2_sta;

  /* [0xc8]: REG (rw) Channel 2 calibration register */
  uint32_t ch2_calib;

  /* [0xcc]: REG (rw) Channel 2 saturation register */
  uint32_t ch2_sat;

  /* [0xd0]: REG (rw) Channel 2 trigger threshold configuration register */
  uint32_t ch2_trig_thres;

  /* [0xd4]: REG (rw) Channel 2 trigger delay */
  uint32_t ch2_trig_dly;

  /* padding to: 64 words */
  uint32_t __padding_2[10];

  /* [0x100]: REG (rw) Channel 3 control register */
  uint32_t ch3_ctl;

  /* [0x104]: REG (ro) Channel 3 status register */
  uint32_t ch3_sta;

  /* [0x108]: REG (rw) Channel 3 calibration register */
  uint32_t ch3_calib;

  /* [0x10c]: REG (rw) Channel 3 saturation register */
  uint32_t ch3_sat;

  /* [0x110]: REG (rw) Channel 3 trigger threshold configuration register */
  uint32_t ch3_trig_thres;

  /* [0x114]: REG (rw) Channel 3 trigger delay */
  uint32_t ch3_trig_dly;

  /* padding to: 80 words */
  uint32_t __padding_3[10];

  /* [0x140]: REG (rw) Channel 4 control register */
  uint32_t ch4_ctl;

  /* [0x144]: REG (ro) Channel 4 status register */
  uint32_t ch4_sta;

  /* [0x148]: REG (rw) Channel 4 gain calibration register */
  uint32_t ch4_calib;

  /* [0x14c]: REG (rw) Channel 4 saturation register */
  uint32_t ch4_sat;

  /* [0x150]: REG (rw) Channel 4 trigger threshold configuration register */
  uint32_t ch4_trig_thres;

  /* [0x154]: REG (rw) Channel 4 trigger delay */
  uint32_t ch4_trig_dly;

  /* padding to: 85 words */
  uint32_t __padding_4[42];
};

#endif /* __CHEBY__FMC_ADC_100MS_CSR__H__ */
