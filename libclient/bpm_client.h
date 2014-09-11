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
#include "rw_param_client.h"

struct _acq_buf_t;

/* Our structure */
struct _bpm_client_t {
    mdp_client_t *mdp_client;                   /* Majordomo client instance */
    const struct _acq_buf_t *acq_buf;           /* Acquisition buffer table */
};

typedef struct _bpm_client_t bpm_client_t;

/******************** ACQ SMIO Structures ******************/
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

/******************** FMC130M SMIO Functions ******************/

/* Blink the FMC Leds. This is only used for debug and for demostration
 * purposes.
 * Returns BPM_CLIENT_SUCCESS if ok and BPM_CLIIENT_ERR_SERVER if
 * if server could not complete the request */
bpm_client_err_e bpm_blink_leds (bpm_client_t *self, char *service, uint32_t leds);

/* Simple AD9510 Condig test.
 * Returns BPM_CLIENT_SUCCESS if ok and BPM_CLIIENT_ERR_SERVER if
 * if server could not complete the request */
bpm_client_err_e bpm_ad9510_cfg_test (bpm_client_t *self, char *service);

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

/********************** ACQ SMIO Functions ********************/

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

#endif

