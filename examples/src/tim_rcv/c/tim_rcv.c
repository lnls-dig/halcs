/*
 * Timing receiver core client example
 */

#include <getopt.h>
#include <czmq.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <halcs_client.h>

#define DFLT_BIND_FOLDER            "/tmp/halcs"

#define DFLT_HALCS_NUMBER           0
#define DFLT_BOARD_NUMBER           0

#define FUNC_TIM_RCV_PHASE_MEAS_NAVG_IDX            0
#define FUNC_TIM_RCV_DMTD_A_DEGLITCHER_THRES_IDX    1
#define FUNC_TIM_RCV_DMTD_B_DEGLITCHER_THRES_IDX    2
#define FUNC_TIM_RCV_PHASE_MEAS_IDX                 3
#define FUNC_TIM_RCV_DMTD_A_FREQ_IDX                4
#define FUNC_TIM_RCV_DMTD_A_VALID_IDX               5
#define FUNC_TIM_RCV_DMTD_B_FREQ_IDX                6
#define FUNC_TIM_RCV_DMTD_B_VALID_IDX               7
#define FUNC_TIM_RCV_END                            8

#define MAX_NUM_FUNCS                               FUNC_TIM_RCV_END

typedef halcs_client_err_e (*halcs_func_fp) (halcs_client_t *self, char *service,
        uint32_t value);

static const halcs_func_fp halcs_tim_rcv_funcs [MAX_NUM_FUNCS] = {
    &halcs_set_phase_meas_navg,
    &halcs_set_dmtd_a_deglitcher_thres,
    &halcs_set_dmtd_b_deglitcher_thres,
    &halcs_set_phase_meas,
    &halcs_set_dmtd_a_freq,
    &halcs_set_dmtd_a_valid,
    &halcs_set_dmtd_b_freq,
    &halcs_set_dmtd_b_valid
};

static struct option long_options[] =
{
    {"help",                no_argument,         NULL, 'h'},
    {"brokerendp",          required_argument,   NULL, 'b'},
    {"verbose",             no_argument,         NULL, 'v'},
    {"halcsnumber",         required_argument,   NULL, 's'},
    {"boardslot",           required_argument,   NULL, 'o'},
    {"phasemeasnavg",       required_argument,   NULL, 'p'},
    {"dmtdadeglitch",       required_argument,   NULL, 'a'},
    {"dmtdbdeglitch",       required_argument,   NULL, 'r'},
    {NULL, 0, NULL, 0}
};

static const char* shortopt = "hb:vo:s:p:a:r:";

void print_help (char *program_name)
{
    fprintf (stdout, "HALCSD Acquisition Utility\n"
            "Usage: %s [options]\n"
            "\n"
            "  -h  --help                           Display this usage information\n"
            "  -b  --brokerendp <Broker endpoint>   Broker endpoint\n"
            "  -v  --verbose                        Verbose output\n"
            "  -o  --boardslot <Board slot number = [1-12]> \n"
            "  -s  --halcsnumber <HALCS number = [0|1]> HALCS number\n"
            "                                       Board slot number\n"
            "  -p  --phasemeasnavg <Number of phase measurement averages = [0-(2^32-1)]> \n"
            "  -a  --dmtdadeglitch <Number of DMTD A deglitcher threshold = [0-(2^16-1)]> \n"
            "  -r  --dmtdbdeglitch <Number of DMTD B deglitcher threshold = [0-(2^16-1)]> \n",
            program_name);
}

typedef struct _func_call_t {
    int call;
    char *arg;
} func_call_t;

int main (int argc, char *argv [])
{
    int verbose = 0;
    char *broker_endp = NULL;
    char *num_samples_str = NULL;
    char *board_number_str = NULL;
    char *halcs_number_str = NULL;
    int opt;
    func_call_t func_call [MAX_NUM_FUNCS] = {{0}};

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

            case 'p':
                func_call [FUNC_TIM_RCV_PHASE_MEAS_NAVG_IDX].call = 1;
                func_call [FUNC_TIM_RCV_PHASE_MEAS_NAVG_IDX].arg = strdup (optarg);
                break;

            case 'a':
                func_call [FUNC_TIM_RCV_DMTD_A_DEGLITCHER_THRES_IDX].call = 1;
                func_call [FUNC_TIM_RCV_DMTD_A_DEGLITCHER_THRES_IDX].arg = strdup (optarg);
                break;

            case 'r':
                func_call [FUNC_TIM_RCV_DMTD_B_DEGLITCHER_THRES_IDX].call = 1;
                func_call [FUNC_TIM_RCV_DMTD_B_DEGLITCHER_THRES_IDX].arg = strdup (optarg);
                break;

            case '?':
                fprintf (stderr, "[client:acq] Option not recognized or missing argument\n");
                print_help (argv [0]);
                exit (1);
                break;

            default:
                fprintf (stderr, "[client:acq] Could not parse options\n");
                print_help (argv [0]);
                exit (1);
         }
    }

    /* Set default broker address */
    if (broker_endp == NULL) {
        fprintf (stderr, "[client:acq]: Setting default broker endpoint: %s\n",
                "ipc://"DFLT_BIND_FOLDER);
        broker_endp = strdup ("ipc://"DFLT_BIND_FOLDER);
    }

    /* Set default board number */
    uint32_t board_number;
    if (board_number_str == NULL) {
        fprintf (stderr, "[client:acq]: Setting default value to BOARD number: %u\n",
                DFLT_BOARD_NUMBER);
        board_number = DFLT_BOARD_NUMBER;
    }
    else {
        board_number = strtoul (board_number_str, NULL, 10);
    }

    /* Set default halcs number */
    uint32_t halcs_number;
    if (halcs_number_str == NULL) {
        fprintf (stderr, "[client:acq]: Setting default value to HALCS number: %u\n",
                DFLT_HALCS_NUMBER);
        halcs_number = DFLT_HALCS_NUMBER;
    }
    else {
        halcs_number = strtoul (halcs_number_str, NULL, 10);
    }

    char service[50];
    snprintf (service, sizeof (service), "HALCS%u:DEVIO:TIM_RCV%u", board_number, halcs_number);

    halcs_client_t *halcs_client = halcs_client_new (broker_endp, verbose, NULL);
    if (halcs_client == NULL) {
        fprintf (stderr, "[client:acq]: halcs_client could not be created\n");
        goto err_halcs_client_new;
    }

    uint32_t i;
    for (i = 0; i < MAX_NUM_FUNCS; ++i) {
        if (func_call [i].call == 1) {
            fprintf (stdout, "[client:tim_rcv_ctl]: Calling function #%d\n", i);
            fprintf (stdout, "[client:tim_rcv_ctl]: Argument: %lu\n", strtoul (func_call [i].arg, NULL, 10));
            halcs_client_err_e err = halcs_tim_rcv_funcs [i]
                (halcs_client, service, strtoul (func_call [i].arg, NULL, 10));

            if (err != HALCS_CLIENT_SUCCESS) {
                fprintf (stderr, "[client:tim_rcv_ctl]: Error executing remote "
                        "function: %s\n", halcs_client_err_str (err));
            }
        }
    }

    uint32_t value = 0;
    halcs_client_err_e err = halcs_get_phase_meas (halcs_client, service, &value);
    if (err == HALCS_CLIENT_SUCCESS) {
        fprintf (stderr, "[client:tim_rcv_ctl]: Phase Meas: %u\n", value);
    }
    else {
        fprintf (stderr, "[client:tim_rcv_ctl]: Phase Meas Error: %s\n", halcs_client_err_str (err));
        goto halcs_err;
    }

    err = halcs_get_dmtd_a_freq (halcs_client, service, &value);
    if (err == HALCS_CLIENT_SUCCESS) {
        fprintf (stderr, "[client:tim_rcv_ctl]: DMTD A Freq: %u\n", value);
    }
    else {
        fprintf (stderr, "[client:tim_rcv_ctl]: DMTD A Freq Error: %s\n", halcs_client_err_str (err));
        goto halcs_err;
    }

    err = halcs_get_dmtd_b_freq (halcs_client, service, &value);
    if (err == HALCS_CLIENT_SUCCESS) {
        fprintf (stderr, "[client:tim_rcv_ctl]: DMTD B Freq: %u\n", value);
    }
    else {
        fprintf (stderr, "[client:tim_rcv_ctl]: DMTD B Freq Error: %s\n", halcs_client_err_str (err));
        goto halcs_err;
    }

halcs_err:
err_halcs_client_new:
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
