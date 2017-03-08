/*
 *  * Simple example demonstrating the communication between
 *   * a client and the FPGA device
 *    */

#include <getopt.h>
#include <czmq.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <halcs_client.h>

#define DFLT_BIND_FOLDER            "/tmp/halcs"

#define DFLT_NUM_SAMPLES            4096
#define DFLT_CHAN_NUM               0

#define DFLT_HALCS_NUMBER           0

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

                printf ("%8d\t %8d\t %8d\t %8d\n",
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
    else if (chan == 2 || chan == 4 || chan == 9 /* I/Q channels */ ) {
        int32_t *raw_data32 = (int32_t *) data;
        if (file_fmt == TEXT) {
            for (uint32_t i = 0; i < (size/sizeof(uint32_t)) / 8; i++) {
                if (zctx_interrupted) {
                    break;
                }

                printf ("%8d\t %8d\t %8d\t %8d\t %8d\t %8d\t %8d\t %8d\n",
                        raw_data32[(i*8)],
                        raw_data32[(i*8)+1],
                        raw_data32[(i*8)+2],
                        raw_data32[(i*8)+3],
                        raw_data32[(i*8)+4],
                        raw_data32[(i*8)+5],
                        raw_data32[(i*8)+6],
                        raw_data32[(i*8)+7]);
            }
        }
        else if (file_fmt == BINARY) {
            fwrite (raw_data32, 8, size/8, stdout);
        }
    }
    else {
        int32_t *raw_data32 = (int32_t *) data;
        if (file_fmt == TEXT) {
            for (uint32_t i = 0; i < (size/sizeof(uint32_t)) / 4; i++) {
                if (zctx_interrupted) {
                    break;
                }

                printf ("%8d\t %8d\t %8d\t %8d\n",
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

static struct option long_options[] =
{
    {"help",                no_argument,         NULL, 'h'},
    {"brokerendp",          required_argument,   NULL, 'b'},
    {"verbose",             no_argument,         NULL, 'v'},
    {"halcsnumber",           required_argument,   NULL, 's'},
    {"boardslot",           required_argument,   NULL, 'o'},
    {"channumber",          required_argument,   NULL, 'c'},
    {"numsamples",          required_argument,   NULL, 'n'},
    {"filefmt",             required_argument,   NULL, 'f'},
    {NULL, 0, NULL, 0}
};

static const char* shortopt = "hb:vo:s:c:n:f:";

void print_help (char *program_name)
{
    fprintf (stdout, "HALCSD Acquisition Utility\n"
            "Usage: %s [options]\n"
            "\n"
            "  -h  --help                           Display this usage information\n"
            "  -b  --brokerendp <Broker endpoint>   Broker endpoint\n"
            "  -v  --verbose                        Verbose output\n"
            "  -o  --boardslot <Board slot number = [1-12]> \n"
            "  -s  --halcsnumber <HALCS number = [0|1]> HALCS number\n"
            "                                       Board slot number\n"
            "  -c  --channumber <Channel>           Channel number\n"
            "                                     <Channel> must be one of the following:\n"
            "                                     0 -> ADC; 1 -> ADC_SWAP; 2 -> Mixer IQ; 3 -> DUMMY0;\n"
            "                                     4 -> TBT Decim IQ; 5 -> DUMMY1; 6 -> TBT Amp;\n"
            "                                     7 -> TBT Phase; 8 -> TBT Pos; 9 -> FOFB Decim IQ;\n"
            "                                     10 -> DUMMY2; 11 -> FOFB Amp; 12 -> FOFB Pha;\n"
            "                                     13 -> FOFB Pos; 14 -> Monit Amp; 15 -> Monit Pha; 16 -> Monit Pos]\n"
            "  -n  --numsamples <Number of samples> Number of samples\n"
            "  -f  --filefmt <Output format = [0 = text | 1=binary]>\n"
            "                                       Output format\n",
            program_name);
}

int main (int argc, char *argv [])
{
    int verbose = 0;
    char *broker_endp = NULL;
    char *num_samples_str = NULL;
    char *board_number_str = NULL;
    char *halcs_number_str = NULL;
    char *chan_str = NULL;
    char *file_fmt_str = NULL;
    int opt;

    while ((opt = getopt_long (argc, argv, shortopt, long_options, NULL)) != -1) {
        /* Get the user selected options */
        switch (opt) {
            /* Display Help */
            case 'h':
                print_help (argv [0]);
                exit (1);
                break;

            case 'b':
                broker_endp = strdup (optarg);
                break;

            case 'v':
                verbose = 1;
                break;

            case 'o':
                board_number_str = strdup (optarg);
                break;

            case 's':
                halcs_number_str = strdup (optarg);
                break;

            case 'c':
                chan_str = strdup (optarg);
                break;

            case 'n':
                num_samples_str = strdup (optarg);
                break;

            case 'f':
                file_fmt_str = strdup (optarg);
                break;

            case '?':
                fprintf (stderr, "[client:acq] Option not recognized or missing argument\n");
                print_help (argv [0]);
                exit (1);
                break;

            default:
                fprintf (stderr, "[client:acq] Could not parse options\n");
                print_help (argv [0]);
                exit (1);
         }
    }

    /* Set default broker address */
    if (broker_endp == NULL) {
        fprintf (stderr, "[client:acq]: Setting default broker endpoint: %s\n",
                "ipc://"DFLT_BIND_FOLDER);
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
        fprintf (stderr, "[client:acq]: Setting default value to HALCS number: %u\n",
                DFLT_HALCS_NUMBER);
        halcs_number = DFLT_HALCS_NUMBER;
    }
    else {
        halcs_number = strtoul (halcs_number_str, NULL, 10);
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
    snprintf (service, sizeof (service), "HALCS%u:DEVIO:ACQ%u", board_number, halcs_number);

    halcs_client_t *halcs_client = halcs_client_new (broker_endp, verbose, NULL);
    if (halcs_client == NULL) {
        fprintf (stderr, "[client:acq]: halcs_client could be created\n");
        goto err_halcs_client_new;
    }

    /* Set trigger to skip */
    uint32_t acq_trig = 0;
    halcs_client_err_e err = halcs_set_acq_trig (halcs_client, service, acq_trig);
    if (err != HALCS_CLIENT_SUCCESS){
        fprintf (stderr, "[client:acq]: halcs_acq_set_trig failed\n");
        goto err_halcs_set_acq_trig;
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
    err = halcs_get_curve (halcs_client, service, &acq_trans,
            50000, new_acq);
    if (err != HALCS_CLIENT_SUCCESS){
        fprintf (stderr, "[client:acq]: halcs_get_curve failed\n");
        goto err_halcs_get_curve;
    }

    if (!freopen (NULL, "wb", stdout)) {
        fprintf (stderr, "[client:acq]: Could not set output mode to binary\n");
        goto err_set_file_mode;
    }
    print_data (chan, data, acq_trans.block.bytes_read, file_fmt);

err_set_file_mode:
err_halcs_get_curve:
err_halcs_set_acq_trig:
err_halcs_client_new:
    free (file_fmt_str);
    file_fmt_str = NULL;
    free (chan_str);
    chan_str = NULL;
    free (board_number_str);
    board_number_str = NULL;
    free (halcs_number_str);
    halcs_number_str = NULL;
    free (num_samples_str);
    num_samples_str = NULL;
    free (broker_endp);
    broker_endp = NULL;
    halcs_client_destroy (&halcs_client);

    return 0;
}
