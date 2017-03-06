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
    double x;
    double y;
    double q;
    double sum;
} bpm_sample_t;

/* Prepare for a single-pass acquisition. */
bpm_single_pass_t *bpm_single_pass_new (acq_client_t *acq_client, char *service,
        bpm_parameters_t *bpm_parameters, uint32_t samples_pre,
        uint32_t samples_post);

/* Free resources used for single-pass acquisition */
void bpm_single_pass_destroy (bpm_single_pass_t **self_p);

/* Configure generic trigger parameters: hysteresis filter samples and
 * selection of rising or falling signal slope. */
void bpm_single_pass_configure_trigger (bpm_single_pass_t *self,
        uint32_t hysteresis_samples, uint32_t slope);

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

#ifdef __cplusplus
}
#endif

#endif
