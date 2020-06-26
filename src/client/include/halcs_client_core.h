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
struct _smio_dsp_data_t;
struct _smio_dsp_monit_data_t;

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
halcs_client_err_e halcs_func_exec_size (halcs_client_t *self, const disp_op_t *func,
        char *service, uint32_t *input, uint32_t *output, uint32_t output_size);

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

/******************** FMC100M SMIO Functions ******************/

/* Read FMC status.
 * Returns HALCS_CLIENT_SUCCESS if ok and HALCS_CLIIENT_ERR_SERVER if
 * if server could not complete the request */
halcs_client_err_e halcs_get_adc100_status (halcs_client_t *self, char *service,
        uint32_t *adc100_status);

/* Read/Write FMC ACQ LED.
 * Returns HALCS_CLIENT_SUCCESS if ok and HALCS_CLIIENT_ERR_SERVER if
 * if server could not complete the request */
halcs_client_err_e halcs_set_adc100_acq_led (halcs_client_t *self, char *service,
        uint32_t adc100_acq_led);
halcs_client_err_e halcs_get_adc100_acq_led (halcs_client_t *self, char *service,
        uint32_t *adc100_acq_led);

/* Read ADCs Status FMC.
 * Returns HALCS_CLIENT_SUCCESS if ok and HALCS_CLIIENT_ERR_SERVER if
 * if server could not complete the request */
halcs_client_err_e halcs_get_adc100_adc_status (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t *adc100_adc_status);

/* Read/Write calib gain for FMC ADCs.
 * Returns HALCS_CLIENT_SUCCESS if ok and HALCS_CLIIENT_ERR_SERVER if
 * if server could not complete the request */
halcs_client_err_e halcs_set_adc100_adc_calib_gain (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t adc100_adc_calib_gain);
halcs_client_err_e halcs_get_adc100_adc_calib_gain (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t *adc100_adc_calib_gain);

/* Read/Write calib offset for FMC ADCs.
 * Returns HALCS_CLIENT_SUCCESS if ok and HALCS_CLIIENT_ERR_SERVER if
 * if server could not complete the request */
halcs_client_err_e halcs_set_adc100_adc_calib_offset (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t adc100_adc_calib_offset);
halcs_client_err_e halcs_get_adc100_adc_calib_offset (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t *adc100_adc_calib_offset);

/* Read/Write calib saturation for FMC ADCs.
 * Returns HALCS_CLIENT_SUCCESS if ok and HALCS_CLIIENT_ERR_SERVER if
 * if server could not complete the request */
halcs_client_err_e halcs_set_adc100_adc_sat (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t adc100_adc_sat);
halcs_client_err_e halcs_get_adc100_adc_sat (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t *adc100_adc_sat);

/* Read/Write Solid State Relays for FMC ADCs.
 * Returns HALCS_CLIENT_SUCCESS if ok and HALCS_CLIIENT_ERR_SERVER if
 * if server could not complete the request */
halcs_client_err_e halcs_set_adc100_adc_ssr (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t adc100_adc_ssr);
halcs_client_err_e halcs_get_adc100_adc_ssr (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t *adc100_adc_ssr);

/* Read sampling frequency.
 * Returns HALCS_CLIENT_SUCCESS if ok and HALCS_CLIIENT_ERR_SERVER if
 * if server could not complete the request */
halcs_client_err_e halcs_get_adc100_fs_freq (halcs_client_t *self, char *service,
        uint32_t *adc100_fs_freq);

/* Read/Write calibration pattern enable.
 * Returns HALCS_CLIENT_SUCCESS if ok and HALCS_CLIIENT_ERR_SERVER if
 * if server could not complete the request */
halcs_client_err_e halcs_set_adc100_ltc2174_test_patt (halcs_client_t *self, char *service,
        uint32_t adc100_ltc2174_test_patt);
halcs_client_err_e halcs_get_adc100_ltc2174_test_patt (halcs_client_t *self, char *service,
        uint32_t *adc100_ltc2174_test_patt);

/* Read/Write calibration pattern data.
 * Returns HALCS_CLIENT_SUCCESS if ok and HALCS_CLIIENT_ERR_SERVER if
 * if server could not complete the request */
halcs_client_err_e halcs_set_adc100_ltc2174_test_patt_data (halcs_client_t *self, char *service,
        uint32_t adc100_ltc2174_test_patt_data);
halcs_client_err_e halcs_get_adc100_ltc2174_test_patt_data (halcs_client_t *self, char *service,
        uint32_t *adc100_ltc2174_test_patt_data);

/* Reset LTC2174.
 * Returns HALCS_CLIENT_SUCCESS if ok and HALCS_CLIIENT_ERR_SERVER if
 * if server could not complete the request */
halcs_client_err_e halcs_set_adc100_ltc2174_reset (halcs_client_t *self, char *service,
        uint32_t adc100_ltc2174_reset);

/* Read/Write twos complement format.
 * Returns HALCS_CLIENT_SUCCESS if ok and HALCS_CLIIENT_ERR_SERVER if
 * if server could not complete the request */
halcs_client_err_e halcs_set_adc100_ltc2174_twos_compl (halcs_client_t *self, char *service,
        uint32_t adc100_ltc2174_twos_compl);
halcs_client_err_e halcs_get_adc100_ltc2174_twos_compl (halcs_client_t *self, char *service,
        uint32_t *adc100_ltc2174_twos_compl);

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

/* MMCM Reset. Sets or clears the reset to FMC ADC MMCM.
 * Returns HALCS_CLIENT_SUCCESS if ok and HALCS_CLIIENT_ERR_SERVER if
 * if server could not complete the request */
halcs_client_err_e halcs_set_adc_mmcm_rst (halcs_client_t *self, char *service,
        uint32_t adc_mmcm_rst);
halcs_client_err_e halcs_get_adc_mmcm_rst (halcs_client_t *self, char *service,
        uint32_t *adc_mmcm_rst);

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

/* RST ISLA216P Functions. Set the ADC ISLA216P to reset itself
 * 0: nothing
 * 1: reset ADCs
 *
 * Returns HALCS_CLIENT_SUCCESS if ok and HALCS_CLIIENT_ERR_SERVER if
 * if server could not complete the request */
halcs_client_err_e halcs_set_rst_isla216p (halcs_client_t *self, char *service,
        uint32_t rst_isla216p);

/* RST SWAP Functions. Set the SWAP module to reset itself
 * 0: nothing
 * 1: reset SWAP module
 *
 * Returns HALCS_CLIENT_SUCCESS if ok and HALCS_CLIIENT_ERR_SERVER if
 * if server could not complete the request */
halcs_client_err_e halcs_set_rst_swap (halcs_client_t *self, char *service,
        uint32_t rst_swap);

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
halcs_client_err_e halcs_set_si571_fstartup (halcs_client_t *self, char *service,
        double si571_fstartup);
halcs_client_err_e halcs_get_si571_fstartup (halcs_client_t *self, char *service,
        double *si571_fstartup);

/* FMC EEPROM Read/Write.
 * Returns HALCS_CLIENT_SUCCESS if ok and HALCS_CLIIENT_ERR_SERVER if
 * if server could not complete the request */
halcs_client_err_e halcs_set_24aa64_data (halcs_client_t *self, char *service,
        uint32_t addr, uint32_t data);
halcs_client_err_e halcs_get_24aa64_data (halcs_client_t *self, char *service,
        uint32_t addr, uint32_t *data);

/* AD9510 Read/Write.
 * Returns HALCS_CLIENT_SUCCESS if ok and HALCS_CLIIENT_ERR_SERVER if
 * if server could not complete the request */
halcs_client_err_e halcs_set_ad9510_data (halcs_client_t *self, char *service,
        uint32_t addr, uint32_t data);
halcs_client_err_e halcs_get_ad9510_data (halcs_client_t *self, char *service,
        uint32_t addr, uint32_t *data);

/******************** FMC250M SMIO Functions ******************/

/* The three set of group functions provide a low-lovel interface to the FPGA
 * firmware. The correct usage to set the ADC clock and data paths delay are:
 *
 * 1) Set the delay value with halcs_set_adc250_dly_val<*> for the desired channel.
 *      Accepted values are from 0 to 31.
 * 2) Set the delay line which will be updated with halcs_set_adc250_dly_line<*> for the desired channel
 *      Accepted values are the bitmask of the desired lines with bit 16 corresponding
 *      to the clock line and bits 15-0 to ADC bits 15 to 0
 * 3) call the update functions halcs_set_adc250_dly_updt<*> for the desired channel.
 */

/* These set of functions read/write (set/get) the ADC delay values.
 * All of the functions returns HALCS_CLIENT_SUCCESS if the
 * parameter was correctly set or error (see halcs_client_err.h
 * for all possible errors)*/
halcs_client_err_e halcs_set_adc250_data_dly0 (halcs_client_t *self, char *service,
        uint32_t adc250_data_dly0);
halcs_client_err_e halcs_get_adc250_data_dly0 (halcs_client_t *self, char *service,
        uint32_t *adc250_data_dly0);
halcs_client_err_e halcs_set_adc250_data_dly1 (halcs_client_t *self, char *service,
        uint32_t adc250_data_dly1);
halcs_client_err_e halcs_get_adc250_data_dly1 (halcs_client_t *self, char *service,
        uint32_t *adc250_data_dly1);
halcs_client_err_e halcs_set_adc250_data_dly2 (halcs_client_t *self, char *service,
        uint32_t adc250_data_dly2);
halcs_client_err_e halcs_get_adc250_data_dly2 (halcs_client_t *self, char *service,
        uint32_t *adc250_data_dly2);
halcs_client_err_e halcs_set_adc250_data_dly3 (halcs_client_t *self, char *service,
        uint32_t adc250_data_dly3);
halcs_client_err_e halcs_get_adc250_data_dly3 (halcs_client_t *self, char *service,
        uint32_t *adc250_data_dly3);

halcs_client_err_e halcs_set_adc250_clk_dly0 (halcs_client_t *self, char *service,
        uint32_t adc250_clk_dly0);
halcs_client_err_e halcs_get_adc250_clk_dly0 (halcs_client_t *self, char *service,
        uint32_t *adc250_clk_dly0);
halcs_client_err_e halcs_set_adc250_clk_dly1 (halcs_client_t *self, char *service,
        uint32_t adc250_clk_dly1);
halcs_client_err_e halcs_get_adc250_clk_dly1 (halcs_client_t *self, char *service,
        uint32_t *adc250_clk_dly1);
halcs_client_err_e halcs_set_adc250_clk_dly2 (halcs_client_t *self, char *service,
        uint32_t adc250_clk_dly2);
halcs_client_err_e halcs_get_adc250_clk_dly2 (halcs_client_t *self, char *service,
        uint32_t *adc250_clk_dly2);
halcs_client_err_e halcs_set_adc250_clk_dly3 (halcs_client_t *self, char *service,
        uint32_t adc250_clk_dly3);
halcs_client_err_e halcs_get_adc250_clk_dly3 (halcs_client_t *self, char *service,
        uint32_t *adc250_clk_dly3);

/* ADC delay line functions */
/* These set of functions read/write (set/get) the ADC delay line values,
 * meaning which lines (16 = clock, 15-0 = data bits) are to be updated
 * on halcs_set_adc250_dly_updt<*> functions.
 * All of the functions returns HALCS_CLIENT_SUCCESS if the
 * parameter was correctly set or error (see halcs_client_err.h
 * for all possible errors)*/
halcs_client_err_e halcs_set_adc250_dly_line0 (halcs_client_t *self, char *service,
        uint32_t adc250_dly_line0);
halcs_client_err_e halcs_get_adc250_dly_line0 (halcs_client_t *self, char *service,
        uint32_t *adc250_dly_line0);
halcs_client_err_e halcs_set_adc250_dly_line1 (halcs_client_t *self, char *service,
        uint32_t adc250_dly_line1);
halcs_client_err_e halcs_get_adc250_dly_line1 (halcs_client_t *self, char *service,
        uint32_t *adc250_dly_line1);
halcs_client_err_e halcs_set_adc250_dly_line2 (halcs_client_t *self, char *service,
        uint32_t adc250_dly_line2);
halcs_client_err_e halcs_get_adc250_dly_line2 (halcs_client_t *self, char *service,
        uint32_t *adc250_dly_line2);
halcs_client_err_e halcs_set_adc250_dly_line3 (halcs_client_t *self, char *service,
        uint32_t adc250_dly_line3);
halcs_client_err_e halcs_get_adc250_dly_line3 (halcs_client_t *self, char *service,
        uint32_t *adc250_dly_line3);

/* ADC update functions */
/* These set of functions read/write (set/get) the ADC delay update values
 * for each channel. This will effectively update the FPGA delay primitives to
 * the previous delay values set by the corresponding halcs_get_adc250_dly_updt<*>
 * functions.
 * All of the functions returns HALCS_CLIENT_SUCCESS if the
 * parameter was correctly set or error (see halcs_client_err.h
 * for all possible errors)*/
halcs_client_err_e halcs_set_adc250_dly_updt0 (halcs_client_t *self, char *service,
        uint32_t adc250_dly_updt0);
halcs_client_err_e halcs_get_adc250_dly_updt0 (halcs_client_t *self, char *service,
        uint32_t *adc250_dly_updt0);
halcs_client_err_e halcs_set_adc250_dly_updt1 (halcs_client_t *self, char *service,
        uint32_t adc250_dly_updt1);
halcs_client_err_e halcs_get_adc250_dly_updt1 (halcs_client_t *self, char *service,
        uint32_t *adc250_dly_updt1);
halcs_client_err_e halcs_set_adc250_dly_updt2 (halcs_client_t *self, char *service,
        uint32_t adc250_dly_updt2);
halcs_client_err_e halcs_get_adc250_dly_updt2 (halcs_client_t *self, char *service,
        uint32_t *adc250_dly_updt2);
halcs_client_err_e halcs_set_adc250_dly_updt3 (halcs_client_t *self, char *service,
        uint32_t adc250_dly_updt3);
halcs_client_err_e halcs_get_adc250_dly_updt3 (halcs_client_t *self, char *service,
        uint32_t *adc250_dly_updt3);

/* Higher-level ADC delay functions */
/* These set of functions write (set) the ADC delays for each channel.
 * This functions wrap all of the functionality present by the 3 groups above
 * in a conveninent way to the user.
 * All of the functions returns HALCS_CLIENT_SUCCESS if the
 * parameter was correctly set or error (see halcs_client_err.h
 * for all possible errors)*/
halcs_client_err_e halcs_set_adc250_dly0 (halcs_client_t *self, char *service,
        uint32_t dly_type0, uint32_t dly_val0);
halcs_client_err_e halcs_set_adc250_dly1 (halcs_client_t *self, char *service,
        uint32_t dly_type1, uint32_t dly_val1);
halcs_client_err_e halcs_set_adc250_dly2 (halcs_client_t *self, char *service,
        uint32_t dly_type2, uint32_t dly_val2);
halcs_client_err_e halcs_set_adc250_dly3 (halcs_client_t *self, char *service,
        uint32_t dly_type3, uint32_t dly_val3);

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

/* Set ISLA216P test mode */
halcs_client_err_e halcs_set_test_mode_adc (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t mode);

/* Set ISLA216P reset modes */
halcs_client_err_e halcs_set_rst_modes_adc (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t rst);

/* Set ISLA216P portconfig */
halcs_client_err_e halcs_set_portconfig_adc (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t portconfig);

/* Set/Get ISLA216P register values */
halcs_client_err_e halcs_set_reg_adc (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t addr, uint32_t val);
halcs_client_err_e halcs_get_reg_adc (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t addr, uint32_t *val);

/* get ISLA216P temperature */
halcs_client_err_e halcs_get_temp_adc (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t *temp);

/* get ISLA216P calibration status */
halcs_client_err_e halcs_get_cal_status_adc (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t *cal_status);

/********************** FMCPICO1M_4CH Functions ********************/

/* FMC board LEDs Control */
/* These set of functions read (get) or write (set) FMCPICO LEDs
 * All of the functions returns HALCS_CLIENT_SUCCESS if the
 * parameter was correctly set or error (see halcs_client_err.h
 * for all possible errors)*/
halcs_client_err_e halcs_set_fmcpico_led1 (halcs_client_t *self, char *service,
        uint32_t fmcpico_led1);
halcs_client_err_e halcs_get_fmcpico_led1 (halcs_client_t *self, char *service,
        uint32_t *fmcpico_led1);
halcs_client_err_e halcs_set_fmcpico_led2 (halcs_client_t *self, char *service,
        uint32_t fmcpico_led2);
halcs_client_err_e halcs_get_fmcpico_led2 (halcs_client_t *self, char *service,
        uint32_t *fmcpico_led2);

/* FMC board RNGs Control */
/* These set of functions read (get) or write (set) FMCPICO RNGs
 * All of the functions returns HALCS_CLIENT_SUCCESS if the
 * parameter was correctly set or error (see halcs_client_err.h
 * for all possible errors)*/
halcs_client_err_e halcs_set_fmcpico_rng_r0 (halcs_client_t *self, char *service,
        uint32_t fmcpico_rng_r0);
halcs_client_err_e halcs_get_fmcpico_rng_r0 (halcs_client_t *self, char *service,
        uint32_t *fmcpico_rng_r0);
halcs_client_err_e halcs_set_fmcpico_rng_r1 (halcs_client_t *self, char *service,
        uint32_t fmcpico_rng_r1);
halcs_client_err_e halcs_get_fmcpico_rng_r1 (halcs_client_t *self, char *service,
        uint32_t *fmcpico_rng_r1);
halcs_client_err_e halcs_set_fmcpico_rng_r2 (halcs_client_t *self, char *service,
        uint32_t fmcpico_rng_r2);
halcs_client_err_e halcs_get_fmcpico_rng_r2 (halcs_client_t *self, char *service,
        uint32_t *fmcpico_rng_r2);
halcs_client_err_e halcs_set_fmcpico_rng_r3 (halcs_client_t *self, char *service,
        uint32_t fmcpico_rng_r3);
halcs_client_err_e halcs_get_fmcpico_rng_r3 (halcs_client_t *self, char *service,
        uint32_t *fmcpico_rng_r3);

/* RAW ADC data functions */
/* These set of functions read (get) the RAW ADC values.
 * All of the functions returns HALCS_CLIENT_SUCCESS if the
 * parameter was correctly set or error (see halcs_client_err.h
 * for all possible errors)*/
halcs_client_err_e halcs_set_fmcpico_adc_data0 (halcs_client_t *self, char *service,
        uint32_t fmcpico_adc_data0);
halcs_client_err_e halcs_get_fmcpico_adc_data0 (halcs_client_t *self, char *service,
        uint32_t *fmcpico_adc_data0);
halcs_client_err_e halcs_set_fmcpico_adc_data1 (halcs_client_t *self, char *service,
        uint32_t fmcpico_adc_data1);
halcs_client_err_e halcs_get_fmcpico_adc_data1 (halcs_client_t *self, char *service,
        uint32_t *fmcpico_adc_data1);
halcs_client_err_e halcs_set_fmcpico_adc_data2 (halcs_client_t *self, char *service,
        uint32_t fmcpico_adc_data2);
halcs_client_err_e halcs_get_fmcpico_adc_data2 (halcs_client_t *self, char *service,
        uint32_t *fmcpico_adc_data2);
halcs_client_err_e halcs_set_fmcpico_adc_data3 (halcs_client_t *self, char *service,
        uint32_t fmcpico_adc_data3);
halcs_client_err_e halcs_get_fmcpico_adc_data3 (halcs_client_t *self, char *service,
        uint32_t *fmcpico_adc_data3);

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

/* Test Data functions */
/* These set of functions read (get) the test data enable.
 * All of the functions returns HALCS_CLIENT_SUCCESS if the
 * parameter was correctly set or error (see halcs_client_err.h
 * for all possible errors)*/
halcs_client_err_e halcs_set_dsp_cfg_test_data (halcs_client_t *self, char *service,
        uint32_t dsp_cfg_test_data);
halcs_client_err_e halcs_get_dsp_cfg_test_data (halcs_client_t *self, char *service,
        uint32_t *dsp_cfg_test_data);

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

/* Get Monitoring Amp/Pos values */
/* All of the functions returns HALCS_CLIENT_SUCCESS if the
 * parameter was correctly set or error (see halcs_client_err.h
 * for all possible errors) */
halcs_client_err_e halcs_get_monit_amp_pos (halcs_client_t *self, char *service,
        struct _smio_dsp_data_t *dsp_data);

/* Monitoring 1 Position values */
/* These set of functions read (get) the Monitoring position values.
 * All of the functions returns HALCS_CLIENT_SUCCESS if the
 * parameter was correctly set or error (see halcs_client_err.h
 * for all possible errors)*/
halcs_client_err_e halcs_set_monit1_pos_x (halcs_client_t *self, char *service,
        uint32_t monit1_pos_x);
halcs_client_err_e halcs_get_monit1_pos_x (halcs_client_t *self, char *service,
        uint32_t *monit1_pos_x);
halcs_client_err_e halcs_set_monit1_pos_y (halcs_client_t *self, char *service,
        uint32_t monit1_pos_y);
halcs_client_err_e halcs_get_monit1_pos_y (halcs_client_t *self, char *service,
        uint32_t *monit1_pos_y);
halcs_client_err_e halcs_set_monit1_pos_q (halcs_client_t *self, char *service,
        uint32_t monit1_pos_q);
halcs_client_err_e halcs_get_monit1_pos_q (halcs_client_t *self, char *service,
        uint32_t *monit1_pos_q);
halcs_client_err_e halcs_set_monit1_pos_sum (halcs_client_t *self, char *service,
        uint32_t monit1_pos_sum);
halcs_client_err_e halcs_get_monit1_pos_sum (halcs_client_t *self, char *service,
        uint32_t *monit1_pos_sum);

/* Monitoring 1 Amplitude values */
/* These set of functions read (get) the Monitoring amplitude values.
 * All of the functions returns HALCS_CLIENT_SUCCESS if the
 * parameter was correctly set or error (see halcs_client_err.h
 * for all possible errors)*/
halcs_client_err_e halcs_set_monit1_amp_ch0 (halcs_client_t *self, char *service,
        uint32_t monit1_amp_ch0);
halcs_client_err_e halcs_get_monit1_amp_ch0 (halcs_client_t *self, char *service,
        uint32_t *monit1_amp_ch0);
halcs_client_err_e halcs_set_monit1_amp_ch1 (halcs_client_t *self, char *service,
        uint32_t monit1_amp_ch1);
halcs_client_err_e halcs_get_monit1_amp_ch1 (halcs_client_t *self, char *service,
        uint32_t *monit1_amp_ch1);
halcs_client_err_e halcs_set_monit1_amp_ch2 (halcs_client_t *self, char *service,
        uint32_t monit1_amp_ch2);
halcs_client_err_e halcs_get_monit1_amp_ch2 (halcs_client_t *self, char *service,
        uint32_t *monit1_amp_ch2);
halcs_client_err_e halcs_set_monit1_amp_ch3 (halcs_client_t *self, char *service,
        uint32_t monit1_amp_ch3);
halcs_client_err_e halcs_get_monit1_amp_ch3 (halcs_client_t *self, char *service,
        uint32_t *monit1_amp_ch3);

/* Monitoring 1 Update values */
/* These set of functions read (get) the Monitoring update value, which
 * effectively updates the AMP/POS values in the FPGA. After this,
 * reading AMP/POS values are guaranteed to stay fixed until the next
 * update.
 * All of the functions returns HALCS_CLIENT_SUCCESS if the
 * parameter was correctly set or error (see halcs_client_err.h
 * for all possible errors)*/
halcs_client_err_e halcs_set_monit1_updt (halcs_client_t *self, char *service,
        uint32_t monit1_updt);
halcs_client_err_e halcs_get_monit1_updt (halcs_client_t *self, char *service,
        uint32_t *monit1_updt);

/* Get Monitoring 1 Amp/Pos values */
/* All of the functions returns HALCS_CLIENT_SUCCESS if the
 * parameter was correctly set or error (see halcs_client_err.h
 * for all possible errors) */
halcs_client_err_e halcs_get_monit1_amp_pos (halcs_client_t *self, char *service,
        struct _smio_dsp_data_t *dsp_data);

/* Switching Tag Enable. This sets/gets the tag synchronization */
/* All of the functions returns HALCS_CLIENT_SUCCESS if the
 * parameter was correctly set or error (see halcs_client_err.h
 * for all possible errors)*/
halcs_client_err_e halcs_set_sw_tag_en (halcs_client_t *self, char *service,
        uint32_t sw_tag_en);
halcs_client_err_e halcs_get_sw_tag_en (halcs_client_t *self, char *service,
        uint32_t *sw_tag_en);

/* Switching Data Mask Enable. This sets/gets the Switching masking of samples*/
halcs_client_err_e halcs_set_sw_data_mask_en (halcs_client_t *self, char *service,
        uint32_t sw_data_mask_en);
halcs_client_err_e halcs_get_sw_data_mask_en (halcs_client_t *self, char *service,
        uint32_t *sw_data_mask_en);

/* Switching Data Mask. This sets/gets the Switching mask to the specified number of samples*/
halcs_client_err_e halcs_set_sw_data_mask_samples (halcs_client_t *self, char *service,
        uint32_t sw_data_mask_samples);
halcs_client_err_e halcs_get_sw_data_mask_samples (halcs_client_t *self, char *service,
        uint32_t *sw_data_mask_samples);

/* TBT Tag Enable. This sets/gets the tag synchronization */
/* All of the functions returns HALCS_CLIENT_SUCCESS if the
 * parameter was correctly set or error (see halcs_client_err.h
 * for all possible errors)*/
halcs_client_err_e halcs_set_tbt_tag_en (halcs_client_t *self, char *service,
        uint32_t tbt_tag_en);
halcs_client_err_e halcs_get_tbt_tag_en (halcs_client_t *self, char *service,
        uint32_t *tbt_tag_en);

/* TBT Tag Delay. This sets/gets the tag delay synchronization */
halcs_client_err_e halcs_set_tbt_tag_dly (halcs_client_t *self, char *service,
        uint32_t tbt_tag_dly);
halcs_client_err_e halcs_get_tbt_tag_dly (halcs_client_t *self, char *service,
        uint32_t *tbt_tag_dly);

/* TBT Data Mask Enable. This sets/gets the TBT masking of samples*/
halcs_client_err_e halcs_set_tbt_data_mask_en (halcs_client_t *self, char *service,
        uint32_t tbt_data_mask_en);
halcs_client_err_e halcs_get_tbt_data_mask_en (halcs_client_t *self, char *service,
        uint32_t *tbt_data_mask_en);

/* TBT Data Mask. This sets/gets the TBT mask to the specified number of samples at the beginning */
halcs_client_err_e halcs_set_tbt_data_mask_samples_beg (halcs_client_t *self, char *service,
        uint32_t tbt_data_mask_samples_beg);
halcs_client_err_e halcs_get_tbt_data_mask_samples_beg (halcs_client_t *self, char *service,
        uint32_t *tbt_data_mask_samples_beg);

/* TBT Data Mask Ending. This sets/gets the TBT mask to the specified number of samples at the end*/
halcs_client_err_e halcs_set_tbt_data_mask_samples_end (halcs_client_t *self, char *service,
        uint32_t tbt_data_mask_samples_end);
halcs_client_err_e halcs_get_tbt_data_mask_samples_end (halcs_client_t *self, char *service,
        uint32_t *tbt_data_mask_samples_end);

/* SW Tag Desync Counter Reset. This sets/gets the SW Tag desync counter reset */
halcs_client_err_e halcs_set_sw_tag_desync_cnt_rst (halcs_client_t *self, char *service,
        uint32_t sw_tag_desync_cnt_rst);
halcs_client_err_e halcs_get_sw_tag_desync_cnt_rst (halcs_client_t *self, char *service,
        uint32_t *sw_tag_desync_cnt_rst);

/* SW tag desync counter. This gets the Swithcing tag desync counter */
halcs_client_err_e halcs_get_sw_tag_desync_cnt (halcs_client_t *self, char *service,
        uint32_t *sw_tag_desync_cnt);

/* TBT Tag Desync Counter Reset. This sets/gets the TBT Tag desync counter reset */
halcs_client_err_e halcs_set_tbt_tag_desync_cnt_rst (halcs_client_t *self, char *service,
        uint32_t tbt_tag_desync_cnt_rst);
halcs_client_err_e halcs_get_tbt_tag_desync_cnt_rst (halcs_client_t *self, char *service,
        uint32_t *tbt_tag_desync_cnt_rst);

/* TBT tag desync counter. This gets the Swithcing tag desync counter */
halcs_client_err_e halcs_get_tbt_tag_desync_cnt (halcs_client_t *self, char *service,
        uint32_t *tbt_tag_desync_cnt);

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
#define MIN_WAIT_TIME           1                           /* in ms */
#define MSECS                   1000                        /* in seconds */

/* Helper Function */

/* This function execute the given function *func in a disp_op_t
 * for a specific amount of time (timeout).
 * Returns HALCS_CLIENT_SUCCESS if the functions has been successfully
 * executed or error otherwise.
 * (see halcs_client_err.h for all possible errors)*/
halcs_client_err_e func_polling (halcs_client_t *self, char *name,
        char *service, uint32_t *input, uint32_t *output, int timeout);

/********************** AFC Timing Functions ********************/

halcs_client_err_e afc_timing_set_link_status (halcs_client_t *self, char *service,
        uint32_t link_status);
halcs_client_err_e afc_timing_get_link_status (halcs_client_t *self, char *service,
        uint32_t *link_status);

halcs_client_err_e afc_timing_set_rxen_status (halcs_client_t *self, char *service,
        uint32_t rxen_status);
halcs_client_err_e afc_timing_get_rxen_status (halcs_client_t *self, char *service,
        uint32_t *rxen_status);

halcs_client_err_e afc_timing_set_ref_clk_locked (halcs_client_t *self, char *service,
        uint32_t ref_clk_locked);
halcs_client_err_e afc_timing_get_ref_clk_locked (halcs_client_t *self, char *service,
        uint32_t *ref_clk_locked);

halcs_client_err_e afc_timing_set_evren (halcs_client_t *self, char *service,
        uint32_t evren);
halcs_client_err_e afc_timing_get_evren (halcs_client_t *self, char *service,
        uint32_t *evren);

halcs_client_err_e afc_timing_set_alive (halcs_client_t *self, char *service,
        uint32_t alive);
halcs_client_err_e afc_timing_get_alive (halcs_client_t *self, char *service,
        uint32_t *alive);

halcs_client_err_e halcs_set_afc_timing_amc_en (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t amc_en);
halcs_client_err_e halcs_get_afc_timing_amc_en (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t *amc_en);

halcs_client_err_e halcs_set_afc_timing_amc_pol (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t amc_pol);
halcs_client_err_e halcs_get_afc_timing_amc_pol (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t *amc_pol);

halcs_client_err_e halcs_set_afc_timing_amc_log (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t amc_log);
halcs_client_err_e halcs_get_afc_timing_amc_log (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t *amc_log);

halcs_client_err_e halcs_set_afc_timing_amc_itl (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t amc_itl);
halcs_client_err_e halcs_get_afc_timing_amc_itl (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t *amc_itl);

halcs_client_err_e halcs_set_afc_timing_amc_src (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t amc_src);
halcs_client_err_e halcs_get_afc_timing_amc_src (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t *amc_src);

halcs_client_err_e halcs_set_afc_timing_amc_dir (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t amc_dir);
halcs_client_err_e halcs_get_afc_timing_amc_dir (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t *amc_dir);

halcs_client_err_e halcs_set_afc_timing_amc_pulses (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t amc_pulses);
halcs_client_err_e halcs_get_afc_timing_amc_pulses (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t *amc_pulses);

halcs_client_err_e halcs_set_afc_timing_amc_evt (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t amc_evt);
halcs_client_err_e halcs_get_afc_timing_amc_evt (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t *amc_evt);

halcs_client_err_e halcs_set_afc_timing_amc_dly (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t amc_dly);
halcs_client_err_e halcs_get_afc_timing_amc_dly (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t *amc_dly);

halcs_client_err_e halcs_set_afc_timing_amc_wdt (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t amc_wdt);
halcs_client_err_e halcs_get_afc_timing_amc_wdt (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t *amc_wdt);

halcs_client_err_e halcs_set_afc_timing_fmc1_en (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t fmc1_en);
halcs_client_err_e halcs_get_afc_timing_fmc1_en (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t *fmc1_en);

halcs_client_err_e halcs_set_afc_timing_fmc1_pol (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t fmc1_pol);
halcs_client_err_e halcs_get_afc_timing_fmc1_pol (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t *fmc1_pol);

halcs_client_err_e halcs_set_afc_timing_fmc1_log (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t fmc1_log);
halcs_client_err_e halcs_get_afc_timing_fmc1_log (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t *fmc1_log);

halcs_client_err_e halcs_set_afc_timing_fmc1_itl (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t fmc1_itl);
halcs_client_err_e halcs_get_afc_timing_fmc1_itl (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t *fmc1_itl);

halcs_client_err_e halcs_set_afc_timing_fmc1_src (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t fmc1_src);
halcs_client_err_e halcs_get_afc_timing_fmc1_src (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t *fmc1_src);

halcs_client_err_e halcs_set_afc_timing_fmc1_dir (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t fmc1_dir);
halcs_client_err_e halcs_get_afc_timing_fmc1_dir (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t *fmc1_dir);

halcs_client_err_e halcs_set_afc_timing_fmc1_pulses (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t fmc1_pulses);
halcs_client_err_e halcs_get_afc_timing_fmc1_pulses (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t *fmc1_pulses);

halcs_client_err_e halcs_set_afc_timing_fmc1_evt (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t fmc1_evt);
halcs_client_err_e halcs_get_afc_timing_fmc1_evt (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t *fmc1_evt);

halcs_client_err_e halcs_set_afc_timing_fmc1_dly (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t fmc1_dly);
halcs_client_err_e halcs_get_afc_timing_fmc1_dly (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t *fmc1_dly);

halcs_client_err_e halcs_set_afc_timing_fmc1_wdt (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t fmc1_wdt);
halcs_client_err_e halcs_get_afc_timing_fmc1_wdt (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t *fmc1_wdt);

halcs_client_err_e halcs_set_afc_timing_fmc2_en (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t fmc2_en);
halcs_client_err_e halcs_get_afc_timing_fmc2_en (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t *fmc2_en);

halcs_client_err_e halcs_set_afc_timing_fmc2_pol (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t fmc2_pol);
halcs_client_err_e halcs_get_afc_timing_fmc2_pol (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t *fmc2_pol);

halcs_client_err_e halcs_set_afc_timing_fmc2_log (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t fmc2_log);
halcs_client_err_e halcs_get_afc_timing_fmc2_log (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t *fmc2_log);

halcs_client_err_e halcs_set_afc_timing_fmc2_itl (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t fmc2_itl);
halcs_client_err_e halcs_get_afc_timing_fmc2_itl (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t *fmc2_itl);

halcs_client_err_e halcs_set_afc_timing_fmc2_src (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t fmc2_src);
halcs_client_err_e halcs_get_afc_timing_fmc2_src (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t *fmc2_src);

halcs_client_err_e halcs_set_afc_timing_fmc2_dir (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t fmc2_dir);
halcs_client_err_e halcs_get_afc_timing_fmc2_dir (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t *fmc2_dir);

halcs_client_err_e halcs_set_afc_timing_fmc2_pulses (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t fmc2_pulses);
halcs_client_err_e halcs_get_afc_timing_fmc2_pulses (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t *fmc2_pulses);

halcs_client_err_e halcs_set_afc_timing_fmc2_evt (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t fmc2_evt);
halcs_client_err_e halcs_get_afc_timing_fmc2_evt (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t *fmc2_evt);

halcs_client_err_e halcs_set_afc_timing_fmc2_dly (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t fmc2_dly);
halcs_client_err_e halcs_get_afc_timing_fmc2_dly (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t *fmc2_dly);

halcs_client_err_e halcs_set_afc_timing_fmc2_wdt (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t fmc2_wdt);
halcs_client_err_e halcs_get_afc_timing_fmc2_wdt (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t *fmc2_wdt);

halcs_client_err_e afc_timing_set_rtm_freq_kp (halcs_client_t *self, char *service,
        uint32_t rtm_freq_kp);
halcs_client_err_e afc_timing_get_rtm_freq_kp (halcs_client_t *self, char *service,
        uint32_t *rtm_freq_kp);

halcs_client_err_e afc_timing_set_rtm_freq_ki (halcs_client_t *self, char *service,
        uint32_t rtm_freq_ki);
halcs_client_err_e afc_timing_get_rtm_freq_ki (halcs_client_t *self, char *service,
        uint32_t *rtm_freq_ki);

halcs_client_err_e afc_timing_set_rtm_phase_kp (halcs_client_t *self, char *service,
        uint32_t rtm_phase_kp);
halcs_client_err_e afc_timing_get_rtm_phase_kp (halcs_client_t *self, char *service,
        uint32_t *rtm_phase_kp);

halcs_client_err_e afc_timing_set_rtm_phase_ki (halcs_client_t *self, char *service,
        uint32_t rtm_phase_ki);
halcs_client_err_e afc_timing_get_rtm_phase_ki (halcs_client_t *self, char *service,
        uint32_t *rtm_phase_ki);

halcs_client_err_e afc_timing_set_rtm_phase_set (halcs_client_t *self, char *service,
        uint32_t rtm_phase_set);
halcs_client_err_e afc_timing_get_rtm_phase_set (halcs_client_t *self, char *service,
        uint32_t *rtm_phase_set);

halcs_client_err_e afc_timing_set_rtm_phase_navg (halcs_client_t *self, char *service,
        uint32_t rtm_phase_navg);
halcs_client_err_e afc_timing_get_rtm_phase_navg (halcs_client_t *self, char *service,
        uint32_t *rtm_phase_navg);

halcs_client_err_e afc_timing_set_rtm_phase_div_exp (halcs_client_t *self, char *service,
        uint32_t rtm_phase_div_exp);
halcs_client_err_e afc_timing_get_rtm_phase_div_exp (halcs_client_t *self, char *service,
        uint32_t *rtm_phase_div_exp);

halcs_client_err_e afc_timing_set_rtm_rfreq_hi (halcs_client_t *self, char *service,
        uint32_t rtm_rfreq_hi);
halcs_client_err_e afc_timing_get_rtm_rfreq_hi (halcs_client_t *self, char *service,
        uint32_t *rtm_rfreq_hi);

halcs_client_err_e afc_timing_set_rtm_rfreq_lo (halcs_client_t *self, char *service,
        uint32_t rtm_rfreq_lo);
halcs_client_err_e afc_timing_get_rtm_rfreq_lo (halcs_client_t *self, char *service,
        uint32_t *rtm_rfreq_lo);

halcs_client_err_e afc_timing_set_rtm_n1 (halcs_client_t *self, char *service,
        uint32_t rtm_n1);
halcs_client_err_e afc_timing_get_rtm_n1 (halcs_client_t *self, char *service,
        uint32_t *rtm_n1);

halcs_client_err_e afc_timing_set_rtm_hs_div (halcs_client_t *self, char *service,
        uint32_t rtm_hs_div);
halcs_client_err_e afc_timing_get_rtm_hs_div (halcs_client_t *self, char *service,
        uint32_t *rtm_hs_div);

halcs_client_err_e afc_timing_set_afc_freq_kp (halcs_client_t *self, char *service,
        uint32_t afc_freq_kp);
halcs_client_err_e afc_timing_get_afc_freq_kp (halcs_client_t *self, char *service,
        uint32_t *afc_freq_kp);

halcs_client_err_e afc_timing_set_afc_freq_ki (halcs_client_t *self, char *service,
        uint32_t afc_freq_ki);
halcs_client_err_e afc_timing_get_afc_freq_ki (halcs_client_t *self, char *service,
        uint32_t *afc_freq_ki);

halcs_client_err_e afc_timing_set_afc_phase_kp (halcs_client_t *self, char *service,
        uint32_t afc_phase_kp);
halcs_client_err_e afc_timing_get_afc_phase_kp (halcs_client_t *self, char *service,
        uint32_t *afc_phase_kp);

halcs_client_err_e afc_timing_set_afc_phase_ki (halcs_client_t *self, char *service,
        uint32_t afc_phase_ki);
halcs_client_err_e afc_timing_get_afc_phase_ki (halcs_client_t *self, char *service,
        uint32_t *afc_phase_ki);

halcs_client_err_e afc_timing_set_afc_phase_set (halcs_client_t *self, char *service,
        uint32_t afc_phase_set);
halcs_client_err_e afc_timing_get_afc_phase_set (halcs_client_t *self, char *service,
        uint32_t *afc_phase_set);

halcs_client_err_e afc_timing_set_afc_phase_navg (halcs_client_t *self, char *service,
        uint32_t afc_phase_navg);
halcs_client_err_e afc_timing_get_afc_phase_navg (halcs_client_t *self, char *service,
        uint32_t *afc_phase_navg);

halcs_client_err_e afc_timing_set_afc_phase_div_exp (halcs_client_t *self, char *service,
        uint32_t afc_phase_div_exp);
halcs_client_err_e afc_timing_get_afc_phase_div_exp (halcs_client_t *self, char *service,
        uint32_t *afc_phase_div_exp);

halcs_client_err_e afc_timing_set_afc_rfreq_hi (halcs_client_t *self, char *service,
        uint32_t afc_rfreq_hi);
halcs_client_err_e afc_timing_get_afc_rfreq_hi (halcs_client_t *self, char *service,
        uint32_t *afc_rfreq_hi);

halcs_client_err_e afc_timing_set_afc_rfreq_lo (halcs_client_t *self, char *service,
        uint32_t afc_rfreq_lo);
halcs_client_err_e afc_timing_get_afc_rfreq_lo (halcs_client_t *self, char *service,
        uint32_t *afc_rfreq_lo);

halcs_client_err_e afc_timing_set_afc_n1 (halcs_client_t *self, char *service,
        uint32_t afc_n1);
halcs_client_err_e afc_timing_get_afc_n1 (halcs_client_t *self, char *service,
        uint32_t *afc_n1);

halcs_client_err_e afc_timing_set_afc_hs_div (halcs_client_t *self, char *service,
        uint32_t afc_hs_div);
halcs_client_err_e afc_timing_get_afc_hs_div (halcs_client_t *self, char *service,
        uint32_t *afc_hs_div);

/********************** INIT Functions ********************/

/* Init Check function */
/* This function is used so a client can be sure everything is initialized properly
 * and a HALCS is ready go. The functions returns
 * HALCS_CLIENT_SUCCESS if the parameter was correctly set or error
 * (see halcs_client_err.h for all possible errors)*/

halcs_client_err_e halcs_get_init_check (halcs_client_t *self, char *service,
        uint32_t *init_check_out);

/********************** TIM RCV Functions ********************/

/* DMTD Number of averages function. The functions returns
 * HALCS_CLIENT_SUCCESS if the parameter was correctly set or error
 * (see halcs_client_err.h for all possible errors)*/
halcs_client_err_e halcs_set_phase_meas_navg (halcs_client_t *self, char *service,
        uint32_t phase_meas_navg);
halcs_client_err_e halcs_get_phase_meas_navg (halcs_client_t *self, char *service,
        uint32_t *phase_meas_navg);

/* DMTD A deglitcher threshold. The functions returns
 * HALCS_CLIENT_SUCCESS if the parameter was correctly set or error
 * (see halcs_client_err.h for all possible errors)*/
halcs_client_err_e halcs_set_dmtd_a_deglitcher_thres (halcs_client_t *self, char *service,
        uint32_t dmtd_a_deglitcher_thres);
halcs_client_err_e halcs_get_dmtd_a_deglitcher_thres (halcs_client_t *self, char *service,
        uint32_t *dmtd_a_deglitcher_thres);

/* DMTD B deglitcher threshold. The functions returns
 * HALCS_CLIENT_SUCCESS if the parameter was correctly set or error
 * (see halcs_client_err.h for all possible errors)*/
halcs_client_err_e halcs_set_dmtd_b_deglitcher_thres (halcs_client_t *self, char *service,
        uint32_t dmtd_b_deglitcher_thres);
halcs_client_err_e halcs_get_dmtd_b_deglitcher_thres (halcs_client_t *self, char *service,
        uint32_t *dmtd_b_deglitcher_thres);

/* DMTD Phase measurement function. The functions returns
 * HALCS_CLIENT_SUCCESS if the parameter was correctly set or error
 * (see halcs_client_err.h for all possible errors)*/
halcs_client_err_e halcs_set_phase_meas (halcs_client_t *self, char *service,
        uint32_t phase_meas);
halcs_client_err_e halcs_get_phase_meas (halcs_client_t *self, char *service,
        uint32_t *phase_meas);

/* DMTD A Frequency measurement function. The functions returns
 * HALCS_CLIENT_SUCCESS if the parameter was correctly set or error
 * (see halcs_client_err.h for all possible errors)*/
halcs_client_err_e halcs_set_dmtd_a_freq (halcs_client_t *self, char *service,
        uint32_t dmtd_a_freq);
halcs_client_err_e halcs_get_dmtd_a_freq (halcs_client_t *self, char *service,
        uint32_t *dmtd_a_freq);

/* DMTD A Frequency valid function. New frequncy values are update on each
 * valid output by this function. After reading the frequency value, write
 * 0 to this. The functions returns HALCS_CLIENT_SUCCESS if the parameter was
 * correctly set or error (see halcs_client_err.h for all possible errors)*/
halcs_client_err_e halcs_set_dmtd_a_valid (halcs_client_t *self, char *service,
        uint32_t dmtd_a_valid);
halcs_client_err_e halcs_get_dmtd_a_valid (halcs_client_t *self, char *service,
        uint32_t *dmtd_a_valid);

/* DMTD B Frequency measurement function. The functions returns
 * HALCS_CLIENT_SUCCESS if the parameter was correctly set or error
 * (see halcs_client_err.h for all possible errors)*/
halcs_client_err_e halcs_set_dmtd_b_freq (halcs_client_t *self, char *service,
        uint32_t dmtd_b_freq);
halcs_client_err_e halcs_get_dmtd_b_freq (halcs_client_t *self, char *service,
        uint32_t *dmtd_b_freq);

/* DMTD B Frequency valid function. New frequncy values are update on each
 * valid output by this function. After reading the frequency value, write
 * 0 to this. The functions returns HALCS_CLIENT_SUCCESS if the parameter was
 * correctly set or error (see halcs_client_err.h for all possible errors)*/
halcs_client_err_e halcs_set_dmtd_b_valid (halcs_client_t *self, char *service,
        uint32_t dmtd_b_valid);
halcs_client_err_e halcs_get_dmtd_b_valid (halcs_client_t *self, char *service,
        uint32_t *dmtd_b_valid);

#ifdef __cplusplus
}
#endif

#endif
