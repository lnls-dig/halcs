/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _HALCS_CLIENT_CORE_H_
#define _HALCS_CLIENT_CORE_H_

#ifdef __cplusplus
extern "C" {
#endif

struct _smio_rffe_data_block_t;
struct _smio_rffe_version_t;
struct _smio_afc_diag_revision_data_t;

/********************************************************/
/************************ Our API ***********************/
/********************************************************/


/* Create an instance of the HALCS client. This must be called
 * before any operation involving communicating with the HALCS
 * server. Return an instance of the halcs client */
halcs_client_t *halcs_client_new (char *broker_endp, int verbose,
        const char *log_file_name);

/* Create an instance of the HALCS client. This must be called
 * before any operation involving communicating with the HALCS
 * server, specifying the send/recv timeout in ms.
 * Return an instance of the halcs client */
halcs_client_t *halcs_client_new_time (char *broker_endp, int verbose,
        const char *log_file_name, int timeout);

/* Create an instance of the HALCS client, with the log filemode specified
 * by "log_mode" as in fopen () call. This must be called before any operation
 * involving communicating with the HALCS server. Return an instance of the halcs
 * client */
halcs_client_t *halcs_client_new_log_mode (char *broker_endp, int verbose,
        const char *log_file_name, const char *log_mode);

/* Create an instance of the HALCS client, with the log filemode specified
 * by "log_mode" as in fopen () call, and the send/recv timeout in ms.
 * This must be called before any operation involving communicating with the
 * HALCS server. Return an instance of the halcs client */
halcs_client_t *halcs_client_new_log_mode_time (char *broker_endp, int verbose,
        const char *log_file_name, const char *log_mode, int timeout);

/* Destroy an instance of the HALCS client. This must be called
 * after all operations involving the communication with the HALCS
 * server */
void halcs_client_destroy (halcs_client_t **self_p);

/* General function to execute all the other modules functions */
halcs_client_err_e halcs_func_exec (halcs_client_t *self, const disp_op_t *func,
        char *service, uint32_t *input, uint32_t *output);

/* Translate function's name and returns its structure */
const disp_op_t* halcs_func_translate (char *name);

/* Wrapper to halcs_func_exec which translates the function name to
 * its exp_ops structure */
halcs_client_err_e halcs_func_trans_exec (halcs_client_t *self, char *name,
        char *service, uint32_t *input, uint32_t *output);

/********************** Accessor Methods **********************/

/* Get MLM client handler from client */
mlm_client_t *halcs_get_mlm_client (halcs_client_t *self);

/* Returns the client poller */
zpoller_t *halcs_client_get_poller (halcs_client_t *self);

/* Set the timeout paramter to send;recv functions */
halcs_client_err_e halcs_client_set_timeout (halcs_client_t *self, int timeout);

/* Get the timeout parameter */
uint32_t halcs_client_get_timeout (halcs_client_t *self);

/******************** FMC130M SMIO Functions ******************/

/* Blink the FMC Leds. This is only used for debug and for demostration
 * purposes.
 * Returns HALCS_CLIENT_SUCCESS if ok and HALCS_CLIIENT_ERR_SERVER if
 * if server could not complete the request */
halcs_client_err_e halcs_set_fmc_leds (halcs_client_t *self, char *service,
        uint32_t fmc_leds);
halcs_client_err_e halcs_get_fmc_leds (halcs_client_t *self, char *service,
        uint32_t *fmc_leds);

/* Macros for compatibility */
#define halcs_blink_leds halcs_set_fmc_leds

/* Simple AD9510 Config test.
 * Returns HALCS_CLIENT_SUCCESS if ok and HALCS_CLIIENT_ERR_SERVER if
 * if server could not complete the request */
halcs_client_err_e halcs_set_ad9510_defaults (halcs_client_t *self, char *service,
        uint32_t ad9510_defaults);
halcs_client_err_e halcs_get_ad9510_defaults (halcs_client_t *self, char *service,
        uint32_t *ad9510_defaults);

/* Macros for compatibility */
#define halcs_ad9510_cfg_defaults halcs_set_ad9510_defaults

/* FMC PLL FUNCTION pin. Sets or clears the FMC PLL FUNCTION pin. This pin
 * has a general purpose based on the 0x59 SPI AD9510 register.
 * Returns HALCS_CLIENT_SUCCESS if ok and HALCS_CLIIENT_ERR_SERVER if
 * if server could not complete the request */
halcs_client_err_e halcs_set_fmc_pll_function (halcs_client_t *self, char *service,
        uint32_t fmc_pll_function);
halcs_client_err_e halcs_get_fmc_pll_function (halcs_client_t *self, char *service,
        uint32_t *fmc_pll_function);

/* FMC PLL STATUS pin. Sets or clears the FMC PLL STATUS pin. This pin
 * outputs the value selected on AD9510 MUX STATUS register.
 * Returns HALCS_CLIENT_SUCCESS if ok and HALCS_CLIIENT_ERR_SERVER if
 * if server could not complete the request */
halcs_client_err_e halcs_set_fmc_pll_status (halcs_client_t *self, char *service,
        uint32_t fmc_pll_status);
halcs_client_err_e halcs_get_fmc_pll_status (halcs_client_t *self, char *service,
        uint32_t *fmc_pll_status);

/* CLK SEL Functions. Set the reference input clock for the AD9510.
 * 0: clock from external source (MMCX J4)
 * 1: clock from FMC PIN (FMC_CLK line)
 *
 * Returns HALCS_CLIENT_SUCCESS if ok and HALCS_CLIIENT_ERR_SERVER if
 * if server could not complete the request */
halcs_client_err_e halcs_set_fmc_clk_sel (halcs_client_t *self, char *service,
        uint32_t clk_sel);
halcs_client_err_e halcs_get_fmc_clk_sel (halcs_client_t *self, char *service,
        uint32_t *clk_sel);

/* ADC LTC2208 Control */
/* These set of functions read (get) or write (set) some ADC LTC2208
 * functionalities. Check LTC2208 datasheet for details.
 * All of the functions returns HALCS_CLIENT_SUCCESS if the
 * parameter was correctly set or error (see halcs_client_err.h
 * for all possible errors)*/
halcs_client_err_e halcs_set_adc_rand (halcs_client_t *self, char *service,
        uint32_t adc_rand);
halcs_client_err_e halcs_get_adc_rand (halcs_client_t *self, char *service,
        uint32_t *adc_rand);
halcs_client_err_e halcs_set_adc_dith (halcs_client_t *self, char *service,
        uint32_t adc_dith);
halcs_client_err_e halcs_get_adc_dith (halcs_client_t *self, char *service,
        uint32_t *adc_dith);
halcs_client_err_e halcs_set_adc_shdn (halcs_client_t *self, char *service,
        uint32_t adc_shdn);
halcs_client_err_e halcs_get_adc_shdn (halcs_client_t *self, char *service,
        uint32_t *adc_shdn);
halcs_client_err_e halcs_set_adc_pga (halcs_client_t *self, char *service,
        uint32_t adc_pga);
halcs_client_err_e halcs_get_adc_pga (halcs_client_t *self, char *service,
        uint32_t *adc_pga);

/* RAW ADC data functions */
/* These set of functions read (get) the RAW ADC values.
 * All of the functions returns HALCS_CLIENT_SUCCESS if the
 * parameter was correctly set or error (see halcs_client_err.h
 * for all possible errors)*/
halcs_client_err_e halcs_set_adc_data0 (halcs_client_t *self, char *service,
        uint32_t adc_data0);
halcs_client_err_e halcs_get_adc_data0 (halcs_client_t *self, char *service,
        uint32_t *adc_data0);
halcs_client_err_e halcs_set_adc_data1 (halcs_client_t *self, char *service,
        uint32_t adc_data1);
halcs_client_err_e halcs_get_adc_data1 (halcs_client_t *self, char *service,
        uint32_t *adc_data1);
halcs_client_err_e halcs_set_adc_data2 (halcs_client_t *self, char *service,
        uint32_t adc_data2);
halcs_client_err_e halcs_get_adc_data2 (halcs_client_t *self, char *service,
        uint32_t *adc_data2);
halcs_client_err_e halcs_set_adc_data3 (halcs_client_t *self, char *service,
        uint32_t adc_data3);
halcs_client_err_e halcs_get_adc_data3 (halcs_client_t *self, char *service,
        uint32_t *adc_data3);

/* ADC delay value functions */

/* The three set of group functions provide a low-lovel interface to the FPGA
 * firmware. The correct usage to set the ADC clock and data paths delay are:
 *
 * 1) Set the delay value with halcs_set_adc_dly_val<*> for the desired channel.
 *      Accepted values are from 0 to 31.
 * 2) Set the delay line which will be updated with halcs_set_adc_dly_line<*> for the desired channel
 *      Accepted values are the bitmask of the desired lines with bit 16 corresponding
 *      to the clock line and bits 15-0 to ADC bits 15 to 0
 * 3) call the update functions halcs_set_adc_dly_updt<*> for the desired channel.
 */

/* These set of functions read/write (set/get) the ADC delay values.
 * All of the functions returns HALCS_CLIENT_SUCCESS if the
 * parameter was correctly set or error (see halcs_client_err.h
 * for all possible errors)*/
halcs_client_err_e halcs_set_adc_dly_val0 (halcs_client_t *self, char *service,
        uint32_t adc_dly_val0);
halcs_client_err_e halcs_get_adc_dly_val0 (halcs_client_t *self, char *service,
        uint32_t *adc_dly_val0);
halcs_client_err_e halcs_set_adc_dly_val1 (halcs_client_t *self, char *service,
        uint32_t adc_dly_val1);
halcs_client_err_e halcs_get_adc_dly_val1 (halcs_client_t *self, char *service,
        uint32_t *adc_dly_val1);
halcs_client_err_e halcs_set_adc_dly_val2 (halcs_client_t *self, char *service,
        uint32_t adc_dly_val2);
halcs_client_err_e halcs_get_adc_dly_val2 (halcs_client_t *self, char *service,
        uint32_t *adc_dly_val2);
halcs_client_err_e halcs_set_adc_dly_val3 (halcs_client_t *self, char *service,
        uint32_t adc_dly_val3);
halcs_client_err_e halcs_get_adc_dly_val3 (halcs_client_t *self, char *service,
        uint32_t *adc_dly_val3);

/* ADC delay line functions */
/* These set of functions read/write (set/get) the ADC delay line values,
 * meaning which lines (16 = clock, 15-0 = data bits) are to be updated
 * on halcs_set_adc_dly_updt<*> functions.
 * All of the functions returns HALCS_CLIENT_SUCCESS if the
 * parameter was correctly set or error (see halcs_client_err.h
 * for all possible errors)*/
halcs_client_err_e halcs_set_adc_dly_line0 (halcs_client_t *self, char *service,
        uint32_t adc_dly_line0);
halcs_client_err_e halcs_get_adc_dly_line0 (halcs_client_t *self, char *service,
        uint32_t *adc_dly_line0);
halcs_client_err_e halcs_set_adc_dly_line1 (halcs_client_t *self, char *service,
        uint32_t adc_dly_line1);
halcs_client_err_e halcs_get_adc_dly_line1 (halcs_client_t *self, char *service,
        uint32_t *adc_dly_line1);
halcs_client_err_e halcs_set_adc_dly_line2 (halcs_client_t *self, char *service,
        uint32_t adc_dly_line2);
halcs_client_err_e halcs_get_adc_dly_line2 (halcs_client_t *self, char *service,
        uint32_t *adc_dly_line2);
halcs_client_err_e halcs_set_adc_dly_line3 (halcs_client_t *self, char *service,
        uint32_t adc_dly_line3);
halcs_client_err_e halcs_get_adc_dly_line3 (halcs_client_t *self, char *service,
        uint32_t *adc_dly_line3);

/* ADC update functions */
/* These set of functions read/write (set/get) the ADC delay update values
 * for each channel. This will effectively update the FPGA delay primitives to
 * the previous delay values set by the corresponding halcs_get_adc_dly_updt<*>
 * functions.
 * All of the functions returns HALCS_CLIENT_SUCCESS if the
 * parameter was correctly set or error (see halcs_client_err.h
 * for all possible errors)*/
halcs_client_err_e halcs_set_adc_dly_updt0 (halcs_client_t *self, char *service,
        uint32_t adc_dly_updt0);
halcs_client_err_e halcs_get_adc_dly_updt0 (halcs_client_t *self, char *service,
        uint32_t *adc_dly_updt0);
halcs_client_err_e halcs_set_adc_dly_updt1 (halcs_client_t *self, char *service,
        uint32_t adc_dly_updt1);
halcs_client_err_e halcs_get_adc_dly_updt1 (halcs_client_t *self, char *service,
        uint32_t *adc_dly_updt1);
halcs_client_err_e halcs_set_adc_dly_updt2 (halcs_client_t *self, char *service,
        uint32_t adc_dly_updt2);
halcs_client_err_e halcs_get_adc_dly_updt2 (halcs_client_t *self, char *service,
        uint32_t *adc_dly_updt2);
halcs_client_err_e halcs_set_adc_dly_updt3 (halcs_client_t *self, char *service,
        uint32_t adc_dly_updt3);
halcs_client_err_e halcs_get_adc_dly_updt3 (halcs_client_t *self, char *service,
        uint32_t *adc_dly_updt3);

/* Higher-level ADC delay functions */
/* These set of functions write (set) the ADC delays for each channel.
 * This functions wrap all of the functionality present by the 3 groups above
 * in a conveninent way to the user.
 * All of the functions returns HALCS_CLIENT_SUCCESS if the
 * parameter was correctly set or error (see halcs_client_err.h
 * for all possible errors)*/
halcs_client_err_e halcs_set_adc_dly0 (halcs_client_t *self, char *service,
        uint32_t dly_type0, uint32_t dly_val0);
halcs_client_err_e halcs_set_adc_dly1 (halcs_client_t *self, char *service,
        uint32_t dly_type1, uint32_t dly_val1);
halcs_client_err_e halcs_set_adc_dly2 (halcs_client_t *self, char *service,
        uint32_t dly_type2, uint32_t dly_val2);
halcs_client_err_e halcs_set_adc_dly3 (halcs_client_t *self, char *service,
        uint32_t dly_type3, uint32_t dly_val3);

/* FMC TEST data enable. Sets or clears the ADC test data switch. This
 * enables or disables the ADC test RAMP output.
 * Returns HALCS_CLIENT_SUCCESS if ok and HALCS_CLIIENT_ERR_SERVER if
 * if server could not complete the request */
halcs_client_err_e halcs_set_adc_test_data_en (halcs_client_t *self, char *service,
        uint32_t adc_test_data_en);
halcs_client_err_e halcs_get_adc_test_data_en (halcs_client_t *self, char *service,
        uint32_t *adc_test_data_en);

/* FMC SI571 Output Control. Enables or disables the Si571 output.
 * Returns HALCS_CLIENT_SUCCESS if ok and HALCS_CLIIENT_ERR_SERVER if
 * if server could not complete the request */
halcs_client_err_e halcs_set_si571_oe (halcs_client_t *self, char *service,
        uint32_t si571_oe);
halcs_client_err_e halcs_get_si571_oe (halcs_client_t *self, char *service,
        uint32_t *si571_oe);

/* FMC trigger direction control.
 * Returns HALCS_CLIENT_SUCCESS if ok and HALCS_CLIIENT_ERR_SERVER if
 * if server could not complete the request */
halcs_client_err_e halcs_set_trig_dir (halcs_client_t *self, char *service,
        uint32_t trig_dir);
halcs_client_err_e halcs_get_trig_dir (halcs_client_t *self, char *service,
        uint32_t *trig_dir);

/* FMC trigger termination control.
 * Returns HALCS_CLIENT_SUCCESS if ok and HALCS_CLIIENT_ERR_SERVER if
 * if server could not complete the request */
halcs_client_err_e halcs_set_trig_term (halcs_client_t *self, char *service,
        uint32_t trig_term);
halcs_client_err_e halcs_get_trig_term (halcs_client_t *self, char *service,
        uint32_t *trig_term);

/* FMC trigger value control.
 * Returns HALCS_CLIENT_SUCCESS if ok and HALCS_CLIIENT_ERR_SERVER if
 * if server could not complete the request */
halcs_client_err_e halcs_set_trig_val (halcs_client_t *self, char *service,
        uint32_t trig_val);
halcs_client_err_e halcs_get_trig_val (halcs_client_t *self, char *service,
        uint32_t *trig_val);

/* FMC AD9510 Control.
 * Returns HALCS_CLIENT_SUCCESS if ok and HALCS_CLIIENT_ERR_SERVER if
 * if server could not complete the request */
halcs_client_err_e halcs_set_ad9510_pll_a_div (halcs_client_t *self, char *service,
        uint32_t ad9510_pll_a_div);
halcs_client_err_e halcs_get_ad9510_pll_a_div (halcs_client_t *self, char *service,
        uint32_t *ad9510_pll_a_div);

halcs_client_err_e halcs_set_ad9510_pll_b_div (halcs_client_t *self, char *service,
        uint32_t ad9510_pll_b_div);
halcs_client_err_e halcs_get_ad9510_pll_b_div (halcs_client_t *self, char *service,
        uint32_t *ad9510_pll_b_div);

halcs_client_err_e halcs_set_ad9510_pll_prescaler (halcs_client_t *self, char *service,
        uint32_t ad9510_pll_prescaler);
halcs_client_err_e halcs_get_ad9510_pll_prescaler (halcs_client_t *self, char *service,
        uint32_t *ad9510_pll_prescaler);

halcs_client_err_e halcs_set_ad9510_r_div (halcs_client_t *self, char *service,
        uint32_t ad9510_r_div);
halcs_client_err_e halcs_get_ad9510_r_div (halcs_client_t *self, char *service,
        uint32_t *ad9510_r_div);

halcs_client_err_e halcs_set_ad9510_pll_pdown (halcs_client_t *self, char *service,
        uint32_t ad9510_pll_pdown);
halcs_client_err_e halcs_get_ad9510_pll_pdown (halcs_client_t *self, char *service,
        uint32_t *ad9510_pll_pdown);

halcs_client_err_e halcs_set_ad9510_mux_status (halcs_client_t *self, char *service,
        uint32_t ad9510_mux_status);
halcs_client_err_e halcs_get_ad9510_mux_status (halcs_client_t *self, char *service,
        uint32_t *ad9510_mux_status);

halcs_client_err_e halcs_set_ad9510_cp_current (halcs_client_t *self, char *service,
        uint32_t ad9510_cp_current);
halcs_client_err_e halcs_get_ad9510_cp_current (halcs_client_t *self, char *service,
        uint32_t *ad9510_cp_current);

halcs_client_err_e halcs_set_ad9510_outputs (halcs_client_t *self, char *service,
        uint32_t ad9510_outputs);
halcs_client_err_e halcs_get_ad9510_outputs (halcs_client_t *self, char *service,
        uint32_t *ad9510_outputs);

halcs_client_err_e halcs_set_ad9510_pll_clk_sel (halcs_client_t *self, char *service,
        uint32_t ad9510_pll_clk_sel);
halcs_client_err_e halcs_get_ad9510_pll_clk_sel (halcs_client_t *self, char *service,
        uint32_t *ad9510_pll_clk_sel);

/* FMC SI571 Control.
 * Returns HALCS_CLIENT_SUCCESS if ok and HALCS_CLIIENT_ERR_SERVER if
 * if server could not complete the request */
halcs_client_err_e halcs_set_si571_freq (halcs_client_t *self, char *service,
        double si571_freq);
halcs_client_err_e halcs_get_si571_freq (halcs_client_t *self, char *service,
        double *si571_freq);
halcs_client_err_e halcs_set_si571_defaults (halcs_client_t *self, char *service,
        double si571_defaults);

/******************** FMC250M SMIO Functions ******************/

/* ADC ISLA216P Control */
/* These set of functions read (get) or write (set) some ADC ISLA216P
 * functionalities. Check ISLA216P datasheet for details.
 * All of the functions returns HALCS_CLIENT_SUCCESS if the
 * parameter was correctly set or error (see halcs_client_err.h
 * for all possible errors)*/
halcs_client_err_e halcs_set_rst_adcs (halcs_client_t *self, char *service,
        uint32_t rst_adcs);
halcs_client_err_e halcs_get_rst_adcs (halcs_client_t *self, char *service,
        uint32_t *rst_adcs);
halcs_client_err_e halcs_set_rst_div_adcs (halcs_client_t *self, char *service,
        uint32_t rst_div_adcs);
halcs_client_err_e halcs_get_rst_div_adcs (halcs_client_t *self, char *service,
        uint32_t *rst_div_adcs);
halcs_client_err_e halcs_set_sleep_adcs (halcs_client_t *self, char *service,
        uint32_t sleep_adcs);
halcs_client_err_e halcs_get_sleep_adcs (halcs_client_t *self, char *service,
        uint32_t *sleep_adcs);

halcs_client_err_e halcs_set_test_mode0 (halcs_client_t *self, char *service,
        uint32_t test_mode0);
halcs_client_err_e halcs_set_test_mode1 (halcs_client_t *self, char *service,
        uint32_t test_mode1);
halcs_client_err_e halcs_set_test_mode2 (halcs_client_t *self, char *service,
        uint32_t test_mode2);
halcs_client_err_e halcs_set_test_mode3 (halcs_client_t *self, char *service,
        uint32_t test_mode3);

/********************** ACQ SMIO Functions ********************/

/* Acquisition request */
typedef struct {
    uint32_t num_samples_pre;                   /* Number of pre-trigger samples */
    uint32_t num_samples_post;                  /* Number of post-trigger samples */
    uint32_t num_shots;                         /* Number of shots */
    uint32_t chan;                              /* Acquisition channel number */
} acq_req_t;

/* Acquisition data block */
typedef struct {
    uint32_t idx;                               /* Block index */

    uint32_t *data;                             /* Block or complete curve read */
    uint32_t data_size;                         /* data_out buffer size */
    uint32_t bytes_read;                        /* Number of bytes effectively read */
} acq_block_t;

/* Acquisition transaction */
typedef struct {
    acq_req_t req;                              /* Request */
    acq_block_t block;                          /* Block or whole curve read */
} acq_trans_t;

/* Acquisition channel definitions */
typedef struct {
    uint32_t chan;
    uint32_t sample_size;
} acq_chan_t;

/* Acquisition channel definitions */
extern acq_chan_t acq_chan[END_CHAN_ID];

/* Start acquisition on a specific channel with an specific number of samples,
 * through the use of acq_req_t structure.
 * Returns HALCS_CLIENT_SUCCESS if ok and HALCS_CLIIENT_ERR_SERVER if the server
 * could not complete the request */
halcs_client_err_e halcs_acq_start (halcs_client_t *self, char *service,
        acq_req_t *acq_req);

/* Check if apreviouly started acquisition finished.
 * Returns HALCS_CLIENT_SUCCESS if ok and HALCS_CLIIENT_ERR_AGAIN if the acquistion
 * did not complete */
halcs_client_err_e halcs_acq_check (halcs_client_t *self, char *service);

/* Wait for the previouly started acquistion to complete with a maximum tolerated
 * wait.
 * Returns HALCS_CLIENT_SUCCESS if the acquistion finished under the specified
 * timeout or HALCS_CLIIENT_ERR_TIMEOUT if the acquistion did not completed in time */
halcs_client_err_e halcs_acq_check_timed (halcs_client_t *self, char *service,
        int timeout);

/* Get an specific data block from a previously completed acquisiton by setting
 * the desired block index in acq_trans->block.idx and the desired channel in
 * acq_trans->req.channel.
 * Returns HALCS_CLIENT_SUCCESS if the block was read or HALCS_CLIENT_ERR_SERVER
 * otherwise. The data read is returned in acq_trans->block.data along with
 * the number of bytes effectivly read in acq_trans->block.bytes_read */
halcs_client_err_e halcs_acq_get_data_block (halcs_client_t *self, char *service,
        acq_trans_t *acq_trans);

/* Get a whole curve a previously completed acquisition by setting
 * the desired channel in acq_trans->req.channel.
 * Returns HALCS_CLIENT_SUCCESS if the block was read or HALCS_CLIENT_ERR_SERVER
 * otherwise. The data read is returned in acq_trans->block.data along with
 * the number of bytes effectively read in acq_trans->block.bytes_read */
halcs_client_err_e halcs_acq_get_curve (halcs_client_t *self, char *service,
        acq_trans_t *acq_trans);

/* Perform a full acquisition process (Acquisition request, checking if
 * its done and receiving the full curve).
 * Returns HALCS_CLIENT_SUCCESS if the curve was read or HALCS_CLIENT_ERR_SERVER
 * otherwise. The data read is returned in acq_trans->block.data along with
 * the number of bytes effectivly read in acq_trans->block.bytes_read */
halcs_client_err_e halcs_full_acq (halcs_client_t *self, char *service,
        acq_trans_t *acq_trans, int timeout);

/* Compatibility version of the old halcs_full_acq. Performs a full acquisition
 * if new_acq = 1 and a curve readout if new_acq = 0*/
halcs_client_err_e halcs_full_acq_compat (halcs_client_t *self, char *service,
        acq_trans_t *acq_trans, int timeout, bool new_acq);

/* Macros for compatibility */
#define halcs_data_acquire halcs_acq_start
#define halcs_check_data_acquire halcs_acq_check
#define halcs_check_data_acquire_timed halcs_acq_check_timed
#define halcs_get_data_block halcs_acq_get_data_block
#define halcs_get_curve halcs_full_acq_compat

/* Configure acquisition trigger. Trigger types are: 0 -> skip trigger,
 * 1 -> external trigger, 2 -> data-driven trigger, 3 -> software trigger.
 * Returns HALCS_CLIENT_SUCCESS if the trigger was correctly set or
 * or an error (see halcs_client_err.h for all possible errors)*/
halcs_client_err_e halcs_set_acq_trig (halcs_client_t *self, char *service,
        uint32_t trig);
halcs_client_err_e halcs_get_acq_trig (halcs_client_t *self, char *service,
        uint32_t *trig);

/* Configure data-driven trigger polarity. Options are: 0 -> positive slope (
 * 0 -> 1), 1 -> negative slope (1 -> 0).
 * Returns HALCS_CLIENT_SUCCESS if the trigger was correctly set or
 * or an error (see halcs_client_err.h for all possible errors)*/
halcs_client_err_e halcs_set_acq_data_trig_pol (halcs_client_t *self, char *service,
        uint32_t data_trig_pol);
halcs_client_err_e halcs_get_acq_data_trig_pol (halcs_client_t *self, char *service,
        uint32_t *data_trig_pol);

/* Configure data-driven trigger selection. Options are: 0 -> channel data
 * sample 0, 1 -> channel data sample 1, 2 -> channel data sample 2, 3 -> channel
 * data sample 3
 * Returns HALCS_CLIENT_SUCCESS if the trigger was correctly set or
 * or an error (see halcs_client_err.h for all possible errors)*/
halcs_client_err_e halcs_set_acq_data_trig_sel (halcs_client_t *self, char *service,
        uint32_t data_trig_sel);
halcs_client_err_e halcs_get_acq_data_trig_sel (halcs_client_t *self, char *service,
        uint32_t *data_trig_sel);

/* Configure data-driven trigger hysteresis filter. data_trig_filt is an integer
 * number from 0 to 2^8-1, meaning the number of steady counts after the data
 * sample goes above or below the data threshold. This is only valid for
 * data-driven trigger.
 * Returns HALCS_CLIENT_SUCCESS if the trigger was correctly set or
 * or an error (see halcs_client_err.h for all possible errors)*/
halcs_client_err_e halcs_set_acq_data_trig_filt (halcs_client_t *self, char *service,
        uint32_t data_trig_filt);
halcs_client_err_e halcs_get_acq_data_trig_filt (halcs_client_t *self, char *service,
        uint32_t *data_trig_filt);

/* Configure data-driven signed threshold. data_trig_thres is signed integer
 * number from -2^31 to 2^31-1, meaning the number to be compared on which the
 * data-driven trigger will be detected. This is only valid for
 * data-driven trigger.
 * Returns HALCS_CLIENT_SUCCESS if the trigger was correctly set or
 * or an error (see halcs_client_err.h for all possible errors)*/
halcs_client_err_e halcs_set_acq_data_trig_thres (halcs_client_t *self, char *service,
        uint32_t data_trig_thres);
halcs_client_err_e halcs_get_acq_data_trig_thres (halcs_client_t *self, char *service,
        uint32_t *data_trig_thres);

/* Configure trigger delay. hw_trig_dly is an integer number from 0 to 2^32-1,
 * meaning the number of ADC clock cycles after which the detected trigger
 * will be used. This is only valid for external or data-driven trigger types.
 * Returns HALCS_CLIENT_SUCCESS if the trigger was correctly set or
 * or an error (see halcs_client_err.h for all possible errors)*/
halcs_client_err_e halcs_set_acq_hw_trig_dly (halcs_client_t *self, char *service,
        uint32_t hw_trig_dly);
halcs_client_err_e halcs_get_acq_hw_trig_dly (halcs_client_t *self, char *service,
        uint32_t *hw_trig_dly);

/* Generate software trigger. Options are: 1 generates a software trigger, 0
 * generates nothing.
 * Returns HALCS_CLIENT_SUCCESS if the trigger was correctly set or
 * or an error (see halcs_client_err.h for all possible errors)*/
halcs_client_err_e halcs_set_acq_sw_trig (halcs_client_t *self, char *service,
        uint32_t sw_trig);
halcs_client_err_e halcs_get_acq_sw_trig (halcs_client_t *self, char *service,
        uint32_t *sw_trig);

/* Stops the acquisition FSM. Options are: 1 stop the FSM, 0
 * generates nothing.
 * Returns HALCS_CLIENT_SUCCESS if the trigger was correctly set or
 * or an error (see halcs_client_err.h for all possible errors)*/
halcs_client_err_e halcs_set_acq_fsm_stop (halcs_client_t *self, char *service,
        uint32_t fsm_stop);
halcs_client_err_e halcs_get_acq_fsm_stop (halcs_client_t *self, char *service,
        uint32_t *fsm_stop);

/* Selects which channel to look for a data trigger.
 * Returns HALCS_CLIENT_SUCCESS if the trigger was correctly set or
 * or an error (see halcs_client_err.h for all possible errors)*/
halcs_client_err_e halcs_set_acq_data_trig_chan (halcs_client_t *self, char *service,
        uint32_t data_trig_chan);
halcs_client_err_e halcs_get_acq_data_trig_chan (halcs_client_t *self, char *service,
        uint32_t *data_trig_chan);

/********************** DSP Functions ********************/

/* K<direction> functions */
/* These set of functions write (set) or read (get) the Kx, Ky or Ksum values
 * for the delta over sigma calculation. All of the functions returns
 * HALCS_CLIENT_SUCCESS if the parameter was correctly set or error
 * (see halcs_client_err.h for all possible errors)*/

halcs_client_err_e halcs_set_kx (halcs_client_t *self, char *service, uint32_t kx);
halcs_client_err_e halcs_get_kx (halcs_client_t *self, char *service,
        uint32_t *kx_out);
halcs_client_err_e halcs_set_ky (halcs_client_t *self, char *service, uint32_t ky);
halcs_client_err_e halcs_get_ky (halcs_client_t *self, char *service,
        uint32_t *ky_out);
halcs_client_err_e halcs_set_ksum (halcs_client_t *self, char *service, uint32_t ksum);
halcs_client_err_e halcs_get_ksum (halcs_client_t *self, char *service,
        uint32_t *ksum_out);

/* Delta-Sigma Threshold functions */
/* These set of functions write (set) or read (get) the delta-sigma
 * sum threslhold calculation. If the selected threshold is, at any time,
 * bigger than the calculaterd SUM in the FPGA, the position calculation
 * for the delta over sigma calculation. All of the functions returns
 * HALCS_CLIENT_SUCCESS if the parameter was correctly set or error
 * (see halcs_client_err.h for all possible errors)*/

halcs_client_err_e halcs_set_ds_tbt_thres (halcs_client_t *self, char *service,
        uint32_t ds_tbt_thres);
halcs_client_err_e halcs_get_ds_tbt_thres (halcs_client_t *self, char *service,
        uint32_t *ds_tbt_thres);
halcs_client_err_e halcs_set_ds_fofb_thres (halcs_client_t *self, char *service,
        uint32_t ds_fofb_thres);
halcs_client_err_e halcs_get_ds_fofb_thres (halcs_client_t *self, char *service,
        uint32_t *ds_fofb_thres);
halcs_client_err_e halcs_set_ds_monit_thres (halcs_client_t *self, char *service,
        uint32_t ds_monit_thres);
halcs_client_err_e halcs_get_ds_monit_thres (halcs_client_t *self, char *service,
        uint32_t *ds_monit_thres);

/* Monitoring Position values */
/* These set of functions read (get) the Monitoring position values.
 * All of the functions returns HALCS_CLIENT_SUCCESS if the
 * parameter was correctly set or error (see halcs_client_err.h
 * for all possible errors)*/
halcs_client_err_e halcs_set_monit_pos_x (halcs_client_t *self, char *service,
        uint32_t monit_pos_x);
halcs_client_err_e halcs_get_monit_pos_x (halcs_client_t *self, char *service,
        uint32_t *monit_pos_x);
halcs_client_err_e halcs_set_monit_pos_y (halcs_client_t *self, char *service,
        uint32_t monit_pos_y);
halcs_client_err_e halcs_get_monit_pos_y (halcs_client_t *self, char *service,
        uint32_t *monit_pos_y);
halcs_client_err_e halcs_set_monit_pos_q (halcs_client_t *self, char *service,
        uint32_t monit_pos_q);
halcs_client_err_e halcs_get_monit_pos_q (halcs_client_t *self, char *service,
        uint32_t *monit_pos_q);
halcs_client_err_e halcs_set_monit_pos_sum (halcs_client_t *self, char *service,
        uint32_t monit_pos_sum);
halcs_client_err_e halcs_get_monit_pos_sum (halcs_client_t *self, char *service,
        uint32_t *monit_pos_sum);

/* Monitoring Amplitude values */
/* These set of functions read (get) the Monitoring amplitude values.
 * All of the functions returns HALCS_CLIENT_SUCCESS if the
 * parameter was correctly set or error (see halcs_client_err.h
 * for all possible errors)*/
halcs_client_err_e halcs_set_monit_amp_ch0 (halcs_client_t *self, char *service,
        uint32_t monit_amp_ch0);
halcs_client_err_e halcs_get_monit_amp_ch0 (halcs_client_t *self, char *service,
        uint32_t *monit_amp_ch0);
halcs_client_err_e halcs_set_monit_amp_ch1 (halcs_client_t *self, char *service,
        uint32_t monit_amp_ch1);
halcs_client_err_e halcs_get_monit_amp_ch1 (halcs_client_t *self, char *service,
        uint32_t *monit_amp_ch1);
halcs_client_err_e halcs_set_monit_amp_ch2 (halcs_client_t *self, char *service,
        uint32_t monit_amp_ch2);
halcs_client_err_e halcs_get_monit_amp_ch2 (halcs_client_t *self, char *service,
        uint32_t *monit_amp_ch2);
halcs_client_err_e halcs_set_monit_amp_ch3 (halcs_client_t *self, char *service,
        uint32_t monit_amp_ch3);
halcs_client_err_e halcs_get_monit_amp_ch3 (halcs_client_t *self, char *service,
        uint32_t *monit_amp_ch3);

/* Monitoring Update values */
/* These set of functions read (get) the Monitoring update value, which
 * effectively updates the AMP/POS values in the FPGA. After this,
 * reading AMP/POS values are guaranteed to stay fixed until the next
 * update.
 * All of the functions returns HALCS_CLIENT_SUCCESS if the
 * parameter was correctly set or error (see halcs_client_err.h
 * for all possible errors)*/
halcs_client_err_e halcs_set_monit_updt (halcs_client_t *self, char *service,
        uint32_t monit_updt);
halcs_client_err_e halcs_get_monit_updt (halcs_client_t *self, char *service,
        uint32_t *monit_updt);

/********************** SWAP Functions ********************/

/* Switching functions */
/* These set of functions write (set) or read (get) the (de)switching
 * scheme. In order to turn the switching on, it is necessary to first
 * turn the switching on with halcs_set_sw () and then enable the switching
 * clock with halcs_set_sw_en (). failuere to do so might work, but the initial
 * state between the FPGA and the RFFE is undetermined. All of the functions
 * returns HALCS_CLIENT_SUCCESS if the parameter was correctly set or error
 * (see halcs_client_err.h for all possible errors)*/
halcs_client_err_e halcs_set_sw (halcs_client_t *self, char *service,
        uint32_t sw);
halcs_client_err_e halcs_get_sw (halcs_client_t *self, char *service,
        uint32_t *sw);

/* Switching clock functions */
/* These set of functions write (set) or read (get) the switching
 * clock frequency. It is determined by the currenct ADC clock frequency
 * divided by the desired value. For instance, to use a switching frequency
 * that is 1000 times slower than the ADC clock, the div_clk parameter
 * must be set to 1000. All of the functions returns HALCS_CLIENT_SUCCESS if
 * the parameter was correctly set or error (see halcs_client_err.h for
 * all possible errors)*/
halcs_client_err_e halcs_set_div_clk (halcs_client_t *self, char *service,
        uint32_t div_clk);
halcs_client_err_e halcs_get_div_clk (halcs_client_t *self, char *service,
        uint32_t *div_clk);

/* Switching delay functions */
/* These set of functions write (set) or read (get) the (de)switching
 * delay to the switching state in the RFFE. Its granularity is measured in
 * ADC clock cycles. All of the functions returns HALCS_CLIENT_SUCCESS if
 * the parameter was correctly set or error (see halcs_client_err.h for
 * all possible errors)*/
halcs_client_err_e halcs_set_sw_dly (halcs_client_t *self, char *service,
        uint32_t sw_dly);
halcs_client_err_e halcs_get_sw_dly (halcs_client_t *self, char *service,
        uint32_t *sw_dly);

/********************** RFFE Functions ********************/

/* Attenuator functions */
/* These set of functions write (set) or read (get) the RFFE attenuator
 * values. All of the functions returns HALCS_CLIENT_SUCCESS if the parameter
 * was correctly set or error (see halcs_client_err.h for all possible errors)*/
halcs_client_err_e halcs_set_rffe_att (halcs_client_t *self, char *service,
        double rffe_att);
halcs_client_err_e halcs_get_rffe_att (halcs_client_t *self, char *service,
        double *rffe_att);

/* Temperature functions */
/* These set of functions read (get) the RFFE temperature
 * values. All of the functions returns HALCS_CLIENT_SUCCESS if the parameter
 * was correctly set or error (see halcs_client_err.h for all possible errors)*/
halcs_client_err_e halcs_get_rffe_temp_ac (halcs_client_t *self, char *service,
        double *rffe_temp_ac);
halcs_client_err_e halcs_get_rffe_temp_bd (halcs_client_t *self, char *service,
        double *rffe_temp_bd);

/* Temperature control set point unctions */
/* These set of functions write (set) or read (get) the RFFE set point
 * values. All of the functions returns HALCS_CLIENT_SUCCESS if the parameter
 * was correctly set or error (see halcs_client_err.h for all possible errors)*/
halcs_client_err_e halcs_set_rffe_set_point_ac (halcs_client_t *self, char *service,
        double rffe_set_point_ac);
halcs_client_err_e halcs_get_rffe_set_point_ac (halcs_client_t *self, char *service,
        double *rffe_set_point_ac);
halcs_client_err_e halcs_set_rffe_set_point_bd (halcs_client_t *self, char *service,
        double rffe_set_point_bd);
halcs_client_err_e halcs_get_rffe_set_point_bd (halcs_client_t *self, char *service,
        double *rffe_set_point_bd);

/* Temperature control enable functions */
/* These set of functions write (set) or read (get) the RFFE temperatue control
 * scheme. All of the functions returns HALCS_CLIENT_SUCCESS if the parameter
 * was correctly set or error (see halcs_client_err.h for all possible errors)*/
halcs_client_err_e halcs_set_rffe_temp_control (halcs_client_t *self, char *service,
        uint8_t rffe_temp_control);
halcs_client_err_e halcs_get_rffe_temp_control (halcs_client_t *self, char *service,
        uint8_t *rffe_temp_control);

/* RFFE output functions */
/* These set of functions write (set) or read (get) the RFFE output
 * values. All of the functions returns HALCS_CLIENT_SUCCESS if the parameter
 * was correctly set or error (see halcs_client_err.h for all possible errors)*/
halcs_client_err_e halcs_set_rffe_heater_ac (halcs_client_t *self, char *service,
        double rffe_heater_ac);
halcs_client_err_e halcs_get_rffe_heater_ac (halcs_client_t *self, char *service,
        double *rffe_heater_ac);
halcs_client_err_e halcs_set_rffe_heater_bd (halcs_client_t *self, char *service,
        double rffe_heater_bd);
halcs_client_err_e halcs_get_rffe_heater_bd (halcs_client_t *self, char *service,
        double *rffe_heater_bd);

/* Reset functions */
/* These set of functions write (set) or read (get) the RFFE reset state
 * scheme. All of the functions returns HALCS_CLIENT_SUCCESS if the parameter
 * was correctly set or error (see halcs_client_err.h for all possible errors)*/
halcs_client_err_e halcs_set_rffe_reset (halcs_client_t *self, char *service,
        uint8_t rffe_reset);
halcs_client_err_e halcs_get_rffe_reset (halcs_client_t *self, char *service,
        uint8_t *rffe_reset);

/* Reprogramming functions */
/* These set of functions write (set) or read (get) the RFFE reprogramming
 * scheme. All of the functions returns HALCS_CLIENT_SUCCESS if the parameter
 * was correctly set or error (see halcs_client_err.h for all possible errors)*/
halcs_client_err_e halcs_set_rffe_reprog (halcs_client_t *self, char *service,
        uint8_t rffe_reprog);
halcs_client_err_e halcs_get_rffe_reprog (halcs_client_t *self, char *service,
        uint8_t *rffe_reprog);

/* Data functions */
/* These set of functions write (set) or read (get) the RFFE new firmware data
 * for the reprogramming scheme. All of the functions returns HALCS_CLIENT_SUCCESS
 * if the parameter was correctly set or error (see halcs_client_err.h for all
 * possible errors)*/
halcs_client_err_e halcs_set_rffe_data (halcs_client_t *self, char *service,
        struct _smio_rffe_data_block_t *rffe_data_block);
halcs_client_err_e halcs_get_rffe_data (halcs_client_t *self, char *service,
        struct _smio_rffe_data_block_t *rffe_data_block);

/* Version functions */
/* These set of functions read (get) the RFFE version.
 * All of the functions returns HALCS_CLIENT_SUCCESS if the parameter was
 * correctly set or error (see halcs_client_err.h for all possible errors)*/
halcs_client_err_e halcs_get_rffe_version (halcs_client_t *self, char *service,
        struct _smio_rffe_version_t *rffe_version);

/* PID functions */
/* These set of functions write (set) read (get) the PID parameters.
 * All of the functions returns HALCS_CLIENT_SUCCESS if the parameter was
 * correctly set or error (see halcs_client_err.h for all possible errors)*/
halcs_client_err_e halcs_set_rffe_pid_ac_kp (halcs_client_t *self, char *service,
        double rffe_pid_ac_kp);
halcs_client_err_e halcs_get_rffe_pid_ac_kp (halcs_client_t *self, char *service,
        double *rffe_pid_ac_kp);
halcs_client_err_e halcs_set_rffe_pid_ac_ti (halcs_client_t *self, char *service,
        double rffe_pid_ac_ti);
halcs_client_err_e halcs_get_rffe_pid_ac_ti (halcs_client_t *self, char *service,
        double *rffe_pid_ac_ti);
halcs_client_err_e halcs_set_rffe_pid_ac_td (halcs_client_t *self, char *service,
        double rffe_pid_ac_td);
halcs_client_err_e halcs_get_rffe_pid_ac_td (halcs_client_t *self, char *service,
        double *rffe_pid_ac_td);
halcs_client_err_e halcs_set_rffe_pid_bd_kp (halcs_client_t *self, char *service,
        double rffe_pid_bd_kp);
halcs_client_err_e halcs_get_rffe_pid_bd_kp (halcs_client_t *self, char *service,
        double *rffe_pid_bd_kp);
halcs_client_err_e halcs_set_rffe_pid_bd_ti (halcs_client_t *self, char *service,
        double rffe_pid_bd_ti);
halcs_client_err_e halcs_get_rffe_pid_bd_ti (halcs_client_t *self, char *service,
        double *rffe_pid_bd_ti);
halcs_client_err_e halcs_set_rffe_pid_bd_td (halcs_client_t *self, char *service,
        double rffe_pid_bd_td);
halcs_client_err_e halcs_get_rffe_pid_bd_td (halcs_client_t *self, char *service,
        double *rffe_pid_bd_td);

/********************** AFC Diagnostics Functions ********************/
/* AFC Card Slot functions */
/* These set of functions write (set) read (get) the card slot.
 * All of the functions returns HALCS_CLIENT_SUCCESS if the parameter was
 * correctly set or error (see halcs_client_err.h for all possible errors)*/
halcs_client_err_e halcs_set_afc_diag_card_slot (halcs_client_t *self, char *service,
        uint32_t afc_diag_card_slot);
halcs_client_err_e halcs_get_afc_diag_card_slot (halcs_client_t *self, char *service,
        uint32_t *afc_diag_card_slot);

/* AFC IPMI Address functions */
/* These set of functions write (set) read (get) the IPMI address.
 * All of the functions returns HALCS_CLIENT_SUCCESS if the parameter was
 * correctly set or error (see halcs_client_err.h for all possible errors)*/
halcs_client_err_e halcs_set_afc_diag_ipmi_addr (halcs_client_t *self, char *service,
        uint32_t afc_diag_ipmi_addr);
halcs_client_err_e halcs_get_afc_diag_ipmi_addr (halcs_client_t *self, char *service,
        uint32_t *afc_diag_ipmi_addr);

/* AFC Revision functions */
/* These set of functions read (get) revision information, like build ID,
 * date, user name and email by the person who compiled the sofware.
 * All of the functions returns HALCS_CLIENT_SUCCESS if the parameter was
 * correctly set or error (see halcs_client_err.h for all possible errors)*/
halcs_client_err_e halcs_get_afc_diag_build_revision (halcs_client_t *self, char *service,
        struct _smio_afc_diag_revision_data_t *revision_data);
halcs_client_err_e halcs_get_afc_diag_build_date (halcs_client_t *self, char *service,
        struct _smio_afc_diag_revision_data_t *revision_data);
halcs_client_err_e halcs_get_afc_diag_build_user_name (halcs_client_t *self, char *service,
        struct _smio_afc_diag_revision_data_t *revision_data);
halcs_client_err_e halcs_get_afc_diag_build_user_email (halcs_client_t *self, char *service,
        struct _smio_afc_diag_revision_data_t *revision_data);

/****************************** Trigger Functions ****************************/

/* Trigger Direction functions */
/* These set of functions write (set) or read (get) the trigger direction of
 * a specified channel "chan".
 * All of the functions returns HALCS_CLIENT_SUCCESS if the parameter was
 * correctly set or error (see halcs_client_err.h for all possible errors)*/
halcs_client_err_e halcs_set_trigger_dir (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t dir);
halcs_client_err_e halcs_get_trigger_dir (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t *dir);

/* Trigger Direction Polarity functions */
/* These set of functions write (set) or read (get) the trigger direction
 * polarity of a specified channel "chan". When set to 0, the output
 * direction signal is kept the same as the FPGA internal. When set to
 * 1, the output direction signal is reversed.
 * All of the functions returns HALCS_CLIENT_SUCCESS if the parameter was
 * correctly set or error (see halcs_client_err.h for all possible errors)*/
halcs_client_err_e halcs_set_trigger_dir_pol (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t dir_pol);
halcs_client_err_e halcs_get_trigger_dir_pol (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t *dir_pol);

/* Trigger Receive/Transmit Counter Reset functions */
/* These set of functions write (set) or read (get) the reset of the
 * receiver or transmitter trigger counters of a specified channel "chan".
 * All of the functions returns HALCS_CLIENT_SUCCESS if the parameter was
 * correctly set or error (see halcs_client_err.h for all possible errors)*/
halcs_client_err_e halcs_set_trigger_rcv_count_rst (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t rcv_count_rst);
halcs_client_err_e halcs_get_trigger_rcv_count_rst (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t *rcv_count_rst);
halcs_client_err_e halcs_set_trigger_transm_count_rst (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t transm_count_rst);
halcs_client_err_e halcs_get_trigger_transm_count_rst (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t *transm_count_rst);

/* Trigger Receive/Transmit Debounce/Extension length functions */
/* These set of functions write (set) or read (get) the debounce (for receiver) and
 * pulse extension (for transmitter) for the triggers of a specified channel "chan".
 * All of the functions returns HALCS_CLIENT_SUCCESS if the parameter was
 * correctly set or error (see halcs_client_err.h for all possible errors)*/
halcs_client_err_e halcs_set_trigger_rcv_len (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t rcv_len);
halcs_client_err_e halcs_get_trigger_rcv_len (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t *rcv_len);
halcs_client_err_e halcs_set_trigger_transm_len (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t transm_len);
halcs_client_err_e halcs_get_trigger_transm_len (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t *transm_len);

/* Trigger Receive/Transmit Trigger Counter functions */
/* These set of functions write (set) or read (get) the trigger counters
 * of a specified channel "chan".
 * All of the functions returns HALCS_CLIENT_SUCCESS if the parameter was
 * correctly set or error (see halcs_client_err.h for all possible errors)*/
halcs_client_err_e halcs_set_trigger_count_rcv (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t count_rcv);
halcs_client_err_e halcs_get_trigger_count_rcv (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t *count_rcv);
halcs_client_err_e halcs_set_trigger_count_transm (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t count_transm);
halcs_client_err_e halcs_get_trigger_count_transm (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t *count_transm);

/**************************** Trigger Mux Functions ***************************/

/* Trigger Receive Source functions */
/* These set of functions write (set) or read (get) the trigger source for the
 * receiver of a specified channel "chan".
 * All of the functions returns HALCS_CLIENT_SUCCESS if the parameter was
 * correctly set or error (see halcs_client_err.h for all possible errors)*/
halcs_client_err_e halcs_set_trigger_rcv_src (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t rcv_src);
halcs_client_err_e halcs_get_trigger_rcv_src (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t *rcv_src);

/* Trigger Receive Selection functions */
/* These set of functions write (set) or read (get) the trigger selection for the
 * receiver of a specified channel "chan".
 * All of the functions returns HALCS_CLIENT_SUCCESS if the parameter was
 * correctly set or error (see halcs_client_err.h for all possible errors)*/
halcs_client_err_e halcs_set_trigger_rcv_in_sel (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t rcv_in_sel);
halcs_client_err_e halcs_get_trigger_rcv_in_sel (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t *rcv_in_sel);

/* Trigger Transmit Source functions */
/* These set of functions write (set) or read (get) the trigger source for the
 * transmitter of a specified channel "chan".
 * All of the functions returns HALCS_CLIENT_SUCCESS if the parameter was
 * correctly set or error (see halcs_client_err.h for all possible errors)*/
halcs_client_err_e halcs_set_trigger_transm_src (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t transm_src);
halcs_client_err_e halcs_get_trigger_transm_src (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t *transm_src);

/* Trigger Transmit Selection functions */
/* These set of functions write (set) or read (get) the trigger selection for the
 * transmitter of a specified channel "chan".
 * All of the functions returns HALCS_CLIENT_SUCCESS if the parameter was
 * correctly set or error (see halcs_client_err.h for all possible errors)*/
halcs_client_err_e halcs_set_trigger_transm_out_sel (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t transm_out_sel);
halcs_client_err_e halcs_get_trigger_transm_out_sel (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t *transm_out_sel);

/****************************** Helper Functions ****************************/
/* Helper Function */

/* This function execute the given function *func in a disp_op_t
 * for a specific amount of time (timeout).
 * Returns HALCS_CLIENT_SUCCESS if the functions has been successfully
 * executed or error otherwise.
 * (see halcs_client_err.h for all possible errors)*/
halcs_client_err_e func_polling (halcs_client_t *self, char *name,
        char *service, uint32_t *input, uint32_t *output, int timeout);

/********************** AFC Timing Functions ********************/

/* Status functions */
halcs_client_err_e afc_timing_get_link_status (halcs_client_t *self, char *service,
        uint32_t *link_status);
halcs_client_err_e afc_timing_get_rxen_status (halcs_client_t *self, char *service,
        uint32_t *rxen_status);
halcs_client_err_e afc_timing_get_ref_clk_locked (halcs_client_t *self, char *service,
        uint32_t *ref_clk_locked);

/* Event code channel 0 functions */
halcs_client_err_e afc_timing_set_evt_code_0 (halcs_client_t *self, char *service,
        uint32_t evt_code_0);
halcs_client_err_e afc_timing_get_evt_code_0 (halcs_client_t *self, char *service,
        uint32_t *evt_code_0);

/* Event code channel 1 functions */
halcs_client_err_e afc_timing_set_evt_code_1 (halcs_client_t *self, char *service,
        uint32_t evt_code_1);
halcs_client_err_e afc_timing_get_evt_code_1 (halcs_client_t *self, char *service,
        uint32_t *evt_code_1);

/* Event code channel 2 functions */
halcs_client_err_e afc_timing_set_evt_code_2 (halcs_client_t *self, char *service,
        uint32_t evt_code_2);
halcs_client_err_e afc_timing_get_evt_code_2 (halcs_client_t *self, char *service,
        uint32_t *evt_code_2);

/* Event code channel 3 functions */
halcs_client_err_e afc_timing_set_evt_code_3 (halcs_client_t *self, char *service,
        uint32_t evt_code_3);
halcs_client_err_e afc_timing_get_evt_code_3 (halcs_client_t *self, char *service,
        uint32_t *evt_code_3);

/* Event code channel 4 functions */
halcs_client_err_e afc_timing_set_evt_code_4 (halcs_client_t *self, char *service,
        uint32_t evt_code_4);
halcs_client_err_e afc_timing_get_evt_code_4 (halcs_client_t *self, char *service,
        uint32_t *evt_code_4);

/* Event code channel 5 functions */
halcs_client_err_e afc_timing_set_evt_code_5 (halcs_client_t *self, char *service,
        uint32_t evt_code_5);
halcs_client_err_e afc_timing_get_evt_code_5 (halcs_client_t *self, char *service,
        uint32_t *evt_code_5);

/* Event code channel 6 functions */
halcs_client_err_e afc_timing_set_evt_code_6 (halcs_client_t *self, char *service,
        uint32_t evt_code_6);
halcs_client_err_e afc_timing_get_evt_code_6 (halcs_client_t *self, char *service,
        uint32_t *evt_code_6);

/* Event delay channel 0 functions */
halcs_client_err_e afc_timing_set_evt_delay_0 (halcs_client_t *self, char *service,
        uint32_t evt_delay_0);
halcs_client_err_e afc_timing_get_evt_delay_0 (halcs_client_t *self, char *service,
        uint32_t *evt_delay_0);

/* Event delay channel 1 functions */
halcs_client_err_e afc_timing_set_evt_delay_1 (halcs_client_t *self, char *service,
        uint32_t evt_delay_1);
halcs_client_err_e afc_timing_get_evt_delay_1 (halcs_client_t *self, char *service,
        uint32_t *evt_delay_1);

/* Event delay channel 2 functions */
halcs_client_err_e afc_timing_set_evt_delay_2 (halcs_client_t *self, char *service,
        uint32_t evt_delay_2);
halcs_client_err_e afc_timing_get_evt_delay_2 (halcs_client_t *self, char *service,
        uint32_t *evt_delay_2);

/* Event delay channel 3 functions */
halcs_client_err_e afc_timing_set_evt_delay_3 (halcs_client_t *self, char *service,
        uint32_t evt_delay_3);
halcs_client_err_e afc_timing_get_evt_delay_3 (halcs_client_t *self, char *service,
        uint32_t *evt_delay_3);

/* Event delay channel 4 functions */
halcs_client_err_e afc_timing_set_evt_delay_4 (halcs_client_t *self, char *service,
        uint32_t evt_delay_4);
halcs_client_err_e afc_timing_get_evt_delay_4 (halcs_client_t *self, char *service,
        uint32_t *evt_delay_4);

/* Event delay channel 5 functions */
halcs_client_err_e afc_timing_set_evt_delay_5 (halcs_client_t *self, char *service,
        uint32_t evt_delay_5);
halcs_client_err_e afc_timing_get_evt_delay_5 (halcs_client_t *self, char *service,
        uint32_t *evt_delay_5);

/* Event delay channel 6 functions */
halcs_client_err_e afc_timing_set_evt_delay_6 (halcs_client_t *self, char *service,
        uint32_t evt_delay_6);
halcs_client_err_e afc_timing_get_evt_delay_6 (halcs_client_t *self, char *service,
        uint32_t *evt_delay_6);

/* Event width channel 0 functions */
halcs_client_err_e afc_timing_set_evt_width_0 (halcs_client_t *self, char *service,
        uint32_t evt_width_0);
halcs_client_err_e afc_timing_get_evt_width_0 (halcs_client_t *self, char *service,
        uint32_t *evt_width_0);

/* Event width channel 1 functions */
halcs_client_err_e afc_timing_set_evt_width_1 (halcs_client_t *self, char *service,
        uint32_t evt_width_1);
halcs_client_err_e afc_timing_get_evt_width_1 (halcs_client_t *self, char *service,
        uint32_t *evt_width_1);

/* Event width channel 2 functions */
halcs_client_err_e afc_timing_set_evt_width_2 (halcs_client_t *self, char *service,
        uint32_t evt_width_2);
halcs_client_err_e afc_timing_get_evt_width_2 (halcs_client_t *self, char *service,
        uint32_t *evt_width_2);

/* Event width channel 3 functions */
halcs_client_err_e afc_timing_set_evt_width_3 (halcs_client_t *self, char *service,
        uint32_t evt_width_3);
halcs_client_err_e afc_timing_get_evt_width_3 (halcs_client_t *self, char *service,
        uint32_t *evt_width_3);

/* Event width channel 4 functions */
halcs_client_err_e afc_timing_set_evt_width_4 (halcs_client_t *self, char *service,
        uint32_t evt_width_4);
halcs_client_err_e afc_timing_get_evt_width_4 (halcs_client_t *self, char *service,
        uint32_t *evt_width_4);

/* Event width channel 5 functions */
halcs_client_err_e afc_timing_set_evt_width_5 (halcs_client_t *self, char *service,
        uint32_t evt_width_5);
halcs_client_err_e afc_timing_get_evt_width_5 (halcs_client_t *self, char *service,
        uint32_t *evt_width_5);

/* Event width channel 6 functions */
halcs_client_err_e afc_timing_set_evt_width_6 (halcs_client_t *self, char *service,
        uint32_t evt_width_6);
halcs_client_err_e afc_timing_get_evt_width_6 (halcs_client_t *self, char *service,
        uint32_t *evt_width_6);

/* Proportional Gain of Frequency feedback functions */
halcs_client_err_e afc_timing_set_freq_kp (halcs_client_t *self, char *service,
        uint32_t freq_kp);
halcs_client_err_e afc_timing_get_freq_kp (halcs_client_t *self, char *service,
        uint32_t *freq_kp);

/* Integral Gain of Frequency feedback functions */
halcs_client_err_e afc_timing_set_freq_ki (halcs_client_t *self, char *service,
        uint32_t freq_ki);
halcs_client_err_e afc_timing_get_freq_ki (halcs_client_t *self, char *service,
        uint32_t *freq_ki);

/* Proportional Gain of Phase feedback functions */
halcs_client_err_e afc_timing_set_phase_kp (halcs_client_t *self, char *service,
        uint32_t phase_kp);
halcs_client_err_e afc_timing_get_phase_kp (halcs_client_t *self, char *service,
        uint32_t *phase_kp);

/* Integral Gain of Phase feedback functions */
halcs_client_err_e afc_timing_set_phase_ki (halcs_client_t *self, char *service,
        uint32_t phase_ki);
halcs_client_err_e afc_timing_get_phase_ki (halcs_client_t *self, char *service,
        uint32_t *phase_ki);

/* Phase Setpoint functions */
halcs_client_err_e afc_timing_set_phase_set (halcs_client_t *self, char *service,
        uint32_t phase_set);
halcs_client_err_e afc_timing_get_phase_set (halcs_client_t *self, char *service,
        uint32_t *phase_set);

/* Average Exponent functions */
halcs_client_err_e afc_timing_set_avg_exponent (halcs_client_t *self, char *service,
        uint32_t avg_exponent);
halcs_client_err_e afc_timing_get_avg_exponent (halcs_client_t *self, char *service,
        uint32_t *avg_exponent);

/* RTM Si570 rfreq[37-19] functions */
halcs_client_err_e afc_timing_set_rtm_rfreq_hi (halcs_client_t *self, char *service,
        uint32_t rtm_rfreq_hi);
halcs_client_err_e afc_timing_get_rtm_rfreq_hi (halcs_client_t *self, char *service,
        uint32_t *rtm_rfreq_hi);

/* RTM Si570 rfreq[18-0] functions */
halcs_client_err_e afc_timing_set_rtm_rfreq_lo (halcs_client_t *self, char *service,
        uint32_t rtm_rfreq_lo);
halcs_client_err_e afc_timing_get_rtm_rfreq_lo (halcs_client_t *self, char *service,
        uint32_t *rtm_rfreq_lo);

/* RTM Si570 n1 functions */
halcs_client_err_e afc_timing_set_rtm_n1 (halcs_client_t *self, char *service,
        uint32_t rtm_n1);
halcs_client_err_e afc_timing_get_rtm_n1 (halcs_client_t *self, char *service,
        uint32_t *rtm_n1);

/* RTM Si570 hs_div functions */
halcs_client_err_e afc_timing_set_rtm_hs_div (halcs_client_t *self, char *service,
        uint32_t rtm_hs_div);
halcs_client_err_e afc_timing_get_rtm_hs_div (halcs_client_t *self, char *service,
        uint32_t *rtm_hs_div);

/* AFC Si570 rfreq[37-19] functions */
halcs_client_err_e afc_timing_set_afc_rfreq_hi (halcs_client_t *self, char *service,
        uint32_t afc_rfreq_hi);
halcs_client_err_e afc_timing_get_afc_rfreq_hi (halcs_client_t *self, char *service,
        uint32_t *afc_rfreq_hi);

/* AFC Si570 rfreq[18-0] functions */
halcs_client_err_e afc_timing_set_afc_rfreq_lo (halcs_client_t *self, char *service,
        uint32_t afc_rfreq_lo);
halcs_client_err_e afc_timing_get_afc_rfreq_lo (halcs_client_t *self, char *service,
        uint32_t *afc_rfreq_lo);

/* AFC Si570 n1 functions */
halcs_client_err_e afc_timing_set_afc_n1 (halcs_client_t *self, char *service,
        uint32_t afc_n1);
halcs_client_err_e afc_timing_get_afc_n1 (halcs_client_t *self, char *service,
        uint32_t *afc_n1);

/* AFC Si570 hs_div functions */
halcs_client_err_e afc_timing_set_afc_hs_div (halcs_client_t *self, char *service,
        uint32_t afc_hs_div);
halcs_client_err_e afc_timing_get_afc_hs_div (halcs_client_t *self, char *service,
        uint32_t *afc_hs_div);

#ifdef __cplusplus
}
#endif

#endif
