/*
 *  * Simple example demonstrating the communication between
 *   * a client and the FPGA device
 *    */

#include <czmq.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <bpm_client.h>

#define DFLT_BIND_FOLDER            "/tmp/bpm"

#define DFLT_NUM_SAMPLES            4096
#define DFLT_CHAN_NUM               0

#define DFLT_BPM_NUMBER             0
#define MAX_BPM_NUMBER              1

#define DFLT_BOARD_NUMBER           0

#define MIN_NUM_SAMPLES             4
/* Arbitrary hard limits */
#define MAX_NUM_SAMPLES             (1 << 28)
#define MAX_NUM_CHANS               (1 << 8)

#define DFLT_FILE_FMT               0

typedef enum {
    TEXT = 0,
    BINARY,
    END_FILE_FMT
} file_fmt_e;

void print_data (uint32_t chan, uint32_t *data, uint32_t size, file_fmt_e file_fmt)
{
    /* FIXME: Make it more generic */
    if (chan == 0 || chan == 1 /* Only ADC and ADC SWAP */ ) {
        int16_t *raw_data16 = (int16_t *) data;
        if (file_fmt == TEXT) {
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
        else if (file_fmt == BINARY) {
            fwrite (raw_data16, 2, size/2, stdout);
        }
    }
    else {
        int32_t *raw_data32 = (int32_t *) data;
        if (file_fmt == TEXT) {
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
        else if (file_fmt == BINARY) {
            fwrite (raw_data32, 4, size/4, stdout);
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
            "\t-bpm <BPM number = [0|1]>\n"
            "\t-ch <chan_str> Acquisition channel\n"
            "\t-filefmt <File format = [0 = {text} |1 = {binary}]> File Format\n"
            , program_name);
}

int main (int argc, char *argv [])
{
    int verbose = 0;
    char *broker_endp = NULL;
    char *num_samples_str = NULL;
    char *board_number_str = NULL;
    char *bpm_number_str = NULL;
    char *chan_str = NULL;
    char *file_fmt_str = NULL;
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
        else if (streq(argv[i], "-bpm"))
        {
            str_p = &bpm_number_str;
        }
        else if (streq(argv[i], "-filefmt"))
        {
            str_p = &file_fmt_str;
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

        if (chan > END_CHAN_ID-1) {
            fprintf (stderr, "[client:acq]: Channel number too big! Defaulting to: %u\n",
                    MAX_NUM_CHANS);
            chan = END_CHAN_ID-1;
        }
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

    /* Set default bpm number */
    uint32_t bpm_number;
    if (bpm_number_str == NULL) {
        fprintf (stderr, "[client:leds]: Setting default value to BPM number: %u\n",
                DFLT_BPM_NUMBER);
        bpm_number = DFLT_BPM_NUMBER;
    }
    else {
        bpm_number = strtoul (bpm_number_str, NULL, 10);

        if (bpm_number > MAX_BPM_NUMBER) {
            fprintf (stderr, "[client:leds]: BPM number too big! Defaulting to: %u\n",
                    MAX_BPM_NUMBER);
            bpm_number = MAX_BPM_NUMBER;
        }
    }

    /* Set default file format */
    file_fmt_e file_fmt;
    if (file_fmt_str == NULL) {
        fprintf (stderr, "[client:acq]: Setting default value to 'file_fmt'\n");
        file_fmt = DFLT_FILE_FMT;
    }
    else {
        file_fmt = strtoul (file_fmt_str, NULL, 10);

        if (file_fmt > END_FILE_FMT-1) {
            fprintf (stderr, "[client:acq]: Invalid file format (-file_fmt) Defaulting to:: %u\n",
                    0);
            file_fmt = 0;
        }
    }

    char service[50];
    snprintf (service, sizeof (service), "BPM%u:DEVIO:ACQ%u", board_number, bpm_number);

    bpm_client_t *bpm_client = bpm_client_new (broker_endp, verbose, NULL);
    if (bpm_client == NULL) {
        fprintf (stderr, "[client:acq]: bpm_client could be created\n");
        goto err_bpm_client_new;
    }

    /* Set trigger to skip */
    uint32_t acq_trig = 0;
    bpm_client_err_e err = bpm_set_acq_trig (bpm_client, service, acq_trig);
    if (err != BPM_CLIENT_SUCCESS){
        fprintf (stderr, "[client:acq]: bpm_acq_set_trig failed\n");
        goto err_bpm_set_acq_trig;
    }

    uint32_t data_size = num_samples*acq_chan[chan].sample_size;
    uint32_t *data = (uint32_t *) zmalloc (data_size*sizeof (uint8_t));
    bool new_acq = true;
    acq_trans_t acq_trans = {.req =   {
                                        .num_samples_pre = num_samples,
                                        .num_samples_post = 0,
                                        .num_shots = 1,
                                        .chan = chan,
                                      },
                             .block = {
                                        .data = data,
                                        .data_size = data_size,
                                      }
                            };
    err = bpm_get_curve (bpm_client, service, &acq_trans,
            50000, new_acq);
    if (err != BPM_CLIENT_SUCCESS){
        fprintf (stderr, "[client:acq]: bpm_get_curve failed\n");
        goto err_bpm_get_curve;
    }

    //fprintf (stdout, "[client:acq]: bpm_get_curve was successfully executed\n");
    //fprintf (stdout, "clear\n");
    if (!freopen (NULL, "wb", stdout)) {
        fprintf (stderr, "[client:acq]: Could not set output mode to binary\n");
        goto err_set_file_mode;
    }
    print_data (chan, data, acq_trans.block.bytes_read, file_fmt);

err_set_file_mode:
err_bpm_get_curve:
err_bpm_set_acq_trig:
err_bpm_client_new:
    str_p = &file_fmt_str;
    free (*str_p);
    file_fmt_str = NULL;
    str_p = &chan_str;
    free (*str_p);
    chan_str = NULL;
    str_p = &board_number_str;
    free (*str_p);
    board_number_str = NULL;
    str_p = &bpm_number_str;
    free (*str_p);
    bpm_number_str = NULL;
    str_p = &num_samples_str;
    free (*str_p);
    num_samples_str = NULL;
    str_p = &broker_endp;
    free (*str_p);
    broker_endp = NULL;
    bpm_client_destroy (&bpm_client);

    return 0;
}
