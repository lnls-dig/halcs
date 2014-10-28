#include "ddr3_map.h"

#if defined(__BOARD_ML605__)

const acq_buf_t __acq_buf[END_CHAN_ID] = {
    /*** Acquistion 0 Channel Parameters ***/
    {
        .id = ADC0_CHAN_ID,
        .start_addr = DDR3_ADC0_START_ADDR,
        .end_addr = DDR3_ADC0_END_ADDR,
        .max_samples = DDR3_ADC0_MAX_SAMPLES,
        .sample_size = DDR3_ADC0_SAMPLE_SIZE
    },
    {
        .id = TBTAMP0_CHAN_ID,
        .start_addr = DDR3_TBTAMP0_START_ADDR,
        .end_addr = DDR3_TBTAMP0_END_ADDR,
        .max_samples =  DDR3_TBTAMP0_MAX_SAMPLES,
        .sample_size = DDR3_TBTAMP0_SAMPLE_SIZE
    },
    {
        .id = TBTPOS0_CHAN_ID,
        .start_addr = DDR3_TBTPOS0_START_ADDR,
        .end_addr = DDR3_TBTPOS0_END_ADDR,
        .max_samples =DDR3_TBTPOS0_MAX_SAMPLES,
        .sample_size = DDR3_TBTPOS0_SAMPLE_SIZE
    },
    {
        .id = FOFBAMP0_CHAN_ID,
        .start_addr = DDR3_FOFBAMP0_START_ADDR,
        .end_addr = DDR3_FOFBAMP0_END_ADDR,
        .max_samples =  DDR3_FOFBAMP0_MAX_SAMPLES,
        .sample_size = DDR3_FOFBAMP0_SAMPLE_SIZE
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
    },
};

#elif defined(__BOARD_AFCV3__)

const acq_buf_t __acq_buf[END_CHAN_ID] = {
    /*** Acquistion 0 Channel Parameters ***/
    {
        .id = ADC0_CHAN_ID,
        .start_addr = DDR3_ADC0_START_ADDR,
        .end_addr = DDR3_ADC0_END_ADDR,
        .max_samples = DDR3_ADC0_MAX_SAMPLES,
        .sample_size = DDR3_ADC0_SAMPLE_SIZE
    },
    {
        .id = MIX0_CHAN_ID,
        .start_addr = DDR3_MIX0_START_ADDR,
        .end_addr = DDR3_MIX0_END_ADDR,
        .max_samples = DDR3_MIX0_MAX_SAMPLES,
        .sample_size = DDR3_MIX0_SAMPLE_SIZE
    },
    {
        .id = TBTAMP0_CHAN_ID,
        .start_addr = DDR3_TBTAMP0_START_ADDR,
        .end_addr = DDR3_TBTAMP0_END_ADDR,
        .max_samples = DDR3_TBTAMP0_MAX_SAMPLES,
        .sample_size = DDR3_TBTAMP0_SAMPLE_SIZE
    },
    {
        .id = TBTPOS0_CHAN_ID,
        .start_addr = DDR3_TBTPOS0_START_ADDR,
        .end_addr = DDR3_TBTPOS0_END_ADDR,
        .max_samples = DDR3_TBTPOS0_MAX_SAMPLES,
        .sample_size = DDR3_TBTPOS0_SAMPLE_SIZE
    },
    {
        .id = FOFBAMP0_CHAN_ID,
        .start_addr = DDR3_FOFBAMP0_START_ADDR,
        .end_addr = DDR3_FOFBAMP0_END_ADDR,
        .max_samples = DDR3_FOFBAMP0_MAX_SAMPLES,
        .sample_size = DDR3_FOFBAMP0_SAMPLE_SIZE
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
    },
    /*** Acquistion 1 Channel Parameters ***/
    {
        .id = ADC1_CHAN_ID,
        .start_addr = DDR3_ADC1_START_ADDR,
        .end_addr = DDR3_ADC1_END_ADDR,
        .max_samples = DDR3_ADC1_MAX_SAMPLES,
        .sample_size = DDR3_ADC1_SAMPLE_SIZE
    },
    {
        .id = MIX1_CHAN_ID,
        .start_addr = DDR3_MIX1_START_ADDR,
        .end_addr = DDR3_MIX1_END_ADDR,
        .max_samples = DDR3_MIX1_MAX_SAMPLES,
        .sample_size = DDR3_MIX1_SAMPLE_SIZE
    },
    {
        .id = TBTAMP1_CHAN_ID,
        .start_addr = DDR3_TBTAMP1_START_ADDR,
        .end_addr = DDR3_TBTAMP1_END_ADDR,
        .max_samples = DDR3_TBTAMP1_MAX_SAMPLES,
        .sample_size = DDR3_TBTAMP1_SAMPLE_SIZE
    },
    {
        .id = TBTPOS1_CHAN_ID,
        .start_addr = DDR3_TBTPOS1_START_ADDR,
        .end_addr = DDR3_TBTPOS1_END_ADDR,
        .max_samples = DDR3_TBTPOS1_MAX_SAMPLES,
        .sample_size = DDR3_TBTPOS1_SAMPLE_SIZE
    },
    {
        .id = FOFBAMP1_CHAN_ID,
        .start_addr = DDR3_FOFBAMP1_START_ADDR,
        .end_addr = DDR3_FOFBAMP1_END_ADDR,
        .max_samples = DDR3_FOFBAMP1_MAX_SAMPLES,
        .sample_size = DDR3_FOFBAMP1_SAMPLE_SIZE
    },
    {
        .id = FOFBPOS1_CHAN_ID,
        .start_addr = DDR3_FOFBPOS1_START_ADDR,
        .end_addr = DDR3_FOFBPOS1_END_ADDR,
        .max_samples = DDR3_FOFBPOS1_MAX_SAMPLES,
        .sample_size = DDR3_FOFBPOS1_SAMPLE_SIZE
    },
    {
        .id = MONITAMP1_CHAN_ID,
        .start_addr = DDR3_MONITAMP1_START_ADDR,
        .end_addr = DDR3_MONITAMP1_END_ADDR,
        .max_samples = DDR3_MONITAMP1_MAX_SAMPLES,
        .sample_size = DDR3_MONITAMP1_SAMPLE_SIZE
    },
    {
        .id = MONITPOS1_CHAN_ID,
        .start_addr = DDR3_MONITPOS1_START_ADDR,
        .end_addr = DDR3_MONITPOS1_END_ADDR,
        .max_samples = DDR3_MONITPOS1_MAX_SAMPLES,
        .sample_size = DDR3_MONITPOS1_SAMPLE_SIZE
    },
    {
        .id = MONIT1POS1_CHAN_ID,
        .start_addr = DDR3_MONIT1POS1_START_ADDR,
        .end_addr = DDR3_MONIT1POS1_END_ADDR,
        .max_samples = DDR3_MONIT1POS1_MAX_SAMPLES,
        .sample_size = DDR3_MONIT1POS1_SAMPLE_SIZE
    },
};

#else

#error "Could not initialize acq_buf. Unsupported board!"

#endif
