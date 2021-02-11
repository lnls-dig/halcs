/*
 *   Simple example demonstrating the communication between
 *   a client and the FPGA device
 *
 *  */

#include <getopt.h>
#include <czmq.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <halcs_client.h>

#define DFLT_BIND_FOLDER            "/tmp/halcs"

#define DFLT_HALCS_NUMBER             0
#define MAX_HALCS_NUMBER              1

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
    {"rcvsrc",              required_argument,   NULL, 'r'},
    {"rcvsel",              required_argument,   NULL, 'p'},
    {"rcvcount",            no_argument,         NULL, 'a'},
    {"transmsrc",           required_argument,   NULL, 't'},
    {"transsel",            required_argument,   NULL, 'u'},
    {"transmcount",         no_argument,         NULL, 'e'},
    {"rcvlen",              required_argument,   NULL, 'l'},
    {"trnlen",              required_argument,   NULL, 'm'},
    {"dir",                 required_argument,   NULL, 'd'},
    {"dirpol",              required_argument,   NULL, 'x'},
    {NULL, 0, NULL, 0}
};

static const char* shortopt = "hb:vo:s:c:r:p:at:u:el:m:d:x:";

void print_help (char *program_name)
{
    fprintf (stdout, "HALCSD Trigger Utility\n"
            "Usage: %s [options]\n"
            "\n"
            "  -h  --help                           Display this usage information\n"
            "  -b  --brokerendp <Broker endpoint>   Broker endpoint\n"
            "  -v  --verbose                        Verbose output\n"
            "  -o  --boardslot <Board slot number = [1-12]> \n"
	    "  -s  --halcsnumber <HALCS number = [0|1]> HALCS number\n"
            "                                       Board slot number\n"
            "  -c  --channumber <Channel | Channel Range>\n"
            "                                     Trigger Channel number\n"
            "                                     <Channel> must be one of the following:\n"
            "                                     0 -> ADC; 1 -> ADC_SWAP; 2 -> Mixer IQ120; 3 -> Mixer IQ340;\n"
            "                                     4 -> TBT Decim IQ120; 5 -> TBT Decim IQ340; 6 -> TBT Amp;\n"
            "                                     7 -> TBT Phase; 8 -> TBT Pos; 9 -> FOFB Decim IQ120;\n"
            "                                     10 -> FOFB Decim IQ340; 11 -> FOFB Amp; 12 -> FOFB Pha;\n"
            "                                     13 -> FOFB Pos; 14 -> Monit Amp; 15 -> Monit Pha; 16 -> Monit Pos]\n"
            "                                     17 -> Swithcing Clock]\n"
            "  -r  --rcvsrc <Receive source for the selected channel = [0 = trigger backplane, 1 = FPGA internal]> \n"
            "  -p  --rcvsel <Receive selection source for the selected channel = [TBD]> \n"
            "  -a  --rcvcount                     Receiver pulse counter\n"
            "  -t  --transmsrc <Transmit source for the selected channel = [0 = trigger backplane, 1 = FPGA internal]> \n"
            "  -u  --transmsel <Transmit selection source for the selected channel = [TBD]> \n"
            "  -e  --transmcount                  Transmitter pulse counter\n"
            "  -l  --rcvlen <Receive debounce length> \n"

            "  -m  --trnlen <Transmit extension length> \n"
            "  -d  --dir <Trigger direction = [0 = FPGA Output, 1 = FPGA Input]> \n"
            "  -x  --dirpol <Trigger direction polarity = [0 = Polarity unchanged, 1 = Polarity reversed]> \n",
            program_name);
}

int main (int argc, char *argv [])
{
    int verbose = 0;
    char *broker_endp = NULL;
    char *num_samples_str = NULL;
    char *board_number_str = NULL;
    char *halcs_number_str = NULL;
    char *chan_str = NULL;
    int chan_sel = 0;
    char *rcvsrc_str = NULL;
    int rcvsrc_sel = 0;
    char *rcvsel_str = NULL;
    int rcvsel_sel = 0;
    int rcvcount = 0;
    char *transmsrc_str = NULL;
    int transmsrc_sel = 0;
    char *transmsel_str = NULL;
    int transmsel_sel = 0;
    char *rcvlen_str = NULL;
    int rcvlen_sel = 0;
    char *trnlen_str = NULL;
    int trnlen_sel = 0;
    int transmcount = 0;
    char *dir_str = NULL;
    int dir_sel = 0;
    char *dirpol_str = NULL;
    int dirpol_sel = 0;
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

            case 'r':
                rcvsrc_str = strdup (optarg);
                rcvsrc_sel = 1;
                break;

            case 'p':
                rcvsel_str = strdup (optarg);
                rcvsel_sel = 1;
                break;

            case 'a':
                rcvcount = 1;
                break;

            case 't':
                transmsrc_str = strdup (optarg);
                transmsrc_sel = 1;
                break;

            case 'u':
                transmsel_str = strdup (optarg);
                transmsel_sel = 1;
                break;

            case 'e':
                transmcount = 1;
                break;

            case 'l':
                rcvlen_str = strdup (optarg);
                rcvlen_sel = 1;
                break;

            case 'm':
                trnlen_str = strdup (optarg);
                trnlen_sel = 1;
                break;

            case 'd':
                dir_str = strdup (optarg);
                dir_sel = 1;
                break;

            case 'x':
                dirpol_str = strdup (optarg);
                dirpol_sel = 1;
                break;

            case '?':
                fprintf (stderr, "[client:trigger] Option not recognized or missing argument\n");
                print_help (argv [0]);
                exit (1);
                break;

            default:
                fprintf (stderr, "[client:trigger] Could not parse options\n");
                print_help (argv [0]);
                exit (1);
         }
    }

    /* If we want to change a channel property, we must select a channel first */
    if ((rcvsrc_sel == 1 || rcvsel_sel == 1 || transmsrc_sel == 1 || transmsel_sel == 1 ||
        dir_sel == 1 || dirpol_sel == 1) && chan_sel == 0) {
        fprintf (stderr, "[client:trigger]: Channel number not selected (use -c or --channumber option)\n");
        exit (1);
    }

    /* Set default broker address */
    if (broker_endp == NULL) {
        fprintf (stderr, "[client:trigger]: Setting default broker endpoint: %s\n",
                "ipc://"DFLT_BIND_FOLDER);
        broker_endp = strdup ("ipc://"DFLT_BIND_FOLDER);
    }

    /* Set default channel */
    uint32_t chan_min;
    uint32_t chan_max;
    if (chan_str == NULL) {
        fprintf (stderr, "[client:trigger]: Setting default value to 'chan'\n");
        chan_min = DFLT_CHAN_NUM;
        chan_max = chan_min+1;
    }
    else {
        if (sscanf (chan_str, "[%u:%u]", &chan_min, &chan_max) != 2) {
            if (sscanf (chan_str, "%u", &chan_min) != 1) {
                fprintf (stderr, "[client:trigger]: Unexpected channel format\n");
                exit (1);
            }   
            chan_max = chan_min+1;
        }
        else {
            chan_max++;
        }
   
        if (chan_max < chan_min) {
            fprintf (stderr, "[client:trigger]: Channel range must be ascending\n");
            exit (1);
        }
    }

    /* Set default board number */
    uint32_t board_number;
    if (board_number_str == NULL) {
        fprintf (stderr, "[client:trigger]: Setting default value to BOARD number: %u\n",
                DFLT_BOARD_NUMBER);
        board_number = DFLT_BOARD_NUMBER;
    }
    else {
        board_number = strtoul (board_number_str, NULL, 10);
    }

    /* Set default halcs number */
    uint32_t halcs_number;
    if (halcs_number_str == NULL) {
        fprintf (stderr, "[client:trigger]: Setting default value to HALCS number: %u\n",
                DFLT_HALCS_NUMBER);
        halcs_number = DFLT_HALCS_NUMBER;
    }
    else {
        halcs_number = strtoul (halcs_number_str, NULL, 10);

        if (halcs_number > MAX_HALCS_NUMBER) {
            fprintf (stderr, "[client:trigger]: HALCS number too big! Defaulting to: %u\n",
                    MAX_HALCS_NUMBER);
            halcs_number = MAX_HALCS_NUMBER;
        }
    }

    char service_iface[50];
    snprintf (service_iface, sizeof (service_iface), "HALCS%u:DEVIO:TRIGGER_IFACE0", board_number);
    char service_mux[50];
    snprintf (service_mux, sizeof (service_mux), "HALCS%u:DEVIO:TRIGGER_MUX%u", board_number, halcs_number);

    halcs_client_t *halcs_client = halcs_client_new (broker_endp, verbose, NULL);
    if (halcs_client == NULL) {
        fprintf (stderr, "[client:trigger]: halcs_client could be created\n");
        goto err_halcs_client_new;
    }

    uint32_t chan;
    for (chan = chan_min; chan < chan_max; ++chan) {
        uint32_t rcvsrc = 0;
        if (rcvsrc_sel == 1) {
            rcvsrc = strtoul (rcvsrc_str, NULL, 10);
            halcs_client_err_e err = halcs_set_trigger_rcv_src (halcs_client, service_mux, chan, rcvsrc);
            if (err != HALCS_CLIENT_SUCCESS){
                fprintf (stderr, "[client:trigger]: halcs_set_trigger_rcv_src failed\n");
                goto err_halcs_set;
            }
        }

        uint32_t rcvsel = 0;
        if (rcvsel_sel == 1) {
            rcvsel = strtoul (rcvsel_str, NULL, 10);
            halcs_client_err_e err = halcs_set_trigger_rcv_in_sel (halcs_client, service_mux, chan, rcvsel);
            if (err != HALCS_CLIENT_SUCCESS){
                fprintf (stderr, "[client:trigger]: halcs_set_trigger_rcv_sel failed\n");
                goto err_halcs_set;
            }
        }

        if (rcvcount == 1) {
            uint32_t arg = 0;
            halcs_client_err_e err = halcs_get_trigger_count_rcv (halcs_client, service_iface, chan, &arg);

            if (err != HALCS_CLIENT_SUCCESS){
                fprintf (stderr, "[client:trigger]: halcs_get_trigger_count_rcv failed\n");
                goto err_halcs_set;
            }
            printf ("[client:trigger]: halcs_get_trigger_count_rcv: %u\n", arg);
        }

        uint32_t transmsrc = 0;
        if (transmsrc_sel == 1) {
            transmsrc = strtoul (transmsrc_str, NULL, 10);
            halcs_client_err_e err = halcs_set_trigger_transm_src (halcs_client, service_mux, chan, transmsrc);
            if (err != HALCS_CLIENT_SUCCESS){
                fprintf (stderr, "[client:trigger]: halcs_set_trigger_transm_sel failed\n");
                goto err_halcs_set;
            }
        }

        uint32_t transmsel = 0;
        if (transmsel_sel == 1) {
            transmsel = strtoul (transmsel_str, NULL, 10);
            halcs_client_err_e err = halcs_set_trigger_transm_out_sel (halcs_client, service_mux, chan, transmsel);
            if (err != HALCS_CLIENT_SUCCESS){
                fprintf (stderr, "[client:trigger]: halcs_set_trigger_transm_sel failed\n");
                goto err_halcs_set;
            }
        }

        if (transmcount == 1) {
            uint32_t arg = 0;
            halcs_client_err_e err = halcs_get_trigger_count_transm (halcs_client, service_iface, chan, &arg);

            if (err != HALCS_CLIENT_SUCCESS){
                fprintf (stderr, "[client:trigger]: halcs_get_trigger_count_transm failed\n");
                goto err_halcs_set;
            }
            printf ("[client:trigger]: halcs_get_trigger_count_transm: %u\n", arg);
        }

        uint32_t rcvlen = 0;
        if (rcvlen_sel == 1) {
            rcvlen = strtoul (rcvlen_str, NULL, 10);
            halcs_client_err_e err = halcs_set_trigger_rcv_len (halcs_client, service_iface, chan, rcvlen);
            if (err != HALCS_CLIENT_SUCCESS){
                fprintf (stderr, "[client:trigger]: halcs_set_trigger_rcv_len failed\n");
                goto err_halcs_set;
            }
        }

        uint32_t trnlen = 0;
        if (trnlen_sel == 1) {
            trnlen = strtoul (trnlen_str, NULL, 10);
            halcs_client_err_e err = halcs_set_trigger_transm_len (halcs_client, service_iface, chan, trnlen);
            if (err != HALCS_CLIENT_SUCCESS){
                fprintf (stderr, "[client:trigger]: halcs_set_trigger_transm_len failed\n");
                goto err_halcs_set;
            }
        }

        uint32_t dir = 0;
        if (dir_sel == 1) {
            dir = strtoul (dir_str, NULL, 10);
            halcs_client_err_e err = halcs_set_trigger_dir (halcs_client, service_iface, chan, dir);
            if (err != HALCS_CLIENT_SUCCESS){
                fprintf (stderr, "[client:trigger]: halcs_set_trigger_dir failed\n");
                goto err_halcs_set;
            }
        }

        uint32_t dirpol = 0;
        if (dirpol_sel == 1) {
            dirpol = strtoul (dirpol_str, NULL, 10);
            halcs_client_err_e err = halcs_set_trigger_dir_pol (halcs_client, service_iface, chan, dirpol);
            if (err != HALCS_CLIENT_SUCCESS){
                fprintf (stderr, "[client:trigger]: halcs_set_trigger_dir_pol failed\n");
                goto err_halcs_set;
            }
        }

        /* Read all parameters from this channel */
        uint32_t arg = 0;
        halcs_get_trigger_rcv_src (halcs_client, service_mux, chan, &arg);
        fprintf (stderr, "[client:trigger]: halcs_get_trigger_rcv_src: %u\n", arg);
        halcs_get_trigger_rcv_in_sel (halcs_client, service_mux, chan, &arg);
        fprintf (stderr, "[client:trigger]: halcs_get_trigger_rcv_in_sel: %u\n", arg);
        halcs_get_trigger_transm_src (halcs_client, service_mux, chan, &arg);
        fprintf (stderr, "[client:trigger]: halcs_get_trigger_transm_src: %u\n", arg);
        halcs_get_trigger_transm_out_sel (halcs_client, service_mux, chan, &arg);
        fprintf (stderr, "[client:trigger]: halcs_get_trigger_transm_out_sel: %u\n", arg);
        halcs_get_trigger_rcv_len (halcs_client, service_iface, chan, &arg);
        fprintf (stderr, "[client:trigger]: halcs_get_trigger_rcv_len: %u\n", arg);
        halcs_get_trigger_transm_len (halcs_client, service_iface, chan, &arg);
        fprintf (stderr, "[client:trigger]: halcs_get_trigger_transm_len: %u\n", arg);
        halcs_get_trigger_dir (halcs_client, service_iface, chan, &arg);
        fprintf (stderr, "[client:trigger]: halcs_get_trigger_dir: %u\n", arg);
        halcs_get_trigger_dir_pol (halcs_client, service_iface, chan, &arg);
        fprintf (stderr, "[client:trigger]: halcs_get_trigger_dir_pol: %u\n", arg);
    }

err_halcs_set:
err_halcs_client_new:
    free (dirpol_str);
    dirpol_str = NULL;
    free (dir_str);
    dir_str = NULL;
    free (trnlen_str);
    trnlen_str = NULL;
    free (rcvlen_str);
    rcvlen_str = NULL;
    free (transmsel_str);
    transmsel_str = NULL;
    free (transmsrc_str);
    transmsrc_str = NULL;
    free (rcvsel_str);
    rcvsel_str = NULL;
    free (rcvsrc_str);
    rcvsrc_str = NULL;
    free (chan_str);
    chan_str = NULL;
    free (board_number_str);
    board_number_str = NULL;
    free (halcs_number_str);
    halcs_number_str = NULL;
    free (num_samples_str);
    num_samples_str = NULL;
    free (broker_endp);
    broker_endp = NULL;
    halcs_client_destroy (&halcs_client);

    return 0;
}
