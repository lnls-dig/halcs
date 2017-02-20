/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include <czmq.h>
#include "acq_chan_gen_defs.h"
#include "board.h"
#include "ddr3_map.h"
#include "ddr3_map_structs.h"

#ifdef __BOARD_AFCV3_1__

const acq_buf_t __acq_buf[NUM_ACQ_CORE_SMIOS][END_CHAN_ID] = {
    /*** Acquisition Core 0 Channel Parameters ***/
    {
        {
            .id = ADC_CHAN_ID,
            .start_addr = DDR3_ADC0_START_ADDR,
            .end_addr = DDR3_ADC0_END_ADDR,
            .max_samples = DDR3_ADC0_MAX_SAMPLES,
            .sample_size = DDR3_ADC0_SAMPLE_SIZE
        },
        {
            .id = ADCSWAP_CHAN_ID,
            .start_addr = DDR3_ADCSWAP0_START_ADDR,
            .end_addr = DDR3_ADCSWAP0_END_ADDR,
            .max_samples = DDR3_ADCSWAP0_MAX_SAMPLES,
            .sample_size = DDR3_ADCSWAP0_SAMPLE_SIZE
        },
        {
            .id = MIXIQ_CHAN_ID,
            .start_addr = DDR3_MIXIQ0_START_ADDR,
            .end_addr = DDR3_MIXIQ0_END_ADDR,
            .max_samples = DDR3_MIXIQ0_MAX_SAMPLES,
            .sample_size = DDR3_MIXIQ0_SAMPLE_SIZE
        },
        {
            .id = DUMMY0_CHAN_ID,
            .start_addr = DDR3_DUMMY00_START_ADDR,
            .end_addr = DDR3_DUMMY00_END_ADDR,
            .max_samples = DDR3_DUMMY00_MAX_SAMPLES,
            .sample_size = DDR3_DUMMY00_SAMPLE_SIZE
        },
        {
            .id = TBTDECIMIQ_CHAN_ID,
            .start_addr = DDR3_TBTDECIMIQ0_START_ADDR,
            .end_addr = DDR3_TBTDECIMIQ0_END_ADDR,
            .max_samples = DDR3_TBTDECIMIQ0_MAX_SAMPLES,
            .sample_size = DDR3_TBTDECIMIQ0_SAMPLE_SIZE
        },
        {
            .id = DUMMY1_CHAN_ID,
            .start_addr = DDR3_DUMMY10_START_ADDR,
            .end_addr = DDR3_DUMMY10_END_ADDR,
            .max_samples = DDR3_DUMMY10_MAX_SAMPLES,
            .sample_size = DDR3_DUMMY10_SAMPLE_SIZE
        },
        {
            .id = TBTAMP_CHAN_ID,
            .start_addr = DDR3_TBTAMP0_START_ADDR,
            .end_addr = DDR3_TBTAMP0_END_ADDR,
            .max_samples = DDR3_TBTAMP0_MAX_SAMPLES,
            .sample_size = DDR3_TBTAMP0_SAMPLE_SIZE
        },
        {
            .id = TBTPHA_CHAN_ID,
            .start_addr = DDR3_TBTPHA0_START_ADDR,
            .end_addr = DDR3_TBTPHA0_END_ADDR,
            .max_samples = DDR3_TBTPHA0_MAX_SAMPLES,
            .sample_size = DDR3_TBTPHA0_SAMPLE_SIZE
        },
        {
            .id = TBTPOS_CHAN_ID,
            .start_addr = DDR3_TBTPOS0_START_ADDR,
            .end_addr = DDR3_TBTPOS0_END_ADDR,
            .max_samples = DDR3_TBTPOS0_MAX_SAMPLES,
            .sample_size = DDR3_TBTPOS0_SAMPLE_SIZE
        },
        {
            .id = FOFBDECIMIQ_CHAN_ID,
            .start_addr = DDR3_FOFBDECIMIQ0_START_ADDR,
            .end_addr = DDR3_FOFBDECIMIQ0_END_ADDR,
            .max_samples = DDR3_FOFBDECIMIQ0_MAX_SAMPLES,
            .sample_size = DDR3_FOFBDECIMIQ0_SAMPLE_SIZE
        },
        {
            .id = DUMMY2_CHAN_ID,
            .start_addr = DDR3_DUMMY20_START_ADDR,
            .end_addr = DDR3_DUMMY20_END_ADDR,
            .max_samples = DDR3_DUMMY20_MAX_SAMPLES,
            .sample_size = DDR3_DUMMY20_SAMPLE_SIZE
        },
        {
            .id = FOFBAMP_CHAN_ID,
            .start_addr = DDR3_FOFBAMP0_START_ADDR,
            .end_addr = DDR3_FOFBAMP0_END_ADDR,
            .max_samples = DDR3_FOFBAMP0_MAX_SAMPLES,
            .sample_size = DDR3_FOFBAMP0_SAMPLE_SIZE
        },
        {
            .id = FOFBPHA_CHAN_ID,
            .start_addr = DDR3_FOFBPHA0_START_ADDR,
            .end_addr = DDR3_FOFBPHA0_END_ADDR,
            .max_samples = DDR3_FOFBPHA0_MAX_SAMPLES,
            .sample_size = DDR3_FOFBPHA0_SAMPLE_SIZE
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
    },
    /*** Acquisition Core 1 Channel Parameters ***/
    {
        {
            .id = ADC_CHAN_ID,
            .start_addr = DDR3_ADC1_START_ADDR,
            .end_addr = DDR3_ADC1_END_ADDR,
            .max_samples = DDR3_ADC1_MAX_SAMPLES,
            .sample_size = DDR3_ADC1_SAMPLE_SIZE
        },
        {
            .id = ADCSWAP_CHAN_ID,
            .start_addr = DDR3_ADCSWAP1_START_ADDR,
            .end_addr = DDR3_ADCSWAP1_END_ADDR,
            .max_samples = DDR3_ADCSWAP1_MAX_SAMPLES,
            .sample_size = DDR3_ADCSWAP1_SAMPLE_SIZE
        },
        {
            .id = MIXIQ_CHAN_ID,
            .start_addr = DDR3_MIXIQ1_START_ADDR,
            .end_addr = DDR3_MIXIQ1_END_ADDR,
            .max_samples = DDR3_MIXIQ1_MAX_SAMPLES,
            .sample_size = DDR3_MIXIQ1_SAMPLE_SIZE
        },
        {
            .id = DUMMY0_CHAN_ID,
            .start_addr = DDR3_DUMMY01_START_ADDR,
            .end_addr = DDR3_DUMMY01_END_ADDR,
            .max_samples = DDR3_DUMMY01_MAX_SAMPLES,
            .sample_size = DDR3_DUMMY01_SAMPLE_SIZE
        },
        {
            .id = TBTDECIMIQ_CHAN_ID,
            .start_addr = DDR3_TBTDECIMIQ1_START_ADDR,
            .end_addr = DDR3_TBTDECIMIQ1_END_ADDR,
            .max_samples = DDR3_TBTDECIMIQ1_MAX_SAMPLES,
            .sample_size = DDR3_TBTDECIMIQ1_SAMPLE_SIZE
        },
        {
            .id = DUMMY1_CHAN_ID,
            .start_addr = DDR3_DUMMY11_START_ADDR,
            .end_addr = DDR3_DUMMY11_END_ADDR,
            .max_samples = DDR3_DUMMY11_MAX_SAMPLES,
            .sample_size = DDR3_DUMMY11_SAMPLE_SIZE
        },
        {
            .id = TBTAMP_CHAN_ID,
            .start_addr = DDR3_TBTAMP1_START_ADDR,
            .end_addr = DDR3_TBTAMP1_END_ADDR,
            .max_samples = DDR3_TBTAMP1_MAX_SAMPLES,
            .sample_size = DDR3_TBTAMP1_SAMPLE_SIZE
        },
        {
            .id = TBTPHA_CHAN_ID,
            .start_addr = DDR3_TBTPHA1_START_ADDR,
            .end_addr = DDR3_TBTPHA1_END_ADDR,
            .max_samples = DDR3_TBTPHA1_MAX_SAMPLES,
            .sample_size = DDR3_TBTPHA1_SAMPLE_SIZE
        },
        {
            .id = TBTPOS_CHAN_ID,
            .start_addr = DDR3_TBTPOS1_START_ADDR,
            .end_addr = DDR3_TBTPOS1_END_ADDR,
            .max_samples = DDR3_TBTPOS1_MAX_SAMPLES,
            .sample_size = DDR3_TBTPOS1_SAMPLE_SIZE
        },
        {
            .id = FOFBDECIMIQ_CHAN_ID,
            .start_addr = DDR3_FOFBDECIMIQ1_START_ADDR,
            .end_addr = DDR3_FOFBDECIMIQ1_END_ADDR,
            .max_samples = DDR3_FOFBDECIMIQ1_MAX_SAMPLES,
            .sample_size = DDR3_FOFBDECIMIQ1_SAMPLE_SIZE
        },
        {
            .id = DUMMY2_CHAN_ID,
            .start_addr = DDR3_DUMMY21_START_ADDR,
            .end_addr = DDR3_DUMMY21_END_ADDR,
            .max_samples = DDR3_DUMMY21_MAX_SAMPLES,
            .sample_size = DDR3_DUMMY21_SAMPLE_SIZE
        },
        {
            .id = FOFBAMP_CHAN_ID,
            .start_addr = DDR3_FOFBAMP1_START_ADDR,
            .end_addr = DDR3_FOFBAMP1_END_ADDR,
            .max_samples = DDR3_FOFBAMP1_MAX_SAMPLES,
            .sample_size = DDR3_FOFBAMP1_SAMPLE_SIZE
        },
        {
            .id = FOFBPHA_CHAN_ID,
            .start_addr = DDR3_FOFBPHA1_START_ADDR,
            .end_addr = DDR3_FOFBPHA1_END_ADDR,
            .max_samples = DDR3_FOFBPHA1_MAX_SAMPLES,
            .sample_size = DDR3_FOFBPHA1_SAMPLE_SIZE
        },
        {
            .id = FOFBPOS_CHAN_ID,
            .start_addr = DDR3_FOFBPOS1_START_ADDR,
            .end_addr = DDR3_FOFBPOS1_END_ADDR,
            .max_samples = DDR3_FOFBPOS1_MAX_SAMPLES,
            .sample_size = DDR3_FOFBPOS1_SAMPLE_SIZE
        },
        {
            .id = MONITAMP_CHAN_ID,
            .start_addr = DDR3_MONITAMP1_START_ADDR,
            .end_addr = DDR3_MONITAMP1_END_ADDR,
            .max_samples = DDR3_MONITAMP1_MAX_SAMPLES,
            .sample_size = DDR3_MONITAMP1_SAMPLE_SIZE
        },
        {
            .id = MONITPOS_CHAN_ID,
            .start_addr = DDR3_MONITPOS1_START_ADDR,
            .end_addr = DDR3_MONITPOS1_END_ADDR,
            .max_samples = DDR3_MONITPOS1_MAX_SAMPLES,
            .sample_size = DDR3_MONITPOS1_SAMPLE_SIZE
        },
        {
            .id = MONIT1POS_CHAN_ID,
            .start_addr = DDR3_MONIT1POS1_START_ADDR,
            .end_addr = DDR3_MONIT1POS1_END_ADDR,
            .max_samples = DDR3_MONIT1POS1_MAX_SAMPLES,
            .sample_size = DDR3_MONIT1POS1_SAMPLE_SIZE
        },
    },
    /*** Acquisition Core 2 Channel Parameters ***/
    {
        {
            .id = ADC_CHAN_ID,
            .start_addr = DDR3_ADC2_START_ADDR,
            .end_addr = DDR3_ADC2_END_ADDR,
            .max_samples = DDR3_ADC2_MAX_SAMPLES,
            .sample_size = DDR3_ADC2_SAMPLE_SIZE
        }
    },
    /*** Acquisition Core 3 Channel Parameters ***/
    {
        {
            .id = ADC_CHAN_ID,
            .start_addr = DDR3_ADC3_START_ADDR,
            .end_addr = DDR3_ADC3_END_ADDR,
            .max_samples = DDR3_ADC3_MAX_SAMPLES,
            .sample_size = DDR3_ADC3_SAMPLE_SIZE
        }
    },
};

#endif
