/*
 * Controlling the RFFE board
 */

#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <bpm_client.h>

#define DFLT_BIND_FOLDER                        "/tmp/bpm"

/* Default value definitions */

#define DFLT_BPM_NUMBER                         0
#define MAX_BPM_NUMBER                          1

#define DFLT_BOARD_NUMBER                       0

#define FUNC_RFFE_ATT_IDX                       0
#define FUNC_RFFE_SET_POINT_AC_IDX              1
#define FUNC_RFFE_SET_POINT_BD_IDX              2
#define FUNC_RFFE_TEMP_CONTROL_IDX              3
#define FUNC_RFFE_SET_HEATER_AC_IDX             4
#define FUNC_RFFE_SET_HEATER_BD_IDX             5
#define FUNC_RFFE_RESET_IDX                     6
#define FUNC_RFFE_SET_PID_AC_KP_IDX             8
#define FUNC_RFFE_SET_PID_AC_TI_IDX             9
#define FUNC_RFFE_SET_PID_AC_TD_IDX             10
#define FUNC_RFFE_SET_PID_BD_KP_IDX             11
#define FUNC_RFFE_SET_PID_BD_TI_IDX             12
#define FUNC_RFFE_SET_PID_BD_TD_IDX             13
#define FUNC_RFFE_END                           14

#define MAX_NUM_FUNCS                           FUNC_RFFE_END

/* Functions redeclaration for compatibility with double arguments */
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
    &bpm_set_rffe_att,
    &bpm_set_rffe_set_point_ac,
    &bpm_set_rffe_set_point_bd,
    &bpm_set_rffe_temp_control_double,  /* function "double" argument version */
    &bpm_set_rffe_heater_ac,
    &bpm_set_rffe_heater_bd,
    &bpm_set_rffe_reset_double,         /* function "double" argument version */
    &bpm_set_rffe_pid_ac_kp,
    &bpm_set_rffe_pid_ac_ti,
    &bpm_set_rffe_pid_ac_td,
    &bpm_set_rffe_pid_bd_kp,
    &bpm_set_rffe_pid_bd_ti,
    &bpm_set_rffe_pid_bd_td,
};

void print_help (char *program_name)
{
    printf( "Usage: %s [options]\n"
            "\t-h This help message\n"
            "\t-v Verbose output\n"
            "\t-b <broker_endpoint> Broker endpoint\n"
            "\t-board <AMC board = [0|1|2|3|4|5]>\n"
            "\t-bpm <BPM number = [0|1]>\n"
            "\t-att <on/off = [0 - 31.5 dB / 0.5 dB step]> Attenuator value\n"
            "\t-set_point_ac <set point = [ambient temp. - 60.0 oC]> Set point AC\n"
            "\t-set_point_ac <set point = [ambient temp. - 60.0 oC]> Set point BD\n"
            "\t-temp_control <on/off = [0|1]> Temperature control\n"
            "\t-heater_ac <> Resistor AC control\n"
            "\t-heater_bd <> Resistor BD control\n"
            "\t-reset <on/off = [0|1]> Reset\n"
            "\t-pid_ac_kp <> PID AC KP\n"
            "\t-pid_ac_ti <> PID AC TI\n"
            "\t-pid_ac_td <> PID AC TD\n"
            "\t-pid_bd_kp <> PID BD KP\n"
            "\t-pid_bd_ti <> PID BD TI\n"
            "\t-pid_bd_td <> PID BD TD\n"
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
    bpm_client_err_e err = BPM_CLIENT_SUCCESS;
    char *broker_endp = NULL;
    char *board_number_str = NULL;
    char *bpm_number_str = NULL;
    char *att_str = NULL;
    char *set_point_ac_str = NULL;
    char *set_point_bd_str = NULL;
    char *heater_ac_str = NULL;
    char *heater_bd_str = NULL;
    char *pid_ac_kp_str = NULL;
    char *pid_ac_ti_str = NULL;
    char *pid_ac_td_str = NULL;
    char *pid_bd_kp_str = NULL;
    char *pid_bd_ti_str = NULL;
    char *pid_bd_td_str = NULL;
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
        else if (streq (argv[i], "-att")) { /* att1: attenuator value */
            func_call [FUNC_RFFE_ATT_IDX].call = 1;
            func_call [FUNC_RFFE_ATT_IDX].arg = &att_str;
            str_p = &att_str;
        }
        else if (streq (argv[i], "-set_point_ac")) { /* set_point_ac: temperature set point value */
            func_call [FUNC_RFFE_SET_POINT_AC_IDX].call = 1;
            func_call [FUNC_RFFE_SET_POINT_AC_IDX].arg = &set_point_ac_str;
            str_p = &set_point_ac_str;
        }
        else if (streq (argv[i], "-set_point_bd")) { /* set_point_bd: Temperature set value */
            func_call [FUNC_RFFE_SET_POINT_BD_IDX].call = 1;
            func_call [FUNC_RFFE_SET_POINT_BD_IDX].arg = &set_point_bd_str;
            str_p = &set_point_bd_str;
        }
        else if (streq (argv[i], "-temp_control")) { /* temp_control: on/off */
            func_call [FUNC_RFFE_TEMP_CONTROL_IDX].call = 1;
            func_call [FUNC_RFFE_TEMP_CONTROL_IDX].arg = &temp_control_str;
            str_p = &temp_control_str;
        }
        else if (streq (argv[i], "-heater_ac")) { /* heater_ac: temperature set point value */
            func_call [FUNC_RFFE_SET_HEATER_AC_IDX].call = 1;
            func_call [FUNC_RFFE_SET_HEATER_AC_IDX].arg = &heater_ac_str;
            str_p = &heater_ac_str;
        }
        else if (streq (argv[i], "-heater_bd")) { /* heater_bd: Temperature set value */
            func_call [FUNC_RFFE_SET_HEATER_BD_IDX].call = 1;
            func_call [FUNC_RFFE_SET_HEATER_BD_IDX].arg = &heater_bd_str;
            str_p = &heater_bd_str;
        }
        else if (streq (argv[i], "-pid_ac_kp")) {
            func_call [FUNC_RFFE_SET_PID_AC_KP_IDX].call = 1;
            func_call [FUNC_RFFE_SET_PID_AC_KP_IDX].arg = &pid_ac_kp_str;
            str_p = &pid_ac_kp_str;
        }
        else if (streq (argv[i], "-pid_ac_ti")) {
            func_call [FUNC_RFFE_SET_PID_AC_TI_IDX].call = 1;
            func_call [FUNC_RFFE_SET_PID_AC_TI_IDX].arg = &pid_ac_ti_str;
            str_p = &pid_ac_ti_str;
        }
        else if (streq (argv[i], "-pid_ac_td")) {
            func_call [FUNC_RFFE_SET_PID_AC_TD_IDX].call = 1;
            func_call [FUNC_RFFE_SET_PID_AC_TD_IDX].arg = &pid_ac_td_str;
            str_p = &pid_ac_td_str;
        }
        else if (streq (argv[i], "-pid_bd_kp")) {
            func_call [FUNC_RFFE_SET_PID_BD_KP_IDX].call = 1;
            func_call [FUNC_RFFE_SET_PID_BD_KP_IDX].arg = &pid_bd_kp_str;
            str_p = &pid_bd_kp_str;
        }
        else if (streq (argv[i], "-pid_bd_ti")) {
            func_call [FUNC_RFFE_SET_PID_BD_TI_IDX].call = 1;
            func_call [FUNC_RFFE_SET_PID_BD_TI_IDX].arg = &pid_bd_ti_str;
            str_p = &pid_bd_ti_str;
        }
        else if (streq (argv[i], "-pid_bd_td")) {
            func_call [FUNC_RFFE_SET_PID_BD_TD_IDX].call = 1;
            func_call [FUNC_RFFE_SET_PID_BD_TD_IDX].arg = &pid_bd_td_str;
            str_p = &pid_bd_td_str;
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
    snprintf (service, sizeof (service), "BPM%u:DEVIO:RFFE%u", board_number, bpm_number);

    bpm_client_t *bpm_client = bpm_client_new (broker_endp, verbose, NULL);
    if (bpm_client == NULL) {
        fprintf (stderr, "[client:acq]: bpm_client could be created\n");
        goto err_bpm_client_new;
    }

    for (i = 0; i < MAX_NUM_FUNCS; ++i) {
        if (func_call [i].call == 1) {
            fprintf (stdout, "[client:rffe_ctl]: Calling function #%d\n", i);
            fprintf (stdout, "[client:rffe_ctl]: Argument: %f\n", strtod (*func_call [i].arg, NULL));
            err = bpm_rffe_funcs [i]
                (bpm_client, service, strtod (*func_call [i].arg, NULL));

            if (err != BPM_CLIENT_SUCCESS) {
                fprintf (stderr, "[client:rffe_ctl]: Error executing remote "
                        "function: %s\n", bpm_client_err_str (err));
            }
        }
    }

    double temp;
    err = bpm_get_rffe_temp_ac (bpm_client, service, &temp);
    if (err != BPM_CLIENT_SUCCESS) {
        fprintf (stderr, "[client:rffe_ctl]: Error executing remote "
                "function: %s\n", bpm_client_err_str (err));
    }
    fprintf (stdout, "[client:rffe_ctl]: Temp AC: %f\n", temp);

    err = bpm_get_rffe_temp_bd (bpm_client, service, &temp);
    if (err != BPM_CLIENT_SUCCESS) {
        fprintf (stderr, "[client:rffe_ctl]: Error executing remote "
                "function: %s\n", bpm_client_err_str (err));
    }
    fprintf (stdout, "[client:rffe_ctl]: Temp BD: %f\n", temp);

    double att;
    err = bpm_get_rffe_att (bpm_client, service, &att);
    if (err != BPM_CLIENT_SUCCESS) {
        fprintf (stderr, "[client:rffe_ctl]: Error executing remote "
                "function: %s\n", bpm_client_err_str (err));
    }
    fprintf (stdout, "[client:rffe_ctl]: Att: %f\n", att);

err_bpm_client_new:
    bpm_client_destroy (&bpm_client);

    /* ugly... */
    free (att_str);
    free (set_point_ac_str);
    free (set_point_bd_str);
    free (temp_control_str);
    free (reset_str);
    free (bpm_number_str);
    free (broker_endp);

    return 0;
}
