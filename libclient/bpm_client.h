/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _BPM_CLIENT_H_
#define _BPM_CLIENT_H_

#include <inttypes.h>
#include <mdp.h>

#include "bpm_client_codes.h"
#include "bpm_client_err.h"

typedef struct _bpm_client_t bpm_client_t;

/* Our API */

/* Create an instance of the BPM client. This must be called
 * before any operation involving communicating with the BPM
 * server. Return an instance of the bpm client */
bpm_client_t *bpm_client_new (char *broker_endp, int verbose);

/* Destroy an instance of the BPM client. This must be called
 * after all operations involving the communication with the BPM
 * server */
void bpm_client_destroy (bpm_client_t **self_p);

/* Blink the FMC Leds. This is only used for debug and for demostration
 * purposes. Return BPM_CLI_RECODE_OK if ok and BPM_CLI_RECODE_ERR if
 * error */
bpm_client_err_e bpm_blink_leds (bpm_client_t *self, char *service, uint32_t leds);

#endif
