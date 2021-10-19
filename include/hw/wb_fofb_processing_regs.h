#ifndef __CHEBY__WB_FOFB_PROCESSING__H__
#define __CHEBY__WB_FOFB_PROCESSING__H__
#define WB_FOFB_PROCESSING_SIZE 12 /* 0xc */

/* None */
#define WB_FOFB_PROCESSING_RAM_COEFF_DAT 0x0UL

/* None */
#define WB_FOFB_PROCESSING_RAM_COEFF_ADDR 0x4UL

/* None */
#define WB_FOFB_PROCESSING_RAM_WRITE 0x8UL
#define WB_FOFB_PROCESSING_RAM_WRITE_ENABLE 0x1UL

struct wb_fofb_processing {
  /* [0x0]: REG (rw) (no description) */
  uint32_t ram_coeff_dat;

  /* [0x4]: REG (rw) (no description) */
  uint32_t ram_coeff_addr;

  /* [0x8]: REG (rw) (no description) */
  uint32_t ram_write;
};

#endif /* __CHEBY__WB_FOFB_PROCESSING__H__ */
