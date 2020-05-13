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

#define DFLT_BIND_FOLDER            "/tmp/malamute"

#define DFLT_HALCS_NUMBER           0
#define DFLT_BOARD_NUMBER           0

#define MAX_CHANNEL_NUM             4
#define ALL_CHANNELS_NUM            4

enum fa100m14b4c_input_range {
    FMC_100M_4CH_RANGE_10V = 0x0,
    FMC_100M_4CH_RANGE_1V = 0x1,
    FMC_100M_4CH_RANGE_100mV = 0x2,
    FMC_100M_4CH_RANGE_OPEN = 0x3,       /* Channel disconnected from ADC */
    FMC_100M_4CH_RANGE_10V_CAL = 0x4,    /* Channel disconnected from ADC */
    FMC_100M_4CH_RANGE_1V_CAL = 0x5,     /* Channel disconnected from ADC */
    FMC_100M_4CH_RANGE_100mV_CAL = 0x6,  /* Channel disconnected from ADC */
    FMC_100M_4CH_RANGE_END = 0x7,        /* End marker */
};

static const int ssr_options[] = {
    [FMC_100M_4CH_RANGE_10V_CAL]   = 0x44,
    [FMC_100M_4CH_RANGE_1V_CAL]    = 0x40,
    [FMC_100M_4CH_RANGE_100mV_CAL] = 0x42,
    [FMC_100M_4CH_RANGE_10V]       = 0x45,
    [FMC_100M_4CH_RANGE_1V]        = 0x11,
    [FMC_100M_4CH_RANGE_100mV]     = 0x23,
    [FMC_100M_4CH_RANGE_OPEN]      = 0x00,
};

static struct option long_options[] =
{
    {"help",                no_argument,         NULL, 'h'},
    {"brokerendp",          required_argument,   NULL, 'b'},
    {"verbose",             no_argument,         NULL, 'v'},
    {"halcsnumber",         required_argument,   NULL, 's'},
    {"boardslot",           required_argument,   NULL, 'o'},
    {"test_pattern",        required_argument,   NULL, 't'},
    {"test_pattern_en",     required_argument,   NULL, 'e'},
    {"channel",             required_argument,   NULL, 'c'},
    {"ssr",                 required_argument,   NULL, 'r'},
    {"termination",         required_argument,   NULL, 'm'},
    {NULL, 0, NULL, 0}
};

static const char* shortopt = "hb:vo:s:t:e:c:r:m:";

void print_help (char *program_name)
{
    fprintf (stdout, "HALCSD FMC ADC 100M control utility\n"
            "Usage: %s [options]\n"
            "\n"
            "  -h  --help                           Display this usage information\n"
            "  -b  --brokerendp <Broker endpoint>   Broker endpoint\n"
            "  -v  --verbose                        Verbose output\n"
            "  -o  --boardslot <Board slot number = [1-12]> \n"
            "  -s  --halcsnumber <HALCS number = [0|1]> HALCS number\n"
            "                                       Board slot number\n"
            "  -t  --test_pattern <Pattern>         Test pattern\n"
            "  -e  --test_pattern_en <Enable>       Enable test pattern\n"
            "  -c  --channel <Channel=[0-3]>        Channel to apply operation. Select 4, for all channel\n"
            "  -r  --ssr <SSR option>               Select SSR option\n"
            "  -m  --termination <Term=[0|1]>       Select 50-ohm termination\n",
            program_name);
}

int main (int argc, char *argv [])
{
    int verbose = 0;
    char *broker_endp = NULL;
    char *board_number_str = NULL;
    char *halcs_number_str = NULL;
    char *test_pattern_str = NULL;
    char *test_pattern_en_str = NULL;
    char *channel_str = NULL;
    char *ssr_str = NULL;
    char *termination_str = NULL;
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

            case 't':
                test_pattern_str = strdup (optarg);
                break;

            case 'e':
                test_pattern_en_str = strdup (optarg);
                break;

            case 'r':
                ssr_str = strdup (optarg);
                break;

            case 'm':
                termination_str = strdup (optarg);
                break;

            case 'c':
                channel_str = strdup (optarg);
                break;

            case '?':
                fprintf (stderr, "[client:fmc100m_4ch_ctl] Option not recognized or missing argument\n");
                print_help (argv [0]);
                exit (1);
                break;

            default:
                fprintf (stderr, "[client:fmc100m_4ch_ctl] Could not parse options\n");
                print_help (argv [0]);
                exit (1);
         }
    }

    /* Set default broker address */
    if (broker_endp == NULL) {
        fprintf (stderr, "[client:fmc100m_4ch_ctl]: Setting default broker endpoint: %s\n",
                "ipc://"DFLT_BIND_FOLDER);
        broker_endp = strdup ("ipc://"DFLT_BIND_FOLDER);
    }

    /* Set default board number */
    uint32_t board_number;
    if (board_number_str == NULL) {
        fprintf (stderr, "[client:fmc100m_4ch_ctl]: Setting default value to BOARD number: %u\n",
                DFLT_BOARD_NUMBER);
        board_number = DFLT_BOARD_NUMBER;
    }
    else {
        board_number = strtoul (board_number_str, NULL, 10);
    }

    /* Set default halcs number */
    uint32_t halcs_number;
    if (halcs_number_str == NULL) {
        fprintf (stderr, "[client:fmc100m_4ch_ctl]: Setting default value to HALCS number: %u\n",
                DFLT_HALCS_NUMBER);
        halcs_number = DFLT_HALCS_NUMBER;
    }
    else {
        halcs_number = strtoul (halcs_number_str, NULL, 10);
    }

    char service[50];
    snprintf (service, sizeof (service), "HALCS%u:DEVIO:FMC_100M_4CH%u", board_number, halcs_number);

    halcs_client_t *halcs_client = halcs_client_new (broker_endp, verbose, NULL);
    if (halcs_client == NULL) {
        fprintf (stderr, "[client:fmc100m_4ch_ctl]: halcs_client could be created\n");
        goto err_halcs_client_new;
    }

    halcs_client_err_e err;
    uint32_t test_pattern = 0;
    if (test_pattern_str != NULL) {
        test_pattern = strtoul (test_pattern_str, NULL, 10);

        fprintf (stdout, "[client:fmc100m_4ch_ctl]: test_pattern = 0x%08X\n", test_pattern);
        err = halcs_set_adc100_ltc2174_test_patt_data (halcs_client, service, test_pattern);
        if (err != HALCS_CLIENT_SUCCESS){
            fprintf (stderr, "[client:fmc100m_4ch_ctl]: halcs_set_test_pattern failed\n");
            goto err_halcs_exit;
        }
    }

    uint32_t test_pattern_en = 0;
    if (test_pattern_en_str != NULL) {
        test_pattern_en = strtoul (test_pattern_en_str, NULL, 10);

        fprintf (stdout, "[client:fmc100m_4ch_ctl]: test_pattern_en = 0x%08X\n", test_pattern_en);
        err = halcs_set_adc100_ltc2174_test_patt (halcs_client, service, test_pattern_en);
        if (err != HALCS_CLIENT_SUCCESS){
            fprintf (stderr, "[client:fmc100m_4ch_ctl]: halcs_set_test_pattern_en failed\n");
            goto err_halcs_exit;
        }
    }

    uint32_t termination = 0;
    if (termination_str != NULL) {
        termination = strtoul (termination_str, NULL, 10);

        fprintf (stdout, "[client:fmc100m_4ch_ctl]: termination = 0x%08X\n", termination);
    }

    uint32_t ssr = 0;
    if (ssr_str != NULL) {
        uint32_t channel = 0;
        if (channel_str != NULL) {
            channel = strtoul (channel_str, NULL, 10);
            fprintf (stdout, "[client:fmc100m_4ch_ctl]: channel = 0x%08X\n", channel);
        }
        else {
            fprintf (stderr, "[client:fmc100m_4ch_ctl]: Option --ssr requires --channel option\n");
            goto err_halcs_exit;
        }

        if (channel > MAX_CHANNEL_NUM) {
            fprintf (stderr, "[client:fmc100m_4ch_ctl]: Option --channel is out of range\n");
            goto err_halcs_exit;
        }

        ssr = strtoul (ssr_str, NULL, 10);

        /* Check if out of limits */
        if (ssr > FMC_100M_4CH_RANGE_END-1) {
            fprintf (stderr, "[client:fmc100m_4ch_ctl]: --ssr option is out of range\n");
            goto err_halcs_exit;
        }

        fprintf (stdout, "[client:fmc100m_4ch_ctl]: ssr = 0x%08X\n", ssr);

        uint32_t channel_start = 0;
        uint32_t channel_end = 0;
        if (channel == ALL_CHANNELS_NUM) {
            channel_start = 0;
            channel_end = ALL_CHANNELS_NUM;
        }
        else {
           channel_start = channel;
           channel_end = channel+1;
        }

        uint32_t i;
        for (i = channel_start; i < channel_end; ++i) {
            uint32_t ssr_term = ssr_options[ssr] | ((termination)? 0x08 : 0x00);
            err = halcs_set_adc100_adc_ssr (halcs_client, service, i, ssr_term);
            if (err != HALCS_CLIENT_SUCCESS) {
                fprintf (stderr, "[client:fmc100m_4ch_ctl]: halcs_set_ssr failed\n");
                goto err_halcs_exit;
            }
            fprintf (stdout, "[client:fmc100m_4ch_ctl]: ssr option 0x%08X, ssr bits 0x%08X set for channel #%u\n",
                ssr, ssr_term, i);
        }
    }

    uint32_t chan = 0;
    uint32_t status = 0;
    for (chan = 0; chan < 4; ++chan) {
        status = 0;
        err = halcs_get_adc100_adc_status (halcs_client, service, chan, &status);
        if (err != HALCS_CLIENT_SUCCESS){
            fprintf (stderr, "[client:fmc100m_4ch_ctl]: halcs_get_adc100_adc_status failed\n");
            goto err_halcs_exit;
        }

        fprintf (stdout, "[client:fmc100m_4ch_ctl]: FMC channel #%u status = 0x%08X\n", chan, status);
    }

err_halcs_exit:
err_halcs_client_new:
    free (channel_str);
    channel_str = NULL;
    free (termination_str);
    termination_str = NULL;
    free (ssr_str);
    ssr_str = NULL;
    free (test_pattern_en_str);
    test_pattern_en_str = NULL;
    free (test_pattern_str);
    test_pattern_str = NULL;
    free (board_number_str);
    board_number_str = NULL;
    free (halcs_number_str);
    halcs_number_str = NULL;
    free (broker_endp);
    broker_endp = NULL;
    halcs_client_destroy (&halcs_client);

    return 0;
}
