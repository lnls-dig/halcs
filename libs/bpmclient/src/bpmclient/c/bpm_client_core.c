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

#ifdef ADCSWAP_CHAN_ID
#   define ADC_CHANNEL_ID_TO_USE ADCSWAP_CHAN_ID
#else
#   define ADC_CHANNEL_ID_TO_USE ADC_CHAN_ID
#endif

#define POSITIVE_SLOPE 0
#define FIRST_SAMPLE 0

/* Our structure */
struct _bpm_single_pass_t {
    acq_client_t *acq_client;               /* Acquisition client interface */
    char *service;                          /* Acquisition client service */
    bpm_parameters_t* bpm_parameters;       /* BPM parameters */
    acq_req_t request;                      /* Acquisition request parameters */
    acq_trans_t transaction;                /* Acquisition transaction */

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
        char *service, bpm_parameters_t *bpm_parameters, uint32_t samples_pre,
        uint32_t samples_post, uint32_t num_shots);

static halcs_client_err_e _configure_trigger (bpm_single_pass_t *self);

static void _configure_request (bpm_single_pass_t *self, uint32_t samples_pre,
        uint32_t samples_post, uint32_t num_shots);

static void _setup_transaction (bpm_single_pass_t *self);

static void _process_single_pass_sample (bpm_single_pass_t *self,
        bpm_sample_t *sample);

static double _squared_value (int16_t adc_sample);

static void _calculate_bpm_sample (bpm_parameters_t *parameters, double a,
        double b, double c, double d, bpm_sample_t *sample);

static void _release_transaction (bpm_single_pass_t *self);

bpm_single_pass_t *bpm_single_pass_new (char *broker_endp, int verbose,
        const char *log_file_name, char *service,
        bpm_parameters_t *bpm_parameters, uint32_t samples_pre,
        uint32_t samples_post, uint32_t num_shots)
{
    acq_client_t *acq_client = acq_client_new (broker_endp, verbose,
        log_file_name);

    ASSERT_ALLOC (acq_client, err_acq_client_new);

    return _bpm_single_pass_new (acq_client, service, bpm_parameters,
            samples_pre, samples_post, num_shots);

err_acq_client_new:
    return NULL;
}

bpm_single_pass_t *bpm_single_pass_new_time (char *broker_endp, int verbose,
        const char *log_file_name, int timeout, char *service,
        bpm_parameters_t *bpm_parameters, uint32_t samples_pre,
        uint32_t samples_post, uint32_t num_shots)
{
    acq_client_t *acq_client = acq_client_new_time (broker_endp, verbose,
        log_file_name, timeout);

    ASSERT_ALLOC (acq_client, err_acq_client_new);

    return _bpm_single_pass_new (acq_client, service, bpm_parameters,
            samples_pre, samples_post, num_shots);

err_acq_client_new:
    return NULL;
}

bpm_single_pass_t *bpm_single_pass_new_log_mode (char *broker_endp, int verbose,
        const char *log_file_name, const char *log_mode, char *service,
        bpm_parameters_t *bpm_parameters, uint32_t samples_pre,
        uint32_t samples_post, uint32_t num_shots)
{
    acq_client_t *acq_client = acq_client_new_log_mode (broker_endp, verbose,
        log_file_name, log_mode);

    ASSERT_ALLOC (acq_client, err_acq_client_new);

    return _bpm_single_pass_new (acq_client, service, bpm_parameters,
            samples_pre, samples_post, num_shots);

err_acq_client_new:
    return NULL;
}

bpm_single_pass_t *bpm_single_pass_new_log_mode_time (char *broker_endp,
        int verbose, const char *log_file_name, const char *log_mode,
        int timeout, char *service, bpm_parameters_t *bpm_parameters,
        uint32_t samples_pre, uint32_t samples_post, uint32_t num_shots)
{
    acq_client_t *acq_client = acq_client_new_log_mode_time (broker_endp,
        verbose, log_file_name, log_mode, timeout);

    ASSERT_ALLOC (acq_client, err_acq_client_new);

    return _bpm_single_pass_new (acq_client, service, bpm_parameters,
            samples_pre, samples_post, num_shots);

err_acq_client_new:
    return NULL;
}

static bpm_single_pass_t *_bpm_single_pass_new (acq_client_t *acq_client,
        char *service, bpm_parameters_t *bpm_parameters, uint32_t samples_pre,
        uint32_t samples_post, uint32_t num_shots)
{
    assert (service);

    bpm_single_pass_t *self = zmalloc (sizeof (bpm_single_pass_t));

    self->acq_client = acq_client;
    self->service = strdup (service);

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
}

void bpm_single_pass_destroy (bpm_single_pass_t **self_p)
{
    assert (self_p);

    if (*self_p) {
        bpm_single_pass_t *self = *self_p;

        _release_transaction (self);
        acq_client_destroy (&self->acq_client);

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
    const uint32_t ADC_AFTER_UNSWAP = ADC_CHANNEL_ID_TO_USE;

    acq_req_t *request = &self->request;

    request->num_samples_pre = samples_pre;
    request->num_samples_post = samples_post;
    request->num_shots = num_shots;
    request->chan = ADC_AFTER_UNSWAP;
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
        const uint32_t ADC_CHANNEL = ADC_CHANNEL_ID_TO_USE;

        uint32_t threshold = self->trigger_threshold;
        uint32_t active_sample = self->trigger_active_sample;

        CHECK_ERR (acq_set_data_trig_thres (acq_client, service, threshold));
        CHECK_ERR (acq_set_data_trig_sel (acq_client, service, active_sample));
        CHECK_ERR (acq_set_data_trig_chan (acq_client, service, ADC_CHANNEL));
    }

    return HALCS_CLIENT_SUCCESS;
}

static void _setup_transaction (bpm_single_pass_t *self)
{
    const acq_chan_t *channels = acq_get_chan (self->acq_client);
    acq_req_t *request = &self->request;
    acq_trans_t *transaction = &self->transaction;

    uint32_t num_samples = (request->num_samples_pre + request->num_samples_post) * 
            request->num_shots;
    uint32_t sample_size = channels[request->chan].sample_size;

    assert (sample_size == 4 * sizeof (uint16_t));

    memcpy (&transaction->req, request, sizeof (transaction->req));

    transaction->block.data = zmalloc (num_samples * sample_size);
    transaction->block.data_size = num_samples * sample_size;
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

    double a = 0.0;
    double b = 0.0;
    double c = 0.0;
    double d = 0.0;

    int16_t *buffer_values = (int16_t*) self->transaction.block.data;

    for (uint32_t index = 0; index < num_samples; ++index) {
        a += _squared_value (buffer_values[0]);
        b += _squared_value (buffer_values[1]);
        c += _squared_value (buffer_values[2]);
        d += _squared_value (buffer_values[3]);

        buffer_values += 4;
    }

    DBE_DEBUG (DBG_LIB_CLIENT | DBG_LVL_TRACE, "[libbpmclient] "
            "_process_single_pass_sample: (A^2, B^2, C^2, D^2) = "
            "(%f, %f, %f, %f)\n", a, b, c, d);

    a = sqrt (a);
    b = sqrt (b);
    c = sqrt (c);
    d = sqrt (d);

    DBE_DEBUG (DBG_LIB_CLIENT | DBG_LVL_TRACE, "[libbpmclient] "
            "_process_single_pass_sample: (A, B, C, D) = (%f, %f, %f, %f)\n",
            a, b, c, d);

    _calculate_bpm_sample (self->bpm_parameters, a, b, c, d, sample);
}

static double _squared_value (int16_t adc_sample)
{
    double value = (double) adc_sample;

    return value * value;
}

static void _calculate_bpm_sample (bpm_parameters_t *parameters, double a,
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
            "_calculate_bpm_sample: (A, B, C, D, X, Y, Q, SUM) = "
            "(%f, %f, %f, %f, %f, %f, %f, %f)\n",
            sample->a, sample->b, sample->c, sample->d,
            sample->x, sample->y, sample->q, sample->sum);
}

static void _release_transaction (bpm_single_pass_t *self)
{
    acq_trans_t *transaction = &self->transaction;

    free (transaction->block.data);

    transaction->block.data = NULL;
    transaction->block.data_size = 0;
}
