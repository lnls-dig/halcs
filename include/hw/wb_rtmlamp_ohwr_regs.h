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
#define RTMLAMP_OHWR_CH_0_CTL_PI_OL_TRIANG_ENABLE    RTMLAMP_OHWR_REGS_CH_0_CTL_PI_OL_TRIANG_ENABLE
#define RTMLAMP_OHWR_CH_0_CTL_PI_OL_SQUARE_ENABLE    RTMLAMP_OHWR_REGS_CH_0_CTL_PI_OL_SQUARE_ENABLE
#define RTMLAMP_OHWR_CH_0_CTL_PI_SP_SQUARE_ENABLE    RTMLAMP_OHWR_REGS_CH_0_CTL_PI_SP_SQUARE_ENABLE
#define RTMLAMP_OHWR_CH_0_CTL_PI_ENABLE              RTMLAMP_OHWR_REGS_CH_0_CTL_PI_ENABLE


#define RTMLAMP_OHWR_REG_CH_0_DAC                    RTMLAMP_OHWR_REGS_CH_0_DAC

/* Definitions for DAC channel 0 control register */
#define RTMLAMP_OHWR_CH_0_DAC_DATA_MASK              RTMLAMP_OHWR_REGS_CH_0_DAC_DATA_MASK
#define RTMLAMP_OHWR_CH_0_DAC_DATA_SHIFT             RTMLAMP_OHWR_REGS_CH_0_DAC_DATA_SHIFT
#define RTMLAMP_OHWR_CH_0_DAC_DATA_R(val)            ((val & RTMLAMP_OHWR_CH_0_DAC_DATA_MASK) >> RTMLAMP_OHWR_CH_0_DAC_DATA_SHIFT)
#define RTMLAMP_OHWR_CH_0_DAC_DATA_W(val)            ((val & RTMLAMP_OHWR_CH_0_DAC_DATA_MASK) << RTMLAMP_OHWR_CH_0_DAC_DATA_SHIFT)
#define RTMLAMP_OHWR_CH_0_DAC_WR                     RTMLAMP_OHWR_REGS_CH_0_DAC_WR

#define RTMLAMP_OHWR_REG_PI_KP                       RTMLAMP_OHWR_REGS_PI_KP

/* Definitions for DAC channel 0 control register */
#define RTMLAMP_OHWR_PI_KP_DATA_MASK                 RTMLAMP_OHWR_REGS_PI_KP_DATA_MASK
#define RTMLAMP_OHWR_PI_KP_DATA_SHIFT                RTMLAMP_OHWR_REGS_PI_KP_DATA_SHIFT
#define RTMLAMP_OHWR_PI_KP_DATA_R(val)               ((val & RTMLAMP_OHWR_PI_KP_DATA_MASK) >> RTMLAMP_OHWR_PI_KP_DATA_SHIFT)
#define RTMLAMP_OHWR_PI_KP_DATA_W(val)               ((val & RTMLAMP_OHWR_PI_KP_DATA_MASK) << RTMLAMP_OHWR_PI_KP_DATA_SHIFT)

#define RTMLAMP_OHWR_REG_PI_TI                       RTMLAMP_OHWR_REGS_PI_TI

/* Definitions for DAC channel 0 control register */
#define RTMLAMP_OHWR_PI_TI_DATA_MASK                 RTMLAMP_OHWR_REGS_PI_TI_DATA_MASK
#define RTMLAMP_OHWR_PI_TI_DATA_SHIFT                RTMLAMP_OHWR_REGS_PI_TI_DATA_SHIFT
#define RTMLAMP_OHWR_PI_TI_DATA_R(val)               ((val & RTMLAMP_OHWR_PI_TI_DATA_MASK) >> RTMLAMP_OHWR_PI_TI_DATA_SHIFT)
#define RTMLAMP_OHWR_PI_TI_DATA_W(val)               ((val & RTMLAMP_OHWR_PI_TI_DATA_MASK) << RTMLAMP_OHWR_PI_TI_DATA_SHIFT)

#define RTMLAMP_OHWR_REG_PI_SP                       RTMLAMP_OHWR_REGS_PI_SP

/* Definitions for DAC channel 0 control register */
#define RTMLAMP_OHWR_PI_SP_DATA_MASK                 RTMLAMP_OHWR_REGS_PI_SP_DATA_MASK
#define RTMLAMP_OHWR_PI_SP_DATA_SHIFT                RTMLAMP_OHWR_REGS_PI_SP_DATA_SHIFT
#define RTMLAMP_OHWR_PI_SP_DATA_R(val)               ((val & RTMLAMP_OHWR_PI_SP_DATA_MASK) >> RTMLAMP_OHWR_PI_SP_DATA_SHIFT)
#define RTMLAMP_OHWR_PI_SP_DATA_W(val)               ((val & RTMLAMP_OHWR_PI_SP_DATA_MASK) << RTMLAMP_OHWR_PI_SP_DATA_SHIFT)

#define RTMLAMP_OHWR_REG_PI_OL_DAC_CNT_MAX           RTMLAMP_OHWR_REGS_PI_OL_DAC_CNT_MAX

/* Definitions for DAC channel 0 control register */
#define RTMLAMP_OHWR_PI_OL_DAC_CNT_MAX_DATA_MASK     RTMLAMP_OHWR_REGS_PI_OL_DAC_CNT_MAX_DATA_MASK
#define RTMLAMP_OHWR_PI_OL_DAC_CNT_MAX_DATA_SHIFT    RTMLAMP_OHWR_REGS_PI_OL_DAC_CNT_MAX_DATA_SHIFT
#define RTMLAMP_OHWR_PI_OL_DAC_CNT_MAX_DATA_R(val)   ((val & RTMLAMP_OHWR_PI_OL_DAC_CNT_MAX_DATA_MASK) >> RTMLAMP_OHWR_PI_OL_DAC_CNT_MAX_DATA_SHIFT)
#define RTMLAMP_OHWR_PI_OL_DAC_CNT_MAX_DATA_W(val)   ((val & RTMLAMP_OHWR_PI_OL_DAC_CNT_MAX_DATA_MASK) << RTMLAMP_OHWR_PI_OL_DAC_CNT_MAX_DATA_SHIFT)
#define RTMLAMP_OHWR_PI_OL_DAC_CNT_MAX_RESERVED_DATA_MASK  \
                                                     RTMLAMP_OHWR_REGS_PI_OL_DAC_CNT_MAX_RESERVED_DATA_MASK
#define RTMLAMP_OHWR_PI_OL_DAC_CNT_MAX_RESERVED_DATA_SHIFT \
                                                     RTMLAMP_OHWR_REGS_PI_OL_DAC_CNT_MAX_RESERVED_DATA_SHIFT
#define RTMLAMP_OHWR_PI_OL_DAC_CNT_MAX_RESERVED_DATA_R(val) \
                                                     ((val & RTMLAMP_OHWR_PI_OL_DAC_CNT_MAX_RESERVED_DATA_MASK) >> RTMLAMP_OHWR_PI_OL_DAC_CNT_MAX_RESERVED_DATA_SHIFT)
#define RTMLAMP_OHWR_PI_OL_DAC_CNT_MAX_RESERVED_DATA_W(val) \
                                                     ((val & RTMLAMP_OHWR_PI_OL_DAC_CNT_MAX_RESERVED_DATA_MASK) << RTMLAMP_OHWR_PI_OL_DAC_CNT_MAX_RESERVED_DATA_SHIFT)

#define RTMLAMP_OHWR_REG_PI_SP_LIM_INF               RTMLAMP_OHWR_REGS_PI_SP_LIM_INF

/* Definitions for DAC channel 0 control register */
#define RTMLAMP_OHWR_PI_SP_LIM_INF_DATA_MASK         RTMLAMP_OHWR_REGS_PI_SP_LIM_INF_DATA_MASK
#define RTMLAMP_OHWR_PI_SP_LIM_INF_DATA_SHIFT        RTMLAMP_OHWR_REGS_PI_SP_LIM_INF_DATA_SHIFT
#define RTMLAMP_OHWR_PI_SP_LIM_INF_DATA_R(val)       ((val & RTMLAMP_OHWR_PI_SP_LIM_INF_DATA_MASK) >> RTMLAMP_OHWR_PI_SP_LIM_INF_DATA_SHIFT)
#define RTMLAMP_OHWR_PI_SP_LIM_INF_DATA_W(val)       ((val & RTMLAMP_OHWR_PI_SP_LIM_INF_DATA_MASK) << RTMLAMP_OHWR_PI_SP_LIM_INF_DATA_SHIFT)


#endif /* __WB_RTMLAMP_OHWR_CC_REGS_H__ */
