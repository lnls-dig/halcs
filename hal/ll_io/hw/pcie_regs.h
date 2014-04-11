/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _PCIE_REGS_H_
#define _PCIE_REGS_H_

/* Some FPGA PCIe registers. These are inside bar0 */
#define PCIE_CFG_REG_SDRAM_PG               0x1C
#define PCIE_CFG_REG_GSR                    0x20
#define PCIE_CFG_GSR_BIT_DDR_RDY            (0x1 << 7)
#define PCIE_CFG_REG_WB_PG                  0x24

/* Some FPGA PCIe constants */
/* SDRAM is accesses via 32-bit BAR (32-bit addressing) */
#define PCIE_SDRAM_PG_SHIFT                 0           /* bits */
/* #define PCIE_SDRAM_PG_MAX                   19 */          /* bits */
#define PCIE_SDRAM_PG_MAX                   17          /* bits */
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
/********** PCIe Address MSB (BARs) part extractor **********/
/************************************************************/

/********** BAR numbers **********/
#define BAR0NO                              0
#define BAR2NO                              2
#define BAR4NO                              4

/* For the PCIe device, we only have up to 28 address bits, as
 * the 4 MSB are reserved for selecting the BAR to operate on */
/********** PCIe Address MSB (BARs) part extractor **********/
#define PCIE_ADDR_BAR_SHIFT                 28          /* bits */
#define PCIE_ADDR_BAR_MAX                   4           /* bits */
#define PCIE_ADDR_BAR_MASK                  (((1 << PCIE_ADDR_BAR_MAX)-1) << \
                                                PCIE_ADDR_BAR_SHIFT)

/* PCIe LSB (generic) part extractor */
#define PCIE_ADDR_BAR(addr)                 ((addr & PCIE_ADDR_BAR_MASK) >> \
                                                PCIE_ADDR_BAR_SHIFT)

/* For use by LL_IO */
/* We divide the PCIe Wishbone addresses as:
 * bits 32 to 28: BAR address (0, 2 or 4)
 * bits 27 to 0: Wishbone physical address (2^28-1 to 0)*/
#define BAR4_ADDR                       (BAR4NO << PCIE_ADDR_BAR_SHIFT)
#define BAR2_ADDR                       (BAR2NO << PCIE_ADDR_BAR_SHIFT)
#define BAR0_ADDR                       (BAR0NO << PCIE_ADDR_BAR_SHIFT)

/********** PCIe Address LSB (generic) part extractor **********/
#define PCIE_ADDR_GEN_SHIFT                 0           /* bits */
#define PCIE_ADDR_GEN_MAX                   28          /* bits */
#define PCIE_ADDR_GEN_MASK                  (((1 << PCIE_ADDR_GEN_MAX)-1) << \
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

/********** Read or write to BAR **********/
#define BAR32_RW(barp, addr, datap, rw)                             \
    do {                                                            \
        (rw) ?                                                      \
        (*(datap) = *((barp) + (addr))) :                           \
        (*((barp) + (addr)) = *(datap));                            \
    } while (0)

/* FIXME: Figure it out another convenient way of doing this without hiding code! */
/* Macros for ease the PCIe BAR pointer verbosity. */
#define BAR_ACCESS_GEN(bar_addr)        (((llio_dev_pcie_t *)self->dev_handler)->bar_addr)
#define BAR0                            BAR_ACCESS_GEN(bar0)
#define BAR2                            BAR_ACCESS_GEN(bar2)
#define BAR4                            BAR_ACCESS_GEN(bar4)

/* Setting WB and SDRAM pages */
#define SET_PG(which, num)              \
    do {                                \
        BAR0[which >> WB_DWORD_ACC] =   \
                num;                    \
    } while (0)

#define SET_SDRAM_PG(num)               \
    SET_PG(PCIE_CFG_REG_SDRAM_PG, num)

#define SET_WB_PG(num)                  \
    SET_PG(PCIE_CFG_REG_WB_PG, num)

#endif
