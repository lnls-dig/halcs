#ifndef _WB_I2C_REGS_H_
#define _WB_I2C_REGS_H_

#include <inttypes.h>

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

#define I2C_PROTO_REG_PRER_LO                       0x00000000      /* Clock Low */

/* read-write whole register */
#define I2C_PROTO_PRER_LO_SIZE                      8
#define I2C_PROTO_PRER_LO_SHIFT                     0
#define I2C_PROTO_PRER_LO_MASK                      WBGEN2_GEN_MASK(I2C_PROTO_PRER_LO_SHIFT, \
                                                                    I2C_PROTO_PRER_LO_SIZE)
#define I2C_PROTO_PRER_LO_W(value)                  WBGEN2_GEN_WRITE(value, I2C_PROTO_PRER_LO_SHIFT, \
                                                                        I2C_PROTO_PRER_LO_SIZE)
#define I2C_PROTO_PRER_LO_R(reg)                    WBGEN2_GEN_READ(reg, I2C_PROTO_PRER_LO_SHIFT, \
                                                                        I2C_PROTO_PRER_LO_SIZE)

#define I2C_PROTO_REG_PRER_HI                       0x00000004      /* Clock High */

#define I2C_PROTO_PRER_HI_SIZE                      8
#define I2C_PROTO_PRER_HI_SHIFT                     0
#define I2C_PROTO_PRER_HI_MASK                      WBGEN2_GEN_MASK(I2C_PROTO_PRER_HI_SHIFT, \
                                                                    I2C_PROTO_PRER_HI_SIZE)
#define I2C_PROTO_PRER_HI_W(value)                  WBGEN2_GEN_WRITE(value, I2C_PROTO_PRER_HI_SHIFT, \
                                                                        I2C_PROTO_PRER_HI_SIZE)
#define I2C_PROTO_PRER_HI_R(reg)                    WBGEN2_GEN_READ(reg, I2C_PROTO_PRER_HI_SHIFT, \
                                                                        I2C_PROTO_PRER_HI_SIZE)

#define I2C_PROTO_REG_CTR                           0x00000008      /* Control REG */

/* Definitions for I2C_PROTO_REG_CTR */
#define I2C_PROTO_CTR_INT_SIZE                      1
#define I2C_PROTO_CTR_INT_SHIFT                     6
#define I2C_PROTO_CTR_INT                           WBGEN2_GEN_MASK(I2C_PROTO_CTR_INT_SHIFT, \
                                                                    I2C_PROTO_CTR_INT_SIZE)

#define I2C_PROTO_CTR_EN_SIZE                       1
#define I2C_PROTO_CTR_EN_SHIFT                      7
#define I2C_PROTO_CTR_EN                            WBGEN2_GEN_MASK(I2C_PROTO_CTR_EN_SHIFT, \
                                                                    I2C_PROTO_CTR_EN_SIZE)

#define I2C_PROTO_REG_TXR                           0x0000000C      /* Transmit REG */

/* Definitions for I2C_PROTO_REG_TXR */

/* read-write whole register */
#define I2C_PROTO_TXR_SIZE                          8       /* I2C addresses are 7 bits (no 10-bit support ) */
#define I2C_PROTO_TXR_SHIFT                         0
#define I2C_PROTO_TXR_MASK                          WBGEN2_GEN_MASK(I2C_PROTO_TXR_SHIFT, \
                                                                   I2C_PROTO_TXR_SIZE)
#define I2C_PROTO_TXR_W(value)                      WBGEN2_GEN_WRITE(value, I2C_PROTO_TXR_SHIFT, \
                                                                       I2C_PROTO_TXR_SIZE)
#define I2C_PROTO_TXR_R(reg)                        WBGEN2_GEN_READ(reg, I2C_PROTO_TXR_SHIFT, \
                                                                       I2C_PROTO_TXR_SIZE)

#define I2C_PROTO_TXR_RW_SIZE                       1
#define I2C_PROTO_TXR_RW_SHIFT                      0
#define I2C_PROTO_TXR_RW                            WBGEN2_GEN_MASK(I2C_PROTO_TXR_RW_SHIFT, \
                                                                    I2C_PROTO_TXR_RW_SIZE)

#define I2C_PROTO_TXR_ADDR_SIZE                     7       /* I2C addresses are 7 bits (no 10-bit support ) */
#define I2C_PROTO_TXR_ADDR_SHIFT                    1
#define I2C_PROTO_TXR_ADDR_MASK                     WBGEN2_GEN_MASK(I2C_PROTO_TXR_ADDR_SHIFT, \
                                                                   I2C_PROTO_TXR_ADDR_SIZE)
#define I2C_PROTO_TXR_ADDR_W(value)                 WBGEN2_GEN_WRITE(value, I2C_PROTO_TXR_ADDR_SHIFT, \
                                                                       I2C_PROTO_TXR_ADDR_SIZE)
#define I2C_PROTO_TXR_ADDR_R(reg)                   WBGEN2_GEN_READ(reg, I2C_PROTO_TXR_ADDR_SHIFT, \
                                                                       I2C_PROTO_TXR_ADDR_SIZE)

#define I2C_PROTO_REG_RXR                           0x0000000C      /* Receive REG */

/* Definitions for I2C_PROTO_REG_RXR */
/* read-write whole register */
#define I2C_PROTO_RXR_SIZE                          8       /* I2C addresses are 7 bits (no 10-bit support ) */
#define I2C_PROTO_RXR_SHIFT                         0
#define I2C_PROTO_RXR_MASK                          WBGEN2_GEN_MASK(I2C_PROTO_RXR_SHIFT, \
                                                                   I2C_PROTO_RXR_SIZE)
#define I2C_PROTO_RXR_W(value)                      WBGEN2_GEN_WRITE(value, I2C_PROTO_RXR_SHIFT, \
                                                                       I2C_PROTO_RXR_SIZE)
#define I2C_PROTO_RXR_R(reg)                        WBGEN2_GEN_READ(reg, I2C_PROTO_RXR_SHIFT, \
                                                                       I2C_PROTO_RXR_SIZE)

#define I2C_PROTO_REG_CR                            0x00000010      /* Command REG */

/* Definitions for I2C_PROTO_REG_CR */
/* read-write whole register */
#define I2C_PROTO_CR_SIZE                           8       /* I2C addresses are 7 bits (no 10-bit support ) */
#define I2C_PROTO_CR_SHIFT                          0
#define I2C_PROTO_CR_MASK                           WBGEN2_GEN_MASK(I2C_PROTO_CR_SHIFT, \
                                                                   I2C_PROTO_CR_SIZE)
#define I2C_PROTO_CR_W(value)                       WBGEN2_GEN_WRITE(value, I2C_PROTO_CR_SHIFT, \
                                                                       I2C_PROTO_CR_SIZE)
#define I2C_PROTO_CR_R(reg)                         WBGEN2_GEN_READ(reg, I2C_PROTO_CR_SHIFT, \
                                                                       I2C_PROTO_CR_SIZE)

#define I2C_PROTO_CR_IACK_SIZE                      1
#define I2C_PROTO_CR_IACK_SHIFT                     0
#define I2C_PROTO_CR_IACK                           WBGEN2_GEN_MASK(I2C_PROTO_CR_IACK_SHIFT, \
                                                                    I2C_PROTO_CR_IACK_SIZE)

#define I2C_PROTO_CR_ACK_SIZE                       1
#define I2C_PROTO_CR_ACK_SHIFT                      3
#define I2C_PROTO_CR_ACK                            WBGEN2_GEN_MASK(I2C_PROTO_CR_ACK_SHIFT, \
                                                                    I2C_PROTO_CR_ACK_SIZE)

#define I2C_PROTO_CR_WR_SIZE                        1
#define I2C_PROTO_CR_WR_SHIFT                       4
#define I2C_PROTO_CR_WR                             WBGEN2_GEN_MASK(I2C_PROTO_CR_WR_SHIFT, \
                                                                    I2C_PROTO_CR_WR_SIZE)

#define I2C_PROTO_CR_RD_SIZE                        1
#define I2C_PROTO_CR_RD_SHIFT                       5
#define I2C_PROTO_CR_RD                             WBGEN2_GEN_MASK(I2C_PROTO_CR_RD_SHIFT, \
                                                                    I2C_PROTO_CR_RD_SIZE)

#define I2C_PROTO_CR_STO_SIZE                       1
#define I2C_PROTO_CR_STO_SHIFT                      6
#define I2C_PROTO_CR_STO                            WBGEN2_GEN_MASK(I2C_PROTO_CR_STO_SHIFT, \
                                                                   I2C_PROTO_CR_STO_SIZE)

#define I2C_PROTO_CR_STA_SIZE                       1
#define I2C_PROTO_CR_STA_SHIFT                      7
#define I2C_PROTO_CR_STA                            WBGEN2_GEN_MASK(I2C_PROTO_CR_STA_SHIFT, \
                                                                   I2C_PROTO_CR_STA_SIZE)

#define I2C_PROTO_REG_SR                            0x00000010      /* Status REG */

/* Definitions for I2C_PROTO_REG_SR */

/* Interrupt Flag */
#define I2C_PROTO_SR_IF_SIZE                        1
#define I2C_PROTO_SR_IF_SHIFT                       0
#define I2C_PROTO_SR_IF                             WBGEN2_GEN_MASK(I2C_PROTO_SR_IF_SHIFT, \
                                                                    I2C_PROTO_SR_IF_SIZE)

/* Transfer in progress */
#define I2C_PROTO_SR_TIP_SIZE                       1
#define I2C_PROTO_SR_TIP_SHIFT                      1
#define I2C_PROTO_SR_TIP                            WBGEN2_GEN_MASK(I2C_PROTO_SR_TIP_SHIFT, \
                                                                    I2C_PROTO_SR_TIP_SIZE)

/* Arbitration Lost */
#define I2C_PROTO_SR_AL_SIZE                        1
#define I2C_PROTO_SR_AL_SHIFT                       5
#define I2C_PROTO_SR_AL                             WBGEN2_GEN_MASK(I2C_PROTO_SR_AL_SHIFT, \
                                                                    I2C_PROTO_SR_AL_SIZE)

/* I2C Busy */
#define I2C_PROTO_SR_BUSY_SIZE                      1
#define I2C_PROTO_SR_BUSY_SHIFT                     6
#define I2C_PROTO_SR_BUSY                           WBGEN2_GEN_MASK(I2C_PROTO_SR_BUSY_SHIFT, \
                                                                    I2C_PROTO_SR_BUSY_SIZE)

/* Interrupt Flag */
#define I2C_PROTO_SR_RXACK_SIZE                     1
#define I2C_PROTO_SR_RXACK_SHIFT                    7
#define I2C_PROTO_SR_RXACK                          WBGEN2_GEN_MASK(I2C_PROTO_SR_RXACK_SHIFT, \
                                                                    I2C_PROTO_SR_RXACK_SIZE)

#endif
