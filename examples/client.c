/*
 * Simple example demonstrating the communication between
 * a client and the FPGA device
 */

#include <mdp.h>
#include <inttypes.h>

#define LEDS_OPERATION                  0

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

int main (int argc, char *argv [])
{
    int verbose = (argc > 1 && streq (argv [1], "-v"));
    bpm_client_t *bpm_client = bpm_client_new ("ipc:///tmp/bpm/0", verbose);

    unsigned i;
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
    return 0;
}
