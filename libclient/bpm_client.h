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

/******************** ACQ SMIO Structures ******************/
/* Acquistion request */
struct _acq_req_t {
    uint32_t num_samples;                       /* Number of samples */
    uint32_t chan;                              /* Acquisition channel number */
};

typedef struct _acq_req_t acq_req_t;

/* Acquistion data block */
struct _acq_block_t {
    uint32_t idx;                               /* Block index */

    uint32_t *data;                             /* Block or complete curve read */
    uint32_t data_size;                         /* data_out buffer size */
    uint32_t bytes_read;                        /* Number of bytes effectively read */
};

typedef struct _acq_block_t acq_block_t;

/* Acquistion transaction */
struct _acq_trans_t {
    acq_req_t req;                              /* Request */
    acq_block_t block;                          /* Block or whole curve read */
};

typedef struct _acq_trans_t acq_trans_t;

/********************************************************/
/************************ Our API ***********************/
/********************************************************/

/* Create an instance of the BPM client. This must be called
 * before any operation involving communicating with the BPM
 * server. Return an instance of the bpm client */
bpm_client_t *bpm_client_new (char *broker_endp, int verbose);

/* Destroy an instance of the BPM client. This must be called
 * after all operations involving the communication with the BPM
 * server */
void bpm_client_destroy (bpm_client_t **self_p);

/******************** FMC130M SMIO Functions ******************/

/* Blink the FMC Leds. This is only used for debug and for demostration
 * purposes.
 * Returns BPM_CLIENT_SUCCESS if ok and BPM_CLIIENT_ERR_SERVER if
 * if server could not complete the request */
bpm_client_err_e bpm_blink_leds (bpm_client_t *self, char *service, uint32_t leds);

/********************** ACQ SMIO Functions ********************/

/* Start acquisition on a specific channel with an spoecif number of samples,
 * through the use of acq_req_t structure.
 * Returns BPM_CLIENT_SUCCESS if ok and BPM_CLIIENT_ERR_SERVER if the server
 * could not complete the request */
bpm_client_err_e bpm_data_acquire (bpm_client_t *self, char *service,
        acq_req_t *acq_req);

/* Check if apreviouly started acquisition finished.
 * Returns BPM_CLIENT_SUCCESS if ok and BPM_CLIIENT_ERR_AGAIN if the acquistion
 * did not complete */
bpm_client_err_e bpm_check_data_acquire (bpm_client_t *self, char *service);

/* Wait for the previouly started acquistion to complete with a maximum tolerated
 * wait.
 * Returns BPM_CLIENT_SUCCESS if the acquistion finished under the specified
 * timeout or BPM_CLIIENT_ERR_TIMEOUT if the acquistion did not completed in time */
bpm_client_err_e bpm_wait_data_acquire_timed (bpm_client_t *self, char *service,
        int timeout);

/* Get an specific data block from a previously completed acquisiton by setting
 * the desired block index in acq_trans->block.idx and the desired channel in
 * acq_trans->req.channel.
 * Returns BPM_CLIENT_SUCCESS if the block was read or BPM_CLIENT_ERR_SERVER
 * otherwise. The data read is returned in acq_trans->block.data along with
 * the number of bytes effectivly read in acq_trans->block.bytes_read */
bpm_client_err_e bpm_get_data_block (bpm_client_t *self, char *service,
        acq_trans_t *acq_trans);

/* Get a complete curve from a previously completed acquisiton by setting
 * the the desired channel in acq_trans->req.channel.
 * Returns BPM_CLIENT_SUCCESS if the curve was read or BPM_CLIENT_ERR_SERVER
 * otherwise. The data read is returned in acq_trans->block.data along with
 * the number of bytes effectivly read in acq_trans->block.bytes_read */
bpm_client_err_e bpm_get_curve (bpm_client_t *self, char *service,
        acq_trans_t *acq_trans);

#endif
