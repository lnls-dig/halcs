/*
 *  * Simple example demonstrating the communication between
 *   * a client and the FPGA device
 *    */

#include <getopt.h>
#include <czmq.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <acq_client.h>

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
#define DFLT_NEWACQ_NUM             1

typedef enum {
    TEXT = 0,
    BINARY,
    END_FILE_FMT
} file_fmt_e;

void print_sample (uint8_t *data, uint32_t atom_size)
{
    switch (atom_size) {
        case 2:
            printf ("%8d\t", *(int16_t *) data);
        break;

        case 4:
            printf ("%8d\t", *(int32_t *) data);
        break;

        case 8:
            printf ("%8" PRId64 "\t", *(int64_t *) data);
        break;

        default:
            printf ("%8d\t", *(int16_t *) data);
        break;
    }
}

void print_data (uint32_t *data, uint32_t sample_size,
    uint32_t atom_size, uint32_t num_atoms, uint32_t size, file_fmt_e file_fmt)
{
    uint8_t *raw_data = (uint8_t *) data;

    if (file_fmt == TEXT) {
        for (uint32_t i = 0; i < size/sample_size; i++) {
            if (zctx_interrupted) {
                break;
            }

            for (uint32_t atom = 0; atom < num_atoms; ++atom) {
                print_sample(raw_data + (atom_size)*((i*num_atoms)+atom),
                    atom_size);
            }
            printf ("\n");
        }
    }
    else if (file_fmt == BINARY) {
        fwrite (raw_data, atom_size, size/atom_size, stdout);
    }
}

static struct option long_options[] =
{
    {"help",                no_argument,         NULL, 'h'},
    {"brokerendp",          required_argument,   NULL, 'b'},
    {"verbose",             no_argument,         NULL, 'v'},
    {"halcsnumber",         required_argument,   NULL, 's'},
    {"boardslot",           required_argument,   NULL, 'o'},
    {"channumber",          required_argument,   NULL, 'c'},
    {"numsamples",          required_argument,   NULL, 'n'},
    {"filefmt",             required_argument,   NULL, 'f'},
    {"newacq",              required_argument,   NULL, 'a'},
    {NULL, 0, NULL, 0}
};

static const char* shortopt = "hb:vo:s:c:n:f:a:";

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
            "                                     <Channel> is dependant on the FPGA firmware\n"
            "  -n  --numsamples <Number of samples> Number of samples\n"
            "  -f  --filefmt <Output format = [0 = text | 1=binary]>\n"
            "                                       Output format\n"
            "  -a  --newcq <Trigger new acquisition = [0 = no | 1 = yes]\n",
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
    char *new_acq_str = NULL;
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

            case 'a':
                new_acq_str = strdup (optarg);
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

    /* Set new_acq */
    uint32_t new_acq;
    if (new_acq_str == NULL) {
        new_acq = DFLT_NEWACQ_NUM;
    }
    else {
        new_acq = strtoul (new_acq_str, NULL, 10);

        if (new_acq != 0 && new_acq != 1) {
            fprintf (stderr, "[client:acq]: Newacq parameter is invalid\n");
            exit (1);
        }
    }

    char service[50];
    snprintf (service, sizeof (service), "HALCS%u:DEVIO:ACQ%u", board_number, halcs_number);

    acq_client_t *acq_client = acq_client_new (broker_endp, verbose, NULL);
    if (acq_client == NULL) {
        fprintf (stderr, "[client:acq]: acq_client could not be created\n");
        goto err_acq_client_new;
    }

    acq_set_fsm_stop (acq_client, service, 1);

    /* Set trigger to skip */
    uint32_t acq_trig = 0;
    halcs_client_err_e err = acq_set_trig (acq_client, service, acq_trig);
    if (err != HALCS_CLIENT_SUCCESS){
        fprintf (stderr, "[client:acq]: acq_set_trig failed\n");
        goto err_acq_set_trig;
    }

    uint32_t req_ch_sample_size = 0;
    uint32_t req_ch_atom_width = 0;
    uint32_t req_ch_atom_size = 0;
    uint32_t req_ch_num_atoms = 0;
    err = halcs_get_acq_ch_sample_size (acq_client, service, chan, &req_ch_sample_size);
    err |= halcs_get_acq_ch_atom_width (acq_client, service, chan, &req_ch_atom_width);
    err |= halcs_get_acq_ch_num_atoms (acq_client, service, chan, &req_ch_num_atoms);
    if (err != HALCS_CLIENT_SUCCESS){
        fprintf (stderr, "[client:acq]: getting channel properties failed\n");
        goto err_acq_get_prop;
    }

    /* bit to byte */
    req_ch_atom_size = req_ch_atom_width/8;

    uint32_t data_size = num_samples*req_ch_sample_size;
    uint32_t *data = (uint32_t *) zmalloc (data_size*sizeof (uint8_t));
    bool new_acq_arg = new_acq;
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
    err = acq_full_compat (acq_client, service, &acq_trans, 50000, new_acq_arg);
    if (err != HALCS_CLIENT_SUCCESS){
        fprintf (stderr, "[client:acq]: acq_full_compat failed\n");
        goto err_acq_full_compat;
    }

    if (!freopen (NULL, "wb", stdout)) {
        fprintf (stderr, "[client:acq]: Could not set output mode to binary\n");
        goto err_set_file_mode;
    }
    print_data (data, req_ch_sample_size, req_ch_atom_size,
        req_ch_num_atoms, acq_trans.block.bytes_read, file_fmt);

err_set_file_mode:
err_acq_full_compat:
err_acq_get_prop:
err_acq_set_trig:
err_acq_client_new:
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
    acq_client_destroy (&acq_client);

    return 0;
}
