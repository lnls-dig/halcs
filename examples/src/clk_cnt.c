#include <getopt.h>
#include <czmq.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <halcs_client.h>
#include <stdlib.h>

#define DFLT_BIND_FOLDER            "/tmp/malamute"

#define DFLT_HALCS_NUMBER           0
#define DFLT_BOARD_NUMBER           0

static struct option long_options[] =
{
  {"brokerendp",          required_argument,   NULL, 'b'},
  {"halcsnumber",         required_argument,   NULL, 's'},
  {"boardslot",           required_argument,   NULL, 'o'},
  {NULL, 0, NULL, 0}
};

static const char* shortopt = "b:s:o:";

/* Our read function */
typedef halcs_client_err_e (*halcs_func_fp) (halcs_client_t *self, char *service,
        uint32_t chan, uint32_t *value);

int main (int argc, char *argv [])
{
  char *broker_endp = NULL;
  char *board_number_str = NULL;
  char *halcs_number_str = NULL;

  int opt;
  while ((opt = getopt_long (argc, argv, shortopt, long_options, NULL)) != -1) {
    /* Get the user selected options */
    switch (opt) {
      case 'b':
        broker_endp = optarg;
        break;
      case 'o':
        board_number_str = optarg;
        break;
      case 's':
        halcs_number_str = optarg;
        break;
      default:
        fprintf (stderr, "[client:clk_cnt] Could not parse options\n");
        exit (1);
      }
  }

  halcs_client_t *halcs_client = halcs_client_new (broker_endp, 1, NULL);

  if (halcs_client == NULL) {
    fprintf (stderr, "[client:DSP]: halcs_client couldn't be created\n");
	 return 1;
  }

  /* Set default broker address */
  if (broker_endp == NULL) {
    fprintf (stderr, "[client:fofb_processing]: Setting default broker endpoint: %s\n",
            "ipc://"DFLT_BIND_FOLDER);
    broker_endp = "ipc://"DFLT_BIND_FOLDER;
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
  snprintf (service, sizeof (service), "HALCS%u:DEVIO:CLK_CNT%u", board_number, halcs_number);

  halcs_set_clk_cnt_trig_act(halcs_client, service, 1);
  uint32_t val = 0;
  halcs_get_clk_cnt_cnt_snap(halcs_client, service, &val);
  printf("cnt_snap: %ju\n", (uintmax_t)val);

  /* Try to read up until the point where the error occurs, anyway */
  halcs_client_destroy (&halcs_client);

  return 0;
}
