/*
 *   Simple example demonstrating the communication between
 *   a client and the FPGA device
 *  */

#include <getopt.h>
#include <czmq.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <halcs_client.h>

#define DFLT_BIND_FOLDER            "/tmp/halcs"

#define DFLT_BOARD_NUMBER           0

#define DFLT_CHAN_NUM               0

static struct option long_options[] =
{
    {"help",                no_argument,         NULL, 'h'},
    {"brokerendp",          required_argument,   NULL, 'b'},
    {"verbose",             no_argument,         NULL, 'v'},
    {"halcsnumber",         required_argument,   NULL, 's'},
    {"boardslot",           required_argument,   NULL, 'o'},
    {"channumber",          required_argument,   NULL, 'c'},
    {"status",              no_argument,         NULL, 'y'},
    {"amp_iflag_l",         no_argument,         NULL, 'r'},
    {"amp_tflag_l",         no_argument,         NULL, 'p'},
    {"amp_iflag_r",         no_argument,         NULL, 'a'},
    {"amp_tflag_r",         no_argument,         NULL, 't'},
    {"amp_en",              required_argument,   NULL, 'u'},
    {"mode",                required_argument,   NULL, 'q'},
    {"pi_kp",               required_argument,   NULL, 'k'},
    {"pi_ti",               required_argument,   NULL, 'z'},
    {"pi_sp",               required_argument,   NULL, 'j'},
    {"dac_data",            required_argument,   NULL, 'd'},
    {"lim_a",               required_argument,   NULL, 'g'},
    {"lim_b",               required_argument,   NULL, 'n'},
    {"cnt",                 required_argument,   NULL, 'f'},
    {"eff_adc",             required_argument,   NULL, 'm'},
    {"eff_dac",             required_argument,   NULL, 'e'},
    {"eff_sp",              required_argument,   NULL, 'l'},
    {NULL, 0, NULL, 0}
};

extern disp_op_t rtmlamp_ohwr_pi_kp_exp;
extern disp_op_t rtmlamp_ohwr_pi_ti_exp;
extern disp_op_t rtmlamp_ohwr_pi_sp_exp;
extern disp_op_t rtmlamp_ohwr_lim_a_exp;
extern disp_op_t rtmlamp_ohwr_lim_b_exp;

static const char* shortopt = "hb:vo:s:c:yrpatu:q:k:z:j:d:g:n:f:mel";

void print_help (char *program_name)
{
    fprintf (stdout, "HALCSD RTMLAMP_OHWR Utility\n"
            "Usage: %s [options]\n"
            "\n"
            "  -h  --help                           Display this usage information\n"
            "  -b  --brokerendp <Broker endpoint>   Broker endpoint\n"
            "  -v  --verbose                        Verbose output\n"
            "  -o  --boardslot <Board slot number = [1-12]> \n"
	    "  -s  --halcsnumber <HALCS number = [0|1]> HALCS number\n"
            "                                       Board slot number\n"
            "  -c  --channumber <Channel | Channel Range>\n"
            "                                       Channel number\n"
            "  -y  --status                         Get RTM LAMP Status register\n"
            "  -r  --amp_iflag_l                    Amplifier left overcurrent flag \n"
            "  -p  --amp_tflag_l                    Amplifier left overtemperature flag \n"
            "  -a  --amp_iflag_r                    Amplifier right overcurrent flag \n"
            "  -t  --amp_tflag_r                    Amplifier right overtemperature flag \n"
            "  -u  --amp_en <[0 = disable, 1 = enable>\n"
            "                                       Amplifier enable (must set --channnumber)\n"

            "  -q  --mode <[0 = OL_MANUAL , 1 = OL_TEST_SQR, 2 = CL_MANUAL, 3 = CL_TEST_SQR, 4 = CL_FOFB>\n"
            "                                       Power supply mode (must set --channnumber)\n"
            "  -k  --pi_kp                          PI KP parameter\n"
            "  -z  --pi_ti                          PI TI parameter\n"
            "  -j  --pi_sp                          PI setpoint (also used for PI test modes)\n"
            "  -d  --cnt                            Square wave test period in 10 ns steps\n"
            "  -g  --dac_data                       DAC to be sent to DAC\n" 
            "  -n  --lim_a                          PI setpoint inferior limit (for PI test modes)\n"
            "  -f  --lim_b                          PI setpoint inferior limit (for PI test modes)\n"
            "  -m  --eff_adc                        ADC instantaneous measurement\n" 
            "  -e  --eff_dac                        DAC instantaneous effective data\n" 
            "  -l  --eff_sp                         Set point instantaneous effective data\n",
            program_name);
}

int main (int argc, char *argv [])
{
    int verbose = 0;
    char *broker_endp = NULL;
    char *board_number_str = NULL;
    char *halcs_number_str = NULL;
    char *chan_str = NULL;
    int chan_sel = 0;
    int status = 0;
    int amp_iflag_l_sel = 0;
    int amp_tflag_l_sel = 0;
    int amp_iflag_r_sel = 0;
    int amp_tflag_r_sel = 0;
    char *amp_en_str = NULL;
    char *mode_str =  NULL;
    char *pi_kp_str = NULL;
    char *pi_ti_str = NULL;
    char *pi_sp_str = NULL;
    char *dac_data_str = NULL;
    char *lim_a_str = NULL;
    char *lim_b_str = NULL; 
    char *cnt_str = NULL;
    char *eff_adc_str = NULL;
    char *eff_dac_str = NULL;
    char *eff_sp_str = NULL;
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

            case 'c':
                chan_str = strdup (optarg);
                chan_sel = 1;
                break;

            case 'y':
                status = 1;
                break;

            case 'r':
                amp_iflag_l_sel = 1;
                break;

            case 'p':
                amp_tflag_l_sel = 1;
                break;

            case 'a':
                amp_iflag_r_sel = 1;
                break;

            case 't':
                amp_iflag_r_sel = 1;
                break;

            case 'u':
                amp_en_str = strdup (optarg);
                break;

            case 'q':
                mode_str = strdup (optarg);
                break;

            case 'k':
                pi_kp_str = strdup (optarg);
                break;

            case 'z':
                pi_ti_str = strdup (optarg);
                break;

            case 'j':
                pi_sp_str = strdup (optarg);
                break;

            case 'd':
                dac_data_str = strdup (optarg);
                break;

            case 'g':
                lim_a_str = strdup (optarg);
                break;

            case 'n':
                lim_b_str = strdup (optarg);
                break;

            case 'f':
                cnt_str = strdup (optarg);
                break;

            case 'm':
                eff_adc_str = strdup (optarg);
                break;

            case 'e':
                eff_dac_str = strdup (optarg);
                break;

            case 'l':
                eff_sp_str = strdup (optarg);
                break;

            case '?':
                fprintf (stderr, "[client:rtmlamp_ohwr] Option not recognized or missing argument\n");
                print_help (argv [0]);
                exit (1);
                break;

            default:
                fprintf (stderr, "[client:rtmlamp_ohwr] Could not parse options\n");
                print_help (argv [0]);
                exit (1);
         }
    }

    /* If we want to change a channel property, we must select a channel first */
    if ((amp_iflag_l_sel == 1 ||
         amp_tflag_l_sel == 1 ||
         amp_iflag_r_sel == 1 ||
         amp_iflag_r_sel == 1 ||
         amp_en_str != NULL ||
         dac_data_str != NULL) &&
             chan_sel == 0) {
        fprintf (stderr, "[client:rtmlamp_ohwr]: Channel number not selected (use -c or --channumber option)\n");
        exit (1);
    }

    /* Set default broker address */
    if (broker_endp == NULL) {
        fprintf (stderr, "[client:rtmlamp_ohwr]: Setting default broker endpoint: %s\n",
                "ipc://"DFLT_BIND_FOLDER);
        broker_endp = strdup ("ipc://"DFLT_BIND_FOLDER);
    }

    /* Set default channel */
    uint32_t chan_min;
    uint32_t chan_max;
    if (chan_str == NULL) {
        fprintf (stderr, "[client:rtmlamp_ohwr]: Setting default value to 'chan'\n");
        chan_min = DFLT_CHAN_NUM;
        chan_max = chan_min+1;
    }
    else {
        if (sscanf (chan_str, "[%u:%u]", &chan_min, &chan_max) != 2) {
            if (sscanf (chan_str, "%u", &chan_min) != 1) {
                fprintf (stderr, "[client:rtmlamp_ohwr]: Unexpected channel format\n");
                exit (1);
            }
            chan_max = chan_min+1;
        }
        else {
            chan_max++;
        }

        if (chan_max < chan_min) {
            fprintf (stderr, "[client:rtmlamp_ohwr]: Channel range must be ascending\n");
            exit (1);
        }
    }

    /* Set default board number */
    uint32_t board_number;
    if (board_number_str == NULL) {
        fprintf (stderr, "[client:rtmlamp_ohwr]: Setting default value to BOARD number: %u\n",
                DFLT_BOARD_NUMBER);
        board_number = DFLT_BOARD_NUMBER;
    }
    else {
        board_number = strtoul (board_number_str, NULL, 10);
    }

    /* Set halcs number */
    uint32_t halcs_number=0;
    if (halcs_number_str == NULL) {
        fprintf (stderr, "[client:rtmlamp_ohwr]: No halcs instance specified. Please use --halcsnumber option\n");
        goto err_halcs_set;
    }
    else {
        halcs_number = strtoul (halcs_number_str, NULL, 10);
    }

    char service[50];
    snprintf (service, sizeof (service), "HALCS%u:DEVIO:RTMLAMP_OHWR%u", board_number, halcs_number);

    halcs_client_t *halcs_client = halcs_client_new (broker_endp, verbose, NULL);
    if (halcs_client == NULL) {
        fprintf (stderr, "[client:rtmlamp_ohwr]: halcs_client could be created\n");
        goto err_halcs_client_new;
    }

    if (status == 1) {
        uint32_t arg = 0;
        halcs_client_err_e err = halcs_get_rtmlamp_ohwr_sta (halcs_client, service, &arg);
        if (err != HALCS_CLIENT_SUCCESS){
            fprintf (stderr, "[client:rtmlamp_ohwr]: halcs_get_rtmlamp_ohwr_sta failed\n");
            goto err_halcs_set;
        }
        printf ("[client:rtmlamp_ohwr]: sta: 0x%08X\n", arg);
    }

    uint32_t chan;
    for (chan = chan_min; chan < chan_max; ++chan) {
        if (amp_iflag_l_sel == 1) {
            uint32_t amp_iflag_l = 0;
            halcs_client_err_e err = halcs_get_rtmlamp_ohwr_amp_iflag_l (halcs_client, service, chan, &amp_iflag_l);
            if (err != HALCS_CLIENT_SUCCESS){
                fprintf (stderr, "[client:rtmlamp_ohwr]: halcs_get_rtmlamp_ohwr_amp_iflag_l failed\n");
                goto err_halcs_set;
            }
            printf ("[client:rtmlamp_ohwr]: amp_iflag_l: 0x%08X\n", amp_iflag_l);
        }

        if (amp_tflag_l_sel == 1) {
            uint32_t amp_tflag_l = 0;
            halcs_client_err_e err = halcs_get_rtmlamp_ohwr_amp_tflag_l (halcs_client, service, chan, &amp_tflag_l);
            if (err != HALCS_CLIENT_SUCCESS){
                fprintf (stderr, "[client:rtmlamp_ohwr]: halcs_get_rtmlamp_ohwr_amp_tflag_l failed\n");
                goto err_halcs_set;
            }
            printf ("[client:rtmlamp_ohwr]: amp_tflag_l: 0x%08X\n", amp_tflag_l);
        }

        if (amp_iflag_r_sel == 1) {
            uint32_t amp_iflag_r = 0;
            halcs_client_err_e err = halcs_get_rtmlamp_ohwr_amp_iflag_r (halcs_client, service, chan, &amp_iflag_r);
            if (err != HALCS_CLIENT_SUCCESS){
                fprintf (stderr, "[client:rtmlamp_ohwr]: halcs_get_rtmlamp_ohwr_amp_iflag_r failed\n");
                goto err_halcs_set;
            }
            printf ("[client:rtmlamp_ohwr]: amp_iflag_r: 0x%08X\n", amp_iflag_r);
        }

        if (amp_tflag_r_sel == 1) {
            uint32_t amp_tflag_r = 0;
            halcs_client_err_e err = halcs_get_rtmlamp_ohwr_amp_tflag_r (halcs_client, service, chan, &amp_tflag_r);
            if (err != HALCS_CLIENT_SUCCESS){
                fprintf (stderr, "[client:rtmlamp_ohwr]: halcs_get_rtmlamp_ohwr_amp_tflag_r failed\n");
                goto err_halcs_set;
            }
            printf ("[client:rtmlamp_ohwr]: amp_tflag_r: 0x%08X\n", amp_tflag_r);
        }

        uint32_t amp_en = 0;
        if (amp_en_str != NULL) {
            amp_en = strtoul (amp_en_str, NULL, 10);
            halcs_client_err_e err = halcs_set_rtmlamp_ohwr_amp_en (halcs_client, service, chan, amp_en);
            if (err != HALCS_CLIENT_SUCCESS){
                fprintf (stderr, "[client:rtmlamp_ohwr]: halcs_set_rtmlamp_ohwr_amp_en failed\n");
                goto err_halcs_set;
            }

            uint32_t arg = 0;
            halcs_get_rtmlamp_ohwr_amp_en (halcs_client, service, chan, &arg);
            printf ("[client:rtmlamp_ohwr]: halcs_set_rtmlamp_ohwr_amp_en: 0x%08X\n", arg);
        }

        uint32_t mode = 0;
        if (mode_str != NULL) {
            mode = strtoul (mode_str, NULL, 10);
            halcs_client_err_e err = halcs_set_rtmlamp_ohwr_mode (halcs_client, service, chan, mode);
            if (err != HALCS_CLIENT_SUCCESS){
                fprintf (stderr, "[client:rtmlamp_ohwr]: halcs_set_rtmlamp_ohwr_mode failed\n");
                goto err_halcs_set;
            }

            uint32_t arg = 0;
            halcs_get_rtmlamp_ohwr_mode (halcs_client, service, chan, &arg);
            printf ("[client:rtmlamp_ohwr]: halcs_set_rtmlamp_ohwr_mode: 0x%08X\n", arg);
        }

        if (pi_kp_str != NULL) {
            uint32_t pi_kp = strtoul (pi_kp_str, NULL, 10);
            halcs_client_err_e err = halcs_set_rtmlamp_ohwr_pi_kp (halcs_client, service, chan, pi_kp);
            if (err != HALCS_CLIENT_SUCCESS){
                fprintf (stderr, "[client:rtmlamp_ohwr]: halcs_set_rtmlamp_ohwr_pi_kp failed\n");
                goto err_halcs_set;
            }

            uint32_t arg = 0;
            halcs_get_rtmlamp_ohwr_pi_kp (halcs_client, service, chan, &arg);
            printf ("[client:rtmlamp_ohwr]: pi_kp: 0x%08x\n", arg);
        }

        if (pi_ti_str != NULL) {
            uint32_t pi_ti = strtoul (pi_ti_str, NULL, 10);
            halcs_client_err_e err = halcs_set_rtmlamp_ohwr_pi_ti (halcs_client, service, chan, pi_ti);
            if (err != HALCS_CLIENT_SUCCESS){
                fprintf (stderr, "[client:rtmlamp_ohwr]: halcs_set_rtmlamp_ohwr_pi_ti failed\n");
                goto err_halcs_set;
            }

            uint32_t arg = 0;
            halcs_get_rtmlamp_ohwr_pi_ti (halcs_client, service, chan, &arg);
            printf ("[client:rtmlamp_ohwr]: pi_ti: 0x%08x\n", arg);
        }

        if (pi_sp_str != NULL) {
            uint32_t pi_sp = strtoul (pi_sp_str, NULL, 10);
            halcs_client_err_e err = halcs_set_rtmlamp_ohwr_pi_sp (halcs_client, service, chan, pi_sp);
            if (err != HALCS_CLIENT_SUCCESS){
                fprintf (stderr, "[client:rtmlamp_ohwr]: halcs_set_rtmlamp_ohwr_pi_sp failed\n");
                goto err_halcs_set;
            }

            uint32_t arg = 0;
            halcs_get_rtmlamp_ohwr_pi_sp (halcs_client, service, chan, &arg);
            printf ("[client:rtmlamp_ohwr]: pi_sp: 0x%08x\n", arg);
        }


        uint32_t dac_data = 0;
        if (dac_data_str != NULL) {
            dac_data = strtoul (dac_data_str, NULL, 10);
            halcs_client_err_e err = halcs_set_rtmlamp_ohwr_dac_data (halcs_client, service, chan, dac_data);
            if (err != HALCS_CLIENT_SUCCESS){
                fprintf (stderr, "[client:rtmlamp_ohwr]: halcs_set_rtmlamp_ohwr_dac_data failed\n");
                goto err_halcs_set;
            }

            uint32_t arg = 0;
            halcs_get_rtmlamp_ohwr_dac_data (halcs_client, service, chan, &arg);
            printf ("[client:rtmlamp_ohwr]: dac_data: 0x%08X\n", arg);
        }

        if (cnt_str != NULL) {
            uint32_t cnt = strtoul (cnt_str, NULL, 10);
            halcs_client_err_e err = halcs_set_rtmlamp_ohwr_cnt (halcs_client, service, chan, cnt);
            if (err != HALCS_CLIENT_SUCCESS){
                fprintf (stderr, "[client:rtmlamp_ohwr]: halcs_set_rtmlamp_ohwr_cnt failed\n");
                goto err_halcs_set;
            }

            uint32_t arg = 0;
            halcs_get_rtmlamp_ohwr_cnt (halcs_client, service, chan, &arg);
            printf ("[client:rtmlamp_ohwr]: cnt: 0x%08X\n", arg);
        }

        if (lim_a_str != NULL) {
            uint32_t lim_a = strtoul (lim_a_str, NULL, 10);
            halcs_client_err_e err = halcs_set_rtmlamp_ohwr_lim_a (halcs_client, service, chan, lim_a);
            if (err != HALCS_CLIENT_SUCCESS){
                fprintf (stderr, "[client:rtmlamp_ohwr]: halcs_set_rtmlamp_ohwr_lim_a failed\n");
                goto err_halcs_set;
            }

            uint32_t arg = 0;
            halcs_get_rtmlamp_ohwr_lim_a (halcs_client, service, chan, &arg);
            printf ("[client:rtmlamp_ohwr]: lim_a: 0x%08X\n", arg);
        }

        if (lim_b_str != NULL) {
            uint32_t lim_b = strtoul (lim_b_str, NULL, 10);
            halcs_client_err_e err = halcs_set_rtmlamp_ohwr_lim_b (halcs_client, service, chan, lim_b);
            if (err != HALCS_CLIENT_SUCCESS){
                fprintf (stderr, "[client:rtmlamp_ohwr]: halcs_set_rtmlamp_ohwr_lim_b failed\n");
                goto err_halcs_set;
            }

            uint32_t arg = 0;
            halcs_get_rtmlamp_ohwr_lim_b (halcs_client, service, chan, &arg);
            printf ("[client:rtmlamp_ohwr]: lim_b: 0x%08X\n", arg);
        }

        uint32_t arg = 0;
        halcs_client_err_e err = halcs_get_rtmlamp_ohwr_eff_adc (halcs_client, service, chan, &arg);
        if (err != HALCS_CLIENT_SUCCESS){
            fprintf (stderr, "[client:rtmlamp_ohwr]: halcs_get_rtmlamp_ohwr_eff_adc failed\n");
            goto err_halcs_set;
        }
        printf ("[client:rtmlamp_ohwr]: halcs_set_rtmlamp_ohwr_eff_adc: 0x%08X\n", arg);

        arg = 0;
        err = halcs_get_rtmlamp_ohwr_eff_dac (halcs_client, service, chan, &arg);
        if (err != HALCS_CLIENT_SUCCESS){
            fprintf (stderr, "[client:rtmlamp_ohwr]: halcs_get_rtmlamp_ohwr_eff_dac failed\n");
            goto err_halcs_set;
        }
        printf ("[client:rtmlamp_ohwr]: halcs_set_rtmlamp_ohwr_eff_dac: 0x%08X\n", arg);

        arg = 0;
        err = halcs_get_rtmlamp_ohwr_eff_sp (halcs_client, service, chan, &arg);
        if (err != HALCS_CLIENT_SUCCESS){
            fprintf (stderr, "[client:rtmlamp_ohwr]: halcs_get_rtmlamp_ohwr_eff_sp failed\n");
            goto err_halcs_set;
        }
        printf ("[client:rtmlamp_ohwr]: halcs_set_rtmlamp_ohwr_eff_sp: 0x%08X\n", arg);

    }

err_halcs_set:
err_halcs_client_new:
    free (dac_data_str);
    dac_data_str = NULL;
    free (lim_a_str);
    lim_a_str = NULL;
    free (lim_b_str);
    lim_b_str = NULL;
    free (cnt_str);
    cnt_str = NULL;
    free (pi_sp_str);
    pi_sp_str = NULL;
    free (pi_ti_str);
    pi_ti_str = NULL;
    free (pi_kp_str);
    pi_kp_str = NULL;
    free (mode_str);
    mode_str = NULL;
    free (amp_en_str);
    amp_en_str = NULL;
    free (chan_str);
    chan_str = NULL;
    free (board_number_str);
    board_number_str = NULL;
    free (halcs_number_str);
    halcs_number_str = NULL;
    free (broker_endp);
    broker_endp = NULL;
    halcs_client_destroy (&halcs_client);

    return 0;
}
