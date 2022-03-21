/*
** Example for demonstrating the communication between a HALCS client and FOFB
** Processing SMIO.
**/

#include <errno.h>
#include <getopt.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#include <czmq.h>
#include <halcs_client.h>

#include "hw/wb_fofb_processing_regs.h" // TODO: Is this include okay to be here?

#define UINT32_T_MAX        4294967295  // 2^32 - 1
#define DFLT_BIND_FOLDER    "/tmp/malamute"

static struct option long_opts[] = {
  {"help",          no_argument,        NULL, 'h'},
  {"board-slot",    required_argument,  NULL, 'o'},
  {"halcs-number",  required_argument,  NULL, 'n'},
  {"channel",       required_argument,  NULL, 'c'},
  {"get-coeffs",    no_argument,        NULL, 'g'},
  {"set-coeffs",    required_argument,  NULL, 's'},
  {"broker-endp",   required_argument,  NULL, 'b'},
  {"verbose",       no_argument,        NULL, 'v'},
  {NULL, 0, NULL, 0}
};

static const char *short_opts = "ho:n:c:gs:b:v";

struct optional_args {
  char *broker_endp;
  int verbose;
};

enum operation {
  SET_COEFFS,
  GET_COEFFS
};

enum necessary_args_idx {
  BOARD_SLOT,
  HALCS_NUMBER,
  CHANNEL,
  OP,
  NUM_OF_NECESSARY_ARGS
};

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
    "       %s [OPTIONALS] --board-slot <1-12> --halcs-number <0|1> --channel <0-11> --get-coeffs\n"
    "       %s [OPTIONALS] --board-slot <1-12> --halcs-number <0|1> --channel <0-11> --set-coeffs <filename>\n"
    "\n"
    "  -h  --help                                    display this usage info\n"
    "  -o  --board-slot <board slot number = [1-12]> board slot number\n"
    "  -n  --halcs-number <HALCS number = [0|1]>     HALCS number\n"
    "  -c  --channel <channel number = [0-11]>       channel number\n"
    "  -g  --get-coeffs                              gets coeffs\n"
    "  -s  --set-coeffs <filename>                   sets coeffs\n"
    "\n"
    "OPTIONALS:\n"
    "  -b  --broker-endp <broker endpoint>           broker endpoint (defaults to %s)\n"
    "  -v  --verbose                                 verbose output\n",
    program_name, program_name, program_name, "ipc://"DFLT_BIND_FOLDER);
}

static int _strtou(const char *nptr, uint32_t *const u) {
  int ret;
  char *endptr;

  errno = 0;
  const unsigned long int ul = strtoul(nptr, &endptr, 10);

  if(endptr == nptr) {
    fprintf(stderr, "[client:fofb_processing]: '%s' is not a decimal number\n",
      nptr);
    ret = -1;
  } else if(ul == 0 && errno != 0) {
    fprintf(stderr, "[client:fofb_processing]: '%s': %s\n", nptr, strerror(errno));
    ret = -1;
  } else if(ul > UINT32_T_MAX) {
    fprintf(stderr, "[client:fofb_processing]: %ld is greater than "
      "UINT32_T_MAX\n", ul);
    ret = -1;
  } else {
    *u = (uint32_t)ul;
    ret = 0;
  }

  return ret;
}

int main(int argc, char *argv[]) {
  int ret = 0;

  struct optional_args opt_args = {
    .broker_endp = "ipc://" DFLT_BIND_FOLDER,
    .verbose = 0
  };
  struct necessary_args nec_args = {0};

  smio_fofb_processing_data_block_t coeffs = {0};

  int opt;
  // Parsing arguments
  // ########################################################################### 
  while((opt = getopt_long(argc, argv, short_opts, long_opts, NULL)) != -1) {
    switch(opt) {
      case 'h':
        print_usage(argv[0]);

        ret = 0;
        goto err_halcs_client_not_inst;
        break;

      case 'b':
        opt_args.broker_endp = optarg;
        break;

      case 'o':
        if(_strtou(optarg, &nec_args.board_slot) != 0) {
          ret = -1;
          goto err_halcs_client_not_inst;
        }
        nec_args.is_given[BOARD_SLOT] = true;
        break;

      case 'n':
        if(_strtou(optarg, &nec_args.halcs_number) != 0) {
          ret = -1;
          goto err_halcs_client_not_inst;
        }
        nec_args.is_given[HALCS_NUMBER] = true;
        break;

      case 'c':
        if(_strtou(optarg, &nec_args.channel) != 0) {
          ret = -1;
          goto err_halcs_client_not_inst;
        }
        nec_args.is_given[CHANNEL] = true;
        break;

      case 'g':
        if(!nec_args.is_given[OP]) {
            nec_args.op = GET_COEFFS;
            nec_args.is_given[OP] = true;
        } else { 
          fprintf(stderr, "[client:fofb_processing]: Multiple operations not"
            " allowed!\n");
          print_usage(argv[0]);

          ret = -1;
          goto err_halcs_client_not_inst;
        }
        break;

      case 's':
        errno = 0;
        if(!nec_args.is_given[OP]) {
          FILE *fp;

          fp = fopen(optarg, "r");
          if(fp == NULL) {
            fprintf(stderr, "[client:fofb_processing]:  %s: %s\n", optarg,
              strerror(errno));

            ret = -1;
            goto err_halcs_client_not_inst;
          } else {
            char coeff[20] = {0};

            for(int i = 0;
              i < FOFB_PROCESSING_REGS_RAM_BANK_SIZE/sizeof(uint32_t); i++) {
                if(fgets(coeff, sizeof coeff, fp) != NULL) {
                  if(_strtou(coeff, &coeffs.data[i]) != 0) {
                    ret = -1;
                    goto err_halcs_client_not_inst;
                  }
                } else {
                  fclose(fp);
                  fprintf(stderr, "[client:fofb_processing]: %s: %s\n", optarg,
                    strerror(errno));

                  ret = -1;
                  goto err_halcs_client_not_inst;
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

          ret = -1;
          goto err_halcs_client_not_inst;
        }
       break;

      case 'v':
        opt_args.verbose = 1;
        break;

      case '?':
        fprintf(stderr, "[client:fofb_processing]: Option not recognized or"
          " missing argument\n");
        print_usage(argv[0]);

        ret = -1;
        goto err_halcs_client_not_inst;
        break;

      default:
        fprintf(stderr, "[client:fofb_processing]: Could not parse options\n");
        print_usage(argv[0]);

        ret = -1;
        goto err_halcs_client_not_inst;
      }
  }

  // Checking if necessary arguments were given
  // ########################################################################### 
  for(int i = 0; i < NUM_OF_NECESSARY_ARGS; i++) {
    if(!nec_args.is_given[i]) {
      fprintf(stderr, "[client:fofb_processing]: Missing one or more necessary"
        " arguments!\n");
      print_usage(argv[0]);

      ret = -1;
      goto err_halcs_client_not_inst;
    }
  }

  // Instantiating a HALCS client
  // ########################################################################### 
  halcs_client_t *client =
    halcs_client_new(opt_args.broker_endp, opt_args.verbose, NULL);
  if(client == NULL) {
    fprintf(stderr, "[client:fofb_processing]: HALCS client could not be"
      " created\n");

    ret = -1;
    goto err_halcs_client_not_inst;
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

      ret = -1;
      goto err_halcs_client_inst;
    } else {
      fprintf(stdout, "[client:fofb_processing]: "
        "halcs_fofb_processing_coeff_ram_bank_write succeed\n");
    }
  } else if(nec_args.op == GET_COEFFS) {
    halcs_client_err_e err;

    err = halcs_fofb_processing_coeff_ram_bank_read(client, service,
      nec_args.channel, &coeffs);
    if(err != HALCS_CLIENT_SUCCESS) {
      fprintf(stderr, "[client:fofb_processing]: "
        "halcs_fofb_processing_coeff_ram_bank_read failed\n");

      ret = -1;
      goto err_halcs_client_inst;
    } else {
      fprintf(stdout, "[client:fofb_processing]: "
        "halcs_fofb_processing_coeff_ram_bank_read succeed\n");

      int i;
      fprintf(stdout, "{");
      for(i = 0;
        i < FOFB_PROCESSING_REGS_RAM_BANK_SIZE/sizeof(uint32_t) - 1; i++) {
          fprintf(stdout, "%d, ", coeffs.data[i]);
        }
      fprintf(stdout, "%d}\n", coeffs.data[i]);
    }
  }

err_halcs_client_inst:
  halcs_client_destroy(&client);
err_halcs_client_not_inst:

  return ret;
}
