#ifndef _DDR3_MAP_H_
#define _DDR3_MAP_H_

typedef struct {
  uint32_t id;
  uint32_t start_addr;
  uint32_t end_addr;
  uint32_t max_samples;
  uint32_t sample_size;
} acq_buf_t;

extern const acq_buf_t __acq_buf[NUM_ACQ_CORE_SMIOS][END_CHAN_ID];

#endif

