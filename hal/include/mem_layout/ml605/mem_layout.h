#ifndef _MEM_LAYOUT_H_
#define _MEM_LAYOUT_H_

#include "pcie_regs.h"

/*********************** Static ML605 FPGA layout ***********************/

/* Should be autodiscovered by SDB */

/* Wishbone RAW Addresses */
#define FMC130M_BASE_RAW_ADDR                       0x00310000

#define FPGA_CTRL_RAW_REGS                          (FMC130M_BASE_RAW_ADDR + 0x0000)
#define FPGA_SI571_RAW_I2C                          (FMC130M_BASE_RAW_ADDR + 0x0100)
#define FPGA_AD9510_RAW_SPI                         (FMC130M_BASE_RAW_ADDR + 0x0200)
#define FPGA_EEPROM_RAW_I2C                         (FMC130M_BASE_RAW_ADDR + 0x0300)
#define FPGA_LM75A_RAW_I2C                          (FMC130M_BASE_RAW_ADDR + 0x0400)

#define DSP_BASE_RAW_ADDR                           0x00308000

#define DSP_CTRL_RAW_REGS                           (DSP_BASE_RAW_ADDR + 0x00000000)
#define DSP_BPM_RAW_SWAP                            (DSP_BASE_RAW_ADDR + 0x00000100)

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

/* Wishbone Addresses */
#define FMC130M_BASE_ADDR                           (BAR4_ADDR | FMC130M_BASE_RAW_ADDR)

#define FPGA_CTRL_REGS                              (BAR4_ADDR | FPGA_CTRL_RAW_REGS)
#define FPGA_SI571_I2C                              (BAR4_ADDR | FPGA_SI571_RAW_I2C)
#define FPGA_AD9510_SPI                             (BAR4_ADDR | FPGA_AD9510_RAW_SPI)
#define FPGA_EEPROM_I2C                             (BAR4_ADDR | FPGA_EEPROM_RAW_I2C)
#define FPGA_LM75A_I2C                              (BAR4_ADDR | FPGA_LM75A_RAW_I2C)

#define DSP_BASE_ADDR                               (BAR4_ADDR | DSP_BASE_RAW_ADDR)

#define DSP_CTRL_REGS                               (BAR4_ADDR | DSP_CTRL_RAW_REGS)
#define DSP_BPM_SWAP                                (BAR4_ADDR | DSP_BPM_RAW_SWAP)

#define WB_ACQ_BASE_ADDR                            (BAR4_ADDR | WB_ACQ_BASE_RAW_ADDR)

/* Large Memory Addresses */
#define LARGE_MEM_ADDR                              (BAR2_ADDR | LARGE_MEM_RAW_ADDR)

#endif

