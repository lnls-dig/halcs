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

/* Definitions for FOFB_PROCESSING_REGS */

#define WB_FOFB_PROCESSING_REG_RAM_WRITE                     WB_FOFB_PROCESSING_REGS_RAM_WRITE
#define WB_FOFB_PROCESSING_RAM_WRITE_ENABLE                  WB_FOFB_PROCESSING_REGS_RAM_WRITE_ENABLE

#define WB_FOFB_PROCESSING_REG_RAM                           WB_FOFB_PROCESSING_REGS_RAM_ADDR
#define WB_FOFB_PROCESSING_REG_RAM_ADDR                      WB_FOFB_PROCESSING_REG_RAM

#define WB_FOFB_PROCESSING_REG_RAM_IN                        WB_FOFB_PROCESSING_REGS_RAM_DATA_IN
#define WB_FOFB_PROCESSING_REG_RAM_OUT                       WB_FOFB_PROCESSING_REGS_RAM_DATA_OUT

/* Definitions for RAM address */

#define WB_FOFB_PROCESSING_RAM_ADDR_MASK                     0xffffffffUL
#define WB_FOFB_PROCESSING_RAM_ADDR_SHIFT                    0
#define WB_FOFB_PROCESSING_RAM_ADDR_R(val)                   ((val & WB_FOFB_PROCESSING_RAM_ADDR_MASK) >> WB_FOFB_PROCESSING_RAM_ADDR_SHIFT)
#define WB_FOFB_PROCESSING_RAM_ADDR_W(val)                   ((val & WB_FOFB_PROCESSING_RAM_ADDR_MASK) << WB_FOFB_PROCESSING_RAM_ADDR_SHIFT)

/* Definitions for RAM data in */

#define WB_FOFB_PROCESSING_RAM_IN_DATA_MASK                  0xffffffffUL
#define WB_FOFB_PROCESSING_RAM_IN_DATA_SHIFT                 0
#define WB_FOFB_PROCESSING_RAM_IN_DATA_R(val)                ((val & WB_FOFB_PROCESSING_RAM_IN_DATA_MASK) >> WB_FOFB_PROCESSING_RAM_IN_DATA_SHIFT)
#define WB_FOFB_PROCESSING_RAM_IN_DATA_W(val)                ((val & WB_FOFB_PROCESSING_RAM_IN_DATA_MASK) << WB_FOFB_PROCESSING_RAM_IN_DATA_SHIFT)

/* Definitions for RAM data out */

#define WB_FOFB_PROCESSING_RAM_OUT_DATA_MASK                 0xffffffffUL
#define WB_FOFB_PROCESSING_RAM_OUT_DATA_SHIFT                0
#define WB_FOFB_PROCESSING_RAM_OUT_DATA_R(val)               ((val & WB_FOFB_PROCESSING_RAM_OUT_DATA_MASK) >> WB_FOFB_PROCESSING_RAM_OUT_DATA_SHIFT)
#define WB_FOFB_PROCESSING_RAM_OUT_DATA_W(val)               ((val & WB_FOFB_PROCESSING_RAM_OUT_DATA_MASK) << WB_FOFB_PROCESSING_RAM_OUT_DATA_SHIFT)

#endif /* __WB_FOFB_PROCESSING_REGS_H__ */

