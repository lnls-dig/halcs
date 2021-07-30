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
    {"num_gts",             required_argument,   NULL, 'g'},
    {"firmware_ver",        no_argument,         NULL, 'c'},
    {"link_partners",       no_argument,         NULL, 'l'},
    {"soft_err_cnt",        no_argument,         NULL, 'm'},
    {"hard_err_cnt",        no_argument,         NULL, 'n'},
    {"frame_err_cnt",       no_argument,         NULL, 'j'},
    {"rx_pck_cnt",          no_argument,         NULL, 'r'},
    {"tx_pck_cnt",          no_argument,         NULL, 'x'},
    {"fod_process_time",    no_argument,         NULL, 'y'},
    {"bpm_cnt",             no_argument,         NULL, 'k'},
    {"timeframe_cnt",       no_argument,         NULL, 'w'},
    {NULL, 0, NULL, 0}
};

static const char* shortopt = "hb:vo:s:t:f:e:g:clrxykwmnj";

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
            "  -g  --num_gts <Number of GTs>        Number of GTs\n"
            "  -c  --firmware_ver                   Firmware version\n"
            "  -l  --link_partners                  Link partners ID\n"
            "  -m  --soft_err_cnt                   Number of soft errors\n"
            "  -n  --hard_err_cnt                   Number of hard errors\n"
            "  -j  --frame_err_cnt                  Number os frame errors\n"
            "  -r  --rx_pck_cnt                     RX packet count\n"
            "  -x  --tx_pck_cnt                     TX packet count\n"
            "  -y  --fod_process_time               FOD process time [in userclk cycles]\n"
            "  -k  --bpm_cnt                        BPM count in current timeframe\n"
            "  -w  --timeframe_cnt                  Total number of timeframes\n",
            program_name);
}

#define FUNC_FOFB_CTRL_LINK_PARTNER_IDX         0
#define FUNC_FOFB_CTRL_SOFT_ERR_CNT_IDX         1
#define FUNC_FOFB_CTRL_HARD_ERR_CNT_IDX         2
#define FUNC_FOFB_CTRL_FRAME_ERR_CNT_IDX        3
#define FUNC_FOFB_CTRL_RX_PCK_CNT_IDX           4
#define FUNC_FOFB_CTRL_TX_PCK_CNT_IDX           5
#define FUNC_FOFB_CTRL_END_IDX                  6

#define MAX_NUM_FUNCS                           FUNC_FOFB_CTRL_END_IDX

/* Our read function */
typedef halcs_client_err_e (*halcs_func_fp) (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t *value);

typedef struct {
    halcs_func_fp func;
    const char *func_name;
    int call;
} func_call_t;

int main (int argc, char *argv [])
{
    int ret = 0;
    int verbose = 0;
    int firmware_ver = 0;
    int fod_process_time = 0;
    int bpm_cnt = 0;
    int timeframe_cnt = 0;
    char *broker_endp = NULL;
    char *board_number_str = NULL;
    char *halcs_number_str = NULL;
    char *bpm_id_str = NULL;
    char *time_frame_len_str = NULL;
    char *cc_enable_str = NULL;
    char *num_gts_str = NULL;
    func_call_t func_call [MAX_NUM_FUNCS] = {{0}};
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

            case 'g':
                num_gts_str = strdup (optarg);
                break;

            case 'c':
                firmware_ver = 1;
                break;

            case 'l':
                func_call [FUNC_FOFB_CTRL_LINK_PARTNER_IDX].func = 
                    &halcs_get_fofb_ctrl_link_partner;
                func_call [FUNC_FOFB_CTRL_LINK_PARTNER_IDX].func_name = "link_partner";
                func_call [FUNC_FOFB_CTRL_LINK_PARTNER_IDX].call = 1;
                break;

            case 'm':
                func_call [FUNC_FOFB_CTRL_SOFT_ERR_CNT_IDX].func = 
                    &halcs_get_fofb_ctrl_soft_err_cnt;
                func_call [FUNC_FOFB_CTRL_SOFT_ERR_CNT_IDX].func_name = "soft_err_cnt";
                func_call [FUNC_FOFB_CTRL_SOFT_ERR_CNT_IDX].call = 1;
                break;

            case 'n':
                func_call [FUNC_FOFB_CTRL_HARD_ERR_CNT_IDX].func = 
                    &halcs_get_fofb_ctrl_hard_err_cnt;
                func_call [FUNC_FOFB_CTRL_HARD_ERR_CNT_IDX].func_name = "hard_err_cnt";
                func_call [FUNC_FOFB_CTRL_HARD_ERR_CNT_IDX].call = 1;
                break;

            case 'j':
                func_call [FUNC_FOFB_CTRL_FRAME_ERR_CNT_IDX].func = 
                    &halcs_get_fofb_ctrl_frame_err_cnt;
                func_call [FUNC_FOFB_CTRL_FRAME_ERR_CNT_IDX].func_name = "frame_err_cnt";
                func_call [FUNC_FOFB_CTRL_FRAME_ERR_CNT_IDX].call = 1;
                break;

            case 'r':
                func_call [FUNC_FOFB_CTRL_RX_PCK_CNT_IDX].func = 
                    &halcs_get_fofb_ctrl_rx_pck_cnt;
                func_call [FUNC_FOFB_CTRL_RX_PCK_CNT_IDX].func_name = "rx_pck_cnt";
                func_call [FUNC_FOFB_CTRL_RX_PCK_CNT_IDX].call = 1;
                break;

            case 'x':
                func_call [FUNC_FOFB_CTRL_TX_PCK_CNT_IDX].func = 
                    &halcs_get_fofb_ctrl_tx_pck_cnt;
                func_call [FUNC_FOFB_CTRL_TX_PCK_CNT_IDX].func_name = "tx_pck_cnt";
                func_call [FUNC_FOFB_CTRL_TX_PCK_CNT_IDX].call = 1;
                break;

            case 'y':
                fod_process_time = 1;
                break;

            case 'k':
                bpm_cnt = 1;
                break;

            case 'w':
                timeframe_cnt = 1;
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

    uint32_t num_gts = 0;
    if (num_gts_str == NULL) {
        fprintf (stderr, "[client:fofb_ctrl]: num_gts option not set\n");
        print_help (argv[0]);
        goto err_no_num_gts;
    }
    else {
        num_gts = strtoul (num_gts_str, NULL, 10);
    }

    char service[50];
    snprintf (service, sizeof (service), "HALCS%u:DEVIO:FOFB_CTRL%u", board_number, halcs_number);

    halcs_client_t *halcs_client = halcs_client_new (broker_endp, verbose, NULL);
    if (halcs_client == NULL) {
        fprintf (stderr, "[client:fofb_ctrl]: halcs_client could be created\n");
        ret = 1;
        goto err_halcs_client_new;
    }

    halcs_client_err_e err;
    uint32_t bpm_id = 0;
    if (bpm_id_str != NULL) {
        bpm_id = strtoul (bpm_id_str, NULL, 10);

        err = halcs_set_fofb_ctrl_bpm_id (halcs_client, service, bpm_id);
        if (err != HALCS_CLIENT_SUCCESS){
            fprintf (stderr, "[client:fofb_ctrl]: halcs_set_bpm_id failed\n");
            ret = 2;
            goto err_halcs_exit;
        }

        err = halcs_get_fofb_ctrl_bpm_id_rdback (halcs_client, service, &bpm_id);
        if (err != HALCS_CLIENT_SUCCESS){
            fprintf (stderr, "[client:fofb_ctrl]: halcs_get_bpm_id_rdback failed\n");
            ret = 2;
            goto err_halcs_exit;
        }

        if (verbose) {
            fprintf (stdout, "[client:fofb_ctrl]: bpm_id_rdback = 0x%08X\n", bpm_id);
        }
        else {
            fprintf (stdout, "0x%08X\n", bpm_id);
        }
    }


    uint32_t time_frame_len = 0;
    if (time_frame_len_str != NULL) {
        time_frame_len = strtoul (time_frame_len_str, NULL, 10);

        err = halcs_set_fofb_ctrl_time_frame_len (halcs_client, service, time_frame_len);
        if (err != HALCS_CLIENT_SUCCESS){
            fprintf (stderr, "[client:fofb_ctrl]: halcs_set_time_frame_len failed\n");
            ret = 2;
            goto err_halcs_exit;
        }
    }

    uint32_t cc_enable = 0;
    if (cc_enable_str != NULL) {
        cc_enable = strtoul (cc_enable_str, NULL, 10);
        if (cc_enable != 0 && cc_enable != 1) {
            fprintf (stderr, "[client:fofb_ctrl]: Invalid cc_enable argument\n");
            ret = 2;
            goto err_halcs_exit;
        }

        err = halcs_set_fofb_ctrl_cc_enable (halcs_client, service, cc_enable);
        if (err != HALCS_CLIENT_SUCCESS){
            fprintf (stderr, "[client:fofb_ctrl]: halcs_set_cc_enable failed\n");
            ret = 2;
            goto err_halcs_exit;
        }
    }

    /* At the end of all register set we need to call acq_part funcion
     * to trigger FOFB ram readout by the device */
    err = halcs_set_fofb_ctrl_act_part (halcs_client, service, 0x1);
    if (err != HALCS_CLIENT_SUCCESS){
        fprintf (stderr, "[client:fofb_ctrl]: halcs_set_fofb_ctrl_act_part failed\n");
        ret = 2;
        goto err_halcs_exit;
    }

    uint32_t firmware_ver_get = 0;
    if (firmware_ver) {
        err = halcs_get_fofb_ctrl_firmware_ver (halcs_client, service, &firmware_ver_get);
        if (err != HALCS_CLIENT_SUCCESS){
            fprintf (stderr, "[client:fofb_ctrl]: halcs_get_firmware_ver failed\n");
            ret = 2;
            goto err_halcs_exit;
        }

        if (verbose) {
            fprintf (stdout, "[client:fofb_ctrl]: firmware_ver = 0x%08X\n", firmware_ver_get);
        }
        else {
            fprintf (stdout, "0x%08X\n", firmware_ver_get);
        }
    }

    for (uint32_t func = 0; func < MAX_NUM_FUNCS; ++func) {
        if (func_call[func].call == 1) {
            uint32_t param_get[num_gts];
            err = HALCS_CLIENT_SUCCESS;

            for (uint32_t i = 0; i < num_gts; ++i) {
                err |= func_call[func].func (halcs_client, service, i, &param_get[i]);
            }

            if (err != HALCS_CLIENT_SUCCESS){
                fprintf (stderr, "[client:fofb_ctrl]: %s failed\n", func_call[func].func_name);
                ret = 2;
                goto err_halcs_exit;
            }

            for (uint32_t i = 0; i < num_gts; ++i) {
                if (verbose) {
                    fprintf (stdout, "[client:fofb_ctrl]: %s, chan %u = %u\n", func_call[func].func_name,
                        i+1, param_get[i]);
                }
                else {
                    fprintf (stdout, "%u", param_get[i]);

                    if (i < num_gts-1) {
                        fprintf (stdout, ",");
                    }
                    else {
                        fprintf (stdout, "\n");
                    }
                }
            }
        }
    }

    uint32_t fod_process_time_get = 0;
    if (fod_process_time) {
        err = halcs_get_fofb_ctrl_fod_process_time (halcs_client, service, &fod_process_time_get);
        if (err != HALCS_CLIENT_SUCCESS){
            fprintf (stderr, "[client:fofb_ctrl]: halcs_get_fod_process_time failed\n");
            ret = 2;
            goto err_halcs_exit;
        }

        if (verbose) {
            fprintf (stdout, "[client:fofb_ctrl]: fod_process_time = %u usrclk cycles\n", fod_process_time_get);
        }
        else {
            fprintf (stdout, "%u\n", fod_process_time_get);
        }
    }

    uint32_t bpm_cnt_get = 0;
    if (bpm_cnt) {
        err = halcs_get_fofb_ctrl_bpm_count (halcs_client, service, &bpm_cnt_get);
        if (err != HALCS_CLIENT_SUCCESS){
            fprintf (stderr, "[client:fofb_ctrl]: halcs_get_bpm_count failed\n");
            ret = 2;
            goto err_halcs_exit;
        }

        if (verbose) {
            fprintf (stdout, "[client:fofb_ctrl]: bpm_cnt = %u\n", bpm_cnt_get);
        }
        else {
            fprintf (stdout, "%u\n", bpm_cnt_get);
        }
    }

    uint32_t timeframe_cnt_get = 0;
    if (timeframe_cnt) {
        err = halcs_get_fofb_ctrl_time_frame_count (halcs_client, service, &timeframe_cnt_get);
        if (err != HALCS_CLIENT_SUCCESS){
            fprintf (stderr, "[client:fofb_ctrl]: halcs_get_time_frame_count failed\n");
            ret = 2;
            goto err_halcs_exit;
        }

        if (verbose) {
            fprintf (stdout, "[client:fofb_ctrl]: timeframe_cnt = %u\n", timeframe_cnt_get);
        }
        else {
            fprintf (stdout, "%u\n", timeframe_cnt_get);
        }
    }

err_halcs_exit:
    /* Try to read up until the point where the error occurs, anyway */
    halcs_set_fofb_ctrl_act_part (halcs_client, service, 0x1);
    halcs_client_destroy (&halcs_client);
err_halcs_client_new:
err_no_num_gts:
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

    return ret;
}
