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

