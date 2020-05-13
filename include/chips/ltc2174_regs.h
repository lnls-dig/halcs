/*
* Copyright (C) 2014 LNLS (www.lnls.br)
* Author: Lucas Russo <lucas.russo@lnls.br>
*
* Released according to the GNU GPL, version 3 or any later version.
*/

#ifndef _LTC2174_REGS_H_
#define _LTC2174_REGS_H_

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

/* LTC2174 Instruction Header for 16-bit transfers */

/* LTC2174 Transfer type field */
#define LTC2174_HDR_RW_SIZE                          1
#define LTC2174_HDR_RW_SHIFT                         7
#define LTC2174_HDR_RW                               WBGEN2_GEN_MASK(LTC2174_HDR_RW_SHIFT, \
                                                                        LTC2174_HDR_RW_SIZE)

/* LTC2174 Address field. Only the bits 0 to 6 are available. The remaining bit 7
 * is fixed to 0, as mandated by the LTC2174 datasheet */
#define LTC2174_HDR_ADDR_SIZE                        7
#define LTC2174_HDR_ADDR_SHIFT                       0
#define LTC2174_HDR_ADDR_MASK                        WBGEN2_GEN_MASK(LTC2174_HDR_ADDR_SHIFT, \
                                                                        LTC2174_HDR_ADDR_SIZE)
#define LTC2174_HDR_ADDR_W(value)                    WBGEN2_GEN_WRITE(value, LTC2174_HDR_ADDR_SHIFT, \
                                                                        LTC2174_HDR_ADDR_SIZE)
#define LTC2174_HDR_ADDR_R(reg)                      WBGEN2_GEN_READ(reg, LTC2174_HDR_ADDR_SHIFT, \
                                                                            LTC2174_HDR_ADDR_SIZE)

/* Pad bytes to 16-bit */
#define LTC2174_HDR_ADDR_PAD_SIZE                    0

#define LTC2174_INSTADDR_SIZE                        (LTC2174_HDR_RW_SIZE + \
                                                        LTC2174_HDR_ADDR_PAD_SIZE + \
                                                        LTC2174_HDR_ADDR_SIZE)

/* LTC2174 Data for 16-bit transfers */
#define LTC2174_DATA_SIZE                            8
#define LTC2174_DATA_SHIFT                           0
#define LTC2174_DATA_MASK                            WBGEN2_GEN_MASK(LTC2174_DATA_SHIFT, \
                                                                        LTC2174_DATA_SIZE)
#define LTC2174_DATA_W(value)                        WBGEN2_GEN_WRITE(value, LTC2174_DATA_SHIFT, \
                                                                        LTC2174_DATA_SIZE)
#define LTC2174_DATA_R(reg)                          WBGEN2_GEN_READ(reg, LTC2174_DATA_SHIFT, \
                                                                        LTC2174_DATA_SIZE)

#define LTC2174_TRANS_SIZE                           (LTC2174_INSTADDR_SIZE+LTC2174_DATA_SIZE)          /* in bits */

/* LTC2174 Register map, as described by the LTC2174 datasheet. Not all of them
 * are descried here, just the ones we use */

#define LTC2174_REG_RESET                            0x00

/* Definition for register LTC2174_REG_RESET_MSB */
#define LTC2174_RESET_EN_SHIFT                       7
#define LTC2174_RESET_EN_SIZE                        1
#define LTC2174_RESET_EN                             WBGEN2_GEN_MASK(LTC2174_RESET_EN_SHIFT, \
                                                                              LTC2174_RESET_EN_SIZE)

#define LTC2174_REG_FORMAT_PDOWN                     0x01

/* Definition for register LTC2174_REG_FORMAT_PDOWN */
#define LTC2174_FORMAT_PDOWN_TWOS_COMPL_SHIFT        5
#define LTC2174_FORMAT_PDOWN_TWOS_COMPL_SIZE         1
#define LTC2174_FORMAT_PDOWN_TWOS_COMPL              WBGEN2_GEN_MASK(LTC2174_FORMAT_PDOWN_TWOS_COMPL_SHIFT, \
                                                               LTC2174_FORMAT_PDOWN_TWOS_COMPL_SIZE)

#define LTC2174_REG_TEST_PATT_MSB                    0x03

/* Definition for register LTC2174_REG_TEST_PATT_MSB */
#define LTC2174_TEST_PATT_OUTTEST_SHIFT              7
#define LTC2174_TEST_PATT_OUTTEST_SIZE               1
#define LTC2174_TEST_PATT_OUTTEST                    WBGEN2_GEN_MASK(LTC2174_TEST_PATT_OUTTEST_SHIFT, \
                                                                     LTC2174_TEST_PATT_OUTTEST_SIZE)

#define LTC2174_TEST_PATT_MSB_SIZE                   6
#define LTC2174_TEST_PATT_MSB_SHIFT                  0
#define LTC2174_TEST_PATT_MSB_MASK                   WBGEN2_GEN_MASK(LTC2174_TEST_PATT_MSB_SHIFT, \
                                                           LTC2174_TEST_PATT_MSB_SIZE)
#define LTC2174_TEST_PATT_MSB_W(value)               WBGEN2_GEN_WRITE(value, LTC2174_TEST_PATT_MSB_SHIFT, \
                                                           LTC2174_TEST_PATT_MSB_SIZE)
#define LTC2174_TEST_PATT_MSB_R(reg)                 WBGEN2_GEN_READ(reg, LTC2174_TEST_PATT_MSB_SHIFT, \
                                                                       LTC2174_TEST_PATT_MSB_SIZE)


#define LTC2174_REG_TEST_PATT_LSB                    0x04

/* Definition for register LTC2174_REG_TEST_PATT_LSB */
#define LTC2174_TEST_PATT_LSB_SIZE                   8
#define LTC2174_TEST_PATT_LSB_SHIFT                  0
#define LTC2174_TEST_PATT_LSB_MASK                   WBGEN2_GEN_MASK(LTC2174_TEST_PATT_LSB_SHIFT, \
                                                           LTC2174_TEST_PATT_LSB_SIZE)
#define LTC2174_TEST_PATT_LSB_W(value)               WBGEN2_GEN_WRITE(value, LTC2174_TEST_PATT_LSB_SHIFT, \
                                                           LTC2174_TEST_PATT_LSB_SIZE)
#define LTC2174_TEST_PATT_LSB_R(reg)                 WBGEN2_GEN_READ(reg, LTC2174_TEST_PATT_LSB_SHIFT, \
                                                                       LTC2174_TEST_PATT_LSB_SIZE)

#endif
