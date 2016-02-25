#ifndef _MEM_LAYOUT_H_
#define _MEM_LAYOUT_H_

#include "pcie_regs.h"
#include "acq_chan.h"

#define NUM_MAX_SLOTS                               1
#define NUM_MAX_BPM_PER_SLOT                        2
#define NUM_MAX_BPMS                                (NUM_MAX_SLOTS * NUM_MAX_BPM_PER_SLOT)

#define NUM_FMC130M_4CH_SMIOS                       1
#define NUM_FMC250M_4CH_SMIOS                       1

/*********************** Static ML605 FPGA layout ***********************/
/* FMC_130M Components */
#define FMC_130M_CTRL_RAW_REGS_OFFS                 0x0000
#define FMC_130M_SI571_RAW_I2C_OFFS                 0x0100
#define FMC_130M_AD9510_RAW_SPI_OFFS                0x0200
#define FMC_130M_EEPROM_RAW_I2C_OFFS                0x0300
#define FMC_130M_LM75A_RAW_I2C_OFFS                 0x0400

/* FMC_250M Components */
#define FMC_250M_CTRL_RAW_REGS_OFFS                 0x0000
#define FMC_250M_AMC7823_RAW_SPI_OFFS               0x0100
#define FMC_250M_ISLA216P_RAW_SPI_OFFS              0x0200
#define FMC_250M_AD9510_RAW_SPI_OFFS                0x0300
#define FMC_250M_SI571_RAW_I2C_OFFS                 0x0400
#define FMC_250M_EEPROM_RAW_I2C_OFFS                0x0500

/* DSP Components */
#define DSP_CTRL_RAW_REGS_OFFS                      0x0000
#define DSP_BPM_RAW_SWAP_OFFS                       0x0100

/* ACQ Components */
#define WB_ACQ_CORE_CTRL_RAW_REGS_OFFS              0x0000

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

#define FMC1_250M_BASE_RAW_ADDR                     0x00310000

#define FMC1_250M_CTRL_RAW_REGS                     (FMC1_250M_BASE_RAW_ADDR +  \
                                                        FMC_250M_CTRL_RAW_REGS_OFFS)
#define FMC1_250M_AMC7823_RAW_SPI                   (FMC1_250M_BASE_RAW_ADDR +  \
                                                        FMC_250M_AMC7823_RAW_SPI_OFFS)
#define FMC1_250M_ISLA216P_RAW_SPI                  (FMC1_250M_BASE_RAW_ADDR +  \
                                                        FMC_250M_ISLA216P_RAW_SPI_OFFS)
#define FMC1_250M_AD9510_RAW_SPI                    (FMC1_250M_BASE_RAW_ADDR + \
                                                        FMC_250M_AD9510_RAW_SPI_OFFS)
#define FMC1_250M_SI571_RAW_I2C                     (FMC1_250M_BASE_RAW_ADDR +  \
                                                        FMC_250M_SI571_RAW_I2C_OFFS)
#define FMC1_250M_EEPROM_RAW_I2C                    (FMC1_250M_BASE_RAW_ADDR + \
                                                        FMC_250M_EEPROM_RAW_I2C_OFFS)

#define DSP1_BASE_RAW_ADDR                          0x00308000

#define DSP1_CTRL_RAW_REGS                          (DSP1_BASE_RAW_ADDR +       \
                                                        DSP_CTRL_RAW_REGS_OFFS)
#define DSP1_BPM_RAW_SWAP                           (DSP1_BASE_RAW_ADDR +       \
                                                        DSP_BPM_RAW_SWAP_OFFS)

#define WB_ACQ1_BASE_RAW_ADDR                       0x00330000

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
#define FMC_130M_CTRL_REGS_OFFS                     (/*BAR4_ADDR |*/ FMC_130M_CTRL_RAW_REGS_OFFS)
#define FMC_130M_SI571_I2C_OFFS                     (/*BAR4_ADDR |*/ FMC_130M_SI571_RAW_I2C_OFFS)
#define FMC_130M_AD9510_SPI_OFFS                    (/*BAR4_ADDR |*/ FMC_130M_AD9510_RAW_SPI_OFFS)
#define FMC_130M_EEPROM_I2C_OFFS                    (/*BAR4_ADDR |*/ FMC_130M_EEPROM_RAW_I2C_OFFS)
#define FMC_130M_LM75A_I2C_OFFS                     (/*BAR4_ADDR |*/ FMC_130M_LM75A_RAW_I2C_OFFS)

/* FMC_250M Components */
#define FMC_250M_CTRL_REGS_OFFS                     (BAR4_ADDR | FMC_250M_CTRL_RAW_REGS_OFFS)
#define FMC_250M_AMC7823_SPI_OFFS                   (BAR4_ADDR | FMC_250M_AMC7823_RAW_SPI_OFFS)
#define FMC_250M_ISLA216P_SPI_OFFS                  (BAR4_ADDR | FMC_250M_ISLA216P_RAW_SPI_OFFS)
#define FMC_250M_AD9510_SPI_OFFS                    (BAR4_ADDR | FMC_250M_AD9510_RAW_SPI_OFFS)
#define FMC_250M_SI571_I2C_OFFS                     (BAR4_ADDR | FMC_250M_SI571_RAW_I2C_OFFS)
#define FMC_250M_EEPROM_I2C_OFFS                    (BAR4_ADDR | FMC_250M_EEPROM_RAW_I2C_OFFS)

/* DSP Components */
#define DSP_CTRL_REGS_OFFS                          (/*BAR4_ADDR |*/ DSP_CTRL_RAW_REGS_OFFS)
#define DSP_BPM_SWAP_OFFS                           (/*BAR4_ADDR |*/ DSP_BPM_RAW_SWAP_OFFS)

/* ACQ Components */
#define WB_ACQ_CORE_CTRL_REGS_OFFS                  (/*BAR4_ADDR |*/ WB_ACQ_CORE_CTRL_RAW_REGS_OFFS)

/* Wishbone Addresses */
#define FMC1_130M_BASE_ADDR                         (BAR4_ADDR | FMC1_130M_BASE_RAW_ADDR)

#define FMC1_130M_CTRL_REGS                         (/*BAR4_ADDR |*/ FMC1_130M_CTRL_RAW_REGS)
#define FMC1_130M_SI571_I2C                         (/*BAR4_ADDR |*/ FMC1_130M_SI571_RAW_I2C)
#define FMC1_130M_AD9510_SPI                        (/*BAR4_ADDR |*/ FMC1_130M_AD9510_RAW_SPI)
#define FMC1_130M_EEPROM_I2C                        (/*BAR4_ADDR |*/ FMC1_130M_EEPROM_RAW_I2C)
#define FMC1_130M_LM75A_I2C                         (/*BAR4_ADDR |*/ FMC1_130M_LM75A_RAW_I2C)

#define FMC1_250M_BASE_ADDR                         (BAR4_ADDR | FMC1_250M_BASE_RAW_ADDR)

#define FMC1_250M_CTRL_REGS                         (/*BAR4_ADDR |*/ FMC1_250M_CTRL_RAW_REGS)
#define FMC1_250M_AMC7823_SPI                       (/*BAR4_ADDR |*/ FMC1_250M_AMC7823_RAW_SPI)
#define FMC1_250M_ISLA216P_SPI                      (/*BAR4_ADDR |*/ FMC1_250M_ISLA216P_RAW_SPI)
#define FMC1_250M_AD9510_SPI                        (/*BAR4_ADDR |*/ FMC1_250M_AD9510_RAW_SPI)
#define FMC1_250M_SI571_I2C                         (/*BAR4_ADDR |*/ FMC1_250M_SI571_RAW_I2C)
#define FMC1_250M_EEPROM_I2C                        (/*BAR4_ADDR |*/ FMC1_250M_EEPROM_RAW_I2C)

#define DSP1_BASE_ADDR                              (BAR4_ADDR | DSP1_BASE_RAW_ADDR)

#define DSP1_CTRL_REGS                              (/*BAR4_ADDR |*/ DSP1_CTRL_RAW_REGS)
#define DSP1_BPM_SWAP                               (/*BAR4_ADDR |*/ DSP1_BPM_RAW_SWAP)

#define WB_ACQ1_BASE_ADDR                           (BAR4_ADDR | WB_ACQ1_BASE_RAW_ADDR)

/* Large Memory Addresses */
#define LARGE_MEM_ADDR                              (BAR2_ADDR | LARGE_MEM_RAW_ADDR)

/************************* ML605 Gateware Options *************************/

/********************* FMC130M_4CH SMIO Gateware Options ******************/

/* Chip SPI slave select lines and I2C address */

#define NUM_FMC130M_4CH_LM75A                        2

extern const uint32_t fmc130m_4ch_si571_addr[NUM_FMC130M_4CH_SMIOS];
extern const uint32_t fmc130m_4ch_ad9510_addr[NUM_FMC130M_4CH_SMIOS];
extern const uint32_t fmc130m_4ch_24aa64_addr[NUM_FMC130M_4CH_SMIOS];
extern const uint32_t fmc130m_4ch_lm75a_addr[NUM_FMC130M_4CH_SMIOS][NUM_FMC130M_4CH_LM75A];
extern const uint32_t fmc130m_4ch_pca9547_addr[NUM_FMC130M_4CH_SMIOS];

/********************* FMC250M_4CH SMIO Gateware Options ******************/

/* Chip SPI slave select lines and I2C address */

#define NUM_FMC250M_4CH_ISLA216P                     4

extern const uint32_t fmc250m_4ch_amc7823_addr[NUM_FMC250M_4CH_SMIOS];
extern const uint32_t fmc250m_4ch_isla216p_addr[NUM_FMC250M_4CH_SMIOS][NUM_FMC250M_4CH_ISLA216P];
extern const uint32_t fmc250m_4ch_ad9510_addr[NUM_FMC250M_4CH_SMIOS];
extern const uint32_t fmc250m_4ch_si571_addr[NUM_FMC250M_4CH_SMIOS];
extern const uint32_t fmc250m_4ch_24aa64_addr[NUM_FMC250M_4CH_SMIOS];
extern const uint32_t fmc250m_4ch_pca9547_addr[NUM_FMC250M_4CH_SMIOS];

/*********************** SMIO ACQ Gateware Options ***********************/
/* FIXME: The Gateware does not support yet a way of reading which
 * ACQ channels are valid. So, we hardcoded it here for now. This issue
 * should be addressed soon */

/* We define 5 Channels: ADC 1, TBT AMP 1, TBT POS 1, FOFB AMP 1, FOFB POS 1 */
#define SMIO_ACQ_NUM_CHANNELS                       END_CHAN_ID

#endif

