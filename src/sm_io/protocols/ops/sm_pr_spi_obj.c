/*
 * Copyright (C) 2016 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "bpm_server.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, SM_PR, "[sm_pr:spi_obj]",     \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)   \
    ASSERT_HAL_ALLOC(ptr, SM_PR, "[sm_pr:spi_obj]",             \
            smpr_err_str(SMPR_ERR_ALLOC),                       \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                \
    CHECK_HAL_ERR(err, SM_PR, "[sm_pr:spi_obj]",                \
            smpr_err_str (err_type))

/* Protocol object specification */
struct _smpr_spi_t {
    /* Must be located first */
    smpr_proto_ops_t proto_ops;       /* SPI protocol operations */
    uint32_t ss;                      /* SPI Slave Select */
    uint32_t charlen;                 /* SPI character length:
                                         0 is 128-bit data word, 1 is 1 bit, 2 is 2-bit and so on */
};

/************ Our methods implementation **********/

/* Creates a new instance of the proto_spi */
smpr_spi_t *smpr_spi_new (uint32_t ss, uint32_t charlen)
{
    smpr_spi_t *self = (smpr_spi_t *) zmalloc (sizeof *self);
    ASSERT_ALLOC (self, err_smpr_spi_alloc);

    /* copy SPI operations */
    self->proto_ops = smpr_proto_ops_spi;

    self->ss = ss;
    self->charlen = charlen;

    return self;

err_smpr_spi_alloc:
    return NULL;
}

/* Destroy an instance of the spi */
smpr_err_e smpr_spi_destroy (smpr_spi_t **self_p)
{
    assert (self_p);

    if (*self_p) {
        smpr_spi_t *self = *self_p;

        free (self);
        self_p = NULL;
    }

    return SMPR_SUCCESS;
}

smpr_err_e smpr_spi_set_ss (smpr_spi_t *self, uint32_t ss)
{
    assert (self);
    self->ss = ss;

    return SMPR_SUCCESS;
}

uint32_t smpr_spi_get_ss (smpr_spi_t *self)
{
    assert (self);
    return self->ss;
}

smpr_err_e smpr_spi_set_charlen (smpr_spi_t *self, uint32_t charlen)
{
    assert (self);
    self->charlen = charlen;

    return SMPR_SUCCESS;
}

uint32_t smpr_spi_get_charlen (smpr_spi_t *self)
{
    assert (self);
    return self->charlen;
}

const smpr_proto_ops_t *smpr_spi_get_ops (smpr_spi_t *self)
{
    assert (self);
    return &self->proto_ops;
}
