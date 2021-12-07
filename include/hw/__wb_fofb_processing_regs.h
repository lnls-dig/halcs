#ifndef __CHEBY__WB_FOFB_PROCESSING_REGS__H__
#define __CHEBY__WB_FOFB_PROCESSING_REGS__H__
#define WB_FOFB_PROCESSING_REGS_SIZE 16 /* 0x10 */

/* RAM write register */
#define WB_FOFB_PROCESSING_REGS_RAM_WRITE              0x00000000
#define WB_FOFB_PROCESSING_REGS_RAM_WRITE_ENABLE       0x00000001

/* RAM data input register */
#define WB_FOFB_PROCESSING_REGS_RAM_DATA_IN            0x00000004
#define WB_FOFB_PROCESSING_REGS_RAM_DATA_IN_VAL_MASK   0xffffffff
#define WB_FOFB_PROCESSING_REGS_RAM_DATA_IN_VAL_SHIFT  0

/* RAM data output register */
#define WB_FOFB_PROCESSING_REGS_RAM_DATA_OUT           0x00000008
#define WB_FOFB_PROCESSING_REGS_RAM_DATA_OUT_VAL_MASK  0xffffffff
#define WB_FOFB_PROCESSING_REGS_RAM_DATA_OUT_VAL_SHIFT 0

/* RAM address register */
#define WB_FOFB_PROCESSING_REGS_RAM_ADDR               0x0000000c
#define WB_FOFB_PROCESSING_REGS_RAM_ADDR_VAL_MASK      0x00000fff
#define WB_FOFB_PROCESSING_REGS_RAM_ADDR_VAL_SHIFT     0

struct wb_fofb_processing_regs {
  /* [0x0]: REG (rw) RAM write register */
  uint32_t ram_write;

  /* [0x4]: REG (rw) RAM data input register */
  uint32_t ram_data_in;

  /* [0x8]: REG (ro) RAM data output register */
  uint32_t ram_data_out;

  /* [0xc]: REG (rw) RAM address register */
  uint32_t ram_addr;
};

#endif /* __CHEBY__WB_FOFB_PROCESSING_REGS__H__ */
