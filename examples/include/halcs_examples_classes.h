/*
 * Copyright (C) 2020 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _HALCS_EXAMPLES_CLASSES_H_
#define _HALCS_EXAMPLES_CLASSES_H_

/* version macros for compile-time API detection */

#define HALCS_EXAMPLES_VERSION_MAJOR 2
#define HALCS_EXAMPLES_VERSION_MINOR 5
#define HALCS_EXAMPLES_VERSION_PATCH 0

#define HALCS_EXAMPLES_MAKE_VERSION(major, minor, patch) \
    ((major) * 10000 + (minor) * 100 + (patch))
#define HALCS_EXAMPLES_VERSION \
    HALCS_EXAMPLES_MAKE_VERSION(HALCS_EXAMPLES_VERSION_MAJOR, HALCS_EXAMPLES_VERSION_MINOR, \
            HALCS_EXAMPLES_VERSION_PATCH)

#if defined (__WINDOWS__)
#   if defined LIBHALCS_EXAMPLES_STATIC
#       define HALCS_EXAMPLES_EXPORT
#   elif defined LIBHALCS_EXAMPLES_EXPORTS
#       define HALCS_EXAMPLES_EXPORT __declspec(dllexport)
#   else
#       define HALCS_EXAMPLES_EXPORT __declspec(dllimport)
#   endif
#else
#   define HALCS_EXAMPLES_EXPORT
#endif

/* Opaque class structures to allow forward references */

/* Public API classes */

#endif
