/*
 * Copyright (C) 2015 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _BPM_SERVER_CLASSES_H_
#define _BPM_SERVER_CLASSES_H_

/* Set up environment for the application */
#include "bpm_server_prelude.h"

/* External dependencies */
#include <bsmp/client.h>
#include <convc.h>
#include <disptable.h>
#include <errhand.h>
#include <hutils.h>
#include <ll_io.h>

/* Internal libraries dependencies */
#include "libsdbfs.h"
#include "hal_stddef.h"

/* General dependencies */
#include "board.h"
#include "epics_mapping.h"
#include "revision.h"
#include "acq_chan_gen_defs.h"

/* BPM version macros for compile-time API detection */

#define BPM_VERSION_MAJOR 0
#define BPM_VERSION_MINOR 1
#define BPM_VERSION_PATCH 0

#define BPM_MAKE_VERSION(major, minor, patch) \
    ((major) * 10000 + (minor) * 100 + (patch))
#define BPM_VERSION \
    BPM_MAKE_VERSION(BPM_VERSION_MAJOR, BPM_VERSION_MINOR, BPM_VERSION_PATCH)

#if defined (__WINDOWS__)
#   if defined LIBBPM_STATIC
#       define BPM_EXPORT
#   elif defined LIBBPM_EXPORTS
#       define BPM_EXPORT __declspec(dllexport)
#   else
#       define BPM_EXPORT __declspec(dllimport)
#   endif
#else
#   define BPM_EXPORT
#endif

/* Opaque class structures to allow forward references */

/* Forward dmngr_err_e declaration enumeration */
typedef enum _dmngr_err_e dmngr_err_e;
/* Opaque devio_info_t structure */
typedef struct _devio_info_t devio_info_t;
/* Opaque dmngr_t class structure */
typedef struct _dmngr_t dmngr_t;

/* Forward devio_err_e declaration enumeration */
typedef enum _devio_err_e devio_err_e;
/* Forward devio_type_e declaration enumeration */
typedef enum _devio_type_e devio_type_e;
/* Opaque devio_t structure */
typedef struct _devio_t devio_t;

/* Forward smpr_err_e declaration enumeration */
typedef enum _smpr_err_e smpr_err_e;
/* Opaque sm_pr_t structure */
typedef struct _smpr_t smpr_t;

/* Forward smch_err_e declaration enumeration */
typedef enum _smch_err_e smch_err_e;
/* Opaque sm_ch_24aa64_t structure */
typedef struct _smch_24aa64_t smch_24aa64_t;
/* Opaque sm_ch_ad9510_t structure */
typedef struct _smch_ad9510_t smch_ad9510_t;
/* Opaque sm_ch_pca9547_t structure */
typedef struct _smch_pca9547_t smch_pca9547_t;
/* Opaque sm_ch_si57x_t structure */
typedef struct _smch_si57x_t smch_si57x_t;
/* Opaque sm_ch_rffe_t structure */
typedef struct _smch_rffe_t smch_rffe_t;

/* Forward smio_err_e declaration enumeration */
typedef enum _smio_err_e smio_err_e;
/* Opaque smio_t structure */
typedef struct _smio_t smio_t;

/* Forward msg_err_e declaration enumeration */
typedef enum _msg_err_e msg_err_e;
/* Forward exp_msg_t declaration structure */
typedef struct _exp_msg_zmq_t exp_msg_zmq_t;
/* Forward zmq_server_args_t declaration structure */
typedef struct _zmq_server_args_t zmq_server_args_t;

/* Public API classes */

/* DEV MNGR */
#include "dev_mngr_err.h"
#include "dev_mngr_dev_info.h"
#include "dev_mngr_core.h"

/* DEV_IO */
#include "dev_io_err.h"
#include "dev_io_utils.h"
#include "dev_io_exports.h"
#include "dev_io_core.h"
#include "dev_io.h"

/* SM_PR */
#include "sm_pr_err.h"
#include "sm_pr.h"
#include "sm_pr_spi.h"
#include "hw/wb_spi_regs.h"
#include "protocols/smpr_spi_regs.h"
#include "sm_pr_i2c.h"
#include "hw/wb_i2c_regs.h"
#include "protocols/smpr_i2c_regs.h"
#include "sm_pr_bsmp.h"

/* SM_CH */
#include "sm_ch_err.h"
#include "sm_ch_24aa64.h"
#include "chips/e24aa64_regs.h"
#include "sm_ch_ad9510.h"
#include "chips/ad9510_regs.h"
#include "sm_ch_pca9547.h"
#include "chips/pca9547_regs.h"
#include "sm_ch_si57x.h"
#include "chips/si57x_regs.h"
#include "sm_ch_rffe.h"

/* SM_IO */
#include "sm_io_err.h"
#include "sm_io_exports.h"
#include "sm_io_thsafe_codes.h"
#include "sm_io_bootstrap.h"
#include "sm_io_mod_dispatch.h"
#include "sm_io.h"

/* MSG */
#include "msg_macros.h"
#include "msg_err.h"
/* MSG EXP ops */
#include "exp_ops_codes.h"
#include "exp_msg_zmq.h"
/* MSG SMIO THSAFE ops */
#include "smio_thsafe_zmq_server.h"
#include "smio_thsafe_zmq_client.h"
/* General MSG */
#include "thsafe_msg_zmq.h"
#include "msg.h"

/* Other headers */
#include "bpm_client.h"
#include "rw_param.h"
#include "rw_param_codes.h"

#endif
