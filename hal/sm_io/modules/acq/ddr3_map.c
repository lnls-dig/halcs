#include "ddr3_map.h"

#if defined(__BOARD_ML605__)

const acq_buf_t __acq_buf[END_CHAN_ID] = {
  {
      .id = ADC0_CHAN_ID,
      .start_addr = DDR3_ADC0_START_ADDR,
      .end_addr = DDR3_ADC0_END_ADDR,
      .max_samples = DDR3_ADC0_MAX_SAMPLES,
      .sample_size = DDR3_ADC_SAMPLE_SIZE
  },
  {
      .id = TBTAMP0_CHAN_ID,
      .start_addr = DDR3_TBTAMP0_START_ADDR,
      .end_addr = DDR3_TBTAMP0_END_ADDR,
      .max_samples =  DDR3_TBTAMP0_MAX_SAMPLES,
      .sample_size = DDR3_TBTAMP_SAMPLE_SIZE
  },
  {
      .id = TBTPOS0_CHAN_ID,
      .start_addr = DDR3_TBTPOS0_START_ADDR,
      .end_addr = DDR3_TBTPOS0_END_ADDR,
      .max_samples =DDR3_TBTPOS0_MAX_SAMPLES,
      .sample_size = DDR3_TBTPOS_SAMPLE_SIZE
  },
  {
      .id = FOFBAMP0_CHAN_ID,
      .start_addr = DDR3_FOFBAMP0_START_ADDR,
      .end_addr = DDR3_FOFBAMP0_END_ADDR,
      .max_samples =  DDR3_FOFBAMP0_MAX_SAMPLES,
      .sample_size = DDR3_FOFBAMP_SAMPLE_SIZE
  },
  {
      .id = FOFBPOS0_CHAN_ID,
      .start_addr = DDR3_FOFBPOS0_START_ADDR,
      .end_addr = DDR3_FOFBPOS0_END_ADDR,
      .max_samples = DDR3_FOFBPOS0_MAX_SAMPLES,
      .sample_size = DDR3_FOFBPOS_SAMPLE_SIZE
  }
};

#elif defined(__BOARD_AFCV3__)

const acq_buf_t __acq_buf[END_CHAN_ID] = {
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
      .id = ADC1_CHAN_ID,
      .start_addr = DDR3_ADC1_START_ADDR,
      .end_addr = DDR3_ADC1_END_ADDR,
      .max_samples = DDR3_ADC1_MAX_SAMPLES,
      .sample_size = DDR3_ADC1_SAMPLE_SIZE
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
  }
};

#else

#error "Could not initialize acq_buf. Unsupported board!"

#endif
