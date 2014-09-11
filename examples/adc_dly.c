/*
 *  * Simple example demonstrating the communication between
 *   * a client and the FPGA device
 *    */

#include <mdp.h>
#include <czmq.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <bpm_client.h>

#define DFLT_BIND_FOLDER            "/tmp/bpm"

/* Default value definitions */
#define DEFAULT_ADC_DLY_VALUE       10
#define MAX_ADC_DLY_VALUE           31

#define DEFAULT_CHAN_NUM            0
#define MAX_CHAN_NUM                3 /* 0-3 */

void print_help (char *program_name)
{
    printf( "Usage: %s [options]\n"
            "\t-h This help message\n"
            "\t-v Verbose output\n"
            "\t-b <broker_endpoint> Broker endpoint\n"
            "\t-ch <channel> ADC channel\n"
            "\t-val <delay_value> ADC delay value (0-31)\n"
            , program_name);
}

int main (int argc, char *argv [])
{
    int verbose = 0;
    char *broker_endp = NULL;
    char *dly_val_str = NULL;
    char *chan_str = NULL;
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
        else if (streq (argv[i], "-val")) { /* val: delay value */
            str_p = &dly_val_str;
        }
        else if (streq (argv[i], "-ch")) { /* ch: channel */
            str_p = &chan_str;
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

    bpm_client_t *bpm_client = bpm_client_new (broker_endp, verbose, NULL);

    /* Set default ADC Delay values */
    uint32_t dly_val = 0;
    if (dly_val_str == NULL) {
        fprintf (stderr, "[client:adc_dly]: Setting default ADC delay value to: %u\n",
                DEFAULT_ADC_DLY_VALUE);
        dly_val = DEFAULT_ADC_DLY_VALUE;
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

    /* Set default channel */
    uint32_t chan;
    if (chan_str == NULL) {
        fprintf (stderr, "[client:adc_dly]: Setting default value to: %u\n",
                DEFAULT_CHAN_NUM);
        chan = DEFAULT_CHAN_NUM;
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

    /* Call the appropriate delay fucntion. FIXME: the case construct is
     * not generic nor expansible */
    switch(chan) {
        case 0:
            bpm_set_adc_dly0 (bpm_client, "BPM0:DEVIO:FMC130M_4CH0", chan, dly_val);
            break;
        case 1:
            bpm_set_adc_dly1 (bpm_client, "BPM0:DEVIO:FMC130M_4CH0", chan, dly_val);
            break;
        case 2:
            bpm_set_adc_dly2 (bpm_client, "BPM0:DEVIO:FMC130M_4CH0", chan, dly_val);
            break;
        case 3:
            bpm_set_adc_dly3 (bpm_client, "BPM0:DEVIO:FMC130M_4CH0", chan, dly_val);
            break;
        default:
            bpm_set_adc_dly0 (bpm_client, "BPM0:DEVIO:FMC130M_4CH0", chan, dly_val);
            break;
    }

    str_p = &chan_str;
    free (*str_p);
    chan_str = NULL;
    str_p = &dly_val_str;
    free (*str_p);
    dly_val_str = NULL;
    str_p = &broker_endp;
    free (*str_p);
    broker_endp = NULL;
    bpm_client_destroy (&bpm_client);

    return 0;
}
