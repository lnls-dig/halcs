/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "bpm_server.h"

/* Private headers */
#include "sm_pr_bsmp_defaults.h"

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, SM_PR, "[sm_pr:bsmp]",            \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)       \
    ASSERT_HAL_ALLOC(ptr, SM_PR, "[sm_pr:bsmp]",                    \
            smpr_err_str(SMPR_ERR_ALLOC),                           \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                    \
    CHECK_HAL_ERR(err, SM_PR, "[sm_pr:bsmp]",                       \
            smpr_err_str (err_type))

#define SMPR_PROTO_BSMP_CLIENT(smpr_handler)         (smpr_handler->client)

/* BSMP glue structure. Needed to overcome the need of global variables */
typedef struct {
    smio_t *parent;
} smpr_proto_glue_bsmp_t;

/* BSMP glue structure */
smpr_proto_glue_bsmp_t bsmp_glue;

/* Device endpoint */
typedef struct {
    uint64_t base;                                      /* Core base address */
    bsmp_client_t *client;                              /* BSMP client instance */
    struct bsmp_func_info_list *funcs_list;             /* BSMP function handler */
    struct bsmp_var_info_list *vars_list;               /* BSMP variables handler */
    struct bsmp_curve_info_list *curves_list;           /* BSMP curves handler */
    /* Unused */
    struct bsmp_group_list *groups_list;                /* BSMP groups handler */
} smpr_proto_bsmp_t;

static smpr_err_e _smpr_proto_bsmp_get_handlers (smpr_t *self);
static int _smpr_proto_bsmp_send (uint8_t *data, uint32_t *count);
static int _smpr_proto_bsmp_recv (uint8_t *data, uint32_t *count);

/*************** Our methods implementation **********/

/* Creates a new instance of the proto_bsmp */
static smpr_proto_bsmp_t * smpr_proto_bsmp_new (uint64_t base)
{
    smpr_proto_bsmp_t *self = (smpr_proto_bsmp_t *) zmalloc (sizeof *self);
    ASSERT_ALLOC (self, err_smpr_proto_bsmp_alloc);

    self->client = (bsmp_client_t *) zmalloc (sizeof *self->client);
    ASSERT_ALLOC (self->client, err_smpr_proto_client_alloc);

    self->base = base;

    return self;

err_smpr_proto_client_alloc:
    free (self);
err_smpr_proto_bsmp_alloc:
    return NULL;
}

/* Destroy an instance of the proto_bsmp */
static smpr_err_e smpr_proto_bsmp_destroy (smpr_proto_bsmp_t **self_p)
{
    assert (self_p);

    if (*self_p) {
        smpr_proto_bsmp_t *self = *self_p;

        free (self->client);
        free (self);
        self_p = NULL;
    }

    return SMPR_SUCCESS;
}

/************ smpr_proto_ops_bsmp Implementation **********/

/* Open BSMP protocol */
int bsmp_open (smpr_t *self, uint64_t base, void *args)
{
    (void) args;
    assert (self);

    /* Create new bsmp */
    smpr_proto_bsmp_t *bsmp_proto = smpr_proto_bsmp_new (base);
    ASSERT_TEST(bsmp_proto != NULL, "Could not allocate proto_handler",
            err_proto_handler_alloc);

    /* We need to initialize the BASMP protocol here, as we don't have access to
     * to parent pointer inside the smpr_proto_bsmp_new () */
    smio_t *parent = smpr_get_parent (self);
    /* Initialize global glue BSMP variable for usage with the send and recv
     * functions */
    bsmp_glue.parent = parent;

    /* Attach specific protocol handler to generic one */
    smpr_err_e err = smpr_set_handler (self, bsmp_proto);
    ASSERT_TEST(err == SMPR_SUCCESS, "Could not set protocol handler",
            err_proto_handler_set);

    /* Finish initializing BSMP protocol here */
    enum bsmp_err berr = bsmp_client_init (bsmp_proto->client, _smpr_proto_bsmp_send,
            _smpr_proto_bsmp_recv);
    if (berr != BSMP_SUCCESS) {
        DBE_DEBUG (DBG_SM_PR | DBG_LVL_ERR, "[sm_pr:bsmp] Could not initialize "
                "BSMP protocol handler: %s\n", bsmp_error_str (berr));
            goto err_proto_handler_init;
    }

    err = _smpr_proto_bsmp_get_handlers (self);
    ASSERT_TEST (err == SMPR_SUCCESS, "Could not get BSMP handlers",
            err_bsmp_handler_init);

    DBE_DEBUG (DBG_SM_PR | DBG_LVL_INFO, "[sm_pr:bsmp] Created BSMP client "
            "instance\n");

    return 0;

err_bsmp_handler_init:
    /* There is no bsmp_destroy, as it has nothing allocated dynamically */
err_proto_handler_init:
    smpr_unset_handler (self);
err_proto_handler_set:
    smpr_proto_bsmp_destroy (&bsmp_proto);
err_proto_handler_alloc:
    return -1;
}

/* Release BSMP protocol device */
int bsmp_release (smpr_t *self)
{
    assert (self);

    /* Deattach specific protocol handler to generic one */
    smpr_proto_bsmp_t *bsmp_proto = (smpr_proto_bsmp_t *) smpr_unset_handler (self);
    ASSERT_TEST (bsmp_proto != NULL, "Could not unset protocol handler",
            err_proto_handler_unset);

    /* Destroy BSMP client instance. Ne need to call any destroy function,
     * as the BSMP library does not dynamically allocates anything */

    /* Null the parent SMIO pointer */
    bsmp_glue.parent = NULL;

    /* Deattach specific protocol handler to generic one */
    smpr_err_e err = smpr_proto_bsmp_destroy (&bsmp_proto);
    ASSERT_TEST (err==SMPR_SUCCESS, "Could not close device appropriately", err_dealloc);
    DBE_DEBUG (DBG_SM_PR | DBG_LVL_INFO, "[sm_pr:bsmp] Closed BSMP protocol handler\n");

    return 0;

err_dealloc:
err_proto_handler_unset:
    return -1;
}

/* The following functions are not suitable to our SMPR operations structure, but
 * it does makes sense to be here, in our protocol handling "class" */

/* Read/Write to RFFE vars by ID */
smpr_err_e smpr_bsmp_read_var_by_id (smpr_t *self, uint32_t id, uint8_t *data,
        size_t size)
{
    assert (self);
    assert (data);

    smpr_err_e err = SMPR_SUCCESS;
    smpr_proto_bsmp_t *bsmp_proto = smpr_get_handler (self);
    ASSERT_TEST(bsmp_proto != NULL, "Could not get SMPR protocol handler",
            err_proto_handler, SMPR_ERR_PROTO_INFO);
    bsmp_client_t *bsmp_client = SMPR_PROTO_BSMP_CLIENT(bsmp_proto);

    /* Check if the ID is valid */
    ASSERT_TEST(id < bsmp_proto->vars_list->count, "Invalid BSMP variable ID",
            err_inv_id, SMPR_ERR_INV_FUNC_PARAM);

    /* Get associated variable info */
    struct bsmp_var_info *var_info = &bsmp_proto->vars_list->list[id];

    /* Check if the variable fits in out output buffer */
    ASSERT_TEST(var_info->size <= size, "Variable size is too small for BSMP "
            "variable", err_size_too_small, SMPR_ERR_INV_FUNC_PARAM);

    enum bsmp_err berr = bsmp_read_var (bsmp_client, var_info, data);
    ASSERT_TEST(berr == BSMP_SUCCESS, "Could not read BSMP variable",
            err_bsmp, SMPR_ERR_RW_SMIO /* FIXME: return better error? */ );

err_bsmp:
err_size_too_small:
err_inv_id:
err_proto_handler:
    return err;
}

smpr_err_e smpr_bsmp_write_var_by_id (smpr_t *self, uint32_t id, uint8_t *data,
        size_t size)
{
    assert (self);
    assert (data);

    smpr_err_e err = SMPR_SUCCESS;
    smpr_proto_bsmp_t *bsmp_proto = smpr_get_handler (self);
    ASSERT_TEST(bsmp_proto != NULL, "Could not get SMPR protocol handler",
            err_proto_handler, SMPR_ERR_PROTO_INFO);
    bsmp_client_t *bsmp_client = SMPR_PROTO_BSMP_CLIENT(bsmp_proto);

    /* Check if the ID is valid */
    ASSERT_TEST(id < bsmp_proto->vars_list->count, "Invalid BSMP variable ID",
            err_inv_id, SMPR_ERR_INV_FUNC_PARAM);

    /* Get associated variable info */
    struct bsmp_var_info *var_info = &bsmp_proto->vars_list->list[id];

    /* Check if the variable size matches the one on the descriptor */
    ASSERT_TEST(var_info->size <= size, "Variable size is too small for BSMP "
            "variable", err_size_differs, SMPR_ERR_INV_FUNC_PARAM);

    enum bsmp_err berr = bsmp_write_var (bsmp_client, var_info, data);
    ASSERT_TEST(berr == BSMP_SUCCESS, "Could not write BSMP variable",
            err_bsmp, SMPR_ERR_RW_SMIO /* FIXME: return better error? */ );

err_bsmp:
err_size_differs:
err_inv_id:
err_proto_handler:
    return err;
}

/* Call RFFE functions by ID */
smpr_err_e smpr_bsmp_func_exec_by_id (smpr_t *self, uint32_t id, uint8_t *write_data,
        size_t write_size, uint8_t *read_data, size_t read_size)
{
    assert (self);
    assert (write_data);
    assert (read_data);

    smpr_err_e err = SMPR_SUCCESS;
    smpr_proto_bsmp_t *bsmp_proto = smpr_get_handler (self);
    ASSERT_TEST(bsmp_proto != NULL, "Could not get SMPR protocol handler",
            err_proto_handler, SMPR_ERR_PROTO_INFO);
    bsmp_client_t *bsmp_client = SMPR_PROTO_BSMP_CLIENT(bsmp_proto);

    /* Check if the ID is valid */
    ASSERT_TEST(id < bsmp_proto->funcs_list->count, "Invalid BSMP function ID",
            err_inv_id, SMPR_ERR_INV_FUNC_PARAM);

    /* Get associated function info */
    struct bsmp_func_info *func_info = &bsmp_proto->funcs_list->list[id];

    /* Check if the functions input and output  size matches the descriptor */
    ASSERT_TEST(func_info->input_size == write_size,
            "Write data does not match the BSMP function size",
            err_in_size_differs, SMPR_ERR_INV_FUNC_PARAM);
    ASSERT_TEST(func_info->output_size == read_size,
            "Read data does not match the BSMP function size",
            err_out_size_differs, SMPR_ERR_INV_FUNC_PARAM);

    uint8_t func_err = 0;
    enum bsmp_err berr = bsmp_func_execute (bsmp_client, func_info, &func_err, write_data,
            read_data);
    ASSERT_TEST(berr == BSMP_SUCCESS, "Could not write BSMP variable",
            err_bsmp, SMPR_ERR_RW_SMIO /* FIXME: return better error? */ );

err_bsmp:
err_in_size_differs:
err_out_size_differs:
err_inv_id:
err_proto_handler:
    return err;
}

/* Read RFFE curves by ID */
smpr_err_e smpr_bsmp_read_curve_by_id (smpr_t *self, uint32_t id, uint8_t *read_data,
        size_t size, size_t *valid_bytes)
{
    assert (self);
    assert (read_data);

    smpr_err_e err = SMPR_SUCCESS;
    smpr_proto_bsmp_t *bsmp_proto = smpr_get_handler (self);
    ASSERT_TEST(bsmp_proto != NULL, "Could not get SMPR protocol handler",
            err_proto_handler, SMPR_ERR_PROTO_INFO);
    bsmp_client_t *bsmp_client = SMPR_PROTO_BSMP_CLIENT(bsmp_proto);

    /* Check if the ID is valid */
    ASSERT_TEST(id < bsmp_proto->curves_list->count, "Invalid BSMP curve ID",
            err_inv_id, SMPR_ERR_INV_FUNC_PARAM);

    /* Get associated function info */
    struct bsmp_curve_info *curve_info = &bsmp_proto->curves_list->list[id];

    /* Check if the curve size matches the one on the descriptor */
    ASSERT_TEST(curve_info->nblocks*curve_info->block_size <= size,
            "Curve buffer size is too small",
            err_curve_size_small, SMPR_ERR_INV_FUNC_PARAM);

    uint32_t valid_bytes_read = 0;
    enum bsmp_err berr = bsmp_read_curve (bsmp_client, curve_info, read_data,
            &valid_bytes_read);
    ASSERT_TEST(berr == BSMP_SUCCESS, "Could not read BSMP curve",
            err_bsmp, SMPR_ERR_RW_SMIO /* FIXME: return better error? */ );

    /* Tell user how many were actually read */
    if (valid_bytes != NULL) {
        *valid_bytes = valid_bytes_read;
    }

err_bsmp:
err_curve_size_small:
err_inv_id:
err_proto_handler:
    return err;
}

/*************************** Helper functions *********************************/

/* Get all BSMP handlers */
static smpr_err_e _smpr_proto_bsmp_get_handlers (smpr_t *self)
{
    smpr_err_e err = SMPR_SUCCESS;
    smpr_proto_bsmp_t *bsmp_proto = smpr_get_handler (self);
    ASSERT_TEST(bsmp_proto != NULL, "Could not get SMPR protocol handler",
            err_proto_handler, SMPR_ERR_PROTO_INFO);

    /* Get all BSMP entities */
    enum bsmp_err berr = bsmp_get_funcs_list (bsmp_proto->client, &bsmp_proto->funcs_list);
    ASSERT_TEST(berr==BSMP_SUCCESS, "Could not retrieve list of functions",
            err_ret_funcs, SMPR_ERR_PROTO_INFO);

    /* Print list of functions */
    DBE_DEBUG (DBG_SM_PR | DBG_LVL_INFO, "[sm_pr:bsmp] BSMP server has %d Functions(s):\n",
            bsmp_proto->funcs_list->count);
    uint32_t i;
    for(i = 0; i < bsmp_proto->funcs_list->count; ++i) {
        DBE_DEBUG (DBG_SM_PR | DBG_LVL_INFO,
                "[sm_pr:bsmp] ID[%d] INPUT[%2d bytes] OUTPUT[%2d bytes]\n",
                bsmp_proto->funcs_list->list[i].id,
                bsmp_proto->funcs_list->list[i].input_size,
                bsmp_proto->funcs_list->list[i].output_size);
    }

    berr = bsmp_get_vars_list (bsmp_proto->client, &bsmp_proto->vars_list);
    ASSERT_TEST(berr==BSMP_SUCCESS, "Could not retrieve list of variables",
            err_ret_vars, SMPR_ERR_PROTO_INFO);

    /* Print list of variables */
    DBE_DEBUG (DBG_SM_PR | DBG_LVL_INFO, "[sm_pr:bsmp] BSMP server has %d Variable(s):\n",
            bsmp_proto->vars_list->count);
    for(i = 0; i < bsmp_proto->vars_list->count; ++i) {
        DBE_DEBUG (DBG_SM_PR | DBG_LVL_INFO,
                "[sm_pr:bsmp] ID[%d] SIZE[%2d] %s\n",
                bsmp_proto->vars_list->list[i].id,
                bsmp_proto->vars_list->list[i].size,
                bsmp_proto->vars_list->list[i].writable ? "WRITABLE" : "READ-ONLY");
    }

    berr = bsmp_get_curves_list (bsmp_proto->client, &bsmp_proto->curves_list);
    ASSERT_TEST(berr==BSMP_SUCCESS, "Could not retrieve list of curves",
            err_ret_curves, SMPR_ERR_PROTO_INFO);

    /* Print list of curves */
    DBE_DEBUG (DBG_SM_PR | DBG_LVL_INFO, "[sm_pr:bsmp] BSMP server has %d Curve(s):\n",
            bsmp_proto->curves_list->count);
    for(i = 0; i < bsmp_proto->curves_list->count; ++i) {
        DBE_DEBUG (DBG_SM_PR | DBG_LVL_INFO,
                "[sm_pr:bsmp] ID[%d] BLOCKS[%3d (%5d bytes each)] %s\n",
                bsmp_proto->curves_list->list[i].id,
                bsmp_proto->curves_list->list[i].nblocks,
                bsmp_proto->curves_list->list[i].block_size,
                bsmp_proto->curves_list->list[i].writable ? "WRITABLE" : "READ-ONLY");
    }

    berr = bsmp_get_groups_list (bsmp_proto->client, &bsmp_proto->groups_list);
    ASSERT_TEST(berr==BSMP_SUCCESS, "Could not retrieve list of groups",
            err_ret_groups, SMPR_ERR_PROTO_INFO);

    /* Print list of groups. Only some proprieties, not the variables that compose them */
    DBE_DEBUG (DBG_SM_PR | DBG_LVL_INFO, "[sm_pr:bsmp] BSMP server has %d Group(s):\n",
            bsmp_proto->groups_list->count);
    for(i = 0; i < bsmp_proto->groups_list->count; ++i) {
        DBE_DEBUG (DBG_SM_PR | DBG_LVL_INFO,
                "[sm_pr:bsmp] ID[%d] SIZE[%2d] %s\n",
                bsmp_proto->groups_list->list[i].id,
                bsmp_proto->groups_list->list[i].size,
                bsmp_proto->groups_list->list[i].writable ? "WRITABLE" : "READ-ONLY");
    }

err_ret_funcs:
err_ret_vars:
err_ret_curves:
err_ret_groups:
err_proto_handler:
    return err;
}

/*************************** Static functions *********************************/
/* BSMP function I/O */
static int _smpr_proto_bsmp_send (uint8_t *data, uint32_t *count)
{
    int err = 0;

    DBE_DEBUG (DBG_LL_IO | DBG_LVL_TRACE, "[sm_pr:bsmp] Sending %u bytes\n", *count);
    ssize_t ret = smio_thsafe_client_write_block (bsmp_glue.parent, 0, *count,
            (uint32_t *) data);
    DBE_DEBUG (DBG_LL_IO | DBG_LVL_TRACE, "[sm_pr:bsmp] Sent %u bytes\n", *count);

    if (ret < 0) {
        *count = 0;
        err = -1;
        goto err_packet_send;
    }

    /* Update count with the number of bytes received */
    *count = ret;

err_packet_send:
    return err;
}

static int _smpr_proto_bsmp_recv (uint8_t *data, uint32_t *count)
{
    uint32_t len = BSMP_HEADER_SIZE;
    int err = 0;

    /* First we must received the packet header */
    DBE_DEBUG (DBG_LL_IO | DBG_LVL_TRACE, "[sm_pr:bsmp] Receiving %u bytes\n", len);
    ssize_t ret = smio_thsafe_client_read_block (bsmp_glue.parent, 0, len,
            (uint32_t *) data);
    DBE_DEBUG (DBG_LL_IO | DBG_LVL_TRACE, "[sm_pr:bsmp] Received %ld bytes\n", ret);

    if (ret < 0) {
        *count = 0;
        err = -1;
        goto err_packet_header;
    }

    *count = ret;

    /* Now, we parse the protocol header and determine how many bytes
     * we have left to read. Very protocol dependent. See BSMP documentation.
     * and example located in
     * https://github.com/lerwys/libbsmp/blob/master/examples/client/puc_serial/main.c
     * FIXME */
    len = (data [1] << 8) + data [2];

    DBE_DEBUG (DBG_LL_IO | DBG_LVL_TRACE, "[sm_pr:bsmp] Receiving another %u "
            "bytes\n", len);
    ret = smio_thsafe_client_read_block (bsmp_glue.parent, 0, len,
            (uint32_t *)(data + *count));
    DBE_DEBUG (DBG_LL_IO | DBG_LVL_TRACE, "[sm_pr:bsmp] Received another %ld "
            "bytes\n", ret);

    if (ret < 0) {
        *count = 0;
        err = -1;
        goto err_packet_payload;
    }

    *count += ret;

err_packet_payload:
err_packet_header:
    return err;
}

const smpr_proto_ops_t smpr_proto_ops_bsmp = {
    .proto_open           = bsmp_open,          /* Open device */
    .proto_release        = bsmp_release,       /* Release device */
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
