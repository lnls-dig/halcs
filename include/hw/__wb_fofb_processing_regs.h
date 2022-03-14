#ifndef __CHEBY__WB_FOFB_PROCESSING_REGS__H__
#define __CHEBY__WB_FOFB_PROCESSING_REGS__H__
#define WB_FOFB_PROCESSING_REGS_SIZE 24576 /* 0x6000 = 24KB */

/* FOFB PROCESSING RAM bank for register map */
#define WB_FOFB_PROCESSING_REGS_RAM_BANK_0 0x0UL
#define WB_FOFB_PROCESSING_REGS_RAM_BANK_0_SIZE 4 /* 0x4 */

/* None */
#define WB_FOFB_PROCESSING_REGS_RAM_BANK_0_DATA 0x0UL

/* FOFB PROCESSING RAM bank for register map */
#define WB_FOFB_PROCESSING_REGS_RAM_BANK_1 0x800UL
#define WB_FOFB_PROCESSING_REGS_RAM_BANK_1_SIZE 4 /* 0x4 */

/* None */
#define WB_FOFB_PROCESSING_REGS_RAM_BANK_1_DATA 0x0UL

/* FOFB PROCESSING RAM bank for register map */
#define WB_FOFB_PROCESSING_REGS_RAM_BANK_2 0x1000UL
#define WB_FOFB_PROCESSING_REGS_RAM_BANK_2_SIZE 4 /* 0x4 */

/* None */
#define WB_FOFB_PROCESSING_REGS_RAM_BANK_2_DATA 0x0UL

/* FOFB PROCESSING RAM bank for register map */
#define WB_FOFB_PROCESSING_REGS_RAM_BANK_3 0x1800UL
#define WB_FOFB_PROCESSING_REGS_RAM_BANK_3_SIZE 4 /* 0x4 */

/* None */
#define WB_FOFB_PROCESSING_REGS_RAM_BANK_3_DATA 0x0UL

/* FOFB PROCESSING RAM bank for register map */
#define WB_FOFB_PROCESSING_REGS_RAM_BANK_4 0x2000UL
#define WB_FOFB_PROCESSING_REGS_RAM_BANK_4_SIZE 4 /* 0x4 */

/* None */
#define WB_FOFB_PROCESSING_REGS_RAM_BANK_4_DATA 0x0UL

/* FOFB PROCESSING RAM bank for register map */
#define WB_FOFB_PROCESSING_REGS_RAM_BANK_5 0x2800UL
#define WB_FOFB_PROCESSING_REGS_RAM_BANK_5_SIZE 4 /* 0x4 */

/* None */
#define WB_FOFB_PROCESSING_REGS_RAM_BANK_5_DATA 0x0UL

/* FOFB PROCESSING RAM bank for register map */
#define WB_FOFB_PROCESSING_REGS_RAM_BANK_6 0x3000UL
#define WB_FOFB_PROCESSING_REGS_RAM_BANK_6_SIZE 4 /* 0x4 */

/* None */
#define WB_FOFB_PROCESSING_REGS_RAM_BANK_6_DATA 0x0UL

/* FOFB PROCESSING RAM bank for register map */
#define WB_FOFB_PROCESSING_REGS_RAM_BANK_7 0x3800UL
#define WB_FOFB_PROCESSING_REGS_RAM_BANK_7_SIZE 4 /* 0x4 */

/* None */
#define WB_FOFB_PROCESSING_REGS_RAM_BANK_7_DATA 0x0UL

/* FOFB PROCESSING RAM bank for register map */
#define WB_FOFB_PROCESSING_REGS_RAM_BANK_8 0x4000UL
#define WB_FOFB_PROCESSING_REGS_RAM_BANK_8_SIZE 4 /* 0x4 */

/* None */
#define WB_FOFB_PROCESSING_REGS_RAM_BANK_8_DATA 0x0UL

/* FOFB PROCESSING RAM bank for register map */
#define WB_FOFB_PROCESSING_REGS_RAM_BANK_9 0x4800UL
#define WB_FOFB_PROCESSING_REGS_RAM_BANK_9_SIZE 4 /* 0x4 */

/* None */
#define WB_FOFB_PROCESSING_REGS_RAM_BANK_9_DATA 0x0UL

/* FOFB PROCESSING RAM bank for register map */
#define WB_FOFB_PROCESSING_REGS_RAM_BANK_10 0x5000UL
#define WB_FOFB_PROCESSING_REGS_RAM_BANK_10_SIZE 4 /* 0x4 */

/* None */
#define WB_FOFB_PROCESSING_REGS_RAM_BANK_10_DATA 0x0UL

/* FOFB PROCESSING RAM bank for register map */
#define WB_FOFB_PROCESSING_REGS_RAM_BANK_11 0x5800UL
#define WB_FOFB_PROCESSING_REGS_RAM_BANK_11_SIZE 4 /* 0x4 */

/* None */
#define WB_FOFB_PROCESSING_REGS_RAM_BANK_11_DATA 0x0UL

struct wb_fofb_processing_regs {
  /* [0x0]: MEMORY FOFB PROCESSING RAM bank for register map */
  struct ram_bank_0 {
    /* [0x0]: REG (rw) (no description) */
    uint32_t data;
  } ram_bank_0[512];

  /* [0x800]: MEMORY FOFB PROCESSING RAM bank for register map */
  struct ram_bank_1 {
    /* [0x0]: REG (rw) (no description) */
    uint32_t data;
  } ram_bank_1[512];

  /* [0x1000]: MEMORY FOFB PROCESSING RAM bank for register map */
  struct ram_bank_2 {
    /* [0x0]: REG (rw) (no description) */
    uint32_t data;
  } ram_bank_2[512];

  /* [0x1800]: MEMORY FOFB PROCESSING RAM bank for register map */
  struct ram_bank_3 {
    /* [0x0]: REG (rw) (no description) */
    uint32_t data;
  } ram_bank_3[512];

  /* [0x2000]: MEMORY FOFB PROCESSING RAM bank for register map */
  struct ram_bank_4 {
    /* [0x0]: REG (rw) (no description) */
    uint32_t data;
  } ram_bank_4[512];

  /* [0x2800]: MEMORY FOFB PROCESSING RAM bank for register map */
  struct ram_bank_5 {
    /* [0x0]: REG (rw) (no description) */
    uint32_t data;
  } ram_bank_5[512];

  /* [0x3000]: MEMORY FOFB PROCESSING RAM bank for register map */
  struct ram_bank_6 {
    /* [0x0]: REG (rw) (no description) */
    uint32_t data;
  } ram_bank_6[512];

  /* [0x3800]: MEMORY FOFB PROCESSING RAM bank for register map */
  struct ram_bank_7 {
    /* [0x0]: REG (rw) (no description) */
    uint32_t data;
  } ram_bank_7[512];

  /* [0x4000]: MEMORY FOFB PROCESSING RAM bank for register map */
  struct ram_bank_8 {
    /* [0x0]: REG (rw) (no description) */
    uint32_t data;
  } ram_bank_8[512];

  /* [0x4800]: MEMORY FOFB PROCESSING RAM bank for register map */
  struct ram_bank_9 {
    /* [0x0]: REG (rw) (no description) */
    uint32_t data;
  } ram_bank_9[512];

  /* [0x5000]: MEMORY FOFB PROCESSING RAM bank for register map */
  struct ram_bank_10 {
    /* [0x0]: REG (rw) (no description) */
    uint32_t data;
  } ram_bank_10[512];

  /* [0x5800]: MEMORY FOFB PROCESSING RAM bank for register map */
  struct ram_bank_11 {
    /* [0x0]: REG (rw) (no description) */
    uint32_t data;
  } ram_bank_11[512];
};

#endif /* __CHEBY__WB_FOFB_PROCESSING_REGS__H__ */
