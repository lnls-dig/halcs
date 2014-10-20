#ifndef _ACQ_CHAN_AFCV3_H_
#define _ACQ_CHAN_AFCV3_H_

#define BLOCK_SIZE 131072

/************************ Acquistion 0 Channel Parameters **************/

/* ADC */
#define ADC0_CHAN_ID                    0
#define ADC0_SAMPLE_SIZE                8 /* 8 Bytes -> ADC0 = 16-bit / ADC1 = 16-bit ... */

/* MIXER I */
#define MIX0_CHAN_ID                    (ADC0_CHAN_ID + 1)
#define MIX0_SAMPLE_SIZE                16 /* 16 Bytes -> MIX0 = 32-bit / MIX1 = 32-bit ... */

/* TBT AMP */
#define TBTAMP0_CHAN_ID                 (MIX0_CHAN_ID + 1)
#define TBTAMP0_SAMPLE_SIZE             16 /* 16 Bytes -> TBTAMP0 = 32-bit / TBTAMP1 = 32-bit ... */

/* TBT POS */
#define TBTPOS0_CHAN_ID                 (TBTAMP0_CHAN_ID + 1)
#define TBTPOS0_SAMPLE_SIZE             16 /* 16 Bytes -> X = 32-bit / Y = 32-bit ... */

/* FOFB AMP */
#define FOFBAMP0_CHAN_ID                (TBTPOS0_CHAN_ID + 1)
#define FOFBAMP0_SAMPLE_SIZE            16 /* 16 Bytes -> FOFBAMP0 = 32-bit / FOFBAMP1 = 32-bit ... */

/* FOFB POS */
#define FOFBPOS0_CHAN_ID                (FOFBAMP0_CHAN_ID + 1)
#define FOFBPOS0_SAMPLE_SIZE            16  /* 16 Bytes -> X = 32-bit / Y = 32-bit ... */

/* MONIT AMP */
#define MONITAMP0_CHAN_ID               (FOFBPOS0_CHAN_ID + 1)
#define MONITAMP0_SAMPLE_SIZE           16 /* 16 Bytes -> FOFBAMP0 = 32-bit / FOFBAMP1 = 32-bit ... */

/* MONIT POS */
#define MONITPOS0_CHAN_ID               (MONITAMP0_CHAN_ID + 1)
#define MONITPOS0_SAMPLE_SIZE           16 /* 16 Bytes -> X = 32-bit / Y = 32-bit ... */

/* MONIT1 POS */
#define MONIT1POS0_CHAN_ID              (MONITPOS0_CHAN_ID + 1)
#define MONIT1POS0_SAMPLE_SIZE          16 /* 16 Bytes -> X = 32-bit / Y = 32-bit ... */

/************************ Acquistion 1 Channel Parameters **************/

/* ADC */
#define ADC1_CHAN_ID                    (MONIT1POS0_CHAN_ID + 1)
#define ADC1_SAMPLE_SIZE                8 /* 8 Bytes -> ADC0 = 16-bit / ADC1 = 16-bit ... */

/* MIXER I */
#define MIX1_CHAN_ID                    (ADC1_CHAN_ID + 1)
#define MIX1_SAMPLE_SIZE                16 /* 16 Bytes -> MIX0 = 32-bit / MIX1 = 32-bit ... */

/* TBT AMP */
#define TBTAMP1_CHAN_ID                 (MIX1_CHAN_ID + 1)
#define TBTAMP1_SAMPLE_SIZE             16 /* 16 Bytes -> TBTAMP1 = 32-bit / TBTAMP1 = 32-bit ... */

/* TBT POS */
#define TBTPOS1_CHAN_ID                 (TBTAMP1_CHAN_ID + 1)
#define TBTPOS1_SAMPLE_SIZE             16 /* 16 Bytes -> X = 32-bit / Y = 32-bit ... */

/* FOFB AMP */
#define FOFBAMP1_CHAN_ID                (TBTPOS1_CHAN_ID + 1)
#define FOFBAMP1_SAMPLE_SIZE            16 /* 16 Bytes -> FOFBAMP1 = 32-bit / FOFBAMP1 = 32-bit ... */

/* FOFB POS */
#define FOFBPOS1_CHAN_ID                (FOFBAMP1_CHAN_ID + 1)
#define FOFBPOS1_SAMPLE_SIZE            16  /* 16 Bytes -> X = 32-bit / Y = 32-bit ... */

/* MONIT AMP */
#define MONITAMP1_CHAN_ID               (FOFBPOS1_CHAN_ID + 1)
#define MONITAMP1_SAMPLE_SIZE           16 /* 16 Bytes -> FOFBAMP1 = 32-bit / FOFBAMP1 = 32-bit ... */

/* MONIT POS */
#define MONITPOS1_CHAN_ID               (MONITAMP1_CHAN_ID + 1)
#define MONITPOS1_SAMPLE_SIZE           16 /* 16 Bytes -> X = 32-bit / Y = 32-bit ... */

/* MONIT1 POS */
#define MONIT1POS1_CHAN_ID              (MONITPOS1_CHAN_ID + 1)
#define MONIT1POS1_SAMPLE_SIZE          16 /* 16 Bytes -> X = 32-bit / Y = 32-bit ... */

/* End of channels placeholder */
#define END_CHAN_ID                     (MONIT1POS1_CHAN_ID + 1)

#endif

