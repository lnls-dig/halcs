#ifndef _WB_SPI_REGS_H_
#define _WB_SPI_REGS_H_

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

/* OpenCores SPI registers description with modification
 * to support bidirectional on a single MOSI line */

#define SPI_PROTO_REG_RX0                           0x00000000
#define SPI_PROTO_REG_TX0                           0x00000000
#define SPI_PROTO_REG_RX1                           0x00000004
#define SPI_PROTO_REG_TX1                           0x00000004
#define SPI_PROTO_REG_RX2                           0x00000008
#define SPI_PROTO_REG_TX2                           0x00000008
#define SPI_PROTO_REG_RX3                           0x0000000C
#define SPI_PROTO_REG_TX3                           0x0000000C

#define SPI_PROTO_REG_RXTX_NUM                      4

/* read-write whole register */
#define SPI_PROTO_TX0_W(value)                     (value)
#define SPI_PROTO_TX0_R(reg)                       (reg)
#define SPI_PROTO_TX1_W(value)                     (value)
#define SPI_PROTO_TX1_R(reg)                       (reg)
#define SPI_PROTO_TX2_W(value)                     (value)
#define SPI_PROTO_TX2_R(reg)                       (reg)
#define SPI_PROTO_TX3_W(value)                     (value)
#define SPI_PROTO_TX3_R(reg)                       (reg)

#define SPI_PROTO_REG_CTRL                          0x00000010

/* Definitions for SPI_PROTO_REG_CTRL */

/*#define SPI_PROTO_CTRL_THREE_WIRE                   (1<<16)
#define SPI_PROTO_CTRL_RES2                         (1<<15)
#define SPI_PROTO_CTRL_DIR                          (1<<14)*/
#define SPI_PROTO_CTRL_ASS                          WBGEN2_GEN_MASK(13, 1)
#define SPI_PROTO_CTRL_IE                           WBGEN2_GEN_MASK(12, 1)
#define SPI_PROTO_CTRL_LSB                          WBGEN2_GEN_MASK(11, 1)
#define SPI_PROTO_CTRL_TXNEG                        WBGEN2_GEN_MASK(10, 1)
#define SPI_PROTO_CTRL_RXNEG                        WBGEN2_GEN_MASK(9, 1)
#define SPI_PROTO_CTRL_GO_BSY                       WBGEN2_GEN_MASK(8, 1)
#define SPI_PROTO_CTRL_BSY                          WBGEN2_GEN_MASK(8, 1)

#define SPI_PROTO_CTRL_CHARLEN_SIZE                 7
#define SPI_PROTO_CTRL_CHARLEN_SHIFT                0
#define SPI_PROTO_CTRL_CHARLEN_MASK                 WBGEN2_GEN_MASK(SPI_PROTO_CTRL_CHARLEN_SHIFT, \
                                                                     SPI_PROTO_CTRL_CHARLEN_SIZE)
#define SPI_PROTO_CTRL_CHARLEN_W(value)             WBGEN2_GEN_WRITE(value, SPI_PROTO_CTRL_CHARLEN_SHIFT, \
                                                                             SPI_PROTO_CTRL_CHARLEN_SIZE)
#define SPI_PROTO_CTRL_CHARLEN_R(reg)               WBGEN2_GEN_READ(reg, SPI_PROTO_CTRL_CHARLEN_SHIFT, \
                                                                             SPI_PROTO_CTRL_CHARLEN_SIZE)

/* read-write whole register */
#define SPI_PROTO_CTRL_SIZE                         16
#define SPI_PROTO_CTRL_SHIFT                        0
#define SPI_PROTO_CTRL_MASK                         WBGEN2_GEN_MASK(SPI_PROTO_CTRL_SHIFT, \
                                                                    SPI_PROTO_CTRL_SIZE)
#define SPI_PROTO_CTRL_W(value)                     (value)
#define SPI_PROTO_CTRL_R(reg)                       (reg)

#define SPI_PROTO_REG_DIVIDER                       0x00000014

/* Definitions for SPI_PROTO_REG_DIVIDER */
#define SPI_PROTO_DIVIDER_SIZE                      16
#define SPI_PROTO_DIVIDER_SHIFT                     0
#define SPI_PROTO_DIVIDER_MASK                      WBGEN2_GEN_MASK(SPI_PROTO_DIVIDER_SHIFT, \
                                                                    SPI_PROTO_DIVIDER_SIZE)
#define SPI_PROTO_DIVIDER_W(value)                  WBGEN2_GEN_WRITE(value, SPI_PROTO_DIVIDER_SHIFT, \
                                                                            SPI_PROTO_DIVIDER_SIZE)
#define SPI_PROTO_DIVIDER_R(reg)                    WBGEN2_GEN_READ(reg, SPI_PROTO_DIVIDER_SHIFT, \
                                                                            SPI_PROTO_DIVIDER_SIZE)

#define SPI_PROTO_REG_SS                            0x00000018

/* Definitions for SPI_PROTO_REG_SS */
#define SPI_PROTO_SS_SIZE                           8
#define SPI_PROTO_SS_SHIFT                          0
#define SPI_PROTO_SS_MASK                           WBGEN2_GEN_MASK(SPI_PROTO_SS_SHIFT, \
                                                                    SPI_PROTO_SS_SIZE)
#define SPI_PROTO_SS_W(value)                       WBGEN2_GEN_WRITE(value, SPI_PROTO_SS_SHIFT, \
                                                                            SPI_PROTO_SS_SIZE)
#define SPI_PROTO_SS_R(reg)                         WBGEN2_GEN_READ(reg, SPI_PROTO_SS_SHIFT, \
                                                                        SPI_PROTO_SS_SIZE)

#define SPI_PROTO_REG_CFG_BIDIR                     0x0000001C

/* Definitions for SPI_PROTO_REG_CFG_BIDIR */
#define SPI_PROTO_CFG_BIDIR_CHARLEN_SIZE            7
#define SPI_PROTO_CFG_BIDIR_CHARLEN_SHIFT           0
#define SPI_PROTO_CFG_BIDIR_CHARLEN_MASK            WBGEN2_GEN_MASK(SPI_PROTO_CFG_BIDIR_CHARLEN_SHIFT, \
                                                          SPI_PROTO_CFG_BIDIR_CHARLEN_SIZE)
#define SPI_PROTO_CFG_BIDIR_CHARLEN_W(value)        WBGEN2_GEN_WRITE(value, SPI_PROTO_CFG_BIDIR_CHARLEN_SHIFT, \
                                                                  SPI_PROTO_CFG_BIDIR_CHARLEN_SIZE)
#define SPI_PROTO_CFG_BIDIR_CHARLEN_R(reg)          WBGEN2_GEN_READ(reg, SPI_PROTO_CFG_BIDIR_CHARLEN_SHIFT, \
                                                                  SPI_PROTO_CFG_BIDIR_CHARLEN_SIZE)

#define SPI_PROTO_CFG_BIDIR_EN_SIZE                 1
#define SPI_PROTO_CFG_BIDIR_EN_SHIFT                7
#define SPI_PROTO_CFG_BIDIR_EN                      WBGEN2_GEN_MASK(SPI_PROTO_CFG_BIDIR_EN_SHIFT, \
                                                               SPI_PROTO_CFG_BIDIR_EN_SIZE)

/* For RX data from MISO (single line) */
#define SPI_PROTO_REG_RX0_SINGLE                    0x00000020
#define SPI_PROTO_REG_RX1_SINGLE                    0x00000024
#define SPI_PROTO_REG_RX2_SINGLE                    0x00000028
#define SPI_PROTO_REG_RX3_SINGLE                    0x0000002C

/* read-write whole register */
#define SPI_PROTO_TX0_SINGLE_W(value)               (value)
#define SPI_PROTO_TX0_SINGLE_R(reg)                 (reg)
#define SPI_PROTO_TX1_SINGLE_W(value)               (value)
#define SPI_PROTO_TX1_SINGLE_R(reg)                 (reg)
#define SPI_PROTO_TX2_SINGLE_W(value)               (value)
#define SPI_PROTO_TX2_SINGLE_R(reg)                 (reg)
#define SPI_PROTO_TX3_SINGLE_W(value)               (value)
#define SPI_PROTO_TX3_SINGLE_R(reg)                 (reg)

#endif
