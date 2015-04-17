/*
* Copyright (C) 2014 LNLS (www.lnls.br)
* Author: Lucas Russo <lucas.russo@lnls.br>
*
* Released according to the GNU LGPL, version 3 or any later version.
*/

#ifndef _PCA9547_REGS_H_
#define _PCA9547_REGS_H_

/* Copied from wb_acq_core_regs.h */
#if defined( __GNUC__)
#define PACKED __attribute__ ((packed))
#else
#error "Unsupported compiler?"
#endif

/* Copied from wb_acq_core_regs.h */
#ifndef __WBGEN2_MACROS_DEFINED__
#define __WBGEN2_MACROS_DEFINED__
#define WBGEN2_GEN_MASK(offset, size) (((1<<(size))-1) << (offset))
#define WBGEN2_GEN_WRITE(value, offset, size) (((value) & ((1<<(size))-1)) << (offset))
#define WBGEN2_GEN_READ(reg, offset, size) (((reg) >> (offset)) & ((1<<(size))-1))
#define WBGEN2_SIGN_EXTEND(value, bits) (((value) & (1<<bits) ? ~((1<<(bits))-1): 0 ) | (value))
#endif

#define PCA9547_DATA_TRANS_SIZE                             8                       /* in bits */
#define PCA9547_BYTE_TRANS_SIZE                             PCA9547_DATA_TRANS_SIZE /* in bits */

#define PCA9547_MAX_NUM_CHANNEL                             8                       /* Max number of channels */

#define PCA9547_CHANNEL_SEL_EN_SHIFT                        3
#define PCA9547_CHANNEL_SEL_EN_SIZE                         1
#define PCA9547_CHANNEL_SEL_EN                              WBGEN2_GEN_MASK(PCA9547_CHANNEL_SEL_EN_SHIFT, \
                                                                     PCA9547_CHANNEL_SEL_EN_SIZE)

#define PCA9547_CHANNEL_SEL_EN_OFF                          0                    /* No Channel selected */

#endif

