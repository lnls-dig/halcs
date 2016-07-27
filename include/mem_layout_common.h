#ifndef _MEM_LAYOUT_COMMON_
#define _MEM_LAYOUT_COMMON_

#ifdef __cplusplus
extern "C" {
#endif

#include "pcie_regs.h"

/* FMC_ACTIVE_CLK Component */
#define FMC_ACTIVE_CLK_CTRL_RAW_REGS_OFFS           0x0000
#define FMC_ACTIVE_CLK_SI571_RAW_I2C_OFFS           0x0100
#define FMC_ACTIVE_CLK_AD9510_RAW_SPI_OFFS          0x0200

/* FMC_130M Components */
#define FMC_130M_CTRL_RAW_REGS_OFFS                 0x00000
#define FMC_130M_FMC_ADC_COMMON_RAW_REGS_OFFS       0x01000
#define FMC_130M_FMC_ACTIVE_CLK_RAW_OFFS            0x02000
#define FMC_130M_EEPROM_RAW_I2C_OFFS                0x03000
#define FMC_130M_LM75A_RAW_I2C_OFFS                 0x04000

/* FMC_250M Components */
#define FMC_250M_CTRL_RAW_REGS_OFFS                 0x00000
#define FMC_250M_FMC_ADC_COMMON_RAW_REGS_OFFS       0x01000
#define FMC_250M_FMC_ACTIVE_CLK_RAW_OFFS            0x02000
#define FMC_250M_EEPROM_RAW_I2C_OFFS                0x03000
#define FMC_250M_AMC7823_RAW_SPI_OFFS               0x04000
#define FMC_250M_ISLA216P_RAW_SPI_OFFS              0x05000

/* DSP Components */
#define DSP_CTRL_RAW_REGS_OFFS                      0x0000
#define DSP_BPM_RAW_SWAP_OFFS                       0x0100

/* AFC DIAG Components */
#define WB_AFC_DIAG_CTRL_RAW_REGS_OFFS              0x0000

/* ACQ Components */
#define WB_ACQ_CORE_CTRL_RAW_REGS_OFFS              0x0000

/* Trigger Interface Components */
#define WB_TRIGGER_IFACE_RAW_REG_OFFS               0x0000

/* Trigger Mux Components */
#define WB_TRIGGER_MUX_RAW_REG_OFFS                 0x0000

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

/* FMC_ACTIVE_CLK Component */
#define FMC_ACTIVE_CLK_CTRL_REGS_OFFS               (BAR4_ADDR | FMC_ACTIVE_CLK_CTRL_RAW_REGS_OFFS)
#define FMC_ACTIVE_CLK_SI571_I2C_OFFS               (BAR4_ADDR | FMC_ACTIVE_CLK_SI571_RAW_I2C_OFFS)
#define FMC_ACTIVE_CLK_AD9510_SPI_OFFS              (BAR4_ADDR | FMC_ACTIVE_CLK_AD9510_RAW_SPI_OFFS)

/* FMC_130M Components */
#define FMC_130M_CTRL_REGS_OFFS                     (BAR4_ADDR | FMC_130M_CTRL_RAW_REGS_OFFS)
#define FMC_130M_FMC_ADC_COMMON_OFFS                (BAR4_ADDR | FMC_130M_FMC_ADC_COMMON_RAW_REGS_OFFS)
#define FMC_130M_FMC_ACTIVE_CLK_OFFS                (BAR4_ADDR | FMC_130M_FMC_ACTIVE_CLK_RAW_OFFS)
#define FMC_130M_EEPROM_I2C_OFFS                    (BAR4_ADDR | FMC_130M_EEPROM_RAW_I2C_OFFS)
#define FMC_130M_LM75A_I2C_OFFS                     (BAR4_ADDR | FMC_130M_LM75A_RAW_I2C_OFFS)

/* FMC_250M Components */
#define FMC_250M_CTRL_REGS_OFFS                     (BAR4_ADDR | FMC_250M_CTRL_RAW_REGS_OFFS)
#define FMC_250M_FMC_ADC_COMMON_OFFS                (BAR4_ADDR | FMC_250M_FMC_ADC_COMMON_RAW_REGS_OFFS)
#define FMC_250M_FMC_ACTIVE_CLK_OFFS                (BAR4_ADDR | FMC_250M_FMC_ACTIVE_CLK_RAW_OFFS)
#define FMC_250M_EEPROM_I2C_OFFS                    (BAR4_ADDR | FMC_250M_EEPROM_RAW_I2C_OFFS)
#define FMC_250M_AMC7823_SPI_OFFS                   (BAR4_ADDR | FMC_250M_AMC7823_RAW_SPI_OFFS)
#define FMC_250M_ISLA216P_SPI_OFFS                  (BAR4_ADDR | FMC_250M_ISLA216P_RAW_SPI_OFFS)

/* DSP Components */
#define DSP_CTRL_REGS_OFFS                          (BAR4_ADDR | DSP_CTRL_RAW_REGS_OFFS)
#define DSP_BPM_SWAP_OFFS                           (BAR4_ADDR | DSP_BPM_RAW_SWAP_OFFS)

/* ACQ Components */
#define WB_ACQ_CORE_CTRL_REGS_OFFS                  (BAR4_ADDR | WB_ACQ_CORE_CTRL_RAW_REGS_OFFS)

/* Trigger Interface Components */
#define WB_TRIGGER_IFACE_REG_OFFS                   (BAR4_ADDR | WB_TRIGGER_IFACE_RAW_REG_OFFS)

/* Trigger Mux Components */
#define WB_TRIGGER_MUX_REG_OFFS                     (BAR4_ADDR | WB_TRIGGER_MUX_RAW_REG_OFFS)

/* Large Memory Addresses */
#define LARGE_MEM_ADDR                              (BAR2_ADDR | LARGE_MEM_RAW_ADDR)

#ifdef __cplusplus
}
#endif

#endif
