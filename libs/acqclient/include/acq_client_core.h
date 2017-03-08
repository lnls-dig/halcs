/*
 * Copyright (C) 2017 LNLS (www.lnls.br)
 * Authors: Lucas Russo <lucas.russo@lnls.br>
 *          Janito Vaqueiro Ferreira Filho <janito.filho@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _ACQ_CLIENT_CORE_H_
#define _ACQ_CLIENT_CORE_H_

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************/
/************************ Our API ***********************/
/********************************************************/

/********************** ACQ SMIO Functions ********************/

/* Acquisition channel definitions */
typedef struct {
    uint32_t chan;
    uint32_t sample_size;
} acq_chan_t;

/* Acquisition request */
typedef struct {
    uint32_t num_samples_pre;                   /* Number of pre-trigger samples */
    uint32_t num_samples_post;                  /* Number of post-trigger samples */
    uint32_t num_shots;                         /* Number of shots */
    uint32_t chan;                              /* Acquisition channel number */
} acq_req_t;

/* Acquisition data block */
typedef struct {
    uint32_t idx;                               /* Block index */

    uint32_t *data;                             /* Block or complete curve read */
    uint32_t data_size;                         /* data_out buffer size */
    uint32_t bytes_read;                        /* Number of bytes effectively read */
} acq_block_t;

/* Acquisition transaction */
typedef struct {
    acq_req_t req;                              /* Request */
    acq_block_t block;                          /* Block or whole curve read */
} acq_trans_t;

/* Acquisition trigger types. Use this to safely configure the trigger
 * and the acq_client_trig_str () function to get trigger strings */
enum _acq_client_trig_e
{
    ACQ_CLIENT_TRIG_SKIP = 0,                   /* No trigger configured */
    ACQ_CLIENT_TRIG_EXTERNAL,                   /* External trigger */
    ACQ_CLIENT_TRIG_DATA_DRIVEN,                /* Data-driven (data offset) trigger */
    ACQ_CLIENT_TRIG_SOFTWARE,                   /* Software generated trigger */
    ACQ_CLIENT_TRIG_END                         /* End of enum marker */
};

typedef enum _acq_client_trig_e acq_client_trig_e;

/* Convert enumeration type to long string */
const char * acq_client_trig_str (acq_client_trig_e err);

/* Convert enumeration type to short string */
const char * acq_client_trig_sstr (acq_client_trig_e err);

/* Create an instance of the acquisition client. This must be called
 * before any acquisition operation involving communicating with the HALCS
 * server. Return an instance of the acquisition client */
acq_client_t *acq_client_new (char *broker_endp, int verbose,
        const char *log_file_name);

/* Create an instance of the acquisition client. This must be called
 * before any acquisition operation involving communicating with the HALCS
 * server, specifying the send/recv timeout in ms.
 * Return an instance of the acquisition client */
acq_client_t *acq_client_new_time (char *broker_endp, int verbose,
        const char *log_file_name, int timeout);

/* Create an instance of the acquisition client, with the log filemode specified
 * by "log_mode" as in fopen () call. This must be called before any acquisition
 * operation involving communicating with the HALCS server. Return an instance
 * of the acquisition client */
acq_client_t *acq_client_new_log_mode (char *broker_endp, int verbose,
        const char *log_file_name, const char *log_mode);

/* Create an instance of the acquisition client, with the log filemode specified
 * by "log_mode" as in fopen () call, and the send/recv timeout in ms.
 * This must be called before any acquisition operation involving communicating
 * with the HALCS server. Return an instance of the acquisition client */
acq_client_t *acq_client_new_log_mode_time (char *broker_endp, int verbose,
        const char *log_file_name, const char *log_mode, int timeout);

/* Destroy an instance of the acquisition client. */
void acq_client_destroy (acq_client_t **self_p);

/* Get current acquisition channel */
const acq_chan_t* acq_get_chan (acq_client_t *self);

/* Set current acquisition channel. Responsibility over the acq_chan_t structure
 * memory remains with the caller. */
void acq_set_chan (acq_client_t *self, const acq_chan_t* channel);

/* Start acquisition on a specific channel with an specific number of samples,
 * through the use of acq_req_t structure.
 * Returns HALCS_CLIENT_SUCCESS if ok and HALCS_CLIIENT_ERR_SERVER if the server
 * could not complete the request */
halcs_client_err_e acq_start (acq_client_t *self, char *service,
        acq_req_t *acq_req);

/* Check if apreviouly started acquisition finished.
 * Returns HALCS_CLIENT_SUCCESS if ok and HALCS_CLIIENT_ERR_AGAIN if the acquistion
 * did not complete */
halcs_client_err_e acq_check (acq_client_t *self, char *service);

/* Wait for the previouly started acquistion to complete with a maximum tolerated
 * wait.
 * Returns HALCS_CLIENT_SUCCESS if the acquistion finished under the specified
 * timeout or HALCS_CLIIENT_ERR_TIMEOUT if the acquistion did not completed in time */
halcs_client_err_e acq_check_timed (acq_client_t *self, char *service,
        int timeout);

/* Get an specific data block from a previously completed acquisiton by setting
 * the desired block index in acq_trans->block.idx and the desired channel in
 * acq_trans->req.channel.
 * Returns HALCS_CLIENT_SUCCESS if the block was read or HALCS_CLIENT_ERR_SERVER
 * otherwise. The data read is returned in acq_trans->block.data along with
 * the number of bytes effectivly read in acq_trans->block.bytes_read */
halcs_client_err_e acq_get_data_block (acq_client_t *self, char *service,
        acq_trans_t *acq_trans);

/* Get a whole curve a previously completed acquisition by setting
 * the desired channel in acq_trans->req.channel.
 * Returns HALCS_CLIENT_SUCCESS if the block was read or HALCS_CLIENT_ERR_SERVER
 * otherwise. The data read is returned in acq_trans->block.data along with
 * the number of bytes effectively read in acq_trans->block.bytes_read */
halcs_client_err_e acq_get_curve (acq_client_t *self, char *service,
        acq_trans_t *acq_trans);

/* Perform a full acquisition process (Acquisition request, checking if
 * its done and receiving the full curve).
 * Returns HALCS_CLIENT_SUCCESS if the curve was read or HALCS_CLIENT_ERR_SERVER
 * otherwise. The data read is returned in acq_trans->block.data along with
 * the number of bytes effectivly read in acq_trans->block.bytes_read */
halcs_client_err_e acq_full (acq_client_t *self, char *service,
        acq_trans_t *acq_trans, int timeout);

/* Compatibility version of the old halcs_full_acq. Performs a full acquisition
 * if new_acq = 1 and a curve readout if new_acq = 0*/
halcs_client_err_e acq_full_compat (acq_client_t *self, char *service,
        acq_trans_t *acq_trans, int timeout, bool new_acq);

/* Configure acquisition trigger. Trigger types are: 0 -> skip trigger,
 * 1 -> external trigger, 2 -> data-driven trigger, 3 -> software trigger.
 * Returns HALCS_CLIENT_SUCCESS if the trigger was correctly set or
 * or an error (see halcs_client_err.h for all possible errors)*/
halcs_client_err_e acq_set_trig (acq_client_t *self, char *service,
        uint32_t trig);
halcs_client_err_e acq_get_trig (acq_client_t *self, char *service,
        uint32_t *trig);

/* Configure data-driven trigger polarity. Options are: 0 -> positive slope (
 * 0 -> 1), 1 -> negative slope (1 -> 0).
 * Returns HALCS_CLIENT_SUCCESS if the trigger was correctly set or
 * or an error (see halcs_client_err.h for all possible errors)*/
halcs_client_err_e acq_set_data_trig_pol (acq_client_t *self, char *service,
        uint32_t data_trig_pol);
halcs_client_err_e acq_get_data_trig_pol (acq_client_t *self, char *service,
        uint32_t *data_trig_pol);

/* Configure data-driven trigger selection. Options are: 0 -> channel data
 * sample 0, 1 -> channel data sample 1, 2 -> channel data sample 2, 3 -> channel
 * data sample 3
 * Returns HALCS_CLIENT_SUCCESS if the trigger was correctly set or
 * or an error (see halcs_client_err.h for all possible errors)*/
halcs_client_err_e acq_set_data_trig_sel (acq_client_t *self, char *service,
        uint32_t data_trig_sel);
halcs_client_err_e acq_get_data_trig_sel (acq_client_t *self, char *service,
        uint32_t *data_trig_sel);

/* Configure data-driven trigger hysteresis filter. data_trig_filt is an integer
 * number from 0 to 2^8-1, meaning the number of steady counts after the data
 * sample goes above or below the data threshold. This is only valid for
 * data-driven trigger.
 * Returns HALCS_CLIENT_SUCCESS if the trigger was correctly set or
 * or an error (see halcs_client_err.h for all possible errors)*/
halcs_client_err_e acq_set_data_trig_filt (acq_client_t *self, char *service,
        uint32_t data_trig_filt);
halcs_client_err_e acq_get_data_trig_filt (acq_client_t *self, char *service,
        uint32_t *data_trig_filt);

/* Configure data-driven signed threshold. data_trig_thres is signed integer
 * number from -2^31 to 2^31-1, meaning the number to be compared on which the
 * data-driven trigger will be detected. This is only valid for
 * data-driven trigger.
 * Returns HALCS_CLIENT_SUCCESS if the trigger was correctly set or
 * or an error (see halcs_client_err.h for all possible errors)*/
halcs_client_err_e acq_set_data_trig_thres (acq_client_t *self, char *service,
        uint32_t data_trig_thres);
halcs_client_err_e acq_get_data_trig_thres (acq_client_t *self, char *service,
        uint32_t *data_trig_thres);

/* Configure trigger delay. hw_trig_dly is an integer number from 0 to 2^32-1,
 * meaning the number of ADC clock cycles after which the detected trigger
 * will be used. This is only valid for external or data-driven trigger types.
 * Returns HALCS_CLIENT_SUCCESS if the trigger was correctly set or
 * or an error (see halcs_client_err.h for all possible errors)*/
halcs_client_err_e acq_set_hw_trig_dly (acq_client_t *self, char *service,
        uint32_t hw_trig_dly);
halcs_client_err_e acq_get_hw_trig_dly (acq_client_t *self, char *service,
        uint32_t *hw_trig_dly);

/* Generate software trigger. Options are: 1 generates a software trigger, 0
 * generates nothing.
 * Returns HALCS_CLIENT_SUCCESS if the trigger was correctly set or
 * or an error (see halcs_client_err.h for all possible errors)*/
halcs_client_err_e acq_set_sw_trig (acq_client_t *self, char *service,
        uint32_t sw_trig);
halcs_client_err_e acq_get_sw_trig (acq_client_t *self, char *service,
        uint32_t *sw_trig);

/* Stops the acquisition FSM. Options are: 1 stop the FSM, 0
 * generates nothing.
 * Returns HALCS_CLIENT_SUCCESS if the trigger was correctly set or
 * or an error (see halcs_client_err.h for all possible errors)*/
halcs_client_err_e acq_set_fsm_stop (acq_client_t *self, char *service,
        uint32_t fsm_stop);
halcs_client_err_e acq_get_fsm_stop (acq_client_t *self, char *service,
        uint32_t *fsm_stop);

/* Selects which channel to look for a data trigger.
 * Returns HALCS_CLIENT_SUCCESS if the trigger was correctly set or
 * or an error (see halcs_client_err.h for all possible errors)*/
halcs_client_err_e acq_set_data_trig_chan (acq_client_t *self, char *service,
        uint32_t data_trig_chan);
halcs_client_err_e acq_get_data_trig_chan (acq_client_t *self, char *service,
        uint32_t *data_trig_chan);


#ifdef __cplusplus
}
#endif

#endif
