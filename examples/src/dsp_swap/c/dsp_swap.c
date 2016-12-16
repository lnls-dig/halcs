/*
 * Controlling the DSP and SWAP FPGA modules
 */

#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <halcs_client.h>

#define DFLT_BIND_FOLDER            "/tmp/halcs"

#define DFLT_HALCS_NUMBER             0
#define MAX_HALCS_NUMBER              1

#define DFLT_BOARD_NUMBER           0

void print_help (char *program_name)
{
    printf( "Usage: %s [options]\n"
            "\t-h This help message\n"
            "\t-v Verbose output\n"
            "\t-board <AMC board = [0|1|2|3|4|5]>\n"
            "\t-halcs <HALCS number = [0|1]>\n"
            "\t-b <broker_endpoint> Broker endpoint\n"
            "\t-sw <Switching = [0|1]> Start switching counters\n"
            "\t-div_clk <Switching divider = [0 to ADC clock period]> Switching clock divider\n"
            "\t-sw_dly <Switching/Deswitching delay = [0 to ADC clock period]> Switching/Deswitching delay\n"
            , program_name);
}

int main (int argc, char *argv [])
{
    int verbose = 0;
    char *broker_endp = NULL;
    char *board_number_str = NULL;
    char *halcs_number_str = NULL;
    char *sw_str = NULL;
    char *div_clk_str = NULL;
    char *sw_dly_str = NULL;
    char **str_p = NULL;

    if (argc < 3) {
        print_help (argv[0]);
        exit (1);
    }

    /* FIXME: This is rather buggy! */
    /* Simple handling of command-line options. This should be done
     *      * with getopt, for instance*/
    int i;
    for (i = 1; i < argc; i++)
    {
        if (streq(argv[i], "-v")) {
            verbose = 1;
        }
        else if (streq(argv[i], "-h"))
        {
            print_help (argv [0]);
            exit (1);
        }
        else if (streq(argv[i], "-board"))
        {
            str_p = &board_number_str;
        }
        else if (streq(argv[i], "-halcs"))
        {
            str_p = &halcs_number_str;
        }
        else if (streq (argv[i], "-sw")) {
            str_p = &sw_str;
        }
        else if (streq (argv[i], "-div_clk")) {
            str_p = &div_clk_str;
        }
        else if (streq (argv[i], "-sw_dly")) {
            str_p = &sw_dly_str;
        }
        else if (streq (argv[i], "-b")) {
            str_p = &broker_endp;
        }
        /* Fallout for options with parameters */
        else {
            *str_p = strdup (argv[i]);
        }
    }

    /* Set default broker address */
    if (broker_endp == NULL) {
        broker_endp = strdup ("ipc://"DFLT_BIND_FOLDER);
    }

    /* Set default board number */
    uint32_t board_number;
    if (board_number_str == NULL) {
        fprintf (stderr, "[client:dsp]: Setting default value to BOARD number: %u\n",
                DFLT_BOARD_NUMBER);
        board_number = DFLT_BOARD_NUMBER;
    }
    else {
        board_number = strtoul (board_number_str, NULL, 10);
    }

    /* Set default halcs number */
    uint32_t halcs_number;
    if (halcs_number_str == NULL) {
        fprintf (stderr, "[client:dsp]: Setting default value to HALCS number: %u\n",
                DFLT_HALCS_NUMBER);
        halcs_number = DFLT_HALCS_NUMBER;
    }
    else {
        halcs_number = strtoul (halcs_number_str, NULL, 10);

        if (halcs_number > MAX_HALCS_NUMBER) {
            fprintf (stderr, "[client:dsp]: HALCS number too big! Defaulting to: %u\n",
                    MAX_HALCS_NUMBER);
            halcs_number = MAX_HALCS_NUMBER;
        }
    }

    /* Generate the service names for each SMIO */
    char service_dsp[50];
    snprintf (service_dsp, sizeof (service_dsp), "HALCS%u:DEVIO:DSP%u", board_number, halcs_number);
    char service_swap[50];
    snprintf (service_swap, sizeof (service_swap), "HALCS%u:DEVIO:SWAP%u", board_number, halcs_number);

    halcs_client_t *halcs_client = halcs_client_new (broker_endp, verbose, NULL);
    if (halcs_client == NULL) {
        fprintf (stderr, "[client:acq]: halcs_client could be created\n");
        goto err_halcs_client_new;
    }

    uint32_t kx_set = 10000000;
    fprintf (stdout, "[client:dsp]: kx = %u\n", kx_set);
    halcs_client_err_e err = halcs_set_kx (halcs_client, service_dsp, kx_set);
    if (err != HALCS_CLIENT_SUCCESS){
        fprintf (stderr, "[client:dsp]: halcs_set_kx failed\n");
        goto err_halcs_set;
    }
    fprintf (stdout, "[client:dsp]: halcs_set_kx was successfully executed\n");

    uint32_t ds_tbt_thres = 200000;
    fprintf (stdout, "[client:dsp]: ds_tbt_thres = %u\n", ds_tbt_thres);
    err = halcs_set_ds_tbt_thres (halcs_client, service_dsp, ds_tbt_thres);
    if (err != HALCS_CLIENT_SUCCESS){
        fprintf (stderr, "[client:dsp]: halcs_set_tbt_ds_thres failed\n");
        goto err_halcs_set;
    }
    fprintf (stdout, "[client:dsp]: halcs_set_ds_tbt_thres was successfully executed\n");

    uint32_t kx_get;
    err = halcs_get_kx (halcs_client, service_dsp, &kx_get);
    if (err != HALCS_CLIENT_SUCCESS){
        fprintf (stderr, "[client:dsp]: halcs_get_kx failed\n");
        goto err_halcs_get;
    }
    fprintf (stdout, "[client:dsp]: halcs_get_kx = %u was successfully executed\n",
                    kx_get);

    uint32_t ky_set = 10000000;
    fprintf (stdout, "[client:dsp]: ky = %u\n", ky_set);
    err = halcs_set_ky (halcs_client, service_dsp, ky_set);
    if (err != HALCS_CLIENT_SUCCESS){
        fprintf (stderr, "[client:dsp]: halcs_set_ky failed\n");
        goto err_halcs_set;
    }
    fprintf (stdout, "[client:dsp]: halcs_set_ky was successfully executed\n");

    uint32_t ky_get;
    err = halcs_get_ky (halcs_client, service_dsp, &ky_get);
    if (err != HALCS_CLIENT_SUCCESS){
        fprintf (stderr, "[client:dsp]: halcs_get_ky failed\n");
        goto err_halcs_get;
    }
    fprintf (stdout, "[client:dsp]: halcs_get_ky = %u was successfully executed\n",
                    ky_get);

    uint32_t ksum_set = 0x7FFFFF;  // 1.0 FIX25_24;
    fprintf (stdout, "[client:dsp]: ksum = %u\n", ksum_set);
    err = halcs_set_ksum (halcs_client, service_dsp, ksum_set);
    if (err != HALCS_CLIENT_SUCCESS){
        fprintf (stderr, "[client:dsp]: halcs_set_ksum failed\n");
        goto err_halcs_set;
    }
    fprintf (stdout, "[client:dsp]: halcs_set_ksum was successfully executed\n");

    uint32_t ksum_get;
    err = halcs_get_ksum (halcs_client, service_dsp, &ksum_get);
    if (err != HALCS_CLIENT_SUCCESS){
        fprintf (stderr, "[client:dsp]: halcs_get_ksum failed\n");
        goto err_halcs_get;
    }
    fprintf (stdout, "[client:dsp]: halcs_get_ksum = %u was successfully executed\n",
                    ksum_get);

    uint32_t sw = 0;
    if (sw_str != NULL) {
        sw = strtoul (sw_str, NULL, 10);

        fprintf (stdout, "[client:swap]: sw = %u\n", sw);
        err = halcs_set_sw (halcs_client, service_swap, sw);
        if (err != HALCS_CLIENT_SUCCESS){
            fprintf (stderr, "[client:swap]: halcs_set_sw failed\n");
            goto err_halcs_exit;
        }
        fprintf (stdout, "[client:swap]: halcs_set_sw was successfully executed\n");
    }

    uint32_t div_clk = 0;
    if (div_clk_str != NULL) {
        div_clk = strtoul (div_clk_str, NULL, 10);

        fprintf (stdout, "[client:swap]: div_clk = %u\n", div_clk);
        err = halcs_set_div_clk (halcs_client, service_swap, div_clk);
        if (err != HALCS_CLIENT_SUCCESS){
            fprintf (stderr, "[client:swap]: halcs_set_div_clk failed\n");
            goto err_halcs_exit;
        }
        fprintf (stdout, "[client:swap]: halcs_set_div_clk was successfully executed\n");
    }

    uint32_t sw_dly = 0;
    if (sw_dly_str != NULL) {
        sw_dly = strtoul (sw_dly_str, NULL, 10);

        fprintf (stdout, "[client:swap]: sw_dly = %u\n", sw_dly);
        err = halcs_set_sw_dly (halcs_client, service_swap, sw_dly);
        if (err != HALCS_CLIENT_SUCCESS){
            fprintf (stderr, "[client:swap]: halcs_set_sw_dly failed\n");
            goto err_halcs_exit;
        }
        fprintf (stdout, "[client:swap]: halcs_set_sw_dly was successfully executed\n");
    }

err_halcs_client_new:
err_halcs_exit:
err_halcs_get:
err_halcs_set:
    halcs_client_destroy (&halcs_client);
    str_p = &div_clk_str;
    free (*str_p);
    div_clk_str = NULL;
    str_p = &sw_dly_str;
    free (*str_p);
    sw_dly_str = NULL;
    str_p = &sw_str;
    free (*str_p);
    sw_str = NULL;
    str_p = &broker_endp;
    free (*str_p);
    broker_endp = NULL;
    str_p = &board_number_str;
    free (*str_p);
    board_number_str = NULL;
    str_p = &halcs_number_str;
    free (*str_p);
    halcs_number_str = NULL;

    return 0;
}
