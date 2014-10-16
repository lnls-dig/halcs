#ifndef _ACQ_CHAN_ML605_H_
#define _ACQ_CHAN_ML605_H_

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

#define END_CHAN_ID 5

#endif

