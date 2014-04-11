/*
 * Simple example demonstrating the communication between
 * a client and the FPGA device
 */

#include <mdp.h>
#include <inttypes.h>

int main (int argc, char *argv [])
{
    int verbose = (argc > 1 && streq (argv [1], "-v"));
    mdp_client_t *client = mdp_client_new ("ipc:///tmp/bpm/0", verbose);

    unsigned i;
    for (i = 0; i < 32768; ++i) {
        unsigned int j;
        uint32_t operation = 0;
        uint32_t leds = (1 << 1);
        for (j = 0; j < 3; ++j) {
            if (!zctx_interrupted) {
                zmsg_t *request = zmsg_new ();
                zmsg_addmem (request, &operation, sizeof (operation));
                zmsg_addmem (request, &leds, sizeof (leds));
                mdp_client_send (client, "BPM0:DEVIO:FMC130M_4CH", &request);
                usleep (80000);
                leds <<= 1;
            }
        }
    }

    mdp_client_destroy (&client);
    return 0;
}
