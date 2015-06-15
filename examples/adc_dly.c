/*
 * Controlling the ADC delays
 */

#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <bpm_client.h>

#define DFLT_BIND_FOLDER            "/tmp/bpm"

/* Default value definitions */

#define DFLT_BPM_NUMBER             0
#define MAX_BPM_NUMBER              1

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
            "\t-bpm <BPM number = [0|1]>\n"
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
    char *bpm_number_str = NULL;
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
        else if (streq(argv[i], "-bpm"))
        {
            str_p = &bpm_number_str;
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

    /* Set default channel */
    uint32_t chan;
    if (chan_str == NULL) {
        fprintf (stderr, "[client:adc_dly]: Setting default value to: %u\n",
                DFLT_CHAN_NUM);
        chan = DFLT_CHAN_NUM;
    }
    else {
        chan = strtoul (chan_str, NULL, 10);

        if (chan > MAX_CHAN_NUM) {
            fprintf (stderr, "[client:adc_dly]: Channel number too big! Defaulting to: %u\n",
                    MAX_CHAN_NUM);
            chan = MAX_CHAN_NUM;
        }
    }
    fprintf (stdout, "[client:adc_dly]: chan = %u\n", chan);

    /* Set ADC default line values */
    uint32_t lines = 0;
    if (lines_str == NULL) {
        fprintf (stderr, "[client:adc_dly]: Setting default ADC lines to: %u\n",
                DFLT_LINES_NUM);
        lines = DFLT_LINES_NUM;
    }
    else {
        lines = strtoul (lines_str, NULL, 10);

        if (lines > MAX_LINES_NUM) {
            fprintf (stderr, "[client:adc_dly]: ADC delay value too big! Defaulting to: %u\n",
                    MAX_LINES_NUM);
            lines = MAX_LINES_NUM;
        }
    }
    fprintf (stdout, "[client:adc_dly]: ADC lines value = %u\n", lines);

    /* Set default ADC Delay values */
    uint32_t dly_val = 0;
    if (dly_val_str == NULL) {
        fprintf (stderr, "[client:adc_dly]: Setting default ADC delay value to: %u\n",
                DFLT_ADC_DLY_VALUE);
        dly_val = DFLT_ADC_DLY_VALUE;
    }
    else {
        dly_val = strtoul (dly_val_str, NULL, 10);

        if (dly_val > MAX_ADC_DLY_VALUE) {
            fprintf (stderr, "[client:adc_dly]: ADC delay value too big! Defaulting to: %u\n",
                    MAX_ADC_DLY_VALUE);
            dly_val = MAX_ADC_DLY_VALUE;
        }
    }
    fprintf (stdout, "[client:adc_dly]: ADC delay value = %u\n", dly_val);

    char service[50];
    snprintf (service, sizeof (service), "BPM%u:DEVIO:FMC130M_4CH%u", board_number, bpm_number);

    bpm_client_t *bpm_client = bpm_client_new (broker_endp, verbose, NULL);
    if (bpm_client == NULL) {
        fprintf (stderr, "[client:acq]: bpm_client could be created\n");
        goto err_bpm_client_new;
    }

    /* Call the appropriate delay function. FIXME: the case construct is
     * not generic nor expansible */
    switch(chan) {
        case 0:
            bpm_set_adc_dly0 (bpm_client, service, lines, dly_val);
            break;
        case 1:
            bpm_set_adc_dly1 (bpm_client, service, lines, dly_val);
            break;
        case 2:
            bpm_set_adc_dly2 (bpm_client, service, lines, dly_val);
            break;
        case 3:
            bpm_set_adc_dly3 (bpm_client, service, lines, dly_val);
            break;
        default:
            bpm_set_adc_dly0 (bpm_client, service, lines, dly_val);
            break;
    }

err_bpm_client_new:
    bpm_client_destroy (&bpm_client);

    str_p = &board_number_str;
    free (*str_p);
    board_number_str = NULL;
    str_p = &bpm_number_str;
    free (*str_p);
    bpm_number_str = NULL;
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
