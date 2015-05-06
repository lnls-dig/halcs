#ifndef _ACQ_CHAN_GEN_DEFS_H_
#define _ACQ_CHAN_GEN_DEFS_H_

/* Acquisition word type */
enum _acq_word_type_e {
    ACQ_WORD_TYPE_ERROR = 0xFF,
    ACQ_WORD_TYPE_NONE  = 0,           /* used as terminator */
    ACQ_WORD_TYPE_INT16 = 1,
    ACQ_WORD_TYPE_UINT16,
    ACQ_WORD_TYPE_INT32,
    ACQ_WORD_TYPE_UINT32,
    ACQ_WORD_TYPE_INT64,
    ACQ_WORD_TYPE_UINT64,
    ACQ_WORD_TYPE_FLOAT,
    ACQ_WORD_TYPE_DOUBLE,
};

/* Acquisition word type */
typedef enum _acq_word_type_e acq_word_type_e;

/* Acquisition channel definitions */
struct _acq_chan_desc_t {
    uint32_t chan;                      /* Channel number */
    uint32_t num_words_sample;          /* Number of words per sample */
    acq_word_type_e word_type;          /* Word type */
    uint32_t sample_size;               /* Total sample size */
};

typedef struct _acq_chan_desc_t acq_chan_desc_t;

#define BLOCK_SIZE 131072

#endif

