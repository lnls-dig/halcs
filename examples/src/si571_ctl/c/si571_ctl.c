/*
 * Controlling the Si571 in FMC 130 MSPS chip
 */

#include <inttypes.h>
#include <halcs_client.h>

#define DFLT_BIND_FOLDER            "/tmp/halcs"

#define DFLT_HALCS_NUMBER             0
#define MAX_HALCS_NUMBER              1

#define DFLT_BOARD_NUMBER           0

#define DFLT_SI571_FREQ             100000000.000   /* 100 MHz */
#define MAX_SI571_FREQ              1000000000.000  /* 1GHz */

void print_help (char *program_name)
{
    printf( "Usage: %s [options]\n"
            "\t-h This help message\n"
            "\t-v Verbose output\n"
            "\t-board <AMC board = [0|1|2|3|4|5]>\n"
            "\t-halcs <HALCS number = [0|1]>\n"
            "\t-b <broker_endpoint> Broker endpoint\n"
            "\t-freq <frequency> Si571 frequency\n",
            program_name);
}

int main (int argc, char *argv [])
{
    int verbose = 0;
    char *broker_endp = NULL;
    char *board_number_str = NULL;
    char *halcs_number_str = NULL;
    char *si571_freq_str = NULL;
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
        else if (streq(argv[i], "-freq"))
        {
            str_p = &si571_freq_str;
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
        fprintf (stderr, "[client:si571_ctl]: Setting default value to HALCS number: %u\n",
                DFLT_HALCS_NUMBER);
        halcs_number = DFLT_HALCS_NUMBER;
    }
    else {
        halcs_number = strtoul (halcs_number_str, NULL, 10);

        if (halcs_number > MAX_HALCS_NUMBER) {
            fprintf (stderr, "[client:si571_ctl]: HALCS number too big! Defaulting to: %u\n",
                    MAX_HALCS_NUMBER);
            halcs_number = MAX_HALCS_NUMBER;
        }
    }

    /* Set default frequency */
    double si571_freq;
    if (si571_freq_str == NULL) {
        fprintf (stderr, "[client:si571_ctl]: Setting default value to HALCS number: %f\n",
                DFLT_SI571_FREQ);
        si571_freq = DFLT_SI571_FREQ;
    }
    else {
        si571_freq = strtod (si571_freq_str, NULL);

        if (si571_freq > MAX_SI571_FREQ) {
            fprintf (stderr, "[client:si571_ctl]: HALCS number too big! Defaulting to: %f\n",
                    MAX_SI571_FREQ);
            si571_freq = MAX_SI571_FREQ;
        }
    }

    char service[50];
    snprintf (service, sizeof (service), "HALCS%u:DEVIO:FMC_ACTIVE_CLK%u", board_number, halcs_number);

    halcs_client_t *halcs_client = halcs_client_new (broker_endp, verbose, NULL);
    if (halcs_client == NULL) {
        fprintf (stderr, "[client:acq]: halcs_client could be created\n");
        goto err_halcs_client_new;
    }

    halcs_client_err_e err = halcs_set_si571_freq (halcs_client, service, si571_freq);
    if (err != HALCS_CLIENT_SUCCESS){
        fprintf (stderr, "[client:si571_ctl]: Si571 Set frequency failed\n");
        goto err_halcs_set_freq;
    }

    err = halcs_set_si571_oe (halcs_client, service, 0x1);
    if (err != HALCS_CLIENT_SUCCESS){
        fprintf (stderr, "[client:si571_ctl]: Si571 Set Output Enable failed\n");
        goto err_halcs_set_freq;
    }

    double freq = 0;
    err = halcs_get_si571_freq (halcs_client, service, &freq);
    if (err != HALCS_CLIENT_SUCCESS){
        fprintf (stderr, "[client:si571_ctl]: Si571 Set frequency failed\n");
        goto err_halcs_set_freq;
    }

    printf ("Freq: %f\n", freq);

err_halcs_client_new:
err_halcs_set_freq:
    halcs_client_destroy (&halcs_client);

    str_p = &si571_freq_str;
    free (*str_p);
    si571_freq_str = NULL;
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
