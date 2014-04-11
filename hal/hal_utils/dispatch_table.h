/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

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
typedef void* (*disp_table_func_fp)(void *owner, void * args);

struct _func_fp_wrapper_t {
    disp_table_func_fp func_fp;
};

/* Opaque class structure */
typedef struct _func_fp_wrapper_t func_fp_wrapper_t;

/***************** Our methods *****************/

disp_table_t *disp_table_new (void);
halutils_err_e disp_table_destroy (disp_table_t **self_p);
halutils_err_e disp_table_insert (disp_table_t *self, uint32_t key,
    disp_table_func_fp func_fp);
halutils_err_e disp_table_insert_all (disp_table_t *self, const disp_table_func_fp *disp_table_fp,
    const uint32_t *code_table, uint32_t size);
halutils_err_e disp_table_remove (disp_table_t *self, uint32_t key);
halutils_err_e disp_table_remove_all (disp_table_t *self);
void *disp_table_call (disp_table_t *self, uint32_t key, void *owner, void *args);

#endif

