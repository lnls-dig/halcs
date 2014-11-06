/*
* Copyright (C) 2014 LNLS (www.lnls.br)
* Author: Lucas Russo <lucas.russo@lnls.br>
*
* Released according to the GNU LGPL, version 3 or any later version.
*/

#ifndef _AD9510_REGS_H_
#define _AD9510_REGS_H_

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

#define AD9510_TRASNS_SIZE                          24          /* in bits */

/* AD9510 Instruction Header for 24-bit transfers */

/* AD9510 Transfer type field */
#define AD9510_HDR_RW_SIZE                          1
#define AD9510_HDR_RW_SHIFT                         23
#define AD9510_HDR_RW                               WBGEN2_GEN_MASK(AD9510_HDR_RW_SHIFT, \
                                                                    AD9510_HDR_RW_SIZE)
/* AD9510 Byte transfer field */
#define AD9510_HDR_BT_SIZE                          2
#define AD9510_HDR_BT_SHIFT                         21
#define AD9510_HDR_BT_MASK                          WBGEN2_GEN_MASK(AD9510_HDR_BT_SHIFT, \
                                                                   AD9510_HDR_BT_SIZE)
#define AD9510_HDR_BT_W(value)                      WBGEN2_GEN_WRITE(value, AD9510_HDR_BT_SHIFT, \
                                                                   AD9510_HDR_BT_SIZE)
#define AD9510_HDR_BT_R(reg)                        WBGEN2_GEN_READ(reg, AD9510_HDR_BT_SHIFT, \
                                                                        AD9510_HDR_BT_SIZE)

/* AD9510 Address field. Only the bits 0 to 6 are available. The remaining 7 to 12
 * are fixed to 0, as mandated by the AD9510 rev. B datasheet */
#define AD9510_HDR_ADDR_SIZE                        7
#define AD9510_HDR_ADDR_SHIFT                       8
#define AD9510_HDR_ADDR_MASK                        WBGEN2_GEN_MASK(AD9510_HDR_ADDR_SHIFT, \
                                                                        AD9510_HDR_ADDR_SIZE)
#define AD9510_HDR_ADDR_W(value)                    WBGEN2_GEN_WRITE(value, AD9510_HDR_ADDR_SHIFT, \
                                                                        AD9510_HDR_ADDR_SIZE)
#define AD9510_HDR_ADDR_R(reg)                      WBGEN2_GEN_READ(reg, AD9510_HDR_ADDR_SHIFT, \
                                                                            AD9510_HDR_ADDR_SIZE)

/* AD9510 Data for 24-bit transfers */
#define AD9510_DATA_SIZE                            8
#define AD9510_DATA_SHIFT                           0
#define AD9510_DATA_MASK                            WBGEN2_GEN_MASK(AD9510_DATA_SHIFT, \
                                                                        AD9510_DATA_SIZE)
#define AD9510_DATA_W(value)                        WBGEN2_GEN_WRITE(value, AD9510_DATA_SHIFT, \
                                                                        AD9510_DATA_SIZE)
#define AD9510_DATA_R(reg)                          WBGEN2_GEN_READ(reg, AD9510_DATA_SHIFT, \
                                                                        AD9510_DATA_SIZE)

/* AD9510 Register map, as described by the AD9510 reb. B datasheet. Not all of them
 * are descried here, just the ones we use */
#define AD9510_REG_CFG_SERIAL                       0x00

/* Definition for register AD9510_REG_CFG_SERIAL */
#define AD9510_CFG_SERIAL_LONG_INST_SHIFT           4
#define AD9510_CFG_SERIAL_LONG_INST_SIZE            1
#define AD9510_CFG_SERIAL_LONG_INST                 WBGEN2_GEN_MASK(AD9510_CFG_SERIAL_LONG_INST_SHIFT, \
                                                                    AD9510_CFG_SERIAL_LONG_INST_SIZE)

#define AD9510_CFG_SERIAL_SOFT_RST_SHIFT            5
#define AD9510_CFG_SERIAL_SOFT_RST_SIZE             1
#define AD9510_CFG_SERIAL_SOFT_RST                  WBGEN2_GEN_MASK(AD9510_CFG_SERIAL_SOFT_RST_SHIFT, \
                                                                    AD9510_CFG_SERIAL_SOFT_RST_SIZE)

#define AD9510_CFG_SERIAL_LSB_FIRST_SHIFT           6
#define AD9510_CFG_SERIAL_LSB_FIRST_SIZE            1
#define AD9510_CFG_SERIAL_LSB_FIRST                 WBGEN2_GEN_MASK(AD9510_CFG_SERIAL_LSB_FIRST_SHIFT, \
                                                                    AD9510_CFG_SERIAL_LSB_FIRST_SIZE)

#define AD9510_REG_LVPECL_OUT0                      0x3C
#define AD9510_REG_LVPECL_OUT1                      0x3D
#define AD9510_REG_LVPECL_OUT2                      0x3E
#define AD9510_REG_LVPECL_OUT3                      0x3F

/* Definition for register AD9510_REG_LVPECL_OUT */
#define AD9510_LVPECL_OUT_PDOWN_SIZE                2
#define AD9510_LVPECL_OUT_PDOWN_SHIFT               0
#define AD9510_LVPECL_OUT_PDOWN_MASK                WBGEN2_GEN_MASK(AD9510_LVPECL_OUT_PDOWN_SHIFT, \
                                                        AD9510_LVPECL_OUT_PDOWN_SIZE)
#define AD9510_LVPECL_OUT_PDOWN_W(value)            WBGEN2_GEN_WRITE(value, AD9510_LVPECL_OUT_PDOWN_SHIFT, \
                                                        AD9510_LVPECL_OUT_PDOWN_SIZE)
#define AD9510_LVPECL_OUT_PDOWN_R(reg)              WBGEN2_GEN_READ(reg, AD9510_LVPECL_OUT_PDOWN_SHIFT, \
                                                                    AD9510_LVPECL_OUT_PDOWN_SIZE)

#define AD9510_LVPECL_OUT_LVL_SIZE                  2
#define AD9510_LVPECL_OUT_LVL_SHIFT                 2
#define AD9510_LVPECL_OUT_LVL_MASK                  WBGEN2_GEN_MASK(AD9510_LVPECL_OUT_LVL_SHIFT, \
                                                          AD9510_LVPECL_OUT_LVL_SIZE)
#define AD9510_LVPECL_OUT_LVL_W(value)              WBGEN2_GEN_WRITE(value, AD9510_LVPECL_OUT_LVL_SHIFT, \
                                                          AD9510_LVPECL_OUT_LVL_SIZE)
#define AD9510_LVPECL_OUT_LVL_R(reg)                WBGEN2_GEN_READ(reg, AD9510_LVPECL_OUT_LVL_SHIFT, \
                                                                      AD9510_LVPECL_OUT_LVL_SIZE)

#define AD9510_REG_LVDS_CMOS_OUT4                   0x40
#define AD9510_REG_LVDS_CMOS_OUT5                   0x41
#define AD9510_REG_LVDS_CMOS_OUT6                   0x42
#define AD9510_REG_LVDS_CMOS_OUT7                   0x43

/* Definition for register AD9510_REG_LVDS_CMOS_OUT */
#define AD9510_LVDS_CMOS_PDOWN_SHIFT                0
#define AD9510_LVDS_CMOS_PDOWN_SIZE                 1
#define AD9510_LVDS_CMOS_PDOWN                      WBGEN2_GEN_MASK(AD9510_LVDS_CMOS_PDOWN_SHIFT, \
                                                                         AD9510_LVDS_CMOS_PDOWN_SIZE)

#define AD9510_LVDS_CMOS_CURR_SIZE                  2
#define AD9510_LVDS_CMOS_CURR_SHIFT                 1
#define AD9510_LVDS_CMOS_CURR_MASK                  WBGEN2_GEN_MASK(AD9510_LVDS_CMOS_CURR_SHIFT, \
                                                          AD9510_LVDS_CMOS_CURR_SIZE)
#define AD9510_LVDS_CMOS_CURR_W(value)              WBGEN2_GEN_WRITE(value, AD9510_LVDS_CMOS_CURR_SHIFT, \
                                                          AD9510_LVDS_CMOS_CURR_SIZE)
#define AD9510_LVDS_CMOS_CURR_R(reg)                WBGEN2_GEN_READ(reg, AD9510_LVDS_CMOS_CURR_SHIFT, \
                                                                      AD9510_LVDS_CMOS_CURR_SIZE)

#define AD9510_LVDS_CMOS_SELECT_SHIFT               3
#define AD9510_LVDS_CMOS_SELECT_SIZE                1
#define AD9510_LVDS_CMOS_SELECT                     WBGEN2_GEN_MASK(AD9510_LVDS_CMOS_SELECT_SHIFT, \
                                                                         AD9510_LVDS_CMOS_SELECT_SIZE)

#define AD9510_LVDS_CMOS_INV_CMOS_SHIFT             4
#define AD9510_LVDS_CMOS_INV_CMOS_SIZE              1
#define AD9510_LVDS_CMOS_INV_CMOS                   WBGEN2_GEN_MASK(AD9510_LVDS_CMOS_INV_CMOS_SHIFT, \
                                                                         AD9510_LVDS_CMOS_INV_CMOS_SIZE)

#define AD9510_REG_CLK_OPT                          0x45

/* Definition for register AD9510_REG_CLK_OPT */
#define AD9510_CLK_OPT_SEL_CLK1_SHIFT               0
#define AD9510_CLK_OPT_SEL_CLK1_SIZE                1
#define AD9510_CLK_OPT_SEL_CLK1                     WBGEN2_GEN_MASK(AD9510_CLK_OPT_SEL_CLK1_SHIFT,\
                                                                        AD9510_CLK_OPT_SEL_CLK1_SIZE)

#define AD9510_CLK_OPT_CLK1_PD_SHIFT                1
#define AD9510_CLK_OPT_CLK1_PD_SIZE                 1
#define AD9510_CLK_OPT_CLK1_PD                      WBGEN2_GEN_MASK(AD9510_CLK_OPT_CLK1_PD_SHIFT,\
                                                                        AD9510_CLK_OPT_CLK1_PD_SIZE)

#define AD9510_CLK_OPT_CLK2_PD_SHIFT                2
#define AD9510_CLK_OPT_CLK2_PD_SIZE                 1
#define AD9510_CLK_OPT_CLK2_PD                      WBGEN2_GEN_MASK(AD9510_CLK_OPT_CLK2_PD_SHIFT,\
                                                                        AD9510_CLK_OPT_CLK2_PD_SIZE)

#define AD9510_CLK_OPT_PS_PD_SHIFT                  3
#define AD9510_CLK_OPT_PS_PD_SIZE                   1
#define AD9510_CLK_OPT_PS_PD                        WBGEN2_GEN_MASK(AD9510_CLK_OPT_PS_PD_SHIFT,\
                                                                        AD9510_CLK_OPT_PS_PD_SIZE)

#define AD9510_CLK_OPT_REFIN_PD_SHIFT               4
#define AD9510_CLK_OPT_REFIN_PD_SIZE                1
#define AD9510_CLK_OPT_REFIN_PD                     WBGEN2_GEN_MASK(AD9510_CLK_OPT_REFIN_PD_SHIFT,\
                                                                        AD9510_CLK_OPT_REFIN_PD_SIZE)

#define AD9510_CLK_OPT_ALLCLKS_PD_SHIFT             5
#define AD9510_CLK_OPT_ALLCLKS_PD_SIZE              1
#define AD9510_CLK_OPT_ALLCLKS_PD                   WBGEN2_GEN_MASK(AD9510_CLK_OPT_ALLCLKS_PD_SHIFT,\
                                                                        AD9510_CLK_OPT_ALLCLKS_PD_SIZE)

#define AD9510_REG_DIV0_DCYCLE                      0x48
#define AD9510_REG_DIV1_DCYCLE                      0x4A
#define AD9510_REG_DIV2_DCYCLE                      0x4C
#define AD9510_REG_DIV3_DCYCLE                      0x4E
#define AD9510_REG_DIV4_DCYCLE                      0x50
#define AD9510_REG_DIV5_DCYCLE                      0x52
#define AD9510_REG_DIV6_DCYCLE                      0x54
#define AD9510_REG_DIV7_DCYCLE                      0x56

/* Definition for register AD9510_REG_DIVX_DCYCLE */
#define AD9510_DIV_DCYCLE_HIGH_SIZE                 4
#define AD9510_DIV_DCYCLE_HIGH_SHIFT                0
#define AD9510_DIV_DCYCLE_HIGH_MASK                 WBGEN2_GEN_MASK(AD9510_DIV_DCYCLE_HIGH_SHIFT, \
                                                                        AD9510_DIV_DCYCLE_HIGH_SIZE)
#define AD9510_DIV_DCYCLE_HIGH_W(value)             WBGEN2_GEN_WRITE(value, AD9510_DIV_DCYCLE_HIGH_SHIFT, \
                                                                        AD9510_DIV_DCYCLE_HIGH_SIZE)
#define AD9510_DIV_DCYCLE_HIGH_R(reg)               WBGEN2_GEN_READ(reg, AD9510_DIV_DCYCLE_HIGH_SHIFT, \
                                                                        AD9510_DIV_DCYCLE_HIGH_SIZE)

#define AD9510_DIV_DCYCLE_LOW_SIZE                  4
#define AD9510_DIV_DCYCLE_LOW_SHIFT                 4
#define AD9510_DIV_DCYCLE_LOW_MASK                  WBGEN2_GEN_MASK(AD9510_DIV_DCYCLE_LOW_SHIFT, \
                                                                        AD9510_DIV_DCYCLE_LOW_SIZE)
#define AD9510_DIV_DCYCLE_LOW_W(value)              WBGEN2_GEN_WRITE(value, AD9510_DIV_DCYCLE_LOW_SHIFT, \
                                                                        AD9510_DIV_DCYCLE_LOW_SIZE)
#define AD9510_DIV_DCYCLE_LOW_R(reg)                WBGEN2_GEN_READ(reg, AD9510_DIV_DCYCLE_LOW_SHIFT, \
                                                                        AD9510_DIV_DCYCLE_LOW_SIZE)

#define AD9510_REG_DIV0_OPT                         0x49
#define AD9510_REG_DIV1_OPT                         0x4B
#define AD9510_REG_DIV2_OPT                         0x4D
#define AD9510_REG_DIV3_OPT                         0x4F
#define AD9510_REG_DIV4_OPT                         0x51
#define AD9510_REG_DIV5_OPT                         0x53
#define AD9510_REG_DIV6_OPT                         0x55
#define AD9510_REG_DIV7_OPT                         0x57

/* Definition for register AD9510_REG_DIVX_OPT */
#define AD9510_DIV_OPT_PHASE_SIZE                   4
#define AD9510_DIV_OPT_PHASE_SHIFT                  0
#define AD9510_DIV_OPT_PHASE_MASK                   WBGEN2_GEN_MASK(AD9510_DIV_OPT_PHASE_SHIFT, \
                                                                        AD9510_DIV_OPT_PHASE_SIZE)
#define AD9510_DIV_OPT_PHASE_W(value)               WBGEN2_GEN_WRITE(value, AD9510_DIV_OPT_PHASE_SHIFT, \
                                                                        AD9510_DIV_OPT_PHASE_SIZE)
#define AD9510_DIV_OPT_PHASE_R(reg)                 WBGEN2_GEN_READ(reg, AD9510_DIV_OPT_PHASE_SHIFT, \
                                                                        AD9510_DIV_OPT_PHASE_SIZE)

#define AD9510_DIV_START_HIGH_SHIFT                 4
#define AD9510_DIV_START_HIGH_SIZE                  1
#define AD9510_DIV_START_HIGH                       WBGEN2_GEN_MASK(AD9510_DIV_START_HIGH_SHIFT,\
                                                                        AD9510_DIV_START_HIGH_SIZE)

#define AD9510_DIV_FORCE_SHIFT                      5
#define AD9510_DIV_FORCE_SIZE                       1
#define AD9510_DIV_FORCE                            WBGEN2_GEN_MASK(AD9510_DIV_FORCE_SHIFT,\
                                                                        AD9510_DIV_FORCE_SIZE)

#define AD9510_DIV_IGN_CHIP_SYNC_SHIFT              6
#define AD9510_DIV_IGN_CHIP_SYNC_SIZE               1
#define AD9510_DIV_IGN_CHIP_SYNC                    WBGEN2_GEN_MASK(AD9510_DIV_IGN_CHIP_SYNC_SHIFT,\
                                                                        AD9510_DIV_IGN_CHIP_SYNC_SIZE)

#define AD9510_DIV_BYPASS_SHIFT                     7
#define AD9510_DIV_BYPASS_SIZE                      1
#define AD9510_DIV_BYPASS                           WBGEN2_GEN_MASK(AD9510_DIV_BYPASS_SHIFT,\
                                                                        AD9510_DIV_BYPASS_SIZE)

#define AD9510_REG_UPDATE_REGS                      0x5A

/* Definition for register AD9510_REG_UPDATE_REGS */
#define AD9510_UPDATE_REGS_VAL_SHIFT                0
#define AD9510_UPDATE_REGS_VAL_SIZE                 1
#define AD9510_UPDATE_REGS_VAL                      WBGEN2_GEN_MASK(AD9510_UPDATE_REGS_VAL_SHIFT,\
                                                                    AD9510_UPDATE_REGS_VAL_SIZE)

#define AD9510_REG_FUNCTION                         0x58

/* Definition for register AD9510_REG_FUNCTION */
#define AD9510_FUNCTION_SYNC_EN_SHIFT               0
#define AD9510_FUNCTION_SYNC_EN_SIZE                1
#define AD9510_FUNCTION_SYNC_EN                     WBGEN2_GEN_MASK(AD9510_FUNCTION_SYNC_EN_SHIFT,\
                                                               AD9510_FUNCTION_SYNC_EN_SIZE)

#define AD9510_FUNCTION_SYNC_SEL_SHIFT              1
#define AD9510_FUNCTION_SYNC_SEL_SIZE               1
#define AD9510_FUNCTION_SYNC_SEL                    WBGEN2_GEN_MASK(AD9510_FUNCTION_SYNC_SEL_SHIFT,\
                                                               AD9510_FUNCTION_SYNC_SEL_SIZE)

#define AD9510_FUNCTION_SYNC_REG_SHIFT              2
#define AD9510_FUNCTION_SYNC_REG_SIZE               1
#define AD9510_FUNCTION_SYNC_REG                    WBGEN2_GEN_MASK(AD9510_FUNCTION_SYNC_REG_SHIFT,\
                                                               AD9510_FUNCTION_SYNC_REG_SIZE)

#define AD9510_FUNCTION_REF_PD_SHIFT                3
#define AD9510_FUNCTION_REF_PD_SIZE                 1
#define AD9510_FUNCTION_REF_PD                      WBGEN2_GEN_MASK(AD9510_FUNCTION_REF_PD_SHIFT,\
                                                               AD9510_FUNCTION_REF_PD_SIZE)

#define AD9510_FUNCTION_SYNC_PD_SHIFT               4
#define AD9510_FUNCTION_SYNC_PD_SIZE                1
#define AD9510_FUNCTION_SYNC_PD                     WBGEN2_GEN_MASK(AD9510_FUNCTION_SYNC_PD_SHIFT,\
                                                               AD9510_FUNCTION_SYNC_PD_SIZE)

#define AD9510_FUNCTION_FUNC_SEL_SIZE               2
#define AD9510_FUNCTION_FUNC_SEL_SHIFT              5
#define AD9510_FUNCTION_FUNC_SEL_MASK               WBGEN2_GEN_MASK(AD9510_FUNCTION_FUNC_SEL_SHIFT, \
                                                                    AD9510_FUNCTION_FUNC_SEL_SIZE)
#define AD9510_FUNCTION_FUNC_SEL_W(value)           WBGEN2_GEN_WRITE(value, AD9510_FUNCTION_FUNC_SEL_SHIFT, \
                                                                    AD9510_FUNCTION_FUNC_SEL_SIZE)
#define AD9510_FUNCTION_FUNC_SEL_R(reg)             WBGEN2_GEN_READ(reg, AD9510_FUNCTION_FUNC_SEL_SHIFT, \
                                                                    AD9510_FUNCTION_FUNC_SEL_SIZE)

#endif

