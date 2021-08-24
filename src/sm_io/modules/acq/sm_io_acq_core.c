/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "halcs_server.h"
/* Private headers */
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
#if 0
    uint32_t smio_id = smio_get_id (parent);
#endif
    char *board_type = NULL;

    smio_err_e err = smio_get_board_type (parent, &board_type);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not get board_type",
            err_get_board_type);
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO, "[sm_io:acq_core] board_type: %s, Inst ID: %u\n",
            board_type, inst_id);

    /* Lookup symbols */
    const size_t **acq_core_num_acq_core_smios_p =
        hutils_lookup_symbol ("pvar_const_size_t_p_", board_type, "_num_acq_core_smios");
    ASSERT_ALLOC(acq_core_num_acq_core_smios_p, err_lookup_sym);
    const size_t acq_core_num_acq_core_smios = **acq_core_num_acq_core_smios_p;

    const uint32_t **acq_core_end_chan_id_p =
        hutils_lookup_symbol ("pvar_const_uint32_t_p_", board_type, "_end_chan_id");
    ASSERT_ALLOC(acq_core_end_chan_id_p, err_lookup_sym);
    const uint32_t acq_core_end_chan_id = **acq_core_end_chan_id_p;

    const acq_buf_t **acq_core_buf =
        hutils_lookup_symbol ("pvar_const_acq_buf_t_p_", board_type, "_acq_buf");
    ASSERT_ALLOC(acq_core_buf, err_lookup_sym);

    const size_t **acq_core_mem_total_size_p =
        hutils_lookup_symbol ("pvar_const_size_t_p_", board_type, "_mem_total_size");
    ASSERT_ALLOC(acq_core_mem_total_size_p, err_lookup_sym);
    const size_t acq_core_mem_total_size = **acq_core_mem_total_size_p;

    const size_t **acq_core_mem_region_size_p =
        hutils_lookup_symbol ("pvar_const_size_t_p_", board_type, "_mem_region_size");
    ASSERT_ALLOC(acq_core_mem_region_size_p, err_lookup_sym);
    const size_t acq_core_mem_region_size = **acq_core_mem_region_size_p;

    //const size_t **acq_core_ddr3_data_width_p =
    //    hutils_lookup_symbol ("pvar_const_size_t_p_", board_type, "_ddr3_data_width");
    //ASSERT_ALLOC(acq_core_ddr3_data_width_p, err_lookup_sym);
    //const size_t acq_core_ddr3_data_width = **acq_core_ddr3_data_width_p;

    const size_t **acq_core_ddr3_byte_2_bit_p =
        hutils_lookup_symbol ("pvar_const_size_t_p_", board_type, "_ddr3_byte_2_bit");
    ASSERT_ALLOC(acq_core_ddr3_byte_2_bit_p, err_lookup_sym);
    const size_t acq_core_ddr3_byte_2_bit = **acq_core_ddr3_byte_2_bit_p;

    self->ddr_byte_2_bit = acq_core_ddr3_byte_2_bit;

    const size_t **acq_core_ddr3_payload_size_p =
        hutils_lookup_symbol ("pvar_const_size_t_p_", board_type, "_ddr3_payload_size");
    ASSERT_ALLOC(acq_core_ddr3_payload_size_p, err_lookup_sym);
    const size_t acq_core_ddr3_payload_size = **acq_core_ddr3_payload_size_p;

    self->ddr_payload_size = acq_core_ddr3_payload_size;

    /* Get the number of acquisition channels. If 0, this register is probably
     * unimplemented in gateware, so default it to the board END_CHAN_ID */
    GET_PARAM(parent, acq, 0x0, ACQ_CORE, ACQ_CHAN_CTL,
            NUM_CHAN, MULT_BIT_PARAM, self->num_chan, NO_FMT_FUNC);
    if (self->num_chan == 0) {
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO, "[sm_io:acq_core] Number of channels is 0. "
            "using board defaults: %u\n", acq_core_end_chan_id);
        self->num_chan = acq_core_end_chan_id;
    }

    /* Get the number of multishot RAM size */
    uint32_t multishot_ram_size_impl = 0;
    GET_PARAM(parent, acq, 0x0, ACQ_CORE, SHOTS,
            MULTISHOT_RAM_SIZE_IMPL, SINGLE_BIT_PARAM, multishot_ram_size_impl,
            NO_FMT_FUNC);
    GET_PARAM(parent, acq, 0x0, ACQ_CORE, SHOTS,
            MULTISHOT_RAM_SIZE, MULT_BIT_PARAM, self->multishot_ram_size, NO_FMT_FUNC);
    if (!multishot_ram_size_impl) {
        self->multishot_ram_size = ACQ_CORE_MULTISHOT_MEM_SIZE;
    }
    else if (self->multishot_ram_size == 0) {
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_WARN, "[sm_io:acq_core] Multishot RAM size is 0. "
            "It will not be possible to perform multishot acquisitions for this module\n");
    }

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO, "[sm_io:acq_core] Multishot RAM size: %u\n",
        self->multishot_ram_size);

    /* Initialize acq_buf */
    self->acq_buf = (acq_buf_t *) zmalloc ((sizeof *self->acq_buf) * self->num_chan);
    ASSERT_ALLOC (self->acq_buf, err_acq_buf_alloc);

    self->curr_chan = 0;

    uint32_t __acq_buf_max_inst_id = acq_core_num_acq_core_smios-1;

    /* Initialize sample_size and max_samples for acq_buf */
    for (uint32_t i = 0; i < self->num_chan; i++) {
        /* These are all in bit sizes */
        uint32_t int_ch_width = 0;
        uint32_t num_coalesce = 0;
        uint32_t num_atoms = 0;
        uint32_t atom_width = 0;
        /* These are all in bytes */
        uint32_t sample_size = 0;
        //uint32_t max_samples = 0;
        uint32_t first_addr = 0;
        //uint32_t last_addr = 0;
        uint32_t acq_buf_id = (inst_id > __acq_buf_max_inst_id ||
                                i > self->num_chan-1)? 
                                    i :
                                    (*acq_core_buf)[inst_id*acq_core_num_acq_core_smios + i].id;
        uint32_t acq_buf_start_addr = (inst_id > __acq_buf_max_inst_id ||
                                i > self->num_chan-1)? 
                                    (acq_core_mem_total_size - acq_core_mem_region_size) : 
                                    (*acq_core_buf)[inst_id*acq_core_num_acq_core_smios + i].start_addr;
        //uint32_t acq_buf_end_addr =(inst_id > __acq_buf_max_inst_id ||
        //                        i > self->num_chan-1)? 
        //                            (acq_core_mem_total_size - acq_core_ddr3_data_width) : 
        //                            (*acq_core_buf)[inst_id*acq_core_num_acq_core_smios + i].end_addr;

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

        ASSERT_TEST(int_ch_width > 0, "Invalid int_ch_width property", err_int_ch_width);
        ASSERT_TEST(num_coalesce > 0, "Invalid num_coalesce property", err_num_coalesce);
        ASSERT_TEST(num_atoms > 0, "Invalid num_atoms property", err_num_atoms);
        ASSERT_TEST(atom_width > 0, "Invalid atom_width property", err_atom_width);

        /* These are exported to clients so they know how to split the array */
        UNUSED(num_atoms);
        UNUSED(atom_width);

        sample_size = int_ch_width/acq_core_ddr3_byte_2_bit * num_coalesce;
        first_addr = acq_buf_start_addr;
        /* This is the last possible address to store a sample. be safe in case
         * this is a dummy area with 0 bytes space */
        //last_addr = ((acq_buf_end_addr - sample_size) > acq_buf_start_addr)?
        //    (acq_buf_end_addr - sample_size) : acq_buf_start_addr;
        //max_samples = (last_addr - first_addr) / sample_size;

        self->acq_buf[i].id = acq_buf_id;
        self->acq_buf[i].start_addr = first_addr;
        self->acq_buf[i].end_addr = first_addr + 0x0FFFFFF8;
        self->acq_buf[i].sample_size = sample_size;
        self->acq_buf[i].max_samples = 33554431;

        DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO, "[sm_io:acq_core] Channel properties, ACQ %u:\n"
            "\tID = %u\n"
            "\tStart Addr = 0x%08X\n"
            "\tEnd Addr = 0x%08X\n"
            "\tSample Size = %u\n"
            "\tMax Samples = %u\n",
            inst_id, self->acq_buf[i].id, self->acq_buf[i].start_addr,
            self->acq_buf[i].end_addr, self->acq_buf[i].sample_size, self->acq_buf[i].max_samples);
    }

    /* Initialize acq_params */
    self->acq_params = (acq_params_t *) zmalloc ((sizeof *self->acq_params) * self->num_chan);
    ASSERT_ALLOC (self->acq_params, err_acq_params_alloc);

    /* Set default value for all channels */
    for (uint32_t i = 0; i < self->num_chan; i++) {
        self->acq_params[i].num_samples_pre = num_samples_pre;
        self->acq_params[i].num_samples_post = num_samples_post;
        self->acq_params[i].num_shots = num_shots;
        /* Default trigger address is the beggining of the channel address */
        self->acq_params[i].trig_addr = self->acq_buf[i].start_addr;
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO, "[sm_io:acq_core] Acquisition parameters, ACQ %u:\n"
            "\tID = %u\n"
            "\tNumber of Samples Pre-Trigger = %u\n"
            "\tNumber of Samples Post-Trigger = %u\n"
            "\tNumber of Shots = %u\n"
            "\tTrigger Address = 0x%08X\n",
            inst_id, i, self->acq_params[i].num_samples_pre, self->acq_params[i].num_samples_post,
            self->acq_params[i].num_shots, self->acq_params[i].trig_addr);
    }

#if 0
    /* Get SDB info for this core */
    smio_err_e err = SMIO_SUCCESS;

    err = smio_get_sdb_info (parent, smio_id, inst_id, ACQ_SDB_VENDORID, ACQ_SDB_DEVID,
        &self->sdbutils_info);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not get SDB info", err_sdb_info);

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO, "[sm_io:acq_core] abi_class: %" PRIu16 ", "
        "abi_ver_major: %" PRIu8 ", abi_ver_minor: %" PRIu8 ", bus_specific: %" PRIu32 "\n",
        self->sdbutils_info.abi_class,
        self->sdbutils_info.abi_ver_major,
        self->sdbutils_info.abi_ver_minor,
        self->sdbutils_info.bus_specific);
#endif

    return self;

#if 0
err_sdb_info:
#endif
err_acq_params_alloc:
    free (self->acq_params);
err_atom_width:
err_num_atoms:
err_num_coalesce:
err_int_ch_width:
    free(self->acq_buf);
err_acq_buf_alloc:
err_lookup_sym:
err_get_board_type:
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

        free (self->acq_params);
        free (self->acq_buf);
        free (self);
        *self_p = NULL;
    }

    return SMIO_SUCCESS;
}

