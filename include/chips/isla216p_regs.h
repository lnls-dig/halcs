/*
* Copyright (C) 2014 LNLS (www.lnls.br)
* Author: Lucas Russo <lucas.russo@lnls.br>
*
* Released according to the GNU GPL, version 3 or any later version.
*/

#ifndef _ISLA216_REGS_H_
#define _ISLA216_REGS_H_

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

#if 0
#define ISLA216P_ADC0                                            0
#define ISLA216P_SS_ADC0                                         ((1 << 0) & 0xff)
#define ISLA216P_ADC1                                            1
#define ISLA216P_SS_ADC1                                         ((1 << 1) & 0xff)
#define ISLA216P_ADC2                                            2
#define ISLA216P_SS_ADC2                                         ((1 << 2) & 0xff)
#define ISLA216P_ADC3                                            3
#define ISLA216P_SS_ADC3                                         ((1 << 3) & 0xff)

#define NUM_ISLA216P                                             4
#endif


/* ISLA216P Instruction Header */

/* ISLA216P has 16 bits for instruction/address: addr(13)+length(2)+rw(1) */

/* ISLA216P Transfer type field */
#define ISLA216P_HDR_RW_SIZE                        1
#define ISLA216P_HDR_RW_SHIFT                       23
#define ISLA216P_HDR_RW                             WBGEN2_GEN_MASK(ISLA216P_HDR_RW_SHIFT, \
                                                               ISLA216P_HDR_RW_SIZE)

/* ISLA216P Byte transfer field */
#define ISLA216P_HDR_BT_SIZE                        2
#define ISLA216P_HDR_BT_SHIFT                       21
#define ISLA216P_HDR_BT_MASK                        WBGEN2_GEN_MASK(ISLA216P_HDR_BT_SHIFT, \
                                                              ISLA216P_HDR_BT_SIZE)
#define ISLA216P_HDR_BT_W(value)                    WBGEN2_GEN_WRITE(value, ISLA216P_HDR_BT_SHIFT, \
                                                              ISLA216P_HDR_BT_SIZE)
#define ISLA216P_HDR_BT_R(reg)                      WBGEN2_GEN_READ(reg, ISLA216P_HDR_BT_SHIFT, \
                                                                      ISLA216P_HDR_BT_SIZE)

/* ISLA216P Address field */
#define ISLA216P_HDR_ADDR_SIZE                      13
#define ISLA216P_HDR_ADDR_SHIFT                     8
#define ISLA216P_HDR_ADDR_MASK                      WBGEN2_GEN_MASK(ISLA216P_HDR_ADDR_SHIFT, \
                                                                      ISLA216P_HDR_ADDR_SIZE)
#define ISLA216P_HDR_ADDR_W(value)                  WBGEN2_GEN_WRITE(value, ISLA216P_HDR_ADDR_SHIFT, \
                                                                      ISLA216P_HDR_ADDR_SIZE)
#define ISLA216P_HDR_ADDR_R(reg)                    WBGEN2_GEN_READ(reg, ISLA216P_HDR_ADDR_SHIFT, \
                                                                          ISLA216P_HDR_ADDR_SIZE)

#define ISLA216P_INSTADDR_SIZE                      (ISLA216P_HDR_RW_SIZE + \
                                                      ISLA216P_HDR_BT_SIZE + \
                                                      ISLA216P_HDR_ADDR_SIZE)

/* ISLA216P Data */

/* ISLA216P Data for 8-bit transfers */
#define ISLA216P_DATA_SIZE                          8
#define ISLA216P_DATA_SHIFT                         0
#define ISLA216P_DATA_MASK                          WBGEN2_GEN_MASK(ISLA216P_DATA_SHIFT, \
                                                                      ISLA216P_DATA_SIZE)
#define ISLA216P_DATA_W(value)                      WBGEN2_GEN_WRITE(value, ISLA216P_DATA_SHIFT, \
                                                                      ISLA216P_DATA_SIZE)
#define ISLA216P_DATA_R(reg)                        WBGEN2_GEN_READ(reg, ISLA216P_DATA_SHIFT, \
                                                                      ISLA216P_DATA_SIZE)

/* ISLA216P transaction size */

#define ISLA216P_TRANS_SIZE                         (ISLA216P_INSTADDR_SIZE + \
                                                            ISLA216P_DATA_SIZE)

/*
 *  Internal ISLA216P register description from ISLA216P25
 *  datasheet Incomplete! Byte addressed!
 */

#define ISLA216P_REG_PORTCONFIG                     0x00

/* Definition for register ISLA216P_REG_PORTCONFIG */

#define ISLA216P_PORTCONFIG_SDO_ACTIVE_LSB_SIZE     1
#define ISLA216P_PORTCONFIG_SDO_ACTIVE_LSB_SHIFT    0
#define ISLA216P_PORTCONFIG_SDO_ACTIVE_LSB          WBGEN2_GEN_MASK(ISLA216P_PORTCONFIG_SDO_ACTIVE_LSB_SHIFT, \
                                                        ISLA216P_PORTCONFIG_SDO_ACTIVE_LSB_SIZE)

#define ISLA216P_PORTCONFIG_LSB_FIRST_LSB_SIZE      1
#define ISLA216P_PORTCONFIG_LSB_FIRST_LSB_SHIFT     1
#define ISLA216P_PORTCONFIG_LSB_FIRST_LSB           WBGEN2_GEN_MASK(ISLA216P_PORTCONFIG_LSB_FIRST_LSB_SHIFT, \
                                                         ISLA216P_PORTCONFIG_LSB_FIRST_LSB_SIZE)

#define ISLA216P_PORTCONFIG_SOFT_RESET_LSB_SIZE     1
#define ISLA216P_PORTCONFIG_SOFT_RESET_LSB_SHIFT    2
#define ISLA216P_PORTCONFIG_SOFT_RESET_LSB          WBGEN2_GEN_MASK(ISLA216P_PORTCONFIG_SOFT_RESET_LSB_SHIFT, \
                                                        ISLA216P_PORTCONFIG_SOFT_RESET_LSB_SIZE)

#define ISLA216P_PORTCONFIG_SOFT_RESET_MSB_SIZE     1
#define ISLA216P_PORTCONFIG_SOFT_RESET_MSB_SHIFT    5
#define ISLA216P_PORTCONFIG_SOFT_RESET_MSB          WBGEN2_GEN_MASK(ISLA216P_PORTCONFIG_SOFT_RESET_MSB_SHIFT, \
                                                        ISLA216P_PORTCONFIG_SOFT_RESET_MSB_SIZE)

#define ISLA216P_PORTCONFIG_MSB_FIRST_MSB_SIZE      1
#define ISLA216P_PORTCONFIG_MSB_FIRST_MSB_SHIFT     6
#define ISLA216P_PORTCONFIG_MSB_FIRST_MSB           WBGEN2_GEN_MASK(ISLA216P_PORTCONFIG_MSB_FIRST_MSB_SHIFT, \
                                                         ISLA216P_PORTCONFIG_MSB_FIRST_MSB_SIZE)

#define ISLA216P_PORTCONFIG_SDO_ACTIVE_MSB_SIZE     1
#define ISLA216P_PORTCONFIG_SDO_ACTIVE_MSB_SHIFT    7
#define ISLA216P_PORTCONFIG_SDO_ACTIVE_MSB          WBGEN2_GEN_MASK(ISLA216P_PORTCONFIG_SDO_ACTIVE_MSB_SHIFT, \
                                                        ISLA216P_PORTCONFIG_SDO_ACTIVE_MSB_SIZE)

#define ISLA216P_PORTCONFIG_SDO_ACTIVE              (ISLA216P_PORTCONFIG_SDO_ACTIVE_LSB | ISLA216P_PORTCONFIG_SDO_ACTIVE_MSB)
#define ISLA216P_PORTCONFIG_LSB_FIRST               (ISLA216P_PORTCONFIG_LSB_FIRST_LSB | ISLA216P_PORTCONFIG_MSB_FIRST_MSB)
#define ISLA216P_PORTCONFIG_SOFT_RESET              (ISLA216P_PORTCONFIG_SOFT_RESET_LSB | ISLA216P_PORTCONFIG_SOFT_RESET_MSB)

#define ISLA216P_REG_SPI_BURSTEND                   0x02

/* Definition for register ISLA216P_REG_SPI_BURSTEND */

#define ISLA216P_REG_CHIPID                         0x08

/* Definition for register ISLA216P_CHIPID_REG */

#define ISLA216P_CHIPID_SIZE                        8
#define ISLA216P_CHIPID_SHIFT                       0
#define ISLA216P_CHIPID_MASK                        WBGEN2_GEN_MASK(ISLA216P_CHIPID_SHIFT, \
                                                                             ISLA216P_CHIPID_SIZE)
#define ISLA216P_CHIPID_W(value)                    WBGEN2_GEN_WRITE(value, ISLA216P_CHIPID_SHIFT, \
                                                                             ISLA216P_CHIPID_SIZE)
#define ISLA216P_CHIPID_R(reg)                      WBGEN2_GEN_READ(reg, ISLA216P_CHIPID_SHIFT, \
                                                                             ISLA216P_CHIPID_SIZE)

#define ISLA216P_REG_CHIPVER                        0x09

/* Definition for register ISLA216P_REG_CHIPVER */

#define ISLA216P_CHIPVER_SIZE                       8
#define ISLA216P_CHIPVER_SHIFT                      0
#define ISLA216P_CHIPVER_MASK                       WBGEN2_GEN_MASK(ISLA216P_CHIPVER_SHIFT, \
                                                                            ISLA216P_CHIPVER_SIZE)
#define ISLA216P_CHIPVER_W(value)                   WBGEN2_GEN_WRITE(value, ISLA216P_CHIPVER_SHIFT, \
                                                                            ISLA216P_CHIPVER_SIZE)
#define ISLA216P_CHIPVER_R(reg)                     WBGEN2_GEN_READ(reg, ISLA216P_CHIPVER_SHIFT, \
                                                                            ISLA216P_CHIPVER_SIZE)

#define ISLA216P_REG_MODESADC0                      0x25

/* Definition for register ISLA216P_MODESADC0_REG */

#define ISLA216P_REG_MODESADC1                      0x2b

/* Definition for register ISLA216P_MODESADC1_REG */

#define ISLA216P_REG_CLKDIV                         0x72

/* Definition for register ISLA216P_CLKDIV_REG */

#define ISLA216P_REG_OUTMODEA                       0x73

/* Definition for register ISLA216P_REG_OUTMODEA */

#define ISLA216P_OUTMODEA_FMT_SIZE                  3
#define ISLA216P_OUTMODEA_FMT_SHIFT                 0
#define ISLA216P_OUTMODEA_FMT_MASK                  WBGEN2_GEN_MASK(ISLA216P_OUTMODEA_FMT_SHIFT, \
                                                                       ISLA216P_OUTMODEA_FMT_SIZE)
#define ISLA216P_OUTMODEA_FMT_W(value)              WBGEN2_GEN_WRITE(value, ISLA216P_OUTMODEA_FMT_SHIFT, \
                                                                       ISLA216P_OUTMODEA_FMT_SIZE)
#define ISLA216P_OUTMODEA_FMT_R(reg)                WBGEN2_GEN_READ(reg, ISLA216P_OUTMODEA_FMT_SHIFT, \
                                                                       ISLA216P_OUTMODEA_FMT_SIZE)

#define ISLA216P_OUTMODEA_FMT_2COMPL                0
#define ISLA216P_OUTMODEA_FMT_GRAYCODE              (1<<1)
#define ISLA216P_OUTMODEA_FMT_OFSBIN                (1<<2)

#define ISLA216P_OUTMODEA_MODE_SIZE                 3
#define ISLA216P_OUTMODEA_MODE_SHIFT                5
#define ISLA216P_OUTMODEA_MODE_MASK                 WBGEN2_GEN_MASK(ISLA216P_OUTMODEA_MODE_SHIFT, \
                                                                      ISLA216P_OUTMODEA_MODE_SIZE)
#define ISLA216P_OUTMODEA_MODE_W(value)             WBGEN2_GEN_WRITE(value, ISLA216P_OUTMODEA_MODE_SHIFT, \
                                                                      ISLA216P_OUTMODEA_MODE_SIZE)
#define ISLA216P_OUTMODEA_MODE_R(reg)               WBGEN2_GEN_READ(reg, ISLA216P_OUTMODEA_MODE_SHIFT, \
                                                                      ISLA216P_OUTMODEA_MODE_SIZE)

#define ISLA216P_OUTMODA_MODE_LVDS3                 0
#define ISLA216P_OUTMODA_MODE_LVDS2                 1
#define ISLA216P_OUTMODA_MODE_LVCMOS                (1<<2)

#define ISLA216P_OUTMODEB_REG                       0x74

/* Definition for register ISLA216P_REG_OUTMODEB */

#define ISLA216P_REG_CALSTATUS                      0xb6

/* Definition for register ISLA216P_REG_CALSTATUS */

#define ISLA216P_CALCSTATUS_CALCDONE_SIZE           1
#define ISLA216P_CALCSTATUS_CALCDONE_SHIFT          0
#define ISLA216P_CALCSTATUS_CALCDONE                WBGEN2_GEN_MASK(ISLA216P_CALCSTATUS_CALCDONE_SHIFT, \
                                                              ISLA216P_CALCSTATUS_CALCDONE_SIZE)

#define ISLA216P_REG_TESTIO                         0xC0

/* Definition for register ISLA216P_REG_TESTIO */

#define ISLA216P_TESTIO_USRMODE_SIZE                3
#define ISLA216P_TESTIO_USRMODE_SHIFT               0
#define ISLA216P_TESTIO_USRMODE_MASK                WBGEN2_GEN_MASK(ISLA216P_TESTIO_USRMODE_SHIFT, \
                                                                     ISLA216P_TESTIO_USRMODE_SIZE)
#define ISLA216P_TESTIO_USRMODE_W(value)            WBGEN2_GEN_WRITE(value, ISLA216P_TESTIO_USRMODE_SHIFT, \
                                                                     ISLA216P_TESTIO_USRMODE_SIZE)
#define ISLA216P_TESTIO_USRMODE_R(reg)              WBGEN2_GEN_READ(reg, ISLA216P_TESTIO_USRMODE_SHIFT, \
                                                                     ISLA216P_TESTIO_USRMODE_SIZE)

#define ISLA216P_TESTIO_USRMODE_USRPAT1             0
#define ISLA216P_TESTIO_USRMODE_CYCPAT13            1
#define ISLA216P_TESTIO_USRMODE_CYCPAT135           2
#define ISLA216P_TESTIO_USRMODE_CYCPAT1357          3

#define ISLA216P_TESTIO_OUTMODE_SIZE                4
#define ISLA216P_TESTIO_OUTMODE_SHIFT               4
#define ISLA216P_TESTIO_OUTMODE_MASK                WBGEN2_GEN_MASK(ISLA216P_TESTIO_OUTMODE_SHIFT, \
                                                                     ISLA216P_TESTIO_OUTMODE_SIZE)
#define ISLA216P_TESTIO_OUTMODE_W(value)            WBGEN2_GEN_WRITE(value, ISLA216P_TESTIO_OUTMODE_SHIFT, \
                                                                     ISLA216P_TESTIO_OUTMODE_SIZE)
#define ISLA216P_TESTIO_OUTMODE_R(reg)              WBGEN2_GEN_READ(reg, ISLA216P_TESTIO_OUTMODE_SHIFT, \
                                                                     ISLA216P_TESTIO_OUTMODE_SIZE)

#define ISLA216P_TESTIO_OUTMODE_OFF                 0
#define ISLA216P_TESTIO_OUTMODE_MIDSHORT            1
#define ISLA216P_TESTIO_OUTMODE_PLUS_FSSHORT        2
#define ISLA216P_TESTIO_OUTMODE_MINUS_FSSHORT       3
#define ISLA216P_TESTIO_OUTMODE_RES0                4
#define ISLA216P_TESTIO_OUTMODE_RES1                5
#define ISLA216P_TESTIO_OUTMODE_RES2                6
#define ISLA216P_TESTIO_OUTMODE_RES3                7
#define ISLA216P_TESTIO_OUTMODE_USRPAT              8
#define ISLA216P_TESTIO_OUTMODE_RES4                9
#define ISLA216P_TESTIO_OUTMODE_RAMP                10
#define ISLA216P_TESTIO_OUTMODE_RES5                11
#define ISLA216P_TESTIO_OUTMODE_RES6                12
#define ISLA216P_TESTIO_OUTMODE_RES7                13
#define ISLA216P_TESTIO_OUTMODE_RES8                14
#define ISLA216P_TESTIO_OUTMODE_RES9                15

#define ISLA216P_REG_NAPSLP                         0x25

/* Definition for register ISLA216P_REG_NAPSLP */

#define ISLA216P_NAPSLP_SIZE                        3
#define ISLA216P_NAPSLP_SHIFT                       0
#define ISLA216P_NAPSLP_MASK                        WBGEN2_GEN_MASK(ISLA216P_NAPSLP_SHIFT, \
                                                                     ISLA216P_NAPSLP_SIZE)
#define ISLA216P_NAPSLP_W(value)                    WBGEN2_GEN_WRITE(value, ISLA216P_NAPSLP_SHIFT, \
                                                                     ISLA216P_NAPSLP_SIZE)
#define ISLA216P_NAPSLP_R(reg)                      WBGEN2_GEN_READ(reg, ISLA216P_NAPSLP_SHIFT, \
                                                                     ISLA216P_NAPSLP_SIZE)
#define ISLA216P_NAPSLP_PIN_CONTROL                0
#define ISLA216P_NAPSLP_NORMAL_OPERATION           (1<<0)
#define ISLA216P_NAPSLP_NAP_MODE                   (1<<1) 
#define ISLA216P_NAPSLP_SLEEP_MODE                 (1<<2) 

#endif

