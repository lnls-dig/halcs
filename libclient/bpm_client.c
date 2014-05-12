/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include "bpm_client.h"
#include "bpm_client_codes.h"

/* Our structure */
struct _bpm_client_t {
    mdp_client_t *mdp_client;
};

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

bpm_cli_recode_t bpm_blink_leds (bpm_client_t *self, char *service, uint32_t leds)
{
    uint32_t operation = BPM_CLI_OPCODE_LEDS;

    zmsg_t *request = zmsg_new ();
    zmsg_addmem (request, &operation, sizeof (operation));
    zmsg_addmem (request, &leds, sizeof (leds));
    mdp_client_send (self->mdp_client, service, &request);

    return BPM_CLI_RECODE_OK;
}

