/*
 * Controlling the FMC 250 MSPS leds
 */

#include <inttypes.h>
#include <halcs_client.h>

#define DFLT_BIND_FOLDER            "/tmp/halcs"

#define DFLT_HALCS_NUMBER             0
#define MAX_HALCS_NUMBER              1

#define DFLT_BOARD_NUMBER           0

#define DFLT_TEST_MODE              0
#define MAX_TEST_MODE               4

void print_help (char *program_name)
{
    printf( "Usage: %s [options]\n"
            "\t-h This help message\n"
            "\t-v Verbose output\n"
            "\t-board <AMC board = [0|1|2|3|4|5]>\n"
            "\t-halcs <HALCS number = [0|1]>\n"
            "\t-sleep_adcs <Test mode = [0-15]>\n"
            "\t-b <broker_endpoint> Broker endpoint\n", program_name);
}

int main (int argc, char *argv [])
{
    int verbose = 0;
    char *broker_endp = NULL;
    char *board_number_str = NULL;
    char *halcs_number_str = NULL;
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
        else if (streq(argv[i], "-halcs"))
        {
            str_p = &halcs_number_str;
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


    /* Set default halcs number */
    uint32_t halcs_number;
    if (halcs_number_str == NULL) {
        fprintf (stderr, "[client:fmc250m_sleep_adcs]: Setting default value to HALCS number: %u\n",
                DFLT_HALCS_NUMBER);
        halcs_number = DFLT_HALCS_NUMBER;
    }
    else {
        halcs_number = strtoul (halcs_number_str, NULL, 10);

        if (halcs_number > MAX_HALCS_NUMBER) {
            fprintf (stderr, "[client:fmc250m_sleep_adcs]: HALCS number too big! Defaulting to: %u\n",
                    MAX_HALCS_NUMBER);
            halcs_number = MAX_HALCS_NUMBER;
        }
    }

    /* Set default halcs number */
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
    snprintf (service, sizeof (service), "HALCS%u:DEVIO:FMC250M_4CH%u", board_number, halcs_number);

    halcs_client_t *halcs_client = halcs_client_new (broker_endp, verbose, NULL);
    if (halcs_client == NULL) {
        fprintf (stderr, "[client:fmc250m_sleep_adcs]: halcs_client could be created\n");
        goto err_halcs_client_new;
    }

    halcs_client_err_e err = 0x0;
    err |= halcs_set_rst_adcs (halcs_client, service, 0x1);
    err |= halcs_set_rst_div_adcs (halcs_client, service, 0x1);
    err |= halcs_set_sleep_adcs (halcs_client, service, sleep_adcs);
    for (i = 0; i < 4; ++i) {
        err |= halcs_set_rst_modes_adc (halcs_client, service, i, 0x1);
    } 
    if (err != HALCS_CLIENT_SUCCESS)  {
        fprintf (stderr, "[client:fmc250_sleep_adcs]: halcs_set_sleep_adcs error\n");
        goto err_halcs_client_new;
    }

err_halcs_client_new:
    halcs_client_destroy (&halcs_client);

    str_p = &sleep_adcs_str;
    free (*str_p);
    sleep_adcs_str = NULL;
    str_p = &broker_endp;
    free (*str_p);
    broker_endp = NULL;
    str_p = &board_number_str;
    free (*str_p);
    board_number_str = NULL;
    str_p = &halcs_number_str;
    free (*str_p);
    halcs_number_str = NULL;
    return 0;
}
