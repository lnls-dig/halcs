/*
 * Copyright (C) 2021 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "halcs_server.h"
/* Private headers */
#include "sm_io_fofb_ctrl_defaults.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, SM_IO, "[sm_io:fofb_ctrl_defaults]",    \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)       \
    ASSERT_HAL_ALLOC(ptr, SM_IO, "[sm_io:fofb_ctrl_defaults]",            \
            smio_err_str(SMIO_ERR_ALLOC),                           \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                    \
    CHECK_HAL_ERR(err, SM_IO, "[sm_io:fofb_ctrl_defaults]",               \
            smio_err_str (err_type))

#define SMIO_FOFB_CTRL_LIBHALCSCLIENT_LOG_MODE                "a"

/* We use the actual libclient to send and configure our default values,
 * maintaining internal consistency. So, in fact, we are sending ourselves
 * a message containing the default values. Because of this approach, we
 * only get to default our values when the functions are already exported
 * to the broker, which happens on a late stage. This could cause a fast
 * client to get an inconsistent state from our server */
/* TODO: Avoid exporting the functions before we have initialized
 * our server with the default values */
smio_err_e fofb_ctrl_config_defaults (char *broker_endp, char *service,
       const char *log_file_name)
{
    UNUSED(log_file_name);
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO, "[sm_io:fofb_ctrl_defaults] Configuring SMIO "
            "FOFB_CTRL with default values ...\n");
    halcs_client_err_e client_err = HALCS_CLIENT_SUCCESS;
    smio_err_e err = SMIO_SUCCESS;

    halcs_client_t *config_client = halcs_client_new_log_mode (broker_endp, 0,
            log_file_name, SMIO_FOFB_CTRL_LIBHALCSCLIENT_LOG_MODE);
    ASSERT_ALLOC(config_client, err_alloc_client);

    client_err = halcs_set_fofb_ctrl_time_frame_len (config_client, service, FOFB_CTRL_DFLT_TIME_FRAME_LEN);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set TIME FRAME LEN",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = halcs_set_fofb_ctrl_time_frame_dly (config_client, service, FOFB_CTRL_DFLT_TIME_FRAME_DLY);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set TIME FRAME DLY",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = halcs_set_fofb_ctrl_bpm_id (config_client, service, FOFB_CTRL_DFLT_BPM_ID);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set BPM ID",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = halcs_set_fofb_ctrl_cc_enable (config_client, service, FOFB_CTRL_DFLT_CC_ENABLE);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set CC ENABLE",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = halcs_set_fofb_ctrl_act_part (config_client, service, 0x1);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set ACT PART value",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

err_param_set:
    halcs_client_destroy (&config_client);
err_alloc_client:
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO, "[sm_io:fofb_ctrl_defaults] Exiting Config thread %s\n",
        service);
    return err;
}
