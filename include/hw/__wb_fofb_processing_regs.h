#ifndef __CHEBY__WB_FOFB_PROCESSING_REGS__H__
#define __CHEBY__WB_FOFB_PROCESSING_REGS__H__
#define WB_FOFB_PROCESSING_REGS_SIZE 28672 /* 0x7000 = 28KB */

/* fofb processing coefficients fixed-point position constant */
#define WB_FOFB_PROCESSING_REGS_COEFFS_FIXED_POINT_POS 0x0UL
#define WB_FOFB_PROCESSING_REGS_COEFFS_FIXED_POINT_POS_VAL_MASK 0xffffffffUL
#define WB_FOFB_PROCESSING_REGS_COEFFS_FIXED_POINT_POS_VAL_SHIFT 0

/* fofb processing accumulators' gains fixed-point position register */
#define WB_FOFB_PROCESSING_REGS_ACCS_GAINS_FIXED_POINT_POS 0x4UL
#define WB_FOFB_PROCESSING_REGS_ACCS_GAINS_FIXED_POINT_POS_VAL_MASK 0xffffffffUL
#define WB_FOFB_PROCESSING_REGS_ACCS_GAINS_FIXED_POINT_POS_VAL_SHIFT 0

/* fofb processing accumulator gain register (per channel) */
#define WB_FOFB_PROCESSING_REGS_ACC_GAIN_0 0x8UL
#define WB_FOFB_PROCESSING_REGS_ACC_GAIN_0_VAL_MASK 0xffffffffUL
#define WB_FOFB_PROCESSING_REGS_ACC_GAIN_0_VAL_SHIFT 0

/* fofb processing accumulator control register (per channel) */
#define WB_FOFB_PROCESSING_REGS_ACC_CTL_0 0xcUL
#define WB_FOFB_PROCESSING_REGS_ACC_CTL_0_CLEAR 0x1UL
#define WB_FOFB_PROCESSING_REGS_ACC_CTL_0_FREEZE 0x2UL

/* fofb processing accumulator gain register (per channel) */
#define WB_FOFB_PROCESSING_REGS_ACC_GAIN_1 0x10UL
#define WB_FOFB_PROCESSING_REGS_ACC_GAIN_1_VAL_MASK 0xffffffffUL
#define WB_FOFB_PROCESSING_REGS_ACC_GAIN_1_VAL_SHIFT 0

/* fofb processing accumulator control register (per channel) */
#define WB_FOFB_PROCESSING_REGS_ACC_CTL_1 0x14UL
#define WB_FOFB_PROCESSING_REGS_ACC_CTL_1_CLEAR 0x1UL
#define WB_FOFB_PROCESSING_REGS_ACC_CTL_1_FREEZE 0x2UL

/* fofb processing accumulator gain register (per channel) */
#define WB_FOFB_PROCESSING_REGS_ACC_GAIN_2 0x18UL
#define WB_FOFB_PROCESSING_REGS_ACC_GAIN_2_VAL_MASK 0xffffffffUL
#define WB_FOFB_PROCESSING_REGS_ACC_GAIN_2_VAL_SHIFT 0

/* fofb processing accumulator control register (per channel) */
#define WB_FOFB_PROCESSING_REGS_ACC_CTL_2 0x1cUL
#define WB_FOFB_PROCESSING_REGS_ACC_CTL_2_CLEAR 0x1UL
#define WB_FOFB_PROCESSING_REGS_ACC_CTL_2_FREEZE 0x2UL

/* fofb processing accumulator gain register (per channel) */
#define WB_FOFB_PROCESSING_REGS_ACC_GAIN_3 0x20UL
#define WB_FOFB_PROCESSING_REGS_ACC_GAIN_3_VAL_MASK 0xffffffffUL
#define WB_FOFB_PROCESSING_REGS_ACC_GAIN_3_VAL_SHIFT 0

/* fofb processing accumulator control register (per channel) */
#define WB_FOFB_PROCESSING_REGS_ACC_CTL_3 0x24UL
#define WB_FOFB_PROCESSING_REGS_ACC_CTL_3_CLEAR 0x1UL
#define WB_FOFB_PROCESSING_REGS_ACC_CTL_3_FREEZE 0x2UL

/* fofb processing accumulator gain register (per channel) */
#define WB_FOFB_PROCESSING_REGS_ACC_GAIN_4 0x28UL
#define WB_FOFB_PROCESSING_REGS_ACC_GAIN_4_VAL_MASK 0xffffffffUL
#define WB_FOFB_PROCESSING_REGS_ACC_GAIN_4_VAL_SHIFT 0

/* fofb processing accumulator control register (per channel) */
#define WB_FOFB_PROCESSING_REGS_ACC_CTL_4 0x2cUL
#define WB_FOFB_PROCESSING_REGS_ACC_CTL_4_CLEAR 0x1UL
#define WB_FOFB_PROCESSING_REGS_ACC_CTL_4_FREEZE 0x2UL

/* fofb processing accumulator gain register (per channel) */
#define WB_FOFB_PROCESSING_REGS_ACC_GAIN_5 0x30UL
#define WB_FOFB_PROCESSING_REGS_ACC_GAIN_5_VAL_MASK 0xffffffffUL
#define WB_FOFB_PROCESSING_REGS_ACC_GAIN_5_VAL_SHIFT 0

/* fofb processing accumulator control register (per channel) */
#define WB_FOFB_PROCESSING_REGS_ACC_CTL_5 0x34UL
#define WB_FOFB_PROCESSING_REGS_ACC_CTL_5_CLEAR 0x1UL
#define WB_FOFB_PROCESSING_REGS_ACC_CTL_5_FREEZE 0x2UL

/* fofb processing accumulator gain register (per channel) */
#define WB_FOFB_PROCESSING_REGS_ACC_GAIN_6 0x38UL
#define WB_FOFB_PROCESSING_REGS_ACC_GAIN_6_VAL_MASK 0xffffffffUL
#define WB_FOFB_PROCESSING_REGS_ACC_GAIN_6_VAL_SHIFT 0

/* fofb processing accumulator control register (per channel) */
#define WB_FOFB_PROCESSING_REGS_ACC_CTL_6 0x3cUL
#define WB_FOFB_PROCESSING_REGS_ACC_CTL_6_CLEAR 0x1UL
#define WB_FOFB_PROCESSING_REGS_ACC_CTL_6_FREEZE 0x2UL

/* fofb processing accumulator gain register (per channel) */
#define WB_FOFB_PROCESSING_REGS_ACC_GAIN_7 0x40UL
#define WB_FOFB_PROCESSING_REGS_ACC_GAIN_7_VAL_MASK 0xffffffffUL
#define WB_FOFB_PROCESSING_REGS_ACC_GAIN_7_VAL_SHIFT 0

/* fofb processing accumulator control register (per channel) */
#define WB_FOFB_PROCESSING_REGS_ACC_CTL_7 0x44UL
#define WB_FOFB_PROCESSING_REGS_ACC_CTL_7_CLEAR 0x1UL
#define WB_FOFB_PROCESSING_REGS_ACC_CTL_7_FREEZE 0x2UL

/* fofb processing accumulator gain register (per channel) */
#define WB_FOFB_PROCESSING_REGS_ACC_GAIN_8 0x48UL
#define WB_FOFB_PROCESSING_REGS_ACC_GAIN_8_VAL_MASK 0xffffffffUL
#define WB_FOFB_PROCESSING_REGS_ACC_GAIN_8_VAL_SHIFT 0

/* fofb processing accumulator control register (per channel) */
#define WB_FOFB_PROCESSING_REGS_ACC_CTL_8 0x4cUL
#define WB_FOFB_PROCESSING_REGS_ACC_CTL_8_CLEAR 0x1UL
#define WB_FOFB_PROCESSING_REGS_ACC_CTL_8_FREEZE 0x2UL

/* fofb processing accumulator gain register (per channel) */
#define WB_FOFB_PROCESSING_REGS_ACC_GAIN_9 0x50UL
#define WB_FOFB_PROCESSING_REGS_ACC_GAIN_9_VAL_MASK 0xffffffffUL
#define WB_FOFB_PROCESSING_REGS_ACC_GAIN_9_VAL_SHIFT 0

/* fofb processing accumulator control register (per channel) */
#define WB_FOFB_PROCESSING_REGS_ACC_CTL_9 0x54UL
#define WB_FOFB_PROCESSING_REGS_ACC_CTL_9_CLEAR 0x1UL
#define WB_FOFB_PROCESSING_REGS_ACC_CTL_9_FREEZE 0x2UL

/* fofb processing accumulator gain register (per channel) */
#define WB_FOFB_PROCESSING_REGS_ACC_GAIN_10 0x58UL
#define WB_FOFB_PROCESSING_REGS_ACC_GAIN_10_VAL_MASK 0xffffffffUL
#define WB_FOFB_PROCESSING_REGS_ACC_GAIN_10_VAL_SHIFT 0

/* fofb processing accumulator control register (per channel) */
#define WB_FOFB_PROCESSING_REGS_ACC_CTL_10 0x5cUL
#define WB_FOFB_PROCESSING_REGS_ACC_CTL_10_CLEAR 0x1UL
#define WB_FOFB_PROCESSING_REGS_ACC_CTL_10_FREEZE 0x2UL

/* fofb processing accumulator gain register (per channel) */
#define WB_FOFB_PROCESSING_REGS_ACC_GAIN_11 0x60UL
#define WB_FOFB_PROCESSING_REGS_ACC_GAIN_11_VAL_MASK 0xffffffffUL
#define WB_FOFB_PROCESSING_REGS_ACC_GAIN_11_VAL_SHIFT 0

/* fofb processing accumulator control register (per channel) */
#define WB_FOFB_PROCESSING_REGS_ACC_CTL_11 0x64UL
#define WB_FOFB_PROCESSING_REGS_ACC_CTL_11_CLEAR 0x1UL
#define WB_FOFB_PROCESSING_REGS_ACC_CTL_11_FREEZE 0x2UL

/* fofb processing maximum saturation value register (per channel) */
#define WB_FOFB_PROCESSING_REGS_SP_MAX_0 0x68UL
#define WB_FOFB_PROCESSING_REGS_SP_MAX_0_VAL_MASK 0xffffffffUL
#define WB_FOFB_PROCESSING_REGS_SP_MAX_0_VAL_SHIFT 0

/* fofb processing minimum saturation value register (per channel) */
#define WB_FOFB_PROCESSING_REGS_SP_MIN_0 0x6cUL
#define WB_FOFB_PROCESSING_REGS_SP_MIN_0_VAL_MASK 0xffffffffUL
#define WB_FOFB_PROCESSING_REGS_SP_MIN_0_VAL_SHIFT 0

/* fofb processing maximum saturation value register (per channel) */
#define WB_FOFB_PROCESSING_REGS_SP_MAX_1 0x70UL
#define WB_FOFB_PROCESSING_REGS_SP_MAX_1_VAL_MASK 0xffffffffUL
#define WB_FOFB_PROCESSING_REGS_SP_MAX_1_VAL_SHIFT 0

/* fofb processing minimum saturation value register (per channel) */
#define WB_FOFB_PROCESSING_REGS_SP_MIN_1 0x74UL
#define WB_FOFB_PROCESSING_REGS_SP_MIN_1_VAL_MASK 0xffffffffUL
#define WB_FOFB_PROCESSING_REGS_SP_MIN_1_VAL_SHIFT 0

/* fofb processing maximum saturation value register (per channel) */
#define WB_FOFB_PROCESSING_REGS_SP_MAX_2 0x78UL
#define WB_FOFB_PROCESSING_REGS_SP_MAX_2_VAL_MASK 0xffffffffUL
#define WB_FOFB_PROCESSING_REGS_SP_MAX_2_VAL_SHIFT 0

/* fofb processing minimum saturation value register (per channel) */
#define WB_FOFB_PROCESSING_REGS_SP_MIN_2 0x7cUL
#define WB_FOFB_PROCESSING_REGS_SP_MIN_2_VAL_MASK 0xffffffffUL
#define WB_FOFB_PROCESSING_REGS_SP_MIN_2_VAL_SHIFT 0

/* fofb processing maximum saturation value register (per channel) */
#define WB_FOFB_PROCESSING_REGS_SP_MAX_3 0x80UL
#define WB_FOFB_PROCESSING_REGS_SP_MAX_3_VAL_MASK 0xffffffffUL
#define WB_FOFB_PROCESSING_REGS_SP_MAX_3_VAL_SHIFT 0

/* fofb processing minimum saturation value register (per channel) */
#define WB_FOFB_PROCESSING_REGS_SP_MIN_3 0x84UL
#define WB_FOFB_PROCESSING_REGS_SP_MIN_3_VAL_MASK 0xffffffffUL
#define WB_FOFB_PROCESSING_REGS_SP_MIN_3_VAL_SHIFT 0

/* fofb processing maximum saturation value register (per channel) */
#define WB_FOFB_PROCESSING_REGS_SP_MAX_4 0x88UL
#define WB_FOFB_PROCESSING_REGS_SP_MAX_4_VAL_MASK 0xffffffffUL
#define WB_FOFB_PROCESSING_REGS_SP_MAX_4_VAL_SHIFT 0

/* fofb processing minimum saturation value register (per channel) */
#define WB_FOFB_PROCESSING_REGS_SP_MIN_4 0x8cUL
#define WB_FOFB_PROCESSING_REGS_SP_MIN_4_VAL_MASK 0xffffffffUL
#define WB_FOFB_PROCESSING_REGS_SP_MIN_4_VAL_SHIFT 0

/* fofb processing maximum saturation value register (per channel) */
#define WB_FOFB_PROCESSING_REGS_SP_MAX_5 0x90UL
#define WB_FOFB_PROCESSING_REGS_SP_MAX_5_VAL_MASK 0xffffffffUL
#define WB_FOFB_PROCESSING_REGS_SP_MAX_5_VAL_SHIFT 0

/* fofb processing minimum saturation value register (per channel) */
#define WB_FOFB_PROCESSING_REGS_SP_MIN_5 0x94UL
#define WB_FOFB_PROCESSING_REGS_SP_MIN_5_VAL_MASK 0xffffffffUL
#define WB_FOFB_PROCESSING_REGS_SP_MIN_5_VAL_SHIFT 0

/* fofb processing maximum saturation value register (per channel) */
#define WB_FOFB_PROCESSING_REGS_SP_MAX_6 0x98UL
#define WB_FOFB_PROCESSING_REGS_SP_MAX_6_VAL_MASK 0xffffffffUL
#define WB_FOFB_PROCESSING_REGS_SP_MAX_6_VAL_SHIFT 0

/* fofb processing minimum saturation value register (per channel) */
#define WB_FOFB_PROCESSING_REGS_SP_MIN_6 0x9cUL
#define WB_FOFB_PROCESSING_REGS_SP_MIN_6_VAL_MASK 0xffffffffUL
#define WB_FOFB_PROCESSING_REGS_SP_MIN_6_VAL_SHIFT 0

/* fofb processing maximum saturation value register (per channel) */
#define WB_FOFB_PROCESSING_REGS_SP_MAX_7 0xa0UL
#define WB_FOFB_PROCESSING_REGS_SP_MAX_7_VAL_MASK 0xffffffffUL
#define WB_FOFB_PROCESSING_REGS_SP_MAX_7_VAL_SHIFT 0

/* fofb processing minimum saturation value register (per channel) */
#define WB_FOFB_PROCESSING_REGS_SP_MIN_7 0xa4UL
#define WB_FOFB_PROCESSING_REGS_SP_MIN_7_VAL_MASK 0xffffffffUL
#define WB_FOFB_PROCESSING_REGS_SP_MIN_7_VAL_SHIFT 0

/* fofb processing maximum saturation value register (per channel) */
#define WB_FOFB_PROCESSING_REGS_SP_MAX_8 0xa8UL
#define WB_FOFB_PROCESSING_REGS_SP_MAX_8_VAL_MASK 0xffffffffUL
#define WB_FOFB_PROCESSING_REGS_SP_MAX_8_VAL_SHIFT 0

/* fofb processing minimum saturation value register (per channel) */
#define WB_FOFB_PROCESSING_REGS_SP_MIN_8 0xacUL
#define WB_FOFB_PROCESSING_REGS_SP_MIN_8_VAL_MASK 0xffffffffUL
#define WB_FOFB_PROCESSING_REGS_SP_MIN_8_VAL_SHIFT 0

/* fofb processing maximum saturation value register (per channel) */
#define WB_FOFB_PROCESSING_REGS_SP_MAX_9 0xb0UL
#define WB_FOFB_PROCESSING_REGS_SP_MAX_9_VAL_MASK 0xffffffffUL
#define WB_FOFB_PROCESSING_REGS_SP_MAX_9_VAL_SHIFT 0

/* fofb processing minimum saturation value register (per channel) */
#define WB_FOFB_PROCESSING_REGS_SP_MIN_9 0xb4UL
#define WB_FOFB_PROCESSING_REGS_SP_MIN_9_VAL_MASK 0xffffffffUL
#define WB_FOFB_PROCESSING_REGS_SP_MIN_9_VAL_SHIFT 0

/* fofb processing maximum saturation value register (per channel) */
#define WB_FOFB_PROCESSING_REGS_SP_MAX_10 0xb8UL
#define WB_FOFB_PROCESSING_REGS_SP_MAX_10_VAL_MASK 0xffffffffUL
#define WB_FOFB_PROCESSING_REGS_SP_MAX_10_VAL_SHIFT 0

/* fofb processing minimum saturation value register (per channel) */
#define WB_FOFB_PROCESSING_REGS_SP_MIN_10 0xbcUL
#define WB_FOFB_PROCESSING_REGS_SP_MIN_10_VAL_MASK 0xffffffffUL
#define WB_FOFB_PROCESSING_REGS_SP_MIN_10_VAL_SHIFT 0

/* fofb processing maximum saturation value register (per channel) */
#define WB_FOFB_PROCESSING_REGS_SP_MAX_11 0xc0UL
#define WB_FOFB_PROCESSING_REGS_SP_MAX_11_VAL_MASK 0xffffffffUL
#define WB_FOFB_PROCESSING_REGS_SP_MAX_11_VAL_SHIFT 0

/* fofb processing minimum saturation value register (per channel) */
#define WB_FOFB_PROCESSING_REGS_SP_MIN_11 0xc4UL
#define WB_FOFB_PROCESSING_REGS_SP_MIN_11_VAL_MASK 0xffffffffUL
#define WB_FOFB_PROCESSING_REGS_SP_MIN_11_VAL_SHIFT 0

/* fofb processing coefficients ram bank */
#define WB_FOFB_PROCESSING_REGS_COEFFS_RAM_BANK_0 0x800UL
#define WB_FOFB_PROCESSING_REGS_COEFFS_RAM_BANK_0_SIZE 4 /* 0x4 */

/* None */
#define WB_FOFB_PROCESSING_REGS_COEFFS_RAM_BANK_0_DATA 0x0UL

/* fofb processing coefficients ram bank */
#define WB_FOFB_PROCESSING_REGS_COEFFS_RAM_BANK_1 0x1000UL
#define WB_FOFB_PROCESSING_REGS_COEFFS_RAM_BANK_1_SIZE 4 /* 0x4 */

/* None */
#define WB_FOFB_PROCESSING_REGS_COEFFS_RAM_BANK_1_DATA 0x0UL

/* fofb processing coefficients ram bank */
#define WB_FOFB_PROCESSING_REGS_COEFFS_RAM_BANK_2 0x1800UL
#define WB_FOFB_PROCESSING_REGS_COEFFS_RAM_BANK_2_SIZE 4 /* 0x4 */

/* None */
#define WB_FOFB_PROCESSING_REGS_COEFFS_RAM_BANK_2_DATA 0x0UL

/* fofb processing coefficients ram bank */
#define WB_FOFB_PROCESSING_REGS_COEFFS_RAM_BANK_3 0x2000UL
#define WB_FOFB_PROCESSING_REGS_COEFFS_RAM_BANK_3_SIZE 4 /* 0x4 */

/* None */
#define WB_FOFB_PROCESSING_REGS_COEFFS_RAM_BANK_3_DATA 0x0UL

/* fofb processing coefficients ram bank */
#define WB_FOFB_PROCESSING_REGS_COEFFS_RAM_BANK_4 0x2800UL
#define WB_FOFB_PROCESSING_REGS_COEFFS_RAM_BANK_4_SIZE 4 /* 0x4 */

/* None */
#define WB_FOFB_PROCESSING_REGS_COEFFS_RAM_BANK_4_DATA 0x0UL

/* fofb processing coefficients ram bank */
#define WB_FOFB_PROCESSING_REGS_COEFFS_RAM_BANK_5 0x3000UL
#define WB_FOFB_PROCESSING_REGS_COEFFS_RAM_BANK_5_SIZE 4 /* 0x4 */

/* None */
#define WB_FOFB_PROCESSING_REGS_COEFFS_RAM_BANK_5_DATA 0x0UL

/* fofb processing coefficients ram bank */
#define WB_FOFB_PROCESSING_REGS_COEFFS_RAM_BANK_6 0x3800UL
#define WB_FOFB_PROCESSING_REGS_COEFFS_RAM_BANK_6_SIZE 4 /* 0x4 */

/* None */
#define WB_FOFB_PROCESSING_REGS_COEFFS_RAM_BANK_6_DATA 0x0UL

/* fofb processing coefficients ram bank */
#define WB_FOFB_PROCESSING_REGS_COEFFS_RAM_BANK_7 0x4000UL
#define WB_FOFB_PROCESSING_REGS_COEFFS_RAM_BANK_7_SIZE 4 /* 0x4 */

/* None */
#define WB_FOFB_PROCESSING_REGS_COEFFS_RAM_BANK_7_DATA 0x0UL

/* fofb processing coefficients ram bank */
#define WB_FOFB_PROCESSING_REGS_COEFFS_RAM_BANK_8 0x4800UL
#define WB_FOFB_PROCESSING_REGS_COEFFS_RAM_BANK_8_SIZE 4 /* 0x4 */

/* None */
#define WB_FOFB_PROCESSING_REGS_COEFFS_RAM_BANK_8_DATA 0x0UL

/* fofb processing coefficients ram bank */
#define WB_FOFB_PROCESSING_REGS_COEFFS_RAM_BANK_9 0x5000UL
#define WB_FOFB_PROCESSING_REGS_COEFFS_RAM_BANK_9_SIZE 4 /* 0x4 */

/* None */
#define WB_FOFB_PROCESSING_REGS_COEFFS_RAM_BANK_9_DATA 0x0UL

/* fofb processing coefficients ram bank */
#define WB_FOFB_PROCESSING_REGS_COEFFS_RAM_BANK_10 0x5800UL
#define WB_FOFB_PROCESSING_REGS_COEFFS_RAM_BANK_10_SIZE 4 /* 0x4 */

/* None */
#define WB_FOFB_PROCESSING_REGS_COEFFS_RAM_BANK_10_DATA 0x0UL

/* fofb processing coefficients ram bank */
#define WB_FOFB_PROCESSING_REGS_COEFFS_RAM_BANK_11 0x6000UL
#define WB_FOFB_PROCESSING_REGS_COEFFS_RAM_BANK_11_SIZE 4 /* 0x4 */

/* None */
#define WB_FOFB_PROCESSING_REGS_COEFFS_RAM_BANK_11_DATA 0x0UL

/* fofb processing setpoints ram bank */
#define WB_FOFB_PROCESSING_REGS_SETPOINTS_RAM_BANK 0x6800UL
#define WB_FOFB_PROCESSING_REGS_SETPOINTS_RAM_BANK_SIZE 4 /* 0x4 */

/* None */
#define WB_FOFB_PROCESSING_REGS_SETPOINTS_RAM_BANK_DATA 0x0UL

struct wb_fofb_processing_regs {
  /* [0x0]: REG (rw) fofb processing coefficients fixed-point position constant */
  uint32_t coeffs_fixed_point_pos;

  /* [0x4]: REG (ro) fofb processing accumulators' gains fixed-point position register */
  uint32_t accs_gains_fixed_point_pos;

  /* [0x8]: REG (rw) fofb processing accumulator gain register (per channel) */
  uint32_t acc_gain_0;

  /* [0xc]: REG (rw) fofb processing accumulator control register (per channel) */
  uint32_t acc_ctl_0;

  /* [0x10]: REG (rw) fofb processing accumulator gain register (per channel) */
  uint32_t acc_gain_1;

  /* [0x14]: REG (rw) fofb processing accumulator control register (per channel) */
  uint32_t acc_ctl_1;

  /* [0x18]: REG (rw) fofb processing accumulator gain register (per channel) */
  uint32_t acc_gain_2;

  /* [0x1c]: REG (rw) fofb processing accumulator control register (per channel) */
  uint32_t acc_ctl_2;

  /* [0x20]: REG (rw) fofb processing accumulator gain register (per channel) */
  uint32_t acc_gain_3;

  /* [0x24]: REG (rw) fofb processing accumulator control register (per channel) */
  uint32_t acc_ctl_3;

  /* [0x28]: REG (rw) fofb processing accumulator gain register (per channel) */
  uint32_t acc_gain_4;

  /* [0x2c]: REG (rw) fofb processing accumulator control register (per channel) */
  uint32_t acc_ctl_4;

  /* [0x30]: REG (rw) fofb processing accumulator gain register (per channel) */
  uint32_t acc_gain_5;

  /* [0x34]: REG (rw) fofb processing accumulator control register (per channel) */
  uint32_t acc_ctl_5;

  /* [0x38]: REG (rw) fofb processing accumulator gain register (per channel) */
  uint32_t acc_gain_6;

  /* [0x3c]: REG (rw) fofb processing accumulator control register (per channel) */
  uint32_t acc_ctl_6;

  /* [0x40]: REG (rw) fofb processing accumulator gain register (per channel) */
  uint32_t acc_gain_7;

  /* [0x44]: REG (rw) fofb processing accumulator control register (per channel) */
  uint32_t acc_ctl_7;

  /* [0x48]: REG (rw) fofb processing accumulator gain register (per channel) */
  uint32_t acc_gain_8;

  /* [0x4c]: REG (rw) fofb processing accumulator control register (per channel) */
  uint32_t acc_ctl_8;

  /* [0x50]: REG (rw) fofb processing accumulator gain register (per channel) */
  uint32_t acc_gain_9;

  /* [0x54]: REG (rw) fofb processing accumulator control register (per channel) */
  uint32_t acc_ctl_9;

  /* [0x58]: REG (rw) fofb processing accumulator gain register (per channel) */
  uint32_t acc_gain_10;

  /* [0x5c]: REG (rw) fofb processing accumulator control register (per channel) */
  uint32_t acc_ctl_10;

  /* [0x60]: REG (rw) fofb processing accumulator gain register (per channel) */
  uint32_t acc_gain_11;

  /* [0x64]: REG (rw) fofb processing accumulator control register (per channel) */
  uint32_t acc_ctl_11;

  /* [0x68]: REG (rw) fofb processing maximum saturation value register (per channel) */
  uint32_t sp_max_0;

  /* [0x6c]: REG (rw) fofb processing minimum saturation value register (per channel) */
  uint32_t sp_min_0;

  /* [0x70]: REG (rw) fofb processing maximum saturation value register (per channel) */
  uint32_t sp_max_1;

  /* [0x74]: REG (rw) fofb processing minimum saturation value register (per channel) */
  uint32_t sp_min_1;

  /* [0x78]: REG (rw) fofb processing maximum saturation value register (per channel) */
  uint32_t sp_max_2;

  /* [0x7c]: REG (rw) fofb processing minimum saturation value register (per channel) */
  uint32_t sp_min_2;

  /* [0x80]: REG (rw) fofb processing maximum saturation value register (per channel) */
  uint32_t sp_max_3;

  /* [0x84]: REG (rw) fofb processing minimum saturation value register (per channel) */
  uint32_t sp_min_3;

  /* [0x88]: REG (rw) fofb processing maximum saturation value register (per channel) */
  uint32_t sp_max_4;

  /* [0x8c]: REG (rw) fofb processing minimum saturation value register (per channel) */
  uint32_t sp_min_4;

  /* [0x90]: REG (rw) fofb processing maximum saturation value register (per channel) */
  uint32_t sp_max_5;

  /* [0x94]: REG (rw) fofb processing minimum saturation value register (per channel) */
  uint32_t sp_min_5;

  /* [0x98]: REG (rw) fofb processing maximum saturation value register (per channel) */
  uint32_t sp_max_6;

  /* [0x9c]: REG (rw) fofb processing minimum saturation value register (per channel) */
  uint32_t sp_min_6;

  /* [0xa0]: REG (rw) fofb processing maximum saturation value register (per channel) */
  uint32_t sp_max_7;

  /* [0xa4]: REG (rw) fofb processing minimum saturation value register (per channel) */
  uint32_t sp_min_7;

  /* [0xa8]: REG (rw) fofb processing maximum saturation value register (per channel) */
  uint32_t sp_max_8;

  /* [0xac]: REG (rw) fofb processing minimum saturation value register (per channel) */
  uint32_t sp_min_8;

  /* [0xb0]: REG (rw) fofb processing maximum saturation value register (per channel) */
  uint32_t sp_max_9;

  /* [0xb4]: REG (rw) fofb processing minimum saturation value register (per channel) */
  uint32_t sp_min_9;

  /* [0xb8]: REG (rw) fofb processing maximum saturation value register (per channel) */
  uint32_t sp_max_10;

  /* [0xbc]: REG (rw) fofb processing minimum saturation value register (per channel) */
  uint32_t sp_min_10;

  /* [0xc0]: REG (rw) fofb processing maximum saturation value register (per channel) */
  uint32_t sp_max_11;

  /* [0xc4]: REG (rw) fofb processing minimum saturation value register (per channel) */
  uint32_t sp_min_11;

  /* padding to: 512 words */
  uint32_t __padding_0[462];

  /* [0x800]: MEMORY fofb processing coefficients ram bank */
  struct coeffs_ram_bank_0 {
    /* [0x0]: REG (rw) (no description) */
    uint32_t data;
  } coeffs_ram_bank_0[512];

  /* [0x1000]: MEMORY fofb processing coefficients ram bank */
  struct coeffs_ram_bank_1 {
    /* [0x0]: REG (rw) (no description) */
    uint32_t data;
  } coeffs_ram_bank_1[512];

  /* [0x1800]: MEMORY fofb processing coefficients ram bank */
  struct coeffs_ram_bank_2 {
    /* [0x0]: REG (rw) (no description) */
    uint32_t data;
  } coeffs_ram_bank_2[512];

  /* [0x2000]: MEMORY fofb processing coefficients ram bank */
  struct coeffs_ram_bank_3 {
    /* [0x0]: REG (rw) (no description) */
    uint32_t data;
  } coeffs_ram_bank_3[512];

  /* [0x2800]: MEMORY fofb processing coefficients ram bank */
  struct coeffs_ram_bank_4 {
    /* [0x0]: REG (rw) (no description) */
    uint32_t data;
  } coeffs_ram_bank_4[512];

  /* [0x3000]: MEMORY fofb processing coefficients ram bank */
  struct coeffs_ram_bank_5 {
    /* [0x0]: REG (rw) (no description) */
    uint32_t data;
  } coeffs_ram_bank_5[512];

  /* [0x3800]: MEMORY fofb processing coefficients ram bank */
  struct coeffs_ram_bank_6 {
    /* [0x0]: REG (rw) (no description) */
    uint32_t data;
  } coeffs_ram_bank_6[512];

  /* [0x4000]: MEMORY fofb processing coefficients ram bank */
  struct coeffs_ram_bank_7 {
    /* [0x0]: REG (rw) (no description) */
    uint32_t data;
  } coeffs_ram_bank_7[512];

  /* [0x4800]: MEMORY fofb processing coefficients ram bank */
  struct coeffs_ram_bank_8 {
    /* [0x0]: REG (rw) (no description) */
    uint32_t data;
  } coeffs_ram_bank_8[512];

  /* [0x5000]: MEMORY fofb processing coefficients ram bank */
  struct coeffs_ram_bank_9 {
    /* [0x0]: REG (rw) (no description) */
    uint32_t data;
  } coeffs_ram_bank_9[512];

  /* [0x5800]: MEMORY fofb processing coefficients ram bank */
  struct coeffs_ram_bank_10 {
    /* [0x0]: REG (rw) (no description) */
    uint32_t data;
  } coeffs_ram_bank_10[512];

  /* [0x6000]: MEMORY fofb processing coefficients ram bank */
  struct coeffs_ram_bank_11 {
    /* [0x0]: REG (rw) (no description) */
    uint32_t data;
  } coeffs_ram_bank_11[512];

  /* [0x6800]: MEMORY fofb processing setpoints ram bank */
  struct setpoints_ram_bank {
    /* [0x0]: REG (rw) (no description) */
    uint32_t data;
  } setpoints_ram_bank[512];
};

#endif /* __CHEBY__WB_FOFB_PROCESSING_REGS__H__ */
