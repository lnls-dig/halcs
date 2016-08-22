/*
 *  * Simple example demonstrating the communication between
 *   * a client and the FPGA device
 *    */

#include <czmq.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <halcs_client.h>

#define DFLT_BIND_FOLDER            "/tmp/halcs"

#define DFLT_HALCS_NUMBER             0
#define MAX_HALCS_NUMBER              1

#define DFLT_BOARD_NUMBER           0

void print_help (char *program_name)
{
    printf( "Usage: %s [options]\n"
            "\t-h This help message\n"
            "\t-v Verbose output\n"
            "\t-board <AMC board = [0|1|2|3|4|5]>\n"
            "\t-halcs <HALCS number = [0|1]>\n"
            "\t-b <broker_endpoint> Broker endpoint\n"
            , program_name);
}

int main (int argc, char *argv [])
{
    int verbose = 0;
    char *broker_endp = NULL;
    char *board_number_str = NULL;
    char *halcs_number_str = NULL;
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
        else if (streq(argv[i], "-board"))
        {
            str_p = &board_number_str;
        }
        else if (streq(argv[i], "-halcs"))
        {
            str_p = &halcs_number_str;
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
        fprintf (stderr, "[client:afc_diag]: Setting default value to BOARD number: %u\n",
                DFLT_BOARD_NUMBER);
        board_number = DFLT_BOARD_NUMBER;
    }
    else {
        board_number = strtoul (board_number_str, NULL, 10);
    }

    /* Set default halcs number */
    uint32_t halcs_number;
    if (halcs_number_str == NULL) {
        fprintf (stderr, "[client:afc_diag]: Setting default value to HALCS number: %u\n",
                DFLT_HALCS_NUMBER);
        halcs_number = DFLT_HALCS_NUMBER;
    }
    else {
        halcs_number = strtoul (halcs_number_str, NULL, 10);

        if (halcs_number > MAX_HALCS_NUMBER) {
            fprintf (stderr, "[client:afc_diag]: HALCS number too big! Defaulting to: %u\n",
                    MAX_HALCS_NUMBER);
            halcs_number = MAX_HALCS_NUMBER;
        }
    }

    /* Generate the service names for each SMIO */
    char service_afc_diag[50];
    snprintf (service_afc_diag, sizeof (service_afc_diag), "HALCS%u:DEVIO:AFC_DIAG0",
            board_number);

    halcs_client_t *halcs_client = halcs_client_new (broker_endp, verbose, NULL);
    if (halcs_client == NULL) {
        fprintf (stderr, "[client:acq]: halcs_client could be created\n");
        goto err_halcs_client_new;
    }

    uint32_t data;
    halcs_client_err_e err = halcs_get_afc_diag_card_slot (halcs_client, service_afc_diag, &data);
    if (err != HALCS_CLIENT_SUCCESS){
        fprintf (stderr, "[client:afc_diag]: halcs_get_afc_diag_card_slot failed\n");
        goto err_halcs_get;
    }
    fprintf (stdout, "[client:afc_diag]: Card slot: 0x%08X\n", data);

    err = halcs_get_afc_diag_ipmi_addr (halcs_client, service_afc_diag, &data);
    if (err != HALCS_CLIENT_SUCCESS){
        fprintf (stderr, "[client:afc_diag]: halcs_get_afc_diag_ipmi_addr failed\n");
        goto err_halcs_get;
    }
    fprintf (stdout, "[client:afc_diag]: IPMI addr: 0x%08X\n", data);

    smio_afc_diag_revision_data_t rev_data;
    err = halcs_get_afc_diag_build_date (halcs_client, service_afc_diag, &rev_data);
    if (err != HALCS_CLIENT_SUCCESS){
        fprintf (stderr, "[client:afc_diag]: halcs_get_afc_diag_build_user_date failed\n");
        goto err_halcs_get;
    }
    fprintf (stdout, "[client:afc_diag]: Build date: %s\n", (char *) rev_data.data);

    err = halcs_get_afc_diag_build_user_name (halcs_client, service_afc_diag, &rev_data);
    if (err != HALCS_CLIENT_SUCCESS){
        fprintf (stderr, "[client:afc_diag]: halcs_get_afc_diag_build_user_name failed\n");
        goto err_halcs_get;
    }
    fprintf (stdout, "[client:afc_diag]: Build user name: %s\n", (char *) rev_data.data);

    err = halcs_get_afc_diag_build_user_email (halcs_client, service_afc_diag, &rev_data);
    if (err != HALCS_CLIENT_SUCCESS){
        fprintf (stderr, "[client:afc_diag]: halcs_get_afc_diag_build_user_email failed\n");
        goto err_halcs_get;
    }
    fprintf (stdout, "[client:afc_diag]: Build user email: %s\n", (char *) rev_data.data);

err_halcs_client_new:
err_halcs_get:
    halcs_client_destroy (&halcs_client);
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
