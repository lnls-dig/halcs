#ifndef _DDR3_MAP_STRUCTS_H_
#define _DDR3_MAP_STRUCTS_H_

#include <stdint.h>

typedef struct {
  uint32_t id;
  uint32_t start_addr;
  uint32_t end_addr;
  uint32_t max_samples;
  uint32_t sample_size;
} acq_buf_t;

#endif

