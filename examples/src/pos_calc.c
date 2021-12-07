/*
 *  *  * Example for demonstrating the communication between a
 *   *   * pos_calc module and a client
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
  {"amp_gain_ch",         required_argument,   NULL, 'c'},
  {"amp_gain_value",      required_argument,   NULL, 'a'},
  {"amp_gain_rw",         required_argument,   NULL, 'r'},
  {NULL, 0, NULL, 0}
};

static const char* shortopt = "hb:vo:s:c:a:r:";

void print_help (char *program_name)
{
  fprintf (stdout, "\n"
                  "  HALCSD Amplitude Gain control utility\n"
                  "  Usage: %s [options]\n"
                  "\n"
                  "  -h  --help                                     Display this usage information\n"
                  "  -b  --brokerendp  <Broker endpoint>            Broker endpoint\n"
                  "  -v  --verbose                                  Verbose output\n"
                  "  -o  --boardslot   <Board slot number = [1-12]> Board slot number\n"
                  "  -s  --halcsnumber <HALCS number      = [0|1] > HALCS number\n"
                  "  -c  --amp_gain_ch <Select channel    = [0|3] > Channel for setting amplitude gain\n"
                  "  -a  --amp_gain_value                           Value for setting amplitude gain\n"
                  "  -r  --amp_gain_rw <  [Read = 0 | Write = 1]  > Read or write option\n"
                  "\n",
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
  char *amp_gain_ch_str = NULL;
  char *amp_gain_value_str = NULL;
  char *amp_gain_rw_str = NULL;
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
        amp_gain_ch_str = strdup (optarg);
        break;

      case 'a':
        amp_gain_value_str = strdup (optarg);
        break;

      case 'r':
        amp_gain_rw_str = strdup (optarg);
        break;

      case '?':
        fprintf (stderr, "[client:DSP]: Option not recognized or missing argument\n");
        print_help (argv [0]);
        exit (1);
        break;

      default:
        fprintf (stderr, "[client:DSP]: Could not parse options\n");
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
    fprintf (stderr, "[client:DSP]: Setting default broker endpoint: %s\n",
            "ipc://"DFLT_BIND_FOLDER);
    broker_endp = strdup ("ipc://"DFLT_BIND_FOLDER);
  }

  /* Set default board number */
  uint32_t board_number;
  if (board_number_str == NULL) {
    fprintf (stderr, "[client:DSP]: Setting default value to BOARD number: %u\n",
            DFLT_BOARD_NUMBER);
    board_number = DFLT_BOARD_NUMBER;
  }
  else {
    board_number = strtoul (board_number_str, NULL, 10);
  }

  /* Set default halcs number */
  uint32_t halcs_number;
  if (halcs_number_str == NULL) {
    fprintf (stderr, "[client:DSP]: Setting default value to HALCS number: %u\n",
            DFLT_HALCS_NUMBER);
    halcs_number = DFLT_HALCS_NUMBER;
  }
  else {
    halcs_number = strtoul (halcs_number_str, NULL, 10);
  }

  char service[50];
  snprintf (service, sizeof (service), "HALCS%u:DEVIO:DSP%u", board_number, halcs_number);

  /* -------------------------------------------------------------------------------------------------------- */

  /* Set amplitude gain channel */
  uint32_t amp_gain_ch = 0;
  if (amp_gain_ch_str == NULL) {
    fprintf (stderr, "[client:DSP]: amp_gain_ch: NULL\n");
  }
  else {
    amp_gain_ch = strtoul (amp_gain_ch_str, NULL, 10);
  }

  /* Set amplitude gain value */
  uint32_t amp_gain_value = 0;
  if (amp_gain_value_str == NULL) {
    fprintf (stderr, "[client:DSP]: amp_gain_value: NULL\n");
  }
  else {
    amp_gain_value = strtoul (amp_gain_value_str, NULL, 10);
  }

  /* Set amplitude gain read or write option */
  uint32_t amp_gain_rw = 0;
  if (amp_gain_rw_str == NULL) {
    fprintf (stderr, "[client:DSP]: amp_gain_rw: NULL\n");
  }
  else {
    amp_gain_rw = strtoul (amp_gain_rw_str, NULL, 10);
    if (amp_gain_rw == 0) {
      fprintf (stdout, "[client:DSP]: amp_gain_rw: READ\n");
    }
    else if (amp_gain_rw == 1) {
      fprintf (stdout, "[client:DSP]: amp_gain_rw: WRITE\n");
    }
  }

  if (amp_gain_rw == 0) { /*------------------------------------------------------------------------------READ */

    if (amp_gain_ch == 0) {
      /* get amp_gain_ch0 */
      uint32_t amp_gain_ch0_get;
      err = halcs_get_amp_gain_ch0_data (halcs_client, service, &amp_gain_ch0_get);
      if (err != HALCS_CLIENT_SUCCESS){
        fprintf (stderr, "[client:DSP]: halcs_get_amp_gain_ch0_data failed\n");
        ret = 2;
        goto err_halcs_exit;
      }
      fprintf (stdout, "[client:DSP]: halcs_get_amp_gain_ch0_data: %u\n", amp_gain_ch0_get);
    }

    else if (amp_gain_ch == 1) {
      /* get amp_gain_ch1 */
      uint32_t amp_gain_ch1_get;
      err = halcs_get_amp_gain_ch1_data (halcs_client, service, &amp_gain_ch1_get);
      if (err != HALCS_CLIENT_SUCCESS){
        fprintf (stderr, "[client:DSP]: halcs_get_amp_gain_ch1_data failed\n");
        ret = 2;
        goto err_halcs_exit;
      }
      fprintf (stdout, "[client:DSP]: halcs_get_amp_gain_ch1_data: %u\n", amp_gain_ch1_get);
    }

    else if (amp_gain_ch == 2) {
      /* get amp_gain_ch2 */
      uint32_t amp_gain_ch2_get;
      err = halcs_get_amp_gain_ch2_data (halcs_client, service, &amp_gain_ch2_get);
      if (err != HALCS_CLIENT_SUCCESS){
        fprintf (stderr, "[client:DSP]: halcs_get_amp_gain_ch2_data failed\n");
        ret = 2;
        goto err_halcs_exit;
      }
      fprintf (stdout, "[client:DSP]: halcs_get_amp_gain_ch2_data: %u\n", amp_gain_ch2_get);
    }

    else if (amp_gain_ch == 3) {
      /* get amp_gain_ch3 */
      uint32_t amp_gain_ch3_get;
      err = halcs_get_amp_gain_ch3_data (halcs_client, service, &amp_gain_ch3_get);
      if (err != HALCS_CLIENT_SUCCESS){
        fprintf (stderr, "[client:DSP]: halcs_get_amp_gain_ch3_data failed\n");
        ret = 2;
        goto err_halcs_exit;
      }
      fprintf (stdout, "[client:DSP]: halcs_get_amp_gain_ch3_data: %u\n", amp_gain_ch3_get);
    }
  }

  if (amp_gain_rw == 1) { /*------------------------------------------------------------------------------WRITE */

    if (amp_gain_value_str == NULL) {
      fprintf (stderr, "[client:DSP]: Invalid value for amp_gain_value\n");
      exit(1);
    }

    if (amp_gain_ch == 0) {
      /* set amp_gain_ch0 */
      err = halcs_set_amp_gain_ch0_data(halcs_client, service, amp_gain_value);
      if (err != HALCS_CLIENT_SUCCESS){
        fprintf (stderr, "[client:DSP]: halcs_set_amp_gain_ch0_data failed\n");
        ret = 2;
        goto err_halcs_exit;
      }
    }

    else if (amp_gain_ch == 1) {
      /* set amp_gain_ch1 */
      err = halcs_set_amp_gain_ch1_data(halcs_client, service, amp_gain_value);
      if (err != HALCS_CLIENT_SUCCESS){
        fprintf (stderr, "[client:DSP]: halcs_set_amp_gain_ch1_data failed\n");
        ret = 2;
        goto err_halcs_exit;
      }
    }

    else if (amp_gain_ch == 2) {
      /* set amp_gain_ch2 */
      err = halcs_set_amp_gain_ch2_data(halcs_client, service, amp_gain_value);
      if (err != HALCS_CLIENT_SUCCESS){
        fprintf (stderr, "[client:DSP]: halcs_set_amp_gain_ch2_data failed\n");
        ret = 2;
        goto err_halcs_exit;
      }
    }

    else if (amp_gain_ch == 3) {
      /* set amp_gain_ch3 */
      err = halcs_set_amp_gain_ch3_data(halcs_client, service, amp_gain_value);
      if (err != HALCS_CLIENT_SUCCESS){
        fprintf (stderr, "[client:DSP]: halcs_set_amp_gain_ch3_data failed\n");
        ret = 2;
        goto err_halcs_exit;
      }
    }

    else {
      fprintf (stderr, "[client:DSP]: Invalid value for amp_gain_rw\n");
      ret = 2;
      goto err_halcs_exit;
    }
  }

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

