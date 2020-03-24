#ifndef _ACQ_CHAN_H_
#define _ACQ_CHAN_H_

#include <acq_chan_gen_defs.h>

#define NUM_ACQ_CORE_SMIOS              1

/************************ Acquistion 0 Channel Parameters **************/

/* ADC */
#define ADC_CHAN_ID                     0

/* TBT AMP */
#define TBTAMP_CHAN_ID                  (ADC_CHAN_ID + 1)

/* TBT POS */
#define TBTPOS_CHAN_ID                  (TBTAMP_CHAN_ID + 1)

/* FOFB AMP */
#define FOFBAMP_CHAN_ID                 (TBTPOS_CHAN_ID + 1)

/* FOFB POS */
#define FOFBPOS_CHAN_ID                 (FOFBAMP_CHAN_ID + 1)

/* MONIT AMP */
#define MONITAMP_CHAN_ID                (FOFBPOS_CHAN_ID + 1)

/* MONIT POS */
#define MONITPOS_CHAN_ID                (MONITAMP_CHAN_ID + 1)

/* MONIT1 POS */
#define MONIT1POS_CHAN_ID               (MONITPOS_CHAN_ID + 1)

/* End of channels placeholder */
#define END_CHAN_ID                     (MONIT1POS_CHAN_ID + 1)

#endif

