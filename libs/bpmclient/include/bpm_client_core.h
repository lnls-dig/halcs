/*
 * Copyright (C) 2017 LNLS (www.lnls.br)
 * Author: Janito Vaqueiro Ferreira Filho <janito.filho@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _BPM_CLIENT_CORE_H_
#define _BPM_CLIENT_CORE_H_

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************/
/************************ Our API ***********************/
/********************************************************/

/********************* BPM Functions ********************/

/* BPM physical parameters */
typedef struct {
    double kx;
    double ky;
    double kq;
    double ksum;
    double offset_x;
    double offset_y;
    double offset_q;
} bpm_parameters_t;

/* BPM measurement sample */
typedef struct {
    double a;
    double b;
    double c;
    double d;
    double x;
    double y;
    double q;
    double sum;
} bpm_sample_t;

/* Create an instance of the single-pass acquisition client. The single-pass
 * parameters are the service to connect to, the parameters of the BPM, and the
 * number of samples to acquire before and after the trigger. */
bpm_single_pass_t *bpm_single_pass_new (char *broker_endp, int verbose,
        const char *log_file_name, char *service,
        bpm_parameters_t *bpm_parameters, uint32_t samples_pre,
        uint32_t samples_post);

/* Create an instance of the single-pass acquisition client. The single-pass
 * parameters are the service to connect to, the parameters of the BPM, and the
 * number of samples to acquire before and after the trigger. This variation
 * allows specifying the send/recv timeout in ms. */
bpm_single_pass_t *bpm_single_pass_new_time (char *broker_endp, int verbose,
        const char *log_file_name, int timeout, char *service,
        bpm_parameters_t *bpm_parameters, uint32_t samples_pre,
        uint32_t samples_post);

/* Create an instance of the single-pass acquisition client, with the log
 * filemode specified by "log_mode" as in fopen () call. The single-pass
 * parameters are the service to connect to, the parameters of the BPM, and the
 * number of samples to acquire before and after the trigger. */
bpm_single_pass_t *bpm_single_pass_new_log_mode (char *broker_endp, int verbose,
        const char *log_file_name, const char *log_mode, char *service,
        bpm_parameters_t *bpm_parameters, uint32_t samples_pre,
        uint32_t samples_post);

/* Create an instance of the single-pass acquisition client, with the log
 * filemode specified * by "log_mode" as in fopen () call, and the send/recv
 * timeout in ms. The single-pass parameters are the service to connect to, the
 * parameters of the BPM, and the number of samples to acquire before and after
 * the trigger. */
bpm_single_pass_t *bpm_single_pass_new_log_mode_time (char *broker_endp,
        int verbose, const char *log_file_name, const char *log_mode,
        int timeout, char *service, bpm_parameters_t *bpm_parameters,
        uint32_t samples_pre, uint32_t samples_post);

/* Free resources used for single-pass acquisition */
void bpm_single_pass_destroy (bpm_single_pass_t **self_p);

/* Configure generic trigger parameters: hysteresis filter samples and
 * selection of rising or falling signal slope. */
void bpm_single_pass_configure_trigger (bpm_single_pass_t *self,
        uint32_t hysteresis_samples, uint32_t slope, uint32_t trigger_delay);

/* Configure data threshold trigger parameters. */
void bpm_single_pass_configure_data_trigger (bpm_single_pass_t *self,
        uint32_t threshold, uint32_t active_sample);

/* Prepare for an external hardware trigger. */
void bpm_single_pass_configure_external_trigger (bpm_single_pass_t *self);

/* Start single-pass acquisition */
halcs_client_err_e bpm_single_pass_start (bpm_single_pass_t *self);

/* Check if single-pass acquisition has completed */
halcs_client_err_e bpm_single_pass_check (bpm_single_pass_t *self);

/* Calculate single-pass information from acquired data */
halcs_client_err_e bpm_single_pass_sample (bpm_single_pass_t *self,
        bpm_sample_t *sample);

/* Get acquisition request structure */
const acq_req_t *bpm_single_pass_get_acq_request (bpm_single_pass_t *self);

/* Get acquisition transaction structure */
const acq_trans_t *bpm_single_pass_get_acq_transaction (bpm_single_pass_t *self);

#ifdef __cplusplus
}
#endif

#endif
