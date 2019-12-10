/****************************************************************************
 * core/sm_io/src/sm_io/c/chips/sm_ch_rffe_scpi.c
 *
 *   Copyright (C) 2014 LNLS (www.lnls.br). All rights reserved.
 *   Copyright (C) 2019 Augusto Fraga Giachero. All rights reserved.
 *   Author: Augusto Fraga Giachero <afg@augustofg.net>
 *
 * This file is part of HALCS.
 *
 * HALCS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * HALCS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with HALCS. If not, see <https://www.gnu.org/licenses/>.
 *
 ****************************************************************************/

#include "halcs_server.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, SM_CH, "[sm_ch:rffe_scpi]",                \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)           \
    ASSERT_HAL_ALLOC(ptr, SM_CH, "[sm_ch:rffe_scpi]",                   \
            smch_err_str(SMCH_ERR_ALLOC),                               \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                        \
    CHECK_HAL_ERR(err, SM_CH, "[sm_ch:rffe_scpi]",                      \
            smch_err_str (err_type))

#define SMCH_RFFE_SCPI_NAME                    "RFFE_SCPI"
#define SMCH_RFFE_SCPI_USECS_WAIT              10000
#define SMCH_RFFE_SCPI_WAIT(usecs)             usleep(usecs)
#define SMCH_RFFE_SCPI_WAIT_DFLT               SMCH_RFFE_SCPI_WAIT(SMCH_RFFE_SCPI_USECS_WAIT)

struct _smch_rffe_scpi_t {
    smpr_t *proto;                                       /* PROTO protocol object */
};

smch_rffe_scpi_t * smch_rffe_scpi_new (smio_t *parent, const smpr_proto_ops_t *reg_ops,
	int verbose)
{
    UNUSED(verbose);
    assert (parent);

    smch_rffe_scpi_t *self = (smch_rffe_scpi_t *) zmalloc (sizeof *self);
    ASSERT_ALLOC(self, err_self_alloc);

    self->proto = smpr_new (SMCH_RFFE_SCPI_NAME, parent, reg_ops, verbose);
    ASSERT_ALLOC(self->proto, err_proto_alloc);

    /* Initalize the PROTO protocol */
    int smpr_err = smpr_open (self->proto, 0, NULL /* Default parameters are fine */);
    ASSERT_TEST(smpr_err == 0, "Could not initialize SMPR protocol", err_smpr_init);

    DBE_DEBUG (DBG_SM_CH | DBG_LVL_INFO, "[sm_ch:rffe_scpi] Created instance of SMCH\n");

    return self;

err_smpr_init:
    smpr_destroy (&self->proto);
err_proto_alloc:
    free (self);
err_self_alloc:
    return NULL;
}

/* Destroy an instance of the SMCH RFFE_SCPI */
smch_err_e smch_rffe_scpi_destroy (smch_rffe_scpi_t **self_p)
{
    assert (self_p);

    if (*self_p) {
        smch_rffe_scpi_t *self = *self_p;

        smpr_release (self->proto);
        smpr_destroy (&self->proto);
        free (self);
        *self_p = NULL;
    }

    return SMCH_SUCCESS;
}
