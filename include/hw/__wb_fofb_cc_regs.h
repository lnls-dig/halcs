#ifndef __CHEBY__FOFB_CC_REGS__H__
#define __CHEBY__FOFB_CC_REGS__H__
#define FOFB_CC_REGS_SIZE 16384 /* 0x4000 = 16KB */

/* FOFB CC configuration register */
#define FOFB_CC_REGS_CFG_VAL 0x0UL
#define FOFB_CC_REGS_CFG_VAL_ACT_PART 0x1UL
#define FOFB_CC_REGS_CFG_VAL_UNUSED 0x2UL
#define FOFB_CC_REGS_CFG_VAL_ERR_CLR 0x4UL
#define FOFB_CC_REGS_CFG_VAL_CC_ENABLE 0x8UL
#define FOFB_CC_REGS_CFG_VAL_TFS_OVERRIDE 0x10UL

/* FOFB CC Time-of-Arrival configuration register */
#define FOFB_CC_REGS_TOA_CTL 0x4UL
#define FOFB_CC_REGS_TOA_CTL_RD_EN 0x1UL
#define FOFB_CC_REGS_TOA_CTL_RD_STR 0x2UL

/* FOFB CC Time-of-Arrival data */
#define FOFB_CC_REGS_TOA_DATA 0x8UL
#define FOFB_CC_REGS_TOA_DATA_VAL_MASK 0xffffffffUL
#define FOFB_CC_REGS_TOA_DATA_VAL_SHIFT 0

/* FOFB CC Received Buffer configuration register */
#define FOFB_CC_REGS_RCB_CTL 0xcUL
#define FOFB_CC_REGS_RCB_CTL_RD_EN 0x1UL
#define FOFB_CC_REGS_RCB_CTL_RD_STR 0x2UL

/* FOFB CC Received Buffer data */
#define FOFB_CC_REGS_RCB_DATA 0x10UL
#define FOFB_CC_REGS_RCB_DATA_VAL_MASK 0xffffffffUL
#define FOFB_CC_REGS_RCB_DATA_VAL_SHIFT 0

/* FOFB CC X/Y buffer configuration register */
#define FOFB_CC_REGS_XY_BUFF_CTL 0x14UL
#define FOFB_CC_REGS_XY_BUFF_CTL_UNUSED_MASK 0xffffUL
#define FOFB_CC_REGS_XY_BUFF_CTL_UNUSED_SHIFT 0
#define FOFB_CC_REGS_XY_BUFF_CTL_ADDR_MASK 0xffff0000UL
#define FOFB_CC_REGS_XY_BUFF_CTL_ADDR_SHIFT 16

/* FOFB CC X/Y buffer MSB */
#define FOFB_CC_REGS_XY_BUFF_DATA_MSB 0x18UL
#define FOFB_CC_REGS_XY_BUFF_DATA_MSB_VAL_MASK 0xffffffffUL
#define FOFB_CC_REGS_XY_BUFF_DATA_MSB_VAL_SHIFT 0

/* FOFB CC X/Y buffer LSB */
#define FOFB_CC_REGS_XY_BUFF_DATA_LSB 0x1cUL
#define FOFB_CC_REGS_XY_BUFF_DATA_LSB_VAL_MASK 0xffffffffUL
#define FOFB_CC_REGS_XY_BUFF_DATA_LSB_VAL_SHIFT 0

/* FOFB CC RAM for register map */
#define FOFB_CC_REGS_RAM_REG 0x2000UL
#define FOFB_CC_REGS_RAM_REG_SIZE 4 /* 0x4 */

/* None */
#define FOFB_CC_REGS_RAM_REG_DATA 0x0UL

struct fofb_cc_regs {
  /* [0x0]: REG (rw) FOFB CC configuration register */
  uint32_t cfg_val;

  /* [0x4]: REG (rw) FOFB CC Time-of-Arrival configuration register */
  uint32_t toa_ctl;

  /* [0x8]: REG (rw) FOFB CC Time-of-Arrival data */
  uint32_t toa_data;

  /* [0xc]: REG (rw) FOFB CC Received Buffer configuration register */
  uint32_t rcb_ctl;

  /* [0x10]: REG (rw) FOFB CC Received Buffer data */
  uint32_t rcb_data;

  /* [0x14]: REG (rw) FOFB CC X/Y buffer configuration register */
  uint32_t xy_buff_ctl;

  /* [0x18]: REG (rw) FOFB CC X/Y buffer MSB */
  uint32_t xy_buff_data_msb;

  /* [0x1c]: REG (rw) FOFB CC X/Y buffer LSB */
  uint32_t xy_buff_data_lsb;

  /* padding to: 2048 words */
  uint32_t __padding_0[2040];

  /* [0x2000]: MEMORY FOFB CC RAM for register map */
  struct ram_reg {
    /* [0x0]: REG (rw) (no description) */
    uint32_t data;
  } ram_reg[2048];
};

#endif /* __CHEBY__FOFB_CC_REGS__H__ */
