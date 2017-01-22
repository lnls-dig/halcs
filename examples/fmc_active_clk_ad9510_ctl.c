#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <halcs_client.h>

#define DFLT_BIND_FOLDER            "/tmp/halcs"

#define DFLT_HALCS_NUMBER                     0
#define MAX_HALCS_NUMBER                      1

#define DFLT_BOARD_NUMBER                   0
#define DFLT_AD9510_RW                      0
#define MAX_RW_PARAM                        1

void print_help (char *program_name)
{
    printf( "Usage: %s [options]\n"
            "\t-h This help message\n"
            "\t-v Verbose output\n"
            "\t-board <AMC board = [0|1|2|3|4|5]>\n"
            "\t-halcs <HALCS number = [0|1]>\n"
            "\t-b <broker_endpoint> Broker endpoint\n"
            "\t-rw <Read/Write = 0 - Read, 1 - Write>\n"
            "\t-addr <AD9510 Address>\n"
            "\t-data <AD9510 Data>\n"
            , program_name);
}

int main (int argc, char *argv [])
{
    int verbose = 0;
    char *broker_endp = NULL;
    char *board_number_str = NULL;
    char *halcs_number_str = NULL;
    char *ad9510_addr_str = NULL;
    char *ad9510_data_str = NULL;
    char *ad9510_rw_str = 0;
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
        else if (streq (argv[i], "-b"))
        {
            str_p = &broker_endp;
        }
        else if (streq(argv[i], "-rw"))
        {
            str_p = &ad9510_rw_str;
        }
        else if (streq (argv[i], "-addr")) {
            str_p = &ad9510_addr_str;
        }
        else if (streq (argv[i], "-data")) {
            str_p = &ad9510_data_str;
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
        fprintf (stderr, "[client:fmc_active_clk_ad9510_ctl]: Setting default value to BOARD number: %u\n",
                DFLT_BOARD_NUMBER);
        board_number = DFLT_BOARD_NUMBER;
    }
    else {
        board_number = strtoul (board_number_str, NULL, 10);
    }

    /* Set default halcs number */
    uint32_t halcs_number;
    if (halcs_number_str == NULL) {
        fprintf (stderr, "[client:fmc_active_clk_ad9510_ctl]: Setting default value to HALCS number: %u\n",
                DFLT_HALCS_NUMBER);
        halcs_number = DFLT_HALCS_NUMBER;
    }
    else {
        halcs_number = strtoul (halcs_number_str, NULL, 10);

        if (halcs_number > MAX_HALCS_NUMBER) {
            fprintf (stderr, "[client:fmc_active_clk_ad9510_ctl]: HALCS number too big! Defaulting to: %u\n",
                    MAX_HALCS_NUMBER);
            halcs_number = MAX_HALCS_NUMBER;
        }
    }

    /* Set default board number */
    uint32_t ad9510_rw = 0;
    if (ad9510_rw_str == NULL) {
        fprintf (stderr, "[client:fmc_active_clk_ad9510_ctl]: Setting default value to RW parameter: %u\n",
                DFLT_AD9510_RW);
        ad9510_rw = DFLT_AD9510_RW;
    }
    else {
        ad9510_rw = strtoul (ad9510_rw_str, NULL, 10);
        if (ad9510_rw > MAX_RW_PARAM) {
            fprintf (stderr, "[client:fmc_active_clk_ad9510_ctl]: Invalid RW parameter! Options are 0 or 1\n");
            return 0;
        }
    }

    /* AD9510 addr/data */
    uint32_t ad9510_data = 0;
    if (ad9510_data_str == NULL) {
        if (ad9510_rw) {
            fprintf (stderr, "[client:fmc130m_ad9510_ctl]: Missing \"-data\" argument\n");
            return 0;
        }
    }
    else {
        //ad9510_data = strtoul (ad9510_data_str, NULL, 10);
        if (sscanf (ad9510_data_str, "%i", &ad9510_data) != 1) {
            fprintf (stderr, "[client:fmc130m_ad9510_ctl]: Invalid data parameter\n");
            return 0;
        }
    }

    uint32_t ad9510_addr = 0;
    if (ad9510_addr_str == NULL) {
        fprintf (stderr, "[client:fmc130m_ad9510_ctl]: Missing \"-addr\" argument\n");
        return 0;
    }
    else {
        //ad9510_addr = strtoul (ad9510_addr_str, NULL, 10);
        if (sscanf (ad9510_addr_str, "%i", &ad9510_addr) != 1) {
            fprintf (stderr, "[client:fmc130m_ad9510_ctl]: Invalid addr parameter\n");
            return 0;
        }
    }

    /* Generate the service names for each SMIO */
    char service[50];
    snprintf (service, sizeof (service), "HALCS%u:DEVIO:FMC_ACTIVE_CLK%u", board_number, halcs_number);

    halcs_client_t *halcs_client = halcs_client_new (broker_endp, verbose, NULL);
    halcs_client_err_e err = HALCS_CLIENT_SUCCESS;

    uint32_t ad9510_data_read = 0;
    /* Write data */
    if (ad9510_rw) {
        err = halcs_set_ad9510_data (halcs_client, service, ad9510_addr, ad9510_data);
        if (err != HALCS_CLIENT_SUCCESS){
            fprintf (stderr, "[client:fmc_active_clk_ad9510_ctl]: halcs_set_ad9510_data failed\n");
            goto err_halcs_exit;
        }
        fprintf (stdout, "addr = 0x%02X, data = 0x%02X written\n", ad9510_addr, ad9510_data);
    }
    /* Read data */
    else {
        err = halcs_get_ad9510_data (halcs_client, service, ad9510_addr, &ad9510_data_read);
        if (err != HALCS_CLIENT_SUCCESS){
            fprintf (stderr, "[client:fmc_active_clk_ad9510_ctl]: halcs_get_ad9510_data failed\n");
            goto err_halcs_exit;
        }
        fprintf (stdout, "addr = 0x%02X, data = 0x%02X read\n", ad9510_addr, ad9510_data_read);
    }
#if 0
    err = halcs_set_fmc_clk_sel (halcs_client, service, 0x1);
    (void) ad9510_data_read;
    (void) err;
    (void) ad9510_data;
    (void) ad9510_addr;

#endif
err_halcs_exit:
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
