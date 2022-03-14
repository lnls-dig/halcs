/*
** Example for demonstrating the communication between a HALCS client and FOFB
** Processing SMIO.
**/

#include <getopt.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#include <czmq.h>
#include <halcs_client.h>

#include "hw/wb_fofb_processing_regs.h" // TODO: Is this include okay to be here?

#define DFLT_BIND_FOLDER    "/tmp/malamute"

static struct option long_opts[] = {
  {"help",          no_argument,        NULL, 'h'},
  {"board-slot",    required_argument,  NULL, 'o'},
  {"halcs-number",  required_argument,  NULL, 'n'},
  {"channel",       required_argument,  NULL, 'c'},
  {"set-coeffs",    required_argument,  NULL, 's'},
  {"broker-endp",   required_argument,  NULL, 'b'},
  {"verbose",       no_argument,        NULL, 'v'},
  {NULL, 0, NULL, 0}
};

static const char *short_opts = "ho:n:c:s:b:v";

struct optional_args {
  char *broker_endp;
  int verbose;
};

enum operation {
  SET_COEFFS
};

enum necessary_args_idx {
  BOARD_SLOT,
  HALCS_NUMBER,
  CHANNEL,
  OP
};

#define NUM_OF_NECESSARY_ARGS 4
struct necessary_args {
  bool is_given[NUM_OF_NECESSARY_ARGS];

  uint32_t board_slot;
  uint32_t halcs_number;
  uint32_t channel;
  enum operation op;
};

void print_usage(char *program_name) {
  fprintf(stdout,
    "HALCSD FOFB Processing Client Example\n"
    "Usages:\n"
    "       %s --help\n"
    "       %s [OPTIONALS] --board-slot <1-12> --halcs-number <0|1> --channel <0-11> --set-coeffs <filename>\n"
    "\n"
    "  -h  --help                                    display this usage info\n"
    "  -o  --board-slot <board slot number = [1-12]> board slot number\n"
    "  -n  --halcs-number <HALCS number = [0|1]>     HALCS number\n"
    "  -c  --channel <channel number = [0-11]>       channel number\n"
    "  -s  --set-coeffs <filename>                   sets coeffs\n"
    "\n"
    "OPTIONALS:\n"
    "  -b  --broker-endp <broker endpoint>           broker endpoint (defaults to %s)\n"
    "  -v  --verbose                                 verbose output\n",
    program_name, program_name, "ipc://"DFLT_BIND_FOLDER);
}

int main(int argc, char *argv[]) {
  int ret = 0;

  struct optional_args opt_args = {
    .broker_endp = strdup("ipc://"DFLT_BIND_FOLDER),
    .verbose = 0
  };
  struct necessary_args nec_args = {
    .board_slot = 0,
    .halcs_number = 0,
    .channel = 0,
  };
  for(int i = 0; i < NUM_OF_NECESSARY_ARGS; i++) {
    nec_args.is_given[i] = false;
  }

  smio_fofb_processing_data_block_t coeffs;

  int opt;
  // Parsing arguments
  // ########################################################################### 
  while((opt = getopt_long(argc, argv, short_opts, long_opts, NULL)) != -1) {
    switch(opt) {
      case 'h':
        print_usage(argv[0]);

        ret = 0;
        goto err_free_mem;
        break;

      case 'b':
        opt_args.broker_endp = strdup(optarg);
        break;

      case 'o':
        nec_args.board_slot = strtoul(optarg, NULL, 10);
        nec_args.is_given[BOARD_SLOT] = true;
        break;

      case 'n':
        nec_args.halcs_number = strtoul(optarg, NULL, 10);
        nec_args.is_given[HALCS_NUMBER] = true;
        break;

      case 'c':
        nec_args.channel = strtoul(optarg, NULL, 10);
        nec_args.is_given[CHANNEL] = true;
        break;

      case 's':
        if(!nec_args.is_given[OP]) {
          FILE *fp;

          fp = fopen(optarg, "r");
          if(fp == NULL) {
            fprintf(stderr, "[client:fofb_processing]: Something wrong happened"
              " while trying to open %s\n", optarg);

            ret = 1;
            goto err_free_mem;
          } else {
            char coeff[20] = {0};

            for(int i = 0;
              i < FOFB_PROCESSING_REGS_RAM_BANK_SIZE/sizeof(uint32_t); i++) {
                if(fgets(coeff, 20, fp) != NULL) {
                  coeffs.data[i] = strtoul(coeff, NULL, 10);
                } else {
                  fclose(fp);
                  fprintf(stderr, "[client:fofb_processing]: Something wrong"
                    " happened while parsing %s\n", optarg);

                  ret = 1;
                  goto err_free_mem;
                }
            }
            fclose(fp);

            nec_args.op = SET_COEFFS;
            nec_args.is_given[OP] = true;
          }
        } else { 
          fprintf(stderr, "[client:fofb_processing]: Multiple operations not"
            " allowed!\n");
          print_usage(argv[0]);

          ret = 1;
          goto err_free_mem;
        }
       break;

      case 'v':
        opt_args.verbose = 1;
        break;

      case '?':
        fprintf(stderr, "[client:fofb_processing]: Option not recognized or"
          " missing argument\n");
        print_usage(argv[0]);

        ret = 1;
        goto err_free_mem;
        break;

      default:
        fprintf(stderr, "[client:fofb_processing]: Could not parse options\n");
        print_usage(argv[0]);

        ret = 1;
        goto err_free_mem;
      }
  }

  // Checking if necessary arguments were given
  // ########################################################################### 
  for(int i = 0; i < NUM_OF_NECESSARY_ARGS; i++) {
    if(!nec_args.is_given[i]) {
      fprintf(stderr, "[client:fofb_processing]: Missing one or more necessary"
        " arguments!\n");
      print_usage(argv[0]);

      ret = 1;
      goto err_free_mem;
    }
  }

  // Instantiating a HALCS client
  // ########################################################################### 
  halcs_client_t *client =
    halcs_client_new(opt_args.broker_endp, opt_args.verbose, NULL);
  if(client == NULL) {
    fprintf(stderr, "[client:fofb_processing]: HALCS client could not be"
      " created\n");

    ret = 1;
    goto err_free_mem;
  } else {
    fprintf(stdout, "[client:fofb_processing]: HALCS client successfully"
      " created\n");
  }

  // Setting target service name
  // ########################################################################### 
  char service[50];
  snprintf(service, sizeof(service), "HALCS%u:DEVIO:FOFB_PROCESSING%u",
    nec_args.board_slot, nec_args.halcs_number);
  fprintf(stdout, "[client:fofb_processing]: Target service name: %s\n",
    service);

  // Performing operation
  // ########################################################################### 
  if(nec_args.op == SET_COEFFS) {
    halcs_client_err_e err;

    err = halcs_fofb_processing_coeff_ram_bank_write(client, service,
      nec_args.channel, coeffs);
    if(err != HALCS_CLIENT_SUCCESS) {
      fprintf(stderr, "[client:fofb_processing]: "
        "halcs_fofb_processing_coeff_ram_bank_write failed\n");

      ret = 1;
      goto err_halcs_exit;
    } else {
      fprintf(stdout, "[client:fofb_processing]: "
        "halcs_fofb_processing_coeff_ram_bank_write succeed\n");
    }
  }

err_halcs_exit:
  halcs_client_destroy(&client);

err_free_mem:
  free(opt_args.broker_endp);
  opt_args.broker_endp = NULL;

  return ret;
}
