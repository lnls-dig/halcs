/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _CONVC_CLASSES_H_
#define _CONVC_CLASSES_H_

/* Set up environment for the application */
#include "convc_prelude.h"

/* External dependencies */
#include <czmq.h>
#include <errhand.h>

/* version macros for compile-time API detection */

#define CONVC_VERSION_MAJOR 2
#define CONVC_VERSION_MINOR 1
#define CONVC_VERSION_PATCH 0

#define CONVC_MAKE_VERSION(major, minor, patch) \
    ((major) * 10000 + (minor) * 100 + (patch))
#define CONVC_VERSION \
    CONVC_MAKE_VERSION(CONVC_VERSION_MAJOR, CONVC_VERSION_MINOR, \
            CONVC_VERSION_PATCH)

#if defined (__WINDOWS__)
#   if defined LIBCONVC_STATIC
#       define CONVC_EXPORT
#   elif defined LIBCONVC_EXPORTS
#       define CONVC_EXPORT __declspec(dllexport)
#   else
#       define CONVC_EXPORT __declspec(dllimport)
#   endif
#else
#   define CONVC_EXPORT
#endif

/* Opaque class structures to allow forward references */

/* Public API classes */

/* CONVC */
#include "convc_err.h"
#include "convc_core.h"

#endif
