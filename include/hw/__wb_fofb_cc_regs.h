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

/* FOFB CC RAM for register map */
#define FOFB_CC_REGS_RAM_REG 0x2000UL
#define FOFB_CC_REGS_RAM_REG_SIZE 4 /* 0x4 */

/* None */
#define FOFB_CC_REGS_RAM_REG_DATA 0x0UL

struct fofb_cc_regs {
  /* [0x0]: REG (rw) FOFB CC configuration register */
  uint32_t cfg_val;

  /* padding to: 2048 words */
  uint32_t __padding_0[2047];

  /* [0x2000]: MEMORY FOFB CC RAM for register map */
  struct ram_reg {
    /* [0x0]: REG (rw) (no description) */
    uint32_t data;
  } ram_reg[2048];
};

#endif /* __CHEBY__FOFB_CC_REGS__H__ */
