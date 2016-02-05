#ifndef _ACQ_CHAN_H_
#define _ACQ_CHAN_H_

#include <acq_chan_gen_defs.h>

#define NUM_ACQ_CORE_SMIOS              2

/************************ Acquistion 0 Channel Parameters **************/

/* ADC */
#define ADC0_CHAN_ID                    0
#define ADC0_SAMPLE_SIZE                8 /* 8 Bytes -> ADC0 = 16-bit / ADC1 = 16-bit ... */

/* ADC SWAPPED (after the switching module) */
#define ADCSWAP0_CHAN_ID                (ADC0_CHAN_ID + 1)
#define ADCSWAP0_SAMPLE_SIZE            8 /* 8 Bytes -> ADCSWAP0 = 16-bit / ADCSWAP1 = 16-bit ... */

/* MIXER I/Q 1/2 */
#define MIXIQ120_CHAN_ID                (ADCSWAP0_CHAN_ID + 1)
#define MIXIQ120_SAMPLE_SIZE            16 /* 16 Bytes -> MIXI0 = 32-bit / MIXQ0 = 32-bit ... */

/* MIXER I/Q 3/4 */
#define MIXIQ340_CHAN_ID                (MIXIQ120_CHAN_ID + 1)
#define MIXIQ340_SAMPLE_SIZE            16 /* 16 Bytes -> MIXI2 = 32-bit / MIXQ2 = 32-bit ... */

/* TBTDECIM I/Q 1/2 */
#define TBTDECIMIQ120_CHAN_ID           (MIXIQ340_CHAN_ID + 1)
#define TBTDECIMIQ120_SAMPLE_SIZE       16 /* 16 Bytes -> TBTDECIM0 = 32-bit / TBTDECIM1 = 32-bit ... */

/* TBTDECIM I/Q 3/4 */
#define TBTDECIMIQ340_CHAN_ID           (TBTDECIMIQ120_CHAN_ID + 1)
#define TBTDECIMIQ340_SAMPLE_SIZE       16 /* 16 Bytes -> TBTDECIM0 = 32-bit / TBTDECIM1 = 32-bit ... */

/* TBT AMP */
#define TBTAMP0_CHAN_ID                 (TBTDECIMIQ340_CHAN_ID + 1)
#define TBTAMP0_SAMPLE_SIZE             16 /* 16 Bytes -> TBTAMP0 = 32-bit / TBTAMP1 = 32-bit ... */

/* TBT PHASE */
#define TBTPHA0_CHAN_ID                 (TBTAMP0_CHAN_ID + 1)
#define TBTPHA0_SAMPLE_SIZE             16 /* 16 Bytes -> TBTPHA0 = 32-bit / TBTPHA1 = 32-bit ... */

/* TBT POS */
#define TBTPOS0_CHAN_ID                 (TBTPHA0_CHAN_ID + 1)
#define TBTPOS0_SAMPLE_SIZE             16 /* 16 Bytes -> X = 32-bit / Y = 32-bit ... */

/* FOFBDECIM I/Q 1/2 */
#define FOFBDECIMIQ120_CHAN_ID          (TBTPOS0_CHAN_ID + 1)
#define FOFBDECIMIQ120_SAMPLE_SIZE      16 /* 16 Bytes -> FOFBDECIM0 = 32-bit / FOFBDECIM1 = 32-bit ... */

/* FOFBDECIM I/Q 3/4 */
#define FOFBDECIMIQ340_CHAN_ID          (FOFBDECIMIQ120_CHAN_ID + 1)
#define FOFBDECIMIQ340_SAMPLE_SIZE      16 /* 16 Bytes -> FOFBDECIM0 = 32-bit / FOFBDECIM1 = 32-bit ... */

/* FOFB AMP */
#define FOFBAMP0_CHAN_ID                (FOFBDECIMIQ340_CHAN_ID + 1)
#define FOFBAMP0_SAMPLE_SIZE            16 /* 16 Bytes -> FOFBAMP0 = 32-bit / FOFBAMP1 = 32-bit ... */

/* FOFB PHA */
#define FOFBPHA0_CHAN_ID                (FOFBAMP0_CHAN_ID + 1)
#define FOFBPHA0_SAMPLE_SIZE            16 /* 16 Bytes -> FOFBPHA0 = 32-bit / FOFBPHA1 = 32-bit ... */

/* FOFB POS */
#define FOFBPOS0_CHAN_ID                (FOFBPHA0_CHAN_ID + 1)
#define FOFBPOS0_SAMPLE_SIZE            16  /* 16 Bytes -> X = 32-bit / Y = 32-bit ... */

/* MONIT AMP */
#define MONITAMP0_CHAN_ID               (FOFBPOS0_CHAN_ID + 1)
#define MONITAMP0_SAMPLE_SIZE           16 /* 16 Bytes -> MONITAMP0 = 32-bit / MONITAMP1 = 32-bit ... */

/* MONIT POS */
#define MONITPOS0_CHAN_ID               (MONITAMP0_CHAN_ID + 1)
#define MONITPOS0_SAMPLE_SIZE           16 /* 16 Bytes -> X = 32-bit / Y = 32-bit ... */

/* MONIT1 POS */
#define MONIT1POS0_CHAN_ID              (MONITPOS0_CHAN_ID + 1)
#define MONIT1POS0_SAMPLE_SIZE          16 /* 16 Bytes -> X = 32-bit / Y = 32-bit ... */

/* End of channels placeholder */
#define END_CHAN_ID                     (MONIT1POS0_CHAN_ID + 1)

#endif

