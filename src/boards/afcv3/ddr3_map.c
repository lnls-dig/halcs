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

const acq_buf_t __acq_buf[NUM_ACQ_CORE_SMIOS][END_CHAN_ID] = {
    /*** Acquisition Core 0 Channel Parameters ***/
    {
        {
            .id = ADC0_CHAN_ID,
            .start_addr = DDR3_ADC0_START_ADDR,
            .end_addr = DDR3_ADC0_END_ADDR,
            .max_samples = DDR3_ADC0_MAX_SAMPLES,
            .sample_size = DDR3_ADC0_SAMPLE_SIZE
        },
        {
            .id = ADCSWAP0_CHAN_ID,
            .start_addr = DDR3_ADCSWAP0_START_ADDR,
            .end_addr = DDR3_ADCSWAP0_END_ADDR,
            .max_samples = DDR3_ADCSWAP0_MAX_SAMPLES,
            .sample_size = DDR3_ADCSWAP0_SAMPLE_SIZE
        },
        {
            .id = MIXIQ120_CHAN_ID,
            .start_addr = DDR3_MIXIQ120_START_ADDR,
            .end_addr = DDR3_MIXIQ120_END_ADDR,
            .max_samples = DDR3_MIXIQ120_MAX_SAMPLES,
            .sample_size = DDR3_MIXIQ120_SAMPLE_SIZE
        },
        {
            .id = MIXIQ340_CHAN_ID,
            .start_addr = DDR3_MIXIQ340_START_ADDR,
            .end_addr = DDR3_MIXIQ340_END_ADDR,
            .max_samples = DDR3_MIXIQ340_MAX_SAMPLES,
            .sample_size = DDR3_MIXIQ340_SAMPLE_SIZE
        },
        {
            .id = TBTDECIMIQ120_CHAN_ID,
            .start_addr = DDR3_TBTDECIMIQ120_START_ADDR,
            .end_addr = DDR3_TBTDECIMIQ120_END_ADDR,
            .max_samples = DDR3_TBTDECIMIQ120_MAX_SAMPLES,
            .sample_size = DDR3_TBTDECIMIQ120_SAMPLE_SIZE
        },
        {
            .id = TBTDECIMIQ340_CHAN_ID,
            .start_addr = DDR3_TBTDECIMIQ340_START_ADDR,
            .end_addr = DDR3_TBTDECIMIQ340_END_ADDR,
            .max_samples = DDR3_TBTDECIMIQ340_MAX_SAMPLES,
            .sample_size = DDR3_TBTDECIMIQ340_SAMPLE_SIZE
        },
        {
            .id = TBTAMP0_CHAN_ID,
            .start_addr = DDR3_TBTAMP0_START_ADDR,
            .end_addr = DDR3_TBTAMP0_END_ADDR,
            .max_samples = DDR3_TBTAMP0_MAX_SAMPLES,
            .sample_size = DDR3_TBTAMP0_SAMPLE_SIZE
        },
        {
            .id = TBTPHA0_CHAN_ID,
            .start_addr = DDR3_TBTPHA0_START_ADDR,
            .end_addr = DDR3_TBTPHA0_END_ADDR,
            .max_samples = DDR3_TBTPHA0_MAX_SAMPLES,
            .sample_size = DDR3_TBTPHA0_SAMPLE_SIZE
        },
        {
            .id = TBTPOS0_CHAN_ID,
            .start_addr = DDR3_TBTPOS0_START_ADDR,
            .end_addr = DDR3_TBTPOS0_END_ADDR,
            .max_samples = DDR3_TBTPOS0_MAX_SAMPLES,
            .sample_size = DDR3_TBTPOS0_SAMPLE_SIZE
        },
        {
            .id = FOFBDECIMIQ120_CHAN_ID,
            .start_addr = DDR3_FOFBDECIMIQ120_START_ADDR,
            .end_addr = DDR3_FOFBDECIMIQ120_END_ADDR,
            .max_samples = DDR3_FOFBDECIMIQ120_MAX_SAMPLES,
            .sample_size = DDR3_FOFBDECIMIQ120_SAMPLE_SIZE
        },
        {
            .id = FOFBDECIMIQ340_CHAN_ID,
            .start_addr = DDR3_FOFBDECIMIQ340_START_ADDR,
            .end_addr = DDR3_FOFBDECIMIQ340_END_ADDR,
            .max_samples = DDR3_FOFBDECIMIQ340_MAX_SAMPLES,
            .sample_size = DDR3_FOFBDECIMIQ340_SAMPLE_SIZE
        },
        {
            .id = FOFBAMP0_CHAN_ID,
            .start_addr = DDR3_FOFBAMP0_START_ADDR,
            .end_addr = DDR3_FOFBAMP0_END_ADDR,
            .max_samples = DDR3_FOFBAMP0_MAX_SAMPLES,
            .sample_size = DDR3_FOFBAMP0_SAMPLE_SIZE
        },
        {
            .id = FOFBPHA0_CHAN_ID,
            .start_addr = DDR3_FOFBPHA0_START_ADDR,
            .end_addr = DDR3_FOFBPHA0_END_ADDR,
            .max_samples = DDR3_FOFBPHA0_MAX_SAMPLES,
            .sample_size = DDR3_FOFBPHA0_SAMPLE_SIZE
        },
        {
            .id = FOFBPOS0_CHAN_ID,
            .start_addr = DDR3_FOFBPOS0_START_ADDR,
            .end_addr = DDR3_FOFBPOS0_END_ADDR,
            .max_samples = DDR3_FOFBPOS0_MAX_SAMPLES,
            .sample_size = DDR3_FOFBPOS0_SAMPLE_SIZE
        },
        {
            .id = MONITAMP0_CHAN_ID,
            .start_addr = DDR3_MONITAMP0_START_ADDR,
            .end_addr = DDR3_MONITAMP0_END_ADDR,
            .max_samples = DDR3_MONITAMP0_MAX_SAMPLES,
            .sample_size = DDR3_MONITAMP0_SAMPLE_SIZE
        },
        {
            .id = MONITPOS0_CHAN_ID,
            .start_addr = DDR3_MONITPOS0_START_ADDR,
            .end_addr = DDR3_MONITPOS0_END_ADDR,
            .max_samples = DDR3_MONITPOS0_MAX_SAMPLES,
            .sample_size = DDR3_MONITPOS0_SAMPLE_SIZE
        },
        {
            .id = MONIT1POS0_CHAN_ID,
            .start_addr = DDR3_MONIT1POS0_START_ADDR,
            .end_addr = DDR3_MONIT1POS0_END_ADDR,
            .max_samples = DDR3_MONIT1POS0_MAX_SAMPLES,
            .sample_size = DDR3_MONIT1POS0_SAMPLE_SIZE
        }
    },
    /*** Acquisition Core 1 Channel Parameters ***/
    {
        {
            .id = ADC0_CHAN_ID,
            .start_addr = DDR3_ADC1_START_ADDR,
            .end_addr = DDR3_ADC1_END_ADDR,
            .max_samples = DDR3_ADC1_MAX_SAMPLES,
            .sample_size = DDR3_ADC0_SAMPLE_SIZE
        },
        {
            .id = ADCSWAP0_CHAN_ID,
            .start_addr = DDR3_ADCSWAP1_START_ADDR,
            .end_addr = DDR3_ADCSWAP1_END_ADDR,
            .max_samples = DDR3_ADCSWAP1_MAX_SAMPLES,
            .sample_size = DDR3_ADCSWAP0_SAMPLE_SIZE
        },
        {
            .id = MIXIQ120_CHAN_ID,
            .start_addr = DDR3_MIXIQ121_START_ADDR,
            .end_addr = DDR3_MIXIQ121_END_ADDR,
            .max_samples = DDR3_MIXIQ121_MAX_SAMPLES,
            .sample_size = DDR3_MIXIQ120_SAMPLE_SIZE
        },
        {
            .id = MIXIQ340_CHAN_ID,
            .start_addr = DDR3_MIXIQ341_START_ADDR,
            .end_addr = DDR3_MIXIQ341_END_ADDR,
            .max_samples = DDR3_MIXIQ341_MAX_SAMPLES,
            .sample_size = DDR3_MIXIQ340_SAMPLE_SIZE
        },
        {
            .id = TBTDECIMIQ120_CHAN_ID,
            .start_addr = DDR3_TBTDECIMIQ121_START_ADDR,
            .end_addr = DDR3_TBTDECIMIQ121_END_ADDR,
            .max_samples = DDR3_TBTDECIMIQ121_MAX_SAMPLES,
            .sample_size = DDR3_TBTDECIMIQ120_SAMPLE_SIZE
        },
        {
            .id = TBTDECIMIQ340_CHAN_ID,
            .start_addr = DDR3_TBTDECIMIQ341_START_ADDR,
            .end_addr = DDR3_TBTDECIMIQ341_END_ADDR,
            .max_samples = DDR3_TBTDECIMIQ341_MAX_SAMPLES,
            .sample_size = DDR3_TBTDECIMIQ340_SAMPLE_SIZE
        },
        {
            .id = TBTAMP0_CHAN_ID,
            .start_addr = DDR3_TBTAMP1_START_ADDR,
            .end_addr = DDR3_TBTAMP1_END_ADDR,
            .max_samples = DDR3_TBTAMP1_MAX_SAMPLES,
            .sample_size = DDR3_TBTAMP0_SAMPLE_SIZE
        },
        {
            .id = TBTPHA0_CHAN_ID,
            .start_addr = DDR3_TBTPHA1_START_ADDR,
            .end_addr = DDR3_TBTPHA1_END_ADDR,
            .max_samples = DDR3_TBTPHA1_MAX_SAMPLES,
            .sample_size = DDR3_TBTPHA0_SAMPLE_SIZE
        },
        {
            .id = TBTPOS0_CHAN_ID,
            .start_addr = DDR3_TBTPOS1_START_ADDR,
            .end_addr = DDR3_TBTPOS1_END_ADDR,
            .max_samples = DDR3_TBTPOS1_MAX_SAMPLES,
            .sample_size = DDR3_TBTPOS0_SAMPLE_SIZE
        },
        {
            .id = FOFBDECIMIQ120_CHAN_ID,
            .start_addr = DDR3_FOFBDECIMIQ121_START_ADDR,
            .end_addr = DDR3_FOFBDECIMIQ121_END_ADDR,
            .max_samples = DDR3_FOFBDECIMIQ121_MAX_SAMPLES,
            .sample_size = DDR3_FOFBDECIMIQ120_SAMPLE_SIZE
        },
        {
            .id = FOFBDECIMIQ340_CHAN_ID,
            .start_addr = DDR3_FOFBDECIMIQ341_START_ADDR,
            .end_addr = DDR3_FOFBDECIMIQ341_END_ADDR,
            .max_samples = DDR3_FOFBDECIMIQ341_MAX_SAMPLES,
            .sample_size = DDR3_FOFBDECIMIQ340_SAMPLE_SIZE
        },
        {
            .id = FOFBAMP0_CHAN_ID,
            .start_addr = DDR3_FOFBAMP1_START_ADDR,
            .end_addr = DDR3_FOFBAMP1_END_ADDR,
            .max_samples = DDR3_FOFBAMP1_MAX_SAMPLES,
            .sample_size = DDR3_FOFBAMP0_SAMPLE_SIZE
        },
        {
            .id = FOFBPHA0_CHAN_ID,
            .start_addr = DDR3_FOFBPHA1_START_ADDR,
            .end_addr = DDR3_FOFBPHA1_END_ADDR,
            .max_samples = DDR3_FOFBPHA1_MAX_SAMPLES,
            .sample_size = DDR3_FOFBPHA0_SAMPLE_SIZE
        },
        {
            .id = FOFBPOS0_CHAN_ID,
            .start_addr = DDR3_FOFBPOS1_START_ADDR,
            .end_addr = DDR3_FOFBPOS1_END_ADDR,
            .max_samples = DDR3_FOFBPOS1_MAX_SAMPLES,
            .sample_size = DDR3_FOFBPOS0_SAMPLE_SIZE
        },
        {
            .id = MONITAMP0_CHAN_ID,
            .start_addr = DDR3_MONITAMP1_START_ADDR,
            .end_addr = DDR3_MONITAMP1_END_ADDR,
            .max_samples = DDR3_MONITAMP1_MAX_SAMPLES,
            .sample_size = DDR3_MONITAMP0_SAMPLE_SIZE
        },
        {
            .id = MONITPOS0_CHAN_ID,
            .start_addr = DDR3_MONITPOS1_START_ADDR,
            .end_addr = DDR3_MONITPOS1_END_ADDR,
            .max_samples = DDR3_MONITPOS1_MAX_SAMPLES,
            .sample_size = DDR3_MONITPOS0_SAMPLE_SIZE
        },
        {
            .id = MONIT1POS0_CHAN_ID,
            .start_addr = DDR3_MONIT1POS1_START_ADDR,
            .end_addr = DDR3_MONIT1POS1_END_ADDR,
            .max_samples = DDR3_MONIT1POS1_MAX_SAMPLES,
            .sample_size = DDR3_MONIT1POS0_SAMPLE_SIZE
        },
    }
};
