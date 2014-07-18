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

void print_help (char *program_name)
{
    printf( "Usage: %s [options]\n"
            "\t-h This help message\n"
            "\t-v Verbose output\n"
            "\t-b <broker_endpoint> Broker endpoint\n"
            , program_name);
}

int main (int argc, char *argv [])
{
    int verbose = 0;
    char *broker_endp = NULL;
    char **str_p = NULL;

    if (argc < 2) {
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

    bpm_client_t *bpm_client = bpm_client_new (broker_endp, verbose, NULL);

    uint32_t kx_set = 10000000;
    fprintf (stdout, "[client:dsp]: kx = %u\n", kx_set);
    bpm_client_err_e err = bpm_set_kx (bpm_client, "BPM0:DEVIO:DSP", kx_set);
    if (err != BPM_CLIENT_SUCCESS){
        fprintf (stderr, "[client:dsp]: bpm_set_kx failed\n");
        goto err_bpm_set;
    }
    fprintf (stdout, "[client:dsp]: bpm_set_kx was successfully executed\n");

    uint32_t ds_tbt_thres = 200000;
    fprintf (stdout, "[client:dsp]: ds_tbt_thres = %u\n", ds_tbt_thres);
    err = bpm_set_ds_tbt_thres (bpm_client, "BPM0:DEVIO:DSP", ds_tbt_thres);
    if (err != BPM_CLIENT_SUCCESS){
        fprintf (stderr, "[client:dsp]: bpm_set_tbt_ds_thres failed\n");
        goto err_bpm_set;
    }
    fprintf (stdout, "[client:dsp]: bpm_set_ds_tbt_thres was successfully executed\n");

    uint32_t kx_get;
    err = bpm_get_kx (bpm_client, "BPM0:DEVIO:DSP", &kx_get);
    if (err != BPM_CLIENT_SUCCESS){
        fprintf (stderr, "[client:dsp]: bpm_get_kx failed\n");
        goto err_bpm_get;
    }
    fprintf (stdout, "[client:dsp]: bpm_get_kx = %u was successfully executed\n",
					kx_get);

    uint32_t ky_set = 10000000;
    fprintf (stdout, "[client:dsp]: ky = %u\n", ky_set);
    err = bpm_set_ky (bpm_client, "BPM0:DEVIO:DSP", ky_set);
    if (err != BPM_CLIENT_SUCCESS){
        fprintf (stderr, "[client:dsp]: bpm_set_ky failed\n");
        goto err_bpm_set;
    }
    fprintf (stdout, "[client:dsp]: bpm_set_ky was successfully executed\n");

    uint32_t ky_get;
    err = bpm_get_ky (bpm_client, "BPM0:DEVIO:DSP", &ky_get);
    if (err != BPM_CLIENT_SUCCESS){
        fprintf (stderr, "[client:dsp]: bpm_get_ky failed\n");
        goto err_bpm_get;
    }
    fprintf (stdout, "[client:dsp]: bpm_get_ky = %u was successfully executed\n",
					ky_get);

    uint32_t ksum_set = 0x0FFFFFF;  // 1.0 FIX25_24;
    fprintf (stdout, "[client:dsp]: ksum = %u\n", ksum_set);
    err = bpm_set_ksum (bpm_client, "BPM0:DEVIO:DSP", ksum_set);
    if (err != BPM_CLIENT_SUCCESS){
        fprintf (stderr, "[client:dsp]: bpm_set_ksum failed\n");
        goto err_bpm_set;
    }
    fprintf (stdout, "[client:dsp]: bpm_set_ksum was successfully executed\n");

    uint32_t ksum_get;
    err = bpm_get_ksum (bpm_client, "BPM0:DEVIO:DSP", &ksum_get);
    if (err != BPM_CLIENT_SUCCESS){
        fprintf (stderr, "[client:dsp]: bpm_get_ksum failed\n");
        goto err_bpm_get;
    }
    fprintf (stdout, "[client:dsp]: bpm_get_ksum = %u was successfully executed\n",
					ksum_get);

    uint32_t sw = 1;
    err = bpm_set_sw (bpm_client, "BPM0:DEVIO:SWAP", sw);
    if (err != BPM_CLIENT_SUCCESS){
        fprintf (stderr, "[client:swap]: bpm_set_sw failed\n");
        goto err_bpm_set;
    }
    fprintf (stdout, "[client:swap]: bpm_set_sw = %u was successfully executed\n",
					sw);

    uint32_t sw_en = 0;
    err = bpm_set_sw_en (bpm_client, "BPM0:DEVIO:SWAP", sw_en);
    if (err != BPM_CLIENT_SUCCESS){
        fprintf (stderr, "[client:swap]: bpm_set_sw_en failed\n");
        goto err_bpm_set;
    }
    fprintf (stdout, "[client:swap]: bpm_set_sw_en = %u was successfully executed\n",
					sw_en);

    uint32_t div_clk = 250;
    err = bpm_set_div_clk (bpm_client, "BPM0:DEVIO:SWAP", div_clk);
    if (err != BPM_CLIENT_SUCCESS){
        fprintf (stderr, "[client:swap]: bpm_set_div_clk failed\n");
        goto err_bpm_set;
    }
    fprintf (stdout, "[client:swap]: bpm_set_div_clk = %u was successfully executed\n",
					div_clk);

    uint32_t gain_aa = 32768;
    uint32_t gain_ac = 32768;
    err = bpm_set_gain_a (bpm_client, "BPM0:DEVIO:SWAP", gain_aa, gain_ac);
    if (err != BPM_CLIENT_SUCCESS){
        fprintf (stderr, "[client:swap]: bpm_set_gain_a failed\n");
        goto err_bpm_set;
    }
    fprintf (stdout, "[client:swap]: bpm_set_gain_a = direct %u, inverted %u was successfully executed\n",
            gain_aa, gain_ac);

    err = bpm_set_gain_b (bpm_client, "BPM0:DEVIO:SWAP", gain_aa, gain_ac);
    if (err != BPM_CLIENT_SUCCESS){
        fprintf (stderr, "[client:swap]: bpm_set_gain_b failed\n");
        goto err_bpm_set;
    }
    fprintf (stdout, "[client:swap]: bpm_set_gain_b = direct %u, inverted %u was successfully executed\n",
            gain_aa, gain_ac);

    err = bpm_set_gain_c (bpm_client, "BPM0:DEVIO:SWAP", gain_aa, gain_ac);
    if (err != BPM_CLIENT_SUCCESS){
        fprintf (stderr, "[client:swap]: bpm_set_gain_c failed\n");
        goto err_bpm_set;
    }
    fprintf (stdout, "[client:swap]: bpm_set_gain_c = direct %u, inverted %u was successfully executed\n",
            gain_aa, gain_ac);

    err = bpm_set_gain_d (bpm_client, "BPM0:DEVIO:SWAP", gain_aa, gain_ac);
    if (err != BPM_CLIENT_SUCCESS){
        fprintf (stderr, "[client:swap]: bpm_set_gain_d failed\n");
        goto err_bpm_set;
    }
    fprintf (stdout, "[client:swap]: bpm_set_gain_d = direct %u, inverted %u was successfully executed\n",
            gain_aa, gain_ac);

err_bpm_get:
err_bpm_set:
    str_p = &broker_endp;
    free (*str_p);
    broker_endp = NULL;
    bpm_client_destroy (&bpm_client);

    return 0;
}
