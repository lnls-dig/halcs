/*
 * Copyright (C) 2015 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "bpm_server.h"

const uint32_t fmc130m_4ch_si571_addr[NUM_FMC130M_4CH_SMIOS] = {0x49, 0x49};
const uint32_t fmc130m_4ch_ad9510_addr[NUM_FMC130M_4CH_SMIOS] = {0x01, 0x01};

/* The FMC EEPROM was supposed to have address 0x50 and 0x52, but GA1 and GA0
 * are all grounded on hardware */
/* const uint32_t fmc130m_4ch_24aa64_addr[NUM_FMC130M_4CH_SMIOS] = {0x50, 0x52}; */
const uint32_t fmc130m_4ch_24aa64_addr[NUM_FMC130M_4CH_SMIOS] = {0x50, 0x50};
const uint32_t fmc130m_4ch_lm75a_addr[NUM_FMC130M_4CH_SMIOS][NUM_FMC130M_4CH_LM75A] = {
        {0x48, 0x49},
        {0x48, 0x49},
    };

/* This CI PCA9547 is located on the carrier, but it's controlled by the FMC130M_4CH */
const uint32_t fmc130m_4ch_pca9547_addr[NUM_FMC130M_4CH_SMIOS] = {0x70, 0x70};

