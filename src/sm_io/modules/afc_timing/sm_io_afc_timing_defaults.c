/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Joao Brito <joao.brito@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "halcs_server.h"
/* Private headers */
#include "sm_io_afc_timing_defaults.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, SM_IO, "[sm_io:afc_timing_defaults]",        \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)                  \
    ASSERT_HAL_ALLOC(ptr, SM_IO, "[sm_io:afc_timing_defaults]",                \
            smio_err_str(SMIO_ERR_ALLOC),                                      \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                               \
    CHECK_HAL_ERR(err, SM_IO, "[sm_io:afc_timing_defaults]",                   \
            smio_err_str (err_type))

#define SMIO_AFC_TIMING_LIBHALCSCLIENT_LOG_MODE                "a"

/* We use the actual libclient to send and configure our default values,
 * maintaining internal consistency. So, in fact, we are sending ourselves
 * a message containing the default values. Because of this approach, we
 * only get to default our values when the functions are already exported
 * to the broker, which happens on a late stage. This could cause a fast
 * client to get an inconsistent state from our server */
/* TODO: Avoid exporting the functions before we have initialized
 * our server with the default values */
smio_err_e afc_timing_config_defaults (char *broker_endp, char *service,
       const char *log_file_name)
{
    UNUSED(log_file_name);
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO, "[sm_io:afc_timing_defaults] Configuring SMIO "
            "AFC_TIMING with default values ...\n");
    halcs_client_err_e client_err = HALCS_CLIENT_SUCCESS;
    smio_err_e err = SMIO_SUCCESS;

    halcs_client_t *config_client = halcs_client_new_log_mode (broker_endp, 0,
            log_file_name, SMIO_AFC_TIMING_LIBHALCSCLIENT_LOG_MODE);
    ASSERT_ALLOC(config_client, err_alloc_client);

    client_err = afc_timing_set_evt_code_0 (config_client, service, AFC_TIMING_DFLT_EVT_IN0);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set channel 0 event code",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = afc_timing_set_evt_code_1 (config_client, service, AFC_TIMING_DFLT_EVT_IN1);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set channel 1 event code",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = afc_timing_set_evt_code_2 (config_client, service, AFC_TIMING_DFLT_EVT_IN2);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set channel 2 event code",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = afc_timing_set_evt_code_3 (config_client, service, AFC_TIMING_DFLT_EVT_IN3);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set channel 3 event code",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = afc_timing_set_evt_code_4 (config_client, service, AFC_TIMING_DFLT_EVT_IN4);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set channel 4 event code",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = afc_timing_set_evt_code_5 (config_client, service, AFC_TIMING_DFLT_EVT_IN5);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set channel 5 event code",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = afc_timing_set_evt_code_6 (config_client, service, AFC_TIMING_DFLT_EVT_IN6);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set channel 6 event code",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = afc_timing_set_evt_delay_0 (config_client, service, AFC_TIMING_DFLT_EVT_DLY0);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set channel 0 event delay",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = afc_timing_set_evt_delay_1 (config_client, service, AFC_TIMING_DFLT_EVT_DLY1);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set channel 1 event delay",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = afc_timing_set_evt_delay_2 (config_client, service, AFC_TIMING_DFLT_EVT_DLY2);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set channel 2 event delay",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = afc_timing_set_evt_delay_3 (config_client, service, AFC_TIMING_DFLT_EVT_DLY3);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set channel 3 event delay",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = afc_timing_set_evt_delay_4 (config_client, service, AFC_TIMING_DFLT_EVT_DLY4);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set channel 4 event delay",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = afc_timing_set_evt_delay_5 (config_client, service, AFC_TIMING_DFLT_EVT_DLY5);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set channel 5 event delay",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = afc_timing_set_evt_delay_6 (config_client, service, AFC_TIMING_DFLT_EVT_DLY6);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set channel 6 event delay",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = afc_timing_set_evt_width_0 (config_client, service, AFC_TIMING_DFLT_EVT_WDT0);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set channel 0 event width",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = afc_timing_set_evt_width_1 (config_client, service, AFC_TIMING_DFLT_EVT_WDT1);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set channel 1 event width",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = afc_timing_set_evt_width_2 (config_client, service, AFC_TIMING_DFLT_EVT_WDT2);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set channel 2 event width",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = afc_timing_set_evt_width_3 (config_client, service, AFC_TIMING_DFLT_EVT_WDT3);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set channel 3 event width",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = afc_timing_set_evt_width_4 (config_client, service, AFC_TIMING_DFLT_EVT_WDT4);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set channel 4 event width",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = afc_timing_set_evt_width_5 (config_client, service, AFC_TIMING_DFLT_EVT_WDT5);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set channel 5 event width",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = afc_timing_set_evt_width_6 (config_client, service, AFC_TIMING_DFLT_EVT_WDT6);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set channel 6 event width",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = afc_timing_set_freq_kp (config_client, service, AFC_TIMING_DFLT_FREQ_KP);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set proportional gain of frequency feedback",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = afc_timing_set_freq_ki (config_client, service, AFC_TIMING_DFLT_FREQ_KI);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set integral gain of frequency feedback",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = afc_timing_set_phase_kp (config_client, service, AFC_TIMING_DFLT_PHASE_KP);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set proportional gain of phase feedback",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = afc_timing_set_phase_ki (config_client, service, AFC_TIMING_DFLT_PHASE_KI);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set integral gain of phase feedback",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = afc_timing_set_phase_set (config_client, service, AFC_TIMING_DFLT_PHASE_SET);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set set of phase feedback",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = afc_timing_set_avg_exponent (config_client, service, AFC_TIMING_DFLT_DDMTD_AVG_EXP);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set average exponent number of phase feedback",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = afc_timing_set_avg_exponent (config_client, service, AFC_TIMING_DFLT_RTM_RFREQ_HI);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set RTM Si570 rfreq[37-19]",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = afc_timing_set_avg_exponent (config_client, service, AFC_TIMING_DFLT_RTM_RFREQ_LO);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set RTM Si570 rfreq[18-0]",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = afc_timing_set_avg_exponent (config_client, service, AFC_TIMING_DFLT_RTM_N1);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set RTM Si570 n1",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = afc_timing_set_avg_exponent (config_client, service, AFC_TIMING_DFLT_RTM_HS_DIV);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set RTM Si570 hs_div",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = afc_timing_set_avg_exponent (config_client, service, AFC_TIMING_DFLT_AFC_RFREQ_HI);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set AFC Si570 rfreq[37-19]",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = afc_timing_set_avg_exponent (config_client, service, AFC_TIMING_DFLT_AFC_RFREQ_LO);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set AFC Si570 rfreq[18-0]",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = afc_timing_set_avg_exponent (config_client, service, AFC_TIMING_DFLT_AFC_N1);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set AFC Si570 n1",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = afc_timing_set_avg_exponent (config_client, service, AFC_TIMING_DFLT_AFC_HS_DIV);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set AFC Si570 hs_div",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

err_param_set:
    halcs_client_destroy (&config_client);
err_alloc_client:
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO, "[sm_io:afc_timing_defaults] Exiting Config thread %s\n",
        service);
    return err;
}
