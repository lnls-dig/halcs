#ifndef _ACQ_CHAN_AFCV3_H_
#define _ACQ_CHAN_AFCV3_H_

#include <acq_chan_gen_defs.h>

#define NUM_ACQ_CORE_SMIOS              2

/************************ Acquistion 0 Channel Parameters **************/

/* ADC */
#define ADC0_CHAN_ID                    0
#define ADC0_WORD_SIZE                  2                       /* 16-bit data */
#define ADC0_WORD_TYPE                  ACQ_WORD_TYPE_INT16     /* 16-bit signed data */
#define ADC0_NUM_WORDS_SAMPLE           4                       /* ADC 0, ADC 1, ADC 2, ADC 3 */
#define ADC0_SAMPLE_SIZE                (ADC0_WORD_SIZE*ADC0_NUM_WORDS_SAMPLE)

/* ADC SWAPPED (after the switching module) */
#define ADCSWAP0_CHAN_ID                (ADC0_CHAN_ID + 1)
#define ADCSWAP0_WORD_SIZE              2                       /* 16-bit data */
#define ADCSWAP0_WORD_TYPE              ACQ_WORD_TYPE_INT16     /* 16-bit signed data */
#define ADCSWAP0_NUM_WORDS_SAMPLE       4                       /* ADCSWAP 0, ADCSWAP 1, ADCSWAP 2, ADCSWAP 3 */
#define ADCSWAP0_SAMPLE_SIZE            (ADCSWAP0_WORD_SIZE*ADCSWAP0_NUM_WORDS_SAMPLE)

/* MIXER I/Q 1/2 */
#define MIXIQ120_CHAN_ID                (ADCSWAP0_CHAN_ID + 1)
#define MIXIQ120_WORD_SIZE              4                       /* 32-bit data */
#define MIXIQ120_WORD_TYPE              ACQ_WORD_TYPE_INT32     /* 32-bit signed data */
#define MIXIQ120_NUM_WORDS_SAMPLE       4                       /* MIXIQ120 0, MIXIQ120 1, MIXIQ120 2, MIXIQ120 3 */
#define MIXIQ120_SAMPLE_SIZE            (MIXIQ120_WORD_SIZE*MIXIQ120_NUM_WORDS_SAMPLE)

/* MIXER I/Q 3/4 */
#define MIXIQ340_CHAN_ID                (MIXIQ120_CHAN_ID + 1)
#define MIXIQ340_WORD_SIZE              4                       /* 32-bit data */
#define MIXIQ340_WORD_TYPE              ACQ_WORD_TYPE_INT32     /* 32-bit signed data */
#define MIXIQ340_NUM_WORDS_SAMPLE       4                       /* MIXIQ340 0, MIXIQ340 1, MIXIQ340 2, MIXIQ340 3 */
#define MIXIQ340_SAMPLE_SIZE            (MIXIQ340_WORD_SIZE*MIXIQ340_NUM_WORDS_SAMPLE) /* 8 Bytes -> MIXIQ340 = 16-bit / ADC1 = 16-bit ... */

/* TBTDECIM I/Q 1/2 */
#define TBTDECIMIQ120_CHAN_ID           (MIXIQ340_CHAN_ID + 1)
#define TBTDECIMIQ120_WORD_SIZE         4                       /* 32-bit data */
#define TBTDECIMIQ120_WORD_TYPE         ACQ_WORD_TYPE_INT32     /* 32-bit signed data */
#define TBTDECIMIQ120_NUM_WORDS_SAMPLE  4                       /* TBTDECIMIQ120 0, TBTDECIMIQ120 1, TBTDECIMIQ120 2, TBTDECIMIQ120 3 */
#define TBTDECIMIQ120_SAMPLE_SIZE       (TBTDECIMIQ120_WORD_SIZE*TBTDECIMIQ120_NUM_WORDS_SAMPLE) /* 8 Bytes -> TBTDECIMIQ120 = 16-bit / ADC1 = 16-bit ... */

/* TBTDECIM I/Q 3/4 */
#define TBTDECIMIQ340_CHAN_ID           (TBTDECIMIQ120_CHAN_ID + 1)
#define TBTDECIMIQ340_WORD_SIZE         4                       /* 32-bit data */
#define TBTDECIMIQ340_WORD_TYPE         ACQ_WORD_TYPE_INT32     /* 32-bit signed data */
#define TBTDECIMIQ340_NUM_WORDS_SAMPLE  4                       /* TBTDECIMIQ340 0, TBTDECIMIQ340 1, TBTDECIMIQ340 2, TBTDECIMIQ340 3 */
#define TBTDECIMIQ340_SAMPLE_SIZE       (TBTDECIMIQ340_WORD_SIZE*TBTDECIMIQ340_NUM_WORDS_SAMPLE) /* 8 Bytes -> TBTDECIMIQ340 = 16-bit / ADC1 = 16-bit ... */

/* TBT AMP */
#define TBTAMP0_CHAN_ID                 (TBTDECIMIQ340_CHAN_ID + 1)
#define TBTAMP0_WORD_SIZE               4                       /* 32-bit data */
#define TBTAMP0_WORD_TYPE               ACQ_WORD_TYPE_INT32     /* 32-bit signed data */
#define TBTAMP0_NUM_WORDS_SAMPLE        4                       /* TBTAMP0 0, TBTAMP0 1, TBTAMP0 2, TBTAMP0 3 */
#define TBTAMP0_SAMPLE_SIZE             (TBTAMP0_WORD_SIZE*TBTAMP0_NUM_WORDS_SAMPLE) /* 8 Bytes -> TBTAMP0 = 16-bit / ADC1 = 16-bit ... */

/* TBT PHASE */
#define TBTPHA0_CHAN_ID                 (TBTAMP0_CHAN_ID + 1)
#define TBTPHA0_WORD_SIZE               4                       /* 32-bit data */
#define TBTPHA0_WORD_TYPE               ACQ_WORD_TYPE_INT32     /* 32-bit signed data */
#define TBTPHA0_NUM_WORDS_SAMPLE        4                       /* TBTPHA0 0, TBTPHA0 1, TBTPHA0 2, TBTPHA0 3 */
#define TBTPHA0_SAMPLE_SIZE             (TBTPHA0_WORD_SIZE*TBTPHA0_NUM_WORDS_SAMPLE) /* 8 Bytes -> TBTPHA0 = 16-bit / ADC1 = 16-bit ... */

/* TBT POS */
#define TBTPOS0_CHAN_ID                 (TBTPHA0_CHAN_ID + 1)
#define TBTPOS0_WORD_SIZE               4                       /* 32-bit data */
#define TBTPOS0_WORD_TYPE               ACQ_WORD_TYPE_INT32     /* 32-bit signed data */
#define TBTPOS0_NUM_WORDS_SAMPLE        4                       /* TBTPOS0 0, TBTPOS0 1, TBTPOS0 2, TBTPOS0 3 */
#define TBTPOS0_SAMPLE_SIZE             (TBTPOS0_WORD_SIZE*TBTPOS0_NUM_WORDS_SAMPLE) /* 8 Bytes -> TBTPOS0 = 16-bit / ADC1 = 16-bit ... */

/* FOFBDECIM I/Q 1/2 */
#define FOFBDECIMIQ120_CHAN_ID          (TBTPOS0_CHAN_ID + 1)
#define FOFBDECIMIQ120_WORD_SIZE        4                       /* 32-bit data */
#define FOFBDECIMIQ120_WORD_TYPE        ACQ_WORD_TYPE_INT32     /* 32-bit signed data */
#define FOFBDECIMIQ120_NUM_WORDS_SAMPLE 4                       /* FOFBDECIMIQ120 0, FOFBDECIMIQ120 1, FOFBDECIMIQ120 2, FOFBDECIMIQ120 3 */
#define FOFBDECIMIQ120_SAMPLE_SIZE      (FOFBDECIMIQ120_WORD_SIZE*FOFBDECIMIQ120_NUM_WORDS_SAMPLE) /* 8 Bytes -> FOFBDECIMIQ120 = 16-bit / ADC1 = 16-bit ... */

/* FOFBDECIM I/Q 3/4 */
#define FOFBDECIMIQ340_CHAN_ID          (FOFBDECIMIQ120_CHAN_ID + 1)
#define FOFBDECIMIQ340_WORD_SIZE        4                       /* 32-bit data */
#define FOFBDECIMIQ340_WORD_TYPE        ACQ_WORD_TYPE_INT32     /* 32-bit signed data */
#define FOFBDECIMIQ340_NUM_WORDS_SAMPLE 4                       /* FOFBDECIMIQ340 0, FOFBDECIMIQ340 1, FOFBDECIMIQ340 2, FOFBDECIMIQ340 3 */
#define FOFBDECIMIQ340_SAMPLE_SIZE      (FOFBDECIMIQ340_WORD_SIZE*FOFBDECIMIQ340_NUM_WORDS_SAMPLE) /* 8 Bytes -> FOFBDECIMIQ340 = 16-bit / ADC1 = 16-bit ... */

/* FOFB AMP */
#define FOFBAMP0_CHAN_ID                (FOFBDECIMIQ340_CHAN_ID + 1)
#define FOFBAMP0_WORD_SIZE              4                       /* 32-bit data */
#define FOFBAMP0_WORD_TYPE              ACQ_WORD_TYPE_INT32     /* 32-bit signed data */
#define FOFBAMP0_NUM_WORDS_SAMPLE       4                       /* FOFBAMP0 0, FOFBAMP0 1, FOFBAMP0 2, FOFBAMP0 3 */
#define FOFBAMP0_SAMPLE_SIZE            (FOFBAMP0_WORD_SIZE*FOFBAMP0_NUM_WORDS_SAMPLE) /* 8 Bytes -> FOFBAMP0 = 16-bit / ADC1 = 16-bit ... */

/* FOFB PHA */
#define FOFBPHA0_CHAN_ID                (FOFBAMP0_CHAN_ID + 1)
#define FOFBPHA0_WORD_SIZE              4                       /* 32-bit data */
#define FOFBPHA0_WORD_TYPE              ACQ_WORD_TYPE_INT32     /* 32-bit signed data */
#define FOFBPHA0_NUM_WORDS_SAMPLE       4                       /* FOFBPHA0 0, FOFBPHA0 1, FOFBPHA0 2, FOFBPHA0 3 */
#define FOFBPHA0_SAMPLE_SIZE            (FOFBPHA0_WORD_SIZE*FOFBPHA0_NUM_WORDS_SAMPLE) /* 8 Bytes -> FOFBPHA0 = 16-bit / ADC1 = 16-bit ... */

/* FOFB POS */
#define FOFBPOS0_CHAN_ID                (FOFBPHA0_CHAN_ID + 1)
#define FOFBPOS0_WORD_SIZE              4                       /* 32-bit data */
#define FOFBPOS0_WORD_TYPE              ACQ_WORD_TYPE_INT32     /* 32-bit signed data */
#define FOFBPOS0_NUM_WORDS_SAMPLE       4                       /* FOFBPOS0 0, FOFBPOS0 1, FOFBPOS0 2, FOFBPOS0 3 */
#define FOFBPOS0_SAMPLE_SIZE            (FOFBPOS0_WORD_SIZE*FOFBPOS0_NUM_WORDS_SAMPLE) /* 8 Bytes -> FOFBPOS0 = 16-bit / ADC1 = 16-bit ... */

/* MONIT AMP */
#define MONITAMP0_CHAN_ID               (FOFBPOS0_CHAN_ID + 1)
#define MONITAMP0_WORD_SIZE             4                       /* 32-bit data */
#define MONITAMP0_WORD_TYPE             ACQ_WORD_TYPE_INT32     /* 32-bit signed data */
#define MONITAMP0_NUM_WORDS_SAMPLE      4                       /* MONITAMP0 0, MONITAMP0 1, MONITAMP0 2, MONITAMP0 3 */
#define MONITAMP0_SAMPLE_SIZE           (MONITAMP0_WORD_SIZE*MONITAMP0_NUM_WORDS_SAMPLE) /* 8 Bytes -> MONITAMP0 = 16-bit / ADC1 = 16-bit ... */

/* MONIT POS */
#define MONITPOS0_CHAN_ID               (MONITAMP0_CHAN_ID + 1)
#define MONITPOS0_WORD_SIZE             4                       /* 32-bit data */
#define MONITPOS0_WORD_TYPE             ACQ_WORD_TYPE_INT32     /* 32-bit signed data */
#define MONITPOS0_NUM_WORDS_SAMPLE      4                       /* MONITPOS0 0, MONITPOS0 1, MONITPOS0 2, MONITPOS0 3 */
#define MONITPOS0_SAMPLE_SIZE           (MONITPOS0_WORD_SIZE*MONITPOS0_NUM_WORDS_SAMPLE) /* 8 Bytes -> MONITPOS0 = 16-bit / ADC1 = 16-bit ... */

/* MONIT1 POS */
#define MONIT1POS0_CHAN_ID              (MONITPOS0_CHAN_ID + 1)
#define MONIT1POS0_WORD_SIZE            4                       /* 32-bit data */
#define MONIT1POS0_WORD_TYPE            ACQ_WORD_TYPE_INT32     /* 32-bit signed data */
#define MONIT1POS0_NUM_WORDS_SAMPLE     4                       /* MONIT1POS0 0, MONIT1POS0 1, MONIT1POS0 2, MONIT1POS0 3 */
#define MONIT1POS0_SAMPLE_SIZE          (MONIT1POS0_WORD_SIZE*MONIT1POS0_NUM_WORDS_SAMPLE) /* 8 Bytes -> MONIT1POS0 = 16-bit / ADC1 = 16-bit ... */

/* End of channels placeholder */
#define END_CHAN_ID                     (MONIT1POS0_CHAN_ID + 1)

#endif

