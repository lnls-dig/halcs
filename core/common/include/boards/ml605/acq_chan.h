#ifndef _ACQ_CHAN_H_
#define _ACQ_CHAN_H_

#include <acq_chan_gen_defs.h>

#define NUM_ACQ_CORE_SMIOS              1

/************************ Acquistion 0 Channel Parameters **************/

/* ADC */
#define ADC_CHAN_ID                     0
#define ADC_SAMPLE_SIZE                 8 /* 8 Bytes -> ADC0 = 16-bit / ADC1 = 16-bit ... */

/* TBT AMP */
#define TBTAMP_CHAN_ID                  (ADC_CHAN_ID + 1)
#define TBTAMP_SAMPLE_SIZE              16 /* 16 Bytes -> TBTAMP0 = 32-bit / TBTAMP1 = 32-bit ... */

/* TBT POS */
#define TBTPOS_CHAN_ID                  (TBTAMP_CHAN_ID + 1)
#define TBTPOS_SAMPLE_SIZE              16 /* 16 Bytes -> X = 32-bit / Y = 32-bit ... */

/* FOFB AMP */
#define FOFBAMP_CHAN_ID                 (TBTPOS_CHAN_ID + 1)
#define FOFBAMP_SAMPLE_SIZE             16 /* 16 Bytes -> FOFBAMP0 = 32-bit / FOFBAMP1 = 32-bit ... */

/* FOFB POS */
#define FOFBPOS_CHAN_ID                 (FOFBAMP_CHAN_ID + 1)
#define FOFBPOS_SAMPLE_SIZE             16  /* 16 Bytes -> X = 32-bit / Y = 32-bit ... */

/* MONIT AMP */
#define MONITAMP_CHAN_ID                (FOFBPOS_CHAN_ID + 1)
#define MONITAMP_SAMPLE_SIZE            16 /* 16 Bytes -> FOFBAMP0 = 32-bit / FOFBAMP1 = 32-bit ... */

/* MONIT POS */
#define MONITPOS_CHAN_ID                (MONITAMP_CHAN_ID + 1)
#define MONITPOS_SAMPLE_SIZE            16 /* 16 Bytes -> X = 32-bit / Y = 32-bit ... */

/* MONIT1 POS */
#define MONIT1POS_CHAN_ID               (MONITPOS_CHAN_ID + 1)
#define MONIT1POS_SAMPLE_SIZE           16 /* 16 Bytes -> X = 32-bit / Y = 32-bit ... */

/* End of channels placeholder */
#define END_CHAN_ID                     (MONIT1POS_CHAN_ID + 1)

#endif

