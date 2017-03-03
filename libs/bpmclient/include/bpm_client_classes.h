/*
 * Copyright (C) 2017 LNLS (www.lnls.br)
 * Author: Janito Vaqueiro Ferreira Filho <janito.filho@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _BPM_CLIENT_CLASSES_H_
#define _BPM_CLIENT_CLASSES_H_

/* Set up environment for the application */
#include "bpm_client_prelude.h"

/* External dependencies */
#include <malamute.h>
#include <errhand.h>
#include <hutils.h>
#include <disptable.h>
#include <halcs_client.h>
#include <acq_client.h>

/* version macros for compile-time API detection */

#define BPM_CLIENT_VERSION_MAJOR 0
#define BPM_CLIENT_VERSION_MINOR 1
#define BPM_CLIENT_VERSION_PATCH 0

#define BPM_CLIENT_MAKE_VERSION(major, minor, patch) \
    ((major) * 10000 + (minor) * 100 + (patch))
#define BPM_CLIENT_VERSION \
    BPM_CLIENT_MAKE_VERSION(BPM_CLIENT_VERSION_MAJOR, BPM_CLIENT_VERSION_MINOR, \
            BPM_CLIENT_VERSION_PATCH)

#if defined (__WINDOWS__)
#   if defined LIBBPM_CLIENT_STATIC
#       define BPM_CLIENT_EXPORT
#   elif defined LIBBPM_CLIENT_EXPORTS
#       define BPM_CLIENT_EXPORT __declspec(dllexport)
#   else
#       define BPM_CLIENT_EXPORT __declspec(dllimport)
#   endif
#else
#   define BPM_CLIENT_EXPORT
#endif

/* Opaque bpm_client_t structure */
typedef struct _bpm_single_pass_t bpm_single_pass_t;

/* BPM CLIENT */
#include "bpm_client_core.h"

#endif
