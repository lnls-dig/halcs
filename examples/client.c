#include "bpm_client.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include "dispatch_table.h"

#include "sm_io_codes.h"
#include "sm_io_fmc130m_4ch_exports.h"
#include "sm_io_swap_exports.h"
#include "sm_io_dsp_exports.h"
#include "sm_io_acq_exports.h"

#define DFLT_BIND_FOLDER "/tmp/bpm"

#define DEFAULT_NUM_SAMPLES         4096
#define DEFAULT_CHAN_NUM            0

#define DFLT_BPM_NUMBER             0
#define MAX_BPM_NUMBER              1

#define DFLT_BOARD_NUMBER           0
#define MAX_BOARD_NUMBER            10

#define FMC130M_4CH_MODULE_NAME     "FMC130M_4CH"
#define DSP_MODULE_NAME             "DSP"
#define SWAP_MODULE_NAME            "SWAP"
#define ACQ_MODULE_NAME             "ACQ"
#define RFFE_MODULE_NAME            "RFFE"
#define MAX_VARIABLES_NUMBER        sizeof(uint32_t)*8

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

/* Arbitrary hard limits */
#define MAX_NUM_SAMPLES             (1 << 28)
#define MAX_NUM_CHANS               (1 << 8)

#define PRINTV(verbose, fmt, ...)\
    do {\
        if (verbose) {\
            printf (fmt, ## __VA_ARGS__);\
        }\
    }while(0)

void print_data_curve (uint32_t chan, uint32_t *data, uint32_t size)
{
    /* FIXME: Make it more generic */
    if (chan == 0) {
        int16_t *raw_data16 = (int16_t *) data;
        for (uint32_t i = 0; i < (size/sizeof(uint16_t)) / 4; i++) {
            if (zctx_interrupted) {
                break;
            }

            printf ("%6u\t %8d\t %8d\t %8d\t %8d\n", i,
                    raw_data16[(i*4)],
                    raw_data16[(i*4)+1],
                    raw_data16[(i*4)+2],
                    raw_data16[(i*4)+3]);
        }
    } else {
        int32_t *raw_data32 = (int32_t *) data;
        for (uint32_t i = 0; i < (size/sizeof(uint32_t)) / 4; i++) {
            if (zctx_interrupted) {
                break;
            }

            printf ("%6u\t %8d\t %8d\t %8d\t %8d\n", i,
                    raw_data32[(i*4)],
                    raw_data32[(i*4)+1],
                    raw_data32[(i*4)+2],
                    raw_data32[(i*4)+3]);
        }
    }
}

typedef struct _call_var_t {
    char *name;
    char *service;
    int rw;                     // 1 -> read, 0 -> write
    int poll;                   // 1 to keep waiting data in return from server
    uint8_t write_val[MAX_VARIABLES_NUMBER];    // 2 32-bits variables
    uint8_t read_val[MAX_VARIABLES_NUMBER];     // 2 32-bits variables
} call_var_t;

typedef call_var_t call_func_t;

static void _zlist_free_item (void *data)
{
    free(((call_func_t *) data)->name);
    free((call_func_t *) data);
    data = NULL;
}

void append_item (zlist_t* list, call_func_t func)
{
    call_func_t *wrap_func = zmalloc(sizeof(call_func_t));
    *wrap_func = func;
    wrap_func->name = strdup(func.name);
    zlist_append (list, wrap_func);
    zlist_freefn (list, wrap_func, _zlist_free_item, false);
}

int print_var (call_var_t *var)
{
    const disp_op_t* func_structure = bpm_func_translate (var->name);

    switch (DISP_GET_ATYPE(func_structure->retval))
    {
        case DISP_ATYPE_UINT16:
            printf ("%" PRIu16 "\n", ((uint16_t)*(var->read_val)));
            break;

        case DISP_ATYPE_UINT32:
            printf ("%" PRIu32 "\n", ((uint32_t)*(var->read_val)));
            break;

        case DISP_ATYPE_UINT64:
            printf ("%" PRIu64 "\n", ((uint64_t)*(var->read_val)));
            break;

        case DISP_ATYPE_DOUBLE:
            printf ("%f\n", (double)*(var->read_val));
            break;

        default:
            printf ("%" PRIu8 "\n", ((uint8_t )*(var->read_val)));
    }
    return 0;
}

int print_var_v(int verbose, call_var_t *func)
{
    PRINTV(verbose, "%s: ", func->name);
    if (verbose) {
        print_var(func);
    }
    return 0;
}

int print_func_v(int verbose, call_func_t *func)
{
    return print_var_v(verbose, (call_var_t *)func);
}

enum
{
    CHANNEL = 0,
    VALUE,
    SUBOPT_END
};

char *mount_opts[] =
{
    [CHANNEL] = "chan",
    [VALUE] = "value",
    [SUBOPT_END] = NULL
};

bpm_client_err_e parse_subopt (char *subopts, char *mount_opts[], char* name, char *corr_name, uint8_t *input)
{
    bpm_client_err_e err = BPM_CLIENT_SUCCESS;
    char* value;
    char* temp_value = "";
    size_t len = strlen(name);
    char* temp = zmalloc(len);
    memcpy(temp, name, len-1);
    memcpy(corr_name, temp, len);

    while (*subopts != '\0')
    {
        switch (getsubopt (&subopts, mount_opts, &value))
        {
            case CHANNEL:
                    memcpy(corr_name+len-1, value, strlen(value));
                    break;
            case VALUE:
                    temp_value = value;
                    break;
            default:
                    /* Unknown suboption. */
                    printf ("Unknown suboption '%s'\n", value);
                    err = BPM_CLIENT_ERR_INV_FUNCTION;
                    goto inv_function;
        }
    }
    const disp_op_t* temp_func = bpm_func_translate(corr_name);
    if (temp_func == NULL) {
        err = BPM_CLIENT_ERR_INV_FUNCTION;
        goto inv_function;
    }

    if (DISP_GET_ATYPE(temp_func->args[1]) == DISP_ATYPE_DOUBLE) {
        *(double *)(input+4) = strtod(temp_value, NULL);
    } else {
        *(input+4) = (uint32_t) strtoul(temp_value, NULL, 10);
    }

inv_function:
    free (temp);
    value = NULL;
    temp = NULL;
    return err;
}

int main (int argc, char *argv [])
{
    int ch;
    int verbose = 0;
    char *broker_endp = NULL;
    char *default_broker_endp = strdup ("ipc://"DFLT_BIND_FOLDER);
    const char* program_name;
    program_name = argv[0];

    uint32_t bpm_number;
    uint32_t board_number;
    char *board_number_str = NULL;
    char *bpm_number_str = NULL;

    // Acquitision parameters check variables
    int acq_samples_set = 0;
    int32_t acq_samples_val = 0;
    int acq_chan_set = 0;
    uint32_t acq_chan_val = 0;
    int acq_full_call = 0;
    int acq_start = 0;
    int acq_check = 0;
    int acq_get_block = 0;
    uint32_t acq_block_id = 0;
    int check_poll = 0;
    uint32_t poll_timeout = 0;

    void print_usage (FILE* stream, int exit_code)
    {
        fprintf (stream, "BPM Client program\n");
        fprintf (stream, "Usage:  %s options \n", program_name);
        fprintf (stream,
                "  -h  --help                       Display this usage information.\n"
                "  -v  --verbose                    Print verbose messages.\n"
                "  -e  --endpoint <endpoint>        Define broker endpoint\n"
                "  -d  --board <number>             Define the target board\n"
                "  -m  --bpm <number>               Define the target bpm\n"
                "  -l  --leds                       Set board leds\n"
                "  -p  --getpll                     Get PLL value\n"
                "  -P  --setpll <value>             Set PLL value\n"
                "  -L  --ad9510default              Set AD9510 to default values\n"
                "  -c  --getadcdata                 Get ADC data\n"
                "  -C  --setadcdata <value>         Set ADC data\n"
                "  --getdlyval                      Get delay value\n"
                "  --setdlyval <value>              Set delay value\n"
                "  --getdlyline                     Get delay line\n"
                "  --setdlyline <value>             Set delay line\n"
                "  --getdlyupdt                     Get delay update\n"
                "  --setdlyupdt <value>             Set delay update\n"
                "  -V  --setadcdly <value>          Set ADC delay\n"
                "  -n  --getadctest                 Get ADC test\n"
                "  -N  --setadctest <value>         Set ADC test\n"
                "  -o  --getsi571oe                 Get SI571 OE\n"
                "  -O  --setsi571oe <value>         Set SI571 OE\n"
                "  -i  --setsi571freq <value>       Set SI571 frequency\n"
                "  -D  --si571default               Set SI571 to default values\n"
                "  -a  --setad9510plladiv <value>   Set AD9510 PLL A Divider\n"
                "  -b  --setad9510pllbdiv <value>   Set AD9510 PLL B Divider\n"
                "  -r  --setad9510pllpresc <value>  Set AD9510 PLL Prescaler\n"
                "  -R  --setad9510rdiv <value>      Set AD9510 R Divider\n"
                "  -B  --setad9510pdown <value>     Set AD9510 PDown\n"
                "  -M  --setad9510mux <value>       Set AD9510 Multiplexer\n"
                "  -u  --setad9510cpcurr <value>    Set AD9510 CP Current\n"
                "  -U  --setad9510outputs <value>   Set AD9510 Outputs\n"
                "  -k  --setad9510pllclksel <value> Set AD9510 PLL Clock Selection\n"
                "  --getkx                          Get KX value\n"
                "  --setkx <value>                  Set KX value\n"
                "  --getky                          Get KY value\n"
                "  --setky <value>                  Set KY value\n"
                "  --getksum                        Get KSUM value\n"
                "  --setksum <value>                Set KSUM value\n"
                "  --gettbtth                       Get TBT threshold value\n"
                "  --settbtth <value>               Set TBT threshold value\n"
                "  --getfofbth                      Get FOFB threshold value\n"
                "  --setfofbth <value>              Set FOFB threshold value\n"
                "  --getmonitth                     Get Monitoring threshold value\n"
                "  --setmonitth <value>             Set Monitoring threshold value\n"
                "  -j  --getmonitamp                Get Monitoring amplitude\n"
                "  -J  --setmonitamp <value>        Set Monitoring amplitude\n"
                "  -x  --getmonitposx               Get Monitoring X position\n"
                "  -X  --setmonitposx <value>       Set Monitoring X position\n"
                "  -y  --getmonitposy               Get Monitoring Y position\n"
                "  -Y  --setmonitposy <value>       Set Monitoring Y position\n"
                "  -q  --getmonitposq               Get Monitoring position Q\n"
                "  -Q  --setmonitposq <value>       Set Monitoring position Q\n"
                "  -s  --getmonitpossum             Get Monitoring position Sum\n"
                "  -S  --setmonitpossum <value>     Set Monitoring position Sum\n"
                "  -w  --getsw                      Get SW status\n"
                "  -W  --setsw <value>              Set SW status\n"
                "  -t  --getswen                    Get SW Enable status\n"
                "  -T  --setswen <value>            Set SW Enable\n"
                "  -z  --getdivclk                  Get divider clock\n"
                "  -Z  --setdivclk <value>          Set divider clock\n"
                "  -f  --getswdly                   Get SW delay\n"
                "  -F  --setswdly <value>           Set SW delay\n"
                "  --getwdwen                       Get WDW Enable status\n"
                "  --setwdwen <value>               Set WDW Enable\n"
                "  --getwdwdly                      Get WDW delay\n"
                "  --setwdwdly <value>              Set WDW delay\n"
                "  -g  --getgain                    Get RF Gain\n"
                "  -G  --setgain <value [db]>       Set RF Gain\n"
                "  -E  --setsamples   <number of samples>\n"
                "                                     [<number of samples> must be between 4 and\n"
                "                                     ??? (TBD)]\n"
                "  -H  --setchan      <channel> \n"
                "                                   Sets FPGA Acquisition parameters\n"
                "                                     [<channel> must be one of the following:\n"
                "                                     0 -> ADC; 1-> TBT Amp; 2 -> TBT Pos\n"
                "                                     3 -> FOFB Amp; 4-> FOFB Pos]\n"
                "  -I  --acqstart                   Starts FPGA acquistion with the previous parameters\n"
                "  -K  --acqcheck                   Check if the previous acquisition is over\n"
                "  --acqcheckpoll                   Keep checking if the acquisition is over for an amount of time\n"
                "                                    (Must be used with -t <timeout>) \n"
                "  -A  --getblock <block>           Get specified data block from server \n"
                "  --fullacq                        Perform a full acquisition\n"
                "  --timeout    <timeout>           Sets the timeout for the polling function\n"
                );
        exit (exit_code);
    }

    /* Long-only options */
    enum {
        getdlyval = 1000,
        setdlyval,
        getdlyline,
        setdlyline,
        getdlyupdt,
        setdlyupdt,
        setkx,
        getkx,
        setky,
        getky,
        setksum,
        getksum,
        settbtth,
        gettbtth,
        setfofbth,
        getfofbth,
        setmonitth,
        getmonitth,
        setwdwen,
        getwdwen,
        setwdwdly,
        getwdwdly,
        setgaina,
        getgaina,
        setgainb,
        getgainb,
        setgainc,
        getgainc,
        setgaind,
        getgaind,
        rffesetsw,
        rffegetsw,
        rffesetatt,
        rffegetatt,
        rffesettmp,
        rffegettmp,
        rffesetpnt,
        rffegetpnt,
        rffesettmpctr,
        rffegettmpctr,
        rffesetout,
        rffegetout,
        rffesetrst,
        rffegetrst,
        rffesetrpg,
        rffegetrpg,
        rffesetswlvl,
        rffegetswlvl,
        acqcheckpoll,
        getcurve,
        fullacq,
        timeout
    };

    static struct option long_options[] =
    {
        {"help",                no_argument,         NULL, 'h'},    
        {"verbose",             no_argument,         NULL, 'v'},    
        {"endpoint",            required_argument,   NULL, 'e'},
        {"board",               required_argument,   NULL, 'd'},    
        {"bpm",                 required_argument,   NULL, 'm'},    
        {"leds",                required_argument,   NULL, 'l'},
        {"getpll",              no_argument,         NULL, 'p'},
        {"setpll",              required_argument,   NULL, 'P'},
        {"ad9510default",       no_argument,         NULL, 'L'},
        {"getadcdata",          required_argument,   NULL, 'c'},
        {"setadcdata",          required_argument,   NULL, 'C'},
        {"getdlyval",           required_argument,   NULL, getdlyval},
        {"setdlyval",           required_argument,   NULL, setdlyval},
        {"getdlyline",          required_argument,   NULL, getdlyline},
        {"setdlyline",          required_argument,   NULL, setdlyline},
        {"getdlyupdt",          required_argument,   NULL, getdlyupdt},
        {"setdlyupdt",          required_argument,   NULL, setdlyupdt},
        {"setadcdly",           required_argument,   NULL, 'V'},
        {"getadctest",          no_argument,         NULL, 'n'},
        {"setadctest",          required_argument,   NULL, 'N'},
        {"getsi571oe",          no_argument,         NULL, 'o'},
        {"setsi571oe",          required_argument,   NULL, 'O'},
        {"setsi571freq",        required_argument,   NULL, 'i'},
        {"si571default",        required_argument,   NULL, 'D'},
        {"setad9510plladiv",    required_argument,   NULL, 'a'},
        {"setad9510pllbdiv",    required_argument,   NULL, 'b'},
        {"setad9510pllpresc",   required_argument,   NULL, 'r'},
        {"setad9510rdiv",       required_argument,   NULL, 'R'},
        {"setad9510pdown",      required_argument,   NULL, 'B'},
        {"setad9510mux",        required_argument,   NULL, 'M'},
        {"setad9510cpcurr",     required_argument,   NULL, 'u'},
        {"setad9510outputs",    required_argument,   NULL, 'U'},
        {"setad9510pllclksel",  required_argument,   NULL, 'k'},
        {"setkx",               required_argument,   NULL, setkx},
        {"getkx",               no_argument,         NULL, getkx},
        {"setky",               required_argument,   NULL, setky},
        {"getky",               no_argument,         NULL, getky},
        {"setksum",             required_argument,   NULL, setksum},
        {"getksum",             no_argument,         NULL, getksum},
        {"settbtth",            required_argument,   NULL, settbtth},
        {"gettbtth",            no_argument,         NULL, gettbtth},
        {"setfofbth",           required_argument,   NULL, setfofbth},
        {"getfofbth",           no_argument,         NULL, getfofbth},
        {"setmonitth",          required_argument,   NULL, setmonitth},
        {"getmonitth",          no_argument,         NULL, getmonitth},
        {"setmonitamp",         required_argument,   NULL, 'J'},
        {"getmonitamp",         required_argument,   NULL, 'j'},
        {"setmonitposx",        required_argument,   NULL, 'X'},
        {"getmonitposx",        required_argument,   NULL, 'x'},
        {"setmonitposy",        required_argument,   NULL, 'Y'},
        {"getmonitposy",        required_argument,   NULL, 'y'},
        {"setmonitposq",        required_argument,   NULL, 'Q'},
        {"getmonitposq",        required_argument,   NULL, 'q'},
        {"setmonitpossum",      required_argument,   NULL, 'S'},
        {"getmonitpossum",      required_argument,   NULL, 's'},
        {"setsw",               required_argument,   NULL, 'W'},
        {"getsw",               no_argument,         NULL, 'w'},
        {"setswen",             required_argument,   NULL, 'T'},
        {"getswen",             no_argument,         NULL, 't'},
        {"setdivclk",           required_argument,   NULL, 'Z'},
        {"getdivclk",           no_argument,         NULL, 'z'},
        {"setswdly",            required_argument,   NULL, 'F'},
        {"getswdly",            no_argument,         NULL, 'f'},
        {"setwdwen",            required_argument,   NULL, setwdwen},
        {"getwdwen",            no_argument,         NULL, getwdwen},
        {"setwdwdly",           required_argument,   NULL, setwdwdly},
        {"getwdwdly",           no_argument,         NULL, getwdwdly},
        {"setgaina",            required_argument,   NULL, setgaina},
        {"getgaina",            no_argument,         NULL, getgaina},
        {"setgainb",            required_argument,   NULL, setgainb},
        {"getgainb",            no_argument,         NULL, getgainb},
        {"setgainc",            required_argument,   NULL, setgainc},
        {"getgainc",            no_argument,         NULL, getgainc},
        {"setgaind",            required_argument,   NULL, setgaind},
        {"getgaind",            no_argument,         NULL, getgaind},
        {"rffesetsw",           no_argument,         NULL, rffesetsw},
        {"rffegetsw",           no_argument,         NULL, rffegetsw},
        {"rffesetatt",          required_argument,   NULL, rffesetatt},
        {"rffegetatt",          no_argument,         NULL, rffegetatt},
        {"rffesettmp",          required_argument,   NULL, rffesettmp},
        {"rffegettmp",          no_argument,         NULL, rffegettmp},
        {"rffesetpnt",          required_argument,   NULL, rffesetpnt},
        {"rffegetpnt",          no_argument,         NULL, rffegetpnt},
        {"rffesettmpctr",       required_argument,   NULL, rffesettmpctr},
        {"rffegettmpctr",       no_argument,         NULL, rffegettmpctr},
        {"rffesetout",          required_argument,   NULL, rffesetout},
        {"rffegetout",          no_argument,         NULL, rffegetout},
        {"rffesetrst",          required_argument,   NULL, rffesetrst},
        {"rffegetrst",          no_argument,         NULL, rffegetrst},
        {"rffesetrpg",          required_argument,   NULL, rffesetrpg},
        {"rffegetrpg",          no_argument,         NULL, rffegetrpg},
        {"rffesetswlvl",        required_argument,   NULL, rffesetswlvl},
        {"rffegetswlvl",        no_argument,         NULL, rffegetswlvl},
        {"setsamples",          required_argument,   NULL, 'E'},
        {"setchan",             required_argument,   NULL, 'H'},
        {"acqstart",            no_argument,         NULL, 'I'},
        {"acqcheck",            no_argument,         NULL, 'K'},
        {"acqcheckpoll",        no_argument,         NULL, acqcheckpoll},
        {"getblock",            required_argument,   NULL, 'A'},
        {"fullacq",             no_argument,         NULL, fullacq},
        {"timeout",             required_argument,   NULL, timeout},
        {NULL, 0, NULL, 0}
    };

    const char* shortopt = "hve:d:m:l:pP:Lc:C:u:U:V:nN:oO:i:D:a:b:r:R:B:M:u:U:k:j:J:xX:yY:qQ:sS:wW:tT:zZ:fF:E:H:IKA:";

    zlist_t *call_list = zlist_new();
    if (call_list == NULL) {
        fprintf(stderr, "[client]: Error in memory allocation for zlist\n");
    }

    char *corr_name = zmalloc(50);
    call_func_t item = {0};

    while ((ch = getopt_long_only(argc, argv, shortopt , long_options, NULL)) != -1)
    {
        bpm_client_err_e err = BPM_CLIENT_SUCCESS;

        //Get the specified options
        switch (ch)
        {
                //Help
            case 'h':
                print_usage(stderr, 0);

                //Verbose
            case 'v':
                verbose = 1;
                break;

                //Broker Endpoint
            case 'e':
                broker_endp = strdup(optarg);
                break;

                //Board Number
            case 'd':
                board_number_str = strdup(optarg);
                break;

                //BPM number
            case 'm':
                bpm_number_str = strdup(optarg);
                break;

                //Blink Leds
            case 'l':
                item.name = FMC130M_4CH_NAME_LEDS;
                item.service = FMC130M_4CH_MODULE_NAME;
                *(item.write_val) = (uint32_t) strtoul(optarg, NULL, 10);
                append_item (call_list, item);
                break;

                //Get PLL Function
            case 'p':
                item.name = FMC130M_4CH_NAME_PLL_FUNCTION;
                item.service = FMC130M_4CH_MODULE_NAME;
                item.rw = 1;
                *(item.write_val) = item.rw;
                append_item (call_list, item);
                break;

                //Set PLL Function
            case 'P':
                item.name = FMC130M_4CH_NAME_PLL_FUNCTION;
                item.service = FMC130M_4CH_MODULE_NAME;
                item.rw = 0;
                *(item.write_val) = item.rw;
                *(item.write_val+4) = (uint32_t) strtoul(optarg, NULL, 10);
                append_item (call_list, item);
                break;

                //AD9510 Defaults
            case 'L':
                item.name = FMC130M_4CH_NAME_AD9510_CFG_DEFAULTS;
                item.service = FMC130M_4CH_MODULE_NAME;
                item.rw = 0;
                append_item (call_list, item);
                break;

                //Get ADC Data
            case 'c':
                if ((err = parse_subopt (optarg, mount_opts, FMC130M_4CH_NAME_ADC_DATA0, corr_name, item.write_val)) != BPM_CLIENT_SUCCESS) {
                    fprintf(stderr, "%s: %s\n", program_name, bpm_client_err_str(err));
                    return -1;
                }
                item.name = strdup(corr_name);
                item.service = FMC130M_4CH_MODULE_NAME;
                item.rw = 1;
                *(item.write_val) = item.rw;
                append_item (call_list, item);
                free(item.name);
                break;

                //Set ADC Data
            case 'C':
                if ((err = parse_subopt (optarg, mount_opts, FMC130M_4CH_NAME_ADC_DATA0, corr_name, item.write_val)) != BPM_CLIENT_SUCCESS) {
                    fprintf(stderr, "%s: %s\n", program_name, bpm_client_err_str(err));
                    return -1;
                }
                item.name = strdup(corr_name);
                item.service = FMC130M_4CH_MODULE_NAME;
                item.rw = 0;
                *(item.write_val) = item.rw;
                append_item (call_list, item);
                free(item.name);
                break;

                //Get ADC Dly Value
            case getdlyval:
                if ((err = parse_subopt (optarg, mount_opts, FMC130M_4CH_NAME_ADC_DLY_VAL0, corr_name, item.write_val)) != BPM_CLIENT_SUCCESS) {
                    fprintf(stderr, "%s: %s\n", program_name, bpm_client_err_str(err));
                    return -1;
                }
                item.name = strdup(corr_name);
                item.service = FMC130M_4CH_MODULE_NAME;
                item.rw = 1;
                *(item.write_val) = item.rw;
                append_item (call_list, item);
                free(item.name);
                break;

                //Set ADC Dly Value
            case setdlyval:
                if ((err = parse_subopt (optarg, mount_opts, FMC130M_4CH_NAME_ADC_DLY_VAL0, corr_name, item.write_val)) != BPM_CLIENT_SUCCESS) {
                    fprintf(stderr, "%s: %s\n", program_name, bpm_client_err_str(err));
                    return -1;
                }
                item.name = strdup(corr_name);
                item.service = FMC130M_4CH_MODULE_NAME;
                item.rw = 0;
                *(item.write_val) = item.rw;
                append_item (call_list, item);
                free(item.name);
                break;

                //Get ADC Dly Line
            case getdlyline:
                if ((err = parse_subopt (optarg, mount_opts, FMC130M_4CH_NAME_ADC_DLY_LINE0, corr_name, item.write_val)) != BPM_CLIENT_SUCCESS) {
                    fprintf(stderr, "%s: %s\n", program_name, bpm_client_err_str(err));
                    return -1;
                }
                item.name = strdup(corr_name);
                item.service = FMC130M_4CH_MODULE_NAME;
                item.rw = 1;
                *(item.write_val) = item.rw;
                append_item (call_list, item);
                free(item.name);
                break;

                //Set ADC Dly Line
            case setdlyline:
                if ((err = parse_subopt (optarg, mount_opts, FMC130M_4CH_NAME_ADC_DLY_LINE0, corr_name, item.write_val)) != BPM_CLIENT_SUCCESS) {
                    fprintf(stderr, "%s: %s\n", program_name, bpm_client_err_str(err));
                    return -1;
                }
                item.name = strdup(corr_name);
                item.service = FMC130M_4CH_MODULE_NAME;
                item.rw = 0;
                *(item.write_val) = item.rw;
                append_item (call_list, item);
                free(item.name);
                break;

                //Get ADC Dly Update
            case getdlyupdt:
                if ((err = parse_subopt (optarg, mount_opts, FMC130M_4CH_NAME_ADC_DLY_UPDT0, corr_name, item.write_val)) != BPM_CLIENT_SUCCESS) {
                    fprintf(stderr, "%s: %s\n", program_name, bpm_client_err_str(err));
                    return -1;
                }
                item.name = strdup(corr_name);
                item.service = FMC130M_4CH_MODULE_NAME;
                item.rw = 1;
                *(item.write_val) = item.rw;
                append_item (call_list, item);
                free(item.name);
                break;

                //Set ADC Dly Update
            case setdlyupdt:
                if ((err = parse_subopt (optarg, mount_opts, FMC130M_4CH_NAME_ADC_DLY_UPDT0, corr_name, item.write_val)) != BPM_CLIENT_SUCCESS) {
                    fprintf(stderr, "%s: %s\n", program_name, bpm_client_err_str(err));
                    return -1;
                }
                item.name = strdup(corr_name);
                item.service = FMC130M_4CH_MODULE_NAME;
                item.rw = 0;
                *(item.write_val) = item.rw;
                append_item (call_list, item);
                free(item.name);
                break;

                //Set ADC Dly
            case 'V':
                if ((err = parse_subopt (optarg, mount_opts, FMC130M_4CH_NAME_ADC_DLY0, corr_name, item.write_val)) != BPM_CLIENT_SUCCESS) {
                    fprintf(stderr, "%s: %s\n", program_name, bpm_client_err_str(err));
                    return -1;
                }
                item.name = strdup(corr_name);
                item.service = FMC130M_4CH_MODULE_NAME;
                item.rw = 0;
                *(item.write_val) = item.rw;
                append_item (call_list, item);
                free(item.name);
                break;

                //Set Test_data_en
            case 'N':
                item.name = FMC130M_4CH_NAME_TEST_DATA_EN;
                item.service = FMC130M_4CH_MODULE_NAME;
                item.rw = 0;
                *(item.write_val) = item.rw;
                *(item.write_val+4) = (uint32_t) strtoul(optarg, NULL, 10);
                append_item (call_list, item);
                break;

                //Get Test_data_en
            case 'n':
                item.name = FMC130M_4CH_NAME_TEST_DATA_EN;
                item.service = FMC130M_4CH_MODULE_NAME;
                item.rw = 1;
                *(item.write_val) = item.rw;
                append_item (call_list, item);
                break;

                //Set SI571 OE
            case 'O':
                item.name = FMC130M_4CH_NAME_SI571_OE;
                item.service = FMC130M_4CH_MODULE_NAME;
                item.rw = 0;
                *(item.write_val) = item.rw;
                *(item.write_val+4) = (uint32_t) strtoul(optarg, NULL, 10);
                append_item (call_list, item);
                break;

                //Get SI571 OE
            case 'o':
                item.name = FMC130M_4CH_NAME_SI571_OE;
                item.service = FMC130M_4CH_MODULE_NAME;
                item.rw = 1;
                *(item.write_val) = item.rw;
                append_item (call_list, item);
                break;

                //Set SI571 Frequency
            case 'i':
                item.name = FMC130M_4CH_NAME_SI571_SET_FREQ;
                item.service = FMC130M_4CH_MODULE_NAME;
                item.rw = 0;
                *(item.write_val) = item.rw;
                double *db_ptr = (double *)(item.write_val+4);
                *db_ptr = strtod(optarg, NULL);
                append_item (call_list, item);
                break;

                //Get SI571 Defaults
            case 'D':
                item.name = FMC130M_4CH_NAME_SI571_GET_DEFAULTS;
                item.service = FMC130M_4CH_MODULE_NAME;
                item.rw = 1;
                *(item.write_val) = item.rw;
                append_item (call_list, item);
                break;

                //Set AD9510 PLL A Divider 
            case 'a':
                item.name = FMC130M_4CH_NAME_AD9510_PLL_A_DIV;
                item.service = FMC130M_4CH_MODULE_NAME;
                item.rw = 0;
                *(item.write_val) = item.rw;
                *(item.write_val+4) = (uint32_t) strtoul(optarg, NULL, 10);
                append_item (call_list, item);
                break;

                //Set AD9510 PLL B Divider 
            case 'b':
                item.name = FMC130M_4CH_NAME_AD9510_PLL_B_DIV;
                item.service = FMC130M_4CH_MODULE_NAME;
                item.rw = 0;
                *(item.write_val) = item.rw;
                *(item.write_val+4) = (uint32_t) strtoul(optarg, NULL, 10);
                append_item (call_list, item);
                break;

                //Set AD9510 PLL Prescaler
            case 'r':
                item.name = FMC130M_4CH_NAME_AD9510_PLL_PRESCALER;
                item.service = FMC130M_4CH_MODULE_NAME;
                item.rw = 0;
                *(item.write_val) = item.rw;
                *(item.write_val+4) = (uint32_t) strtoul(optarg, NULL, 10);
                append_item (call_list, item);
                break;

                //Set AD9510 R Divider
            case 'R':
                item.name = FMC130M_4CH_NAME_AD9510_PLL_PRESCALER;
                item.service = FMC130M_4CH_MODULE_NAME;
                item.rw = 0;
                *(item.write_val) = item.rw;
                *(item.write_val+4) = (uint32_t) strtoul(optarg, NULL, 10);
                append_item (call_list, item);
                break;

                //Set AD9510 PLL PDown
            case 'B':
                item.name = FMC130M_4CH_NAME_AD9510_PLL_PDOWN;
                item.service = FMC130M_4CH_MODULE_NAME;
                item.rw = 0;
                *(item.write_val) = item.rw;
                *(item.write_val+4) = (uint32_t) strtoul(optarg, NULL, 10);
                append_item (call_list, item);
                break;

                //Set AD9510 MUX Status
            case 'M':
                item.name = FMC130M_4CH_NAME_AD9510_MUX_STATUS;
                item.service = FMC130M_4CH_MODULE_NAME;
                item.rw = 0;
                *(item.write_val) = item.rw;
                *(item.write_val+4) = (uint32_t) strtoul(optarg, NULL, 10);
                append_item (call_list, item);
                break;

                //Set AD9510 CP Current
            case 'u':
                item.name = FMC130M_4CH_NAME_AD9510_CP_CURRENT;
                item.service = FMC130M_4CH_MODULE_NAME;
                item.rw = 0;
                *(item.write_val) = item.rw;
                *(item.write_val+4) = (uint32_t) strtoul(optarg, NULL, 10);
                append_item (call_list, item);
                break;

                //Set AD9510 Outputs
            case 'U':
                item.name = FMC130M_4CH_NAME_AD9510_OUTPUTS;
                item.service = FMC130M_4CH_MODULE_NAME;
                item.rw = 0;
                *(item.write_val) = item.rw;
                *(item.write_val+4) = (uint32_t) strtoul(optarg, NULL, 10);
                append_item (call_list, item);
                break;

                //Set AD9510 PLL Clock Select
            case 'k':
                item.name = FMC130M_4CH_NAME_AD9510_PLL_CLK_SEL;
                item.service = FMC130M_4CH_MODULE_NAME;
                item.rw = 0;
                *(item.write_val) = item.rw;
                *(item.write_val+4) = (uint32_t) strtoul(optarg, NULL, 10);
                append_item (call_list, item);
                break;

                /****** DSP Functions ******/

                //Set Kx
            case setkx:
                item.name = DSP_NAME_SET_GET_KX;
                item.service = DSP_MODULE_NAME;
                item.rw = 0;
                *(item.write_val) = item.rw;
                *(item.write_val+4) = (uint32_t) strtoul(optarg, NULL, 10);
                append_item (call_list, item);
                break;

                //Get Kx
            case getkx:
                item.name = DSP_NAME_SET_GET_KX;
                item.service = DSP_MODULE_NAME;
                item.rw = 1;
                *(item.write_val) = item.rw;
                append_item (call_list, item);
                break;

                //Set Kx
            case setky:
                item.name = DSP_NAME_SET_GET_KY;
                item.service = DSP_MODULE_NAME;
                item.rw = 0;
                *(item.write_val) = item.rw;
                *(item.write_val+4) = (uint32_t) strtoul(optarg, NULL, 10);
                append_item (call_list, item);
                break;

                //Get Ky
            case getky:
                item.name = DSP_NAME_SET_GET_KY;
                item.service = DSP_MODULE_NAME;
                item.rw = 1;
                *(item.write_val) = item.rw;
                append_item (call_list, item);
                break;

                //Set Ksum
            case setksum:
                item.name = DSP_NAME_SET_GET_KSUM;
                item.service = DSP_MODULE_NAME;
                item.rw = 0;
                *(item.write_val) = item.rw;
                *(item.write_val+4) = (uint32_t) strtoul(optarg, NULL, 10);
                append_item (call_list, item);
                break;

                //Get Ksum
            case getksum:
                item.name = DSP_NAME_SET_GET_KSUM;
                item.service = DSP_MODULE_NAME;
                item.rw = 1;
                *(item.write_val) = item.rw;
                append_item (call_list, item);
                break;

                //Set TBT Thres
            case settbtth:
                item.name = DSP_NAME_SET_GET_DS_TBT_THRES;
                item.service = DSP_MODULE_NAME;
                item.rw = 0;
                *(item.write_val) = item.rw;
                *(item.write_val+4) = (uint32_t) strtoul(optarg, NULL, 10);
                append_item (call_list, item);
                break;

                //Get TBT Thres
            case gettbtth:
                item.name = DSP_NAME_SET_GET_DS_TBT_THRES;
                item.service = DSP_MODULE_NAME;
                item.rw = 1;
                *(item.write_val) = item.rw;
                append_item (call_list, item);
                break;

                //Set FOFB Thres
            case setfofbth:
                item.name = DSP_NAME_SET_GET_DS_FOFB_THRES;
                item.service = DSP_MODULE_NAME;
                item.rw = 0;
                *(item.write_val) = item.rw;
                *(item.write_val+4) = (uint32_t) strtoul(optarg, NULL, 10);
                append_item (call_list, item);
                break;

                //Get FOFB Thres
            case getfofbth:
                item.name = DSP_NAME_SET_GET_DS_FOFB_THRES;
                item.service = DSP_MODULE_NAME;
                item.rw = 1;
                *(item.write_val) = item.rw;
                append_item (call_list, item);
                break;

                //Set Monit Thres
            case setmonitth:
                item.name = DSP_NAME_SET_GET_DS_MONIT_THRES;
                item.service = DSP_MODULE_NAME;
                item.rw = 0;
                *(item.write_val) = item.rw;
                *(item.write_val+4) = (uint32_t) strtoul(optarg, NULL, 10);
                append_item (call_list, item);
                break;

                //Get Monit Thres
            case getmonitth:
                item.name = DSP_NAME_SET_GET_DS_MONIT_THRES;
                item.service = DSP_MODULE_NAME;
                item.rw = 1;
                *(item.write_val) = item.rw;
                append_item (call_list, item);
                break;

                //Set Monit Position X
            case 'X':
                item.name = DSP_NAME_SET_GET_MONIT_POS_X;
                item.service = DSP_MODULE_NAME;
                item.rw = 0;
                *(item.write_val) = item.rw;
                *(item.write_val+4) = (uint32_t) strtoul(optarg, NULL, 10);
                append_item (call_list, item);
                break;

                //Get Monit Position X
            case 'x':
                item.name = DSP_NAME_SET_GET_MONIT_POS_X;
                item.service = DSP_MODULE_NAME;
                item.rw = 1;
                *(item.write_val) = item.rw;
                append_item (call_list, item);
                break;

                //Set Monit Position Y
            case 'Y':
                item.name = DSP_NAME_SET_GET_MONIT_POS_Y;
                item.service = DSP_MODULE_NAME;
                item.rw = 0;
                *(item.write_val) = item.rw;
                *(item.write_val+4) = (uint32_t) strtoul(optarg, NULL, 10);
                append_item (call_list, item);
                break;

                //Get Monit Position Y
            case 'y':
                item.name = DSP_NAME_SET_GET_MONIT_POS_Y;
                item.service = DSP_MODULE_NAME;
                item.rw = 1;
                *(item.write_val) = item.rw;
                append_item (call_list, item);
                break;

                //Set Monit Position Q
            case 'Q':
                item.name = DSP_NAME_SET_GET_MONIT_POS_Q;
                item.service = DSP_MODULE_NAME;
                item.rw = 0;
                *(item.write_val) = item.rw;
                *(item.write_val+4) = (uint32_t) strtoul(optarg, NULL, 10);
                append_item (call_list, item);
                break;

                //Get Monit Position Q
            case 'q':
                item.name = DSP_NAME_SET_GET_MONIT_POS_Q;
                item.service = DSP_MODULE_NAME;
                item.rw = 1;
                *(item.write_val) = item.rw;
                append_item (call_list, item);
                break;

                //Set Monit Position SUM
            case 'S':
                item.name = DSP_NAME_SET_GET_MONIT_POS_SUM;
                item.service = DSP_MODULE_NAME;
                item.rw = 0;
                *(item.write_val) = item.rw;
                *(item.write_val+4) = (uint32_t) strtoul(optarg, NULL, 10);
                append_item (call_list, item);
                break;

                //Get Monit Position SUM
            case 's':
                item.name = DSP_NAME_SET_GET_MONIT_POS_SUM;
                item.service = DSP_MODULE_NAME;
                item.rw = 1;
                *(item.write_val) = item.rw;
                append_item (call_list, item);
                break;

                //Get Monit AMP
            case 'J':
                if ((err = parse_subopt (optarg, mount_opts, DSP_NAME_SET_GET_MONIT_AMP_CH0, corr_name, item.write_val)) != BPM_CLIENT_SUCCESS) {
                    fprintf(stderr, "%s: %s\n", program_name, bpm_client_err_str(err));
                    return -1;
                }
                item.name = strdup(corr_name);
                item.service = DSP_MODULE_NAME;
                item.rw = 1;
                *(item.write_val) = item.rw;
                append_item (call_list, item);
                free(item.name);
                break;

                //Set Monit AMP
            case 'j':
                if ((err = parse_subopt (optarg, mount_opts, DSP_NAME_SET_GET_MONIT_AMP_CH0, corr_name, item.write_val)) != BPM_CLIENT_SUCCESS) {
                    fprintf(stderr, "%s: %s\n", program_name, bpm_client_err_str(err));
                    return -1;
                }
                item.name = strdup(corr_name);
                item.service = DSP_MODULE_NAME;
                item.rw = 0;
                *(item.write_val) = item.rw;
                append_item (call_list, item);
                free(item.name);
                break;

                /******** SWAP Module Functions ********/

                //Set SW
            case 'W':
                item.name = SWAP_NAME_SET_GET_SW;
                item.service = SWAP_MODULE_NAME;
                item.rw = 0;
                *(item.write_val) = item.rw;
                *(item.write_val+4) = (uint32_t) strtoul(optarg, NULL, 10);
                append_item (call_list, item);
                break;

                //Get SW
            case 'w':
                item.name = SWAP_NAME_SET_GET_SW;
                item.service = SWAP_MODULE_NAME;
                item.rw = 1;
                *(item.write_val) = item.rw;
                append_item (call_list, item);
                break;

                //Set SW Enable
            case 'T':
                item.name = SWAP_NAME_SET_GET_SW_EN;
                item.service = SWAP_MODULE_NAME;
                item.rw = 0;
                *(item.write_val) = item.rw;
                *(item.write_val+4) = (uint32_t) strtoul(optarg, NULL, 10);
                append_item (call_list, item);
                break;

                //Get SW Enable
            case 't':
                item.name = SWAP_NAME_SET_GET_SW_EN;
                item.service = SWAP_MODULE_NAME;
                item.rw = 1;
                *(item.write_val) = item.rw;
                append_item (call_list, item);
                break;

                //Set SW Delay
            case 'F':
                item.name = SWAP_NAME_SET_GET_SW_DLY;
                item.service = SWAP_MODULE_NAME;
                item.rw = 0;
                *(item.write_val) = item.rw;
                *(item.write_val+4) = (uint32_t) strtoul(optarg, NULL, 10);
                append_item (call_list, item);
                break;

                //Get SW Delay
            case 'f':
                item.name = SWAP_NAME_SET_GET_SW_DLY;
                item.service = SWAP_MODULE_NAME;
                item.rw = 1;
                *(item.write_val) = item.rw;
                append_item (call_list, item);
                break;

                //Set Div Clock
            case 'Z':
                item.name = SWAP_NAME_SET_GET_DIV_CLK;
                item.service = SWAP_MODULE_NAME;
                item.rw = 0;
                *(item.write_val) = item.rw;
                *(item.write_val+4) = (uint32_t) strtoul(optarg, NULL, 10);
                append_item (call_list, item);
                break;

                //Get Div Clock
            case 'z':
                item.name = SWAP_NAME_SET_GET_DIV_CLK;
                item.service = SWAP_MODULE_NAME;
                item.rw = 1;
                *(item.write_val) = item.rw;
                append_item (call_list, item);
                break;

                //Set WDW Enable
            case setwdwen:
                item.name = SWAP_NAME_SET_GET_WDW_EN;
                item.service = SWAP_MODULE_NAME;
                item.rw = 0;
                *(item.write_val) = item.rw;
                *(item.write_val+4) = (uint32_t) strtoul(optarg, NULL, 10);
                append_item (call_list, item);
                break;

                //Get WDW Enable
            case getwdwen:
                item.name = SWAP_NAME_SET_GET_WDW_EN;
                item.service = SWAP_MODULE_NAME;
                item.rw = 1;
                *(item.write_val) = item.rw;
                append_item (call_list, item);
                break;

                //Set WDW Delay
            case setwdwdly:
                item.name = SWAP_NAME_SET_GET_WDW_DLY;
                item.service = SWAP_MODULE_NAME;
                item.rw = 0;
                *(item.write_val) = item.rw;
                *(item.write_val+4) = (uint32_t) strtoul(optarg, NULL, 10);
                append_item (call_list, item);
                break;

                //Get WDW Delay
            case getwdwdly:
                item.name = SWAP_NAME_SET_GET_WDW_DLY;
                item.service = SWAP_MODULE_NAME;
                item.rw = 1;
                *(item.write_val) = item.rw;
                append_item (call_list, item);
                break;

                //Get FE Channel A Gain
            case getgaina:
                item.name = SWAP_NAME_SET_GET_GAIN_A;
                item.service = SWAP_MODULE_NAME;
                item.rw = 1;
                *(item.write_val) = item.rw;
                append_item (call_list, item);
                break;

                //Set FE Channel A Gain
            case setgaina:
                item.name = SWAP_NAME_SET_GET_GAIN_A;
                item.service = SWAP_MODULE_NAME;
                item.rw = 0;
                *(item.write_val) = item.rw;
                *(item.write_val+4) = (uint32_t) strtoul(optarg, NULL, 10);
                append_item (call_list, item);
                break;

                //Get FE Channel B Gain
            case getgainb:
                item.name = SWAP_NAME_SET_GET_GAIN_B;
                item.service = SWAP_MODULE_NAME;
                item.rw = 1;
                *(item.write_val) = item.rw;
                append_item (call_list, item);
                break;

                //Set FE Channel B Gain
            case setgainb:
                item.name = SWAP_NAME_SET_GET_GAIN_B;
                item.service = SWAP_MODULE_NAME;
                item.rw = 0;
                *(item.write_val) = item.rw;
                *(item.write_val+4) = (uint32_t) strtoul(optarg, NULL, 10);
                append_item (call_list, item);
                break;

                //Get FE Channel C Gain
            case getgainc:
                item.name = SWAP_NAME_SET_GET_GAIN_C;
                item.service = SWAP_MODULE_NAME;
                item.rw = 1;
                *(item.write_val) = item.rw;
                append_item (call_list, item);
                break;

                //Set FE Channel C Gain
            case setgainc:
                item.name = SWAP_NAME_SET_GET_GAIN_C;
                item.service = SWAP_MODULE_NAME;
                item.rw = 0;
                *(item.write_val) = item.rw;
                *(item.write_val+4) = (uint32_t) strtoul(optarg, NULL, 10);
                append_item (call_list, item);
                break;

                //Get FE Channel D Gain
            case getgaind:
                item.name = SWAP_NAME_SET_GET_GAIN_D;
                item.service = SWAP_MODULE_NAME;
                item.rw = 1;
                *(item.write_val) = item.rw;
                append_item (call_list, item);
                break;

                //Set FE Channel D Gain
            case setgaind:
                item.name = SWAP_NAME_SET_GET_GAIN_D;
                item.service = SWAP_MODULE_NAME;
                item.rw = 0;
                *(item.write_val) = item.rw;
                *(item.write_val+4) = (uint32_t) strtoul(optarg, NULL, 10);
                append_item (call_list, item);
                break;

                /******** RFFE Module Functions *******/

                //Set RFFE Switching
            case rffesetsw:
                item.name = RFFE_NAME_SET_GET_SW;
                item.service = RFFE_MODULE_NAME;
                item.rw = 0;
                *(item.write_val) = item.rw;
                *(item.write_val+4) = (uint32_t) strtoul(optarg, NULL, 10);
                append_item (call_list, item);
                break;

                //Get RFFE Switching
            case rffegetsw:
                item.name = RFFE_NAME_SET_GET_SW;
                item.service = RFFE_MODULE_NAME;
                item.rw = 1;
                *(item.write_val) = item.rw;
                append_item (call_list, item);
                break;

                //Set RFFE Attenuators
            case rffesetatt:
                if ((err = parse_subopt (optarg, mount_opts, RFFE_NAME_SET_GET_ATT1, corr_name, item.write_val)) != BPM_CLIENT_SUCCESS) {
                    fprintf(stderr, "%s: %s\n", program_name, bpm_client_err_str(err));
                    return -1;
                }
                item.name = strdup(corr_name);
                item.service = RFFE_MODULE_NAME;
                item.rw = 0;
                *(item.write_val) = item.rw;
                append_item (call_list, item);
                free(item.name);
                break;

                //Get RFFE Attenuators
            case rffegetatt:
                if ((err = parse_subopt (optarg, mount_opts, RFFE_NAME_SET_GET_ATT1, corr_name, item.write_val)) != BPM_CLIENT_SUCCESS) {
                    fprintf(stderr, "%s: %s\n", program_name, bpm_client_err_str(err));
                    return -1;
                }
                item.name = strdup(corr_name);
                item.service = RFFE_MODULE_NAME;
                item.rw = 1;
                *(item.write_val) = item.rw;
                append_item (call_list, item);
                free(item.name);
                break;

                //Set RFFE Temperature
            case rffesettmp:
                if ((err = parse_subopt (optarg, mount_opts, RFFE_NAME_SET_GET_TEMP1, corr_name, item.write_val)) != BPM_CLIENT_SUCCESS) {
                    fprintf(stderr, "%s: %s\n", program_name, bpm_client_err_str(err));
                    return -1;
                }
                item.name = strdup(corr_name);
                item.service = RFFE_MODULE_NAME;
                item.rw = 0;
                *(item.write_val) = item.rw;
                append_item (call_list, item);
                free(item.name);
                break;

                //Read RFFE Temperature
            case rffegettmp:
                if ((err = parse_subopt (optarg, mount_opts, RFFE_NAME_SET_GET_TEMP1, corr_name, item.write_val)) != BPM_CLIENT_SUCCESS) {
                    fprintf(stderr, "%s: %s\n", program_name, bpm_client_err_str(err));
                    return -1;
                }
                item.name = strdup(corr_name);
                item.service = RFFE_MODULE_NAME;
                item.rw = 1;
                *(item.write_val) = item.rw;
                append_item (call_list, item);
                free(item.name);
                break;

                //Set RFFE Point
            case rffesetpnt:
                if ((err = parse_subopt (optarg, mount_opts, RFFE_NAME_SET_GET_SET_POINT1, corr_name, item.write_val)) != BPM_CLIENT_SUCCESS) {
                    fprintf(stderr, "%s: %s\n", program_name, bpm_client_err_str(err));
                    return -1;
                }
                item.name = strdup(corr_name);
                item.service = RFFE_MODULE_NAME;
                item.rw = 0;
                *(item.write_val) = item.rw;
                append_item (call_list, item);
                free(item.name);
                break;

                //Get RFFE Point
            case rffegetpnt:
                if ((err = parse_subopt (optarg, mount_opts, RFFE_NAME_SET_GET_SET_POINT1, corr_name, item.write_val)) != BPM_CLIENT_SUCCESS) {
                    fprintf(stderr, "%s: %s\n", program_name, bpm_client_err_str(err));
                    return -1;
                }
                item.name = strdup(corr_name);
                item.service = RFFE_MODULE_NAME;
                item.rw = 1;
                *(item.write_val) = item.rw;
                append_item (call_list, item);
                free(item.name);
                break;

                //Set RFFE Temperature Control
            case rffesettmpctr:
                item.name = RFFE_NAME_SET_GET_TEMP_CONTROL;
                item.service = RFFE_MODULE_NAME;
                item.rw = 0;
                *(item.write_val) = item.rw;
                *(item.write_val+4) = (uint32_t) strtoul(optarg, NULL, 10);
                append_item (call_list, item);
                break;

                //Get RFFE Temperature Control
            case rffegettmpctr:
                item.name = RFFE_NAME_SET_GET_TEMP_CONTROL;
                item.service = RFFE_MODULE_NAME;
                item.rw = 1;
                *(item.write_val) = item.rw;
                append_item (call_list, item);
                break;

                //Set RFFE Output
            case rffesetout:
                if ((err = parse_subopt (optarg, mount_opts, RFFE_NAME_SET_GET_OUTPUT1, corr_name, item.write_val)) != BPM_CLIENT_SUCCESS) {
                    fprintf(stderr, "%s: %s\n", program_name, bpm_client_err_str(err));
                    return -1;
                }
                item.name = strdup(corr_name);
                item.service = RFFE_MODULE_NAME;
                item.rw = 0;
                *(item.write_val) = item.rw;
                append_item (call_list, item);
                free(item.name);
                break;

                //Get RFFE Output
            case rffegetout:
                if ((err = parse_subopt (optarg, mount_opts, RFFE_NAME_SET_GET_OUTPUT1, corr_name, item.write_val)) != BPM_CLIENT_SUCCESS) {
                    fprintf(stderr, "%s: %s\n", program_name, bpm_client_err_str(err));
                    return -1;
                }
                item.name = strdup(corr_name);
                item.service = RFFE_MODULE_NAME;
                item.rw = 1;
                *(item.write_val) = item.rw;
                append_item (call_list, item);
                free(item.name);
                break;

                //Set RFFE Reset
            case rffesetrst:
                item.name = RFFE_NAME_SET_GET_RESET;
                item.service = RFFE_MODULE_NAME;
                item.rw = 0;
                *(item.write_val) = item.rw;
                *(item.write_val+4) = (uint32_t) strtoul(optarg, NULL, 10);
                append_item (call_list, item);
                break;

                //Get RFFE Reset
            case rffegetrst:
                item.name = RFFE_NAME_SET_GET_RESET;
                item.service = RFFE_MODULE_NAME;
                item.rw = 1;
                *(item.write_val) = item.rw;
                append_item (call_list, item);
                break;

                //Set RFFE Reprog
            case rffesetrpg:
                item.name = RFFE_NAME_SET_GET_REPROG;
                item.service = RFFE_MODULE_NAME;
                item.rw = 0;
                *(item.write_val) = item.rw;
                *(item.write_val+4) = (uint32_t) strtoul(optarg, NULL, 10);
                append_item (call_list, item);
                break;

                //Get RFFE Reprog
            case rffegetrpg:
                item.name = RFFE_NAME_SET_GET_REPROG;
                item.service = RFFE_MODULE_NAME;
                item.rw = 1;
                *(item.write_val) = item.rw;
                append_item (call_list, item);
                break;

                //Set RFFE Switch Level
            case rffesetswlvl:
                item.name = RFFE_NAME_SET_GET_SW_LVL;
                item.service = RFFE_MODULE_NAME;
                item.rw = 0;
                *(item.write_val) = item.rw;
                *(item.write_val+4) = (uint32_t) strtoul(optarg, NULL, 10);
                append_item (call_list, item);
                break;

                //Get RFFE Switch Level
            case rffegetswlvl:
                item.name = RFFE_NAME_SET_GET_SW_LVL;
                item.service = RFFE_MODULE_NAME;
                item.rw = 1;
                *(item.write_val) = item.rw;
                append_item (call_list, item);
                break;

                /******** ACQ Module Functions ********/

                // Set Acq Samples
            case 'E':
                acq_samples_set = 1;
                acq_samples_val = (uint32_t) strtoul(optarg, NULL, 10);
                break;

                // Set Acq Chan
            case 'H':
                acq_chan_set = 1;
                acq_chan_val = (uint32_t) strtoul(optarg, NULL, 10);
                break;

                // Set Acq Start
            case 'I':
                acq_start = 1;
                break;

                // Check if the acquisition is finished
            case 'K':
                acq_check = 1;
                check_poll = 0;
                break;

                // Check if the acquisition is finished until timeout (-1 for infinite)
            case acqcheckpoll:
                acq_check = 1;
                check_poll = 1;
                break;

                // Get a single data block from the acquisition
            case 'A':
                acq_get_block = 1;
                acq_block_id = (uint32_t) strtoul(optarg, NULL, 10);
                break;

                // Perform full acq
            case fullacq:
                acq_full_call = 1;
                break;

                // Set Polling timeout
            case timeout:
                poll_timeout = (int) strtoul(optarg, NULL, 10);
                break;

            default:
                fprintf(stderr, "%s: bad option\n", program_name);
                print_usage(stderr, 1);
        }
    }

    /* User input error handling */

    //Use default local broker endpoint if none was given
    if (broker_endp == NULL){
        broker_endp = strdup(default_broker_endp);
    }

    //Check if the board number is within range and set to default if necessary
    if (board_number_str == NULL) {
        fprintf (stderr, "[client]: Setting default value to BOARD number: %u\n",
                DFLT_BOARD_NUMBER);
        board_number = DFLT_BOARD_NUMBER;
    } else {
        board_number = strtoul (board_number_str, NULL, 10);
        if (board_number > MAX_BOARD_NUMBER) {
            fprintf (stderr, "[client]: BOARD number too big! Defaulting to: %u\n",
                    MAX_BOARD_NUMBER);
            board_number = MAX_BOARD_NUMBER;
        }
    }

    //Check if the bpm number is within range and set to default if necessary
    if (bpm_number_str == NULL) {
        fprintf (stderr, "[client]: Setting default value to BPM number: %u\n",
                DFLT_BPM_NUMBER);
        bpm_number = DFLT_BPM_NUMBER;
    } else {
        bpm_number = strtoul (bpm_number_str, NULL, 10);

        if (bpm_number > MAX_BPM_NUMBER) {
            fprintf (stderr, "[client]: BPM number too big! Defaulting to: %u\n",
                    MAX_BPM_NUMBER);
            bpm_number = MAX_BPM_NUMBER;
        }
    }

    // Both Acq Chan and Acq Samples must be set or none of them
    if ((acq_samples_set && !acq_chan_set) || (!acq_samples_set && acq_chan_set)) {
        fprintf(stderr, "%s: If --setsamples or --setchan is set the other must be too!\n", program_name);
        return -1;
    }

    if ( (acq_start || acq_get_block || acq_full_call) && (!acq_samples_set || !acq_chan_set)) {
        if (acq_start) {
            fprintf(stderr, "%s: If --acqstart is requested, --setchan and --setsamples must be set!\n", program_name);
            }
        if (acq_get_block) {
            fprintf(stderr, "%s: To receive a data block, --setsamples and --setchan must be set!\n", program_name);
            }
        if (acq_full_call) {
            fprintf(stderr, "%s: If --fullacq is requested, --setchan and --setsamples must be set!\n", program_name);
            }
        return -1;
    }

    if (acq_chan_set && (acq_chan_val >= END_CHAN_ID)) {
        fprintf(stderr, "%s: Invalid channel selected! This value must be lower than %u \n", program_name, END_CHAN_ID);
        return -1;
    }

    if ((acq_check && check_poll) && (poll_timeout == 0)) {
        fprintf(stderr, "%s: If --acqcheckpoll is set, --timeout must be too!\n", program_name);
        return -1;
    }

    if (acq_full_call && (poll_timeout == 0)) {
        fprintf(stderr, "%s: If --fullacq is requested, --timeout must be set!\n", program_name);
        return -1;
    }

    if (acq_full_call && (acq_start || acq_check || acq_get_block )) {
        printf("%s: If --fullacq is requested, the other acquisition functions dont need to be called. Executing -fullacq only...\n", program_name);
        acq_start = 0;
        acq_check = 0;
        acq_get_block = 0;
    }

    /* If we are here, all the parameters are good and the functions can be executed */
    bpm_client_t *bpm_client = bpm_client_new (broker_endp, verbose, NULL);
    if (bpm_client == NULL) {
        fprintf(stderr, "[client]: Error in memory allocation for bpm_client\n");
    }

    /* Call all functions from the FMC130M_4CH, SWAP and DSP Module that the user specified */
    call_func_t* function = (call_func_t *)zlist_first (call_list);

    for ( ; function != NULL; function = zlist_next (call_list))
    {
        int str_length = snprintf(NULL, 0, "BPM%u:DEVIO:%s%u", board_number, function->service, bpm_number);
        char *func_service = zmalloc (str_length+1);
        sprintf (func_service, "BPM%u:DEVIO:%s%u", board_number, function->service, bpm_number);
        const disp_op_t* func_structure = bpm_func_translate (function->name);
        bpm_client_err_e err = bpm_func_exec (bpm_client, func_structure, func_service, (uint8_t *)function->write_val,
                (uint8_t *)function->read_val);

        if (err != BPM_CLIENT_SUCCESS) {
            fprintf (stderr, "[client]: BPM_ERROR: '%s'\n",bpm_client_err_str (err));
            return -1;
        }

        if (func_structure->retval != DISP_ARG_END && function->rw) {
            print_func_v(1, function);
        }
        free (func_service);
    }
    zlist_destroy (&call_list);

    /***** Acquisition module routines *****/
    int str_length = snprintf(NULL, 0, "BPM%u:DEVIO:ACQ%u", board_number, bpm_number);
    char *acq_service = zmalloc (str_length+1);
    sprintf (acq_service, "BPM%u:DEVIO:ACQ%u", board_number, bpm_number);
    /* Request data acquisition on server */
    if (acq_start) {
    //Wrap the data request parameters
        acq_req_t acq_req = {
            acq_samples_val,        /* Number of samples */
            acq_chan_val            /* Acquisition channel number */
        };
        bpm_client_err_e err = bpm_acq_start(bpm_client, acq_service, &acq_req);
        fprintf (stderr, "[client:acq]: BPM_ERR: '%s'\n", bpm_client_err_str(err));
    }

    /* Check if the previous acquisition has finished */
    if (acq_check) {
        if (check_poll) {
            func_polling (bpm_client, ACQ_NAME_CHECK_DATA_ACQUIRE, acq_service, NULL, NULL, poll_timeout);
        } else {
            bpm_client_err_e err = bpm_acq_check(bpm_client, acq_service);
            fprintf (stderr, "[client:acq]: BPM_ERR: '%s'\n", bpm_client_err_str(err));
        }
    }

    /* Retrieve specific data block */
    if (acq_get_block) {
        uint32_t data_size = acq_samples_val*acq_chan[acq_chan_val].sample_size;
        uint32_t *valid_data = (uint32_t *) zmalloc (data_size*sizeof (uint8_t));
        acq_trans_t acq_trans = {
            .req = {
                .chan = acq_chan_val,
            },
            .block = {
                .idx = acq_block_id,
                .data = valid_data,
                .data_size = data_size
            }
        };

        bpm_client_err_e err = bpm_acq_get_data_block (bpm_client, acq_service, &acq_trans);

        if (err == BPM_CLIENT_SUCCESS) {
            fprintf (stdout, "[client:acq]: bpm_get_block was successfully executed\n");
            print_data_curve (acq_chan_val, acq_trans.block.data, acq_trans.block.bytes_read);
        } else {
            fprintf (stderr, "[client:acq]: bpm_get_block failed\n");
            fprintf (stderr, "[client:acq]: BPM_ERR: '%s'\n", bpm_client_err_str(err));
        }
        free(valid_data);
    }

    /* Perform a full acquisition routine and return a data curve */
    if (acq_full_call) {
        uint32_t data_size = acq_samples_val*acq_chan[acq_chan_val].sample_size;
        uint32_t *valid_data = (uint32_t *) zmalloc (data_size*sizeof (uint8_t));

        acq_trans_t acq_trans = {
            .req = {
                .chan = acq_chan_val,
                .num_samples = acq_samples_val },
            .block = {
                .data = valid_data,
                .data_size = data_size }
        };

        bpm_client_err_e err = bpm_full_acq(bpm_client, acq_service, &acq_trans, poll_timeout);

        if (err == BPM_CLIENT_SUCCESS) {
            print_data_curve (acq_chan_val, acq_trans.block.data, acq_trans.block.bytes_read);
            fprintf (stdout, "[client:acq]: bpm_acq_full_call was successfully executed\n");
        } else {
            fprintf (stderr, "[client:acq]: bpm_acq_full_call failed\n");
            fprintf (stderr, "[client:acq]: BPM_ERR: '%s'\n", bpm_client_err_str(err));
        }
        acq_full_call = 0;
        free(valid_data);
    }

    //Deallocate memory
    free (default_broker_endp);
    free (broker_endp);
    free (acq_service);
    free (board_number_str);
    free (bpm_number_str);
    free (corr_name);
    bpm_client_destroy (&bpm_client);
    return 0;
}
