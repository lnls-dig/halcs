/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
*/

#ifndef _SM_IO_ACQ_CORE_H_
#define _SM_IO_ACQ_CORE_H_

#define ACQ_CORE_MIN_NUM_SHOTS              1
/* FIXME: This needs to match the FPGA firmware. Otherwise,
 * the acq_core module will not start an acquisition and
 * the user will be misinformed. If this number is different
 * from the FPGA firmware nothing will break, but we will loose
 * context of the error */
#define ACQ_CORE_MULTISHOT_MEM_SIZE         2048

/* This must match the FPGA maximum number of channels */
#define ACQ_CORE_NUM_CHAN_DESC              24
#define ACQ_CORE_CHAN_DESC_OFFSET           0x008 /* 2 32-bit registers */

typedef enum {
    TYPE_ACQ_CORE_SKIP=0,
    TYPE_ACQ_CORE_HW_PULSE=1,
    TYPE_ACQ_CORE_HW_DATA_DRIVEN,
    TYPE_ACQ_CORE_SW,
    TYPE_ACQ_CORE_END,
} acq_core_trig_type_e;

#define ACQ_CORE_NUM_TRIGGERS               TYPE_ACQ_CORE_END

typedef struct {
    uint32_t num_samples_pre;               /* Number of pre-trigger samples */
    uint32_t num_samples_post;              /* Number of post-trigger samples */
    uint32_t num_shots;                     /* Number of shots */
    /* Last trigger address. In case of multishot acquisition, this will
       contain only the last trigger address*/
    uint32_t trig_addr;
} acq_params_t;

typedef struct {
    acq_params_t acq_params[END_CHAN_ID];   /* Parameters for each channel */
    uint32_t curr_chan;                     /* Current channel being acquired */
    acq_buf_t *acq_buf;                     /* Channel properties */
} smio_acq_t;

/***************** Our methods *****************/

/* Creates a new instance of the smio realization */
smio_acq_t * smio_acq_new (smio_t *parent, uint32_t num_samples_pre,
        uint32_t num_samples_post, uint32_t num_shots);
/* Destroys the smio realizationn */
smio_err_e smio_acq_destroy (smio_acq_t **self_p);

#endif
