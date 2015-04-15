/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

/* This file is based on the work of Andrzej Wojenski for the FCS project,
 * available, to my knowledge, only in https://github.com/lerwys/fcs
 *
 * Description: Software driver for EEPROM RFFE chip
 */

#include "sm_ch_rffe.h"
#include "sm_pr.h"
#include "hal_assert.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, SM_CH, "[sm_ch:rffe]",                \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)           \
    ASSERT_HAL_ALLOC(ptr, SM_CH, "[sm_ch:rffe]",                        \
            smch_err_str(SMCH_ERR_ALLOC),                               \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                        \
    CHECK_HAL_ERR(err, SM_CH, "[sm_ch:rffe]",                           \
            smch_err_str (err_type))

#define SMCH_RFFE_NAME                    "BSMP_RFFE"
#define SMCH_RFFE_USECS_WAIT              10000
#define SMCH_RFFE_WAIT(usecs)             usleep(usecs)
#define SMCH_RFFE_WAIT_DFLT               SMCH_RFFE_WAIT(SMCH_RFFE_USECS_WAIT)

/* Creates a new instance of the SMCH RFFE */
smch_rffe_t * smch_rffe_new (smio_t *parent, int verbose)
{
    (void) verbose;
    assert (parent);

    smch_rffe_t *self = (smch_rffe_t *) zmalloc (sizeof *self);
    ASSERT_ALLOC(self, err_self_alloc);

    self->bsmp = smpr_new (SMCH_RFFE_NAME, parent, SMPR_BSMP, verbose);
    ASSERT_ALLOC(self->bsmp, err_bsmp_alloc);

    /* Initalize the BSMP protocol */
    int smpr_err = smpr_open (self->bsmp, 0, NULL /* Default parameters are fine */);
    ASSERT_TEST(smpr_err == 0, "Could not initialize SMPR protocol", err_smpr_init);

    DBE_DEBUG (DBG_SM_CH | DBG_LVL_INFO, "[sm_ch:rffe] Created instance of SMCH\n");
    return self;

err_smpr_init:
    smpr_destroy (&self->bsmp);
err_bsmp_alloc:
    free (self);
err_self_alloc:
    return NULL;
}

/* Destroy an instance of the SMCH RFFE */
smch_err_e smch_rffe_destroy (smch_rffe_t **self_p)
{
    assert (self_p);

    if (*self_p) {
        smch_rffe_t *self = *self_p;

        smpr_release (self->bsmp);
        smpr_destroy (&self->bsmp);
        free (self);
        *self_p = NULL;
    }

    return SMCH_SUCCESS;
}

smch_err_e smch_rffe_write_var (smch_rffe_t *self, uint32_t id, uint8_t *data,
        size_t size)
{
    assert (self);
    assert (data);

    smch_err_e err = SMCH_SUCCESS;

    smpr_err_e smpr_err = smpr_bsmp_write_var_by_id (self->bsmp, id, data, size);
    ASSERT_TEST(smpr_err == SMPR_SUCCESS, "Could not write variable to SMPR",
            err_smpr_write_var, SMCH_ERR_RW_SMPR);

err_smpr_write_var:
    return err;
}

smch_err_e smch_rffe_read_var (smch_rffe_t *self, uint32_t id, uint8_t *data,
        size_t size)
{
    assert (self);
    assert (data);

    smch_err_e err = SMCH_SUCCESS;

    smpr_err_e smpr_err = smpr_bsmp_read_var_by_id (self->bsmp, id, data, size);
    ASSERT_TEST(smpr_err == SMPR_SUCCESS, "Could not read variable to SMPR",
            err_smpr_read_var, SMCH_ERR_RW_SMPR);

err_smpr_read_var:
    return err;
}

