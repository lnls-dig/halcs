#ifndef __CHEBY__WB_FOFB_PROCESSING_REGS__H__
#define __CHEBY__WB_FOFB_PROCESSING_REGS__H__
#define WB_FOFB_PROCESSING_REGS_SIZE 28672 /* 0x7000 = 28KB */

/* fofb processing coefficients fixed-point position constant */
#define WB_FOFB_PROCESSING_REGS_COEFFS_FIXED_POINT_POS 0x0UL
#define WB_FOFB_PROCESSING_REGS_COEFFS_FIXED_POINT_POS_VAL_MASK 0xffffffffUL
#define WB_FOFB_PROCESSING_REGS_COEFFS_FIXED_POINT_POS_VAL_SHIFT 0

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

  /* padding to: 512 words */
  uint32_t __padding_0[511];

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
