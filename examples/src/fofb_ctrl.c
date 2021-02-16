/*
 * Example for demonstrating the communication between a
 * a FOFB Ctrl module and a client  
 */

#include <getopt.h>
#include <czmq.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <halcs_client.h>

#define DFLT_BIND_FOLDER            "/tmp/malamute"

#define DFLT_HALCS_NUMBER           0
#define DFLT_BOARD_NUMBER           0

#define MAX_CHANNEL_NUM             4
#define ALL_CHANNELS_NUM            4

static struct option long_options[] =
{
    {"help",                no_argument,         NULL, 'h'},
    {"brokerendp",          required_argument,   NULL, 'b'},
    {"verbose",             no_argument,         NULL, 'v'},
    {"halcsnumber",         required_argument,   NULL, 's'},
    {"boardslot",           required_argument,   NULL, 'o'},
    {"bpm_id",              required_argument,   NULL, 't'},
    {"time_frame_len",      required_argument,   NULL, 'f'},
    {"cc_enable",           required_argument,   NULL, 'e'},
    {"firmware_ver",        no_argument,         NULL, 'c'},
    {"link_partners",       no_argument,         NULL, 'l'},
    {"rx_pck_cnt",          no_argument,         NULL, 'r'},
    {"tx_pck_cnt",          no_argument,         NULL, 'x'},
    {NULL, 0, NULL, 0}
};

static const char* shortopt = "hb:vo:s:t:f:e:clrx";

void print_help (char *program_name)
{
    fprintf (stdout, "HALCSD FOFB Controller control utility\n"
            "Usage: %s [options]\n"
            "\n"
            "  -h  --help                           Display this usage information\n"
            "  -b  --brokerendp <Broker endpoint>   Broker endpoint\n"
            "  -v  --verbose                        Verbose output\n"
            "  -o  --boardslot <Board slot number = [1-12]> \n"
            "  -s  --halcsnumber <HALCS number = [0|1]> HALCS number\n"
            "                                       Board slot number\n"
            "  -t  --bpm_id <ID=[0-512]>            BPM ID\n"
            "  -f  --time_frame_len <Length in clock cycles>\n"
            "                                       Timeframe length\n"
            "  -e  --cc_enable <Enable[0|1]>        CC enable\n"
            "  -c  --firmware_ver                   Firmware version\n"
            "  -l  --link_partners                  Link partners ID\n"
            "  -r  --rx_pck_cnt                     RX packet count\n"
            "  -x  --tx_pck_cnt                     TX packet count\n",
            program_name);
}

int main (int argc, char *argv [])
{
    int verbose = 0;
    int firmware_ver = 0;
    int link_partners = 0;
    int rx_pck_cnt = 0;
    int tx_pck_cnt = 0;
    char *broker_endp = NULL;
    char *board_number_str = NULL;
    char *halcs_number_str = NULL;
    char *bpm_id_str = NULL;
    char *time_frame_len_str = NULL;
    char *cc_enable_str = NULL;
    int opt;

    while ((opt = getopt_long (argc, argv, shortopt, long_options, NULL)) != -1) {
        /* Get the user selected options */
        switch (opt) {
            /* Display Help */
            case 'h':
                print_help (argv [0]);
                exit (1);
                break;

            case 'b':
                broker_endp = strdup (optarg);
                break;

            case 'v':
                verbose = 1;
                break;

            case 'o':
                board_number_str = strdup (optarg);
                break;

            case 's':
                halcs_number_str = strdup (optarg);
                break;

            case 't':
                bpm_id_str = strdup (optarg);
                break;

            case 'f':
                time_frame_len_str = strdup (optarg);
                break;

            case 'e':
                cc_enable_str = strdup (optarg);
                break;

            case 'c':
                firmware_ver = 1;
                break;

            case 'l':
                link_partners = 1;
                break;

            case 'r':
                rx_pck_cnt = 1;
                break;

            case 'x':
                tx_pck_cnt = 1;
                break;

            case '?':
                fprintf (stderr, "[client:fofb_ctrl] Option not recognized or missing argument\n");
                print_help (argv [0]);
                exit (1);
                break;

            default:
                fprintf (stderr, "[client:fofb_ctrl] Could not parse options\n");
                print_help (argv [0]);
                exit (1);
         }
    }

    /* Set default broker address */
    if (broker_endp == NULL) {
        fprintf (stderr, "[client:fofb_ctrl]: Setting default broker endpoint: %s\n",
                "ipc://"DFLT_BIND_FOLDER);
        broker_endp = strdup ("ipc://"DFLT_BIND_FOLDER);
    }

    /* Set default board number */
    uint32_t board_number;
    if (board_number_str == NULL) {
        fprintf (stderr, "[client:fofb_ctrl]: Setting default value to BOARD number: %u\n",
                DFLT_BOARD_NUMBER);
        board_number = DFLT_BOARD_NUMBER;
    }
    else {
        board_number = strtoul (board_number_str, NULL, 10);
    }

    /* Set default halcs number */
    uint32_t halcs_number;
    if (halcs_number_str == NULL) {
        fprintf (stderr, "[client:fofb_ctrl]: Setting default value to HALCS number: %u\n",
                DFLT_HALCS_NUMBER);
        halcs_number = DFLT_HALCS_NUMBER;
    }
    else {
        halcs_number = strtoul (halcs_number_str, NULL, 10);
    }

    char service[50];
    snprintf (service, sizeof (service), "HALCS%u:DEVIO:FOFB_CTRL%u", board_number, halcs_number);

    halcs_client_t *halcs_client = halcs_client_new (broker_endp, verbose, NULL);
    if (halcs_client == NULL) {
        fprintf (stderr, "[client:fofb_ctrl]: halcs_client could be created\n");
        goto err_halcs_client_new;
    }

    halcs_client_err_e err;
    uint32_t bpm_id = 0;
    if (bpm_id_str != NULL) {
        bpm_id = strtoul (bpm_id_str, NULL, 10);

        fprintf (stdout, "[client:fofb_ctrl]: bpm_id = 0x%08X\n", bpm_id);
        err = halcs_set_fofb_ctrl_bpm_id (halcs_client, service, bpm_id);
        if (err != HALCS_CLIENT_SUCCESS){
            fprintf (stderr, "[client:fofb_ctrl]: halcs_set_bpm_id failed\n");
            goto err_halcs_exit;
        }

        err = halcs_get_fofb_ctrl_bpm_id_rdback (halcs_client, service, &bpm_id);
        if (err != HALCS_CLIENT_SUCCESS){
            fprintf (stderr, "[client:fofb_ctrl]: halcs_get_bpm_id_rdback failed\n");
            goto err_halcs_exit;
        }

        fprintf (stdout, "[client:fofb_ctrl]: bpm_id_rdback = 0x%08X\n", bpm_id);
    }


    uint32_t time_frame_len = 0;
    if (time_frame_len_str != NULL) {
        time_frame_len = strtoul (time_frame_len_str, NULL, 10);

        fprintf (stdout, "[client:fofb_ctrl]: time_frame_len = 0x%08X\n", time_frame_len);
        err = halcs_set_fofb_ctrl_time_frame_len (halcs_client, service, time_frame_len);
        if (err != HALCS_CLIENT_SUCCESS){
            fprintf (stderr, "[client:fofb_ctrl]: halcs_set_time_frame_len failed\n");
            goto err_halcs_exit;
        }
    }

    uint32_t cc_enable = 0;
    if (cc_enable_str != NULL) {
        cc_enable = strtoul (cc_enable_str, NULL, 10);
        if (cc_enable != 0 && cc_enable != 1) {
            fprintf (stderr, "[client:fofb_ctrl]: Invalid cc_enable argument\n");
            goto err_halcs_exit;
        }

        fprintf (stdout, "[client:fofb_ctrl]: cc_enable = 0x%08X\n", cc_enable);
        err = halcs_set_fofb_ctrl_cc_enable (halcs_client, service, cc_enable);
        if (err != HALCS_CLIENT_SUCCESS){
            fprintf (stderr, "[client:fofb_ctrl]: halcs_set_cc_enable failed\n");
            goto err_halcs_exit;
        }
    }

    /* At the end of all register set we need to call acq_part funcion
     * to trigger FOFB ram readout by the device */
    err = halcs_set_fofb_ctrl_act_part (halcs_client, service, 0x1);
    if (err != HALCS_CLIENT_SUCCESS){
        fprintf (stderr, "[client:fofb_ctrl]: halcs_set_fofb_ctrl_act_part failed\n");
        goto err_halcs_exit;
    }

    uint32_t firmware_ver_get = 0;
    if (firmware_ver) {
        err = halcs_get_fofb_ctrl_firmware_ver (halcs_client, service, &firmware_ver_get);
        if (err != HALCS_CLIENT_SUCCESS){
            fprintf (stderr, "[client:fofb_ctrl]: halcs_get_firmware_ver failed\n");
            goto err_halcs_exit;
        }
        fprintf (stdout, "[client:fofb_ctrl]: firmware_ver = 0x%08X\n", firmware_ver_get);
    }

    uint32_t link_partners_get[4] = {0};
    if (link_partners) {
        err = halcs_get_fofb_ctrl_link_partner_1 (halcs_client, service, &link_partners_get[0]);
        err |= halcs_get_fofb_ctrl_link_partner_2 (halcs_client, service, &link_partners_get[1]);
        err |= halcs_get_fofb_ctrl_link_partner_3 (halcs_client, service, &link_partners_get[2]);
        err |= halcs_get_fofb_ctrl_link_partner_4 (halcs_client, service, &link_partners_get[3]);
        if (err != HALCS_CLIENT_SUCCESS){
            fprintf (stderr, "[client:fofb_ctrl]: halcs_get_link_partner_X failed\n");
            goto err_halcs_exit;
        }

        fprintf (stdout, "[client:fofb_ctrl]: link_partner_1 = %u\n", link_partners_get[0]);
        fprintf (stdout, "[client:fofb_ctrl]: link_partner_2 = %u\n", link_partners_get[1]);
        fprintf (stdout, "[client:fofb_ctrl]: link_partner_3 = %u\n", link_partners_get[2]);
        fprintf (stdout, "[client:fofb_ctrl]: link_partner_4 = %u\n", link_partners_get[3]);
    }

    uint32_t rx_pck_cnt_get[4] = {0};
    if (rx_pck_cnt) {
        err = halcs_get_fofb_ctrl_rx_pck_cnt_1 (halcs_client, service, &rx_pck_cnt_get[0]);
        err |= halcs_get_fofb_ctrl_rx_pck_cnt_2 (halcs_client, service, &rx_pck_cnt_get[1]);
        err |= halcs_get_fofb_ctrl_rx_pck_cnt_3 (halcs_client, service, &rx_pck_cnt_get[2]);
        err |= halcs_get_fofb_ctrl_rx_pck_cnt_4 (halcs_client, service, &rx_pck_cnt_get[3]);
        if (err != HALCS_CLIENT_SUCCESS){
            fprintf (stderr, "[client:fofb_ctrl]: halcs_get_rx_pck_cnt_X failed\n");
            goto err_halcs_exit;
        }

        fprintf (stdout, "[client:fofb_ctrl]: rx_pck_cnt_1 = %u\n", rx_pck_cnt_get[0]);
        fprintf (stdout, "[client:fofb_ctrl]: rx_pck_cnt_2 = %u\n", rx_pck_cnt_get[1]);
        fprintf (stdout, "[client:fofb_ctrl]: rx_pck_cnt_3 = %u\n", rx_pck_cnt_get[2]);
        fprintf (stdout, "[client:fofb_ctrl]: rx_pck_cnt_4 = %u\n", rx_pck_cnt_get[3]);
    }

    uint32_t tx_pck_cnt_get[4] = {0};
    if (tx_pck_cnt) {
        err = halcs_get_fofb_ctrl_tx_pck_cnt_1 (halcs_client, service, &tx_pck_cnt_get[0]);
        err |= halcs_get_fofb_ctrl_tx_pck_cnt_2 (halcs_client, service, &tx_pck_cnt_get[1]);
        err |= halcs_get_fofb_ctrl_tx_pck_cnt_3 (halcs_client, service, &tx_pck_cnt_get[2]);
        err |= halcs_get_fofb_ctrl_tx_pck_cnt_4 (halcs_client, service, &tx_pck_cnt_get[3]);
        if (err != HALCS_CLIENT_SUCCESS){
            fprintf (stderr, "[client:fofb_ctrl]: halcs_get_tx_pck_cnt_X failed\n");
            goto err_halcs_exit;
        }

        fprintf (stdout, "[client:fofb_ctrl]: tx_pck_cnt_1 = %u\n", tx_pck_cnt_get[0]);
        fprintf (stdout, "[client:fofb_ctrl]: tx_pck_cnt_2 = %u\n", tx_pck_cnt_get[1]);
        fprintf (stdout, "[client:fofb_ctrl]: tx_pck_cnt_3 = %u\n", tx_pck_cnt_get[2]);
        fprintf (stdout, "[client:fofb_ctrl]: tx_pck_cnt_4 = %u\n", tx_pck_cnt_get[3]);
    }

err_halcs_exit:
    /* Try to read up until the point where the error occurs, anyway */
    halcs_set_fofb_ctrl_act_part (halcs_client, service, 0x1);
err_halcs_client_new:
    free (cc_enable_str);
    cc_enable_str = NULL;
    free (time_frame_len_str);
    time_frame_len_str = NULL;
    free (bpm_id_str);
    bpm_id_str = NULL;
    free (board_number_str);
    board_number_str = NULL;
    free (halcs_number_str);
    halcs_number_str = NULL;
    free (broker_endp);
    broker_endp = NULL;
    halcs_client_destroy (&halcs_client);

    return 0;
}
