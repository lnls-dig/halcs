/****************************************************************************
 * core/sm_io/src/sm_io/c/modules/rffe_scpi/sm_io_rffe_scpi_core.c
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
/* Private headers */
#include "sm_io_rffe_scpi_core.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, SM_IO, "[sm_io_rffe_scpi_core]",       \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)       \
    ASSERT_HAL_ALLOC(ptr, SM_IO, "[sm_io_rffe_scpi_core]",               \
            smio_err_str(SMIO_ERR_ALLOC),                           \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                    \
    CHECK_HAL_ERR(err, SM_IO, "[sm_io_rffe_scpi_core]",                  \
            smio_err_str (err_type))

/* Creates a new instance of Device Information */
smio_rffe_scpi_t * smio_rffe_scpi_new (smio_t *parent)
{
    smio_rffe_scpi_t *self = (smio_rffe_scpi_t *) zmalloc (sizeof *self);
    ASSERT_ALLOC(self, err_self_alloc);

    /* Create BSMP protocol for RFFE */
    self->smpr_ctl = smpr_scpi_new ();
    self->smch_ctl = smch_rffe_scpi_new (parent, smpr_scpi_get_ops (self->smpr_ctl), 0);
    ASSERT_ALLOC(self->smch_ctl, err_rffe_alloc);

    return self;

err_rffe_alloc:
    free (self);
err_self_alloc:
    return NULL;
}

/* Destroy an instance of the Device Information */
smio_err_e smio_rffe_scpi_destroy (smio_rffe_scpi_t **self_p)
{
    assert (self_p);

    if (*self_p) {
        smio_rffe_scpi_t *self = *self_p;

        smch_rffe_scpi_destroy (&self->smch_ctl);
        free (self);
        *self_p = NULL;
    }

    return SMIO_SUCCESS;
}
