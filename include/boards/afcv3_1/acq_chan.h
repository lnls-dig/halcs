#ifndef _ACQ_CHAN_H_
#define _ACQ_CHAN_H_

#include <acq_chan_gen_defs.h>

#define NUM_ACQ_CORE_SMIOS              4

/************************ Acquistion 0 Channel Parameters **************/

/* ADC */
#define ADC_CHAN_ID                     0

/* ADC SWAPPED (after the switching module) */
#define ADCSWAP_CHAN_ID                 (ADC_CHAN_ID + 1)

/* MIXER I/Q */
#define MIXIQ_CHAN_ID                   (ADCSWAP_CHAN_ID + 1)

/* DUMMY 0 */
#define DUMMY0_CHAN_ID                  (MIXIQ_CHAN_ID + 1)

/* TBTDECIM I/Q */
#define TBTDECIMIQ_CHAN_ID              (DUMMY0_CHAN_ID + 1)

/* DUMMY 1 */
#define DUMMY1_CHAN_ID                  (TBTDECIMIQ_CHAN_ID + 1)

/* TBT AMP */
#define TBTAMP_CHAN_ID                  (DUMMY1_CHAN_ID + 1)

/* TBT PHASE */
#define TBTPHA_CHAN_ID                  (TBTAMP_CHAN_ID + 1)

/* TBT POS */
#define TBTPOS_CHAN_ID                  (TBTPHA_CHAN_ID + 1)

/* FOFBDECIM I/Q */
#define FOFBDECIMIQ_CHAN_ID             (TBTPOS_CHAN_ID + 1)

/* DUMMY 2 */
#define DUMMY2_CHAN_ID                  (FOFBDECIMIQ_CHAN_ID + 1)

/* FOFB AMP */
#define FOFBAMP_CHAN_ID                 (DUMMY2_CHAN_ID + 1)

/* FOFB PHA */
#define FOFBPHA_CHAN_ID                 (FOFBAMP_CHAN_ID + 1)

/* FOFB POS */
#define FOFBPOS_CHAN_ID                 (FOFBPHA_CHAN_ID + 1)

/* MONIT1 AMP */
#define MONIT1AMP_CHAN_ID               (FOFBPOS_CHAN_ID + 1)

/* MONIT1 POS */
#define MONIT1POS_CHAN_ID               (MONIT1AMP_CHAN_ID + 1)

/* MONIT AMP */
#define MONITAMP_CHAN_ID                (MONIT1POS_CHAN_ID + 1)

/* MONIT POS */
#define MONITPOS_CHAN_ID                (MONITAMP_CHAN_ID + 1)

/* End of channels placeholder */
#define END_CHAN_ID                     (MONITPOS_CHAN_ID + 1)

#endif

