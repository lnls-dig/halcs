/*
 * Simple example demonstrating the communication between
 * a client and the FPGA device
 */

#include <mdp.h>
#include <czmq.h>
#include <inttypes.h>

#include <bpm_client.h>

#define DFLT_BIND_FOLDER            "/tmp/bpm"

void print_help (char *program_name)
{
    printf( "Usage: %s [options]\n"
            "\t-h This help message\n"
            "\t-v Verbose output\n"
            "\t-b <broker_endpoint> Broker endpoint\n", program_name);
}

int main (int argc, char *argv [])
{
    int verbose = 0;
    char *broker_endp = NULL;
    char **str_p;

    if (argc < 2) {
        print_help (argv[0]);
        exit (1);
    }

    /* FIXME: This is rather buggy! */
    /* Simple handling of command-line options. This should be done
     * with getopt, for instance*/
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
        /* Fallout for options with parameters */
        else {
            *str_p = strdup (argv[i]);
        }
    }

    /* Set default broker address */
    if (broker_endp == NULL) {
        broker_endp = strdup ("ipc://"DFLT_BIND_FOLDER);
    }

    bpm_client_t *bpm_client = bpm_client_new (broker_endp, verbose, NULL);

    for (i = 0; i < 32768; ++i) {
        uint32_t leds = (1 << 1);
        unsigned int j;
        for (j = 0; j < 3; ++j) {
            if (zctx_interrupted) {
                break;
            }

            bpm_blink_leds (bpm_client, "BPM0:DEVIO:FMC130M_4CH0", leds);
            usleep (80000);
            leds <<= 1;
        }
    }

    bpm_client_destroy (&bpm_client);
    str_p = &broker_endp;
    free (*str_p);
    broker_endp = NULL;
    return 0;
}
