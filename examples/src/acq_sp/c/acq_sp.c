/*
 *  * Simple example demonstrating the communication between
 *   * a client and the FPGA device
 *    */

#include <czmq.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <bpm_client.h>

#define DFLT_BIND_FOLDER            "/tmp/halcs"

#define DFLT_NUM_SAMPLES            4096
#define DFLT_CHAN_NUM               0

#define DFLT_HALCS_NUMBER           0

#define DFLT_BOARD_NUMBER           0

#define MIN_NUM_SAMPLES             4
/* Arbitrary hard limits */
#define MAX_NUM_SAMPLES             (1 << 28)
#define MAX_NUM_CHANS               (1 << 8)

void print_data (uint32_t chan, uint32_t *data, uint32_t size)
{
    /* FIXME: Make it more generic */
    if (chan == 0 || chan == 1 /* Only ADC and ADC SWAP */ ) {
        int16_t *raw_data16 = (int16_t *) data;
        for (uint32_t i = 0; i < (size/sizeof(uint16_t)) / 4; i++) {
            if (zctx_interrupted) {
                break;
            }

            printf ("%6u\t %8d\t %8d\t %8d\t %8d\n", i,
                    raw_data16[(i*4)],
                    raw_data16[(i*4)+1],
                    raw_data16[(i*4)+2],
                    raw_data16[(i*4)+3]);
        }
    }
    else {
        int32_t *raw_data32 = (int32_t *) data;
        for (uint32_t i = 0; i < (size/sizeof(uint32_t)) / 4; i++) {
            if (zctx_interrupted) {
                break;
            }

            printf ("%6u\t %8d\t %8d\t %8d\t %8d\n", i,
                    raw_data32[(i*4)],
                    raw_data32[(i*4)+1],
                    raw_data32[(i*4)+2],
                    raw_data32[(i*4)+3]);
        }
    }
}

void print_help (char *program_name)
{
    printf( "Usage: %s [options]\n"
            "\t-h This help message\n"
            "\t-v Verbose output\n"
            "\t-b <broker_endpoint> Broker endpoint\n"
            "\t-s <num_samples_str> Number of samples\n"
            "\t-board <AMC board = [0|1|2|3|4|5]>\n"
            "\t-halcs <HALCS number = [0|1]>\n"
            "\t-ch <chan_str> Acquisition channel\n"
            , program_name);
}

int main (int argc, char *argv [])
{
    int verbose = 0;
    char *broker_endp = NULL;
    char *num_samples_str = NULL;
    char *board_number_str = NULL;
    char *halcs_number_str = NULL;
    char *chan_str = NULL;
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
        else if (streq (argv[i], "-b")) {
            str_p = &broker_endp;
        }
        else if (streq (argv[i], "-s")) { /* s: samples */
            str_p = &num_samples_str;
        }
        else if (streq (argv[i], "-ch")) { /* ch: channel */
            str_p = &chan_str;
        }
        else if (streq (argv[i], "-board")) { /* board_number: board number */
            str_p = &board_number_str;
        }
        else if (streq(argv[i], "-halcs"))
        {
            str_p = &halcs_number_str;
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

    /* Set default number samples */
    uint32_t num_samples;
    if (num_samples_str == NULL) {
        fprintf (stderr, "[client:acq_sp]: Setting default value to number of samples: %u\n",
                DFLT_NUM_SAMPLES);
        num_samples = DFLT_NUM_SAMPLES;
    }
    else {
        num_samples = strtoul (num_samples_str, NULL, 10);

        if (num_samples < MIN_NUM_SAMPLES) {
            fprintf (stderr, "[client:acq_sp]: Number of samples too small! Defaulting to: %u\n",
                    MIN_NUM_SAMPLES);
            num_samples = MIN_NUM_SAMPLES;
        }
        else if (num_samples > MAX_NUM_SAMPLES) {
            fprintf (stderr, "[client:acq_sp]: Number of samples too big! Defaulting to: %u\n",
                    MAX_NUM_SAMPLES);
            num_samples = MAX_NUM_SAMPLES;
        }
    }
    //fprintf (stdout, "[client:acq_sp]: num_samples = %u\n", num_samples);

    /* Set default channel */
    uint32_t chan;
    if (chan_str == NULL) {
        fprintf (stderr, "[client:acq_sp]: Setting default value to 'chan'\n");
        chan = DFLT_CHAN_NUM;
    }
    else {
        chan = strtoul (chan_str, NULL, 10);

        if (chan > END_CHAN_ID-1) {
            fprintf (stderr, "[client:acq_sp]: Channel number too big! Defaulting to: %u\n",
                    MAX_NUM_CHANS);
            chan = END_CHAN_ID-1;
        }
    }
    //fprintf (stdout, "[client:acq_sp]: chan = %u\n", chan);

    /* Set default board number */
    uint32_t board_number;
    if (board_number_str == NULL) {
        fprintf (stderr, "[client:acq_sp]: Setting default value to BOARD number: %u\n",
                DFLT_BOARD_NUMBER);
        board_number = DFLT_BOARD_NUMBER;
    }
    else {
        board_number = strtoul (board_number_str, NULL, 10);
    }

    /* Set default halcs number */
    uint32_t halcs_number;
    if (halcs_number_str == NULL) {
        fprintf (stderr, "[client:leds]: Setting default value to HALCS number: %u\n",
                DFLT_HALCS_NUMBER);
        halcs_number = DFLT_HALCS_NUMBER;
    }
    else {
        halcs_number = strtoul (halcs_number_str, NULL, 10);
    }

    char service[50];
    snprintf (service, sizeof (service), "HALCS%u:DEVIO:ACQ%u", board_number, halcs_number);

    bpm_parameters_t bpm_parameters = {.kx       = 10000000,
                                       .ky       = 10000000,
                                       .kq       = 10000000,
                                       .ksum     = 1,
                                       .offset_x = 0,
                                       .offset_y = 0,
                                       .offset_q = 0
                                       };
    bpm_single_pass_t *bpm_single_pass = bpm_single_pass_new (broker_endp, verbose, NULL,
        service, &bpm_parameters, num_samples, num_samples);
    if (bpm_single_pass == NULL) {
        fprintf (stderr, "[client:acq_sp]: bpm_single_pass could not be created\n");
        goto err_bpm_single_pass_new;
    }

    uint32_t hyst = 1; 
    uint32_t slope = 1; 
    uint32_t delay = 0; 
    bpm_single_pass_configure_trigger (bpm_single_pass, hyst, slope, delay);

    uint32_t thres = 500;
    uint32_t active_sample = 0;
    bpm_single_pass_configure_data_trigger (bpm_single_pass,
        thres, active_sample);

    while (1) {
        if (zsys_interrupted) {
            break;
        }
        fprintf (stdout, "[client:acq_sp_sp]: starting new acq_spuisition\n");
        halcs_client_err_e err = bpm_single_pass_start (bpm_single_pass);
        if (err) {
            fprintf (stderr, "[client:acq_sp]: bpm_single_pass_start error\n");
            sleep (1);
            continue;
        }

        fprintf (stdout, "[client:acq_sp]: checking for single pass event\n");
        while (bpm_single_pass_check (bpm_single_pass) != HALCS_CLIENT_SUCCESS)  {
            sleep (1);
        }

        if (zsys_interrupted) {
            break;
        }

        fprintf (stdout, "[client:acq_sp]: calculating single_pass sample\n");
        bpm_sample_t bpm_sample;
        err = bpm_single_pass_sample (bpm_single_pass, &bpm_sample);
        if (err) {
            fprintf (stderr, "[client:acq_sp]: bpm_single_pass_sample error\n");
            sleep (1);
            continue;
        }

        const acq_trans_t * acq_trans = bpm_single_pass_get_acq_transaction (bpm_single_pass);
        const int16_t *datap = (const int16_t *) acq_trans->block.data;

        uint32_t i;
        for (i = 0; i < num_samples+num_samples; ++i) {
            fprintf (stdout, "[client:acq_sp_sp]: %d %d %d %d\n",
            datap[4*i], datap[4*i+1], datap[4*i+2], datap[4*i+3]);
        }

        fprintf (stdout, "[client:acq_sp]: a,b,c,d,x,y,q,sum = (%f,%f,%f,%f,%f,%f,%f%f)\n",
            bpm_sample.a, bpm_sample.b, bpm_sample.c, bpm_sample.d, 
            bpm_sample.x, bpm_sample.y, bpm_sample.q, bpm_sample.sum);

    }

err_bpm_single_pass_new:
    str_p = &chan_str;
    free (*str_p);
    chan_str = NULL;
    str_p = &board_number_str;
    free (*str_p);
    board_number_str = NULL;
    str_p = &halcs_number_str;
    free (*str_p);
    halcs_number_str = NULL;
    str_p = &num_samples_str;
    free (*str_p);
    num_samples_str = NULL;
    str_p = &broker_endp;
    free (*str_p);
    broker_endp = NULL;
    bpm_single_pass_destroy (&bpm_single_pass);

    return 0;
}
