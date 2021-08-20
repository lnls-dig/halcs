#ifndef _ACQ_CHAN_H_
#define _ACQ_CHAN_H_

#include "ddr3_map_structs.h"
#include <acq_chan_gen_defs.h>
#include <stdint.h>

#define __NUM_ACQ_CORE_SMIOS              1

/************************ Acquistion 0 Channel Parameters **************/

typedef enum {
    __ADC_CHAN_ID = 0,
    __TBTAMP_CHAN_ID,
    __TBTPOS_CHAN_ID,
    __FOFBAMP_CHAN_ID,
    __FOFBPOS_CHAN_ID,
    __MONITAMP_CHAN_ID,
    __MONITPOS_CHAN_ID,
    __MONIT1POS_CHAN_ID,
    __END_CHAN_ID
} acq_channel_ids;

#endif
