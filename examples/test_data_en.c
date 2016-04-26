/*
 * Controlling the FMC 130 MSPS test functions
 */

#include <inttypes.h>
#include <bpm_client.h>

#define DFLT_BIND_FOLDER            "/tmp/bpm"

#define DFLT_TEST_DATA_EN           0
#define MAX_TEST_DATA_EN            1

#define DFLT_BPM_NUMBER             0
#define MAX_BPM_NUMBER              1

#define DFLT_BOARD_NUMBER           0

void print_help (char *program_name)
{
    printf( "Usage: %s [options]\n"
            "\t-h This help message\n"
            "\t-v Verbose output\n"
            "\t-board <AMC board = [0|1|2|3|4|5]>\n"
            "\t-bpm <BPM number = [0|1]>\n"
            "\t-e <[0|1]> Enable or disable TEST data\n"
            "\t-b <broker_endpoint> Broker endpoint\n", program_name);
}

int main (int argc, char *argv [])
{
    int verbose = 0;
    char *broker_endp = NULL;
    char *board_number_str = NULL;
    char *bpm_number_str = NULL;
    char *test_data_en_str = NULL;
    char **str_p = NULL;

    if (argc < 3) {
        print_help (argv[0]);
        exit (1);
    }

    /* FIXME: This is rather buggy! */
    /* Simple handling of command-line options. This should be done
     * with getopt, for instance*/
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
        else if (streq (argv[i], "-e")) {
            str_p = &test_data_en_str;
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
        fprintf (stderr, "[client:leds]: Setting default value to BOARD number: %u\n",
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

    /* Set test data enable default */
    uint32_t test_data_en;
    if (test_data_en_str == NULL) {
        fprintf (stderr, "[client:test_data_en]: Setting test DATA enable to 0\n");
        test_data_en = DFLT_TEST_DATA_EN;
    }
    else {
        test_data_en = strtoul (test_data_en_str, NULL, 10);

        if (test_data_en > MAX_TEST_DATA_EN) {
            fprintf (stderr, "[client:test_data_en]: Test data enable value too big! Defaulting to: %u\n",
                    MAX_TEST_DATA_EN);
            test_data_en = MAX_TEST_DATA_EN;
        }
    }
    fprintf (stdout, "[client:test_data_en]: test_data_en = %u\n", test_data_en);

    char service[50];
    snprintf (service, sizeof (service), "BPM%u:DEVIO:FMC_ADC_COMMON%u", board_number, bpm_number);

    bpm_client_t *bpm_client = bpm_client_new (broker_endp, verbose, NULL);
    if (bpm_client == NULL) {
        fprintf (stderr, "[client:acq]: bpm_client could be created\n");
        goto err_bpm_client_new;
    }

    /* Test data enable */
    bpm_set_adc_test_data_en (bpm_client, service, test_data_en);

    /* Cleanup */
err_bpm_client_new:
    bpm_client_destroy (&bpm_client);

    str_p = &board_number_str;
    free (*str_p);
    board_number_str = NULL;
    str_p = &bpm_number_str;
    free (*str_p);
    bpm_number_str = NULL;
    str_p = &test_data_en_str;
    free (*str_p);
    test_data_en_str = NULL;
    str_p = &broker_endp;
    free (*str_p);
    broker_endp = NULL;
    return 0;
}
