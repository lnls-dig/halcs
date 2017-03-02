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

/* Our structure */
struct _bpm_single_pass_t {
    acq_client_t *acq_client;               /* Acquisition client interface */
    char *service;                          /* Acquisition client service */
    bpm_parameters_t* bpm_parameters;       /* BPM parameters */
    acq_req_t request;                      /* Acquisition request parameters */
    uint32_t threshold;                     /* Trigger threshold */
    uint32_t trigger_samples;               /* Trigger hysteresis filter */
};

/********************************************************/
/************************ Our API ***********************/
/********************************************************/

static halcs_client_err_e _configure_trigger(bpm_single_pass_t *self);

static void _configure_request (bpm_single_pass_t *self, uint32_t samples_pre,
        uint32_t samples_post);

static void _setup_transaction (bpm_single_pass_t *self,
        acq_trans_t *transaction);

static void _process_single_pass_sample (bpm_single_pass_t *self,
        void *buffer, bpm_sample_t *sample);

static double _squared_value (uint16_t adc_sample);

static void _calculate_bpm_sample (bpm_parameters_t *parameters, double a,
        double b, double c, double d, bpm_sample_t *sample);

static void _release_transaction (acq_trans_t *transaction);

bpm_single_pass_t *bpm_single_pass_new (acq_client_t *acq_client, char *service,
        bpm_parameters_t *bpm_parameters, uint32_t samples_pre,
        uint32_t samples_post, uint32_t threshold, uint32_t trigger_samples)
{
    assert (acq_client);
    assert (service);

    bpm_single_pass_t *self = zmalloc (sizeof (bpm_single_pass_t));

    self->acq_client = acq_client;
    self->service = strdup (service);

    _configure_request (self, samples_pre, samples_post);

    self->bpm_parameters = bpm_parameters;
    self->threshold = threshold;
    self->trigger_samples = trigger_samples;

    return self;
}

void bpm_single_pass_destroy (bpm_single_pass_t **self_p)
{
    assert (self_p);

    if (*self_p) {
        bpm_single_pass_t *self = *self_p;
        free (self->service);
        free (self);
        *self_p = NULL;
    }
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

    acq_trans_t transaction;

    _setup_transaction (self, &transaction);

    CHECK_ERR (acq_get_curve (self->acq_client, self->service, &transaction));

    _process_single_pass_sample (self, transaction.block.data, sample);
    _release_transaction (&transaction);

    return HALCS_CLIENT_SUCCESS;
}

static void _configure_request (bpm_single_pass_t *self, uint32_t samples_pre,
        uint32_t samples_post)
{
    const uint32_t ADC_AFTER_UNSWAP = ADC_CHANNEL_ID_TO_USE;

    acq_req_t *request = &self->request;

    request->num_samples_pre = samples_pre;
    request->num_samples_post = samples_post;
    request->num_shots = 1;
    request->chan = ADC_AFTER_UNSWAP;
}

static halcs_client_err_e _configure_trigger(bpm_single_pass_t *self)
{
    acq_client_t* acq_client = self->acq_client;
    char *service = self->service;

    const uint32_t DATA_TRIGGER = 2;
    const uint32_t POSITIVE_SLOPE = 0;
    const uint32_t FIRST_SAMPLE = 0;
    const uint32_t ADC_AFTER_UNSWAP = ADC_CHANNEL_ID_TO_USE;

    uint32_t threshold = self->threshold;
    uint32_t trigger_samples = self->trigger_samples;

    CHECK_ERR (acq_set_trig (acq_client, service, DATA_TRIGGER));
    CHECK_ERR (acq_set_data_trig_thres (acq_client, service, threshold));
    CHECK_ERR (acq_set_data_trig_pol (acq_client, service, POSITIVE_SLOPE));
    CHECK_ERR (acq_set_data_trig_sel (acq_client, service, FIRST_SAMPLE));
    CHECK_ERR (acq_set_data_trig_filt (acq_client, service, trigger_samples));
    CHECK_ERR (acq_set_data_trig_chan (acq_client, service, ADC_AFTER_UNSWAP));

    return HALCS_CLIENT_SUCCESS;
}

static void _setup_transaction (bpm_single_pass_t *self,
        acq_trans_t *transaction)
{
    const acq_chan_t *channels = acq_get_chan (self->acq_client);
    acq_req_t *request = &self->request;

    uint32_t num_samples = request->num_samples_pre + request->num_samples_post;
    uint32_t sample_size = channels[request->chan].sample_size;

    assert (sample_size == 4 * sizeof (uint16_t));

    memcpy (&transaction->req, request, sizeof (transaction->req));

    transaction->block.data = zmalloc (num_samples * sample_size);
    transaction->block.data_size = num_samples * sample_size;
}

static void _process_single_pass_sample (bpm_single_pass_t *self,
        void *buffer, bpm_sample_t *sample)
{
    acq_req_t *request = &self->request;
    uint32_t num_samples = request->num_samples_pre + request->num_samples_post;

    double a = 0.0;
    double b = 0.0;
    double c = 0.0;
    double d = 0.0;

    uint16_t *buffer_values = (uint16_t*) buffer;

    for (uint32_t index = 0; index < num_samples; ++index) {
        a += _squared_value (buffer_values[0]);
        b += _squared_value (buffer_values[1]);
        c += _squared_value (buffer_values[2]);
        d += _squared_value (buffer_values[3]);

        buffer_values += 4;
    }

    a = sqrt(a);
    b = sqrt(b);
    c = sqrt(c);
    d = sqrt(d);

    _calculate_bpm_sample (self->bpm_parameters, a, b, c, d, sample);
}

static double _squared_value (uint16_t adc_sample)
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

    sample->x = (uint32_t) (kx * (d - b) / sum + offset_x);
    sample->y = (uint32_t) (ky * (c - a) / sum + offset_y);
    sample->q = (uint32_t) (kq * (a - b + c - d) / sum + offset_q);
    sample->sum = (uint32_t) (ksum * sum);
}

static void _release_transaction (acq_trans_t *transaction)
{
    free (transaction->block.data);

    transaction->block.data = NULL;
    transaction->block.data_size = 0;
}
