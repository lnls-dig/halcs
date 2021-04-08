#ifndef _MEM_LAYOUT_H_
#define _MEM_LAYOUT_H_

#include "mem_layout_common.h"
#include "acq_chan.h"

#define NUM_MAX_SLOTS                               12
#define NUM_MAX_HALCS_PER_SLOT                        2
#define NUM_MAX_HALCSS                                (NUM_MAX_SLOTS * NUM_MAX_HALCS_PER_SLOT)

#define NUM_FMC130M_4CH_SMIOS                       2
#define NUM_FMC250M_4CH_SMIOS                       2

/*********************** Static AFCv3 FPGA layout ***********************/

#define SDB_ADDRESS                                 0x00000000UL

/* Large Memory RAW Addresses. It lives at address 0 */
#define LARGE_MEM_RAW_ADDR                          0x00000000

/************************* AFCv3 Gateware Options *************************/

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

#endif

