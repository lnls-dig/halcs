/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _BPM_CLIENT_H_
#define _BPM_CLIENT_H_

#include <inttypes.h>
#include <mdp.h>

#include "bpm_client_codes.h"
#include "bpm_client_err.h"
#include "acq_chan.h"           /* SMIO acq channel definition */

struct _acq_chan_t;

/* Our structure */
struct _bpm_client_t {
    mdp_client_t *mdp_client;                   /* Majordomo client instance */
    const struct _acq_chan_t *acq_chan;         /* Acquisition buffer table */
};

typedef struct _bpm_client_t bpm_client_t;

/********************************************************/
/************************ Our API ***********************/
/********************************************************/

/* Create an instance of the BPM client. This must be called
 * before any operation involving communicating with the BPM
 * server. Return an instance of the bpm client */
bpm_client_t *bpm_client_new (char *broker_endp, int verbose,
        const char *log_file_name);

/* Create an instance of the BPM client, with the log filemode specified
 * by "log_mode" as in fopen () call. This must be called before any operation
 * involving communicating with the BPM server. Return an instance of the bpm
 * client */
bpm_client_t *bpm_client_new_log_mode (char *broker_endp, int verbose,
        const char *log_file_name, const char *log_mode);

/* Destroy an instance of the BPM client. This must be called
 * after all operations involving the communication with the BPM
 * server */
void bpm_client_destroy (bpm_client_t **self_p);

/* General function to execute all the other modules functions */
bpm_client_err_e bpm_func_exec (bpm_client_t *self, const disp_op_t *func,
        char *service, uint32_t *input, uint32_t *output);

/* Translate function's name and returns its structure */
const disp_op_t* bpm_func_translate (char *name);

/* Wrapper to bpm_func_exec which translates the function name to
 * its exp_ops structure */
 bpm_client_err_e bpm_func_trans_exec (bpm_client_t *self, char *name,
        char *service, uint32_t *input, uint32_t *output);
/******************** FMC130M SMIO Functions ******************/

/* Blink the FMC Leds. This is only used for debug and for demostration
 * purposes.
 * Returns BPM_CLIENT_SUCCESS if ok and BPM_CLIIENT_ERR_SERVER if
 * if server could not complete the request */
bpm_client_err_e bpm_blink_leds (bpm_client_t *self, char *service, uint32_t leds);

/* Simple AD9510 Config test.
 * Returns BPM_CLIENT_SUCCESS if ok and BPM_CLIIENT_ERR_SERVER if
 * if server could not complete the request */
bpm_client_err_e bpm_ad9510_cfg_defaults (bpm_client_t *self, char *service);

/* FMC PLL FUNCTION pin. Sets or clears the FMC PLL FUNCTION pin. This pin
 * has a general purpose based on the 0x59 SPI AD9510 register.
 * Returns BPM_CLIENT_SUCCESS if ok and BPM_CLIIENT_ERR_SERVER if
 * if server could not complete the request */
bpm_client_err_e bpm_set_fmc_pll_function (bpm_client_t *self, char *service,
        uint32_t fmc_pll_function);
bpm_client_err_e bpm_get_fmc_pll_function (bpm_client_t *self, char *service,
        uint32_t *fmc_pll_function);

/* RAW ADC data functions */
/* These set of functions read (get) the RAW ADC values.
 * All of the functions returns BPM_CLIENT_SUCCESS if the
 * parameter was correctly set or error (see bpm_client_err.h
 * for all possible errors)*/
bpm_client_err_e bpm_set_adc_data0 (bpm_client_t *self, char *service,
        uint32_t adc_data0);
bpm_client_err_e bpm_get_adc_data0 (bpm_client_t *self, char *service,
        uint32_t *adc_data0);
bpm_client_err_e bpm_set_adc_data1 (bpm_client_t *self, char *service,
        uint32_t adc_data1);
bpm_client_err_e bpm_get_adc_data1 (bpm_client_t *self, char *service,
        uint32_t *adc_data1);
bpm_client_err_e bpm_set_adc_data2 (bpm_client_t *self, char *service,
        uint32_t adc_data2);
bpm_client_err_e bpm_get_adc_data2 (bpm_client_t *self, char *service,
        uint32_t *adc_data2);
bpm_client_err_e bpm_set_adc_data3 (bpm_client_t *self, char *service,
        uint32_t adc_data3);
bpm_client_err_e bpm_get_adc_data3 (bpm_client_t *self, char *service,
        uint32_t *adc_data3);

/* ADC delay value functions */

/* The three set of group functions provide a low-lovel interface to the FPGA
 * firmware. The correct usage to set the ADC clock and data paths delay are:
 *
 * 1) Set the delay value with bpm_set_adc_dly_val<*> for the desired channel.
 *      Accepted values are from 0 to 31.
 * 2) Set the delay line which will be updated with bpm_set_adc_dly_line<*> for the desired channel
 *      Accepted values are the bitmask of the desired lines with bit 16 corresponding
 *      to the clock line and bits 15-0 to ADC bits 15 to 0
 * 3) call the update functions bpm_set_adc_dly_updt<*> for the desired channel.
 */

/* These set of functions read/write (set/get) the ADC delay values.
 * All of the functions returns BPM_CLIENT_SUCCESS if the
 * parameter was correctly set or error (see bpm_client_err.h
 * for all possible errors)*/
bpm_client_err_e bpm_set_adc_dly_val0 (bpm_client_t *self, char *service,
        uint32_t adc_dly_val0);
bpm_client_err_e bpm_get_adc_dly_val0 (bpm_client_t *self, char *service,
        uint32_t *adc_dly_val0);
bpm_client_err_e bpm_set_adc_dly_val1 (bpm_client_t *self, char *service,
        uint32_t adc_dly_val1);
bpm_client_err_e bpm_get_adc_dly_val1 (bpm_client_t *self, char *service,
        uint32_t *adc_dly_val1);
bpm_client_err_e bpm_set_adc_dly_val2 (bpm_client_t *self, char *service,
        uint32_t adc_dly_val2);
bpm_client_err_e bpm_get_adc_dly_val2 (bpm_client_t *self, char *service,
        uint32_t *adc_dly_val2);
bpm_client_err_e bpm_set_adc_dly_val3 (bpm_client_t *self, char *service,
        uint32_t adc_dly_val3);
bpm_client_err_e bpm_get_adc_dly_val3 (bpm_client_t *self, char *service,
        uint32_t *adc_dly_val3);

/* ADC delay line functions */
/* These set of functions read/write (set/get) the ADC delay line values,
 * meaning which lines (16 = clock, 15-0 = data bits) are to be updated
 * on bpm_set_adc_dly_updt<*> functions.
 * All of the functions returns BPM_CLIENT_SUCCESS if the
 * parameter was correctly set or error (see bpm_client_err.h
 * for all possible errors)*/
bpm_client_err_e bpm_set_adc_dly_line0 (bpm_client_t *self, char *service,
        uint32_t adc_dly_line0);
bpm_client_err_e bpm_get_adc_dly_line0 (bpm_client_t *self, char *service,
        uint32_t *adc_dly_line0);
bpm_client_err_e bpm_set_adc_dly_line1 (bpm_client_t *self, char *service,
        uint32_t adc_dly_line1);
bpm_client_err_e bpm_get_adc_dly_line1 (bpm_client_t *self, char *service,
        uint32_t *adc_dly_line1);
bpm_client_err_e bpm_set_adc_dly_line2 (bpm_client_t *self, char *service,
        uint32_t adc_dly_line2);
bpm_client_err_e bpm_get_adc_dly_line2 (bpm_client_t *self, char *service,
        uint32_t *adc_dly_line2);
bpm_client_err_e bpm_set_adc_dly_line3 (bpm_client_t *self, char *service,
        uint32_t adc_dly_line3);
bpm_client_err_e bpm_get_adc_dly_line3 (bpm_client_t *self, char *service,
        uint32_t *adc_dly_line3);

/* ADC update functions */
/* These set of functions read/write (set/get) the ADC delay update values
 * for each channel. This will effectively update the FPGA delay primitives to
 * the previous delay values set by the corresponding bpm_get_adc_dly_updt<*>
 * functions.
 * All of the functions returns BPM_CLIENT_SUCCESS if the
 * parameter was correctly set or error (see bpm_client_err.h
 * for all possible errors)*/
bpm_client_err_e bpm_set_adc_dly_updt0 (bpm_client_t *self, char *service,
        uint32_t adc_dly_updt0);
bpm_client_err_e bpm_get_adc_dly_updt0 (bpm_client_t *self, char *service,
        uint32_t *adc_dly_updt0);
bpm_client_err_e bpm_set_adc_dly_updt1 (bpm_client_t *self, char *service,
        uint32_t adc_dly_updt1);
bpm_client_err_e bpm_get_adc_dly_updt1 (bpm_client_t *self, char *service,
        uint32_t *adc_dly_updt1);
bpm_client_err_e bpm_set_adc_dly_updt2 (bpm_client_t *self, char *service,
        uint32_t adc_dly_updt2);
bpm_client_err_e bpm_get_adc_dly_updt2 (bpm_client_t *self, char *service,
        uint32_t *adc_dly_updt2);
bpm_client_err_e bpm_set_adc_dly_updt3 (bpm_client_t *self, char *service,
        uint32_t adc_dly_updt3);
bpm_client_err_e bpm_get_adc_dly_updt3 (bpm_client_t *self, char *service,
        uint32_t *adc_dly_updt3);

/* Higher-level ADC delay functions */
/* These set of functions write (set) the ADC delays for each channel.
 * This functions wrap all of the functionality present by the 3 groups above
 * in a conveninent way to the user.
 * All of the functions returns BPM_CLIENT_SUCCESS if the
 * parameter was correctly set or error (see bpm_client_err.h
 * for all possible errors)*/
bpm_client_err_e bpm_set_adc_dly0 (bpm_client_t *self, char *service,
        uint32_t dly_type0, uint32_t dly_val0);
bpm_client_err_e bpm_set_adc_dly1 (bpm_client_t *self, char *service,
        uint32_t dly_type1, uint32_t dly_val1);
bpm_client_err_e bpm_set_adc_dly2 (bpm_client_t *self, char *service,
        uint32_t dly_type2, uint32_t dly_val2);
bpm_client_err_e bpm_set_adc_dly3 (bpm_client_t *self, char *service,
        uint32_t dly_type3, uint32_t dly_val3);

/* FMC TEST data enable. Sets or clears the ADC test data switch. This
 * enables or disables the ADC test RAMP output.
 * Returns BPM_CLIENT_SUCCESS if ok and BPM_CLIIENT_ERR_SERVER if
 * if server could not complete the request */
bpm_client_err_e bpm_set_adc_test_data_en (bpm_client_t *self, char *service,
        uint32_t adc_test_data_en);
bpm_client_err_e bpm_get_adc_test_data_en (bpm_client_t *self, char *service,
        uint32_t *adc_test_data_en);

/* FMC SI571 Output Control. Enables or disables the Si571 output.
 * Returns BPM_CLIENT_SUCCESS if ok and BPM_CLIIENT_ERR_SERVER if
 * if server could not complete the request */
bpm_client_err_e bpm_set_si571_oe (bpm_client_t *self, char *service,
        uint32_t si571_oe);
bpm_client_err_e bpm_get_si571_oe (bpm_client_t *self, char *service,
        uint32_t *si571_oe);

/* FMC trigger direction control.
 * Returns BPM_CLIENT_SUCCESS if ok and BPM_CLIIENT_ERR_SERVER if
 * if server could not complete the request */
bpm_client_err_e bpm_set_trig_dir (bpm_client_t *self, char *service,
        uint32_t trig_dir);
bpm_client_err_e bpm_get_trig_dir (bpm_client_t *self, char *service,
        uint32_t *trig_dir);

/* FMC trigger termination control.
 * Returns BPM_CLIENT_SUCCESS if ok and BPM_CLIIENT_ERR_SERVER if
 * if server could not complete the request */
bpm_client_err_e bpm_set_trig_term (bpm_client_t *self, char *service,
        uint32_t trig_term);
bpm_client_err_e bpm_get_trig_term (bpm_client_t *self, char *service,
        uint32_t *trig_term);

/* FMC trigger value control.
 * Returns BPM_CLIENT_SUCCESS if ok and BPM_CLIIENT_ERR_SERVER if
 * if server could not complete the request */
bpm_client_err_e bpm_set_trig_val (bpm_client_t *self, char *service,
        uint32_t trig_val);
bpm_client_err_e bpm_get_trig_val (bpm_client_t *self, char *service,
        uint32_t *trig_val);

/* FMC AD9510 Control.
 * Returns BPM_CLIENT_SUCCESS if ok and BPM_CLIIENT_ERR_SERVER if
 * if server could not complete the request */
bpm_client_err_e bpm_set_ad9510_pll_a_div (bpm_client_t *self, char *service,
        uint32_t ad9510_pll_a_div);
bpm_client_err_e bpm_set_ad9510_pll_b_div (bpm_client_t *self, char *service,
        uint32_t ad9510_pll_b_div);
bpm_client_err_e bpm_set_ad9510_pll_prescaler (bpm_client_t *self, char *service,
        uint32_t ad9510_pll_prescaler);
bpm_client_err_e bpm_set_ad9510_r_div (bpm_client_t *self, char *service,
        uint32_t ad9510_r_div);
bpm_client_err_e bpm_set_ad9510_pll_pdown (bpm_client_t *self, char *service,
        uint32_t ad9510_pll_pdown);
bpm_client_err_e bpm_set_ad9510_mux_status (bpm_client_t *self, char *service,
        uint32_t ad9510_mux_status);
bpm_client_err_e bpm_set_ad9510_cp_current (bpm_client_t *self, char *service,
        uint32_t ad9510_cp_current);
bpm_client_err_e bpm_set_ad9510_outputs (bpm_client_t *self, char *service,
        uint32_t ad9510_outputs);
bpm_client_err_e bpm_set_ad9510_pll_clk_sel (bpm_client_t *self, char *service,
        uint32_t ad9510_pll_clk_sel);

/* FMC SI571 Control.
 * Returns BPM_CLIENT_SUCCESS if ok and BPM_CLIIENT_ERR_SERVER if
 * if server could not complete the request */
bpm_client_err_e bpm_set_si571_set_freq (bpm_client_t *self, char *service,
        double si571_set_freq);
bpm_client_err_e bpm_set_si571_defaults (bpm_client_t *self, char *service,
        double si571_defaults);

/********************** ACQ SMIO Functions ********************/

/* Acquistion request */
struct _acq_req_t {
    uint32_t num_samples;                       /* Number of samples */
    uint32_t chan;                              /* Acquisition channel number */
};

typedef struct _acq_req_t acq_req_t;

/* Acquistion data block */
struct _acq_block_t {
    uint32_t idx;                               /* Block index */

    uint32_t *data;                             /* Block or complete curve read */
    uint32_t data_size;                         /* data_out buffer size */
    uint32_t bytes_read;                        /* Number of bytes effectively read */
};

typedef struct _acq_block_t acq_block_t;

/* Acquistion transaction */
struct _acq_trans_t {
    acq_req_t req;                              /* Request */
    acq_block_t block;                          /* Block or whole curve read */
};

typedef struct _acq_trans_t acq_trans_t;

/* Acquisition channel definitions */
struct _acq_chan_t {
    uint32_t chan;
    uint32_t sample_size;
};

typedef struct _acq_chan_t acq_chan_t;

/* Acquisition channel definitions */
extern acq_chan_t acq_chan[END_CHAN_ID];

/* Start acquisition on a specific channel with an spoecif number of samples,
 * through the use of acq_req_t structure.
 * Returns BPM_CLIENT_SUCCESS if ok and BPM_CLIIENT_ERR_SERVER if the server
 * could not complete the request */
bpm_client_err_e bpm_data_acquire (bpm_client_t *self, char *service,
        acq_req_t *acq_req);

/* Check if apreviouly started acquisition finished.
 * Returns BPM_CLIENT_SUCCESS if ok and BPM_CLIIENT_ERR_AGAIN if the acquistion
 * did not complete */
bpm_client_err_e bpm_check_data_acquire (bpm_client_t *self, char *service);

/* Wait for the previouly started acquistion to complete with a maximum tolerated
 * wait.
 * Returns BPM_CLIENT_SUCCESS if the acquistion finished under the specified
 * timeout or BPM_CLIIENT_ERR_TIMEOUT if the acquistion did not completed in time */
bpm_client_err_e bpm_wait_data_acquire_timed (bpm_client_t *self, char *service,
        int timeout);

/* Get an specific data block from a previously completed acquisiton by setting
 * the desired block index in acq_trans->block.idx and the desired channel in
 * acq_trans->req.channel.
 * Returns BPM_CLIENT_SUCCESS if the block was read or BPM_CLIENT_ERR_SERVER
 * otherwise. The data read is returned in acq_trans->block.data along with
 * the number of bytes effectivly read in acq_trans->block.bytes_read */
bpm_client_err_e bpm_get_data_block (bpm_client_t *self, char *service,
        acq_trans_t *acq_trans);

/* Get a complete curve from a previously completed acquisiton by setting
 * the the desired channel in acq_trans->req.channel.
 * Returns BPM_CLIENT_SUCCESS if the curve was read or BPM_CLIENT_ERR_SERVER
 * otherwise. The data read is returned in acq_trans->block.data along with
 * the number of bytes effectivly read in acq_trans->block.bytes_read */
bpm_client_err_e bpm_get_curve (bpm_client_t *self, char *service,
        acq_trans_t *acq_trans, int timeout);

/* New version of bpm_data_acquire that uses the general function caller
 * bpm_func_exec */
bpm_client_err_e bpm_acq_start (bpm_client_t *self, char *service, acq_req_t *acq_req);

/* New version of bpm_check_data_acquire that uses the general function caller
 * bpm_func_exec */
bpm_client_err_e bpm_acq_check (bpm_client_t *self, char *service);

/* New version of bpm_get_data_block that uses the general function caller
 * bpm_func_exec */
bpm_client_err_e bpm_acq_get_data_block (bpm_client_t *self, char *service, acq_trans_t *acq_trans);

/* New version of bpm_get_curve that uses the general function caller
 * bpm_func_exec */
bpm_client_err_e bpm_acq_get_curve (bpm_client_t *self, char *service, acq_trans_t *acq_trans);

/* Perform a full acquisition process (Acquisition request, checking if
 * its done and receiving the full curve).
 * Returns BPM_CLIENT_SUCCESS if the curve was read or BPM_CLIENT_ERR_SERVER
 * otherwise. The data read is returned in acq_trans->block.data along with
 * the number of bytes effectivly read in acq_trans->block.bytes_read */
bpm_client_err_e bpm_full_acq (bpm_client_t *self, char *service, acq_trans_t *acq_trans, int timeout);

/********************** DSP Functions ********************/

/* K<direction> functions */
/* These set of functions write (set) or read (get) the Kx, Ky or Ksum values
 * for the delta over sigma calculation. All of the functions returns
 * BPM_CLIENT_SUCCESS if the parameter was correctly set or error
 * (see bpm_client_err.h for all possible errors)*/

bpm_client_err_e bpm_set_kx (bpm_client_t *self, char *service, uint32_t kx);
bpm_client_err_e bpm_get_kx (bpm_client_t *self, char *service,
        uint32_t *kx_out);
bpm_client_err_e bpm_set_ky (bpm_client_t *self, char *service, uint32_t ky);
bpm_client_err_e bpm_get_ky (bpm_client_t *self, char *service,
        uint32_t *ky_out);
bpm_client_err_e bpm_set_ksum (bpm_client_t *self, char *service, uint32_t ksum);
bpm_client_err_e bpm_get_ksum (bpm_client_t *self, char *service,
        uint32_t *ksum_out);

/* Delta-Sigma Threshold functions */
/* These set of functions write (set) or read (get) the delta-sigma
 * sum threslhold calculation. If the selected threshold is, at any time,
 * bigger than the calculaterd SUM in the FPGA, the position calculation
 * for the delta over sigma calculation. All of the functions returns
 * BPM_CLIENT_SUCCESS if the parameter was correctly set or error
 * (see bpm_client_err.h for all possible errors)*/

bpm_client_err_e bpm_set_ds_tbt_thres (bpm_client_t *self, char *service,
        uint32_t ds_tbt_thres);
bpm_client_err_e bpm_get_ds_tbt_thres (bpm_client_t *self, char *service,
        uint32_t *ds_tbt_thres);
bpm_client_err_e bpm_set_ds_fofb_thres (bpm_client_t *self, char *service,
        uint32_t ds_fofb_thres);
bpm_client_err_e bpm_get_ds_fofb_thres (bpm_client_t *self, char *service,
        uint32_t *ds_fofb_thres);
bpm_client_err_e bpm_set_ds_monit_thres (bpm_client_t *self, char *service,
        uint32_t ds_monit_thres);
bpm_client_err_e bpm_get_ds_monit_thres (bpm_client_t *self, char *service,
        uint32_t *ds_monit_thres);

/* Monitoring Position values */
/* These set of functions read (get) the Monitoring position values.
 * All of the functions returns BPM_CLIENT_SUCCESS if the
 * parameter was correctly set or error (see bpm_client_err.h
 * for all possible errors)*/
bpm_client_err_e bpm_set_monit_pos_x (bpm_client_t *self, char *service,
        uint32_t monit_pos_x);
bpm_client_err_e bpm_get_monit_pos_x (bpm_client_t *self, char *service,
        uint32_t *monit_pos_x);
bpm_client_err_e bpm_set_monit_pos_y (bpm_client_t *self, char *service,
        uint32_t monit_pos_y);
bpm_client_err_e bpm_get_monit_pos_y (bpm_client_t *self, char *service,
        uint32_t *monit_pos_y);
bpm_client_err_e bpm_set_monit_pos_q (bpm_client_t *self, char *service,
        uint32_t monit_pos_q);
bpm_client_err_e bpm_get_monit_pos_q (bpm_client_t *self, char *service,
        uint32_t *monit_pos_q);
bpm_client_err_e bpm_set_monit_pos_sum (bpm_client_t *self, char *service,
        uint32_t monit_pos_sum);
bpm_client_err_e bpm_get_monit_pos_sum (bpm_client_t *self, char *service,
        uint32_t *monit_pos_sum);

/* Monitoring Amplitude values */
/* These set of functions read (get) the Monitoring amplitude values.
 * All of the functions returns BPM_CLIENT_SUCCESS if the
 * parameter was correctly set or error (see bpm_client_err.h
 * for all possible errors)*/
bpm_client_err_e bpm_set_monit_amp_ch0 (bpm_client_t *self, char *service,
        uint32_t monit_amp_ch0);
bpm_client_err_e bpm_get_monit_amp_ch0 (bpm_client_t *self, char *service,
        uint32_t *monit_amp_ch0);
bpm_client_err_e bpm_set_monit_amp_ch1 (bpm_client_t *self, char *service,
        uint32_t monit_amp_ch1);
bpm_client_err_e bpm_get_monit_amp_ch1 (bpm_client_t *self, char *service,
        uint32_t *monit_amp_ch1);
bpm_client_err_e bpm_set_monit_amp_ch2 (bpm_client_t *self, char *service,
        uint32_t monit_amp_ch2);
bpm_client_err_e bpm_get_monit_amp_ch2 (bpm_client_t *self, char *service,
        uint32_t *monit_amp_ch2);
bpm_client_err_e bpm_set_monit_amp_ch3 (bpm_client_t *self, char *service,
        uint32_t monit_amp_ch3);
bpm_client_err_e bpm_get_monit_amp_ch3 (bpm_client_t *self, char *service,
        uint32_t *monit_amp_ch3);

/********************** SWAP Functions ********************/

/* Switching functions */
/* These set of functions write (set) or read (get) the (de)switching
 * scheme. In order to turn the switching on, it is necessary to first
 * turn the switching on with bpm_set_sw () and then enable the switching
 * clock with bpm_set_sw_en (). failuere to do so might work, but the initial
 * state between the FPGA and the RFFE is undetermined. All of the functions
 * returns BPM_CLIENT_SUCCESS if the parameter was correctly set or error
 * (see bpm_client_err.h for all possible errors)*/
bpm_client_err_e bpm_set_sw (bpm_client_t *self, char *service,
        uint32_t sw);
bpm_client_err_e bpm_get_sw (bpm_client_t *self, char *service,
        uint32_t *sw);
bpm_client_err_e bpm_set_sw_en (bpm_client_t *self, char *service,
        uint32_t sw_en);
bpm_client_err_e bpm_get_sw_en (bpm_client_t *self, char *service,
        uint32_t *sw_en);

/* Switching clock functions */
/* These set of functions write (set) or read (get) the switching
 * clock frequency. It is determined by the currenct ADC clock frequency
 * divided by the desired value. For instance, to use a switching frequency
 * that is 1000 times slower than the ADC clock, the div_clk parameter
 * must be set to 1000. All of the functions returns BPM_CLIENT_SUCCESS if
 * the parameter was correctly set or error (see bpm_client_err.h for
 * all possible errors)*/
bpm_client_err_e bpm_set_div_clk (bpm_client_t *self, char *service,
        uint32_t div_clk);
bpm_client_err_e bpm_get_div_clk (bpm_client_t *self, char *service,
        uint32_t *div_clk);

/* Switching delay functions */
/* These set of functions write (set) or read (get) the (de)switching
 * delay to the switching state in the RFFE. Its granularity is measured in
 * ADC clock cycles. All of the functions returns BPM_CLIENT_SUCCESS if
 * the parameter was correctly set or error (see bpm_client_err.h for
 * all possible errors)*/
bpm_client_err_e bpm_set_sw_dly (bpm_client_t *self, char *service,
        uint32_t sw_dly);
bpm_client_err_e bpm_get_sw_dly (bpm_client_t *self, char *service,
        uint32_t *sw_dly);

/* Windowing functions */
/* These set of functions write (set) or read (get) the windowing state with
 * bpm_*_wdw () and the delay between the beggining of the window and the switching
 * state change. It is a experimental feature that applies a window to the ADC
 * data, in order to try to reduce the undesired effects of the switching, such
 * as phase shifts. All of the functions returns BPM_CLIENT_SUCCESS if the
 * parameter was correctly set or error (see bpm_client_err.h for all possible
 * errors) */
bpm_client_err_e bpm_set_wdw (bpm_client_t *self, char *service,
        uint32_t wdw);
bpm_client_err_e bpm_get_wdw (bpm_client_t *self, char *service,
        uint32_t *wdw);
bpm_client_err_e bpm_set_wdw_dly (bpm_client_t *self, char *service,
        uint32_t wdw_dly);
bpm_client_err_e bpm_get_wdw_dly (bpm_client_t *self, char *service,
        uint32_t *wdw_dly);

/* Gain functions */
/* TODO: reduce code repetition by, possibilly, group the OPCODES in
 * structure and merge all functions in a single
 * generic one for all channels (A, B, C, D) */
/* These set of functions write (set) or read (get) the gain in all ADC channels,
 * for both the direct path and the inverted path. All of the functions returns
 * BPM_CLIENT_SUCCESS if the parameter was correctly set or error
 * (see bpm_client_err.h for all possible errors) */
bpm_client_err_e bpm_set_gain_a (bpm_client_t *self, char *service,
        uint32_t gain_dir, uint32_t gain_inv);
bpm_client_err_e bpm_get_gain_a (bpm_client_t *self, char *service,
        uint32_t *gain_dir, uint32_t *gain_inv);
bpm_client_err_e bpm_set_gain_b (bpm_client_t *self, char *service,
        uint32_t gain_dir, uint32_t gain_inv);
bpm_client_err_e bpm_get_gain_b (bpm_client_t *self, char *service,
        uint32_t *gain_dir, uint32_t *gain_inv);
bpm_client_err_e bpm_set_gain_c (bpm_client_t *self, char *service,
        uint32_t gain_dir, uint32_t gain_inv);
bpm_client_err_e bpm_get_gain_c (bpm_client_t *self, char *service,
        uint32_t *gain_dir, uint32_t *gain_inv);
bpm_client_err_e bpm_set_gain_d (bpm_client_t *self, char *service,
        uint32_t gain_dir, uint32_t gain_inv);
bpm_client_err_e bpm_get_gain_d (bpm_client_t *self, char *service,
        uint32_t *gain_dir, uint32_t *gain_inv);

/********************** RFFE Functions ********************/

/* Switching functions */
/* These set of functions write (set) or read (get) the RFFE switching
 * scheme. All of the functions returns BPM_CLIENT_SUCCESS if the parameter
 * was correctly set or error (see bpm_client_err.h for all possible errors)*/
bpm_client_err_e bpm_set_rffe_sw (bpm_client_t *self, char *service,
        uint32_t rffe_sw);
bpm_client_err_e bpm_get_rffe_sw (bpm_client_t *self, char *service,
        uint32_t *rffe_sw);

/* Attenuator functions */
/* These set of functions write (set) or read (get) the RFFE attenuator
 * values. All of the functions returns BPM_CLIENT_SUCCESS if the parameter
 * was correctly set or error (see bpm_client_err.h for all possible errors)*/
bpm_client_err_e bpm_set_rffe_att1 (bpm_client_t *self, char *service,
        double rffe_att1);
bpm_client_err_e bpm_get_rffe_att1 (bpm_client_t *self, char *service,
        double *rffe_att1);
bpm_client_err_e bpm_set_rffe_att2 (bpm_client_t *self, char *service,
        double rffe_att2);
bpm_client_err_e bpm_get_rffe_att2 (bpm_client_t *self, char *service,
        double *rffe_att2);

/* Temperature functions */
/* These set of functions read (get) the RFFE temperature
 * values. All of the functions returns BPM_CLIENT_SUCCESS if the parameter
 * was correctly set or error (see bpm_client_err.h for all possible errors)*/
bpm_client_err_e bpm_get_rffe_temp1 (bpm_client_t *self, char *service,
        double *rffe_temp1);
bpm_client_err_e bpm_get_rffe_temp2 (bpm_client_t *self, char *service,
        double *rffe_temp2);
bpm_client_err_e bpm_get_rffe_temp3 (bpm_client_t *self, char *service,
        double *rffe_temp3);
bpm_client_err_e bpm_get_rffe_temp4 (bpm_client_t *self, char *service,
        double *rffe_temp4);

/* Temperature control set point unctions */
/* These set of functions write (set) or read (get) the RFFE set point
 * values. All of the functions returns BPM_CLIENT_SUCCESS if the parameter
 * was correctly set or error (see bpm_client_err.h for all possible errors)*/
bpm_client_err_e bpm_set_rffe_set_point1 (bpm_client_t *self, char *service,
        double rffe_set_point1);
bpm_client_err_e bpm_get_rffe_set_point1 (bpm_client_t *self, char *service,
        double *rffe_set_point1);
bpm_client_err_e bpm_set_rffe_set_point2 (bpm_client_t *self, char *service,
        double rffe_set_point2);
bpm_client_err_e bpm_get_rffe_set_point2 (bpm_client_t *self, char *service,
        double *rffe_set_point2);

/* Temperature control enable functions */
/* These set of functions write (set) or read (get) the RFFE temperatue control
 * scheme. All of the functions returns BPM_CLIENT_SUCCESS if the parameter
 * was correctly set or error (see bpm_client_err.h for all possible errors)*/
bpm_client_err_e bpm_set_rffe_temp_control (bpm_client_t *self, char *service,
        uint32_t rffe_temp_control);
bpm_client_err_e bpm_get_rffe_temp_control (bpm_client_t *self, char *service,
        uint32_t *rffe_temp_control);

/* RFFE output functions */
/* These set of functions write (set) or read (get) the RFFE output
 * values. All of the functions returns BPM_CLIENT_SUCCESS if the parameter
 * was correctly set or error (see bpm_client_err.h for all possible errors)*/
bpm_client_err_e bpm_set_rffe_output1 (bpm_client_t *self, char *service,
        double rffe_output1);
bpm_client_err_e bpm_get_rffe_output1 (bpm_client_t *self, char *service,
        double *rffe_output1);
bpm_client_err_e bpm_set_rffe_output2 (bpm_client_t *self, char *service,
        double rffe_output2);
bpm_client_err_e bpm_get_rffe_output2 (bpm_client_t *self, char *service,
        double *rffe_output2);

/* Reset functions */
/* These set of functions write (set) or read (get) the RFFE reset state
 * scheme. All of the functions returns BPM_CLIENT_SUCCESS if the parameter
 * was correctly set or error (see bpm_client_err.h for all possible errors)*/
bpm_client_err_e bpm_set_rffe_reset (bpm_client_t *self, char *service,
        uint32_t rffe_reset);
bpm_client_err_e bpm_get_rffe_reset (bpm_client_t *self, char *service,
        uint32_t *rffe_reset);

/* Reprogramming functions */
/* These set of functions write (set) or read (get) the RFFE reprogramming
 * scheme. All of the functions returns BPM_CLIENT_SUCCESS if the parameter
 * was correctly set or error (see bpm_client_err.h for all possible errors)*/
bpm_client_err_e bpm_set_rffe_reprog (bpm_client_t *self, char *service,
        uint32_t rffe_reprog);
bpm_client_err_e bpm_get_rffe_reprog (bpm_client_t *self, char *service,
        uint32_t *rffe_reprog);

/* Data functions */
/* These set of functions write (set) or read (get) the RFFE new firmware data
 * for the reprogramming scheme. All of the functions returns BPM_CLIENT_SUCCESS
 * if the parameter was correctly set or error (see bpm_client_err.h for all
 * possible errors)*/
bpm_client_err_e bpm_set_rffe_data (bpm_client_t *self, char *service,
        smio_rffe_data_block_t *rffe_data_block);
bpm_client_err_e bpm_get_rffe_data (bpm_client_t *self, char *service,
        smio_rffe_data_block_t *rffe_data_block);

/* Version functions */
/* These set of functions read (get) the RFFE version.
 * All of the functions returns BPM_CLIENT_SUCCESS if the parameter was
 * correctly set or error (see bpm_client_err.h for all possible errors)*/
bpm_client_err_e bpm_get_rffe_version (bpm_client_t *self, char *service,
        smio_rffe_version_t *rffe_version);

/* Switching level functions */
/* These set of functions write (set) read (get) the RFFE switching level.
 * All of the functions returns BPM_CLIENT_SUCCESS if the parameter was
 * correctly set or error (see bpm_client_err.h for all possible errors)*/
bpm_client_err_e bpm_set_rffe_sw_lvl (bpm_client_t *self, char *service,
        uint32_t rffe_sw_lvl);
bpm_client_err_e bpm_get_rffe_sw_lvl (bpm_client_t *self, char *service,
        uint32_t *rffe_sw_lvl);

/* Helper Function */

/* This function execute the given function *func in a disp_op_t
 * for a specific amount of time (timeout).
 * Returns BPM_CLIENT_SUCCESS if the functions has been successfully
 * executed or error otherwise.
 * (see bpm_client_err.h for all possible errors)*/
bpm_client_err_e func_polling (bpm_client_t *self, char *name,
        char *service, uint32_t *input, uint32_t *output, int timeout);

#endif

