#ifndef _MEM_LAYOUT_COMMON_
#define _MEM_LAYOUT_COMMON_

#ifdef __cplusplus
extern "C" {
#endif

#include "hw/pcie_regs.h"

/* FMC_ACTIVE_CLK Component */
#define FMC_ACTIVE_CLK_CTRL_RAW_REGS_OFFS           0x0000
#define FMC_ACTIVE_CLK_SI571_RAW_I2C_OFFS           0x0100
#define FMC_ACTIVE_CLK_AD9510_RAW_SPI_OFFS          0x0200

/* FMC_100M Component */
#define FMC_100M_4CH_CTRL_RAW_REGS_OFFS             0x0000
#define FMC_100M_4CH_SI571_RAW_I2C_OFFS             0x0500
#define FMC_100M_4CH_LTC2174_RAW_SPI_OFFS           0x0700

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

/* AFC MGMT Component */
#define AFC_MGMT_CTRL_RAW_REGS_OFFS                 0x0000
#define AFC_MGMT_SI57X_RAW_I2C_OFFS                 0x0100

/* Large Memory RAW Addresses. It lives at address 0 */
#define LARGE_MEM_RAW_ADDR                          0x00000000

/* FMC_ACTIVE_CLK Component */
#define FMC_ACTIVE_CLK_CTRL_REGS_OFFS               (FMC_ACTIVE_CLK_CTRL_RAW_REGS_OFFS)
#define FMC_ACTIVE_CLK_SI571_I2C_OFFS               (FMC_ACTIVE_CLK_SI571_RAW_I2C_OFFS)
#define FMC_ACTIVE_CLK_AD9510_SPI_OFFS              (FMC_ACTIVE_CLK_AD9510_RAW_SPI_OFFS)

/* FMC_100M Component */
#define FMC_100M_4CH_CTRL_REGS_OFFS                 (FMC_100M_4CH_CTRL_RAW_REGS_OFFS)
#define FMC_100M_4CH_SI571_I2C_OFFS                 (FMC_100M_4CH_SI571_RAW_I2C_OFFS)
#define FMC_100M_4CH_LTC2174_SPI_OFFS               (FMC_100M_4CH_LTC2174_RAW_SPI_OFFS)

/* FMC_130M Components */
#define FMC_130M_CTRL_REGS_OFFS                     (FMC_130M_CTRL_RAW_REGS_OFFS)
#define FMC_130M_FMC_ADC_COMMON_OFFS                (FMC_130M_FMC_ADC_COMMON_RAW_REGS_OFFS)
#define FMC_130M_FMC_ACTIVE_CLK_OFFS                (FMC_130M_FMC_ACTIVE_CLK_RAW_OFFS)
#define FMC_130M_EEPROM_I2C_OFFS                    (FMC_130M_EEPROM_RAW_I2C_OFFS)
#define FMC_130M_LM75A_I2C_OFFS                     (FMC_130M_LM75A_RAW_I2C_OFFS)

/* FMC_250M Components */
#define FMC_250M_CTRL_REGS_OFFS                     (FMC_250M_CTRL_RAW_REGS_OFFS)
#define FMC_250M_FMC_ADC_COMMON_OFFS                (FMC_250M_FMC_ADC_COMMON_RAW_REGS_OFFS)
#define FMC_250M_FMC_ACTIVE_CLK_OFFS                (FMC_250M_FMC_ACTIVE_CLK_RAW_OFFS)
#define FMC_250M_EEPROM_I2C_OFFS                    (FMC_250M_EEPROM_RAW_I2C_OFFS)
#define FMC_250M_AMC7823_SPI_OFFS                   (FMC_250M_AMC7823_RAW_SPI_OFFS)
#define FMC_250M_ISLA216P_SPI_OFFS                  (FMC_250M_ISLA216P_RAW_SPI_OFFS)

/* AFC_MGMT Component */
#define AFC_MGMT_CTRL_REGS_OFFS                     (AFC_MGMT_CTRL_RAW_REGS_OFFS)
#define AFC_MGMT_SI57X_I2C_OFFS                     (AFC_MGMT_SI57X_RAW_I2C_OFFS)

/* The following is a bit of a hack.
 * We employ a generic API for talking to the hardware.
 * So, our transport layer (PCIe or Ethernet, for now)
 * should be invisible to the SMIO instances.
 *
 * However, PCI devices generally employ multiple BAR
 * registers mapped to different parts of the device.
 * For instance, in the halcs-gw FPGA firmware, the PCIe
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

/* FIXME. Large Memory Addresses. We should not have to specify BAR2_ADDR,
 * as this is specific to PCIe */
#define LARGE_MEM_ADDR                              (BAR2_ADDR | LARGE_MEM_RAW_ADDR)

#ifdef __cplusplus
}
#endif

#endif
