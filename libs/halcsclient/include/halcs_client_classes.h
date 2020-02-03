/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _HALCS_CLIENT_CLASSES_H_
#define _HALCS_CLIENT_CLASSES_H_

/* Set up environment for the application */
#include "halcs_client_prelude.h"

/* External dependencies */
#include <czmq.h>
#include <malamute.h>
#include <errhand.h>
#include <hutils.h>
#include <disptable.h>

/* Internal libraries dependencies */
#include "acq_chan.h"
#include "sm_io_codes.h"

/* HALCS version macros for compile-time API detection */

#define HALCS_CLIENT_VERSION_MAJOR 1
#define HALCS_CLIENT_VERSION_MINOR 9
#define HALCS_CLIENT_VERSION_PATCH 0

#define HALCS_CLIENT_MAKE_VERSION(major, minor, patch) \
    ((major) * 10000 + (minor) * 100 + (patch))
#define HALCS_CLIENT_VERSION \
    HALCS_CLIENT_MAKE_VERSION(HALCS_CLIENT_VERSION_MAJOR, HALCS_CLIENT_VERSION_MINOR, \
            HALCS_CLIENT_VERSION_PATCH)

#if defined (__WINDOWS__)
#   if defined LIBHALCS_CLIENT_STATIC
#       define HALCS_CLIENT_EXPORT
#   elif defined LIBHALCS_CLIENT_EXPORTS
#       define HALCS_CLIENT_EXPORT __declspec(dllexport)
#   else
#       define HALCS_CLIENT_EXPORT __declspec(dllimport)
#   endif
#else
#   define HALCS_CLIENT_EXPORT
#endif

/* Opaque class structures to allow forward references */

/* Public API classes */

/* Opaque halcs_client_t structure */
typedef struct _halcs_client_t halcs_client_t;

/* HALCS CLIENT */
#include "halcs_client_err.h"
#include "halcs_client_rw_param.h"
#include "halcs_client_core.h"

#endif
