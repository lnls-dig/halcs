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

#define FOFB_PROCESSING_REGS_COEFFS_RAM_BANK_SIZE                                     \
  (WB_FOFB_PROCESSING_REGS_COEFFS_RAM_BANK_1 - WB_FOFB_PROCESSING_REGS_COEFFS_RAM_BANK_0)

#define FOFB_PROCESSING_REGS_COEFFS_RAM_BANK_OFFS(chan)                               \
  (WB_FOFB_PROCESSING_REGS_COEFFS_RAM_BANK_0 + FOFB_PROCESSING_REGS_COEFFS_RAM_BANK_SIZE*chan)

#define FOFB_PROCESSING_REGS_SETPOINTS_RAM_BANK_SIZE                           \
  (FOFB_PROCESSING_REGS_COEFFS_RAM_BANK_SIZE)

#define FOFB_PROCESSING_REGS_SETPOINTS_RAM_BANK_OFFS                           \
  (WB_FOFB_PROCESSING_REGS_SETPOINTS_RAM_BANK)

#define FOFB_PROCESSING_REGS_REG_COEFFS_FIXED_POINT_POS     WB_FOFB_PROCESSING_REGS_COEFFS_FIXED_POINT_POS
#define FOFB_PROCESSING_REGS_COEFFS_FIXED_POINT_POS         FOFB_PROCESSING_REGS_REG_COEFFS_FIXED_POINT_POS
#define FOFB_PROCESSING_REGS_COEFFS_FIXED_POINT_POS_MASK    WB_FOFB_PROCESSING_REGS_COEFFS_FIXED_POINT_POS_VAL_MASK
#define FOFB_PROCESSING_REGS_COEFFS_FIXED_POINT_POS_SHIFT   WB_FOFB_PROCESSING_REGS_COEFFS_FIXED_POINT_POS_VAL_SHIFT
#define FOFB_PROCESSING_REGS_COEFFS_FIXED_POINT_POS_R(val)  ((val & FOFB_PROCESSING_REGS_COEFFS_FIXED_POINT_POS_MASK) >> FOFB_PROCESSING_REGS_COEFFS_FIXED_POINT_POS_SHIFT)
#define FOFB_PROCESSING_REGS_COEFFS_FIXED_POINT_POS_W(val)  ((val << FOFB_PROCESSING_REGS_COEFFS_FIXED_POINT_POS_SHIFT) & FOFB_PROCESSING_REGS_COEFFS_FIXED_POINT_POS_MASK)

#define FOFB_PROCESSING_REGS_REG_ACCS_GAINS_FIXED_POINT_POS     WB_FOFB_PROCESSING_REGS_ACCS_GAINS_FIXED_POINT_POS
#define FOFB_PROCESSING_REGS_ACCS_GAINS_FIXED_POINT_POS         FOFB_PROCESSING_REGS_REG_ACCS_GAINS_FIXED_POINT_POS
#define FOFB_PROCESSING_REGS_ACCS_GAINS_FIXED_POINT_POS_MASK    WB_FOFB_PROCESSING_REGS_ACCS_GAINS_FIXED_POINT_POS_VAL_MASK
#define FOFB_PROCESSING_REGS_ACCS_GAINS_FIXED_POINT_POS_SHIFT   WB_FOFB_PROCESSING_REGS_ACCS_GAINS_FIXED_POINT_POS_VAL_SHIFT
#define FOFB_PROCESSING_REGS_ACCS_GAINS_FIXED_POINT_POS_R(val)  ((val & FOFB_PROCESSING_REGS_ACCS_GAINS_FIXED_POINT_POS_MASK) >> FOFB_PROCESSING_REGS_ACCS_GAINS_FIXED_POINT_POS_SHIFT)
#define FOFB_PROCESSING_REGS_ACCS_GAINS_FIXED_POINT_POS_W(val)  ((val << FOFB_PROCESSING_REGS_ACCS_GAINS_FIXED_POINT_POS_SHIFT) & FOFB_PROCESSING_REGS_ACCS_GAINS_FIXED_POINT_POS_MASK)

#define FOFB_PROCESSING_REGS_REG_ACC_GAIN WB_FOFB_PROCESSING_REGS_ACC_GAIN_0
#define FOFB_PROCESSING_REGS_ACC_GAIN_VAL_MASK WB_FOFB_PROCESSING_REGS_ACC_GAIN_0_VAL_MASK
#define FOFB_PROCESSING_REGS_ACC_GAIN_VAL_SHIFT WB_FOFB_PROCESSING_REGS_ACC_GAIN_0_VAL_SHIFT
#define FOFB_PROCESSING_REGS_ACC_GAIN_VAL_R(val)  ((val & FOFB_PROCESSING_REGS_ACC_GAIN_VAL_MASK) >> FOFB_PROCESSING_REGS_ACC_GAIN_VAL_SHIFT)
#define FOFB_PROCESSING_REGS_ACC_GAIN_VAL_W(val)  ((val << FOFB_PROCESSING_REGS_ACC_GAIN_VAL_SHIFT) & FOFB_PROCESSING_REGS_ACC_GAIN_VAL_MASK)

#define FOFB_PROCESSING_REGS_REG_ACC_CTL WB_FOFB_PROCESSING_REGS_ACC_CTL_0
#define FOFB_PROCESSING_REGS_ACC_CTL_CLEAR WB_FOFB_PROCESSING_REGS_ACC_CTL_0_CLEAR
#define FOFB_PROCESSING_REGS_ACC_CTL_FREEZE WB_FOFB_PROCESSING_REGS_ACC_CTL_0_FREEZE

#define FOFB_PROCESSING_REGS_REG_SP_MAX WB_FOFB_PROCESSING_REGS_SP_MAX_0
#define FOFB_PROCESSING_REGS_SP_MAX_VAL_MASK WB_FOFB_PROCESSING_REGS_SP_MAX_0_VAL_MASK
#define FOFB_PROCESSING_REGS_SP_MAX_VAL_SHIFT WB_FOFB_PROCESSING_REGS_SP_MAX_0_VAL_SHIFT
#define FOFB_PROCESSING_REGS_SP_MAX_VAL_R(val)  ((val & FOFB_PROCESSING_REGS_SP_MAX_VAL_MASK) >> FOFB_PROCESSING_REGS_SP_MAX_VAL_SHIFT)
#define FOFB_PROCESSING_REGS_SP_MAX_VAL_W(val)  ((val << FOFB_PROCESSING_REGS_SP_MAX_VAL_SHIFT) & FOFB_PROCESSING_REGS_SP_MAX_VAL_MASK)

#define FOFB_PROCESSING_REGS_REG_SP_MIN WB_FOFB_PROCESSING_REGS_SP_MIN_0
#define FOFB_PROCESSING_REGS_SP_MIN_VAL_MASK WB_FOFB_PROCESSING_REGS_SP_MIN_0_VAL_MASK
#define FOFB_PROCESSING_REGS_SP_MIN_VAL_SHIFT WB_FOFB_PROCESSING_REGS_SP_MIN_0_VAL_SHIFT
#define FOFB_PROCESSING_REGS_SP_MIN_VAL_R(val)  ((val & FOFB_PROCESSING_REGS_SP_MIN_VAL_MASK) >> FOFB_PROCESSING_REGS_SP_MIN_VAL_SHIFT)
#define FOFB_PROCESSING_REGS_SP_MIN_VAL_W(val)  ((val << FOFB_PROCESSING_REGS_SP_MIN_VAL_SHIFT) & FOFB_PROCESSING_REGS_SP_MIN_VAL_MASK)

#define FOFB_PROCESSING_REGS_CHANNEL_DISTANCE (WB_FOFB_PROCESSING_REGS_ACC_GAIN_1 - WB_FOFB_PROCESSING_REGS_ACC_GAIN_0)
#define FOFB_PROCESSING_REGS_CHANNEL_SP_DISTANCE (WB_FOFB_PROCESSING_REGS_SP_MAX_1 - WB_FOFB_PROCESSING_REGS_SP_MAX_0)
#define FOFB_PROCESSING_CHANNEL_COUNT 12

#endif /* __WB_FOFB_PROCESSING_REGS_H__ */
