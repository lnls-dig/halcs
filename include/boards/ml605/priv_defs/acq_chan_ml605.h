#ifndef _ACQ_CHAN_ML605_H_
#define _ACQ_CHAN_ML605_H_

#include <acq_chan_gen_defs.h>

#define NUM_ACQ_CORE_SMIOS              1

/************************ Acquistion 0 Channel Parameters **************/

/* ADC */
#define ADC0_CHAN_ID                    0
#define ADC0_WORD_SIZE                  2                       /* 16-bit data */
#define ADC0_WORD_TYPE                  ACQ_WORD_TYPE_INT16     /* 16-bit signed data */
#define ADC0_NUM_WORDS_SAMPLE           4                       /* ADC 0, ADC 1, ADC 2, ADC 3 */
#define ADC0_SAMPLE_SIZE                (ADC0_WORD_SIZE*ADC0_NUM_WORDS_SAMPLE)

/* TBT AMP */
#define TBTAMP0_CHAN_ID                 (ADC0_CHAN_ID + 1)
#define TBTAMP0_WORD_SIZE               4                       /* 32-bit data */
#define TBTAMP0_WORD_TYPE               ACQ_WORD_TYPE_INT32     /* 32-bit signed data */
#define TBTAMP0_NUM_WORDS_SAMPLE        4                       /* TBTAMP0 0, TBTAMP0 1, TBTAMP0 2, TBTAMP0 3 */
#define TBTAMP0_SAMPLE_SIZE             (TBTAMP0_WORD_SIZE*TBTAMP0_NUM_WORDS_SAMPLE) /* 8 Bytes -> TBTAMP0 = 16-bit / ADC1 = 16-bit ... */

/* TBT POS */
#define TBTPOS0_CHAN_ID                 (TBTAMP0_CHAN_ID + 1)
#define TBTPOS0_WORD_SIZE               4                       /* 32-bit data */
#define TBTPOS0_WORD_TYPE               ACQ_WORD_TYPE_INT32     /* 32-bit signed data */
#define TBTPOS0_NUM_WORDS_SAMPLE        4                       /* TBTPOS0 0, TBTPOS0 1, TBTPOS0 2, TBTPOS0 3 */
#define TBTPOS0_SAMPLE_SIZE             (TBTPOS0_WORD_SIZE*TBTPOS0_NUM_WORDS_SAMPLE) /* 8 Bytes -> TBTPOS0 = 16-bit / ADC1 = 16-bit ... */

/* FOFB AMP */
#define FOFBAMP0_CHAN_ID                (TBTPOS0_CHAN_ID + 1)
#define FOFAMP0_WORD_SIZE               4                       /* 32-bit data */
#define FOFAMP0_WORD_TYPE               ACQ_WORD_TYPE_INT32     /* 32-bit signed data */
#define FOFAMP0_NUM_WORDS_SAMPLE        4                       /* FOFAMP0 0, FOFAMP0 1, FOFAMP0 2, FOFAMP0 3 */
#define FOFAMP0_SAMPLE_SIZE             (FOFAMP0_WORD_SIZE*FOFAMP0_NUM_WORDS_SAMPLE) /* 8 Bytes -> FOFAMP0 = 16-bit / ADC1 = 16-bit ... */

/* FOFB POS */
#define FOFBPOS0_CHAN_ID                (FOFBAMP0_CHAN_ID + 1)
#define FOFOPOS0_WORD_SIZE              4                       /* 32-bit data */
#define FOFOPOS0_WORD_TYPE              ACQ_WORD_TYPE_INT32     /* 32-bit signed data */
#define FOFOPOS0_NUM_WORDS_SAMPLE       4                       /* FOFOPOS0 0, FOFOPOS0 1, FOFOPOS0 2, FOFOPOS0 3 */
#define FOFOPOS0_SAMPLE_SIZE            (FOFOPOS0_WORD_SIZE*FOFOPOS0_NUM_WORDS_SAMPLE) /* 8 Bytes -> FOFOPOS0 = 16-bit / ADC1 = 16-bit ... */

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

