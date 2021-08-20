/*
 * Copyright (C) 2017 LNLS (www.lnls.br)
 * Author: Janito Vaqueiro Ferreira Filho <janito.filho@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "bpm_client.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, LIB_CLIENT, "[libbpmclient]",\
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_ALLOC(ptr, LIB_CLIENT, "[libbpmclient]",        \
            halcs_client_err_str(HALCS_CLIENT_ERR_ALLOC),       \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(expression)                                   \
    do {                                                        \
        halcs_client_err_e result = expression;                 \
        CHECK_HAL_ERR(result, LIB_CLIENT, "[libbpmclient]",     \
                halcs_client_err_str (result));                 \
    } while (0)

#define POSITIVE_SLOPE 0
#define FIRST_SAMPLE 0

/* Our structure */
struct _bpm_single_pass_t {
    acq_client_t *acq_client;               /* Acquisition client interface */
    char *service;                          /* Acquisition client service */
    smio_init_board_type_t* board_type;     /* Board type name */
    bpm_parameters_t* bpm_parameters;       /* BPM parameters */
    acq_req_t request;                      /* Acquisition request parameters */
    acq_trans_t transaction;                /* Acquisition transaction */
    uint32_t adc_chanswap_id;               /* ADC channel ID after swap/unswap */

    uint32_t trigger_type;                  /* Trigger source */
    uint32_t trigger_slope;                 /* Trigger slope selection */
    uint32_t trigger_hysteresis_samples;    /* Trigger hysteresis filter */
    uint32_t trigger_delay;                 /* Trigger delay, in ADC clock cycles */
    uint32_t trigger_active_sample;         /* Data sample to use as trigger */
    uint32_t trigger_threshold;             /* Trigger threshold */
};

/********************************************************/
/************************ Our API ***********************/
/********************************************************/

static bpm_single_pass_t *_bpm_single_pass_new (acq_client_t *acq_client,
        char *broker_endp, char *service, char *service_board, 
        bpm_parameters_t *bpm_parameters, uint32_t samples_pre, 
        uint32_t samples_post, uint32_t num_shots);

static halcs_client_err_e _configure_trigger (bpm_single_pass_t *self);

static void _configure_request (bpm_single_pass_t *self, uint32_t samples_pre,
        uint32_t samples_post, uint32_t num_shots);

static void _setup_transaction (bpm_single_pass_t *self);

static void _process_single_pass_sample (bpm_single_pass_t *self,
        bpm_sample_t *sample);

static double _cast_to_double (uint8_t *adc_sample, uint32_t atom_size);

static double _squared_value (double value);

static void _calculate_bpm_sample_std (bpm_parameters_t *parameters, double a,
        double b, double c, double d, bpm_sample_t *sample);

static void _calculate_bpm_sample_partial (bpm_parameters_t *parameters, double a,
        double b, double c, double d, bpm_sample_t *sample);

static void _calculate_bpm_sample (bpm_parameters_t *parameters, double a,
        double b, double c, double d, bpm_sample_t *sample, bool partial_delta);

static void _release_transaction (bpm_single_pass_t *self);

bpm_single_pass_t *bpm_single_pass_new (char *broker_endp, int verbose,
        const char *log_file_name, char *service, char *service_board,
        bpm_parameters_t *bpm_parameters, uint32_t samples_pre,
        uint32_t samples_post, uint32_t num_shots)
{
    acq_client_t *acq_client = acq_client_new (broker_endp, verbose,
        log_file_name);

    ASSERT_ALLOC (acq_client, err_acq_client_new);

    return _bpm_single_pass_new (acq_client, broker_endp, service, 
            service_board, bpm_parameters, samples_pre, samples_post, 
            num_shots);

err_acq_client_new:
    return NULL;
}

bpm_single_pass_t *bpm_single_pass_new_time (char *broker_endp, int verbose,
        const char *log_file_name, int timeout, char *service, char *service_board,
        bpm_parameters_t *bpm_parameters, uint32_t samples_pre,
        uint32_t samples_post, uint32_t num_shots)
{
    acq_client_t *acq_client = acq_client_new_time (broker_endp, verbose,
        log_file_name, timeout);

    ASSERT_ALLOC (acq_client, err_acq_client_new);

    return _bpm_single_pass_new (acq_client, broker_endp, service, 
            service_board, bpm_parameters, samples_pre, samples_post, 
            num_shots);

err_acq_client_new:
    return NULL;
}

bpm_single_pass_t *bpm_single_pass_new_log_mode (char *broker_endp, int verbose,
        const char *log_file_name, const char *log_mode, char *service, char *service_board,
        bpm_parameters_t *bpm_parameters, uint32_t samples_pre,
        uint32_t samples_post, uint32_t num_shots)
{
    acq_client_t *acq_client = acq_client_new_log_mode (broker_endp, verbose,
        log_file_name, log_mode);

    ASSERT_ALLOC (acq_client, err_acq_client_new);

    return _bpm_single_pass_new (acq_client, broker_endp, service, 
            service_board, bpm_parameters, samples_pre, samples_post, 
            num_shots);

err_acq_client_new:
    return NULL;
}

bpm_single_pass_t *bpm_single_pass_new_log_mode_time (char *broker_endp,
        int verbose, const char *log_file_name, const char *log_mode,
        int timeout, char *service, char *service_board, 
        bpm_parameters_t *bpm_parameters, uint32_t samples_pre, 
        uint32_t samples_post, uint32_t num_shots)
{
    acq_client_t *acq_client = acq_client_new_log_mode_time (broker_endp,
        verbose, log_file_name, log_mode, timeout);

    ASSERT_ALLOC (acq_client, err_acq_client_new);

    return _bpm_single_pass_new (acq_client, broker_endp, service, 
            service_board, bpm_parameters, samples_pre, samples_post, 
            num_shots);

err_acq_client_new:
    return NULL;
}

static bpm_single_pass_t *_bpm_single_pass_new (acq_client_t *acq_client,
        char *broker_endp, char *service, char *service_board, 
        bpm_parameters_t *bpm_parameters, uint32_t samples_pre, 
        uint32_t samples_post, uint32_t num_shots)
{
    assert (service);

    bpm_single_pass_t *self = zmalloc (sizeof (bpm_single_pass_t));

    self->acq_client = acq_client;
    self->service = strdup (service);

    self->board_type = zmalloc (sizeof (*self->board_type));

    /* Get board_type with an ephemeral halcs_client */
    halcs_client_t *halcs_client = halcs_client_new (broker_endp, 0,
            NULL);
    ASSERT_ALLOC (halcs_client, err_halcs_client_new);
    halcs_client_err_e err = halcs_get_init_board_type (halcs_client,
           service_board, self->board_type);
    ASSERT_TEST (err == HALCS_CLIENT_SUCCESS, "Could not get board type",
           err_halcs_get_init_board_type);

    halcs_client_destroy (&halcs_client);

    /* Get channel to use from dynamic loading the symbol */
    const uint32_t **adc_channel_after_swap_p =
        hutils_lookup_symbol ("pvar_const_uint32_t_p_", self->board_type->name, 
            "_adcswap_chan_id");
    ASSERT_ALLOC(adc_channel_after_swap_p, err_lookup_sym);
    self->adc_chanswap_id = **adc_channel_after_swap_p;
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO, "[sm_io:libbpmclient] adc_channel_after_swap: %u\n", 
        self->adc_chanswap_id);

    _configure_request (self, samples_pre, samples_post, num_shots);
    _setup_transaction (self);

    self->bpm_parameters = zmalloc (sizeof (*bpm_parameters));
    memcpy (self->bpm_parameters, bpm_parameters, sizeof (*bpm_parameters));

    self->trigger_type = ACQ_CLIENT_TRIG_DATA_DRIVEN;
    self->trigger_slope = POSITIVE_SLOPE;
    self->trigger_hysteresis_samples = 1;
    self->trigger_delay = 0;
    self->trigger_active_sample = FIRST_SAMPLE;
    self->trigger_threshold = 1;

    return self;

err_lookup_sym:
err_halcs_get_init_board_type:
    halcs_client_destroy (&halcs_client);
err_halcs_client_new:
    free (self->board_type);
    free (self->service);
    free (self);
    return NULL;
}

void bpm_single_pass_destroy (bpm_single_pass_t **self_p)
{
    assert (self_p);

    if (*self_p) {
        bpm_single_pass_t *self = *self_p;

        _release_transaction (self);
        acq_client_destroy (&self->acq_client);

        free (self->board_type);
        free (self->service);
        free (self->bpm_parameters);
        free (self);

        *self_p = NULL;
    }
}

void bpm_single_pass_configure_trigger (bpm_single_pass_t *self,
        uint32_t hysteresis_samples, uint32_t slope, uint32_t trigger_delay)
{
    self->trigger_slope = slope;
    self->trigger_hysteresis_samples = hysteresis_samples;
    self->trigger_delay = trigger_delay;
}

void bpm_single_pass_configure_data_trigger (bpm_single_pass_t *self,
        uint32_t threshold, uint32_t active_sample)
{
    self->trigger_type = ACQ_CLIENT_TRIG_DATA_DRIVEN;
    self->trigger_threshold = threshold;
    self->trigger_active_sample = active_sample;
}

void bpm_single_pass_configure_external_trigger (bpm_single_pass_t *self)
{
    self->trigger_type = ACQ_CLIENT_TRIG_EXTERNAL;
}

halcs_client_err_e bpm_single_pass_start (bpm_single_pass_t *self)
{
    assert (self);

    CHECK_ERR (_configure_trigger (self));

    return acq_start (self->acq_client, self->service, &self->request);
}

halcs_client_err_e bpm_single_pass_check (bpm_single_pass_t *self)
{
    assert (self);

    return acq_check (self->acq_client, self->service);
}

halcs_client_err_e bpm_single_pass_sample (bpm_single_pass_t *self,
        bpm_sample_t* sample)
{
    assert (self);
    assert (sample);

    CHECK_ERR (acq_get_curve (self->acq_client, self->service,
                &self->transaction));

    _process_single_pass_sample (self, sample);

    return HALCS_CLIENT_SUCCESS;
}

const acq_req_t *bpm_single_pass_get_acq_request (bpm_single_pass_t *self)
{
    assert (self);
    return &self->request;
}

const acq_trans_t *bpm_single_pass_get_acq_transaction (bpm_single_pass_t *self)
{
    assert (self);
    return &self->transaction;
}

static void _configure_request (bpm_single_pass_t *self, uint32_t samples_pre,
        uint32_t samples_post, uint32_t num_shots)
{
    acq_req_t *request = &self->request;

    request->num_samples_pre = samples_pre;
    request->num_samples_post = samples_post;
    request->num_shots = num_shots;
    request->chan = self->adc_chanswap_id;
}

static halcs_client_err_e _configure_trigger (bpm_single_pass_t *self)
{
    acq_client_t* acq_client = self->acq_client;
    char *service = self->service;

    uint32_t trigger_type = self->trigger_type;
    uint32_t slope = self->trigger_slope;
    uint32_t hysteresis = self->trigger_hysteresis_samples;
    uint32_t delay = self->trigger_delay;

    CHECK_ERR (acq_set_trig (acq_client, service, trigger_type));
    CHECK_ERR (acq_set_data_trig_pol (acq_client, service, slope));
    CHECK_ERR (acq_set_data_trig_filt (acq_client, service, hysteresis));
    CHECK_ERR (acq_set_hw_trig_dly (acq_client, service, delay));

    if (trigger_type == ACQ_CLIENT_TRIG_DATA_DRIVEN) {
        uint32_t threshold = self->trigger_threshold;
        uint32_t active_sample = self->trigger_active_sample;

        CHECK_ERR (acq_set_data_trig_thres (acq_client, service, threshold));
        CHECK_ERR (acq_set_data_trig_sel (acq_client, service, active_sample));
        CHECK_ERR (acq_set_data_trig_chan (acq_client, service, self->adc_chanswap_id));
    }

    return HALCS_CLIENT_SUCCESS;
}

static void _setup_transaction (bpm_single_pass_t *self)
{
    acq_client_t* acq_client = self->acq_client;
    char *service = self->service;
    acq_req_t *request = &self->request;
    acq_trans_t *transaction = &self->transaction;
    uint32_t chan = self->request.chan;
    uint32_t sample_size = 0;

    uint32_t num_samples = (request->num_samples_pre + request->num_samples_post) *
            request->num_shots;
    halcs_client_err_e err = halcs_get_acq_ch_sample_size (acq_client, service, chan, &sample_size);
    ASSERT_TEST(err == HALCS_CLIENT_SUCCESS, "Could not get sample size",
        err_get_acq_prop);

    memcpy (&transaction->req, request, sizeof (transaction->req));

    transaction->block.data = zmalloc (num_samples * sample_size);
    transaction->block.data_size = num_samples * sample_size;

err_get_acq_prop:
    return;
}

static void _process_single_pass_sample (bpm_single_pass_t *self,
        bpm_sample_t *sample)
{
    DBE_DEBUG (DBG_LIB_CLIENT | DBG_LVL_TRACE, "[libbpmclient] "
            "_process_single_pass_sample: Calculating position sample after "
            "data acquisition\n");

    acq_req_t *request = &self->request;
    uint32_t num_samples = (request->num_samples_pre + request->num_samples_post) *
            request->num_shots;
    double a_energy = 0.0;
    double b_energy = 0.0;
    double c_energy = 0.0;
    double d_energy = 0.0;
    double a_sum = 0.0;
    double b_sum = 0.0;
    double c_sum = 0.0;
    double d_sum = 0.0;
    double a_energy_ac = 0.0;
    double b_energy_ac = 0.0;
    double c_energy_ac = 0.0;
    double d_energy_ac = 0.0;
    uint32_t atom_width = 0;
    uint32_t num_atoms = 0;

    int err = halcs_get_acq_ch_atom_width (self->acq_client, self->service, self->request.chan, &atom_width);
    err |= halcs_get_acq_ch_num_atoms (self->acq_client, self->service, self->request.chan, &num_atoms);
    ASSERT_TEST(err == HALCS_CLIENT_SUCCESS, "Could not get channel properties",
        err_get_acq_prop);
    ASSERT_TEST(num_atoms == 4, "Single-Pass processing is only valid for num_atoms = 4",
        err_num_atoms);

    /* Convert bit to byte */
    atom_width /= 8;

    uint8_t *raw_data = (uint8_t*) self->transaction.block.data;

    for (uint32_t i = 0; i < num_samples; ++i) {
        /* Use the correct BPM convenction for channel 0, 1, 2 and 3
         * A = ch0
         * B = ch2
         * C = ch1
         * D = ch3
         */
        double a_sample = _cast_to_double (raw_data + atom_width*((i*num_atoms)+0), atom_width);
        double c_sample = _cast_to_double (raw_data + atom_width*((i*num_atoms)+1), atom_width);
        double b_sample = _cast_to_double (raw_data + atom_width*((i*num_atoms)+2), atom_width);
        double d_sample = _cast_to_double (raw_data + atom_width*((i*num_atoms)+3), atom_width);

        a_sum += a_sample;
        b_sum += b_sample;
        c_sum += c_sample;
        d_sum += d_sample;

        a_energy += _squared_value (a_sample);
        b_energy += _squared_value (b_sample);
        c_energy += _squared_value (c_sample);
        d_energy += _squared_value (d_sample);
    }

    double a_energy_dc = _squared_value (a_sum) / num_samples;
    double b_energy_dc = _squared_value (b_sum) / num_samples;
    double c_energy_dc = _squared_value (c_sum) / num_samples;
    double d_energy_dc = _squared_value (d_sum) / num_samples;

    DBE_DEBUG (DBG_LIB_CLIENT | DBG_LVL_TRACE, "[libbpmclient] "
            "_process_single_pass_sample: (A^2, B^2, C^2, D^2) = "
            "(%f, %f, %f, %f)\n", a_energy, b_energy, c_energy, d_energy);

    a_energy_ac = a_energy - a_energy_dc;
    b_energy_ac = b_energy - b_energy_dc;
    c_energy_ac = c_energy - c_energy_dc;
    d_energy_ac = d_energy - d_energy_dc;

    double a = sqrt (a_energy_ac);
    double b = sqrt (b_energy_ac);
    double c = sqrt (c_energy_ac);
    double d = sqrt (d_energy_ac);

    DBE_DEBUG (DBG_LIB_CLIENT | DBG_LVL_TRACE, "[libbpmclient] "
            "_process_single_pass_sample: (A, B, C, D) = (%f, %f, %f, %f)\n",
            a, b, c, d);

    _calculate_bpm_sample (self->bpm_parameters, a, b, c, d, sample, true);

err_num_atoms:
err_get_acq_prop:
    return;
}

static double _cast_to_double (uint8_t *adc_sample, uint32_t atom_size)
{
    double value = 0.0;
    switch (atom_size) {
        case 2:
            value = (double) *(int16_t *) adc_sample;
        break;

        case 4:
            value = (double) *(int32_t *) adc_sample;
        break;

        case 8:
            value = (double) *(int64_t *) adc_sample;
        break;

        /* Invalid value */
        default:
            value = -1;
            DBE_DEBUG (DBG_LIB_CLIENT | DBG_LVL_FATAL, "[libbpmclient] "
                    "_cast_to_double: invalid atom_size = %u. ",
                    atom_size);
        break;
    }

    return value;
}

static double _squared_value (double value)
{
    return value * value;
}

static void _calculate_bpm_sample_std (bpm_parameters_t *parameters, double a,
        double b, double c, double d, bpm_sample_t *sample)
{
    double sum = a + b + c + d;

    double kx = parameters->kx;
    double ky = parameters->ky;
    double kq = parameters->kq;
    double ksum = parameters->ksum;

    double offset_x = parameters->offset_x;
    double offset_y = parameters->offset_y;
    double offset_q = parameters->offset_q;

    sample->a = a;
    sample->b = b;
    sample->c = c;
    sample->d = d;

    sample->x = kx * (a - b - c + d) / sum - offset_x;
    sample->y = ky * (a + b - c - d) / sum - offset_y;
    sample->q = kq * (a - b + c - d) / sum - offset_q;
    sample->sum = ksum * sum;

    DBE_DEBUG (DBG_LIB_CLIENT | DBG_LVL_TRACE, "[libbpmclient] "
            "_calculate_bpm_sample_std: (A, B, C, D, X, Y, Q, SUM) = "
            "(%f, %f, %f, %f, %f, %f, %f, %f)\n",
            sample->a, sample->b, sample->c, sample->d,
            sample->x, sample->y, sample->q, sample->sum);
}

static void _calculate_bpm_sample_partial (bpm_parameters_t *parameters, double a,
        double b, double c, double d, bpm_sample_t *sample)
{
    double sum_ac = a + c;
    double sum_bd = b + d;
    double sum_ab = a + b;
    double sum_cd = c + d;
    double sum = sum_ac + sum_bd;

    double kx = parameters->kx;
    double ky = parameters->ky;
    double kq = parameters->kq;
    double ksum = parameters->ksum;

    double offset_x = parameters->offset_x;
    double offset_y = parameters->offset_y;
    double offset_q = parameters->offset_q;

    double partial_ac_pos_x = kx * (a - c) / (2 * sum_ac);
    double partial_bd_pos_x = kx * (b - d) / (2 * sum_bd);

    double partial_ac_pos_y = ky * (a - c) / (2 * sum_ac);
    double partial_bd_pos_y = ky * (b - d) / (2 * sum_bd);

    double partial_ab_pos_q = kq * (a - b) / (2 * sum_ab);
    double partial_cd_pos_q = kq * (c - d) / (2 * sum_cd);

    sample->a = a;
    sample->b = b;
    sample->c = c;
    sample->d = d;

    sample->x = partial_ac_pos_x - partial_bd_pos_x - offset_x;
    sample->y = partial_ac_pos_y + partial_bd_pos_y - offset_y;
    sample->q = partial_ab_pos_q + partial_cd_pos_q - offset_q;
    sample->sum = ksum * sum;

    DBE_DEBUG (DBG_LIB_CLIENT | DBG_LVL_TRACE, "[libbpmclient] "
            "_calculate_bpm_sample_partial: (A, B, C, D, X, Y, Q, SUM) = "
            "(%f, %f, %f, %f, %f, %f, %f, %f)\n",
            sample->a, sample->b, sample->c, sample->d,
            sample->x, sample->y, sample->q, sample->sum);
}

static void _calculate_bpm_sample (bpm_parameters_t *parameters, double a,
        double b, double c, double d, bpm_sample_t *sample, bool partial_delta)
{
    if (partial_delta) {
        _calculate_bpm_sample_partial (parameters, a, b, c, d, sample);
    }
    else {
        _calculate_bpm_sample_std (parameters, a, b, c, d, sample);
    }
}

static void _release_transaction (bpm_single_pass_t *self)
{
    acq_trans_t *transaction = &self->transaction;

    free (transaction->block.data);

    transaction->block.data = NULL;
    transaction->block.data_size = 0;
}
