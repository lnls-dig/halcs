/*
 *  * Simple example demonstrating the communication between
 *   * a client and the FPGA device
 *    */

#include <czmq.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <acq_client.h>

#define DFLT_BIND_FOLDER            "/tmp/halcs"

#define DFLT_NUM_SAMPLES            4096
#define DFLT_CHAN_NUM               0

#define DFLT_HALCS_NUMBER             0
#define MAX_HALCS_NUMBER              1

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
        fprintf (stderr, "[client:acq]: Setting default value to number of samples: %u\n",
                DFLT_NUM_SAMPLES);
        num_samples = DFLT_NUM_SAMPLES;
    }
    else {
        num_samples = strtoul (num_samples_str, NULL, 10);

        if (num_samples < MIN_NUM_SAMPLES) {
            fprintf (stderr, "[client:acq]: Number of samples too small! Defaulting to: %u\n",
                    MIN_NUM_SAMPLES);
            num_samples = MIN_NUM_SAMPLES;
        }
        else if (num_samples > MAX_NUM_SAMPLES) {
            fprintf (stderr, "[client:acq]: Number of samples too big! Defaulting to: %u\n",
                    MAX_NUM_SAMPLES);
            num_samples = MAX_NUM_SAMPLES;
        }
    }
    //fprintf (stdout, "[client:acq]: num_samples = %u\n", num_samples);

    /* Set default channel */
    uint32_t chan;
    if (chan_str == NULL) {
        fprintf (stderr, "[client:acq]: Setting default value to 'chan'\n");
        chan = DFLT_CHAN_NUM;
    }
    else {
        chan = strtoul (chan_str, NULL, 10);
    }
    //fprintf (stdout, "[client:acq]: chan = %u\n", chan);

    /* Set default board number */
    uint32_t board_number;
    if (board_number_str == NULL) {
        fprintf (stderr, "[client:acq]: Setting default value to BOARD number: %u\n",
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

        if (halcs_number > MAX_HALCS_NUMBER) {
            fprintf (stderr, "[client:leds]: HALCS number too big! Defaulting to: %u\n",
                    MAX_HALCS_NUMBER);
            halcs_number = MAX_HALCS_NUMBER;
        }
    }

    char service[50];
    snprintf (service, sizeof (service), "HALCS%u:DEVIO:ACQ%u", board_number, halcs_number);

    acq_client_t *acq_client = acq_client_new (broker_endp, verbose, NULL);
    if (acq_client == NULL) {
        fprintf (stderr, "[client:acq]: acq_client could not be created\n");
        goto err_acq_client_new;
    }

    halcs_client_err_e err = HALCS_CLIENT_SUCCESS;
    /* Change trigger type here if needed */
    uint32_t acq_trig = 2;
    err = acq_set_trig (acq_client, service, acq_trig);
    if (err != HALCS_CLIENT_SUCCESS){
        fprintf (stderr, "[client:acq]: acq_set_trig failed\n");
        goto err_acq_set_trig;
    }

    uint32_t data_trig_thres = 200;
    err = acq_set_data_trig_thres (acq_client, service, data_trig_thres);
    if (err != HALCS_CLIENT_SUCCESS){
        fprintf (stderr, "[client:acq]: acq_set_data_trig_thres failed\n");
        goto err_acq_set_trig;
    }

    uint32_t data_trig_pol = 0;
    err = acq_set_data_trig_pol (acq_client, service, data_trig_pol);
    if (err != HALCS_CLIENT_SUCCESS){
        fprintf (stderr, "[client:acq]: acq_set_data_trig_pol failed\n");
        goto err_acq_set_trig;
    }

    uint32_t data_trig_sel = 0;
    err = acq_set_data_trig_sel (acq_client, service, data_trig_sel);
    if (err != HALCS_CLIENT_SUCCESS){
        fprintf (stderr, "[client:acq]: acq_set_data_trig_sel failed\n");
        goto err_acq_set_trig;
    }

    uint32_t data_trig_filt = 1;
    err = acq_set_data_trig_filt (acq_client, service, data_trig_filt);
    if (err != HALCS_CLIENT_SUCCESS){
        fprintf (stderr, "[client:acq]: acq_set_data_trig_filt failed\n");
        goto err_acq_set_trig;
    }

    uint32_t hw_trig_dly = 0;
    err =  acq_set_hw_trig_dly (acq_client, service, hw_trig_dly);
    if (err != HALCS_CLIENT_SUCCESS){
        fprintf (stderr, "[client:acq]: acq_set_hw_trig_dly failed\n");
        goto err_acq_set_trig;
    }

    uint32_t data_trig_chan = 6;
    err =  acq_set_data_trig_chan (acq_client, service, data_trig_chan);
    if (err != HALCS_CLIENT_SUCCESS){
        fprintf (stderr, "[client:acq]: acq_set_data_trig_chan failed\n");
        goto err_acq_set_trig;
    }

    uint32_t req_ch_sample_size = 0;
    err = halcs_get_acq_ch_sample_size (acq_client, service, chan, &req_ch_sample_size);
    if (err != HALCS_CLIENT_SUCCESS){
        fprintf (stderr, "[client:acq]: getting channel properties failed\n");
        goto err_acq_get_prop;
    }

    uint32_t num_samples_pre = num_samples;
    uint32_t num_samples_post = num_samples;
    uint32_t num_shots = 1;
    uint32_t data_size = (num_samples_pre+num_samples_post+100)*num_shots*
        req_ch_sample_size;
    uint32_t *data = (uint32_t *) zmalloc (data_size*sizeof (uint8_t));
    bool new_acq = true;
    acq_trans_t acq_trans = {.req =   {
                                        .num_samples_pre = num_samples_pre,
                                        .num_samples_post = num_samples_post,
                                        .num_shots = num_shots,
                                        .chan = chan,
                                      },
                             .block = {
                                        .data = data,
                                        .data_size = data_size,
                                      }
                            };
    err = acq_full_compat (acq_client, service, &acq_trans, 500000, new_acq);
    if (err != HALCS_CLIENT_SUCCESS){
        fprintf (stderr, "[client:acq]: acq_full_compat failed\n");
        goto err_acq_full_compat;
    }

    //fprintf (stdout, "[client:acq]: acq_full_compat was successfully executed\n");
    fprintf (stdout, "clear\n");
    print_data (chan, data, acq_trans.block.bytes_read);

err_acq_full_compat:
err_acq_get_prop:
err_acq_set_trig:
err_acq_client_new:
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
    acq_client_destroy (&acq_client);

    return 0;
}
