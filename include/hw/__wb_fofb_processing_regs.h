#ifndef __CHEBY__WB_FOFB_PROCESSING_REGS__H__
#define __CHEBY__WB_FOFB_PROCESSING_REGS__H__
#define WB_FOFB_PROCESSING_REGS_SIZE 16 /* 0x10 */

/* RAM write register */
#define WB_FOFB_PROCESSING_REGS_RAM_WRITE 0x0UL
#define WB_FOFB_PROCESSING_REGS_RAM_WRITE_ENABLE 0x1UL

/* RAM data input register */
#define WB_FOFB_PROCESSING_REGS_RAM_DATA_IN 0x4UL
#define WB_FOFB_PROCESSING_REGS_RAM_DATA_IN_VAL_MASK 0xffffffffUL
#define WB_FOFB_PROCESSING_REGS_RAM_DATA_IN_VAL_SHIFT 0

/* RAM data output register */
#define WB_FOFB_PROCESSING_REGS_RAM_DATA_OUT 0x8UL
#define WB_FOFB_PROCESSING_REGS_RAM_DATA_OUT_VAL_MASK 0xffffffffUL
#define WB_FOFB_PROCESSING_REGS_RAM_DATA_OUT_VAL_SHIFT 0

/* RAM address register */
#define WB_FOFB_PROCESSING_REGS_RAM_ADDR 0xcUL
#define WB_FOFB_PROCESSING_REGS_RAM_ADDR_VAL_MASK 0xfffUL
#define WB_FOFB_PROCESSING_REGS_RAM_ADDR_VAL_SHIFT 0

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
