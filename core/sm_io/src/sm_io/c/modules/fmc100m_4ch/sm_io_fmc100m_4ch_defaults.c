/*
 * Copyright (C) 2020 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "halcs_server.h"
/* Private headers */
#include "sm_io_fmc100m_4ch_defaults.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...)  \
    ASSERT_HAL_TEST(test_boolean, SM_IO, "[sm_io:fmc100m_4ch_defaults]",        \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)                   \
    ASSERT_HAL_ALLOC(ptr, SM_IO, "[sm_io:fmc100m_4ch_defaults]",                \
            smio_err_str(SMIO_ERR_ALLOC),                                       \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                                \
    CHECK_HAL_ERR(err, SM_IO, "[sm_io:fmc100m_4ch_defaults]",                   \
            smio_err_str (err_type))

#define SMIO_FMC_100M_4CH_LIBHALCSCLIENT_LOG_MODE                "a"
#define SMIO_FMC_100M_4CH_NUM_ADCS                                4

/* We use the actual libclient to send and configure our default values,
 * maintaining internal consistency. So, in fact, we are sending ourselves
 * a message containing the default values. Because of this approach, we
 * only get to default our values when the functions are already exported
 * to the broker, which happens on a late stage. This could cause a fast
 * client to get an inconsistent state from our server */
/* TODO: Avoid exporting the functions before we have initialized
 * our server with the default values */
smio_err_e fmc100m_4ch_config_defaults (char *broker_endp, char *service,
       const char *log_file_name)
{
    UNUSED(log_file_name);
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO, "[sm_io:fmc100m_4ch_defaults] Configuring SMIO "
            "FMC_100M_4CH with default values ...\n");
    halcs_client_err_e client_err = HALCS_CLIENT_SUCCESS;
    smio_err_e err = SMIO_SUCCESS;

    halcs_client_t *config_client = halcs_client_new_log_mode (broker_endp, 0,
            log_file_name, SMIO_FMC_100M_4CH_LIBHALCSCLIENT_LOG_MODE);
    ASSERT_ALLOC(config_client, err_alloc_client);

    client_err = halcs_set_si571_defaults (config_client, service, FMC_100M_4CH_DFLT_SI57X_FOUT_FACTORY);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not get Si571 defaults",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = halcs_set_si571_freq (config_client, service, FMC_100M_4CH_DFLT_SI57X_FOUT);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not set Si571 frequency",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = halcs_set_si571_oe (config_client, service, FMC_100M_4CH_DFLT_SI571_OE);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not enable SI571 Output",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    uint32_t status = 0x1;
    client_err = halcs_set_adc100_ltc2174_reset (config_client, service, status);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not reset LTC2174",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    status = 0x1;
    client_err = halcs_set_adc100_ltc2174_twos_compl (config_client, service, status);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not write FMC twos complement bit",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    status = 0x1;
    client_err = halcs_set_adc100_acq_led (config_client, service, status);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not turn LED on",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    status = 0x0;
    client_err = halcs_set_adc100_ltc2174_test_patt (config_client, service, status);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not write FMC test pattern enable",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    status = 0;
    client_err = halcs_get_adc100_ltc2174_test_patt (config_client, service, &status);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not read FMC test pattern enable",
            err_param_set, SMIO_ERR_CONFIG_DFLT);
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO, "[sm_io:fmc100m_4ch_defaults] FMC status pattern enable: 0x%08X\n",
        status);

    status = 0x1;
    client_err = halcs_set_adc100_ltc2174_test_patt_data (config_client, service, status);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not write FMC test pattern data",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    status = 0;
    client_err = halcs_get_adc100_ltc2174_test_patt_data (config_client, service, &status);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not read FMC test pattern data",
            err_param_set, SMIO_ERR_CONFIG_DFLT);
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO, "[sm_io:fmc100m_4ch_defaults] FMC status pattern data: 0x%08X\n",
        status);


    status = 0;
    client_err = halcs_get_adc100_status (config_client, service, &status);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not read FMC status",
            err_param_set, SMIO_ERR_CONFIG_DFLT);
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO, "[sm_io:fmc100m_4ch_defaults] FMC status: 0x%08X\n",
            status);

    uint32_t chan = 0;
    for (chan = 0; chan < SMIO_FMC_100M_4CH_NUM_ADCS; ++chan) {
        status = 0x8000;
        client_err = halcs_set_adc100_adc_calib_gain (config_client, service, chan, status);
        ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not write FMC gain",
                err_param_set, SMIO_ERR_CONFIG_DFLT);

        status = (1 << 16)-1;
        client_err = halcs_set_adc100_adc_sat (config_client, service, chan, status);
        ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not write FMC saturation",
                err_param_set, SMIO_ERR_CONFIG_DFLT);

        status = 0x44; /* 10V input range */
        client_err = halcs_set_adc100_adc_ssr (config_client, service, chan, status);
        ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not write SSR values",
                err_param_set, SMIO_ERR_CONFIG_DFLT);

        status = 0;
        client_err = halcs_get_adc100_adc_status (config_client, service, chan, &status);
        ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not read FMC status",
                err_param_set, SMIO_ERR_CONFIG_DFLT);
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO, "[sm_io:fmc100m_4ch_defaults] FMC CH%u status: 0x%08X\n",
                chan, status);
    }

    status = 0;
    client_err = halcs_get_adc100_fs_freq (config_client, service, &status);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not read FS Freq",
            err_param_set, SMIO_ERR_CONFIG_DFLT);
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO, "[sm_io:fmc100m_4ch_defaults] FMC FS Freq: %u\n",
        status);

err_param_set:
    halcs_client_destroy (&config_client);
err_alloc_client:
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO, "[sm_io:fmc100m_4ch_defaults] Exiting Config thread %s\n",
        service);
    return err;
}
