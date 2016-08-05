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
    ASSERT_HAL_TEST(test_boolean, SM_PR, "[sm_pr:i2c_obj]",     \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)   \
    ASSERT_HAL_ALLOC(ptr, SM_PR, "[sm_pr:i2c_obj]",             \
            smpr_err_str(SMPR_ERR_ALLOC),                       \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                \
    CHECK_HAL_ERR(err, SM_PR, "[sm_pr:i2c_obj]",                \
            smpr_err_str (err_type))

/* Protocol object specification */
struct _smpr_i2c_t {
    /* Must be located first */
    smpr_proto_ops_t proto_ops;         /* I2C protocol operations */
    uint32_t rep_start;                 /* I2C repetitive start */
    uint32_t trans_size;                /* I2C transaction size */
    uint32_t addr;                      /* I2C transaction size */
};

/************ Our methods implementation **********/

/* Creates a new instance of the proto_i2c */
smpr_i2c_t *smpr_i2c_new (uint32_t rep_start, uint32_t trans_size, uint32_t addr)
{
    smpr_i2c_t *self = (smpr_i2c_t *) zmalloc (sizeof *self);
    ASSERT_ALLOC (self, err_smpr_i2c_alloc);

    /* copy I2C operations */
    self->proto_ops = smpr_proto_ops_i2c;

    self->rep_start = rep_start;
    self->trans_size = trans_size;
    self->addr = addr;

    return self;

err_smpr_i2c_alloc:
    return NULL;
}

/* Destroy an instance of the i2c */
smpr_err_e smpr_i2c_destroy (smpr_i2c_t **self_p)
{
    assert (self_p);

    if (*self_p) {
        smpr_i2c_t *self = *self_p;

        free (self);
        self_p = NULL;
    }

    return SMPR_SUCCESS;
}

smpr_err_e smpr_i2c_set_rep_start (smpr_i2c_t *self, uint32_t rep_start)
{
    assert (self);
    self->rep_start = rep_start;

    return SMPR_SUCCESS;
}

uint32_t smpr_i2c_get_rep_start (smpr_i2c_t *self)
{
    assert (self);
    return self->rep_start;
}

smpr_err_e smpr_i2c_set_trans_size (smpr_i2c_t *self, uint32_t trans_size)
{
    assert (self);
    self->trans_size = trans_size;

    return SMPR_SUCCESS;
}

uint32_t smpr_i2c_get_trans_size (smpr_i2c_t *self)
{
    assert (self);
    return self->trans_size;
}

smpr_err_e smpr_i2c_set_addr (smpr_i2c_t *self, uint32_t addr)
{
    assert (self);
    self->addr = addr;

    return SMPR_SUCCESS;
}

uint32_t smpr_i2c_get_addr (smpr_i2c_t *self)
{
    assert (self);
    return self->addr;
}

const smpr_proto_ops_t *smpr_i2c_get_ops (smpr_i2c_t *self)
{
    assert (self);
    return &self->proto_ops;
}

