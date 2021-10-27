/*
 *  * Example for demonstrating the communication between a
 *   * a FOFB Processing module and a client
 *    */

#include <getopt.h>
#include <czmq.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <halcs_client.h>

#define DFLT_BIND_FOLDER            "/tmp/malamute"

#define DFLT_HALCS_NUMBER           0
#define DFLT_BOARD_NUMBER           0

#define RAM_WR_EN                   1
#define RAM_DATA                    0
#define RAM_ADDR                    0

static struct option long_options[] =
{
    {"help",                no_argument,         NULL, 'h'},
    {"brokerendp",          required_argument,   NULL, 'b'},
    {"verbose",             no_argument,         NULL, 'v'},
    {"halcsnumber",         required_argument,   NULL, 's'},
    {"boardslot",           required_argument,   NULL, 'o'},
    {"ram_write",           required_argument,   NULL, 'w'},
    {"ram_data",            required_argument,   NULL, 'd'},
    {"ram_addr",            required_argument,   NULL, 'a'},
    {NULL, 0, NULL, 0}
};

static const char* shortopt = "hb:vo:s:w:d:a:";

void print_help (char *program_name)
{
    fprintf (stdout, "HALCSD FOFB Controller control utility\n"
            "Usage: %s [options]\n"
            "\n"
            "  -h  --help                                   Display this usage information\n"
            "  -b  --brokerendp <Broker endpoint>           Broker endpoint\n"
            "  -v  --verbose                                Verbose output\n"
            "  -o  --boardslot <Board slot number = [1-12]> Board slot number\n"
            "  -s  --halcsnumber <HALCS number = [0|1]>     HALCS number\n"
            "  -w  --ram_write                              RAM write enable\n"
            "  -d  --ram_data                               RAM data\n"
            "  -a  --ram_addr                               RAM addr\n",
            program_name);
}

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
    char *broker_endp = NULL;
    char *board_number_str = NULL;
    char *halcs_number_str = NULL;
    char *ram_write_str = NULL;
    char *ram_data_str = NULL;
    char *ram_addr_str = NULL;
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

            case 'w':
                ram_write_str = strdup (optarg);
                break;

            case 'd':
                ram_data_str = strdup (optarg);
                break;

            case 'a':
                ram_addr_str = strdup (optarg);
                break;

            case '?':
                fprintf (stderr, "[client:fofb_processing] Option not recognized or missing argument\n");
                print_help (argv [0]);
                exit (1);
                break;

            default:
                fprintf (stderr, "[client:fofb_processing] Could not parse options\n");
                print_help (argv [0]);
                exit (1);
         }
    }

    /* Set default broker address */
    if (broker_endp == NULL) {
        fprintf (stderr, "[client:fofb_processing]: Setting default broker endpoint: %s\n",
                "ipc://"DFLT_BIND_FOLDER);
        broker_endp = strdup ("ipc://"DFLT_BIND_FOLDER);
    }

    /* Set default board number */
    uint32_t board_number;
    if (board_number_str == NULL) {
        fprintf (stderr, "[client:fofb_processing]: Setting default value to BOARD number: %u\n",
                DFLT_BOARD_NUMBER);
        board_number = DFLT_BOARD_NUMBER;
    }
    else {
        board_number = strtoul (board_number_str, NULL, 10);
    }

    /* Set default halcs number */
    uint32_t halcs_number;
    if (halcs_number_str == NULL) {
        fprintf (stderr, "[client:fofb_processing]: Setting default value to HALCS number: %u\n",
                DFLT_HALCS_NUMBER);
        halcs_number = DFLT_HALCS_NUMBER;
    }
    else {
        halcs_number = strtoul (halcs_number_str, NULL, 10);
    }

    /* Set RAM write enable */
    uint32_t ram_write;
    if (ram_write_str == NULL) {
        fprintf (stderr, "[client:fofb_processing]: Setting RAM write enable: %u\n",
                RAM_WR_EN);
        ram_write = RAM_WR_EN;
    }
    else {
        ram_write = strtoul (ram_write_str, NULL, 10);
    }

    /* Set RAM data */
    uint32_t ram_data;
    if (ram_data_str == NULL) {
        fprintf (stderr, "[client:fofb_processing]: Setting RAM data: %u\n",
                RAM_DATA);
        ram_data = RAM_DATA;
    }
    else {
        ram_data = strtoul (ram_data_str, NULL, 10);
    }

    /* Set RAM addr */
    uint32_t ram_addr;
    if (ram_addr_str == NULL) {
        fprintf (stderr, "[client:fofb_processing]: Setting RAM addr: %u\n",
                RAM_ADDR);
        ram_addr = RAM_ADDR;
    }
    else {
        ram_addr = strtoul (ram_addr_str, NULL, 10);
    }

    char service[50];
    snprintf (service, sizeof (service), "HALCS%u:DEVIO:FOFB_PROCESSING%u", board_number, halcs_number);

    return ret;
}

