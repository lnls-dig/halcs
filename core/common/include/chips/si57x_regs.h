/*
* Copyright (C) 2014 LNLS (www.lnls.br)
* Author: Lucas Russo <lucas.russo@lnls.br>
*
* Released according to the GNU GPL, version 3 or any later version.
*/

#ifndef _SI57X_REGS_H_
#define _SI57X_REGS_H_

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

#define SI57X_ADDR_TRANS_SIZE                               8          /* in bits */
#define SI57X_DATA_TRANS_SIZE                               8          /* in bits */
#define SI57X_BYTE_TRANS_SIZE                               (SI57X_ADDR_TRANS_SIZE + \
                                                                 SI57X_DATA_TRANS_SIZE) /* in bits */

#define SI57X_DATA_BYTES_MAX                                32
#define SI57X_TRANS_SIZE_MAX                                (SI57X_ADDR_TRANS_SIZE + \
                                                               SI57X_DATA_BYTES_MAX*SI57X_DATA_TRANS_SIZE)

/* FIXME: This is valid for our part number. For different part number, it is different */
#define SI57X_FOUT_FACTORY_DFLT                             155490000.0 /* in Hertz */

#define SI57X_MIN_FREQ                                      10000000L
#define SI57X_MAX_FREQ                                      1417500000L

#define SI57X_FDCO_MIN                                      4850000000LL
#define SI57X_FDCO_MAX                                      5670000000LL
#define SI57X_FDCO_CENTER                                   ((FDCO_MIN + FDCO_MAX) / 2)

#define SI57X_REG_HS_N1                                     7

/* Definition for register SI57X_REG_HS_N1 */
#define SI57X_HS_N1_N1_6_2_SIZE                             5
#define SI57X_HS_N1_N1_6_2_SHIFT                            0
#define SI57X_HS_N1_N1_6_2_MASK                             WBGEN2_GEN_MASK(SI57X_HS_N1_N1_6_2_SHIFT, \
                                                                                  SI57X_HS_N1_N1_6_2_SIZE)
#define SI57X_HS_N1_N1_6_2_W(value)                         WBGEN2_GEN_WRITE(value, SI57X_HS_N1_N1_6_2_SHIFT, \
                                                                                  SI57X_HS_N1_N1_6_2_SIZE)
#define SI57X_HS_N1_N1_6_2_R(reg)                           WBGEN2_GEN_READ(reg, SI57X_HS_N1_N1_6_2_SHIFT, \
                                                                                  SI57X_HS_N1_N1_6_2_SIZE)

#define SI57X_HS_N1_HS_SIZE                                 3
#define SI57X_HS_N1_HS_SHIFT                                5
#define SI57X_HS_N1_HS_MASK                                 WBGEN2_GEN_MASK(SI57X_HS_N1_HS_SHIFT, \
                                                                                      SI57X_HS_N1_HS_SIZE)
#define SI57X_HS_N1_HS_W(value)                             WBGEN2_GEN_WRITE(value, SI57X_HS_N1_HS_SHIFT, \
                                                                                      SI57X_HS_N1_HS_SIZE)
#define SI57X_HS_N1_HS_R(reg)                               WBGEN2_GEN_READ(reg, SI57X_HS_N1_HS_SHIFT, \
                                                                                      SI57X_HS_N1_HS_SIZE)
#define SI57X_HS_N1_HS_OFFSET                               4

#define SI57X_REG_N1_RFREQ0                                 8

/* Definition for register SI57X_REG_N1_RFREQ0 */
#define SI57X_N1_RFREQ0_RFREQ_37_32_SIZE                    6
#define SI57X_N1_RFREQ0_RFREQ_37_32_SHIFT                   0
#define SI57X_N1_RFREQ0_RFREQ_37_32_MASK                    WBGEN2_GEN_MASK(SI57X_N1_RFREQ0_RFREQ_37_32_SHIFT, \
                                                                         SI57X_N1_RFREQ0_RFREQ_37_32_SIZE)
#define SI57X_N1_RFREQ0_RFREQ_37_32_W(value)                WBGEN2_GEN_WRITE(value, SI57X_N1_RFREQ0_RFREQ_37_32_SHIFT, \
                                                                         SI57X_N1_RFREQ0_RFREQ_37_32_SIZE)
#define SI57X_N1_RFREQ0_RFREQ_37_32_R(reg)                  WBGEN2_GEN_READ(reg, SI57X_N1_RFREQ0_RFREQ_37_32_SHIFT, \
                                                                         SI57X_N1_RFREQ0_RFREQ_37_32_SIZE)

#define SI57X_N1_RFREQ0_N1_1_0_SIZE                         2
#define SI57X_N1_RFREQ0_N1_1_0_SHIFT                        6
#define SI57X_N1_RFREQ0_N1_1_0_MASK                         WBGEN2_GEN_MASK(SI57X_N1_RFREQ0_N1_1_0_SHIFT, \
                                                                              SI57X_N1_RFREQ0_N1_1_0_SIZE)
#define SI57X_N1_RFREQ0_N1_1_0_W(value)                     WBGEN2_GEN_WRITE(value, SI57X_N1_RFREQ0_N1_1_0_SHIFT, \
                                                                              SI57X_N1_RFREQ0_N1_1_0_SIZE)
#define SI57X_N1_RFREQ0_N1_1_0_R(reg)                       WBGEN2_GEN_READ(reg, SI57X_N1_RFREQ0_N1_1_0_SHIFT, \
                                                                              SI57X_N1_RFREQ0_N1_1_0_SIZE)

#define SI57X_HS_N1_N1_6_2_SHIFT_MSB                        SI57X_N1_RFREQ0_N1_1_0_SIZE

#define SI57X_REG_RFREQ1                                    9

/* Definition for register SI57X_REG_RFREQ1 */
#define SI57X_RFREQ1_RFREQ_31_24_SIZE                       8
#define SI57X_RFREQ1_RFREQ_31_24_SHIFT                      0
#define SI57X_RFREQ1_RFREQ_31_24_MASK                       WBGEN2_GEN_MASK(SI57X_RFREQ1_RFREQ_31_24_SHIFT, \
                                                                            SI57X_RFREQ1_RFREQ_31_24_SIZE)
#define SI57X_RFREQ1_RFREQ_31_24_W(value)                   WBGEN2_GEN_WRITE(value, SI57X_RFREQ1_RFREQ_31_24_SHIFT, \
                                                                            SI57X_RFREQ1_RFREQ_31_24_SIZE)
#define SI57X_RFREQ1_RFREQ_31_24_R(reg)                     WBGEN2_GEN_READ(reg, SI57X_RFREQ1_RFREQ_31_24_SHIFT, \
                                                                            SI57X_RFREQ1_RFREQ_31_24_SIZE)

#define SI57X_RFREQ1_RFREQ_31_28_SIZE                       4
#define SI57X_RFREQ1_RFREQ_31_28_SHIFT                      0
#define SI57X_RFREQ1_RFREQ_31_28_MASK                       WBGEN2_GEN_MASK(SI57X_RFREQ1_RFREQ_31_28_SHIFT, \
                                                                            SI57X_RFREQ1_RFREQ_31_28_SIZE)

#define SI57X_RFREQ1_RFREQ_27_24_SIZE                       4
#define SI57X_RFREQ1_RFREQ_27_24_SHIFT                      4
#define SI57X_RFREQ1_RFREQ_27_24_MASK                       WBGEN2_GEN_MASK(SI57X_RFREQ1_RFREQ_27_24_SHIFT, \
                                                                            SI57X_RFREQ1_RFREQ_27_24_SIZE)

#define SI57X_REG_RFREQ2                                    10

/* Definition for register SI57X_REG_RFREQ2 */
#define SI57X_RFREQ2_RFREQ_23_16_SIZE                       8
#define SI57X_RFREQ2_RFREQ_23_16_SHIFT                      0
#define SI57X_RFREQ2_RFREQ_23_16_MASK                       WBGEN2_GEN_MASK(SI57X_RFREQ2_RFREQ_23_16_SHIFT, \
                                                                            SI57X_RFREQ2_RFREQ_23_16_SIZE)
#define SI57X_RFREQ2_RFREQ_23_16_W(value)                   WBGEN2_GEN_WRITE(value, SI57X_RFREQ2_RFREQ_23_16_SHIFT, \
                                                                            SI57X_RFREQ2_RFREQ_23_16_SIZE)
#define SI57X_RFREQ2_RFREQ_23_16_R(reg)                     WBGEN2_GEN_READ(reg, SI57X_RFREQ2_RFREQ_23_16_SHIFT, \
                                                                            SI57X_RFREQ2_RFREQ_23_16_SIZE)

#define SI57X_REG_RFREQ3                                    11

/* Definition for register SI57X_REG_RFREQ3 */
#define SI57X_RFREQ3_RFREQ_15_8_SIZE                        8
#define SI57X_RFREQ3_RFREQ_15_8_SHIFT                       0
#define SI57X_RFREQ3_RFREQ_15_8_MASK                        WBGEN2_GEN_MASK(SI57X_RFREQ3_RFREQ_15_8_SHIFT, \
                                                                             SI57X_RFREQ3_RFREQ_15_8_SIZE)
#define SI57X_RFREQ3_RFREQ_15_8_W(value)                    WBGEN2_GEN_WRITE(value, SI57X_RFREQ3_RFREQ_15_8_SHIFT, \
                                                                             SI57X_RFREQ3_RFREQ_15_8_SIZE)
#define SI57X_RFREQ3_RFREQ_15_8_R(reg)                      WBGEN2_GEN_READ(reg, SI57X_RFREQ3_RFREQ_15_8_SHIFT, \
                                                                             SI57X_RFREQ3_RFREQ_15_8_SIZE)

#define SI57X_REG_RFREQ4                                    12

/* Definition for register SI57X_REG_RFREQ4 */
#define SI57X_RFREQ4_RFREQ_7_0_SIZE                         8
#define SI57X_RFREQ4_RFREQ_7_0_SHIFT                        0
#define SI57X_RFREQ4_RFREQ_7_0_MASK                         WBGEN2_GEN_MASK(SI57X_RFREQ4_RFREQ_7_0_SHIFT, \
                                                                              SI57X_RFREQ4_RFREQ_7_0_SIZE)
#define SI57X_RFREQ4_RFREQ_7_0_W(value)                     WBGEN2_GEN_WRITE(value, SI57X_RFREQ4_RFREQ_7_0_SHIFT, \
                                                                              SI57X_RFREQ4_RFREQ_7_0_SIZE)
#define SI57X_RFREQ4_RFREQ_7_0_R(reg)                       WBGEN2_GEN_READ(reg, SI57X_RFREQ4_RFREQ_7_0_SHIFT, \
                                                                              SI57X_RFREQ4_RFREQ_7_0_SIZE)

/* RFREQ general definitions */
#define SI57X_RFREQ_FRAC_SIZE                               28
#define SI57X_RFREQ_INTEGER_SIZE                            10

#define SI57X_REG_CONTROL                                   135

/* Definition for register SI57X_REG_CONTROL */
#define SI57X_CONTROL_RECALL_SHIFT                          0
#define SI57X_CONTROL_RECALL_SIZE                           1
#define SI57X_CONTROL_RECALL                                WBGEN2_GEN_MASK(SI57X_CONTROL_RECALL_SHIFT, \
                                                                       SI57X_CONTROL_RECALL_SIZE)

#define SI57X_CONTROL_FREEZE_ADC_SHIFT                      4
#define SI57X_CONTROL_FREEZE_ADC_SIZE                       1
#define SI57X_CONTROL_FREEZE_ADC                            WBGEN2_GEN_MASK(SI57X_CONTROL_FREEZE_ADC_SHIFT, \
                                                                   SI57X_CONTROL_FREEZE_ADC_SIZE)

#define SI57X_CONTROL_FREEZE_M_SHIFT                        5
#define SI57X_CONTROL_FREEZE_M_SIZE                         1
#define SI57X_CONTROL_FREEZE_M                              WBGEN2_GEN_MASK(SI57X_CONTROL_FREEZE_M_SHIFT, \
                                                                     SI57X_CONTROL_FREEZE_M_SIZE)

#define SI57X_CONTROL_NEWFREQ_SHIFT                         6
#define SI57X_CONTROL_NEWFREQ_SIZE                          1
#define SI57X_CONTROL_NEWFREQ                               WBGEN2_GEN_MASK(SI57X_CONTROL_NEWFREQ_SHIFT, \
                                                                      SI57X_CONTROL_NEWFREQ_SIZE)

#define SI57X_CONTROL_RESET_SHIFT                           7
#define SI57X_CONTROL_RESET_SIZE                            1
#define SI57X_CONTROL_RESET                                 WBGEN2_GEN_MASK(SI57X_CONTROL_RESET_SHIFT, \
                                                                        SI57X_CONTROL_RESET_SIZE)

#define SI57X_REG_FREEZE_DCO                                137

/* Definition for register SI57X_REG_FREEZE_DCO */
#define SI57X_FREEZE_DCO_SHIFT                              4
#define SI57X_FREEZE_DCO_SIZE                               1
#define SI57X_FREEZE_DCO                                    WBGEN2_GEN_MASK(SI57X_FREEZE_DCO_SHIFT, \
                                                                           SI57X_FREEZE_DCO_SIZE)

#define SI57X_REG_START                                     SI57X_REG_HS_N1

#define SI57X_NUM_DIV_REGS                                  6
#define SI57X_NUM_RFREQ_REGS                                5

#endif

