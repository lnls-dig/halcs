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

static struct option long_options[] =
{
    {"help",                no_argument,         NULL, 'h'},
    {"brokerendp",          required_argument,   NULL, 'b'},
    {"verbose",             no_argument,         NULL, 'v'},
    {"halcsnumber",           required_argument,   NULL, 's'},
    {"boardslot",           required_argument,   NULL, 'o'},
    {"channumber",          required_argument,   NULL, 'c'},
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
            "                                       Output format\n",
            program_name);
}

int main (int argc, char *argv [])
{
    int verbose = 0;
    char *broker_endp = NULL;
    char *board_number_str = NULL;
    char *halcs_number_str = NULL;
    char *chan_str = NULL;
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

            case '?':
                fprintf (stderr, "[client:acq_prop] Option not recognized or missing argument\n");
                print_help (argv [0]);
                exit (1);
                break;

            default:
                fprintf (stderr, "[client:acq_prop] Could not parse options\n");
                print_help (argv [0]);
                exit (1);
         }
    }

    /* Set default broker address */
    if (broker_endp == NULL) {
        fprintf (stderr, "[client:acq_prop]: Setting default broker endpoint: %s\n",
                "ipc://"DFLT_BIND_FOLDER);
        broker_endp = strdup ("ipc://"DFLT_BIND_FOLDER);
    }

    /* Set default channel */
    uint32_t chan;
    if (chan_str == NULL) {
        fprintf (stderr, "[client:acq_prop]: Setting default value to 'chan'\n");
        chan = DFLT_CHAN_NUM;
    }
    else {
        chan = strtoul (chan_str, NULL, 10);

        if (chan > END_CHAN_ID-1) {
            fprintf (stderr, "[client:acq_prop]: Channel number too big! Defaulting to: %u\n",
                    MAX_NUM_CHANS);
            chan = END_CHAN_ID-1;
        }
    }

    /* Set default board number */
    uint32_t board_number;
    if (board_number_str == NULL) {
        fprintf (stderr, "[client:acq_prop]: Setting default value to BOARD number: %u\n",
                DFLT_BOARD_NUMBER);
        board_number = DFLT_BOARD_NUMBER;
    }
    else {
        board_number = strtoul (board_number_str, NULL, 10);
    }

    /* Set default halcs number */
    uint32_t halcs_number;
    if (halcs_number_str == NULL) {
        fprintf (stderr, "[client:acq_prop]: Setting default value to HALCS number: %u\n",
                DFLT_HALCS_NUMBER);
        halcs_number = DFLT_HALCS_NUMBER;
    }
    else {
        halcs_number = strtoul (halcs_number_str, NULL, 10);
    }

    char service[50];
    snprintf (service, sizeof (service), "HALCS%u:DEVIO:ACQ%u", board_number, halcs_number);

    acq_client_t *acq_client = acq_client_new (broker_endp, verbose, NULL);
    if (acq_client == NULL) {
        fprintf (stderr, "[client:acq_prop]: acq_client could not be created\n");
        goto err_acq_client_new;
    }

    /* Set trigger to skip */
    uint32_t ch_int_width = 0;
    uint32_t ch_num_coalesce = 0;
    uint32_t ch_num_atoms = 0;
    uint32_t ch_atom_width = 0;
    halcs_client_err_e err = halcs_get_acq_ch_int_width (acq_client, service,
          chan, &ch_int_width);
    err |= halcs_get_acq_ch_num_coalesce (acq_client, service,
          chan, &ch_num_coalesce);
    err |= halcs_get_acq_ch_num_atoms (acq_client, service,
          chan, &ch_num_atoms);
    err |= halcs_get_acq_ch_atom_width (acq_client, service,
          chan, &ch_atom_width);
    if (err != HALCS_CLIENT_SUCCESS){
        fprintf (stderr, "[client:acq_prop]: Could not get acquistion channel properties\n");
        goto err_prop_get;
    }

    fprintf (stdout, "[client:acq_prop]: ch_int_width = %u, ch_num_coalesce = %u, "
        " ch_num_atoms = %u, ch_atom_width = %u\n", ch_int_width, ch_num_coalesce, 
        ch_num_atoms, ch_atom_width);

err_prop_get:
err_acq_client_new:
    free (chan_str);
    chan_str = NULL;
    free (board_number_str);
    board_number_str = NULL;
    free (halcs_number_str);
    halcs_number_str = NULL;
    free (broker_endp);
    broker_endp = NULL;
    acq_client_destroy (&acq_client);

    return 0;
}
