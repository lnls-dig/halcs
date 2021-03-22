/*
 *  * Simple example demonstrating the communication between
 *   * a client and the FPGA device
 *    */

#include <getopt.h>
#include <czmq.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <acq_client.h>

#define DFLT_BIND_FOLDER            "/tmp/halcs"

#define DFLT_NUM_SAMPLES            4096
#define DFLT_CHAN_NUM               0

#define DFLT_HALCS_NUMBER           0

#define DFLT_BOARD_NUMBER           0

#define MIN_NUM_TRIGGER_TYPE        0
#define MAX_NUM_TRIGGER_TYPE        3

#define MIN_NUM_SAMPLES             4
/* Arbitrary hard limits */
#define MAX_NUM_SAMPLES             (1 << 28)
#define MAX_NUM_CHANS               (1 << 8)
#define MIN_NUM_SHOTS               1
#define MAX_NUM_SHOTS               (1 << 16)

#define DFLT_FILE_FMT               0
#define DFLT_NEWACQ_NUM             1

typedef enum {
    TEXT = 0,
    BINARY,
    END_FILE_FMT
} file_fmt_e;

void print_sample (uint8_t *data, uint32_t atom_size)
{
    switch (atom_size) {
        case 2:
            printf ("%8d\t", *(int16_t *) data);
        break;

        case 4:
            printf ("%8d\t", *(int32_t *) data);
        break;

        case 8:
            printf ("%8" PRId64 "\t", *(int64_t *) data);
        break;

        default:
            printf ("%8d\t", *(int16_t *) data);
        break;
    }
}

void print_data (uint32_t *data, uint32_t sample_size,
    uint32_t atom_size, uint32_t num_atoms, uint32_t size, file_fmt_e file_fmt)
{
    uint8_t *raw_data = (uint8_t *) data;

    if (file_fmt == TEXT) {
        for (uint32_t i = 0; i < size/sample_size; i++) {
            if (zctx_interrupted) {
                break;
            }

            for (uint32_t atom = 0; atom < num_atoms; ++atom) {
                print_sample(raw_data + (atom_size)*((i*num_atoms)+atom),
                    atom_size);
            }
            printf ("\n");
        }
    }
    else if (file_fmt == BINARY) {
        fwrite (raw_data, atom_size, size/atom_size, stdout);
    }
}

static struct option long_options[] =
{
    {"help",                no_argument,         NULL, 'h'},
    {"brokerendp",          required_argument,   NULL, 'b'},
    {"verbose",             no_argument,         NULL, 'v'},
    {"halcsnumber",         required_argument,   NULL, 's'},
    {"boardslot",           required_argument,   NULL, 'o'},
    {"channumber",          required_argument,   NULL, 'c'},
    {"numsamples",          required_argument,   NULL, 'n'},
    {"postsamples",         required_argument,   NULL, 'p'},
    {"numshots",            required_argument,   NULL, 't'},
    {"triggertype",         required_argument,   NULL, 'g'},
    {"datatriggerthres",    required_argument,   NULL, 'e'},
    {"datatriggerpol",      required_argument,   NULL, 'l'},
    {"datatriggersel",      required_argument,   NULL, 'z'},
    {"datatriggerfilt",     required_argument,   NULL, 'i'},
    {"datatriggerchan",     required_argument,   NULL, 'r'},
    {"triggerdelay",        required_argument,   NULL, 'd'},
    {"filefmt",             required_argument,   NULL, 'f'},
    {"newacq",              required_argument,   NULL, 'a'},
    {NULL, 0, NULL, 0}
};

static const char* shortopt = "hb:vo:s:c:n:p:t:g:e:l:z:i:r:d:f:a:";

void print_help (char *program_name)
{
    fprintf (stdout, "HALCSD Acquisition Utility\n"
            "Usage: %s [options]\n"
            "\n"
            "  -h  --help                            Display this usage information\n"
            "  -b  --brokerendp <Broker endpoint>    Broker endpoint\n"
            "  -v  --verbose                         Verbose output\n"
            "  -o  --boardslot <Board slot number = [1-12]>\n"
            "                                        Board slot number\n"
            "  -s  --halcsnumber <HALCS number = [0|1]>\n"
            "                                        HALCS instance number\n"
            "  -c  --channumber <Channel>            Channel number\n"
            "                                            <Channel> is dependant on the FPGA firmware\n"
            "  -n  --numsamples <Number of samples>  Number of samples or number of pre-trigger\n"
            "                                            samples if using triggered mode\n"
            "  -p  --postsamples <Number of samples> Number of post-trigger samples\n"
            "                                            if using triggered mode\n"
            "  -t  --numshots <Number of shots>      Number of shots\n"
            "                                            if using triggered mode\n"
            "  -g  --triggertype <Trigger type>      Trigger type. 0 is immediate, 1 is external trigger,\n"
            "                                            2 is data-driver trigger and 3 is software trigger\n"
            "  -e  --datatriggerthres <Trigger threshold>\n"
            "                                        Trigger threshold level for data-driven trigger\n"
            "  -l  --datatriggerpol <Trigger polarity>\n"
            "                                        Trigger polarity. 0 is positive edge, 1 is negative edge\n"
            "  -z  --datatriggersel <Trigger data selection>\n"
            "                                        Trigger data selection within one channel to compare for threshold level\n"
            "  -i  --datatriggerfilt <Trigger hysteresis>\n"
            "                                        Trigger hysteresis value for data trigger detection\n"
            "  -r  --datatriggerchan <Trigger channel>\n"
            "                                        Trigger data channel to be compared with the threshold level\n"
            "  -d  --triggerdelay <Trigger delay>    Trigger delay for all triggers\n"
            "  -f  --filefmt <Output format = [0 = text | 1 = binary]>\n"
            "                                        Output format\n"
            "  -a  --newcq <Trigger new acquisition = [0 = no | 1 = yes]\n",
            program_name);
}

int main (int argc, char *argv [])
{
    int verbose = 0;
    char *broker_endp = NULL;
    char *num_samples_str = NULL;
    char *post_samples_str = NULL;
    char *num_shots_str = NULL;
    char *trigger_type_str = NULL;
    char *datatriggerthres_str = NULL;
    char *datatriggerpol_str = NULL;
    char *datatriggersel_str = NULL;
    char *datatriggerfilt_str = NULL;
    char *datatriggerchan_str = NULL;
    char *triggerdelay_str = NULL;
    char *board_number_str = NULL;
    char *halcs_number_str = NULL;
    char *chan_str = NULL;
    char *new_acq_str = NULL;
    char *file_fmt_str = NULL;
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
                break;

            case 'n':
                num_samples_str = strdup (optarg);
                break;

            case 'p':
                post_samples_str = strdup (optarg);
                break;

            case 't':
                num_shots_str = strdup (optarg);
                break;

            case 'g':
                trigger_type_str = strdup (optarg);
                break;

            case 'e':
               datatriggerthres_str = strdup (optarg);
               break;

            case 'l':
               datatriggerpol_str = strdup (optarg);
               break;

            case 'z':
               datatriggersel_str = strdup (optarg);
               break;

            case 'i':
               datatriggerfilt_str = strdup (optarg);
               break;

            case 'r':
               datatriggerchan_str = strdup (optarg);
               break;

            case 'd':
               triggerdelay_str = strdup (optarg);
               break;

            case 'a':
                new_acq_str = strdup (optarg);
                break;

            case 'f':
                file_fmt_str = strdup (optarg);
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

    /* Set default number samples */
    uint32_t num_samples = 0;
    if (num_samples_str == NULL) {
        fprintf (stderr, "[client:acq]: No number of samples spcified. Please use --numsamples option\n");
        goto err_opt_exit;
    }
    else {
        num_samples = strtoul (num_samples_str, NULL, 10);

        if (num_samples < MIN_NUM_SAMPLES) {
            fprintf (stderr, "[client:acq]: Number of Pre-Trigger samples too small. Minimum number is: %u\n",
                    MIN_NUM_SAMPLES);
            goto err_opt_exit;
        }
        else if (num_samples > MAX_NUM_SAMPLES) {
            fprintf (stderr, "[client:acq]: Number of Pre-Trigger samples too big. Maximum number is: %u\n",
                    MAX_NUM_SAMPLES);
            goto err_opt_exit;
        }
    }

    /* Set post samples */
    uint32_t post_samples = 0;
    if (post_samples_str != NULL) {
        post_samples = strtoul (post_samples_str, NULL, 10);

        if (post_samples > MAX_NUM_SAMPLES) {
            fprintf (stderr, "[client:acq]: Number of Post-Trigger samples too big. Maximum number is: %u\n",
                    MAX_NUM_SAMPLES);
            goto err_opt_exit;
        }
    }

    /* Set number os shots */
    uint32_t num_shots = 1;
    if (num_shots_str != NULL) {
        num_shots = strtoul (num_shots_str, NULL, 10);
        
        if (num_shots < MIN_NUM_SHOTS) {
            fprintf (stderr, "[client:acq]: Number of shots too small. Minimum number is: %u\n",
                    MIN_NUM_SHOTS);
            goto err_opt_exit;
        }
        else if (num_shots > MAX_NUM_SHOTS) {
            fprintf (stderr, "[client:acq]: Number of samples too big. Maximum number is: %u\n",
                    MAX_NUM_SHOTS);
            goto err_opt_exit;
        }
    }

    /* Parse trigger properties */

    uint32_t datatriggerthres = 0;
    if (datatriggerthres_str != NULL) {
        datatriggerthres = strtoul (datatriggerthres_str, NULL, 10);
    }

    uint32_t datatriggerpol = 0;
    if (datatriggerpol_str != NULL) {
        datatriggerpol = strtoul (datatriggerpol_str, NULL, 10);
    }

    uint32_t datatriggersel = 0;
    if (datatriggersel_str != NULL) {
        datatriggersel = strtoul (datatriggersel_str, NULL, 10);
    }

    uint32_t datatriggerfilt = 1;
    if (datatriggerfilt_str != NULL) {
        datatriggerfilt = strtoul (datatriggerfilt_str, NULL, 10);
    }

    uint32_t triggerdelay = 0;
    if (triggerdelay_str != NULL) {
        triggerdelay = strtoul (triggerdelay_str, NULL, 10);
    }

    uint32_t datatriggerchan = 0;
    if (datatriggerchan_str != NULL) {
        datatriggerchan = strtoul (datatriggerchan_str, NULL, 10);
    }

    /* Set trigger type */
    uint32_t trigger_type = 0;
    if (trigger_type_str != NULL) {
        trigger_type = strtoul (trigger_type_str, NULL, 10);

        if (trigger_type > MAX_NUM_TRIGGER_TYPE) {
            fprintf (stderr, "[client:acq]: Trigger type too big. Maximum number is: %u\n",
                    MAX_NUM_TRIGGER_TYPE);
            goto err_opt_exit;
        }
    }

    /* Set default channel */
    uint32_t chan;
    if (chan_str == NULL) {
        fprintf (stderr, "[client:acq]: No channel specified. Please use --chan option\n");
        goto err_opt_exit;
    }
    else {
        chan = strtoul (chan_str, NULL, 10);

        if (chan > MAX_NUM_CHANS-1) {
            fprintf (stderr, "[client:acq]: Channel number too big. Maximum number is: %u\n",
                    MAX_NUM_CHANS);
            goto err_opt_exit;
        }
    }

    /* Set default board number */
    uint32_t board_number;
    if (board_number_str == NULL) {
        fprintf (stderr, "[client:acq]: No board number specified. Please use --boardslot option\n");
        goto err_opt_exit;
    }
    else {
        board_number = strtoul (board_number_str, NULL, 10);
    }

    /* Set default halcs number */
    uint32_t halcs_number;
    if (halcs_number_str == NULL) {
        fprintf (stderr, "[client:acq]: No halcs instance specified. Please use --halcsnumber option\n");
        goto err_opt_exit;
    }
    else {
        halcs_number = strtoul (halcs_number_str, NULL, 10);
    }

    /* Set default file format */
    file_fmt_e file_fmt;
    if (file_fmt_str == NULL) {
        file_fmt = DFLT_FILE_FMT;
    }
    else {
        file_fmt = strtoul (file_fmt_str, NULL, 10);

        if (file_fmt > END_FILE_FMT-1) {
            fprintf (stderr, "[client:acq]: Invalid file format (-file_fmt)\n");
            goto err_opt_exit;
        }
    }

    halcs_client_err_e err = HALCS_CLIENT_SUCCESS;

    /* Set new_acq */
    uint32_t new_acq;
    if (new_acq_str == NULL) {
        new_acq = DFLT_NEWACQ_NUM;
    }
    else {
        new_acq = strtoul (new_acq_str, NULL, 10);

        if (new_acq != 0 && new_acq != 1) {
            fprintf (stderr, "[client:acq]: Invalid --new_acq option parameter\n");
            exit (1);
        }
    }

    char service[50];
    snprintf (service, sizeof (service), "HALCS%u:DEVIO:ACQ%u", board_number, halcs_number);

    acq_client_t *acq_client = acq_client_new (broker_endp, verbose, NULL);
    if (acq_client == NULL) {
        fprintf (stderr, "[client:acq]: acq_client could not be created\n");
        goto err_acq_client_new;
    }

    acq_set_fsm_stop (acq_client, service, 1);

    /* Set trigger properties */

    if (datatriggerthres_str != NULL) {
        halcs_client_err_e err = acq_set_data_trig_thres (acq_client, service, datatriggerthres);
        if (err != HALCS_CLIENT_SUCCESS){
            fprintf (stderr, "[client:acq]: acq_set_data_trig_thres failed\n");
            goto err_acq_set_trig;
        }
    }

    if (datatriggerpol_str != NULL) {
        err = acq_set_data_trig_pol (acq_client, service, datatriggerpol);
        if (err != HALCS_CLIENT_SUCCESS){
            fprintf (stderr, "[client:acq]: acq_set_data_trig_pol failed\n");
            goto err_acq_set_trig;
        }
    }

    if (datatriggersel_str != NULL) {
        err = acq_set_data_trig_sel (acq_client, service, datatriggersel);
        if (err != HALCS_CLIENT_SUCCESS){
            fprintf (stderr, "[client:acq]: acq_set_data_trig_sel failed\n");
            goto err_acq_set_trig;
        }
    }

    if (datatriggerfilt_str != NULL) {
        err = acq_set_data_trig_filt (acq_client, service, datatriggerfilt);
        if (err != HALCS_CLIENT_SUCCESS){
            fprintf (stderr, "[client:acq]: acq_set_data_trig_filt failed\n");
            goto err_acq_set_trig;
        }
    }

    if (datatriggerchan_str != NULL) {
        err =  acq_set_data_trig_chan (acq_client, service, datatriggerchan);
        if (err != HALCS_CLIENT_SUCCESS){
            fprintf (stderr, "[client:acq]: acq_set_data_trig_chan failed\n");
            goto err_acq_set_trig;
        }
    }

    if (triggerdelay_str != NULL) {
        err =  acq_set_hw_trig_dly (acq_client, service, triggerdelay);
        if (err != HALCS_CLIENT_SUCCESS){
            fprintf (stderr, "[client:acq]: acq_set_hw_trig_dly failed\n");
            goto err_acq_set_trig;
        }
    }

    /* Set trigger */
    err = acq_set_trig (acq_client, service, trigger_type);
    if (err != HALCS_CLIENT_SUCCESS){
        fprintf (stderr, "[client:acq]: acq_set_trig failed\n");
        goto err_acq_set_trig;
    }

    uint32_t req_ch_sample_size = 0;
    uint32_t req_ch_atom_width = 0;
    uint32_t req_ch_atom_size = 0;
    uint32_t req_ch_num_atoms = 0;
    err = halcs_get_acq_ch_sample_size (acq_client, service, chan, &req_ch_sample_size);
    err |= halcs_get_acq_ch_atom_width (acq_client, service, chan, &req_ch_atom_width);
    err |= halcs_get_acq_ch_num_atoms (acq_client, service, chan, &req_ch_num_atoms);
    if (err != HALCS_CLIENT_SUCCESS){
        fprintf (stderr, "[client:acq]: Getting channel properties failed\n");
        goto err_acq_get_prop;
    }

    /* bit to byte */
    req_ch_atom_size = req_ch_atom_width/8;

    uint32_t data_size = (num_samples+post_samples)*num_shots*req_ch_sample_size;
    uint32_t *data = (uint32_t *) zmalloc (data_size*sizeof (uint8_t));
    bool new_acq_arg = new_acq;
    acq_trans_t acq_trans = {.req =   {
                                        .num_samples_pre = num_samples,
                                        .num_samples_post = post_samples,
                                        .num_shots = num_shots,
                                        .chan = chan,
                                      },
                             .block = {
                                        .data = data,
                                        .data_size = data_size,
                                      }
                            };
    err = acq_full_compat (acq_client, service, &acq_trans, 50000, new_acq_arg);
    if (err != HALCS_CLIENT_SUCCESS){
        fprintf (stderr, "[client:acq]: acq_full_compat failed\n");
        goto err_acq_full_compat;
    }

    if (!freopen (NULL, "wb", stdout)) {
        fprintf (stderr, "[client:acq]: Could not set output mode to binary\n");
        goto err_set_file_mode;
    }
    print_data (data, req_ch_sample_size, req_ch_atom_size,
        req_ch_num_atoms, acq_trans.block.bytes_read, file_fmt);

err_set_file_mode:
err_acq_full_compat:
err_acq_get_prop:
err_acq_set_trig:
    acq_client_destroy (&acq_client);
err_acq_client_new:
err_opt_exit:
    free (datatriggerthres_str);
    datatriggerthres_str = NULL;
    free (datatriggerpol_str);
    datatriggerpol_str = NULL;
    free (datatriggersel_str);
    datatriggersel_str = NULL;
    free (datatriggerfilt_str);
    datatriggerfilt_str = NULL;
    free (datatriggerchan_str);
    datatriggerchan_str = NULL;
    free (triggerdelay_str);
    triggerdelay_str = NULL;
    free (file_fmt_str);
    file_fmt_str = NULL;
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

    return 0;
}
