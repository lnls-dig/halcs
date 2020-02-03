/*
 * Copyright (C) 2016 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _SDBUTILS_CLASSES_H_
#define _SDBUTILS_CLASSES_H_

/* External dependencies */
#include <czmq.h>
#include <errhand.h>
#include <libsdbfs.h>

/* version macros for compile-time API detection */

#define SDBUTILS_VERSION_MAJOR 1
#define SDBUTILS_VERSION_MINOR 9
#define SDBUTILS_VERSION_PATCH 0

#define SDBUTILS_MAKE_VERSION(major, minor, patch) \
    ((major) * 10000 + (minor) * 100 + (patch))
#define SDBUTILS_VERSION \
    SDBUTILS_MAKE_VERSION(SDBUTILS_VERSION_MAJOR, SDBUTILS_VERSION_MINOR, \
            SDBUTILS_VERSION_PATCH)

#if defined (__WINDOWS__)
#   if defined LIBSDBUTILS_STATIC
#       define SDBUTILS_EXPORT
#   elif defined LIBSDBUTILS_EXPORTS
#       define SDBUTILS_EXPORT __declspec(dllexport)
#   else
#       define SDBUTILS_EXPORT __declspec(dllimport)
#   endif
#else
#   define SDBUTILS_EXPORT
#endif

/* Known vendor IDs */

#define LNLS_VENDOR_ID      0x1000000000001215

/* Opaque class structures to allow forward references */

/* Public API classes */

/* SDBUTILS */
#include "sdbutils_err.h"
#include "sdbutils_core.h"

#endif
