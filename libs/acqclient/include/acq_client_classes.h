/*
 * Copyright (C) 2017 LNLS (www.lnls.br)
 * Authors: Lucas Russo <lucas.russo@lnls.br>
 *          Janito Vaqueiro Ferreira Filho <janito.filho@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _ACQ_CLIENT_CLASSES_H_
#define _ACQ_CLIENT_CLASSES_H_

/* Set up environment for the application */
#include "acq_client_prelude.h"

/* External dependencies */
#include <malamute.h>
#include <errhand.h>
#include <hutils.h>
#include <disptable.h>
#include <halcs_client.h>

/* Internal libraries dependencies */
#include "acq_chan.h"

/* version macros for compile-time API detection */

#define ACQ_CLIENT_VERSION_MAJOR 0
#define ACQ_CLIENT_VERSION_MINOR 5
#define ACQ_CLIENT_VERSION_PATCH 1

#define ACQ_CLIENT_MAKE_VERSION(major, minor, patch) \
    ((major) * 10000 + (minor) * 100 + (patch))
#define ACQ_CLIENT_VERSION \
    ACQ_CLIENT_MAKE_VERSION(ACQ_CLIENT_VERSION_MAJOR, ACQ_CLIENT_VERSION_MINOR, \
            ACQ_CLIENT_VERSION_PATCH)

#if defined (__WINDOWS__)
#   if defined LIBACQ_CLIENT_STATIC
#       define ACQ_CLIENT_EXPORT
#   elif defined LIBACQ_CLIENT_EXPORTS
#       define ACQ_CLIENT_EXPORT __declspec(dllexport)
#   else
#       define ACQ_CLIENT_EXPORT __declspec(dllimport)
#   endif
#else
#   define ACQ_CLIENT_EXPORT
#endif

/* Opaque acq_client_t structure */
typedef struct _acq_client_t acq_client_t;

/* ACQ CLIENT */
#include "acq_client_core.h"

#endif
