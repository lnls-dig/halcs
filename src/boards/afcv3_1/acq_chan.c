/*
 * Copyright (C) 2021 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "acq_chan_gen_defs.h"
#include "boards/afcv3_1/acq_chan.h"
#include "boards/afcv3_1/ddr3_map.h"

// for HUTILS_EXPORT_SYMBOL
#include "hutils.h"

#include <stddef.h>

/******************************************************/
/*                 Channels definitions               */
/******************************************************/

#define ACQ0_START 0x0
#define ACQ0_END 0x10000000
#define ACQ1_START 0x10000000
#define ACQ1_END 0x20000000
#define ACQ2_START 0x20000000
#define ACQ2_END 0x30000000
#define ACQ3_START 0x30000000
#define ACQ3_END 0x40000000

const size_t afcv3_1_num_acq_core_smios         = __NUM_ACQ_CORE_SMIOS;
HUTILS_EXPORT_SYMBOL(const size_t *, const_size_t_p, afcv3_1_num_acq_core_smios);

/* ADC */
const uint32_t afcv3_1_adc_chan_id              = __ADC_CHAN_ID;
HUTILS_EXPORT_SYMBOL(const uint32_t *, const_uint32_t_p, afcv3_1_adc_chan_id);

/* ADC SWAPPED (after the switching module) */
const uint32_t afcv3_1_adcswap_chan_id          = __ADCSWAP_CHAN_ID;
HUTILS_EXPORT_SYMBOL(const uint32_t *, const_uint32_t_p, afcv3_1_adcswap_chan_id);

/* End of channels placeholder */
const uint32_t afcv3_1_end_chan_id              = __END_CHAN_ID;
HUTILS_EXPORT_SYMBOL(const uint32_t *, const_uint32_t_p, afcv3_1_end_chan_id);

const acq_buf_t afcv3_1_acq_buf[__NUM_ACQ_CORE_SMIOS][__END_CHAN_ID] = {
    /*** Acquisition Core 0 Channel Parameters ***/
    {
        {
            .id = __ADC_CHAN_ID,
            .start_addr = ACQ0_START,
            .end_addr = ACQ0_END
        },
        {
            .id = __ADCSWAP_CHAN_ID,
            .start_addr = ACQ0_START,
            .end_addr = ACQ0_END
        },
        {
            .id = __MIXIQ_CHAN_ID,
            .start_addr = ACQ0_START,
            .end_addr = ACQ0_END
        },
        {
            .id = __DUMMY0_CHAN_ID,
            .start_addr = ACQ0_START,
            .end_addr = ACQ0_END
        },
        {
            .id = __TBTDECIMIQ_CHAN_ID,
            .start_addr = ACQ0_START,
            .end_addr = ACQ0_END
        },
        {
            .id = __DUMMY1_CHAN_ID,
            .start_addr = ACQ0_START,
            .end_addr = ACQ0_END
        },
        {
            .id = __TBTAMP_CHAN_ID,
            .start_addr = ACQ0_START,
            .end_addr = ACQ0_END
        },
        {
            .id = __TBTPHA_CHAN_ID,
            .start_addr = ACQ0_START,
            .end_addr = ACQ0_END
        },
        {
            .id = __TBTPOS_CHAN_ID,
            .start_addr = ACQ0_START,
            .end_addr = ACQ0_END
        },
        {
            .id = __FOFBDECIMIQ_CHAN_ID,
            .start_addr = ACQ0_START,
            .end_addr = ACQ0_END
        },
        {
            .id = __DUMMY2_CHAN_ID,
            .start_addr = ACQ0_START,
            .end_addr = ACQ0_END
        },
        {
            .id = __FOFBAMP_CHAN_ID,
            .start_addr = ACQ0_START,
            .end_addr = ACQ0_END
        },
        {
            .id = __FOFBPHA_CHAN_ID,
            .start_addr = ACQ0_START,
            .end_addr = ACQ0_END
        },
        {
            .id = __FOFBPOS_CHAN_ID,
            .start_addr = ACQ0_START,
            .end_addr = ACQ0_END
        },
        {
            .id = __MONIT1AMP_CHAN_ID,
            .start_addr = ACQ0_START,
            .end_addr = ACQ0_END
        },
        {
            .id = __MONIT1POS_CHAN_ID,
            .start_addr = ACQ0_START,
            .end_addr = ACQ0_END
        },
        {
            .id = __MONITAMP_CHAN_ID,
            .start_addr = ACQ0_START,
            .end_addr = ACQ0_END
        },
        {
            .id = __MONITPOS_CHAN_ID,
            .start_addr = ACQ0_START,
            .end_addr = ACQ0_END
        },
    },
    /*** Acquisition Core 1 Channel Parameters ***/
    {
        {
            .id = __ADC_CHAN_ID,
            .start_addr = ACQ1_START,
            .end_addr = ACQ1_END
        },
        {
            .id = __ADCSWAP_CHAN_ID,
            .start_addr = ACQ1_START,
            .end_addr = ACQ1_END
        },
        {
            .id = __MIXIQ_CHAN_ID,
            .start_addr = ACQ1_START,
            .end_addr = ACQ1_END
        },
        {
            .id = __DUMMY0_CHAN_ID,
            .start_addr = ACQ1_START,
            .end_addr = ACQ1_END
        },
        {
            .id = __TBTDECIMIQ_CHAN_ID,
            .start_addr = ACQ1_START,
            .end_addr = ACQ1_END
        },
        {
            .id = __DUMMY1_CHAN_ID,
            .start_addr = ACQ1_START,
            .end_addr = ACQ1_END
        },
        {
            .id = __TBTAMP_CHAN_ID,
            .start_addr = ACQ1_START,
            .end_addr = ACQ1_END
        },
        {
            .id = __TBTPHA_CHAN_ID,
            .start_addr = ACQ1_START,
            .end_addr = ACQ1_END
        },
        {
            .id = __TBTPOS_CHAN_ID,
            .start_addr = ACQ1_START,
            .end_addr = ACQ1_END
        },
        {
            .id = __FOFBDECIMIQ_CHAN_ID,
            .start_addr = ACQ1_START,
            .end_addr = ACQ1_END
        },
        {
            .id = __DUMMY2_CHAN_ID,
            .start_addr = ACQ1_START,
            .end_addr = ACQ1_END
        },
        {
            .id = __FOFBAMP_CHAN_ID,
            .start_addr = ACQ1_START,
            .end_addr = ACQ1_END
        },
        {
            .id = __FOFBPHA_CHAN_ID,
            .start_addr = ACQ1_START,
            .end_addr = ACQ1_END
        },
        {
            .id = __FOFBPOS_CHAN_ID,
            .start_addr = ACQ1_START,
            .end_addr = ACQ1_END
        },
        {
            .id = __MONIT1AMP_CHAN_ID,
            .start_addr = ACQ1_START,
            .end_addr = ACQ1_END
        },
        {
            .id = __MONIT1POS_CHAN_ID,
            .start_addr = ACQ1_START,
            .end_addr = ACQ1_END
        },
        {
            .id = __MONITAMP_CHAN_ID,
            .start_addr = ACQ1_START,
            .end_addr = ACQ1_END
        },
        {
            .id = __MONITPOS_CHAN_ID,
            .start_addr = ACQ1_START,
            .end_addr = ACQ1_END
        },
    },
    /*** Acquisition Core 2 Channel Parameters ***/
    {
        {
            .id = __ADC_CHAN_ID,
            .start_addr = ACQ2_START,
            .end_addr = ACQ2_END
        },
        {
            .id = __ADCSWAP_CHAN_ID,
            .start_addr = ACQ2_START,
            .end_addr = ACQ2_END
        },
        {
            .id = __MIXIQ_CHAN_ID,
            .start_addr = ACQ2_START,
            .end_addr = ACQ2_END
        },
        {
            .id = __DUMMY0_CHAN_ID,
            .start_addr = ACQ2_START,
            .end_addr = ACQ2_END
        },
        {
            .id = __TBTDECIMIQ_CHAN_ID,
            .start_addr = ACQ2_START,
            .end_addr = ACQ2_END
        },
        {
            .id = __DUMMY1_CHAN_ID,
            .start_addr = ACQ2_START,
            .end_addr = ACQ2_END
        },
        {
            .id = __TBTAMP_CHAN_ID,
            .start_addr = ACQ2_START,
            .end_addr = ACQ2_END
        },
        {
            .id = __TBTPHA_CHAN_ID,
            .start_addr = ACQ2_START,
            .end_addr = ACQ2_END
        },
        {
            .id = __TBTPOS_CHAN_ID,
            .start_addr = ACQ2_START,
            .end_addr = ACQ2_END
        },
        {
            .id = __FOFBDECIMIQ_CHAN_ID,
            .start_addr = ACQ2_START,
            .end_addr = ACQ2_END
        },
        {
            .id = __DUMMY2_CHAN_ID,
            .start_addr = ACQ2_START,
            .end_addr = ACQ2_END
        },
        {
            .id = __FOFBAMP_CHAN_ID,
            .start_addr = ACQ2_START,
            .end_addr = ACQ2_END
        },
        {
            .id = __FOFBPHA_CHAN_ID,
            .start_addr = ACQ2_START,
            .end_addr = ACQ2_END
        },
        {
            .id = __FOFBPOS_CHAN_ID,
            .start_addr = ACQ2_START,
            .end_addr = ACQ2_END
        },
        {
            .id = __MONIT1AMP_CHAN_ID,
            .start_addr = ACQ2_START,
            .end_addr = ACQ2_END
        },
        {
            .id = __MONIT1POS_CHAN_ID,
            .start_addr = ACQ2_START,
            .end_addr = ACQ2_END
        },
        {
            .id = __MONITAMP_CHAN_ID,
            .start_addr = ACQ2_START,
            .end_addr = ACQ2_END
        },
        {
            .id = __MONITPOS_CHAN_ID,
            .start_addr = ACQ2_START,
            .end_addr = ACQ2_END
        },
    },
    /*** Acquisition Core 3 Channel Parameters ***/
    {
        {
            .id = __ADC_CHAN_ID,
            .start_addr = ACQ3_START,
            .end_addr = ACQ3_END
        },
        {
            .id = __ADCSWAP_CHAN_ID,
            .start_addr = ACQ3_START,
            .end_addr = ACQ3_END
        },
        {
            .id = __MIXIQ_CHAN_ID,
            .start_addr = ACQ3_START,
            .end_addr = ACQ3_END
        },
        {
            .id = __DUMMY0_CHAN_ID,
            .start_addr = ACQ3_START,
            .end_addr = ACQ3_END
        },
        {
            .id = __TBTDECIMIQ_CHAN_ID,
            .start_addr = ACQ3_START,
            .end_addr = ACQ3_END
        },
        {
            .id = __DUMMY1_CHAN_ID,
            .start_addr = ACQ3_START,
            .end_addr = ACQ3_END
        },
        {
            .id = __TBTAMP_CHAN_ID,
            .start_addr = ACQ3_START,
            .end_addr = ACQ3_END
        },
        {
            .id = __TBTPHA_CHAN_ID,
            .start_addr = ACQ3_START,
            .end_addr = ACQ3_END
        },
        {
            .id = __TBTPOS_CHAN_ID,
            .start_addr = ACQ3_START,
            .end_addr = ACQ3_END
        },
        {
            .id = __FOFBDECIMIQ_CHAN_ID,
            .start_addr = ACQ3_START,
            .end_addr = ACQ3_END
        },
        {
            .id = __DUMMY2_CHAN_ID,
            .start_addr = ACQ3_START,
            .end_addr = ACQ3_END
        },
        {
            .id = __FOFBAMP_CHAN_ID,
            .start_addr = ACQ3_START,
            .end_addr = ACQ3_END
        },
        {
            .id = __FOFBPHA_CHAN_ID,
            .start_addr = ACQ3_START,
            .end_addr = ACQ3_END
        },
        {
            .id = __FOFBPOS_CHAN_ID,
            .start_addr = ACQ3_START,
            .end_addr = ACQ3_END
        },
        {
            .id = __MONIT1AMP_CHAN_ID,
            .start_addr = ACQ3_START,
            .end_addr = ACQ3_END
        },
        {
            .id = __MONIT1POS_CHAN_ID,
            .start_addr = ACQ3_START,
            .end_addr = ACQ3_END
        },
        {
            .id = __MONITAMP_CHAN_ID,
            .start_addr = ACQ3_START,
            .end_addr = ACQ3_END
        },
        {
            .id = __MONITPOS_CHAN_ID,
            .start_addr = ACQ3_START,
            .end_addr = ACQ3_END
        },
    },
};

/* Acessed as a 1D-array */
HUTILS_EXPORT_SYMBOL(const acq_buf_t *, const_acq_buf_t_p, afcv3_1_acq_buf);
