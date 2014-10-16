#ifndef _ACQ_CHAN_AFCV3_H_
#define _ACQ_CHAN_AFCV3_H_

#define BLOCK_SIZE 131072

/* ADC */
#define ADC0_CHAN_ID 0
#define ADC0_SAMPLE_SIZE 8 // 8 Bytes -> ADC0 = 16-bit / ADC1 = 16-bit ...

/* TBT AMP */
#define TBTAMP0_CHAN_ID 1
#define TBTAMP0_SAMPLE_SIZE 16 // 16 Bytes -> TBTAMP0 = 32-bit / TBTAMP1 = 32-bit ...

/* TBT POS */
#define TBTPOS0_CHAN_ID 2
#define TBTPOS0_SAMPLE_SIZE 16 // 16 Bytes -> X = 32-bit / Y = 32-bit ...

/* FOFB AMP */
#define FOFBAMP0_CHAN_ID 3
#define FOFBAMP0_SAMPLE_SIZE 16 // 16 Bytes -> FOFBAMP0 = 32-bit / FOFBAMP1 = 32-bit ...

/* FOFB POS */
#define FOFBPOS0_CHAN_ID 4
#define FOFBPOS0_SAMPLE_SIZE 16 // 16 Bytes -> X = 32-bit / Y = 32-bit ...

/* ADC */
#define ADC1_CHAN_ID 5
#define ADC1_SAMPLE_SIZE 8 // 8 Bytes -> ADC0 = 16-bit / ADC1 = 16-bit ...

/* TBT AMP */
#define TBTAMP1_CHAN_ID 6
#define TBTAMP1_SAMPLE_SIZE 16 // 16 Bytes -> TBTAMP0 = 32-bit / TBTAMP1 = 32-bit ...

/* TBT POS */
#define TBTPOS1_CHAN_ID 7
#define TBTPOS1_SAMPLE_SIZE 16 // 16 Bytes -> X = 32-bit / Y = 32-bit ...

/* FOFB AMP */
#define FOFBAMP1_CHAN_ID 8
#define FOFBAMP1_SAMPLE_SIZE 16 // 16 Bytes -> FOFBAMP0 = 32-bit / FOFBAMP1 = 32-bit ...

/* FOFB POS */
#define FOFBPOS1_CHAN_ID 9
#define FOFBPOS1_SAMPLE_SIZE 16 // 16 Bytes -> X = 32-bit / Y = 32-bit ...

#define END_CHAN_ID 10

#endif

