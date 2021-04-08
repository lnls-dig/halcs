/*
 * Copyright (C) 2015 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _CHIPS_ADDR_H_
#define _CHIPS_ADDR_H_

#include "mem_layout_common.h"
#include "boards/afcv3/acq_chan.h"

#define __NUM_FMC130M_4CH_SMIOS         2
extern const size_t NUM_FMC130M_4CH_SMIOS;
#define __NUM_FMC130M_4CH_LM75A         2
extern const size_t NUM_FMC130M_4CH_LM75A;
#define __NUM_FMC250M_4CH_SMIOS         2
extern const size_t NUM_FMC250M_4CH_SMIOS;
#define __NUM_FMC250M_4CH_ISLA216P      4
extern const size_t NUM_FMC250M_4CH_ISLA216P;

/************************* AFCv3 Gateware Options *************************/

/********************* FMC130M_4CH SMIO Gateware Options ******************/

/* Chip SPI slave select lines and I2C address */

extern const uint32_t fmc130m_4ch_si571_addr[__NUM_FMC130M_4CH_SMIOS];
extern const uint32_t fmc130m_4ch_ad9510_addr[__NUM_FMC130M_4CH_SMIOS];
extern const uint32_t fmc130m_4ch_24aa64_addr[__NUM_FMC130M_4CH_SMIOS];
extern const uint32_t fmc130m_4ch_lm75a_addr[__NUM_FMC130M_4CH_SMIOS][__NUM_FMC130M_4CH_LM75A];
extern const uint32_t fmc130m_4ch_pca9547_addr[__NUM_FMC130M_4CH_SMIOS];

/********************* FMC250M_4CH SMIO Gateware Options ******************/

/* Chip SPI slave select lines and I2C address */

extern const uint32_t fmc250m_4ch_amc7823_addr[__NUM_FMC250M_4CH_SMIOS];
extern const uint32_t fmc250m_4ch_isla216p_addr[__NUM_FMC250M_4CH_SMIOS][__NUM_FMC250M_4CH_ISLA216P];
extern const uint32_t fmc250m_4ch_ad9510_addr[__NUM_FMC250M_4CH_SMIOS];
extern const uint32_t fmc250m_4ch_si571_addr[__NUM_FMC250M_4CH_SMIOS];
extern const uint32_t fmc250m_4ch_24aa64_addr[__NUM_FMC250M_4CH_SMIOS];
extern const uint32_t fmc250m_4ch_pca9547_addr[__NUM_FMC250M_4CH_SMIOS];

#endif
