/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _HUTILS_CLASSES_H_
#define _HUTILS_CLASSES_H_

/* Set up environment for the application */
#include "hutils_prelude.h"

/* External dependencies */
#include <czmq.h>
#include <errhand.h>

/* version macros for compile-time API detection */

#define HUTILS_VERSION_MAJOR 2
#define HUTILS_VERSION_MINOR 5
#define HUTILS_VERSION_PATCH 1

#define HUTILS_MAKE_VERSION(major, minor, patch) \
    ((major) * 10000 + (minor) * 100 + (patch))
#define HUTILS_VERSION \
    HUTILS_MAKE_VERSION(HUTILS_VERSION_MAJOR, HUTILS_VERSION_MINOR, \
            HUTILS_VERSION_PATCH)

#if defined (__WINDOWS__)
#   if defined LIBHUTILS_STATIC
#       define HUTILS_EXPORT
#   elif defined LIBHUTILS_EXPORTS
#       define HUTILS_EXPORT __declspec(dllexport)
#   else
#       define HUTILS_EXPORT __declspec(dllimport)
#   endif
#else
#   define HUTILS_EXPORT
#endif

/* Opaque class structures to allow forward references */

/* Public API classes */

/* HUTILS */
#include "hutils_err.h"
#include "hutils_core.h"
#include "hutils_math.h"
#include "hutils_utils.h"
#include "hutils_symload.h"
#include "hutils_sharelib.h"

#endif
