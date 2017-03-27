/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "halcs_server.h"
/* Private headers */
#include "ddr3_map.h"
#include "sm_io_acq_core.h"
#include "hw/wb_acq_core_regs.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...)  \
    ASSERT_HAL_TEST(test_boolean, SM_IO, "[sm_io_acq_core]",                    \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_HAL_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)                   \
    ASSERT_HAL_ALLOC(ptr, SM_IO, "[sm_io_acq_core]",                            \
            smio_err_str(SMIO_ERR_ALLOC),                                       \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_HAL_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                                \
    CHECK_HAL_ERR(err, SM_IO, "[sm_io_acq_core]",                               \
            smio_err_str (err_type))

/* Creates a new instance of Device Information */
smio_acq_t * smio_acq_new (smio_t *parent, uint32_t num_samples_pre,
        uint32_t num_samples_post, uint32_t num_shots)
{
    smio_acq_t *self = (smio_acq_t *) zmalloc (sizeof *self);
    ASSERT_ALLOC(self, err_self_alloc);
    uint32_t inst_id = smio_get_inst_id (parent);

    /* Initialize acq_buf */
    self->acq_buf = (acq_buf_t *) zmalloc ((sizeof *self->acq_buf) * END_CHAN_ID);
    ASSERT_ALLOC (self->acq_buf, err_acq_buf_alloc);

    self->curr_chan = 0;

    /* Initialize sample_size and max_samples for acq_buf */
    for (uint32_t i = 0; i < END_CHAN_ID; i++) {
        /* These are all in bit sizes */
        uint32_t int_ch_width = 0;
        uint32_t num_coalesce = 0;
        uint32_t num_atoms = 0;
        uint32_t atom_width = 0;
        /* These are all in bytes */
        uint32_t sample_size = 0;
        uint32_t max_samples = 0;
        uint32_t first_addr = 0;
        uint32_t last_addr = 0;
        uint32_t acq_buf_id =  __acq_buf[inst_id][i].id;
        uint32_t acq_buf_start_addr = __acq_buf[inst_id][i].start_addr;
        uint32_t acq_buf_end_addr = __acq_buf[inst_id][i].end_addr;

        /* Get channel properties */
        GET_PARAM_CHANNEL(parent, acq, 0x0, ACQ_CORE, CH0_DESC, INT_WIDTH,
                ACQ_CORE_CHAN_DESC_OFFSET, i, MULT_BIT_PARAM, int_ch_width,
                NO_FMT_FUNC);
        GET_PARAM_CHANNEL(parent, acq, 0x0, ACQ_CORE, CH0_DESC, NUM_COALESCE,
                ACQ_CORE_CHAN_DESC_OFFSET, i, MULT_BIT_PARAM, num_coalesce,
                NO_FMT_FUNC);
        GET_PARAM_CHANNEL(parent, acq, 0x0, ACQ_CORE, CH0_ATOM_DESC, NUM_ATOMS,
                ACQ_CORE_CHAN_DESC_OFFSET, i, MULT_BIT_PARAM, num_atoms,
                NO_FMT_FUNC);
        GET_PARAM_CHANNEL(parent, acq, 0x0, ACQ_CORE, CH0_ATOM_DESC, ATOM_WIDTH,
                ACQ_CORE_CHAN_DESC_OFFSET, i, MULT_BIT_PARAM, atom_width,
                NO_FMT_FUNC);

        /* These are exported to clients so they know how to split the array */
        UNUSED(num_atoms);
        UNUSED(atom_width);

        sample_size = int_ch_width/DDR3_BYTE_2_BIT * num_coalesce;
        first_addr = acq_buf_start_addr;
        /* This is the last possible address to store a sample. be safe in case
         * this is a dummy area with 0 bytes space */
        last_addr = ((acq_buf_end_addr - sample_size) > acq_buf_start_addr)? 
            (acq_buf_end_addr - sample_size) : acq_buf_start_addr;
        max_samples = (last_addr - first_addr) / sample_size;

        self->acq_buf[i].id = acq_buf_id;
        self->acq_buf[i].start_addr = first_addr;
        self->acq_buf[i].end_addr = last_addr;
        self->acq_buf[i].sample_size = sample_size;
        self->acq_buf[i].max_samples = max_samples;

        DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO, "[sm_io:acq_core] Channel properties, ACQ %u:\n"
            "ID = %u\n"
            "Start Addr = 0x%08X\n"
            "End Addr = 0x%08X\n"
            "Sample Size = %u\n"
            "Max Samples = %u\n", 
            inst_id, self->acq_buf[i].id, self->acq_buf[i].start_addr, 
            self->acq_buf[i].end_addr, self->acq_buf[i].sample_size, self->acq_buf[i].max_samples);
    }

    /* Set default value for all channels */
    for (uint32_t i = 0; i < END_CHAN_ID; i++) {
        self->acq_params[i].num_samples_pre = num_samples_pre;
        self->acq_params[i].num_samples_post = num_samples_post;
        self->acq_params[i].num_shots = num_shots;
        /* Default trigger address is the beggining of the channel address */
        self->acq_params[i].trig_addr = self->acq_buf[i].start_addr;
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO, "[sm_io:acq_core] Acquisition parameters, ACQ %u:\n"
            "ID = %u\n"
            "Number of Samples Pre-Trigger = %u\n"
            "Number of Samples Post-Trigger = %u\n"
            "Number of Shots = %u\n"
            "Trigger Address = 0x%08X\n", 
            inst_id, i, self->acq_params[i].num_samples_pre, self->acq_params[i].num_samples_post,
            self->acq_params[i].num_shots, self->acq_params[i].trig_addr);
    }

    /* initilize acquisition buffer areas. Defined in ddr3_map.h */
    if (inst_id > NUM_ACQ_CORE_SMIOS-1) {
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_ERR, "[sm_io:acq_core] Instance ID invalid\n");
        return NULL;
    }

    return self;

err_acq_buf_alloc:
    free (self);
err_self_alloc:
    return NULL;
}

/* Destroy an instance of the Device Information */
smio_err_e smio_acq_destroy (smio_acq_t **self_p)
{
    assert (self_p);

    if (*self_p) {
        smio_acq_t *self = *self_p;

        self->acq_buf = NULL;
        free (self);
        *self_p = NULL;
    }

    return SMIO_SUCCESS;
}

