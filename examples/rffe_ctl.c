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

#define DFLT_BIND_FOLDER                        "/tmp/bpm"

/* Default value definitions */

#define DFLT_BPM_NUMBER                         0
#define MAX_BPM_NUMBER                          1

#define DFLT_BOARD_NUMBER                       0

#define FUNC_RFFE_SW_IDX                        0
#define FUNC_RFFE_ATT1_IDX                      1
#define FUNC_RFFE_ATT2_IDX                      2
#define FUNC_RFFE_SET_POINT1_IDX                3
#define FUNC_RFFE_SET_POINT2_IDX                4
#define FUNC_RFFE_TEMP_CONTROL_IDX              5
#define FUNC_RFFE_RESET_IDX                     6
#define FUNC_RFFE_END                           7

#define MAX_NUM_FUNCS                           FUNC_RFFE_END

/* Functions redeclaration for compatibility with double arguments */
bpm_client_err_e bpm_set_rffe_sw_double (bpm_client_t *self, char *service,
        double rffe_sw)
{
    return bpm_set_rffe_sw (self, service, (rffe_sw == 0.0) ? 0 : 1);
}

bpm_client_err_e bpm_set_rffe_temp_control_double (bpm_client_t *self, char *service,
        double rffe_temp_control)
{
    return bpm_set_rffe_temp_control (self, service,
            (rffe_temp_control == 0.0) ? 0 : 1);
}

bpm_client_err_e bpm_set_rffe_reset_double (bpm_client_t *self, char *service,
        double rffe_reset)
{
    return bpm_set_rffe_reset (self, service, (rffe_reset == 0.0) ? 0 : 1);
}

/* Our dispatch table */
typedef bpm_client_err_e (*bpm_func_fp) (bpm_client_t *self, char *service,
        double value);

static const bpm_func_fp bpm_rffe_funcs [MAX_NUM_FUNCS] = {
    &bpm_set_rffe_sw_double,            /* function "double" argument version */
    &bpm_set_rffe_att1,
    &bpm_set_rffe_att2,
    &bpm_set_rffe_set_point1,
    &bpm_set_rffe_set_point2,
    &bpm_set_rffe_temp_control_double,  /* function "double" argument version */
    &bpm_set_rffe_reset_double,         /* function "double" argument version */
};

void print_help (char *program_name)
{
    printf( "Usage: %s [options]\n"
            "\t-h This help message\n"
            "\t-v Verbose output\n"
            "\t-b <broker_endpoint> Broker endpoint\n"
            "\t-board <AMC board = [0|1|2|3|4|5]>\n"
            "\t-bpm <BPM number = [0|1]>\n"
            "\t-sw <on/off = [0|1]> Output switching\n"
            "\t-att1 <on/off = [0 - 31.5 dB / 0.5 dB step]> Attenuator #1 value\n"
            "\t-att2 <on/off = [0 - 31.5 dB / 0.5 dB step]> Attenuator #2 value\n"
#if 0
            "\t-temp1 <Temperature sensor #1 value>\n"
            "\t-temp2 <Temperature sensor #2 value>\n"
            "\t-temp3 <Temperature sensor #3 value>\n"
            "\t-temp4 <Temperature sensor #4 value>\n"
#endif
            "\t-set_point1 <set point = [ambient temp. - 60.0 oC]> Set point #1\n"
            "\t-set_point2 <set point = [ambient temp. - 60.0 oC]> Set point #2\n"
            "\t-temp_control <on/off = [0|1]> Temperature control\n"
            "\t-reset <on/off = [0|1]> Reset\n"
#if 0
            "\t-version <RFFE software version>\n"
#endif
            , program_name);
}

typedef struct _func_call_t {
    int call;
    char **arg;
} func_call_t;

int main (int argc, char *argv [])
{
    int verbose = 0;
    char *broker_endp = NULL;
    char *board_number_str = NULL;
    char *bpm_number_str = NULL;
    char *sw_str = NULL;
    char *att1_str = NULL;
    char *att2_str = NULL;
    char *set_point1_str = NULL;
    char *set_point2_str = NULL;
    char *temp_control_str = NULL;
    char *reset_str = NULL;
    func_call_t func_call [MAX_NUM_FUNCS] = {{0}};
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
        else if (streq (argv[i], "-sw")) { /* sw: switching value */
            func_call [FUNC_RFFE_SW_IDX].call = 1;
            func_call [FUNC_RFFE_SW_IDX].arg = &sw_str;
            str_p = &sw_str;
        }
        else if (streq (argv[i], "-att1")) { /* att1: attenuator value */
            func_call [FUNC_RFFE_ATT1_IDX].call = 1;
            func_call [FUNC_RFFE_ATT1_IDX].arg = &att1_str;
            str_p = &att1_str;
        }
        else if (streq (argv[i], "-att2")) { /* att2: attenuator value */
            func_call [FUNC_RFFE_ATT2_IDX].call = 1;
            func_call [FUNC_RFFE_ATT2_IDX].arg = &att2_str;
            str_p = &att2_str;
        }
        else if (streq (argv[i], "-set_point1")) { /* set_point1: temperature set point value */
            func_call [FUNC_RFFE_SET_POINT1_IDX].call = 1;
            func_call [FUNC_RFFE_SET_POINT1_IDX].arg = &set_point1_str;
            str_p = &set_point1_str;
        }
        else if (streq (argv[i], "-set_point2")) { /* set_point2: Temperature set value */
            func_call [FUNC_RFFE_SET_POINT2_IDX].call = 1;
            func_call [FUNC_RFFE_SET_POINT2_IDX].arg = &set_point2_str;
            str_p = &set_point2_str;
        }
        else if (streq (argv[i], "-temp_control")) { /* temp_control: on/off */
            func_call [FUNC_RFFE_TEMP_CONTROL_IDX].call = 1;
            func_call [FUNC_RFFE_TEMP_CONTROL_IDX].arg = &temp_control_str;
            str_p = &temp_control_str;
        }
        else if (streq (argv[i], "-reset")) { /* reset: on/off */
            func_call [FUNC_RFFE_RESET_IDX].call = 1;
            func_call [FUNC_RFFE_RESET_IDX].arg = &reset_str;
            str_p = &reset_str;
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
        fprintf (stderr, "[client:rffe]: Setting default value to BOARD number: %u\n",
                DFLT_BOARD_NUMBER);
        board_number = DFLT_BOARD_NUMBER;
    }
    else {
        board_number = strtoul (board_number_str, NULL, 10);
    }

    /* Set default bpm number */
    uint32_t bpm_number;
    if (bpm_number_str == NULL) {
        fprintf (stderr, "[client:rffe]: Setting default value to BPM number: %u\n",
                DFLT_BPM_NUMBER);
        bpm_number = DFLT_BPM_NUMBER;
    }
    else {
        bpm_number = strtoul (bpm_number_str, NULL, 10);

        if (bpm_number > MAX_BPM_NUMBER) {
            fprintf (stderr, "[client:rffe]: BPM number too big! Defaulting to: %u\n",
                    MAX_BPM_NUMBER);
            bpm_number = MAX_BPM_NUMBER;
        }
    }

    char service[50];
    sprintf (service, "BPM%u:DEVIO:RFFE%u", board_number, bpm_number);

    bpm_client_t *bpm_client = bpm_client_new (broker_endp, verbose, NULL);

    for (i = 0; i < MAX_NUM_FUNCS; ++i) {
        if (func_call [i].call == 1) {
            fprintf (stdout, "[client:rffe_ctl]: Calling function #%d\n", i);
            fprintf (stdout, "[client:rffe_ctl]: Argument: %f\n", strtod (*func_call [i].arg, NULL));
            bpm_client_err_e err = bpm_rffe_funcs [i]
                (bpm_client, service, strtod (*func_call [i].arg, NULL));

            if (err != BPM_CLIENT_SUCCESS) {
                fprintf (stderr, "[client:rffe_ctl]: Error executing remote "
                        "function: %s\n", bpm_client_err_str (err));
            }
        }
    }

    bpm_client_destroy (&bpm_client);

    /* ugly... */
    free (sw_str);
    free (att1_str);
    free (att2_str);
    free (set_point1_str);
    free (set_point2_str);
    free (temp_control_str);
    free (reset_str);
    free (bpm_number_str);
    free (broker_endp);

    return 0;
}
