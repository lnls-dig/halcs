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

    client_err = afc_timing_set_freq_sample_rate_prescale (config_client, service, AFC_TIMING_DFLT_FREQ_SAMPLE_RATE_PRESCALE);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set freq sample rate prescaler",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = afc_timing_set_afc_freq_kp (config_client, service, AFC_TIMING_DFLT_AFC_FREQ_KP);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set proportional gain of frequency feedback",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = afc_timing_set_afc_freq_kp (config_client, service, AFC_TIMING_DFLT_AFC_FREQ_KP);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set proportional gain of frequency feedback",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = afc_timing_set_afc_freq_ki (config_client, service, AFC_TIMING_DFLT_AFC_FREQ_KI);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set integral gain of frequency feedback",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = afc_timing_set_afc_phase_kp (config_client, service, AFC_TIMING_DFLT_AFC_PHASE_KP);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set proportional gain of phase feedback",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = afc_timing_set_afc_phase_ki (config_client, service, AFC_TIMING_DFLT_AFC_PHASE_KI);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set integral gain of phase feedback",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = afc_timing_set_afc_phase_set (config_client, service, AFC_TIMING_DFLT_AFC_PHASE_SET);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set set of phase feedback",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = afc_timing_set_afc_phase_navg (config_client, service, AFC_TIMING_DFLT_AFC_PHASE_NAVG);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set set of phase feedback",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = afc_timing_set_afc_phase_div_exp (config_client, service, AFC_TIMING_DFLT_AFC_PHASE_DIV);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set set of phase feedback",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = afc_timing_set_rtm_rfreq_hi (config_client, service, AFC_TIMING_DFLT_RTM_RFREQ_HI);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set RTM Si570 rfreq[37-20]",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = afc_timing_set_rtm_rfreq_lo (config_client, service, AFC_TIMING_DFLT_RTM_RFREQ_LO);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set RTM Si570 rfreq[19-0]",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = afc_timing_set_rtm_n1 (config_client, service, AFC_TIMING_DFLT_RTM_N1);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set RTM Si570 n1",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = afc_timing_set_rtm_hs_div (config_client, service, AFC_TIMING_DFLT_RTM_HS_DIV);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set RTM Si570 hs_div",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = afc_timing_set_afc_rfreq_hi (config_client, service, AFC_TIMING_DFLT_AFC_RFREQ_HI);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set AFC Si570 rfreq[37-20]",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = afc_timing_set_afc_rfreq_lo (config_client, service, AFC_TIMING_DFLT_AFC_RFREQ_LO);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set AFC Si570 rfreq[19-0]",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = afc_timing_set_afc_n1 (config_client, service, AFC_TIMING_DFLT_AFC_N1);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set AFC Si570 n1",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = afc_timing_set_afc_hs_div (config_client, service, AFC_TIMING_DFLT_AFC_HS_DIV);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set AFC Si570 hs_div",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

err_param_set:
    halcs_client_destroy (&config_client);
err_alloc_client:
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO, "[sm_io:afc_timing_defaults] Exiting Config thread %s\n",
        service);
    return err;
}
