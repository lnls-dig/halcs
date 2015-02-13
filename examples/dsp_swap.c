/*
 *  * Simple example demonstrating the communication between
 *   * a client and the FPGA device
 *    */

#include <mdp.h>
#include <czmq.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <bpm_client.h>

#define DFLT_BIND_FOLDER            "/tmp/bpm"

#define DFLT_BPM_NUMBER             0
#define MAX_BPM_NUMBER              1

#define DFLT_BOARD_NUMBER           0
#define MAX_BOARD_NUMBER            5

void print_help (char *program_name)
{
    printf( "Usage: %s [options]\n"
            "\t-h This help message\n"
            "\t-v Verbose output\n"
            "\t-board <AMC board = [0|1|2|3|4|5]>\n"
            "\t-bpm <BPM number = [0|1]>\n"
            "\t-b <broker_endpoint> Broker endpoint\n"
            , program_name);
}

int main (int argc, char *argv [])
{
    int verbose = 0;
    char *broker_endp = NULL;
    char *board_number_str = NULL;
    char *bpm_number_str = NULL;
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
        else if (streq(argv[i], "-bpm"))
        {
            str_p = &bpm_number_str;
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

        if (board_number > MAX_BOARD_NUMBER) {
            fprintf (stderr, "[client:dsp]: BOARD number too big! Defaulting to: %u\n",
                    MAX_BOARD_NUMBER);
            board_number = MAX_BOARD_NUMBER;
        }
    }

    /* Set default bpm number */
    uint32_t bpm_number;
    if (bpm_number_str == NULL) {
        fprintf (stderr, "[client:dsp]: Setting default value to BPM number: %u\n",
                DFLT_BPM_NUMBER);
        bpm_number = DFLT_BPM_NUMBER;
    }
    else {
        bpm_number = strtoul (bpm_number_str, NULL, 10);

        if (bpm_number > MAX_BPM_NUMBER) {
            fprintf (stderr, "[client:dsp]: BPM number too big! Defaulting to: %u\n",
                    MAX_BPM_NUMBER);
            bpm_number = MAX_BPM_NUMBER;
        }
    }

    /* Generate the service names for each SMIO */
    char service_dsp[50];
    sprintf (service_dsp, "BPM%u:DEVIO:DSP%u", board_number, bpm_number);
    char service_swap[50];
    sprintf (service_swap, "BPM%u:DEVIO:SWAP%u", board_number, bpm_number);

    bpm_client_t *bpm_client = bpm_client_new (broker_endp, verbose, NULL);

    uint32_t kx_set = 10000000;
    fprintf (stdout, "[client:dsp]: kx = %u\n", kx_set);
    bpm_client_err_e err = bpm_set_kx (bpm_client, service_dsp, kx_set);
    if (err != BPM_CLIENT_SUCCESS){
        fprintf (stderr, "[client:dsp]: bpm_set_kx failed\n");
        goto err_bpm_set;
    }
    fprintf (stdout, "[client:dsp]: bpm_set_kx was successfully executed\n");

    uint32_t ds_tbt_thres = 200000;
    fprintf (stdout, "[client:dsp]: ds_tbt_thres = %u\n", ds_tbt_thres);
    err = bpm_set_ds_tbt_thres (bpm_client, service_dsp, ds_tbt_thres);
    if (err != BPM_CLIENT_SUCCESS){
        fprintf (stderr, "[client:dsp]: bpm_set_tbt_ds_thres failed\n");
        goto err_bpm_set;
    }
    fprintf (stdout, "[client:dsp]: bpm_set_ds_tbt_thres was successfully executed\n");

    uint32_t kx_get;
    err = bpm_get_kx (bpm_client, service_dsp, &kx_get);
    if (err != BPM_CLIENT_SUCCESS){
        fprintf (stderr, "[client:dsp]: bpm_get_kx failed\n");
        goto err_bpm_get;
    }
    fprintf (stdout, "[client:dsp]: bpm_get_kx = %u was successfully executed\n",
                    kx_get);

    uint32_t ky_set = 10000000;
    fprintf (stdout, "[client:dsp]: ky = %u\n", ky_set);
    err = bpm_set_ky (bpm_client, service_dsp, ky_set);
    if (err != BPM_CLIENT_SUCCESS){
        fprintf (stderr, "[client:dsp]: bpm_set_ky failed\n");
        goto err_bpm_set;
    }
    fprintf (stdout, "[client:dsp]: bpm_set_ky was successfully executed\n");

    uint32_t ky_get;
    err = bpm_get_ky (bpm_client, service_dsp, &ky_get);
    if (err != BPM_CLIENT_SUCCESS){
        fprintf (stderr, "[client:dsp]: bpm_get_ky failed\n");
        goto err_bpm_get;
    }
    fprintf (stdout, "[client:dsp]: bpm_get_ky = %u was successfully executed\n",
                    ky_get);

    uint32_t ksum_set = 0x7FFFFF;  // 1.0 FIX25_24;
    fprintf (stdout, "[client:dsp]: ksum = %u\n", ksum_set);
    err = bpm_set_ksum (bpm_client, service_dsp, ksum_set);
    if (err != BPM_CLIENT_SUCCESS){
        fprintf (stderr, "[client:dsp]: bpm_set_ksum failed\n");
        goto err_bpm_set;
    }
    fprintf (stdout, "[client:dsp]: bpm_set_ksum was successfully executed\n");

    uint32_t ksum_get;
    err = bpm_get_ksum (bpm_client, service_dsp, &ksum_get);
    if (err != BPM_CLIENT_SUCCESS){
        fprintf (stderr, "[client:dsp]: bpm_get_ksum failed\n");
        goto err_bpm_get;
    }
    fprintf (stdout, "[client:dsp]: bpm_get_ksum = %u was successfully executed\n",
                    ksum_get);

    uint32_t sw = 0;
    err = bpm_set_sw (bpm_client, service_swap, sw);
    if (err != BPM_CLIENT_SUCCESS){
        fprintf (stderr, "[client:swap]: bpm_set_sw failed\n");
        goto err_bpm_set;
    }
    fprintf (stdout, "[client:swap]: bpm_set_sw = %u was successfully executed\n",
                    sw);

    uint32_t sw_en = 0;
    err = bpm_set_sw_en (bpm_client, service_swap, sw_en);
    if (err != BPM_CLIENT_SUCCESS){
        fprintf (stderr, "[client:swap]: bpm_set_sw_en failed\n");
        goto err_bpm_set;
    }
    fprintf (stdout, "[client:swap]: bpm_set_sw_en = %u was successfully executed\n",
                    sw_en);

    uint32_t div_clk = 250;
    err = bpm_set_div_clk (bpm_client, service_swap, div_clk);
    if (err != BPM_CLIENT_SUCCESS){
        fprintf (stderr, "[client:swap]: bpm_set_div_clk failed\n");
        goto err_bpm_set;
    }
    fprintf (stdout, "[client:swap]: bpm_set_div_clk = %u was successfully executed\n",
                    div_clk);

    uint32_t gain_aa = 32768;
    uint32_t gain_ac = 32768;
    err = bpm_set_gain_a (bpm_client, service_swap, gain_aa, gain_ac);
    if (err != BPM_CLIENT_SUCCESS){
        fprintf (stderr, "[client:swap]: bpm_set_gain_a failed\n");
        goto err_bpm_set;
    }
    fprintf (stdout, "[client:swap]: bpm_set_gain_a = direct %u, inverted %u was successfully executed\n",
            gain_aa, gain_ac);

    err = bpm_set_gain_b (bpm_client, service_swap, gain_aa, gain_ac);
    if (err != BPM_CLIENT_SUCCESS){
        fprintf (stderr, "[client:swap]: bpm_set_gain_b failed\n");
        goto err_bpm_set;
    }
    fprintf (stdout, "[client:swap]: bpm_set_gain_b = direct %u, inverted %u was successfully executed\n",
            gain_aa, gain_ac);

    err = bpm_set_gain_c (bpm_client, service_swap, gain_aa, gain_ac);
    if (err != BPM_CLIENT_SUCCESS){
        fprintf (stderr, "[client:swap]: bpm_set_gain_c failed\n");
        goto err_bpm_set;
    }
    fprintf (stdout, "[client:swap]: bpm_set_gain_c = direct %u, inverted %u was successfully executed\n",
            gain_aa, gain_ac);

    err = bpm_set_gain_d (bpm_client, service_swap, gain_aa, gain_ac);
    if (err != BPM_CLIENT_SUCCESS){
        fprintf (stderr, "[client:swap]: bpm_set_gain_d failed\n");
        goto err_bpm_set;
    }
    fprintf (stdout, "[client:swap]: bpm_set_gain_d = direct %u, inverted %u was successfully executed\n",
            gain_aa, gain_ac);

err_bpm_get:
err_bpm_set:
    bpm_client_destroy (&bpm_client);
    str_p = &broker_endp;
    free (*str_p);
    broker_endp = NULL;
    str_p = &board_number_str;
    free (*str_p);
    board_number_str = NULL;
    str_p = &bpm_number_str;
    free (*str_p);
    bpm_number_str = NULL;

    return 0;
}
