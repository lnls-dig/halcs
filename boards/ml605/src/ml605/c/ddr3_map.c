/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include <czmq.h>
#include "acq_chan_gen_defs.h"
#include "board.h"
#include "ddr3_map_structs.h"
#include "ddr3_map.h"

const acq_buf_t __acq_buf[NUM_ACQ_CORE_SMIOS][END_CHAN_ID] = {
    /*** Acquistion 0 Channel Parameters ***/
    {
        {
            .id = ADC_CHAN_ID,
            .start_addr = DDR3_ADC0_START_ADDR,
            .end_addr = DDR3_ADC0_END_ADDR,
            .max_samples = DDR3_ADC0_MAX_SAMPLES,
            .sample_size = DDR3_ADC0_SAMPLE_SIZE
        },
        {
            .id = TBTAMP_CHAN_ID,
            .start_addr = DDR3_TBTAMP0_START_ADDR,
            .end_addr = DDR3_TBTAMP0_END_ADDR,
            .max_samples =  DDR3_TBTAMP0_MAX_SAMPLES,
            .sample_size = DDR3_TBTAMP0_SAMPLE_SIZE
        },
        {
            .id = TBTPOS_CHAN_ID,
            .start_addr = DDR3_TBTPOS0_START_ADDR,
            .end_addr = DDR3_TBTPOS0_END_ADDR,
            .max_samples =DDR3_TBTPOS0_MAX_SAMPLES,
            .sample_size = DDR3_TBTPOS0_SAMPLE_SIZE
        },
        {
            .id = FOFBAMP_CHAN_ID,
            .start_addr = DDR3_FOFBAMP0_START_ADDR,
            .end_addr = DDR3_FOFBAMP0_END_ADDR,
            .max_samples =  DDR3_FOFBAMP0_MAX_SAMPLES,
            .sample_size = DDR3_FOFBAMP0_SAMPLE_SIZE
        },
        {
            .id = FOFBPOS_CHAN_ID,
            .start_addr = DDR3_FOFBPOS0_START_ADDR,
            .end_addr = DDR3_FOFBPOS0_END_ADDR,
            .max_samples = DDR3_FOFBPOS0_MAX_SAMPLES,
            .sample_size = DDR3_FOFBPOS0_SAMPLE_SIZE
        },
        {
            .id = MONITAMP_CHAN_ID,
            .start_addr = DDR3_MONITAMP0_START_ADDR,
            .end_addr = DDR3_MONITAMP0_END_ADDR,
            .max_samples = DDR3_MONITAMP0_MAX_SAMPLES,
            .sample_size = DDR3_MONITAMP0_SAMPLE_SIZE
        },
        {
            .id = MONITPOS_CHAN_ID,
            .start_addr = DDR3_MONITPOS0_START_ADDR,
            .end_addr = DDR3_MONITPOS0_END_ADDR,
            .max_samples = DDR3_MONITPOS0_MAX_SAMPLES,
            .sample_size = DDR3_MONITPOS0_SAMPLE_SIZE
        },
        {
            .id = MONIT1POS_CHAN_ID,
            .start_addr = DDR3_MONIT1POS0_START_ADDR,
            .end_addr = DDR3_MONIT1POS0_END_ADDR,
            .max_samples = DDR3_MONIT1POS0_MAX_SAMPLES,
            .sample_size = DDR3_MONIT1POS0_SAMPLE_SIZE
        }
    }
};
