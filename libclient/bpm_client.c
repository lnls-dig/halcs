/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include "bpm_client.h"
#include "hal_assert.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label)  \
    ASSERT_HAL_TEST(test_boolean, BPM_CLIENT, "[libclient]",\
            err_str, err_goto_label)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label)                   \
    ASSERT_HAL_ALLOC(ptr, BPM_CLIENT, "[libclient]",        \
            bpm_client_err_str(BPM_CLIENT_ERR_ALLOC),       \
            err_goto_label)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                            \
    CHECK_HAL_ERR(err, BPM_CLIENT, "[libclient]",           \
            bpm_client_err_str (err_type))

/* Our structure */
struct _bpm_client_t {
    mdp_client_t *mdp_client;
};

/* Our API */
bpm_client_t *bpm_client_new (char *broker_endp, int verbose)
{
    bpm_client_t *self = zmalloc (sizeof *self);
    ASSERT_ALLOC(self, err_self_alloc);
    self->mdp_client = mdp_client_new (broker_endp, verbose);
    ASSERT_TEST(self->mdp_client!=NULL, "Could not create MDP client",
            err_mdp_client);

err_mdp_client:
        free (self);
err_self_alloc:
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

bpm_client_err_e bpm_blink_leds (bpm_client_t *self, char *service, uint32_t leds)
{
    bpm_client_err_e err = BPM_CLIENT_SUCCESS;
    uint32_t operation = BPM_CLIENT_OPCODE_LEDS;

    zmsg_t *request = zmsg_new ();
    err = (request == NULL) ? BPM_CLIENT_ERR_ALLOC : err;
    ASSERT_ALLOC(request, err_send_msg_alloc);
    zmsg_addmem (request, &operation, sizeof (operation));
    zmsg_addmem (request, &leds, sizeof (leds));
    mdp_client_send (self->mdp_client, service, &request);

err_send_msg_alloc:
    return err;
}

