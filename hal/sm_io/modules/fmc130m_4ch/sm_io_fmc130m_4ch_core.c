/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include <stdlib.h>
#include <assert.h>
#include <czmq.h>

#include "sm_io_fmc130m_4ch_core.h"
#include "sm_io_err.h"
#include "hal_assert.h"
#include "hal_stddef.h"
#include "board.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, SM_IO, "[sm_io_fmc130m_4ch_core]", \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_ALLOC(ptr, SM_IO, "[sm_io_fmc130m_4ch_core]", \
            smio_err_str(SMIO_ERR_ALLOC),                   \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                            \
    CHECK_HAL_ERR(err, SM_IO, "[sm_io_fmc130m_4ch_core]",   \
            smio_err_str (err_type))

/* Creates a new instance of Device Information */
smio_fmc130m_4ch_t * smio_fmc130m_4ch_new (smio_t *parent)
{
    (void) parent;
    smio_fmc130m_4ch_t *self = (smio_fmc130m_4ch_t *) zmalloc (sizeof *self);
    ASSERT_ALLOC(self, err_self_alloc);

#ifdef __FMC130M_4CH_TYPE_ACTIVE__
    self->smch_ad9510 = smch_ad9510_new (parent, FMC_130M_AD9510_SPI_OFFS /* Offset
                to the beggining of the SMIO base */, FMC130M_4CH_AD9510_ADDR, 0);
    ASSERT_ALLOC(self->smch_ad9510, err_smch_ad9510_alloc);
#endif

    return self;

#ifdef __FMC130M_4CH_TYPE_ACTIVE__
err_smch_ad9510_alloc:
    free (self);
#endif
err_self_alloc:
    return NULL;
}

/* Destroy an instance of the Device Information */
smio_err_e smio_fmc130m_4ch_destroy (smio_fmc130m_4ch_t **self_p)
{
    assert (self_p);

    if (*self_p) {
        smio_fmc130m_4ch_t *self = *self_p;

#ifdef __FMC130M_4CH_TYPE_ACTIVE__
        smch_ad9510_destroy (&self->smch_ad9510);
#endif
        free (self);
        *self_p = NULL;
    }

    return SMIO_SUCCESS;
}

