/*
 * Controlling the ADC delays
 */

#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <halcs_client.h>

#define DFLT_BIND_FOLDER            "/tmp/halcs"

/* Default value definitions */

#define DFLT_HALCS_NUMBER             0
#define MAX_HALCS_NUMBER              1

#define DFLT_BOARD_NUMBER           0

#define DFLT_ADC_DLY_VALUE          10
#define MAX_ADC_DLY_VALUE           31

#define DFLT_CHAN_NUM               0
#define MAX_CHAN_NUM                3 /* 0-3 */

#define DFLT_LINES_NUM              1 /* DATA */
#define MAX_LINES_NUM               3 /* DATA + CLOCK */

void print_help (char *program_name)
{
    printf( "Usage: %s [options]\n"
            "\t-h This help message\n"
            "\t-v Verbose output\n"
            "\t-b <broker_endpoint> Broker endpoint\n"
            "\t-board <AMC board = [0|1|2|3|4|5]>\n"
            "\t-halcs <HALCS number = [0|1]>\n"
            "\t-ch <channel> ADC channel\n"
            "\t-lines <delay_lines> ADC delay lines (1 = data, 2 = clock, 3 = both)\n"
            "\t-val <delay_value> ADC delay value (0-31)\n"
            , program_name);
}

int main (int argc, char *argv [])
{
    int verbose = 0;
    char *broker_endp = NULL;
    char *board_number_str = NULL;
    char *halcs_number_str = NULL;
    char *chan_str = NULL;
    char *lines_str = NULL;
    char *dly_val_str = NULL;
    char **str_p = NULL;

    if (argc < 3) {
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
        else if (streq(argv[i], "-board"))
        {
            str_p = &board_number_str;
        }
        else if (streq(argv[i], "-halcs"))
        {
            str_p = &halcs_number_str;
        }
        else if (streq (argv[i], "-ch")) { /* ch: channel */
            str_p = &chan_str;
        }
        else if (streq (argv[i], "-lines")) { /* lines: delay lines */
            str_p = &lines_str;
        }
        else if (streq (argv[i], "-val")) { /* val: delay value */
            str_p = &dly_val_str;
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

    /* Set default halcs number */
    uint32_t halcs_number;
    if (halcs_number_str == NULL) {
        fprintf (stderr, "[client:leds]: Setting default value to HALCS number: %u\n",
                DFLT_HALCS_NUMBER);
        halcs_number = DFLT_HALCS_NUMBER;
    }
    else {
        halcs_number = strtoul (halcs_number_str, NULL, 10);

        if (halcs_number > MAX_HALCS_NUMBER) {
            fprintf (stderr, "[client:leds]: HALCS number too big! Defaulting to: %u\n",
                    MAX_HALCS_NUMBER);
            halcs_number = MAX_HALCS_NUMBER;
        }
    }

    /* Set default channel */
    uint32_t chan;
    if (chan_str == NULL) {
        fprintf (stderr, "[client:adc250_dly]: Setting default value to: %u\n",
                DFLT_CHAN_NUM);
        chan = DFLT_CHAN_NUM;
    }
    else {
        chan = strtoul (chan_str, NULL, 10);

        if (chan > MAX_CHAN_NUM) {
            fprintf (stderr, "[client:adc250_dly]: Channel number too big! Defaulting to: %u\n",
                    MAX_CHAN_NUM);
            chan = MAX_CHAN_NUM;
        }
    }
    fprintf (stdout, "[client:adc250_dly]: chan = %u\n", chan);

    /* Set ADC default line values */
    uint32_t lines = 0;
    if (lines_str == NULL) {
        fprintf (stderr, "[client:adc250_dly]: Setting default ADC lines to: %u\n",
                DFLT_LINES_NUM);
        lines = DFLT_LINES_NUM;
    }
    else {
        lines = strtoul (lines_str, NULL, 10);

        if (lines > MAX_LINES_NUM) {
            fprintf (stderr, "[client:adc250_dly]: ADC delay value too big! Defaulting to: %u\n",
                    MAX_LINES_NUM);
            lines = MAX_LINES_NUM;
        }
    }
    fprintf (stdout, "[client:adc250_dly]: ADC lines value = %u\n", lines);

    /* Set default ADC Delay values */
    uint32_t dly_val = 0;
    if (dly_val_str == NULL) {
        fprintf (stderr, "[client:adc250_dly]: Setting default ADC delay value to: %u\n",
                DFLT_ADC_DLY_VALUE);
        dly_val = DFLT_ADC_DLY_VALUE;
    }
    else {
        dly_val = strtoul (dly_val_str, NULL, 10);

        if (dly_val > MAX_ADC_DLY_VALUE) {
            fprintf (stderr, "[client:adc250_dly]: ADC delay value too big! Defaulting to: %u\n",
                    MAX_ADC_DLY_VALUE);
            dly_val = MAX_ADC_DLY_VALUE;
        }
    }
    fprintf (stdout, "[client:adc250_dly]: ADC delay value = %u\n", dly_val);

    char service[50];
    snprintf (service, sizeof (service), "HALCS%u:DEVIO:FMC250M_4CH%u", board_number, halcs_number);

    halcs_client_t *halcs_client = halcs_client_new (broker_endp, verbose, NULL);
    if (halcs_client == NULL) {
        fprintf (stderr, "[client:acq]: halcs_client could be created\n");
        goto err_halcs_client_new;
    }

    /* Call the appropriate delay function. FIXME: the case construct is
     * not generic nor expansible */
    switch(chan) {
        case 0:
            halcs_set_adc250_dly0 (halcs_client, service, lines, dly_val);
            break;
        case 1:
            halcs_set_adc250_dly1 (halcs_client, service, lines, dly_val);
            break;
        case 2:
            halcs_set_adc250_dly2 (halcs_client, service, lines, dly_val);
            break;
        case 3:
            halcs_set_adc250_dly3 (halcs_client, service, lines, dly_val);
            break;
        default:
            halcs_set_adc250_dly0 (halcs_client, service, lines, dly_val);
            break;
    }

err_halcs_client_new:
    halcs_client_destroy (&halcs_client);

    str_p = &board_number_str;
    free (*str_p);
    board_number_str = NULL;
    str_p = &halcs_number_str;
    free (*str_p);
    halcs_number_str = NULL;
    str_p = &chan_str;
    free (*str_p);
    chan_str = NULL;
    str_p = &dly_val_str;
    free (*str_p);
    dly_val_str = NULL;
    str_p = &broker_endp;
    free (*str_p);
    broker_endp = NULL;

    return 0;
}
