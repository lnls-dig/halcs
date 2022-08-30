/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _PCIE_REGS_H_
#define _PCIE_REGS_H_

/* Define 64-bit macros for PCIe */
#ifndef __WBGEN2_PCIE_MACROS_DEFINED__
#define __WBGEN2_PCIE_MACROS_DEFINED__
#define WBGEN2_PCIE_GEN_MASK(offset, size) ((((uint64_t)1<<(size))-1) << (offset))
#define WBGEN2_PCIE_GEN_WRITE(value, offset, size) (((value) & (((uint64_t)1<<(size))-1)) << (offset))
#define WBGEN2_PCIE_GEN_READ(reg, offset, size) (((reg) >> (offset)) & (((uint64_t)1<<(size))-1))
#define WBGEN2_PCIE_SIGN_EXTEND(value, bits) (((value) & ((uint64_t)1<<bits) ? ~(((uint64_t)1<<(bits))-1): 0 ) | (value))
#endif

/* Some FPGA PCIe constants */
/* SDRAM is accesses via 32-bit BAR (32-bit addressing) */
#define PCIE_SDRAM_PG_SHIFT                 0           /* bits */
#define PCIE_SDRAM_PG_MAX                   20          /* bits */
/* #define PCIE_SDRAM_PG_MAX                   17 */        /* bits */
/* #define PCIE_SDRAM_PG_SIZE                (1<<16)*/   /* in Words (32-bit) */
#define PCIE_SDRAM_PG_SIZE                  (1<<PCIE_SDRAM_PG_MAX)  /* in Bytes (8-bit) */
#define PCIE_SDRAM_PG_MASK                  ((PCIE_SDRAM_PG_SIZE-1) << \
                                                PCIE_SDRAM_PG_SHIFT)

/* One less then PCIE_SDRAM_PG_SIZE, as Wishbone is accessed via
 * 64-bit BAR (64-bit addressed) */
#define PCIE_WB_PG_SHIFT                    0           /* bits */
/*#define PCIE_WB_PG_MAX                      17 */          /* bits */
#define PCIE_WB_PG_MAX                      16          /* bits */
/* #define PCIE_WB_PG_SIZE                  (1<<15)*/    /* in Words (32-bit) */
#define PCIE_WB_PG_SIZE                     (1<<PCIE_WB_PG_MAX)  /* in Bytes (8-bit) */
#define PCIE_WB_PG_MASK                     ((PCIE_WB_PG_SIZE-1) << \
                                                PCIE_WB_PG_SHIFT)

/* Wishbone Word Access Shift (in legacy x86 nomenclature) */
#define WB_QWORD_ACC                        3           /* 64-bit addressing */
#define WB_DWORD_ACC                        2           /* 32-bit addressing */
#define WB_WORD_ACC                         1           /* 16-bit addressing */
#define WB_BYTE_ACC                         0           /* 8-bit addressing */

/************************************************************/
/******************* FPGA PCIe Registers ********************/
/************************************************************/
/* FPGA PCIe registers. These are inside bar0r These must match
 * the FPGA firmware */

#define PCIE_CFG_REG_VERSION                (0 << WB_DWORD_ACC)
#define PCIE_CFG_REG_IRQ_STAT               (2 << WB_DWORD_ACC)

/* IRQ Enable. Write '1' turns on the interrupt, '0' masks. */
#define PCIE_CFG_REG_IRQ_EN                 (4 << WB_DWORD_ACC)

#define PCIE_CFG_REG_ERROR                  (6 << WB_DWORD_ACC)  /* Unused */
#define PCIE_CFG_REG_SDRAM_PG               (7 << WB_DWORD_ACC)
#define PCIE_CFG_REG_STATUS                 (8 << WB_DWORD_ACC)

#define PCIE_CFG_STATUS_DDR_RDY             (0x1 << 7)

#define PCIE_CFG_REG_WB_PG                  (9 << WB_DWORD_ACC)
#define PCIE_CFG_REG_CONTROL                (10 << WB_DWORD_ACC)

/* Upstream DMA channel Constants */
#define PCIE_CFG_REG_DMA_US_PAH             (11 << WB_DWORD_ACC)
#define PCIE_CFG_REG_DMA_US_PAL             (12 << WB_DWORD_ACC)
#define PCIE_CFG_REG_DMA_US_HAH             (13 << WB_DWORD_ACC)
#define PCIE_CFG_REG_DMA_US_HAL             (14 << WB_DWORD_ACC)
#define PCIE_CFG_REG_DMA_US_BDAH            (15 << WB_DWORD_ACC)
#define PCIE_CFG_REG_DMA_US_BDAL            (16 << WB_DWORD_ACC)
#define PCIE_CFG_REG_DMA_US_LENG            (17 << WB_DWORD_ACC)
#define PCIE_CFG_REG_DMA_US_CTRL            (18 << WB_DWORD_ACC)
#define PCIE_CFG_REG_DMA_US_STA             (19 << WB_DWORD_ACC)

/* First register of the block*/
#define PCIE_CFG_REG_DMA_US_BASE            PCIE_CFG_REG_DMA_US_PAH

/* Downstream DMA channel Constants */
#define PCIE_CFG_REG_DMA_DS_PAH             (20 << WB_DWORD_ACC)
#define PCIE_CFG_REG_DMA_DS_PAL             (21 << WB_DWORD_ACC)
#define PCIE_CFG_REG_DMA_DS_HAH             (22 << WB_DWORD_ACC)
#define PCIE_CFG_REG_DMA_DS_HAL             (23 << WB_DWORD_ACC)
#define PCIE_CFG_REG_DMA_DS_BDAH            (24 << WB_DWORD_ACC)
#define PCIE_CFG_REG_DMA_DS_BDAL            (25 << WB_DWORD_ACC)
#define PCIE_CFG_REG_DMA_DS_LENG            (26 << WB_DWORD_ACC)
#define PCIE_CFG_REG_DMA_DS_CTRL            (27 << WB_DWORD_ACC)
#define PCIE_CFG_REG_DMA_DS_STA             (28 << WB_DWORD_ACC)

/* First register of the block*/
#define PCIE_CFG_REG_DMA_DS_BASE            PCIE_CFG_REG_DMA_DS_PAH

/* Offset DMA independent registers */
#define PCIE_CFG_REG_DMA_PAH                (0 << WB_DWORD_ACC)
#define PCIE_CFG_REG_DMA_PAL                (1 << WB_DWORD_ACC)
#define PCIE_CFG_REG_DMA_HAH                (2 << WB_DWORD_ACC)
#define PCIE_CFG_REG_DMA_HAL                (3 << WB_DWORD_ACC)
#define PCIE_CFG_REG_DMA_BDAH               (4 << WB_DWORD_ACC)
#define PCIE_CFG_REG_DMA_BDAL               (5 << WB_DWORD_ACC)
#define PCIE_CFG_REG_DMA_LENG               (6 << WB_DWORD_ACC)
#define PCIE_CFG_REG_DMA_CTRL               (7 << WB_DWORD_ACC)
#define PCIE_CFG_REG_DMA_STA                (8 << WB_DWORD_ACC)

/* Bit definitions for PAH/PAL register for both US/DS DMA engines */

#define PCIE_CFG_DMA_PAH_SIZE               32
#define PCIE_CFG_DMA_PAH_SHIFT              32
#define PCIE_CFG_DMA_PAH_MASK               WBGEN2_PCIE_GEN_MASK(PCIE_CFG_DMA_PAH_SHIFT, \
                                                        PCIE_CFG_DMA_PAH_SIZE)
#define PCIE_CFG_DMA_PAH_W(value)           WBGEN2_PCIE_GEN_WRITE(value, PCIE_CFG_DMA_PAH_SHIFT, \
                                                        PCIE_CFG_DMA_PAH_SIZE)
#define PCIE_CFG_DMA_PAH_R(reg)             WBGEN2_PCIE_GEN_READ(reg, PCIE_CFG_DMA_PAH_SHIFT, \
                                                             PCIE_CFG_DMA_PAH_SIZE)

#define PCIE_CFG_DMA_PAL_SIZE               32
#define PCIE_CFG_DMA_PAL_SHIFT              0
#define PCIE_CFG_DMA_PAL_MASK               WBGEN2_PCIE_GEN_MASK(PCIE_CFG_DMA_PAL_SHIFT, \
                                                        PCIE_CFG_DMA_PAL_SIZE)
#define PCIE_CFG_DMA_PAL_W(value)           WBGEN2_PCIE_GEN_WRITE(value, PCIE_CFG_DMA_PAL_SHIFT, \
                                                        PCIE_CFG_DMA_PAL_SIZE)
#define PCIE_CFG_DMA_PAL_R(reg)             WBGEN2_PCIE_GEN_READ(reg, PCIE_CFG_DMA_PAL_SHIFT, \
                                                             PCIE_CFG_DMA_PAL_SIZE)

/* Bit definitions for HAH/HAL register for both US/DS DMA engines */

#define PCIE_CFG_DMA_HAH_SIZE               32
#define PCIE_CFG_DMA_HAH_SHIFT              32
#define PCIE_CFG_DMA_HAH_MASK               WBGEN2_PCIE_GEN_MASK(PCIE_CFG_DMA_HAH_SHIFT, \
                                                        PCIE_CFG_DMA_HAH_SIZE)
#define PCIE_CFG_DMA_HAH_W(value)           WBGEN2_PCIE_GEN_WRITE(value, PCIE_CFG_DMA_HAH_SHIFT, \
                                                        PCIE_CFG_DMA_HAH_SIZE)
#define PCIE_CFG_DMA_HAH_R(reg)             WBGEN2_PCIE_GEN_READ(reg, PCIE_CFG_DMA_HAH_SHIFT, \
                                                             PCIE_CFG_DMA_HAH_SIZE)

#define PCIE_CFG_DMA_HAL_SIZE               32
#define PCIE_CFG_DMA_HAL_SHIFT              0
#define PCIE_CFG_DMA_HAL_MASK               WBGEN2_PCIE_GEN_MASK(PCIE_CFG_DMA_HAL_SHIFT, \
                                                        PCIE_CFG_DMA_HAL_SIZE)
#define PCIE_CFG_DMA_HAL_W(value)           WBGEN2_PCIE_GEN_WRITE(value, PCIE_CFG_DMA_HAL_SHIFT, \
                                                        PCIE_CFG_DMA_HAL_SIZE)
#define PCIE_CFG_DMA_HAL_R(reg)             WBGEN2_PCIE_GEN_READ(reg, PCIE_CFG_DMA_HAL_SHIFT, \
                                                             PCIE_CFG_DMA_HAL_SIZE)

/* Bit definitions for BDAH/BDAL register for both US/DS DMA engines */

#define PCIE_CFG_DMA_BDAH_SIZE              32
#define PCIE_CFG_DMA_BDAH_SHIFT             32
#define PCIE_CFG_DMA_BDAH_MASK              WBGEN2_PCIE_GEN_MASK(PCIE_CFG_DMA_BDAH_SHIFT, \
                                                       PCIE_CFG_DMA_BDAH_SIZE)
#define PCIE_CFG_DMA_BDAH_W(value)          WBGEN2_PCIE_GEN_WRITE(value, PCIE_CFG_DMA_BDAH_SHIFT, \
                                                       PCIE_CFG_DMA_BDAH_SIZE)
#define PCIE_CFG_DMA_BDAH_R(reg)            WBGEN2_PCIE_GEN_READ(reg, PCIE_CFG_DMA_BDAH_SHIFT, \
                                                            PCIE_CFG_DMA_BDAH_SIZE)

#define PCIE_CFG_DMA_BDAL_SIZE              32
#define PCIE_CFG_DMA_BDAL_SHIFT             0
#define PCIE_CFG_DMA_BDAL_MASK              WBGEN2_PCIE_GEN_MASK(PCIE_CFG_DMA_BDAL_SHIFT, \
                                                       PCIE_CFG_DMA_BDAL_SIZE)
#define PCIE_CFG_DMA_BDAL_W(value)          WBGEN2_PCIE_GEN_WRITE(value, PCIE_CFG_DMA_BDAL_SHIFT, \
                                                       PCIE_CFG_DMA_BDAL_SIZE)
#define PCIE_CFG_DMA_BDAL_R(reg)            WBGEN2_PCIE_GEN_READ(reg, PCIE_CFG_DMA_BDAL_SHIFT, \
                                                            PCIE_CFG_DMA_BDAL_SIZE)

/* Bit definitions for LENG register for both US/DS DMA engines */

#define PCIE_CFG_DMA_LENG_SIZE              32
#define PCIE_CFG_DMA_LENG_SHIFT             0
#define PCIE_CFG_DMA_LENG_MASK              WBGEN2_PCIE_GEN_MASK(PCIE_CFG_DMA_LENG_SHIFT, \
                                                       PCIE_CFG_DMA_LENG_SIZE)
#define PCIE_CFG_DMA_LENG_W(value)          WBGEN2_PCIE_GEN_WRITE(value, PCIE_CFG_DMA_LENG_SHIFT, \
                                                       PCIE_CFG_DMA_LENG_SIZE)
#define PCIE_CFG_DMA_LENG_R(reg)            WBGEN2_PCIE_GEN_READ(reg, PCIE_CFG_DMA_LENG_SHIFT, \
                                                            PCIE_CFG_DMA_LENG_SIZE)

/* Bit definitions for CTRL register for both US/DS DMA engines */

#define PCIE_CFG_DMA_CTRL_END_SIZE          1
#define PCIE_CFG_DMA_CTRL_END_SHIFT         8
#define PCIE_CFG_DMA_CTRL_END               WBGEN2_PCIE_GEN_MASK(PCIE_CFG_DMA_CTRL_END_SHIFT, \
                                                    PCIE_CFG_DMA_CTRL_END_SIZE)

#define PCIE_CFG_DMA_CTRL_AINC_SIZE         1
#define PCIE_CFG_DMA_CTRL_AINC_SHIFT        15
#define PCIE_CFG_DMA_CTRL_AINC              WBGEN2_PCIE_GEN_MASK(PCIE_CFG_DMA_CTRL_AINC_SHIFT, \
                                                   PCIE_CFG_DMA_CTRL_AINC_SIZE)

#define PCIE_CFG_DMA_CTRL_BAR_SIZE          3
#define PCIE_CFG_DMA_CTRL_BAR_SHIFT         16
#define PCIE_CFG_DMA_CTRL_BAR_MASK          WBGEN2_PCIE_GEN_MASK(PCIE_CFG_DMA_CTRL_BAR_SHIFT, \
                                                   PCIE_CFG_DMA_CTRL_BAR_SIZE)
#define PCIE_CFG_DMA_CTRL_BAR_W(value)      WBGEN2_PCIE_GEN_WRITE(value, PCIE_CFG_DMA_CTRL_BAR_SHIFT, \
                                                   PCIE_CFG_DMA_CTRL_BAR_SIZE)
#define PCIE_CFG_DMA_CTRL_BAR_R(reg)        WBGEN2_PCIE_GEN_READ(reg, PCIE_CFG_DMA_CTRL_BAR_SHIFT, \
                                                        PCIE_CFG_DMA_CTRL_BAR_SIZE)

#define PCIE_CFG_DMA_CTRL_UPA_SIZE          1
#define PCIE_CFG_DMA_CTRL_UPA_SHIFT         20
#define PCIE_CFG_DMA_CTRL_UPA               WBGEN2_PCIE_GEN_MASK(PCIE_CFG_DMA_CTRL_UPA_SHIFT, \
                                                    PCIE_CFG_DMA_CTRL_UPA_SIZE)

#define PCIE_CFG_DMA_CTRL_LAST_SIZE         1
#define PCIE_CFG_DMA_CTRL_LAST_SHIFT        24
#define PCIE_CFG_DMA_CTRL_LAST              WBGEN2_PCIE_GEN_MASK(PCIE_CFG_DMA_CTRL_LAST_SHIFT, \
                                                   PCIE_CFG_DMA_CTRL_LAST_SIZE)

#define PCIE_CFG_DMA_CTRL_VALID_SIZE        1
#define PCIE_CFG_DMA_CTRL_VALID_SHIFT       25
#define PCIE_CFG_DMA_CTRL_VALID             WBGEN2_PCIE_GEN_MASK(PCIE_CFG_DMA_CTRL_VALID_SHIFT, \
                                                  PCIE_CFG_DMA_CTRL_VALID_SIZE)

/* Bit definitions for STATUS (STA) register for both US/DS DMA engines */
#define PCIE_CFG_DMA_STA_DONE_SIZE          1
#define PCIE_CFG_DMA_STA_DONE_SHIFT         0
#define PCIE_CFG_DMA_STA_DONE               WBGEN2_PCIE_GEN_MASK(PCIE_CFG_DMA_STA_DONE_SHIFT, \
                                                    PCIE_CFG_DMA_STA_DONE_SIZE)

#define PCIE_CFG_DMA_STA_BUSY_SIZE          1
#define PCIE_CFG_DMA_STA_BUSY_SHIFT         1
#define PCIE_CFG_DMA_STA_BUSY               WBGEN2_PCIE_GEN_MASK(PCIE_CFG_DMA_STA_BUSY_SHIFT, \
                                                    PCIE_CFG_DMA_STA_BUSY_SIZE)

#define PCIE_CFG_DMA_STA_BDANULL_SIZE       1
#define PCIE_CFG_DMA_STA_BDANULL_SHIFT      3
#define PCIE_CFG_DMA_STA_BDANULL            WBGEN2_PCIE_GEN_MASK(PCIE_CFG_DMA_STA_BDANULL_SHIFT, \
                                                  PCIE_CFG_DMA_STA_BDANULL_SIZE)

#define PCIE_CFG_DMA_STA_TIMEOUT_SIZE       1
#define PCIE_CFG_DMA_STA_TIMEOUT_SHIFT      4
#define PCIE_CFG_DMA_STA_TIMEOUT            WBGEN2_PCIE_GEN_MASK(PCIE_CFG_DMA_STA_TIMEOUT_SHIFT, \
                                                 PCIE_CFG_DMA_STA_TIMEOUT_SIZE)

#define PCIE_CFG_DMA_STA_NALIGN_SIZE        1
#define PCIE_CFG_DMA_STA_NALIGN_SHIFT       7
#define PCIE_CFG_DMA_STA_NALIGN             WBGEN2_PCIE_GEN_MASK(PCIE_CFG_DMA_STA_NALIGN_SHIFT, \
                                                  PCIE_CFG_DMA_STA_NALIGN_SIZE)

/* Address for MRd channel control */
#define PCIE_CFG_REG_MRD_CTRL               (29 << WB_DWORD_ACC)
/* Address for Tx module control */
#define PCIE_CFG_REG_TX_CTRL                (30 << WB_DWORD_ACC)

#define PCIE_CFG_TX_CTRL_CHANNEL_RST        0x0A

/* Address of Interrupt Generator Control (W) */
#define PCIE_CFG_REG_IG_CONTROL             (32 << WB_DWORD_ACC)
/* Address of Interrupt Generator Latency (W+R) */
#define PCIE_CFG_REG_IG_LATENCY             (33 << WB_DWORD_ACC)
/* Address of Interrupt Generator Assert Number (R) */
#define PCIE_CFG_REG_IG_NUM_ASSERT          (34 << WB_DWORD_ACC)
/* Address of Interrupt Generator Deassert Number (R) */
#define PCIE_CFG_REG_IG_NUM_DEASSERT        (35 << WB_DWORD_ACC)
/* Event Buffer FIFO status (R) + control (W) */
#define PCIE_CFG_REG_EB_STACON              (36 << WB_DWORD_ACC)

/* Upstream DMA transferred byte count (R) */
#define PCIE_CFG_REG_US_TRANSF_BC           (37 << WB_DWORD_ACC)
/* Downstream DMA transferred byte count (R) */
#define PCIE_CFG_REG_DS_TRANSF_BC           (38 << WB_DWORD_ACC)

/* Total number of registers */
#define PCIE_CFG_REG_NUM_OF_ADDRESSES       39

/************************************************************/
/********** PCIe Address MSB (BARs) part extractor **********/
/************************************************************/

/********** BAR numbers **********/
#define BAR0NO                              0ULL
#define BAR2NO                              2ULL
#define BAR4NO                              4ULL

/* For the PCIe device, we only have up to 60 address bits, as
 * the 4 MSB are reserved for selecting the BAR to operate on */
/********** PCIe Address MSB (BARs) part extractor **********/
#define PCIE_ADDR_BAR_SHIFT                 60          /* bits */
#define PCIE_ADDR_BAR_MAX                   4           /* bits */
#define PCIE_ADDR_BAR_MASK                  (((1ULL << PCIE_ADDR_BAR_MAX)-1) << \
                                                PCIE_ADDR_BAR_SHIFT)

/* PCIe LSB (generic) part extractor */
#define PCIE_ADDR_BAR(addr)                 ((addr & PCIE_ADDR_BAR_MASK) >> \
                                                PCIE_ADDR_BAR_SHIFT)

/* For use by LL_IO */
/* We divide the PCIe Wishbone addresses as:
 * bits 64 to 60: BAR address (0, 2 or 4)
 * bits 59 to 0: Wishbone physical address (2^59-1 to 0)*/
#define BAR4_ADDR                           (BAR4NO << PCIE_ADDR_BAR_SHIFT)
#define BAR2_ADDR                           (BAR2NO << PCIE_ADDR_BAR_SHIFT)
#define BAR0_ADDR                           (BAR0NO << PCIE_ADDR_BAR_SHIFT)

/********** PCIe Address LSB (generic) part extractor **********/
#define PCIE_ADDR_GEN_SHIFT                 0           /* bits */
#define PCIE_ADDR_GEN_MAX                   60          /* bits */
#define PCIE_ADDR_GEN_MASK                  (((1ULL << PCIE_ADDR_GEN_MAX)-1) << \
                                                PCIE_ADDR_GEN_SHIFT)

/* PCIe LSB (generic) part extractor */
#define PCIE_ADDR_GEN(addr)                 ((addr & PCIE_ADDR_GEN_MASK) >> \
                                                PCIE_ADDR_GEN_SHIFT)

/* PCIe LSB (generic) part, for use by LL_IO */
#define PCIE_ADDR_WB(addr)                  ((addr & PCIE_ADDR_GEN_MASK) << \
                                                PCIE_ADDR_GEN_SHIFT)

/********** PCIe SDRAM Address Page number and offset extractor **********/
#define PCIE_ADDR_SDRAM_PG_OFFS(addr)       ((addr & PCIE_SDRAM_PG_MASK) >> \
                                                PCIE_SDRAM_PG_SHIFT)
#define PCIE_ADDR_SDRAM_PG(addr)            ((addr & ~PCIE_SDRAM_PG_MASK) >> \
                                                PCIE_SDRAM_PG_MAX)

/********** PCIe WB Address Page number and offset extractor **********/
#define PCIE_ADDR_WB_PG_OFFS(addr)          ((addr & PCIE_WB_PG_MASK) >> \
                                                PCIE_WB_PG_SHIFT)
#define PCIE_ADDR_WB_PG(addr)               ((addr & ~PCIE_WB_PG_MASK) >> \
                                                PCIE_WB_PG_MAX)

/*************************** PCIe BAR RW type **************************/
#define BAR_RW_TYPE                         uint32_t

/********** Read or write to BAR **********/
#define BAR_RW_8(barp, addr, datap, rw)                             \
    do {                                                            \
        (rw) ?                                                      \
        (*(datap) = *(BAR_RW_TYPE *)(((uint8_t *)barp) + (addr))) : \
        (*(BAR_RW_TYPE *)(((uint8_t *)barp) + (addr)) = *(datap));  \
    } while (0)

/* BAR0 is BYTE addressed for the user */
#define BAR0_RW(barp, addr, datap, rw)                              \
    BAR_RW_8(barp, addr, datap, rw)

/* BAR2 is BYTE addressed for the user */
#define BAR2_RW(barp, addr, datap, rw)                              \
    BAR_RW_8(barp, addr, datap, rw)

/* BAR4 is BYTE addresses for the user */
/* On PCIe Core FPGA firmware the wishbone address is provided with
 * only 29 bits, with the LSB zeroed:
 *
 *  bit 31        . . .      bit 3   bit 2   bit 1   bit 0
 *   A31          . . .        A3     '0'     '0'    '0'
 *
 * This is done as the BAR4 is 64-bit addressed. But, the output of the
 * PCIe wrapper are right shifted to avoid dealking with this particularity:
 *
 *  bit 31   bit 30   bit 29  bit 28    . . .      bit 3   bit 2   bit 1   bit 0
 *   '0'      '0'     '0'       A31     . . .        A6     A5      A4      A3
 *
 * */
#define BAR4_RW(barp, addr, datap, rw)                              \
    do {                                                            \
        (rw) ?                                                      \
        (*(datap) = *((barp) + (addr))) :                           \
        (*((barp) + (addr)) = *(datap));                            \
    } while (0)

/********** Read or write block to BAR **********/

#define BAR_RW_8_BLOCK(barp, addr, size, datap, rw)                 \
    do {                                                            \
        if (rw) {                                                   \
            for (uint32_t j = 0; j < size/sizeof (BAR_RW_TYPE); ++j) { \
                *((datap) + j) = *(BAR_RW_TYPE *)(((uint8_t *)barp) + \
                        addr + (j << WB_DWORD_ACC));                \
            }                                                       \
        }                                                           \
        else {                                                      \
            for (uint32_t j = 0; j < size/sizeof (BAR_RW_TYPE); ++j) { \
                *(BAR_RW_TYPE *)(((uint8_t *)barp) +                \
                        addr + (j << WB_DWORD_ACC)) = *((datap) + j); \
            }                                                       \
        }                                                           \
    } while (0)

#define BAR0_RW_BLOCK(barp, addr, size, datap, rw)                  \
    BAR_RW_8_BLOCK(barp, addr, size, datap, rw)

#define BAR2_RW_BLOCK(barp, addr, size, datap, rw)                  \
    BAR_RW_8_BLOCK(barp, addr, size, datap, rw)

#define BAR4_RW_BLOCK(barp, addr, size, datap, rw)                  \
    do {                                                            \
        uint32_t j = 0;                                             \
        if (rw) {                                                   \
            for (j = 0; j < size/sizeof (BAR_RW_TYPE); ++j) {       \
                *((datap) + j) = *((barp) + addr + (j << WB_DWORD_ACC)); \
            }                                                       \
        }                                                           \
        else {                                                      \
            for (j = 0; j < size/sizeof (BAR_RW_TYPE); ++j) {       \
                *((barp) + addr + (j << WB_DWORD_ACC)) = *((datap) + j); \
                /* Read from memory to flush previous write */      \
                *((barp) + addr + (j << WB_DWORD_ACC));             \
            }                                                       \
        }                                                           \
        /* RW one last word if size not multiple of sizeof (barp)*/ \
        if (size % sizeof (BAR_RW_TYPE) != 0) {                     \
            BAR4_RW(barp, addr + (j << WB_DWORD_ACC), (datap) + j, rw); \
        }                                                           \
    } while (0)

/* Setting WB and SDRAM pages */
#define SET_PG(bar0, which, num)                \
    do {                                        \
        bar0[which >> WB_DWORD_ACC] =           \
                num;                            \
    } while (0)

#define SET_SDRAM_PG(bar0, num)                 \
    SET_PG(bar0, PCIE_CFG_REG_SDRAM_PG, num)

#define SET_WB_PG(bar0, num)                    \
    SET_PG(bar0, PCIE_CFG_REG_WB_PG, num)

/* Undef WBGEN macros as we they might have already been
 * set-up */
#ifdef __WBGEN2_MACROS_DEFINED__
#    undef __WBGEN2_MACROS_DEFINED__
#endif
#ifdef WBGEN2_GEN_MASK
#    undef WBGEN2_GEN_MASK
#endif
#ifdef WBGEN2_GEN_WRITE
#   undef WBGEN2_GEN_WRITE
#endif
#ifdef WBGEN2_GEN_READ
#   undef WBGEN2_GEN_READ
#endif
#ifdef WBGEN2_SIGN_EXTEND
#    undef WBGEN2_SIGN_EXTEND
#endif

/* define regular WBGEN macros as some modules rely on this */
#ifndef __WBGEN2_MACROS_DEFINED__
#define __WBGEN2_MACROS_DEFINED__
#define WBGEN2_GEN_MASK(offset, size) (((1<<(size))-1) << (offset))
#define WBGEN2_GEN_WRITE(value, offset, size) (((value) & ((1<<(size))-1)) << (offset))
#define WBGEN2_GEN_READ(reg, offset, size) (((reg) >> (offset)) & ((1<<(size))-1))
#define WBGEN2_SIGN_EXTEND(value, bits) (((value) & (1<<bits) ? ~((1<<(bits))-1): 0 ) | (value))
#endif

#endif
