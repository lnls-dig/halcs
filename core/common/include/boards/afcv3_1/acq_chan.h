#ifndef _ACQ_CHAN_H_
#define _ACQ_CHAN_H_

#include <acq_chan_gen_defs.h>

#define NUM_ACQ_CORE_SMIOS              4

/************************ Acquistion 0 Channel Parameters **************/

/* ADC */
#define ADC_CHAN_ID                     0
#define ADC_SAMPLE_SIZE                 8 /* 8 Bytes -> ADC = 16-bit / ADC1 = 16-bit ... */

/* ADC SWAPPED (after the switching module) */
#define ADCSWAP_CHAN_ID                 (ADC_CHAN_ID + 1)
#define ADCSWAP_SAMPLE_SIZE             8 /* 8 Bytes -> ADCSWAP = 16-bit / ADCSWAP1 = 16-bit ... */

/* MIXER I/Q */
#define MIXIQ_CHAN_ID                   (ADCSWAP_CHAN_ID + 1)
#define MIXIQ_SAMPLE_SIZE               32 /* 32 Bytes -> MIXI = 32-bit / MIXQ = 32-bit ... / MIXI3 = 32-bit / MIXQ3 */

    /* DUMMY 0 */
#define DUMMY0_CHAN_ID                  (MIXIQ_CHAN_ID + 1)
#define DUMMY0_SAMPLE_SIZE              16 /* 16 Bytes -> 0 */

    /* TBTDECIM I/Q */
#define TBTDECIMIQ_CHAN_ID              (DUMMY0_CHAN_ID + 1)
#define TBTDECIMIQ_SAMPLE_SIZE          32 /* 32 Bytes -> TBTDECIM = 32-bit / TBTDECIM1 = 32-bit .../ TBTDECIMI3 = 32-bit / TBTDECIMQ3 */

    /* DUMMY 1 */
#define DUMMY1_CHAN_ID                  (TBTDECIMIQ_CHAN_ID + 1)
#define DUMMY1_SAMPLE_SIZE              16 /* 16 Bytes -> 0 */

    /* TBT AMP */
#define TBTAMP_CHAN_ID                  (DUMMY1_CHAN_ID + 1)
#define TBTAMP_SAMPLE_SIZE              16 /* 16 Bytes -> TBTAMP = 32-bit / TBTAMP1 = 32-bit ... */

    /* TBT PHASE */
#define TBTPHA_CHAN_ID                  (TBTAMP_CHAN_ID + 1)
#define TBTPHA_SAMPLE_SIZE              16 /* 16 Bytes -> TBTPHA = 32-bit / TBTPHA1 = 32-bit ... */

    /* TBT POS */
#define TBTPOS_CHAN_ID                  (TBTPHA_CHAN_ID + 1)
#define TBTPOS_SAMPLE_SIZE              16 /* 16 Bytes -> X = 32-bit / Y = 32-bit ... */

    /* FOFBDECIM I/Q */
#define FOFBDECIMIQ_CHAN_ID             (TBTPOS_CHAN_ID + 1)
#define FOFBDECIMIQ_SAMPLE_SIZE         32 /* 32 Bytes -> FOFBDECIM = 32-bit / FOFBDECIM1 = 32-bit ... / FOFBDECIMI3 = 32-bit / FOFBDECIMQ3 */

    /* DUMMY 2 */
#define DUMMY2_CHAN_ID                  (FOFBDECIMIQ_CHAN_ID + 1)
#define DUMMY2_SAMPLE_SIZE              16 /* 16 Bytes -> 0 */

    /* FOFB AMP */
#define FOFBAMP_CHAN_ID                 (DUMMY2_CHAN_ID + 1)
#define FOFBAMP_SAMPLE_SIZE             16 /* 16 Bytes -> FOFBAMP = 32-bit / FOFBAMP1 = 32-bit ... */

    /* FOFB PHA */
#define FOFBPHA_CHAN_ID                 (FOFBAMP_CHAN_ID + 1)
#define FOFBPHA_SAMPLE_SIZE             16 /* 16 Bytes -> FOFBPHA = 32-bit / FOFBPHA1 = 32-bit ... */

    /* FOFB POS */
#define FOFBPOS_CHAN_ID                 (FOFBPHA_CHAN_ID + 1)
#define FOFBPOS_SAMPLE_SIZE             16  /* 16 Bytes -> X = 32-bit / Y = 32-bit ... */

    /* MONIT AMP */
#define MONITAMP_CHAN_ID                (FOFBPOS_CHAN_ID + 1)
#define MONITAMP_SAMPLE_SIZE            16 /* 16 Bytes -> MONITAMP = 32-bit / MONITAMP1 = 32-bit ... */

    /* MONIT POS */
#define MONITPOS_CHAN_ID                (MONITAMP_CHAN_ID + 1)
#define MONITPOS_SAMPLE_SIZE            16 /* 16 Bytes -> X = 32-bit / Y = 32-bit ... */

    /* MONIT1 POS */
#define MONIT1POS_CHAN_ID               (MONITPOS_CHAN_ID + 1)
#define MONIT1POS_SAMPLE_SIZE           16 /* 16 Bytes -> X = 32-bit / Y = 32-bit ... */

    /* End of channels placeholder */
#define END_CHAN_ID                     (MONIT1POS_CHAN_ID + 1)

#endif

