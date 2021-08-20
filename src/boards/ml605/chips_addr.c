/*
 * Copyright (C) 2015 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "halcs_server.h"

#define __NUM_FMC130M_4CH_SMIOS         1
const size_t ml605_num_fmc130m_4ch_smios      = __NUM_FMC130M_4CH_SMIOS;
HUTILS_EXPORT_SYMBOL(const uint32_t *, const_uint32_t_p, ml605_num_fmc130m_4ch_smios);
#define __NUM_FMC250M_4CH_SMIOS         1
const size_t ml605_num_fmc250m_4ch_smios      = __NUM_FMC250M_4CH_SMIOS;
HUTILS_EXPORT_SYMBOL(const uint32_t *, const_uint32_t_p, ml605_num_fmc250m_4ch_smios);

const uint32_t ml605_fmc130m_4ch_si571_addr[__NUM_FMC130M_4CH_SMIOS] = {0x49};
HUTILS_EXPORT_SYMBOL(const uint32_t *, const_uint32_t_p, ml605_fmc130m_4ch_si571_addr);
const uint32_t ml605_fmc130m_4ch_ad9510_addr[__NUM_FMC130M_4CH_SMIOS] = {0x01};
HUTILS_EXPORT_SYMBOL(const uint32_t *, const_uint32_t_p, ml605_fmc130m_4ch_ad9510_addr);

/* The FMC EEPROM was supposed to have address 0x50 and 0x52, but GA1 and GA0
 * are all grounded on hardware */
const uint32_t ml605_fmc130m_4ch_24aa64_addr[__NUM_FMC130M_4CH_SMIOS] = {0x50};
HUTILS_EXPORT_SYMBOL(const uint32_t *, const_uint32_t_p, ml605_fmc130m_4ch_24aa64_addr);
const uint32_t ml605_fmc130m_4ch_lm75a_addr[__NUM_FMC130M_4CH_SMIOS][NUM_FMC130M_4CH_LM75A] = {
        {0x48, 0x49},
    };
HUTILS_EXPORT_SYMBOL(const uint32_t *, const_uint32_t_p, ml605_fmc130m_4ch_lm75a_addr);

/* This CI PCA9547 is located on the carrier, but it's controlled by the FMC130M_4CH */
const uint32_t ml605_fmc130m_4ch_pca9547_addr[__NUM_FMC130M_4CH_SMIOS] = {0x70};
HUTILS_EXPORT_SYMBOL(const uint32_t *, const_uint32_t_p, ml605_fmc130m_4ch_pca9547_addr);

/* FMC250M_4CH chip addresses */
const uint32_t ml605_fmc250m_4ch_si571_addr[__NUM_FMC250M_4CH_SMIOS] = {0x49};
HUTILS_EXPORT_SYMBOL(const uint32_t *, const_uint32_t_p, ml605_fmc250m_4ch_si571_addr);
const uint32_t ml605_fmc250m_4ch_ad9510_addr[__NUM_FMC250M_4CH_SMIOS] = {0x01};
HUTILS_EXPORT_SYMBOL(const uint32_t *, const_uint32_t_p, ml605_fmc250m_4ch_ad9510_addr);

/* The FMC EEPROM was supposed to have address 0x50 and 0x52, but GA1 and GA0
 * are all grounded on hardware */
const uint32_t ml605_fmc250m_4ch_24aa64_addr[__NUM_FMC250M_4CH_SMIOS] = {0x50};
HUTILS_EXPORT_SYMBOL(const uint32_t *, const_uint32_t_p, ml605_fmc250m_4ch_24aa64_addr);

const uint32_t ml605_fmc250m_4ch_amc7823_addr[__NUM_FMC250M_4CH_SMIOS] = {0x01};
HUTILS_EXPORT_SYMBOL(const uint32_t *, const_uint32_t_p, ml605_fmc250m_4ch_amc7823_addr);
const uint32_t ml605_fmc250m_4ch_isla216p_addr[__NUM_FMC250M_4CH_SMIOS][NUM_FMC250M_4CH_ISLA216P] = {
        {0x01, 0x02, 0x03, 0x04}
};
HUTILS_EXPORT_SYMBOL(const uint32_t *, const_uint32_t_p, ml605_fmc250m_4ch_isla216p_addr);
/* This CI PCA9547 is located on the carrier, but it's controlled by the FMC250M_4CH */
const uint32_t ml605_fmc250m_4ch_pca9547_addr[__NUM_FMC250M_4CH_SMIOS] = {0x70};
HUTILS_EXPORT_SYMBOL(const uint32_t *, const_uint32_t_p, ml605_fmc250m_4ch_pca9547_addr);
