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

/* ADC */
extern const uint32_t ADC_CHAN_ID;

/* TBT AMP */
extern const uint32_t TBTAMP_CHAN_ID;

/* TBT POS */
extern const uint32_t TBTPOS_CHAN_ID;

/* FOFB AMP */
extern const uint32_t FOFBAMP_CHAN_ID;

/* FOFB POS */
extern const uint32_t FOFBPOS_CHAN_ID;

/* MONIT AMP */
extern const uint32_t MONITAMP_CHAN_ID;

/* MONIT POS */
extern const uint32_t MONITPOS_CHAN_ID;

/* MONIT1 POS */
extern const uint32_t MONIT1POS_CHAN_ID;

/* End of channels placeholder */
extern const uint32_t END_CHAN_ID;

/* Channel buffer definition */
extern const acq_buf_t __acq_buf[__NUM_ACQ_CORE_SMIOS][__END_CHAN_ID];

#endif

