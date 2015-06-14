/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _BPM_CLIENT_CLASSES_H_
#define _BPM_CLIENT_CLASSES_H_

/* Set up environment for the application */
#include "bpm_client_prelude.h"

/* External dependencies */
#include <czmq.h>
#include <malamute.h>

/* Internal libraries dependencies */
#include "acq_chan.h"
#include "disp_table.h"
#include "sm_io_codes.h"

/* BPM version macros for compile-time API detection */

#define BPM_CLIENT_VERSION_MAJOR 0
#define BPM_CLIENT_VERSION_MINOR 0
#define BPM_CLIENT_VERSION_PATCH 1

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

/* Opaque class structures to allow forward references */

/* Public API classes */

/* Forward bpm_client_err_e declaration enumeration */
typedef enum _bpm_client_err_e bpm_client_err_e;
/* Opaque bpm_client_t structure */
typedef struct _bpm_client_t bpm_client_t;

/* BPM CLIENT */
#include "bpm_client_err.h"
#include "bpm_client_rw_param.h"
#include "bpm_client_core.h"

#endif
