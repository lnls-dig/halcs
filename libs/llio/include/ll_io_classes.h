/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _LL_IO_CLASSES_H_
#define _LL_IO_CLASSES_H_

/* Set up environment for the application */
#include "ll_io_prelude.h"

/* External dependencies */
#include <czmq.h>
#include <errhand.h>
#include <convc.h>

/* HALCS version macros for compile-time API detection */

#define LL_IO_VERSION_MAJOR 2
#define LL_IO_VERSION_MINOR 0
#define LL_IO_VERSION_PATCH 0

#define LL_IO_MAKE_VERSION(major, minor, patch) \
    ((major) * 10000 + (minor) * 100 + (patch))
#define LL_IO_VERSION \
    LL_IO_MAKE_VERSION(LL_IO_VERSION_MAJOR, LL_IO_VERSION_MINOR, \
            LL_IO_VERSION_PATCH)

#if defined (__WINDOWS__)
#   if defined LIBLL_IO_STATIC
#       define LL_IO_EXPORT
#   elif defined LIBLL_IO_EXPORTS
#       define LL_IO_EXPORT __declspec(dllexport)
#   else
#       define LL_IO_EXPORT __declspec(dllimport)
#   endif
#else
#   define LL_IO_EXPORT
#endif

/* Opaque class structures to allow forward references */

/* Public API classes */
/* Opaque llio_dev_info_t structure */
typedef struct _llio_dev_info_t llio_dev_info_t;
/* Opaque llio_endpoint_t structure */
typedef struct _llio_endpoint_t llio_endpoint_t;
/* Opaque llio_t structure */
typedef struct _llio_t llio_t;

/* LL_IO */
#include "ll_io_err.h"
#include "ll_io_utils.h"
#include "ll_io_dev_info.h"
#include "ll_io_endpoint.h"
#include "ll_io_core.h"

/* LL_IO operations */
#include "ll_io_pcie.h"
#include "ll_io_eth_utils.h"
#include "ll_io_eth.h"

#endif
