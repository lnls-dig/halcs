/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _DISPTABLE_CLASSES_H_
#define _DISPTABLE_CLASSES_H_

/* Set up environment for the application */
#include "disptable_prelude.h"

/* External dependencies */
#include <czmq.h>
#include <errhand.h>
#include <hutils.h>

/* Internal dependencies */

/* version macros for compile-time API detection */

#define DISPTABLE_VERSION_MAJOR 2
#define DISPTABLE_VERSION_MINOR 4
#define DISPTABLE_VERSION_PATCH 1

#define DISPTABLE_MAKE_VERSION(major, minor, patch) \
    ((major) * 10000 + (minor) * 100 + (patch))
#define DISPTABLE_VERSION \
    DISPTABLE_MAKE_VERSION(DISPTABLE_VERSION_MAJOR, DISPTABLE_VERSION_MINOR, \
            DISPTABLE_VERSION_PATCH)

#if defined (__WINDOWS__)
#   if defined LIBDISPTABLE_STATIC
#       define DISPTABLE_EXPORT
#   elif defined LIBDISPTABLE_EXPORTS
#       define DISPTABLE_EXPORT __declspec(dllexport)
#   else
#       define DISPTABLE_EXPORT __declspec(dllimport)
#   endif
#else
#   define DISPTABLE_EXPORT
#endif

/* Opaque class structures to allow forward references */

/* Public API classes */

/* DISPTABLE */
#include "disptable_err.h"
#include "disptable_core.h"

#endif
