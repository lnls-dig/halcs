/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _ERRHAND_CLASSES_H_
#define _ERRHAND_CLASSES_H_

/* Set up environment for the application */
#include "errhand_prelude.h"

/* External dependencies */
#include <czmq.h>

/* Internal dependencies */
#include "varg_macros.h"

/* version macros for compile-time API detection */

#define ERRHAND_VERSION_MAJOR 0
#define ERRHAND_VERSION_MINOR 5
#define ERRHAND_VERSION_PATCH 1

#define ERRHAND_MAKE_VERSION(major, minor, patch) \
    ((major) * 10000 + (minor) * 100 + (patch))
#define ERRHAND_VERSION \
    ERRHAND_MAKE_VERSION(ERRHAND_VERSION_MAJOR, ERRHAND_VERSION_MINOR, \
            ERRHAND_VERSION_PATCH)

#if defined (__WINDOWS__)
#   if defined LIBERRHAND_STATIC
#       define ERRHAND_EXPORT
#   elif defined LIBERRHAND_EXPORTS
#       define ERRHAND_EXPORT __declspec(dllexport)
#   else
#       define ERRHAND_EXPORT __declspec(dllimport)
#   endif
#else
#   define ERRHAND_EXPORT
#endif

/* Opaque class structures to allow forward references */

/* Public API classes */

/* ERRHAND */
#include "errhand_assert.h"
#include "errhand_subsys.h"       /* This must come before "errhand_opts.h" */
#include "errhand_opts.h"
#include "errhand_print.h"
#include "errhand_local_print.h"

#endif
