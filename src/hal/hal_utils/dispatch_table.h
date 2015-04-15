/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

/* This is roughly based on the work of Alessandro Rubini and others
 * "mini-rpc", located on "https://github.com/a-rubini/mini-rpc"
 *
 * In the end, I changed much of the original code and only the main
 * idea with a couple of the original structures remain */

#ifndef _DISPATCH_TABLE_H_
#define _DISPATCH_TABLE_H_

#include <czmq.h>
#include "hal_utils_err.h"

struct _disp_table_t {
    /* Hash containg all the sm_io thsafe operations
     * that we need to handle. It is composed
     * of key (4-char ID) / value (pointer to funtion) */
    zhash_t *table_h;
};

/* Opaque class structure */
typedef struct _disp_table_t disp_table_t;

/* Generic function pointer */
typedef int (*disp_table_func_fp)(void *owner, void * args, void *ret);

/* Argument type (and retval type). The size is encoded in the same word */
enum _disp_at_e {
    DISP_ATYPE_ERROR = 0xFF,
    DISP_ATYPE_NONE  = 0,           /* used as terminator */
    DISP_ATYPE_INT16 = 1,
    DISP_ATYPE_UINT16,
    DISP_ATYPE_INT32,
    DISP_ATYPE_UINT32,
    DISP_ATYPE_INT64,
    DISP_ATYPE_UINT64,
    DISP_ATYPE_DOUBLE,              /* float is promoted to double */
    DISP_ATYPE_STRING,              /* size of strings is strlen() each time */
    DISP_ATYPE_STRUCT,
    DISP_ATYPE_VAR                  /* variable argument size. Allowed to be
                                       up to the size of the encoded argument.
                                       Typically a structure */
};

typedef enum _disp_at_e disp_at_e;

/* FIXME: large arguments can overflow the macro!
 * Encoding of argument type and size in one word */
#define __DISP_ARG_ENCODE(atype, asize) (((atype) << 24) | (asize))
#define DISP_ARG_ENCODE(atype, type) __DISP_ARG_ENCODE(atype, sizeof(type))
#define DISP_GET_ATYPE(word) ((word) >> 24)
#define DISP_GET_ASIZE(word) ((word) & 0xFFFFFF)
#define DISP_ARG_END __DISP_ARG_ENCODE(DISP_ATYPE_NONE, 0) /* zero */

enum _disp_val_owner_e {
    DISP_OWNER_FUNC = 0,            /* Value is owned by the function itself*/
    DISP_OWNER_OTHER                /* Value is owned by someone else and
                                           must take care of allocations, if
                                           necessary */
};

struct _disp_op_t {
    const char *name;                       /* Function name */
    uint32_t opcode;                        /* Operation code */
    disp_table_func_fp func_fp;             /* Pointer to exported function */
    uint32_t retval;                        /* Type of return value */
    enum _disp_val_owner_e retval_owner;    /* Who owns the return value */
    uint32_t args [];                       /* Zero-terminated */
};

struct _disp_op_handler_t {
    const struct _disp_op_t *op;            /* Function description */
    void *ret;                              /* Buffer for function return value */
};

/* Return buffer owner */
typedef enum _disp_val_owner_e disp_val_owner_e;
/* Dispatch exported interface function structure */
typedef struct _disp_op_t disp_op_t;
/* Handler for the Dispatch exported function */
typedef struct _disp_op_handler_t disp_op_handler_t;

/***************** Our methods *****************/

disp_table_t *disp_table_new (void);
halutils_err_e disp_table_destroy (disp_table_t **self_p);

halutils_err_e disp_table_insert (disp_table_t *self, const disp_op_t *disp_op);
halutils_err_e disp_table_insert_all (disp_table_t *self,
        const disp_op_t **disp_ops);
halutils_err_e disp_table_remove (disp_table_t *self, uint32_t key);
halutils_err_e disp_table_remove_all (disp_table_t *self);

halutils_err_e disp_table_fill_desc (disp_table_t *self, disp_op_t **disp_ops,
        const disp_table_func_fp *func_fps);

halutils_err_e disp_table_check_args (disp_table_t *self, uint32_t key,
        void *args, void **ret);
halutils_err_e disp_table_cleanup_args (disp_table_t *self, uint32_t key);
const disp_op_t *disp_table_lookup (disp_table_t *self, uint32_t key);
int disp_table_call (disp_table_t *self, uint32_t key, void *owner, void *args,
        void *ret);
int disp_table_check_call (disp_table_t *self, uint32_t key, void *owner,
        void *args, void **ret);
halutils_err_e disp_table_set_ret (disp_table_t *self, uint32_t key, void **ret);

/*********************** Disp Op Handler functions ****************************/

disp_op_handler_t *disp_op_handler_new (void);
halutils_err_e disp_op_handler_destroy (disp_op_handler_t **self_p);

#endif

