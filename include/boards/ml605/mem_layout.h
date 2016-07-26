#ifndef _MEM_LAYOUT_H_
#define _MEM_LAYOUT_H_

#include "mem_layout_common.h"
#include "acq_chan.h"

#define NUM_MAX_SLOTS                               1
#define NUM_MAX_BPM_PER_SLOT                        2
#define NUM_MAX_BPMS                                (NUM_MAX_SLOTS * NUM_MAX_BPM_PER_SLOT)

#define NUM_FMC130M_4CH_SMIOS                       1
#define NUM_FMC250M_4CH_SMIOS                       1

/*********************** Static ML605 FPGA layout ***********************/

#define SDB_ADDRESS                                 0x00300000UL

/* Should be autodiscovered by SDB */

/* Wishbone RAW Addresses */
#define DSP1_BASE_RAW_ADDR                          0x00308000

#define DSP1_CTRL_RAW_REGS                          (DSP1_BASE_RAW_ADDR +       \
                                                        DSP_CTRL_RAW_REGS_OFFS)
#define DSP1_BPM_RAW_SWAP                           (DSP1_BASE_RAW_ADDR +       \
                                                        DSP_BPM_RAW_SWAP_OFFS)

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

/* Wishbone Addresses */
#define DSP1_BASE_ADDR                              (BAR4_ADDR | DSP1_BASE_RAW_ADDR)

#define DSP1_CTRL_REGS                              (BAR4_ADDR | DSP1_CTRL_RAW_REGS)
#define DSP1_BPM_SWAP                               (BAR4_ADDR | DSP1_BPM_RAW_SWAP)

#define FMC1_130M_BASE_ADDR                         (BAR4_ADDR | FMC1_130M_BASE_RAW_ADDR)

#define FMC1_130M_CTRL_REGS                         (BAR4_ADDR | FMC1_130M_CTRL_RAW_REGS)
#define FMC1_130M_FMC_ADC_COMMON                    (BAR4_ADDR | FMC1_130M_FMC_ADC_COMMON_RAW_REGS)
#define FMC1_130M_FMC_ACTIVE_CLK                    (BAR4_ADDR | FMC1_130M_FMC_ACTIVE_CLK_RAW)
#define FMC1_130M_EEPROM_I2C                        (BAR4_ADDR | FMC1_130M_EEPROM_RAW_I2C)
#define FMC1_130M_LM75A_I2C                         (BAR4_ADDR | FMC1_130M_LM75A_RAW_I2C)

#define FMC1_250M_BASE_ADDR                         (BAR4_ADDR | FMC1_250M_BASE_RAW_ADDR)

#define FMC1_250M_CTRL_REGS                         (BAR4_ADDR | FMC1_250M_CTRL_RAW_REGS)
#define FMC1_250M_FMC_ADC_COMMON                    (BAR4_ADDR | FMC1_250M_FMC_ADC_COMMON_RAW_REGS)
#define FMC1_250M_FMC_ACTIVE_CLK                    (BAR4_ADDR | FMC1_250M_FMC_ACTIVE_CLK_RAW)
#define FMC1_250M_EEPROM_I2C                        (BAR4_ADDR | FMC1_250M_EEPROM_RAW_I2C)
#define FMC1_250M_AMC7823_SPI                       (BAR4_ADDR | FMC1_250M_AMC7823_RAW_SPI)
#define FMC1_250M_ISLA216P_SPI                      (BAR4_ADDR | FMC1_250M_ISLA216P_RAW_SPI)

#define WB_ACQ1_BASE_ADDR                           (BAR4_ADDR | WB_ACQ1_BASE_RAW_ADDR)

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

