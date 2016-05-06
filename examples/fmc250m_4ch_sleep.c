/*
 * Controlling the FMC 250 MSPS leds
 */

#include <inttypes.h>
#include <bpm_client.h>

#define DFLT_BIND_FOLDER            "/tmp/bpm"

#define DFLT_BPM_NUMBER             0
#define MAX_BPM_NUMBER              1

#define DFLT_BOARD_NUMBER           0

#define DFLT_TEST_MODE              0
#define MAX_TEST_MODE               1

void print_help (char *program_name)
{
    printf( "Usage: %s [options]\n"
            "\t-h This help message\n"
            "\t-v Verbose output\n"
            "\t-board <AMC board = [0|1|2|3|4|5]>\n"
            "\t-bpm <BPM number = [0|1]>\n"
            "\t-sleep_adcs <Test mode = [0-15]>\n"
            "\t-b <broker_endpoint> Broker endpoint\n", program_name);
}

int main (int argc, char *argv [])
{
    int verbose = 0;
    char *broker_endp = NULL;
    char *board_number_str = NULL;
    char *bpm_number_str = NULL;
    char *sleep_adcs_str = NULL;
    char **str_p = NULL;

    if (argc < 2) {
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
        else if (streq(argv[i], "-sleep_adcs"))
        {
            str_p = &sleep_adcs_str;
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
        fprintf (stderr, "[client:fmc250m_sleep_adcs]: Setting default value to BOARD number: %u\n",
                DFLT_BOARD_NUMBER);
        board_number = DFLT_BOARD_NUMBER;
    }
    else {
        board_number = strtoul (board_number_str, NULL, 10);
    }

    /* Set default bpm number */
    uint32_t bpm_number;
    if (bpm_number_str == NULL) {
        fprintf (stderr, "[client:fmc250m_sleep_adcs]: Setting default value to BPM number: %u\n",
                DFLT_BPM_NUMBER);
        bpm_number = DFLT_BPM_NUMBER;
    }
    else {
        bpm_number = strtoul (bpm_number_str, NULL, 10);

        if (bpm_number > MAX_BPM_NUMBER) {
            fprintf (stderr, "[client:fmc250m_sleep_adcs]: BPM number too big! Defaulting to: %u\n",
                    MAX_BPM_NUMBER);
            bpm_number = MAX_BPM_NUMBER;
        }
    }

    /* Set default bpm number */
    uint32_t sleep_adcs;
    if (sleep_adcs_str == NULL) {
        fprintf (stderr, "[client:fmc250m_sleep_adcs]: Setting default value to test mode: %u\n",
                DFLT_TEST_MODE);
        sleep_adcs = DFLT_TEST_MODE;
    }
    else {
        sleep_adcs = strtoul (sleep_adcs_str, NULL, 10);

        if (sleep_adcs > MAX_TEST_MODE) {
            fprintf (stderr, "[client:fmc250m_sleep_adcs]: Test mode too big! Defaulting to: %u\n",
                    MAX_TEST_MODE);
            sleep_adcs = MAX_TEST_MODE;
        }
    }

    char service[50];
    snprintf (service, sizeof (service), "BPM%u:DEVIO:FMC250M_4CH%u", board_number, bpm_number);

    bpm_client_t *bpm_client = bpm_client_new (broker_endp, verbose, NULL);
    if (bpm_client == NULL) {
        fprintf (stderr, "[client:fmc250m_sleep_adcs]: bpm_client could be created\n");
        goto err_bpm_client_new;
    }

    bpm_client_err_e err = bpm_set_sleep_adcs (bpm_client, service, sleep_adcs);
    if (err != BPM_CLIENT_SUCCESS) {
        fprintf (stderr, "[client:fmc250_sleep_adcs]: bpm_set_sleep_adcs error\n");
        goto err_bpm_client_new;
    }

err_bpm_client_new:
    bpm_client_destroy (&bpm_client);

    str_p = &sleep_adcs_str;
    free (*str_p);
    sleep_adcs_str = NULL;
    str_p = &broker_endp;
    free (*str_p);
    broker_endp = NULL;
    str_p = &board_number_str;
    free (*str_p);
    board_number_str = NULL;
    str_p = &bpm_number_str;
    free (*str_p);
    bpm_number_str = NULL;
    return 0;
}
