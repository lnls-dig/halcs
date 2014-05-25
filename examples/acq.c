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
#define DFLT_BIND_ADDR              "0"

#define DEFAULT_NUM_SAMPLES         4096

typedef struct _acq_chan_t {
    uint32_t chan;
    uint32_t sample_size;
} acq_chan_t;

/* Global structure merging all of the channel's sample sizes */
acq_chan_t __acq_chan[END_CHAN_ID] = { [0] = {.chan = ADC_CHAN_ID, .sample_size = ADC_SAMPLE_SIZE},
                                     [1] = {.chan = TBTAMP_CHAN_ID, .sample_size = TBTAMP_SAMPLE_SIZE},
                                     [2] = {.chan = TBTPOS_CHAN_ID, .sample_size = TBTPOS_SAMPLE_SIZE},
                                     [3] = {.chan = FOFBAMP_CHAN_ID, .sample_size = FOFBAMP_SAMPLE_SIZE},
                                     [4] = {.chan = FOFBPOS_CHAN_ID, .sample_size = FOFBPOS_SAMPLE_SIZE} };

void print_data (uint32_t chan, uint32_t *data, uint32_t size)
{
    /* FIXME: Make it more generic */
    if (chan == 0) {
        int16_t *raw_data16 = (int16_t *) data;
        for (uint32_t i = 0; i < (size/sizeof(uint16_t)) / 4; i++) {
            if (!zctx_interrupted) {
                printf ("%6u\t %8d\t %8d\t %8d\t %8d\n", i,
                        raw_data16[(i*4)],
                        raw_data16[(i*4)+1],
                        raw_data16[(i*4)+2],
                        raw_data16[(i*4)+3]);
            }
        }
    }
    else {
        int32_t *raw_data32 = (int32_t *) data;
        for (uint32_t i = 0; i < (size/sizeof(uint32_t)) / 4; i++) {
            if (!zctx_interrupted) {
                printf ("%6u\t %8d\t %8d\t %8d\t %8d\n", i,
                        raw_data32[(i*4)],
                        raw_data32[(i*4)+1],
                        raw_data32[(i*4)+2],
                        raw_data32[(i*4)+3]);
            }
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
            "\t-ch <chan_str> Acquisition channel\n"
            , program_name);
}

int main (int argc, char *argv [])
{
    int verbose = 0;
    char *broker_endp = NULL;
    char *num_samples_str = NULL;
    char *chan_str = NULL;
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
        else if (streq (argv[i], "-s")) { /* s: samples */
            str_p = &num_samples_str;
        }
        else if (streq (argv[i], "-ch")) { /* ch: channel */
            str_p = &chan_str;
        }
        /* Fallout for options with parameters */
        else {
            *str_p = strdup (argv[i]);
        }
    }

    /* Set default broker address */
    if (broker_endp == NULL) {
        broker_endp = strdup ("ipc://"DFLT_BIND_FOLDER"/"DFLT_BIND_ADDR);
    }

    bpm_client_t *bpm_client = bpm_client_new (broker_endp, verbose);

    /* Set default number samples */
    uint32_t num_samples;
    if (num_samples_str == NULL) {
        fprintf (stderr, "[client:acq]: Set default value to 'num_samples'\n");
        num_samples = DEFAULT_NUM_SAMPLES;
    }
    else {
        num_samples = (uint32_t) atoi(num_samples_str);
    }
    fprintf (stdout, "[client:acq]: num_samples = %u\n", num_samples);

    /* Set default channel */
    uint32_t chan;
    if (chan_str == NULL) {
        fprintf (stderr, "[client:acq]: Set default value to 'chan'\n");
        chan = 0;
    }
    else {
        chan = (uint32_t) atoi(chan_str);
    }
    fprintf (stdout, "[client:acq]: chan = %u\n", chan);

    uint32_t data_size = num_samples*__acq_chan[chan].sample_size;
    uint32_t *data = (uint32_t *) zmalloc (data_size*sizeof (uint8_t));
    acq_trans_t acq_trans = {.req =   {
                                        .num_samples = num_samples,
                                        .chan = chan,
                                      },
                             .block = {
                                        .data = data,
                                        .data_size = data_size,
                                      }
                            };
    bpm_client_err_e err = bpm_get_curve (bpm_client, "BPM0:DEVIO:ACQ", &acq_trans);
    if (err != BPM_CLIENT_SUCCESS){
        fprintf (stderr, "[client:acq]: bpm_get_curve failed\n");
        goto err_bpm_get_curve;
    }

    fprintf (stdout, "[client:acq]: bpm_get_curve was successfully executed\n");
    print_data (chan, data, acq_trans.block.bytes_read);

err_bpm_get_curve:
    str_p = &chan_str;
    free (*str_p);
    chan_str = NULL;
    str_p = &num_samples_str;
    free (*str_p);
    num_samples_str = NULL;
    str_p = &broker_endp;
    free (*str_p);
    broker_endp = NULL;
    bpm_client_destroy (&bpm_client);

    return 0;
}
