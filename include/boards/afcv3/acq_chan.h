#ifndef _ACQ_CHAN_H_
#define _ACQ_CHAN_H_

#include "ddr3_map_structs.h"
#include <acq_chan_gen_defs.h>
#include <stdint.h>

#define __NUM_ACQ_CORE_SMIOS              4

/************************ Acquistion 0 Channel Parameters **************/

typedef enum {
    __ADC_CHAN_ID = 0,
    __ADCSWAP_CHAN_ID = 1,
    __MIXIQ_CHAN_ID,
    __DUMMY0_CHAN_ID,
    __TBTDECIMIQ_CHAN_ID,
    __DUMMY1_CHAN_ID,
    __TBTAMP_CHAN_ID,
    __TBTPHA_CHAN_ID,
    __TBTPOS_CHAN_ID,
    __FOFBDECIMIQ_CHAN_ID,
    __DUMMY2_CHAN_ID,
    __FOFBAMP_CHAN_ID,
    __FOFBPHA_CHAN_ID,
    __FOFBPOS_CHAN_ID,
    __MONIT1AMP_CHAN_ID,
    __MONIT1POS_CHAN_ID,
    __MONITAMP_CHAN_ID,
    __MONITPOS_CHAN_ID,
    __END_CHAN_ID
} acq_channel_ids;

#endif

