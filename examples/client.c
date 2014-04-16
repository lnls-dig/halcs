/*
 * Simple example demonstrating the communication between
 * a client and the FPGA device
 */

#include <mdp.h>
#include <czmq.h>
#include <inttypes.h>

#define DFLT_BIND_FOLDER            "/tmp/bpm"
#define DFLT_BIND_ADDR              "0"
#define LEDS_OPERATION              0

/* Our tructure */
typedef struct _bpm_client_t {
    mdp_client_t *mdp_client;
} bpm_client_t;

/* Our API */
bpm_client_t *bpm_client_new (char *broker_endp, int verbose)
{
    bpm_client_t *self = zmalloc (sizeof *self);
    self->mdp_client = mdp_client_new (broker_endp, verbose);

    return self;
}

void bpm_client_destroy (bpm_client_t **self_p)
{
    assert (self_p);

    if (*self_p) {
        bpm_client_t *self = *self_p;

        mdp_client_destroy (&self->mdp_client);
        *self_p = NULL;
    }
}

int bpm_blink_leds (bpm_client_t *self, char *service, uint32_t leds)
{
    uint32_t operation = LEDS_OPERATION;

    zmsg_t *request = zmsg_new ();
    zmsg_addmem (request, &operation, sizeof (operation));
    zmsg_addmem (request, &leds, sizeof (leds));
    mdp_client_send (self->mdp_client, service, &request);

    return 0;
}

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
        broker_endp = strdup ("ipc://"DFLT_BIND_FOLDER"/"DFLT_BIND_ADDR);
    }

    bpm_client_t *bpm_client = bpm_client_new (broker_endp, verbose);

    for (i = 0; i < 32768; ++i) {
        uint32_t leds = (1 << 1);
        unsigned int j;
        for (j = 0; j < 3; ++j) {
            if (!zctx_interrupted) {
                bpm_blink_leds (bpm_client, "BPM0:DEVIO:FMC130M_4CH", leds);
                usleep (80000);
                leds <<= 1;
            }
        }
    }

    bpm_client_destroy (&bpm_client);
    str_p = &broker_endp;
    free (*str_p);
    broker_endp = NULL;
    return 0;
}
