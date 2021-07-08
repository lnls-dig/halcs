/*
 * Copyright (C) 2021 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

/*
 * Based on EPICS Base project:
 * modules/libcom/src/osi/os/posix/osdFindSymbol.c
 */

#include "hutils.h"
#include <dlfcn.h>

/* Undef ASSERT_ALLOC to avoid conflicting with other ASSERT_ALLOC */
#ifdef ASSERT_TEST
#undef ASSERT_TEST
#endif
#define ASSERT_TEST(test_boolean, err_str, err_goto_label, /* err_core */ ...) \
    ASSERT_HAL_TEST(test_boolean, HAL_UTILS, "[hutils:symload]",               \
            err_str, err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef ASSERT_ALLOC
#undef ASSERT_ALLOC
#endif
#define ASSERT_ALLOC(ptr, err_goto_label, /* err_core */ ...)           \
    ASSERT_HAL_ALLOC(ptr, HAL_UTILS, "[hutils:symload]",                \
            hutils_err_str(HUTILS_ERR_ALLOC),                           \
            err_goto_label, /* err_core */ __VA_ARGS__)

#ifdef CHECK_ERR
#undef CHECK_ERR
#endif
#define CHECK_ERR(err, err_type)                                        \
    CHECK_HAL_ERR(err, HAL_UTILS, "[hutils:symload]",                   \
            hutils_err_str (err_type))

/*
 * non-POSIX extension available on Linux (glibc at least) and OSX.
 */
#ifndef RTLD_DEFAULT
#  define RTLD_DEFAULT 0
#endif

void *hutils_load_library (const char *name)
{
    return dlopen (name, RTLD_LAZY | RTLD_GLOBAL);
}

const char *hutils_load_error (void)
{
    return dlerror ();
}

void *hutils_find_symbol (const char *name)
{
    return dlsym (RTLD_DEFAULT, name);
}

void *hutils_lookup_symbol (const char* a, const char* b, const char* c)
{
    void *symbol = NULL;
    char *name = hutils_concat_strings3_no_sep (a, b, c);
    ASSERT_ALLOC(name, err_alloc_name);

    symbol = hutils_find_symbol (name);
    if (!symbol) {
        DBE_DEBUG (DBG_HAL_UTILS | DBG_LVL_ERR,
                "[hutils:hutils_lookup_symbol] Unable to find symbol \"%s\": %s\n",
                name, hutils_load_error());
        goto err_find_symbol;
    /* all exported symbols are pointers to the exported object */
    } 

err_find_symbol:
    free (name);
err_alloc_name:
    return symbol;
}
