#ifndef _MEM_LAYOUT_H_
#define _MEM_LAYOUT_H_

#include "pcie_regs.h"

/*********************** Static AFCv3 FPGA layout ***********************/
/* FMC_130M Components */
#define FMC_130M_CTRL_RAW_REGS_OFFS                 0x0000
#define FMC_130M_SI571_RAW_I2C_OFFS                 0x0100
#define FMC_130M_AD9510_RAW_SPI_OFFS                0x0200
#define FMC_130M_EEPROM_RAW_I2C_OFFS                0x0300
#define FMC_130M_LM75A_RAW_I2C_OFFS                 0x0400

/* DSP Components */
#define DSP_CTRL_RAW_REGS_OFFS                      0x0000
#define DSP_BPM_RAW_SWAP_OFFS                       0x0100

/* Should be autodiscovered by SDB */

/* Wishbone RAW Addresses */
#define FMC1_130M_BASE_RAW_ADDR                     0x00310000

#define FMC1_130M_CTRL_RAW_REGS                     (FMC1_130M_BASE_RAW_ADDR +  \
                                                        FMC_130M_CTRL_RAW_REGS_OFFS)
#define FMC1_130M_SI571_RAW_I2C                     (FMC1_130M_BASE_RAW_ADDR +  \
                                                        FMC_130M_SI571_RAW_I2C_OFFS)
#define FMC1_130M_AD9510_RAW_SPI                    (FMC1_130M_BASE_RAW_ADDR + \
                                                        FMC_130M_AD9510_RAW_SPI_OFFS)
#define FMC1_130M_EEPROM_RAW_I2C                    (FMC1_130M_BASE_RAW_ADDR + \
                                                        FMC_130M_EEPROM_RAW_I2C_OFFS)
#define FMC1_130M_LM75A_RAW_I2C                     (FMC1_130M_BASE_RAW_ADDR + \
                                                        FMC_130M_LM75A_RAW_I2C_OFFS)

#define DSP1_BASE_RAW_ADDR                          0x00308000

#define DSP1_CTRL_RAW_REGS                          (DSP1_BASE_RAW_ADDR +       \
                                                        DSP_CTRL_RAW_REGS_OFFS)
#define DSP1_BPM_RAW_SWAP                           (DSP1_BASE_RAW_ADDR +       \
                                                        DSP_BPM_RAW_SWAP_OFFS)

#define FMC2_130M_BASE_RAW_ADDR                     0x00350000

#define FMC2_130M_CTRL_RAW_REGS                     (FMC2_130M_BASE_RAW_ADDR +  \
                                                        FMC_130M_CTRL_RAW_REGS_OFFS)
#define FMC2_130M_SI571_RAW_I2C                     (FMC2_130M_BASE_RAW_ADDR +  \
                                                        FMC_130M_SI571_RAW_I2C_OFFS)
#define FMC2_130M_AD9510_RAW_SPI                    (FMC2_130M_BASE_RAW_ADDR + \
                                                        FMC_130M_AD9510_RAW_SPI_OFFS)
#define FMC2_130M_EEPROM_RAW_I2C                    (FMC2_130M_BASE_RAW_ADDR + \
                                                        FMC_130M_EEPROM_RAW_I2C_OFFS)
#define FMC2_130M_LM75A_RAW_I2C                     (FMC2_130M_BASE_RAW_ADDR + \
                                                        FMC_130M_LM75A_RAW_I2C_OFFS)

#define DSP2_BASE_RAW_ADDR                          0x00340000

#define DSP2_CTRL_RAW_REGS                          (DSP2_BASE_RAW_ADDR +       \
                                                        DSP_CTRL_RAW_REGS_OFFS)
#define DSP2_BPM_RAW_SWAP                           (DSP2_BASE_RAW_ADDR +       \
                                                        DSP_BPM_RAW_SWAP_OFFS)

#define WB_ACQ_BASE_RAW_ADDR                        0x00330000

/* Large Memory RAW Addresses. It lives at address 0 */
#define LARGE_MEM_RAW_ADDR                          0x00000000

/* The following is a bit of a hack.
 * We employ a generic API for talking to the hardware.
 * So, our transport layer (PCIe or Ethernet, for now)
 * should be invisible to the SMIO instances.
 *
 * However, PCI devices generally employ multiple BAR
 * registers mapped to different parts of the device.
 * For instance, in the bpm-gw FPGA firmware, the PCIe
 * core has 3 BARs (BAR0, BAR2 and BAR4) mapped to the
 * following:
 *
 * BAR0 -> PCIe control registers
 * BAR2 -> DDR3 SDRAM
 * BAR4 -> Wishbone (necessary to use pages mechanism)
 *
 * So, we define our addresses as the logic address plus
 * the BAR number. With this, the PCIe transport layer
 * can differentiate between multiple bars and select
 * the correct one to read or write
 */

/* FMC_130M Components */
#define FMC_130M_CTRL_REGS_OFFS                     (BAR4_ADDR | FMC_130M_CTRL_RAW_REGS_OFFS)
#define FMC_130M_SI571_I2C_OFFS                     (BAR4_ADDR | FMC_130M_SI571_I2C_RAW_OFFS)
#define FMC_130M_AD9510_SPI_OFFS                    (BAR4_ADDR | FMC_130M_AD9510_RAW_SPI_OFFS)
#define FMC_130M_EEPROM_I2C_OFFS                    (BAR4_ADDR | FMC_130M_EEPROM_RAW_I2C_OFFS)
#define FMC_130M_LM75A_I2C_OFFS                     (BAR4_ADDR | FMC_130M_LM75A_RAW_I2C_OFFS)

/* DSP Components */
#define DSP_CTRL_REGS_OFFS                          (BAR4_ADDR | DSP_CTRL_RAW_REGS_OFFS)
#define DSP_BPM_SWAP_OFFS                           (BAR4_ADDR | DSP_BPM_RAW_SWAP_OFFS)

/* Wishbone Addresses */
#define FMC1_130M_BASE_ADDR                         (BAR4_ADDR | FMC1_130M_BASE_RAW_ADDR)

#define FMC1_130M_CTRL_REGS                         (BAR4_ADDR | FMC1_130M_CTRL_RAW_REGS)
#define FMC1_130M_SI571_I2C                         (BAR4_ADDR | FMC1_130M_SI571_RAW_I2C)
#define FMC1_130M_AD9510_SPI                        (BAR4_ADDR | FMC1_130M_AD9510_RAW_SPI)
#define FMC1_130M_EEPROM_I2C                        (BAR4_ADDR | FMC1_130M_EEPROM_RAW_I2C)
#define FMC1_130M_LM75A_I2C                         (BAR4_ADDR | FMC1_130M_LM75A_RAW_I2C)

#define DSP1_BASE_ADDR                              (BAR4_ADDR | DSP1_BASE_RAW_ADDR)

#define DSP1_CTRL_REGS                              (BAR4_ADDR | DSP1_CTRL_RAW_REGS)
#define DSP1_BPM_SWAP                               (BAR4_ADDR | DSP1_BPM_RAW_SWAP)

#define WB_ACQ_BASE_ADDR                            (BAR4_ADDR | WB_ACQ_BASE_RAW_ADDR)

#define FMC2_130M_BASE_ADDR                         (BAR4_ADDR | FMC2_130M_BASE_RAW_ADDR)

#define FMC2_130M_CTRL_REGS                         (BAR4_ADDR | FMC2_130M_CTRL_RAW_REGS)
#define FMC2_130M_SI571_I2C                         (BAR4_ADDR | FMC2_130M_SI571_RAW_I2C)
#define FMC2_130M_AD9510_SPI                        (BAR4_ADDR | FMC2_130M_AD9510_RAW_SPI)
#define FMC2_130M_EEPROM_I2C                        (BAR4_ADDR | FMC2_130M_EEPROM_RAW_I2C)
#define FMC2_130M_LM75A_I2C                         (BAR4_ADDR | FMC2_130M_LM75A_RAW_I2C)

#define DSP2_BASE_ADDR                              (BAR4_ADDR | DSP2_BASE_RAW_ADDR)

#define DSP2_CTRL_REGS                              (BAR4_ADDR | DSP2_CTRL_RAW_REGS)
#define DSP2_BPM_SWAP                               (BAR4_ADDR | DSP2_BPM_RAW_SWAP)

/* Large Memory Addresses */
#define LARGE_MEM_ADDR                              (BAR2_ADDR | LARGE_MEM_RAW_ADDR)

/************************* ML605 Gateware Options *************************/


/*********************** SMIO ACQ Gateware Options ***********************/
/* FIXME: The Gateware does not support yet a way of reading which
 * ACQ channels are valid. So, we hardcoded it here for now. This issue
 * should be addressed soon */
#define SMIO_ACQ_NUM_CHANNELS                       5

#endif

