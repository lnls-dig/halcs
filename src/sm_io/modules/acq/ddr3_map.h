#ifndef _DDR3_MAP_H_
#define _DDR3_MAP_H_

#include <inttypes.h>
#include "board.h"

struct _acq_buf_t {
    uint32_t id;                        /* Channel ID */
    uint32_t start_addr;                /* Channel start address */
    uint32_t end_addr;                  /* Channel end address */
    uint32_t max_samples;               /* Maximum number of channels */
    acq_chan_desc_t chan_desc;          /* Channel descriptor */
};

typedef struct _acq_buf_t acq_buf_t;

extern const acq_buf_t __acq_buf[NUM_ACQ_CORE_SMIOS][END_CHAN_ID];

#endif

