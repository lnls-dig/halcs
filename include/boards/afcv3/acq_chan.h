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

/* ADC */
extern const uint32_t ADC_CHAN_ID;

/* ADC SWAPPED (after the switching module) */
extern const uint32_t ADCSWAP_CHAN_ID;

/* MIXER I/Q */
extern const uint32_t MIXIQ_CHAN_ID;

/* DUMMY 0 */
extern const uint32_t DUMMY0_CHAN_ID;

/* TBTDECIM I/Q */
extern const uint32_t TBTDECIMIQ_CHAN_ID;

/* DUMMY 1 */
extern const uint32_t DUMMY1_CHAN_ID;

/* TBT AMP */
extern const uint32_t TBTAMP_CHAN_ID;

/* TBT PHASE */
extern const uint32_t TBTPHA_CHAN_ID;

/* TBT POS */
extern const uint32_t TBTPOS_CHAN_ID;

/* FOFBDECIM I/Q */
extern const uint32_t FOFBDECIMIQ_CHAN_ID;

/* DUMMY 2 */
extern const uint32_t DUMMY2_CHAN_ID;

/* FOFB AMP */
extern const uint32_t FOFBAMP_CHAN_ID;

/* FOFB PHA */
extern const uint32_t FOFBPHA_CHAN_ID;

/* FOFB POS */
extern const uint32_t FOFBPOS_CHAN_ID;

/* MONIT1 AMP */
extern const uint32_t MONIT1AMP_CHAN_ID;

/* MONIT1 POS */
extern const uint32_t MONIT1POS_CHAN_ID;

/* MONIT AMP */
extern const uint32_t MONITAMP_CHAN_ID;

/* MONIT POS */
extern const uint32_t MONITPOS_CHAN_ID;

/* End of channels placeholder */
extern const uint32_t END_CHAN_ID;

/* Channel buffer definition */
extern const acq_buf_t __acq_buf[__NUM_ACQ_CORE_SMIOS][__END_CHAN_ID];

#endif

