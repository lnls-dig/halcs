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

#define FOFB_PROCESSING_REGS_RAM_BANK_NUM   12
#define FOFB_PROCESSING_REGS_RAM_BANK_SIZE                                     \
  (WB_FOFB_PROCESSING_REGS_SIZE/FOFB_PROCESSING_REGS_RAM_BANK_NUM)

#define FOFB_PROCESSING_REGS_RAM_BANK_OFFS(chan)                               \
  (WB_FOFB_PROCESSING_REGS_RAM_BANK_0 + FOFB_PROCESSING_REGS_RAM_BANK_SIZE*chan)

#endif /* __WB_FOFB_PROCESSING_REGS_H__ */
