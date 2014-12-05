#ifndef _DDR3_MAP_H_
#define _DDR3_MAP_H_

#include <inttypes.h>
#include "board.h"

struct _acq_buf_t {
  uint32_t id;
  uint32_t start_addr;
  uint32_t end_addr;
  uint32_t max_samples;
  uint32_t sample_size;
};

typedef struct _acq_buf_t acq_buf_t;

extern const acq_buf_t __acq_buf[NUM_ACQ_CORE_SMIOS][END_CHAN_ID];

#endif

