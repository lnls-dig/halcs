/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "halcs_server.h"
/* Private headers */
#include "sm_io_fmc250m_4ch_defaults.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, SM_IO, "[sm_io:fmc250m_4ch_defaults]",    \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)               \
    ASSERT_HAL_ALLOC(ptr, SM_IO, "[sm_io:fmc250m_4ch_defaults]",            \
            smio_err_str(SMIO_ERR_ALLOC),                                   \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                            \
    CHECK_HAL_ERR(err, SM_IO, "[sm_io:fmc250m_4ch_defaults]",               \
            smio_err_str (err_type))

#define SMIO_FMC250M_4CH_LIBHALCSCLIENT_LOG_MODE                "a"

/* We use the actual libclient to send and configure our default values,
 * maintaining internal consistency. So, in fact, we are sending ourselves
 * a message containing the default values. Because of this approach, we
 * only get to default our values when the functions are already exported
 * to the broker, which happens on a late stage. This could cause a fast
 * client to get an inconsistent state from our server */
/* TODO: Avoid exporting the functions before we have initialized
 * our server with the default values */
smio_err_e fmc250m_4ch_config_defaults (char *broker_endp, char *service,
       const char *log_file_name)
{
    (void) broker_endp;
    (void) service;
    (void) log_file_name;
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO, "[sm_io:fmc250m_4ch_defaults] Configuring SMIO "
            "FMC250M_4CH with default values ...\n");
    smio_err_e err = SMIO_SUCCESS;
    halcs_client_err_e client_err = HALCS_CLIENT_SUCCESS;

    /* We are not using the default config_defaults () callback to
     * initialize our ADC ISLA216P. Instead we rely on a more complex
     * method by using the do_mgmt_op () callback. This is needed because
     * the FMC250M is instantiated before FMC_ACTIVE_CLK SMIO, and therefore
     * no clock is available for the ISLA216P if we do it here.
     */

    /* For FMC250M Active version the approach of using the do_mgmt_op ()
     * callback works fine, but for Passive versions, we need to have
     * initialize the ADCs somewhere else, as we will never receive the
     * "INIT_OK" message from FMC_ACTIVE_CLK SMIO, as there are none.
     *
     * For Active version this is still valid, but we will initialize the ADCs
     * twice, which is fine as well.
     * */

    /* For some reason, the default timeout is not enough for FMC250M SMIO. See github issue
     * #119 */
    halcs_client_t *config_client = halcs_client_new_log_mode (broker_endp, 0,
            log_file_name, SMIO_FMC250M_4CH_LIBHALCSCLIENT_LOG_MODE);
    ASSERT_ALLOC(config_client, err_alloc_client);

    client_err = halcs_set_rst_adcs (config_client, service, FMC250M_4CH_DFLT_RST_ADCS);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not reset ADCs",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = halcs_set_rst_div_adcs (config_client, service, FMC250M_4CH_DFLT_RST_DIV_ADCS);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could not reset DIV CLK ADCs",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    client_err = halcs_set_sleep_adcs (config_client, service, FMC250M_4CH_DFLT_SLEEP_ADCS);
    ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could set activate ADCs",
            err_param_set, SMIO_ERR_CONFIG_DFLT);

    /*
     * After resetting the ADCs with halcs_set_rst_adcs (), all of its functions are
     * reset, even SPI mode. So, we need to restore the 4-wire mode with halcs_set_portconfig_adc ()
     */
    uint32_t i = 0;
    for (i = 0; i < NUM_FMC250M_4CH_ISLA216P; ++i) {
        client_err = halcs_set_rst_modes_adc (config_client, service, i, FMC250M_4CH_DFLT_RST_MODE_ADC);
        client_err |= halcs_set_portconfig_adc (config_client, service, i, FMC250M_4CH_DFLT_PORTCONFIG_ADC);
        ASSERT_TEST(client_err == HALCS_CLIENT_SUCCESS, "Could set ADCs to default parameters",
                err_param_set, SMIO_ERR_CONFIG_DFLT);
    }

err_param_set:
    halcs_client_destroy (&config_client);
err_alloc_client:
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_INFO, "[sm_io:fmc250m_4ch_defaults] Exiting Config thread %s\n",
        service);
    return err;
}
