/*
 *  *  * Example for demonstrating the communication between a
 *   *   * a FOFB Processing module and a client
 *    *    */

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
  {"ram_write_en",        required_argument,   NULL, 'w'},
  {"ram_data_in",         required_argument,   NULL, 'd'},
  {"ram_addr",            required_argument,   NULL, 'a'},
  {NULL, 0, NULL, 0}
};

static const char* shortopt = "hb:vo:s:w:d:a:";

void print_help (char *program_name)
{
  fprintf (stdout, "HALCSD FOFB Controller control utility\n"
          "Usage: %s [options]\n"
          "\n"
          "  -h  --help                                    Display this usage information\n"
          "  -b  --brokerendp <Broker endpoint>            Broker endpoint\n"
          "  -v  --verbose                                 Verbose output\n"
          "  -o  --boardslot <Board slot number = [1-12]>  Board slot number\n"
          "  -s  --halcsnumber <HALCS number = [0|1]>      HALCS number\n"
          "  -w  --ram_write_en <[Read = 0 | Write = 1]>   RAM write enable\n"
          "  -d  --ram_data_in                             RAM data\n"
          "  -a  --ram_addr                                RAM addr\n",
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
  int verbose = 1;
  char *broker_endp = NULL;
  char *board_number_str = NULL;
  char *halcs_number_str = NULL;
  char *ram_write_en_str = NULL;
  char *ram_data_in_str = NULL;
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
        ram_write_en_str = strdup (optarg);
        break;

      case 'd':
        ram_data_in_str = strdup (optarg);
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

  halcs_client_err_e err;

  halcs_client_t *halcs_client = halcs_client_new (broker_endp, verbose, NULL);

  if (halcs_client == NULL) {
    fprintf (stderr, "[client:DSP]: halcs_client could be created\n");
    ret = 1;
    goto err_halcs_client_new;
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

  char service[50];
  snprintf (service, sizeof (service), "HALCS%u:DEVIO:FOFB_PROCESSING%u", board_number, halcs_number);

  /* ------------------------------------------------------------------------------ RAM write enable */

  uint32_t ram_write_en;
  if (ram_write_en_str == NULL) {
    fprintf (stderr, "[client:fofb_processing]: ram_write_en >> option not recognized or missing argument\n");
            print_help (argv [0]);
            exit (1);
  }
  else {
    ram_write_en = strtoul (ram_write_en_str, NULL, 10);
  }

  /* ------------------------------------------------------------------------------ RAM data in */
  uint32_t ram_data_in;
  if (ram_data_in_str == NULL) {
    fprintf (stderr, "[client:fofb_processing]: ram_data_in >> no value");
  }
  else {
    ram_data_in = strtoul (ram_data_in_str, NULL, 10);
  }

  /* ------------------------------------------------------------------------------ RAM addr */
  uint32_t ram_addr;
  if (ram_addr_str == NULL) {
      fprintf (stderr, "[client:fofb_processing]: ram_addr >> option not recognized or missing argument\n");
              print_help (argv [0]);
              exit (1);
  }
  else {
    ram_addr = strtoul (ram_addr_str, NULL, 10);
  }
  fprintf (stdout, "[client:fofb_processing]: ram_addr: %u\n", ram_addr);

  /* get ram_data_out */

  uint32_t ram_data_out_get;

  err = halcs_get_fofb_processing_ram_data_out(halcs_client, service, &ram_data_out_get);

  if (err != HALCS_CLIENT_SUCCESS){
    fprintf (stderr, "[client:fofb_processing]: halcs_get_fofb_processing_ram_data_out failed\n");
    ret = 2;
    goto err_halcs_exit;
  }
  fprintf (stderr, "[client:fofb_processing]: halcs_get_fofb_processing_ram_data_out: %u\n", ram_data_out_get);

err_halcs_exit:
  /* Try to read up until the point where the error occurs, anyway */
  halcs_client_destroy (&halcs_client);

err_halcs_client_new:
  free (broker_endp);
  broker_endp = NULL;
  free (board_number_str);
  board_number_str = NULL;

  return ret;
}
