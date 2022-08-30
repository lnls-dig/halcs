#ifndef __WB_FOFB_PROCESSING_REGS_H__
#define __WB_FOFB_PROCESSING_REGS_H__

#include "__wb_fofb_processing_regs.h"

#ifdef __KERNEL__
#include <linux/types.h>
#else
#include <inttypes.h>
#endif

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

#define FOFB_PROCESSING_REGS_RAM_BANK_SIZE                                     \
  (WB_FOFB_PROCESSING_REGS_RAM_BANK_1 - WB_FOFB_PROCESSING_REGS_RAM_BANK_0)

#define FOFB_PROCESSING_REGS_RAM_BANK_OFFS(chan)                               \
  (WB_FOFB_PROCESSING_REGS_RAM_BANK_0 + FOFB_PROCESSING_REGS_RAM_BANK_SIZE*chan)

#define FOFB_PROCESSING_REGS_REG_FIXED_POINT_POS    WB_FOFB_PROCESSING_REGS_FIXED_POINT_POS
#define FOFB_PROCESSING_REGS_FIXED_POINT_POS        FOFB_PROCESSING_REGS_REG_FIXED_POINT_POS
#define FOFB_PROCESSING_REGS_FIXED_POINT_POS_MASK   WB_FOFB_PROCESSING_REGS_FIXED_POINT_POS_VAL_MASK
#define FOFB_PROCESSING_REGS_FIXED_POINT_POS_SHIFT  WB_FOFB_PROCESSING_REGS_FIXED_POINT_POS_VAL_SHIFT
#define FOFB_PROCESSING_REGS_FIXED_POINT_POS_R(val) ((val & FOFB_PROCESSING_REGS_FIXED_POINT_POS_MASK) >> FOFB_PROCESSING_REGS_FIXED_POINT_POS_SHIFT)
#define FOFB_PROCESSING_REGS_FIXED_POINT_POS_W(val) ((val & FOFB_PROCESSING_REGS_FIXED_POINT_POS_MASK) << FOFB_PROCESSING_REGS_FIXED_POINT_POS_SHIFT)

#endif /* __WB_FOFB_PROCESSING_REGS_H__ */
