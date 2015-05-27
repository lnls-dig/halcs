/*
 * Simple example demonstrating the communication between
 * a client and the FPGA device
 */

#include <mdp.h>
#include <czmq.h>
#include <inttypes.h>

#include <bpm_client.h>

#define DFLT_BIND_FOLDER            "/tmp/bpm"

#define DFLT_BPM_NUMBER             0
#define MAX_BPM_NUMBER              1

#define DFLT_BOARD_NUMBER           0
#define MAX_BOARD_NUMBER            5

#define DFLT_SI571_FREQ             100000000.000   /* 100 MHz */
#define MAX_SI571_FREQ              1000000000.000  /* 1GHz */

void print_help (char *program_name)
{
    printf( "Usage: %s [options]\n"
            "\t-h This help message\n"
            "\t-v Verbose output\n"
            "\t-board <AMC board = [0|1|2|3|4|5]>\n"
            "\t-bpm <BPM number = [0|1]>\n"
            "\t-b <broker_endpoint> Broker endpoint\n"
            "\t-freq <frequency> Si571 frequency\n",
            program_name);
}

int main (int argc, char *argv [])
{
    int verbose = 0;
    char *broker_endp = NULL;
    char *board_number_str = NULL;
    char *bpm_number_str = NULL;
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
        else if (streq(argv[i], "-bpm"))
        {
            str_p = &bpm_number_str;
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
        fprintf (stderr, "[client:si571_ctl]: Setting default value to BOARD number: %u\n",
                DFLT_BOARD_NUMBER);
        board_number = DFLT_BOARD_NUMBER;
    }
    else {
        board_number = strtoul (board_number_str, NULL, 10);

        if (board_number > MAX_BOARD_NUMBER) {
            fprintf (stderr, "[client:si571_ctl]: BOARD number too big! Defaulting to: %u\n",
                    MAX_BOARD_NUMBER);
            board_number = MAX_BOARD_NUMBER;
        }
    }

    /* Set default bpm number */
    uint32_t bpm_number;
    if (bpm_number_str == NULL) {
        fprintf (stderr, "[client:si571_ctl]: Setting default value to BPM number: %u\n",
                DFLT_BPM_NUMBER);
        bpm_number = DFLT_BPM_NUMBER;
    }
    else {
        bpm_number = strtoul (bpm_number_str, NULL, 10);

        if (bpm_number > MAX_BPM_NUMBER) {
            fprintf (stderr, "[client:si571_ctl]: BPM number too big! Defaulting to: %u\n",
                    MAX_BPM_NUMBER);
            bpm_number = MAX_BPM_NUMBER;
        }
    }

    /* Set default frequency */
    double si571_freq;
    if (si571_freq_str == NULL) {
        fprintf (stderr, "[client:si571_ctl]: Setting default value to BPM number: %f\n",
                DFLT_SI571_FREQ);
        si571_freq = DFLT_SI571_FREQ;
    }
    else {
        si571_freq = strtod (si571_freq_str, NULL);

        if (si571_freq > MAX_SI571_FREQ) {
            fprintf (stderr, "[client:si571_ctl]: BPM number too big! Defaulting to: %f\n",
                    MAX_SI571_FREQ);
            si571_freq = MAX_SI571_FREQ;
        }
    }

    char service[50];
    snprintf (service, strlen (service)+1, "BPM%u:DEVIO:FMC130M_4CH%u", board_number, bpm_number);

    bpm_client_t *bpm_client = bpm_client_new (broker_endp, verbose, NULL);
    if (bpm_client == NULL) {
        fprintf (stderr, "[client:acq]: bpm_client could be created\n");
        goto err_bpm_client_new;
    }

    bpm_client_err_e err = bpm_set_si571_set_freq (bpm_client, service, si571_freq);
    if (err != BPM_CLIENT_SUCCESS){
        fprintf (stderr, "[client:si571_ctl]: Si571 Set frequency failed\n");
        goto err_bpm_set_freq;
    }

err_bpm_client_new:
err_bpm_set_freq:
    bpm_client_destroy (&bpm_client);

    str_p = &si571_freq_str;
    free (*str_p);
    si571_freq_str = NULL;
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
