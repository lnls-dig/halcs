/*
 * Copyright (C) 2021 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "acq_chan_gen_defs.h"
#include "boards/ml605/acq_chan.h"
#include "boards/ml605/ddr3_map.h"

// for HUTILS_EXPORT_SYMBOL
#include "hutils.h"

#include <stddef.h>

/******************************************************/
/*                 Channels definitions               */
/******************************************************/

const size_t ml605_num_acq_core_smios   = __NUM_ACQ_CORE_SMIOS;
HUTILS_EXPORT_SYMBOL(const size_t *, const_size_t_p, ml605_num_acq_core_smios);

/* ADC */
const uint32_t ml605_adc_chan_id        = __ADC_CHAN_ID;
HUTILS_EXPORT_SYMBOL(const uint32_t *, const_uint32_t_p, ml605_adc_chan_id);

/* ADC SWAPPED (same as ADC as no switching module for ml605) */
const uint32_t ml605_adcswap_chan_id    = __ADC_CHAN_ID;
HUTILS_EXPORT_SYMBOL(const uint32_t *, const_uint32_t_p, ml605_adcswap_chan_id);

/* End of channels placeholder */
const uint32_t ml605_end_chan_id        = __END_CHAN_ID;
HUTILS_EXPORT_SYMBOL(const uint32_t *, const_uint32_t_p, ml605_end_chan_id);

const acq_buf_t ml605_acq_buf[__NUM_ACQ_CORE_SMIOS][__END_CHAN_ID] = {
    /*** Acquistion 0 Channel Parameters ***/
    {
        {
            .id = __ADC_CHAN_ID,
            .start_addr = DDR3_ADC0_START_ADDR,
            .end_addr = DDR3_ADC0_END_ADDR
        },
        {
            .id = __TBTAMP_CHAN_ID,
            .start_addr = DDR3_TBTAMP0_START_ADDR,
            .end_addr = DDR3_TBTAMP0_END_ADDR
        },
        {
            .id = __TBTPOS_CHAN_ID,
            .start_addr = DDR3_TBTPOS0_START_ADDR,
            .end_addr = DDR3_TBTPOS0_END_ADDR
        },
        {
            .id = __FOFBAMP_CHAN_ID,
            .start_addr = DDR3_FOFBAMP0_START_ADDR,
            .end_addr = DDR3_FOFBAMP0_END_ADDR
        },
        {
            .id = __FOFBPOS_CHAN_ID,
            .start_addr = DDR3_FOFBPOS0_START_ADDR,
            .end_addr = DDR3_FOFBPOS0_END_ADDR
        },
        {
            .id = __MONITAMP_CHAN_ID,
            .start_addr = DDR3_MONITAMP0_START_ADDR,
            .end_addr = DDR3_MONITAMP0_END_ADDR
        },
        {
            .id = __MONITPOS_CHAN_ID,
            .start_addr = DDR3_MONITPOS0_START_ADDR,
            .end_addr = DDR3_MONITPOS0_END_ADDR
        },
        {
            .id = __MONIT1POS_CHAN_ID,
            .start_addr = DDR3_MONIT1POS0_START_ADDR,
            .end_addr = DDR3_MONIT1POS0_END_ADDR
        }
    }
};

/* Acessed as a 1D-array */
HUTILS_EXPORT_SYMBOL(const acq_buf_t *, const_acq_buf_t_p, ml605_acq_buf);
