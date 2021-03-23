#ifndef __WB_RTMLAMP_OHWR_CC_REGS_H__
#define __WB_RTMLAMP_OHWR_CC_REGS_H__

#include "__wb_rtmlamp_ohwr_regs.h"

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

/* Regular registers */

#define RTMLAMP_OHWR_REG_STA                         RTMLAMP_OHWR_REGS_STA 

/* Definitions for RTMLAMP_OHWR_REG_STA */

#define RTMLAMP_OHWR_REG_CTL                         RTMLAMP_OHWR_REGS_CTL 

/* Definitions for RTMLAMP_OHWR_REG_CTL */
#define RTMLAMP_OHWR_CTL_DAC_DATA_FROM_WB            RTMLAMP_OHWR_REGS_CTL_DAC_DATA_FROM_WB

#define RTMLAMP_OHWR_REG_CH_0_STA                    RTMLAMP_OHWR_REGS_CH_0_STA

/* Definitions for Channel 0 status register */
#define RTMLAMP_OHWR_CH_0_STA_AMP_IFLAG_L            RTMLAMP_OHWR_REGS_CH_0_STA_AMP_IFLAG_L 
#define RTMLAMP_OHWR_CH_0_STA_AMP_TFLAG_L            RTMLAMP_OHWR_REGS_CH_0_STA_AMP_TFLAG_L 
#define RTMLAMP_OHWR_CH_0_STA_AMP_IFLAG_R            RTMLAMP_OHWR_REGS_CH_0_STA_AMP_IFLAG_R 
#define RTMLAMP_OHWR_CH_0_STA_AMP_TFLAG_R            RTMLAMP_OHWR_REGS_CH_0_STA_AMP_TFLAG_R 

#define RTMLAMP_OHWR_REG_CH_0_CTL                    RTMLAMP_OHWR_REGS_CH_0_CTL 

/* Definitions for Channel 0 control register */
#define RTMLAMP_OHWR_CH_0_CTL_AMP_EN                 RTMLAMP_OHWR_REGS_CH_0_CTL_AMP_EN

#define RTMLAMP_OHWR_REG_CH_0_DAC                    RTMLAMP_OHWR_REGS_CH_0_DAC 

/* Definitions for DAC channel 0 control register */
#define RTMLAMP_OHWR_CH_0_DAC_DATA_MASK              RTMLAMP_OHWR_REGS_CH_0_DAC_DATA_MASK
#define RTMLAMP_OHWR_CH_0_DAC_DATA_SHIFT             RTMLAMP_OHWR_REGS_CH_0_DAC_DATA_SHIFT 
#define RTMLAMP_OHWR_CH_0_DAC_DATA_R(val)            ((val & RTMLAMP_OHWR_CH_0_DAC_DATA_MASK) >> RTMLAMP_OHWR_CH_0_DAC_DATA_SHIFT) 
#define RTMLAMP_OHWR_CH_0_DAC_DATA_W(val)            ((val & RTMLAMP_OHWR_CH_0_DAC_DATA_MASK) << RTMLAMP_OHWR_CH_0_DAC_DATA_SHIFT)
#define RTMLAMP_OHWR_CH_0_DAC_WR                     RTMLAMP_OHWR_REGS_CH_0_DAC_WR 

#endif /* __WB_RTMLAMP_OHWR_CC_REGS_H__ */
