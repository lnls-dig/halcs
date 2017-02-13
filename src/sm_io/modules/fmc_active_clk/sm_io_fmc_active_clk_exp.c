/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "halcs_server.h"
/* Private headers */
#include "sm_io_fmc_active_clk_codes.h"
#include "sm_io_fmc_active_clk_defaults.h"
#include "sm_io_fmc_active_clk_exports.h"
#include "sm_io_fmc_active_clk_core.h"
#include "sm_io_fmc_active_clk_exp.h"
#include "hw/wb_fmc_active_clk_regs.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...)  \
    ASSERT_HAL_TEST(test_boolean, SM_IO, "[sm_io:fmc_active_clk_exp]",          \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)                   \
    ASSERT_HAL_ALLOC(ptr, SM_IO, "[sm_io:fmc_active_clk_exp]",                  \
            smio_err_str(SMIO_ERR_ALLOC),                                       \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                                \
    CHECK_HAL_ERR(err, SM_IO, "[sm_io:fmc_active_clk_exp]",                     \
            smio_err_str (err_type))

static smch_err_e smch_ad9510_cfg_defaults_compat (smch_ad9510_t *self,
        uint32_t *param);

/************************************************************/
/*********** Specific FMC_ACTIVE_CLK Operations *************/
/************************************************************/

#define HALCS_FMC_ACTIVE_CLK_SI571_OE_MIN             0 /* SI571 Output disable */
#define HALCS_FMC_ACTIVE_CLK_SI571_OE_MAX             1 /* SI571 Output enable */

RW_PARAM_FUNC(fmc_active_clk, si571_oe) {
    SET_GET_PARAM(fmc_active_clk, 0x0, WB_FMC_ACTIVE_CLK_CSR,
            CLK_DISTRIB, SI571_OE, SINGLE_BIT_PARAM,
            HALCS_FMC_ACTIVE_CLK_SI571_OE_MIN, HALCS_FMC_ACTIVE_CLK_SI571_OE_MAX, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

#define HALCS_FMC_ACTIVE_CLK_PLL_FUNC_MIN             0 /* PLL FUNCTION pin 0 */
#define HALCS_FMC_ACTIVE_CLK_PLL_FUNC_MAX             1 /* PLL FUNCTION pin 1 */

RW_PARAM_FUNC(fmc_active_clk, pll_func) {
    SET_GET_PARAM(fmc_active_clk, 0x0, WB_FMC_ACTIVE_CLK_CSR,
            CLK_DISTRIB, PLL_FUNCTION, SINGLE_BIT_PARAM,
            HALCS_FMC_ACTIVE_CLK_PLL_FUNC_MIN, HALCS_FMC_ACTIVE_CLK_PLL_FUNC_MAX, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}


#define HALCS_FMC_ACTIVE_CLK_PLL_STATUS_MIN           0 /* PLL STATUS pin 0 */
#define HALCS_FMC_ACTIVE_CLK_PLL_STATUS_MAX           1 /* PLL STATUS pin 1 */

RW_PARAM_FUNC(fmc_active_clk, pll_status) {
    SET_GET_PARAM(fmc_active_clk, 0x0, WB_FMC_ACTIVE_CLK_CSR,
            CLK_DISTRIB, PLL_STATUS, SINGLE_BIT_PARAM,
            HALCS_FMC_ACTIVE_CLK_PLL_STATUS_MIN, HALCS_FMC_ACTIVE_CLK_PLL_STATUS_MAX, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

#define HALCS_FMC_ACTIVE_CLK_CLK_SEL_MIN              0 /* PLL CLK_SEL pin 0 */
#define HALCS_FMC_ACTIVE_CLK_CLK_SEL_MAX              1 /* PLL CLK_SEL pin 1 */

RW_PARAM_FUNC(fmc_active_clk, clk_sel) {
    SET_GET_PARAM(fmc_active_clk, 0x0, WB_FMC_ACTIVE_CLK_CSR,
            CLK_DISTRIB, CLK_SEL, SINGLE_BIT_PARAM,
            HALCS_FMC_ACTIVE_CLK_CLK_SEL_MIN, HALCS_FMC_ACTIVE_CLK_CLK_SEL_MAX, NO_CHK_FUNC,
            NO_FMT_FUNC, SET_FIELD);
}

/******************************** Chips Export functions *************************/

/* Macros to avoid repetition of the function body AD9510 */
typedef smch_err_e (*smch_ad9510_func_fp) (smch_ad9510_t *self, uint32_t *param);

#define FMC_ACTIVE_CLK_AD9510_FUNC_NAME(func_name)                                 \
    _fmc_active_clk_ad9510_ ## func_name

#define FMC_ACTIVE_CLK_AD9510_FUNC_NAME_HEADER(func_name)                          \
    static int FMC_ACTIVE_CLK_AD9510_FUNC_NAME(func_name) (void *owner, void *args, void *ret)

#define FMC_ACTIVE_CLK_AD9510_FUNC_BODY(owner, args, ret, read_func, write_func,   \
            error_msg)                                                          \
    do {                                                                        \
        assert (owner);                                                         \
        assert (args);                                                          \
                                                                                \
        int err = -FMC_ACTIVE_CLK_OK;                                           \
        SMIO_OWNER_TYPE *self = SMIO_EXP_OWNER(owner);                          \
        smio_fmc_active_clk_t *fmcaclk = smio_get_handler (self);               \
        ASSERT_TEST(fmcaclk != NULL, "Could not get SMIO FMC ACTIVE CLK handler", \
                err_get_fmcaclk_handler, -FMC_ACTIVE_CLK_ERR);                  \
        smch_ad9510_t *smch_ad9510 = SMIO_AD9510_HANDLER(fmcaclk);              \
        uint32_t rw = *(uint32_t *) EXP_MSG_ZMQ_FIRST_ARG(args);                \
        uint32_t param = *(uint32_t *) EXP_MSG_ZMQ_NEXT_ARG(args);              \
                                                                                \
        DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fmc_active_clk_exp] Calling " \
                "AD9510 function\n");                                           \
                                                                                \
        smch_err_e serr = SMCH_SUCCESS;                                         \
        /* Call specific function */                                            \
        if (rw) {                                                               \
            WHEN(ISEMPTY(read_func))(                                           \
                UNUSED(ret);                                                     \
                DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fmc_active_clk_exp] " \
                        "AD9510 read function not implemented\n");              \
                err = -FMC_ACTIVE_CLK_UNINPL;                                   \
                return err;                                                     \
            )                                                                   \
            WHENNOT(ISEMPTY(read_func))(                                        \
                uint32_t value = 0;                                             \
                serr = ((smch_ad9510_func_fp) read_func) (smch_ad9510,          \
                        &value);                                                \
                if (serr != SMCH_SUCCESS) {                                     \
                    err = -FMC_ACTIVE_CLK_ERR;                                  \
                }                                                               \
                else {                                                          \
                    *((uint32_t *) ret) = value;                                \
                    err = sizeof (value);                                       \
                    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fmc_active_clk_exp] " \
                            "AD9510 function read value = 0x%08X\n", value);    \
                }                                                               \
            )                                                                   \
        }                                                                       \
        else {                                                                  \
            WHEN(ISEMPTY(write_func))(                                          \
                DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fmc_active_clk_exp] " \
                        "AD9510 write function not implemented\n");             \
                err = -FMC_ACTIVE_CLK_UNINPL;                                   \
                return err;                                                     \
            )                                                                   \
            WHENNOT(ISEMPTY(write_func))(                                       \
                serr = ((smch_ad9510_func_fp) write_func) (smch_ad9510,         \
                        &param);                                                \
                if (serr != SMCH_SUCCESS) {                                     \
                    err = -FMC_ACTIVE_CLK_ERR;                                  \
                }                                                               \
                else {                                                          \
                    err = -FMC_ACTIVE_CLK_OK;                                   \
                }                                                               \
            )                                                                   \
        }                                                                       \
                                                                                \
err_get_fmcaclk_handler:                                                        \
        return err;                                                             \
    } while(0)

static smch_err_e smch_ad9510_cfg_defaults_compat (smch_ad9510_t *self, uint32_t *param)
{
    UNUSED(param);
    return smch_ad9510_cfg_defaults (self);
}

FMC_ACTIVE_CLK_AD9510_FUNC_NAME_HEADER(cfg_defaults)
{
    FMC_ACTIVE_CLK_AD9510_FUNC_BODY(owner, args, ret, /* No read function */,
            smch_ad9510_cfg_defaults_compat, "Could not set/get AD9510 defaults");
}

FMC_ACTIVE_CLK_AD9510_FUNC_NAME_HEADER(pll_a_div)
{
    FMC_ACTIVE_CLK_AD9510_FUNC_BODY(owner, args, ret, smch_ad9510_get_pll_a_div,
            smch_ad9510_set_pll_a_div, "Could not set/get AD9510 PLL A divider");
}

FMC_ACTIVE_CLK_AD9510_FUNC_NAME_HEADER(pll_b_div)
{
    FMC_ACTIVE_CLK_AD9510_FUNC_BODY(owner, args, ret, smch_ad9510_get_pll_b_div,
            smch_ad9510_set_pll_b_div, "Could not set/get AD9510 PLL B divider");
}

FMC_ACTIVE_CLK_AD9510_FUNC_NAME_HEADER(pll_prescaler)
{
    FMC_ACTIVE_CLK_AD9510_FUNC_BODY(owner, args, ret, smch_ad9510_get_pll_prescaler,
            smch_ad9510_set_pll_prescaler, "Could not set/get AD9510 PLL Prescaler");
}

FMC_ACTIVE_CLK_AD9510_FUNC_NAME_HEADER(r_div)
{
    FMC_ACTIVE_CLK_AD9510_FUNC_BODY(owner, args, ret, smch_ad9510_get_r_div,
            smch_ad9510_set_r_div, "Could not set/get AD9510 R Divider");
}

FMC_ACTIVE_CLK_AD9510_FUNC_NAME_HEADER(pll_pdown)
{
    FMC_ACTIVE_CLK_AD9510_FUNC_BODY(owner, args, ret, smch_ad9510_get_pll_pdown,
            smch_ad9510_set_pll_pdown, "Could not set/get AD9510 PLL power down");
}

FMC_ACTIVE_CLK_AD9510_FUNC_NAME_HEADER(mux_status)
{
    FMC_ACTIVE_CLK_AD9510_FUNC_BODY(owner, args, ret, smch_ad9510_get_mux_status,
            smch_ad9510_set_mux_status, "Could not set/get AD9510 MUX status");
}

FMC_ACTIVE_CLK_AD9510_FUNC_NAME_HEADER(cp_current)
{
    FMC_ACTIVE_CLK_AD9510_FUNC_BODY(owner, args, ret, smch_ad9510_get_cp_current,
        smch_ad9510_set_cp_current, "Could not set/get AD9510 CP current");
}

FMC_ACTIVE_CLK_AD9510_FUNC_NAME_HEADER(outputs)
{
    FMC_ACTIVE_CLK_AD9510_FUNC_BODY(owner, args, ret, smch_ad9510_get_outputs,
        smch_ad9510_set_outputs, "Could not set/get AD9510 Outputs");
}

FMC_ACTIVE_CLK_AD9510_FUNC_NAME_HEADER(pll_clk_sel)
{
    FMC_ACTIVE_CLK_AD9510_FUNC_BODY(owner, args, ret, smch_ad9510_get_pll_clk_sel,
            smch_ad9510_set_pll_clk_sel,
            "Could not set/get AD9510 PLL CLK selection");
}

/* Macros to avoid repetition of the function body Si57X */
typedef smch_err_e (*smch_si57x_func_fp) (smch_si57x_t *self, double *param);

#define FMC_ACTIVE_CLK_SI571_FUNC_NAME(func_name)                               \
    _fmc_active_clk_si571_ ## func_name

#define FMC_ACTIVE_CLK_SI571_FUNC_NAME_HEADER(func_name)                        \
    static int FMC_ACTIVE_CLK_SI571_FUNC_NAME(func_name) (void *owner, void *args, void *ret)

#define FMC_ACTIVE_CLK_SI571_FUNC_BODY(owner, args, ret, read_func, write_func, \
        error_msg)                                                              \
    do {                                                                        \
        UNUSED(ret);                                                             \
        assert (owner);                                                         \
        assert (args);                                                          \
                                                                                \
        int err = -FMC_ACTIVE_CLK_OK;                                           \
        SMIO_OWNER_TYPE *self = SMIO_EXP_OWNER(owner);                          \
        smio_fmc_active_clk_t *fmcaclk = smio_get_handler (self);               \
        ASSERT_TEST(fmcaclk != NULL, "Could not get SMIO FMC ACTIVE CLK handler", \
                err_get_fmcaclk_handler, -FMC_ACTIVE_CLK_ERR);                  \
        smch_si57x_t *smch_si57x = SMIO_SI57X_HANDLER(fmcaclk);                 \
        uint32_t rw = *(uint32_t *) EXP_MSG_ZMQ_FIRST_ARG(args);                \
        double param = *(double *) EXP_MSG_ZMQ_NEXT_ARG(args);                  \
                                                                                \
        smch_err_e serr = SMCH_SUCCESS;                                         \
        /* Call specific function */                                            \
        if (rw) {                                                               \
            WHEN(ISEMPTY(read_func))(                                           \
                UNUSED(ret);                                                     \
                DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fmc_active_clk_exp] " \
                        "SI57x read function not implemented\n");               \
                err = -FMC_ACTIVE_CLK_UNINPL;                                   \
                return err;                                                     \
            )                                                                   \
            WHENNOT(ISEMPTY(read_func))(                                        \
                double value = 0;                                               \
                serr = ((smch_si57x_func_fp) read_func) (smch_si57x,            \
                        &value);                                                \
                if (serr != SMCH_SUCCESS) {                                     \
                    err = -FMC_ACTIVE_CLK_ERR;                                  \
                }                                                               \
                else {                                                          \
                    *((double *) ret) = value;                                  \
                    err = sizeof (value);                                       \
                    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fmc_active_clk_exp] " \
                            "SI57x function read value = 0x%f\n", value);       \
                }                                                               \
            )                                                                   \
        }                                                                       \
        else {                                                                  \
            WHEN(ISEMPTY(write_func))(                                          \
                DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fmc_active_clk_exp] " \
                        "SI57x write function not implemented\n");              \
                err = -FMC_ACTIVE_CLK_UNINPL;                                   \
                return err;                                                     \
            )                                                                   \
            WHENNOT(ISEMPTY(write_func))(                                       \
                serr = ((smch_si57x_func_fp) write_func) (smch_si57x,           \
                        &param);                                                \
                if (serr != SMCH_SUCCESS) {                                     \
                    err = -FMC_ACTIVE_CLK_ERR;                                  \
                }                                                               \
                else {                                                          \
                    err = -FMC_ACTIVE_CLK_OK;                                   \
                }                                                               \
            )                                                                   \
        }                                                                       \
                                                                                \
err_get_fmcaclk_handler:                                                        \
        return err;                                                             \
                                                                                \
    } while(0)

FMC_ACTIVE_CLK_SI571_FUNC_NAME_HEADER(freq)
{
    FMC_ACTIVE_CLK_SI571_FUNC_BODY(owner, args, ret, smch_si57x_get_freq,
            smch_si57x_set_freq, "Could not set SI571 frequency");
}

static smch_err_e smch_si57x_get_defaults_compat (smch_si57x_t *self, double *param)
{
    return smch_si57x_get_defaults (self, *param);
}

FMC_ACTIVE_CLK_SI571_FUNC_NAME_HEADER(get_defaults)
{
    FMC_ACTIVE_CLK_SI571_FUNC_BODY(owner, args, ret, /* No read func*/,
            smch_si57x_get_defaults_compat, "Could not restart SI571 to its defaults");
}

static int _fmc_active_clk_rst_isla216p (void *owner, void *args, void *ret)
{
    UNUSED(ret);
    assert (owner);
    assert (args);
    int err = -FMC_ACTIVE_CLK_OK;
    smio_err_e serr = SMIO_SUCCESS;
    SMIO_OWNER_TYPE *self = SMIO_EXP_OWNER(owner);
    uint32_t inst_id = smio_get_inst_id (self);

    uint32_t rw = *(uint32_t *) EXP_MSG_ZMQ_FIRST_ARG(args);
    UNUSED(rw);

    uint32_t value = *(uint32_t *) EXP_MSG_ZMQ_NEXT_ARG(args);

    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fmc_active_clk] "
            "Calling _fmc_active_clk_rst_isla216p\n");

    /* Tell FMC250M_4CH that we have initialized */
    if (value == 0x1) {
        serr = smio_send_mgmt_msg (self, 0x68e3b1af, inst_id, "INIT_OK");
        ASSERT_TEST(serr == SMIO_SUCCESS, "Could not send MGMT message to FMC250M", err_send_mgmt_msg,
               -FMC_ACTIVE_CLK_ERR);
    }

err_send_mgmt_msg:
    return err;
}

/* Macros to avoid repetition of the function body AD9510 */
typedef smch_err_e (*smch_ad9510_func_fp2) (smch_ad9510_t *self, uint32_t *addr,
        uint32_t *param);

#define FMC_ACTIVE_CLK_AD9510_FUNC_BODY2(owner, args, ret, max_addr, read_func, write_func,   \
                error_msg)                                                         \
        do {                                                                       \
            assert (owner);                                                        \
            assert (args);                                                         \
                                                                                   \
            int err = -FMC_ACTIVE_CLK_OK;                                          \
            SMIO_OWNER_TYPE *self = SMIO_EXP_OWNER(owner);                         \
            smio_fmc_active_clk_t *fmcaclk = smio_get_handler (self);              \
            ASSERT_TEST(fmcaclk != NULL, "Could not get SMIO FMC130M handler",     \
                    err_get_fmcaclk_handler, -FMC_ACTIVE_CLK_ERR);                 \
            uint32_t rw = *(uint32_t *) EXP_MSG_ZMQ_FIRST_ARG(args);               \
            uint32_t addr = *(uint32_t *) EXP_MSG_ZMQ_NEXT_ARG(args);              \
            uint32_t param = *(uint32_t *) EXP_MSG_ZMQ_NEXT_ARG(args);             \
                                                                                   \
            ASSERT_TEST(addr < max_addr, "Received AD9510 addr too big",           \
                    err_rcv_addr, -FMC_ACTIVE_CLK_ERR);                            \
                                                                                   \
            DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fmc_active_clk_exp] Calling " \
                    "AD9510 function, for addr #%u\n", addr);                      \
                                                                                   \
            smch_ad9510_t *smch_ad9510 = SMIO_AD9510_HANDLER(fmcaclk);             \
                                                                                   \
            smch_err_e serr = SMCH_SUCCESS;                                        \
            /* Call specific function */                                           \
            if (rw) {                                                              \
                WHEN(ISEMPTY(read_func))(                                          \
                    UNUSED(ret);                                                    \
                    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fmc_active_clk_exp] " \
                            "AD9510 read function not implemented\n");             \
                    err = -FMC_ACTIVE_CLK_UNINPL;                                  \
                    return err;                                                    \
                )                                                                  \
                WHENNOT(ISEMPTY(read_func))(                                       \
                    UNUSED(param);                                                  \
                    uint32_t value = 0;                                            \
                    serr = ((smch_ad9510_func_fp2) read_func) (smch_ad9510, &addr,  \
                            &value);                                               \
                    if (serr != SMCH_SUCCESS) {                                    \
                        err = -FMC_ACTIVE_CLK_ERR;                                 \
                    }                                                              \
                    else {                                                         \
                        *((uint32_t *) ret) = value;                               \
                        err = sizeof (value);                                      \
                        DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fmc_active_clk_exp] " \
                                "AD9510 function read value = 0x%08X\n", value);   \
                    }                                                              \
                )                                                                  \
            }                                                                      \
            else {                                                                 \
                WHEN(ISEMPTY(write_func))(                                         \
                    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fmc_active_clk_exp] " \
                            "AD9510 write function not implemented\n");            \
                    err = -FMC_ACTIVE_CLK_UNINPL;                                  \
                    return err;                                                    \
                )                                                                  \
                WHENNOT(ISEMPTY(write_func))(                                      \
                    serr = ((smch_ad9510_func_fp2) write_func) (smch_ad9510, &addr, \
                            &param);                                               \
                    if (serr != SMCH_SUCCESS) {                                    \
                        err = -FMC_ACTIVE_CLK_ERR;                                 \
                    }                                                              \
                    else {                                                         \
                        err = -FMC_ACTIVE_CLK_OK;                                  \
                    }                                                              \
                )                                                                  \
            }                                                                      \
                                                                                   \
    err_rcv_addr:                                                                  \
    err_get_fmcaclk_handler:                                                       \
            return err;                                                            \
        } while(0)

static smch_err_e smch_ad9510_read_8_compat (smch_ad9510_t *self,
        uint32_t *addr_arg, uint32_t *data_arg)
{
    uint8_t addr = *(uint8_t *) addr_arg;
    uint8_t *data = (uint8_t * ) data_arg;
    return smch_ad9510_read_8 (self, addr, data);
}

static smch_err_e smch_ad9510_write_8_compat (smch_ad9510_t *self,
        uint32_t *addr_arg, uint32_t *data_arg)
{
    uint8_t addr = *(uint8_t *) addr_arg;
    const uint8_t *data = (const uint8_t *) data_arg;
    return smch_ad9510_write_8_update (self, addr, data);
}

/* AD9510 has valid addresses up to 0x5A */
#define HALCS_FMC_ACTIVE_CLK_AD9510_MAX_ADDR           (0x5A)

FMC_ACTIVE_CLK_AD9510_FUNC_NAME_HEADER(data)
{
    FMC_ACTIVE_CLK_AD9510_FUNC_BODY2(owner, args, ret, HALCS_FMC_ACTIVE_CLK_AD9510_MAX_ADDR,
            smch_ad9510_read_8_compat, smch_ad9510_write_8_compat,
            "Could not read/write AD9510");
}

/* Exported function pointers */
const disp_table_func_fp fmc_active_clk_exp_fp [] = {
    RW_PARAM_FUNC_NAME(fmc_active_clk, si571_oe),
    RW_PARAM_FUNC_NAME(fmc_active_clk, pll_func),
    RW_PARAM_FUNC_NAME(fmc_active_clk, pll_status),
    RW_PARAM_FUNC_NAME(fmc_active_clk, clk_sel),
    FMC_ACTIVE_CLK_AD9510_FUNC_NAME(cfg_defaults),
    FMC_ACTIVE_CLK_AD9510_FUNC_NAME(pll_a_div),
    FMC_ACTIVE_CLK_AD9510_FUNC_NAME(pll_b_div),
    FMC_ACTIVE_CLK_AD9510_FUNC_NAME(pll_prescaler),
    FMC_ACTIVE_CLK_AD9510_FUNC_NAME(r_div),
    FMC_ACTIVE_CLK_AD9510_FUNC_NAME(pll_pdown),
    FMC_ACTIVE_CLK_AD9510_FUNC_NAME(mux_status),
    FMC_ACTIVE_CLK_AD9510_FUNC_NAME(cp_current),
    FMC_ACTIVE_CLK_AD9510_FUNC_NAME(outputs),
    FMC_ACTIVE_CLK_AD9510_FUNC_NAME(pll_clk_sel),
    FMC_ACTIVE_CLK_SI571_FUNC_NAME(freq),
    FMC_ACTIVE_CLK_SI571_FUNC_NAME(get_defaults),
    _fmc_active_clk_rst_isla216p,
    FMC_ACTIVE_CLK_AD9510_FUNC_NAME(data),
    NULL
};

/************************************************************/
/***************** Export methods functions *****************/
/************************************************************/

static smio_err_e _fmc_active_clk_do_op (void *owner, void *msg);

/* Attach an instance of sm_io to dev_io function pointer */
smio_err_e fmc_active_clk_attach (smio_t *self, devio_t *parent)
{
    UNUSED(self);
    UNUSED(parent);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Deattach an instance of sm_io to dev_io function pointer */
smio_err_e fmc_active_clk_deattach (smio_t *self)
{
    UNUSED(self);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Export (register) sm_io to handle operations function pointer */
smio_err_e fmc_active_clk_export_ops (smio_t *self,
        const disp_op_t** smio_exp_ops)
{
    UNUSED(self);
    UNUSED(smio_exp_ops);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

/* Unexport (unregister) sm_io to handle operations function pointer */
smio_err_e fmc_active_clk_unexport_ops (smio_t *self)
{
    UNUSED(self);
    return SMIO_ERR_FUNC_NOT_IMPL;
}


/* Generic wrapper for receiving opcodes and arguments to specific funtions function pointer */
/* FIXME: Code repetition! _devio_do_smio_op () function does almost the same!!! */
smio_err_e _fmc_active_clk_do_op (void *owner, void *msg)
{
    UNUSED(owner);
    UNUSED(msg);
    return SMIO_ERR_FUNC_NOT_IMPL;
}

smio_err_e fmc_active_clk_do_op (void *self, void *msg)
{
    return _fmc_active_clk_do_op (self, msg);
}

const smio_ops_t fmc_active_clk_ops = {
    .attach             = fmc_active_clk_attach,          /* Attach sm_io instance to dev_io */
    .deattach           = fmc_active_clk_deattach,        /* Deattach sm_io instance to dev_io */
    .export_ops         = fmc_active_clk_export_ops,      /* Export sm_io operations to dev_io */
    .unexport_ops       = fmc_active_clk_unexport_ops,    /* Unexport sm_io operations to dev_io */
    .do_op              = fmc_active_clk_do_op            /* Generic wrapper for handling specific operations */
};

/************************************************************/
/****************** Bootstrap Operations ********************/
/************************************************************/

smio_err_e fmc_active_clk_init (smio_t * self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fmc_active_clk_exp] Initializing fmc_active_clk\n");

    smio_err_e err = SMIO_SUCCESS;

    err = smio_set_id (self, FMC_ACTIVE_CLK_SDB_DEVID);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO id", err_set_id);
    err = smio_set_name (self, FMC_ACTIVE_CLK_SDB_NAME);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO name", err_set_name);

    /* Set SMIO ops pointers */
    err = smio_set_ops (self, &fmc_active_clk_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO operations",
            err_smio_set_ops);
    err = smio_set_thsafe_client_ops (self, &smio_thsafe_client_zmq_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO thsafe operations",
            err_smio_set_thsafe_ops);

    /* Fill the disp_op_t description structure with the callbacks. */

    /* disp_op_t structure is const and all of the functions performing on it
     * obviously receives a const argument, but here (and only on the SMIO
     * initialization) we need to make an exception if we want to keep the
     * functions' description and the function pointers separate */
    err = smio_init_exp_ops (self, (disp_op_t **) fmc_active_clk_exp_ops,
            fmc_active_clk_exp_fp);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not fill SMIO "
            "function descriptors with the callbacks", err_fill_desc);

    err = smio_set_exp_ops (self, fmc_active_clk_exp_ops);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO exported operations",
            err_smio_set_exp_ops);

    /* Initialize specific structure */
    smio_fmc_active_clk_t *smio_handler = smio_fmc_active_clk_new (self);
    ASSERT_ALLOC(smio_handler, err_smio_handler_alloc, SMIO_ERR_ALLOC);
    err = smio_set_handler (self, smio_handler);
    ASSERT_TEST(err == SMIO_SUCCESS, "Could not set SMIO handler",
            err_smio_set_handler);

    return err;

err_smio_set_handler:
    smio_fmc_active_clk_destroy (&smio_handler);
err_smio_handler_alloc:
    smio_set_exp_ops (self, NULL);
err_smio_set_exp_ops:
err_fill_desc:
    smio_set_thsafe_client_ops (self, NULL);
err_smio_set_thsafe_ops:
    smio_set_ops (self, NULL);
err_smio_set_ops:
err_set_name:
err_set_id:
    return err;
}

/* Destroy sm_io instance of fmc_active_clk */
smio_err_e fmc_active_clk_shutdown (smio_t *self)
{
    DBE_DEBUG (DBG_SM_IO | DBG_LVL_TRACE, "[sm_io:fmc_active_clk_exp] Shutting down fmc_active_clk\n");

    smio_err_e err = SMIO_SUCCESS;
    smio_fmc_active_clk_t *fmcaclk = smio_get_handler (self);
    ASSERT_TEST(fmcaclk != NULL, "Could not get FMC ACTIVE CLK handler",
            err_fmcaclk_handler, SMIO_ERR_ALLOC /* FIXME: improve return code */);

    /* Destroy SMIO instance */
    smio_fmc_active_clk_destroy (&fmcaclk);
    /* Nullify operation pointers */
    smio_set_exp_ops (self, NULL);
    smio_set_thsafe_client_ops (self, NULL);
    smio_set_ops (self, NULL);

err_fmcaclk_handler:
    return err;
}

const smio_bootstrap_ops_t fmc_active_clk_bootstrap_ops = {
    .init = fmc_active_clk_init,
    .shutdown = fmc_active_clk_shutdown,
    .config_defaults = fmc_active_clk_config_defaults
};

SMIO_MOD_DECLARE(FMC_ACTIVE_CLK_SDB_DEVID, FMC_ACTIVE_CLK_SDB_NAME, fmc_active_clk_bootstrap_ops)
