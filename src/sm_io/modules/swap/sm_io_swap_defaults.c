#include "errhand.h"
#include "sm_io_err.h"
#include "sm_io_swap_defaults.h"
/* We actually use the libclient to do the "heavy" work for us */
#include "bpm_client.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, SM_IO, "[sm_io:swap_defaults]",    \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)       \
    ASSERT_HAL_ALLOC(ptr, SM_IO, "[sm_io:swap_defaults]",           \
            smio_err_str(SMIO_ERR_ALLOC),                           \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                    \
    CHECK_HAL_ERR(err, SM_IO, "[sm_io:swap_defaults]",              \
            smio_err_str (err_type))

#define SMIO_SWAP_LIBBPMCLIENT_LOG_MODE                "a"

/* We use the actual libclient to send and configure our default values,
 * maintaining internal consistency. So, in fact, we are sending ourselves
 * a message containing the default values. Because of this approach, we
 * only get to default our values when the functions are already exported
 * to the broker, which happens on a late stage. This could cause a fast
 * client to get an inconsistent state from our server */
/* TODO: Avoid exporting the functions before we have initialized
 * our server with the default values */
smio_err_e swap_config_defaults (char *broker_endp, char *service,
       const char *log_file_name)
{
    (void) log_file_name;
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO, "[sm_io:swap_defaults] Configuring SMIO "
            "SWAP with default values ...\n");
    bpm_client_err_e client_err = BPM_CLIENT_SUCCESS;
    smio_err_e err = SMIO_SUCCESS;

    bpm_client_t *config_client = bpm_client_new_log_mode (broker_endp, 0,
            log_file_name, SMIO_SWAP_LIBBPMCLIENT_LOG_MODE);
    ASSERT_ALLOC(config_client, err_alloc_client);

    client_err = bpm_set_sw (config_client, service, SWAP_DFLT_SW);
    ASSERT_TEST(client_err == BPM_CLIENT_SUCCESS, "Could not set switching state",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = bpm_set_sw_en (config_client, service, SWAP_DFLT_SW_EN);
    ASSERT_TEST(client_err == BPM_CLIENT_SUCCESS, "Could not set switching enable state",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = bpm_set_gain_a (config_client, service, SWAP_DFLT_GAIN_AA,
            SWAP_DFLT_GAIN_AC);
    ASSERT_TEST(client_err == BPM_CLIENT_SUCCESS, "Could not set channel A gain",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = bpm_set_gain_b (config_client, service, SWAP_DFLT_GAIN_BB,
            SWAP_DFLT_GAIN_BD);
    ASSERT_TEST(client_err == BPM_CLIENT_SUCCESS, "Could not set channel B gain",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = bpm_set_gain_c (config_client, service, SWAP_DFLT_GAIN_CC,
            SWAP_DFLT_GAIN_CA);
    ASSERT_TEST(client_err == BPM_CLIENT_SUCCESS, "Could not set channel C gain",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = bpm_set_gain_d (config_client, service, SWAP_DFLT_GAIN_DD,
            SWAP_DFLT_GAIN_DB);
    ASSERT_TEST(client_err == BPM_CLIENT_SUCCESS, "Could not set channel D gain",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

err_param_set:
    bpm_client_destroy (&config_client);
err_alloc_client:
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO, "[sm_io:swap_defaults] Exiting Config thread %s\n",
        service);
    return err;
}
