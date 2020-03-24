/****************************************************************************
 * core/sm_io/src/sm_io/c/protocols/ops/sm_pr_scpi.c
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
    ASSERT_HAL_TEST(test_boolean, SM_PR, "[sm_pr:scpi]",            \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)       \
    ASSERT_HAL_ALLOC(ptr, SM_PR, "[sm_pr:scpi]",                    \
            smpr_err_str(SMPR_ERR_ALLOC),                           \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                    \
    CHECK_HAL_ERR(err, SM_PR, "[sm_pr:scpi]",                       \
            smpr_err_str (err_type))

#define SMPR_PROTO_SCPI_CLIENT(smpr_handler)         (smpr_handler->client)

/* Device endpoint */
typedef struct {
    uint64_t base;                                      /* Core base address */
} smpr_proto_scpi_t;

/* Protocol object specification */
struct _smpr_scpi_t {
    /* Must be located first */
    smpr_proto_ops_t proto_ops;       /* SCPI protocol operations */
};

/*************** Our methods implementation **********/

/* Creates a new instance of the proto_scpi */
static smpr_proto_scpi_t * smpr_proto_scpi_new (uint64_t base)
{
    smpr_proto_scpi_t *self = (smpr_proto_scpi_t *) zmalloc (sizeof *self);
    ASSERT_ALLOC (self, err_smpr_proto_scpi_alloc);

    self->base = base;

    return self;

    free (self);
err_smpr_proto_scpi_alloc:
    return NULL;
}

/* Destroy an instance of the proto_scpi */
static smpr_err_e smpr_proto_scpi_destroy (smpr_proto_scpi_t **self_p)
{
    assert (self_p);

    if (*self_p) {
        smpr_proto_scpi_t *self = *self_p;

        free (self);
        self_p = NULL;
    }

    return SMPR_SUCCESS;
}

/************ smpr_proto_ops_scpi Implementation **********/

/* Open SCPI protocol */
static int scpi_open (smpr_t *self, uint64_t base, void *args)
{
    UNUSED(args);
    UNUSED(base);
    assert (self);

    /* Create new scpi */
    smpr_proto_scpi_t *scpi_proto = smpr_proto_scpi_new (base);
    ASSERT_TEST(scpi_proto != NULL, "Could not allocate proto_handler",
            err_proto_handler_alloc);

    /* We need to initialize the SCPI protocol here, as we don't have access to
     * to parent pointer inside the smpr_proto_scpi_new () */
    smio_t *parent = smpr_get_parent (self);
    (void) parent;

    /* Attach specific protocol handler to generic one */
    smpr_err_e err = smpr_set_handler (self, scpi_proto);
    ASSERT_TEST(err == SMPR_SUCCESS, "Could not set protocol handler",
            err_proto_handler_set);

    DBE_DEBUG (DBG_SM_PR | DBG_LVL_INFO, "[sm_pr:scpi] Created SCPI client "
            "instance\n");

    return 0;

err_proto_handler_set:
    smpr_proto_scpi_destroy (&scpi_proto);
err_proto_handler_alloc:
    return -1;
}

/* Release SCPI protocol device */
static int scpi_release (smpr_t *self)
{
    assert (self);

    /* Deattach specific protocol handler to generic one */
    smpr_proto_scpi_t *scpi_proto = (smpr_proto_scpi_t *) smpr_unset_handler (self);
    ASSERT_TEST (scpi_proto != NULL, "Could not unset protocol handler",
            err_proto_handler_unset);

    /* Destroy SCPI client instance. Ne need to call any destroy function,
     * as the SCPI library does not dynamically allocates anything */

    /* Deattach specific protocol handler to generic one */
    smpr_err_e err = smpr_proto_scpi_destroy (&scpi_proto);
    ASSERT_TEST (err==SMPR_SUCCESS, "Could not close device appropriately", err_dealloc);
    DBE_DEBUG (DBG_SM_PR | DBG_LVL_INFO, "[sm_pr:scpi] Closed SCPI protocol handler\n");

    return 0;

err_dealloc:
err_proto_handler_unset:
    return -1;
}

static const smpr_proto_ops_t smpr_proto_ops_scpi = {
    .proto_name           = "SCPI",             /* Protocol name */
    .proto_open           = scpi_open,          /* Open device */
    .proto_release        = scpi_release,       /* Release device */
    .proto_read_16        = NULL,               /* Read 16-bit data */
    .proto_read_32        = NULL,               /* Read 32-bit data */
    .proto_read_64        = NULL,               /* Read 64-bit data */
    .proto_write_16       = NULL,               /* Write 16-bit data */
    .proto_write_32       = NULL,               /* Write 32-bit data */
    .proto_write_64       = NULL,               /* Write 64-bit data */
    .proto_read_block     = NULL,               /* Read arbitrary block size data,
                                                    parameter size in bytes */
    .proto_write_block    = NULL,               /* Write arbitrary block size data,
                                                    parameter size in bytes */
    .proto_read_dma       = NULL,               /* Read arbitrary block size data via DMA,
                                                    parameter size in bytes */
    .proto_write_dma      = NULL                /* Write arbitrary block size data via DMA,
                                                    parameter size in bytes */
};

/************ Our methods implementation **********/

/* Creates a new instance of the proto_scpi */
smpr_scpi_t *smpr_scpi_new ()
{
    smpr_scpi_t *self = (smpr_scpi_t *) zmalloc (sizeof *self);
    ASSERT_ALLOC (self, err_smpr_scpi_alloc);

    /* copy SCPI operations */
    self->proto_ops = smpr_proto_ops_scpi;

    return self;

err_smpr_scpi_alloc:
    return NULL;
}

/* Destroy an instance of the scpi */
smpr_err_e smpr_scpi_destroy (smpr_scpi_t **self_p)
{
    assert (self_p);

    if (*self_p) {
        smpr_scpi_t *self = *self_p;

        free (self);
        self_p = NULL;
    }

    return SMPR_SUCCESS;
}

const smpr_proto_ops_t *smpr_scpi_get_ops (smpr_scpi_t *self)
{
    assert (self);
    return &self->proto_ops;
}
