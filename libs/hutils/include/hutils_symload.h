/*
 * Copyright (C) 2021 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

/*
 * Based on EPICS Base project:
 * modules/libcom/src/osi/epicsFindSymbol.h
 */

#ifndef _HUTILS_SYMLOAD_H_
#define _HUTILS_SYMLOAD_H_

#include "hutils_sharelib.h"

#ifdef __cplusplus
extern "C" {
#endif

#define HUTILS_EXPORT_POBJ(typ_name, obj) pvar_ ## typ_name ## _ ## obj

/** Declare an object for exporting.
 *
 * The HUTILS_EXPORT_SYMBOL() macro must be used to declare any object
 * to be accessible by SMIO modules.
 *
 * A variable tatement must be declared with:
 *   int myDebug = 0;
 *   HUTILS_EXPORT_SYMBOL(int, myDebug);
 *
 * C++ code needs to wrap with extern "C" { }
 */
#define HUTILS_EXPORT_SYMBOL(typ, typ_name, obj) \
    HUTILS_SHARE_EXTERN typ *HUTILS_EXPORT_POBJ(typ_name, obj); \
    typ *HUTILS_EXPORT_POBJ(typ_name, obj) = (typ *) (char *) &obj

void *hutils_load_lib (const char *name);
const char *hutils_load_error (void);
void *hutils_find_symbol (const char *name);
void *hutils_lookup_symbol (const char* a, const char* b, const char* c);

#ifdef __cplusplus
}
#endif

#endif
