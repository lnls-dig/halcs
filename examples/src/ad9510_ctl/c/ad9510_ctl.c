/*
 * Controlling the AD9510 chip
 */

#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <halcs_client.h>

#define DFLT_BIND_FOLDER                        "/tmp/halcs"

/* Default value definitions */

#define DFLT_HALCS_NUMBER                         0
#define MAX_HALCS_NUMBER                          1

#define DFLT_BOARD_NUMBER                       0

#define FUNC_AD9510_A_DIV_IDX                   0
#define FUNC_AD9510_B_DIV_IDX                   1
#define FUNC_AD9510_PRESCALER_IDX               2
#define FUNC_AD9510_R_DIV_IDX                   3
#define FUNC_AD9510_PLL_PDOWN_IDX               4
#define FUNC_AD9510_MUX_STATUS_IDX              5
#define FUNC_AD9510_CP_CURRENT_IDX              6
#define FUNC_AD9510_OUTPUTS_IDX                 7
#define FUNC_AD9510_PLL_CLK_SEL_IDX             8
#define FUNC_AD9510_END                         9

#define MAX_NUM_FUNCS                           FUNC_AD9510_END

typedef halcs_client_err_e (*halcs_func_fp) (halcs_client_t *self, char *service,
        uint32_t value);

static const halcs_func_fp halcs_ad9510_funcs [MAX_NUM_FUNCS] = {
    &halcs_set_ad9510_pll_a_div,
    &halcs_set_ad9510_pll_b_div,
    &halcs_set_ad9510_pll_prescaler,
    &halcs_set_ad9510_r_div,
    &halcs_set_ad9510_pll_pdown,
    &halcs_set_ad9510_mux_status,
    &halcs_set_ad9510_cp_current,
    &halcs_set_ad9510_outputs,
    &halcs_set_ad9510_pll_clk_sel,
};

void print_help (char *program_name)
{
    printf( "Usage: %s [options]\n"
            "\t-h This help message\n"
            "\t-v Verbose output\n"
            "\t-b <broker_endpoint> Broker endpoint\n"
            "\t-board <AMC board = [0|1|2|3|4|5]>\n"
            "\t-halcs <HALCS number = [0|1]>\n"
            "\t-a_div <divider> PLL A Divider\n"
            "\t-b_div <divider> PLL B Divider\n"
            "\t-prescaler <value> PLL Prescaler value\n"
            "\t-r_div <value> PLL Prescaler value\n"
            "\t-pll_pdown <value> PLL Power down\n"
            "\t-mux_status <value> PLL Mux status\n"
            "\t-cp_current <value> PLL Charge Pump current [600|1200|...|4800]i uA\n"
            "\t-outputs <value> Clock outputs control = [8-bit bitmask]\n"
            "\t-pll_clk_sel <value> PLL Clock selection = [1|2]\n"
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
    char *halcs_number_str = NULL;
    char *a_div_str = NULL;
    char *b_div_str = NULL;
    char *prescaler_div_str = NULL;
    char *r_div_str = NULL;
    char *pll_pdown_str = NULL;
    char *mux_status_str = NULL;
    char *cp_current_str = NULL;
    char *outputs_str = NULL;
    char *pll_clk_sel_str = NULL;
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
        else if (streq(argv[i], "-halcs"))
        {
            str_p = &halcs_number_str;
        }
        else if (streq (argv[i], "-a_div")) { /* a_div: divider value */
            func_call [FUNC_AD9510_A_DIV_IDX].call = 1;
            func_call [FUNC_AD9510_A_DIV_IDX].arg = &a_div_str;
            str_p = &a_div_str;
        }
        else if (streq (argv[i], "-b_div")) { /* b_div: divider value */
            func_call [FUNC_AD9510_B_DIV_IDX].call = 1;
            func_call [FUNC_AD9510_B_DIV_IDX].arg = &b_div_str;
            str_p = &b_div_str;
        }
        else if (streq (argv[i], "-prescaler_div")) { /* prescaler_div: divider value */
            func_call [FUNC_AD9510_PRESCALER_IDX].call = 1;
            func_call [FUNC_AD9510_PRESCALER_IDX].arg = &prescaler_div_str;
            str_p = &prescaler_div_str;
        }
        else if (streq (argv[i], "-r_div")) { /* r_div: divider value */
            func_call [FUNC_AD9510_R_DIV_IDX].call = 1;
            func_call [FUNC_AD9510_R_DIV_IDX].arg = &r_div_str;
            str_p = &r_div_str;
        }
        else if (streq (argv[i], "-pll_pdown")) { /* pll_pdown: divider value */
            func_call [FUNC_AD9510_PLL_PDOWN_IDX].call = 1;
            func_call [FUNC_AD9510_PLL_PDOWN_IDX].arg = &pll_pdown_str;
            str_p = &pll_pdown_str;
        }
        else if (streq (argv[i], "-mux_status")) { /* mux_status: divider value */
            func_call [FUNC_AD9510_MUX_STATUS_IDX].call = 1;
            func_call [FUNC_AD9510_MUX_STATUS_IDX].arg = &mux_status_str;
            str_p = &mux_status_str;
        }
        else if (streq (argv[i], "-cp_current")) { /* cp_current: divider value */
            func_call [FUNC_AD9510_CP_CURRENT_IDX].call = 1;
            func_call [FUNC_AD9510_CP_CURRENT_IDX].arg = &cp_current_str;
            str_p = &cp_current_str;
        }
        else if (streq (argv[i], "-outputs")) { /* outputs: divider value */
            func_call [FUNC_AD9510_OUTPUTS_IDX].call = 1;
            func_call [FUNC_AD9510_OUTPUTS_IDX].arg = &outputs_str;
            str_p = &outputs_str;
        }
        else if (streq (argv[i], "-pll_clk_sel")) { /* pll_clk_sel: divider value */
            func_call [FUNC_AD9510_PLL_CLK_SEL_IDX].call = 1;
            func_call [FUNC_AD9510_PLL_CLK_SEL_IDX].arg = &pll_clk_sel_str;
            str_p = &pll_clk_sel_str;
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
        fprintf (stderr, "[client:leds]: Setting default value to BOARD number: %u\n",
                DFLT_BOARD_NUMBER);
        board_number = DFLT_BOARD_NUMBER;
    }
    else {
        board_number = strtoul (board_number_str, NULL, 10);
    }

    /* Set default halcs number */
    uint32_t halcs_number;
    if (halcs_number_str == NULL) {
        fprintf (stderr, "[client:leds]: Setting default value to HALCS number: %u\n",
                DFLT_HALCS_NUMBER);
        halcs_number = DFLT_HALCS_NUMBER;
    }
    else {
        halcs_number = strtoul (halcs_number_str, NULL, 10);

        if (halcs_number > MAX_HALCS_NUMBER) {
            fprintf (stderr, "[client:leds]: HALCS number too big! Defaulting to: %u\n",
                    MAX_HALCS_NUMBER);
            halcs_number = MAX_HALCS_NUMBER;
        }
    }

    char service[50];
    snprintf (service, sizeof (service), "HALCS%u:DEVIO:FMC_ACTIVE_CLK%u", board_number, halcs_number);

    halcs_client_t *halcs_client = halcs_client_new (broker_endp, verbose, NULL);
    if (halcs_client == NULL) {
        fprintf (stderr, "[client:acq]: halcs_client could be created\n");
        goto err_halcs_client_new;
    }

    for (i = 0; i < MAX_NUM_FUNCS; ++i) {
        if (func_call [i].call == 1) {
            fprintf (stdout, "[client:ad9510_ctl]: Calling function #%d\n", i);
            fprintf (stdout, "[client:ad9510_ctl]: Argument: %lu\n", strtoul (*func_call [i].arg, NULL, 10));
            halcs_client_err_e err = halcs_ad9510_funcs [i]
                (halcs_client, service, strtoul (*func_call [i].arg, NULL, 10));

            if (err != HALCS_CLIENT_SUCCESS) {
                fprintf (stderr, "[client:ad9510_ctl]: Error executing remote "
                        "function: %s\n", halcs_client_err_str (err));
            }
        }
    }

err_halcs_client_new:
    halcs_client_destroy (&halcs_client);

    /* ugly... */
    free (a_div_str);
    free (b_div_str);
    free (prescaler_div_str);
    free (r_div_str);
    free (pll_pdown_str);
    free (mux_status_str);
    free (cp_current_str);
    free (outputs_str);
    free (pll_clk_sel_str);
    free (board_number_str);
    free (halcs_number_str);
    free (broker_endp);

    return 0;
}
