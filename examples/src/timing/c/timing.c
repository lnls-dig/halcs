/*
 * Controlling the AFC Timing board
 */

#include <inttypes.h>
#include <halcs_client.h>

#define DFLT_BIND_FOLDER            "/tmp/bpm"

#define DFLT_BPM_NUMBER             0
#define MAX_BPM_NUMBER              1

#define DFLT_BOARD_NUMBER           0

void print_help (char *program_name)
{
    printf( "Usage: %s [options]\n"
            "\t-h This help message\n"
            "\t-v Verbose output\n"
            "\t-b <broker_endpoint> Broker endpoint\n"
            "\t-board <AMC board = [0|1|2|3|4|5]>\n"
            "\t-bpm <BPM number = [0|1]>\n", program_name);
}

int main (int argc, char *argv [])
{
    int verbose = 0;
    char *broker_endp = NULL;
    char *board_number_str = NULL;
    char *bpm_number_str = NULL;
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
        fprintf (stderr, "[client:timing]: Setting default value to BOARD number: %u\n",
                DFLT_BOARD_NUMBER);
        board_number = DFLT_BOARD_NUMBER;
    }
    else {
        board_number = strtoul (board_number_str, NULL, 10);
    }

    /* Set default bpm number */
    uint32_t bpm_number;
    if (bpm_number_str == NULL) {
        fprintf (stderr, "[client:timing]: Setting default value to BPM number: %u\n",
                DFLT_BPM_NUMBER);
        bpm_number = DFLT_BPM_NUMBER;
    }
    else {
        bpm_number = strtoul (bpm_number_str, NULL, 10);

        if (bpm_number > MAX_BPM_NUMBER) {
            fprintf (stderr, "[client:timing]: BPM number too big! Defaulting to: %u\n",
                    MAX_BPM_NUMBER);
            bpm_number = MAX_BPM_NUMBER;
        }
    }

    char service [50];
    snprintf (service, sizeof (service), "HALCS%u:DEVIO:LNLS_AFC_TIMING%u",
            board_number, bpm_number);

    halcs_client_t *halcs_client = halcs_client_new (broker_endp, verbose, NULL);
    if (halcs_client == NULL) {
        fprintf (stderr, "[client:timing]: halcs_client could be created\n");
        goto err_halcs_client_new;
    }

    uint32_t wdt = 12500000;
    uint32_t en = 1;
    uint32_t evt = 1;
    uint32_t src = 0;
    uint32_t pulses = 3;
    halcs_client_err_e err;

    err = afc_timing_set_evren (halcs_client, service, 1);

    err |= halcs_set_afc_timing_fmc1_wdt     (halcs_client, service, 0, wdt);
    err |= halcs_set_afc_timing_fmc1_src     (halcs_client, service, 0, src);
    err |= halcs_set_afc_timing_fmc1_evt     (halcs_client, service, 0, evt);
    err |= halcs_set_afc_timing_fmc1_pulses  (halcs_client, service, 0, pulses);
    err |= halcs_set_afc_timing_fmc1_en      (halcs_client, service, 0, en);

    err |= halcs_set_afc_timing_fmc1_wdt     (halcs_client, service, 1, wdt);
    err |= halcs_set_afc_timing_fmc1_src     (halcs_client, service, 1, src);
    err |= halcs_set_afc_timing_fmc1_evt     (halcs_client, service, 1, evt);
    err |= halcs_set_afc_timing_fmc1_pulses  (halcs_client, service, 1, pulses);
    err |= halcs_set_afc_timing_fmc1_en      (halcs_client, service, 1, en);

    err |= halcs_set_afc_timing_fmc1_wdt     (halcs_client, service, 2, wdt);
    err |= halcs_set_afc_timing_fmc1_src     (halcs_client, service, 2, src);
    err |= halcs_set_afc_timing_fmc1_evt     (halcs_client, service, 2, evt);
    err |= halcs_set_afc_timing_fmc1_pulses  (halcs_client, service, 2, pulses);
    err |= halcs_set_afc_timing_fmc1_en      (halcs_client, service, 2, en);

    err |= halcs_set_afc_timing_fmc1_wdt     (halcs_client, service, 3, wdt);
    err |= halcs_set_afc_timing_fmc1_src     (halcs_client, service, 3, src);
    err |= halcs_set_afc_timing_fmc1_evt     (halcs_client, service, 3, evt);
    err |= halcs_set_afc_timing_fmc1_pulses  (halcs_client, service, 3, pulses);
    err |= halcs_set_afc_timing_fmc1_en      (halcs_client, service, 3, en);

    err |= halcs_set_afc_timing_fmc1_wdt     (halcs_client, service, 4, wdt);
    err |= halcs_set_afc_timing_fmc1_src     (halcs_client, service, 4, src);
    err |= halcs_set_afc_timing_fmc1_evt     (halcs_client, service, 4, evt);
    err |= halcs_set_afc_timing_fmc1_pulses  (halcs_client, service, 4, pulses);
    err |= halcs_set_afc_timing_fmc1_en      (halcs_client, service, 4, en);

    err |= halcs_set_afc_timing_fmc2_wdt     (halcs_client, service, 0, wdt);
    err |= halcs_set_afc_timing_fmc2_src     (halcs_client, service, 0, src);
    err |= halcs_set_afc_timing_fmc2_evt     (halcs_client, service, 0, evt);
    err |= halcs_set_afc_timing_fmc2_pulses  (halcs_client, service, 0, pulses);
    err |= halcs_set_afc_timing_fmc2_en      (halcs_client, service, 0, en);

    err |= halcs_set_afc_timing_fmc2_wdt     (halcs_client, service, 1, wdt);
    err |= halcs_set_afc_timing_fmc2_src     (halcs_client, service, 1, src);
    err |= halcs_set_afc_timing_fmc2_evt     (halcs_client, service, 1, evt);
    err |= halcs_set_afc_timing_fmc2_pulses  (halcs_client, service, 1, pulses);
    err |= halcs_set_afc_timing_fmc2_en      (halcs_client, service, 1, en);

    err |= halcs_set_afc_timing_fmc2_wdt     (halcs_client, service, 2, wdt);
    err |= halcs_set_afc_timing_fmc2_src     (halcs_client, service, 2, src);
    err |= halcs_set_afc_timing_fmc2_evt     (halcs_client, service, 2, evt);
    err |= halcs_set_afc_timing_fmc2_pulses  (halcs_client, service, 2, pulses);
    err |= halcs_set_afc_timing_fmc2_en      (halcs_client, service, 2, en);

    err |= halcs_set_afc_timing_fmc2_wdt     (halcs_client, service, 3, wdt);
    err |= halcs_set_afc_timing_fmc2_src     (halcs_client, service, 3, src);
    err |= halcs_set_afc_timing_fmc2_evt     (halcs_client, service, 3, evt);
    err |= halcs_set_afc_timing_fmc2_pulses  (halcs_client, service, 3, pulses);
    err |= halcs_set_afc_timing_fmc2_en      (halcs_client, service, 3, en);

    err |= halcs_set_afc_timing_fmc2_wdt     (halcs_client, service, 4, wdt);
    err |= halcs_set_afc_timing_fmc2_src     (halcs_client, service, 4, src);
    err |= halcs_set_afc_timing_fmc2_evt     (halcs_client, service, 4, evt);
    err |= halcs_set_afc_timing_fmc2_pulses  (halcs_client, service, 4, pulses);
    err |= halcs_set_afc_timing_fmc2_en      (halcs_client, service, 4, en);

    if (err != HALCS_CLIENT_SUCCESS){
        fprintf (stderr, "[client:timing]: afc_timing_set_evt_code_X failed\n");
        goto err_afc_timing;
    }

err_halcs_client_new:
err_afc_timing:
    halcs_client_destroy (&halcs_client);
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
