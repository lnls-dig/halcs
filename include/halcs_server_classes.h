/*
 * Copyright (C) 2015 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _HALCS_SERVER_CLASSES_H_
#define _HALCS_SERVER_CLASSES_H_

/* Set up environment for the application */
#include "halcs_server_prelude.h"

/* External dependencies */
#include <bsmp/client.h>
#include <convc.h>
#include <disptable.h>
#include <errhand.h>
#include <hutils.h>
#include <sdbutils.h>
#include <ll_io.h>

/* Internal libraries dependencies */
#include "libsdbfs.h"
#include "hal_stddef.h"

/* General dependencies */
#include "board.h"
#include "mem_layout_common.h"
#include "chips_addr_common.h"
#include "device_mapping.h"
#include "revision.h"
#include "acq_chan_gen_defs.h"
#include "ddr3_map_structs.h"

#if defined (__WINDOWS__)
#   if defined LIBHALCS_STATIC
#       define HALCS_EXPORT
#   elif defined LIBHALCS_EXPORTS
#       define HALCS_EXPORT __declspec(dllexport)
#   else
#       define HALCS_EXPORT __declspec(dllimport)
#   endif
#else
#   define HALCS_EXPORT
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
/* Opaque devio_proto_t structure */
typedef struct _devio_proto_t devio_proto_t;
/* Opaque devio_t structure */
typedef struct _devio_t devio_t;

/* Forward smpr_err_e declaration enumeration */
typedef enum _smpr_err_e smpr_err_e;
/* Opaque sm_pr_t structure */
typedef struct _smpr_t smpr_t;

/* Opaque smpr_spi_t structure */
typedef struct _smpr_spi_t smpr_spi_t;
/* Opaque smpr_i2c_t structure */
typedef struct _smpr_i2c_t smpr_i2c_t;
/* Opaque smpr_bsmp_t structure */
typedef struct _smpr_bsmp_t smpr_bsmp_t;
/* Opaque smpr_scpi_t structure */
typedef struct _smpr_scpi_t smpr_scpi_t;

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
/* Opaque sm_ch_rffe_scpi_t structure */
typedef struct _smch_rffe_scpi_t smch_rffe_scpi_t;
/* Opaque sm_ch_isla216p_t structure */
typedef struct _smch_isla216p_t smch_isla216p_t;
/* Opaque sm_ch_ltc2174_t structure */
typedef struct _smch_ltc2174_t smch_ltc2174_t;


/* Forward declaration smio_mod_dispatch_t declaration structure */
typedef struct _smio_mod_dispatch_t smio_mod_dispatch_t;
/* Forward smio_err_e declaration enumeration */
typedef enum _smio_err_e smio_err_e;
/* Opaque smio_t structure */
typedef struct _smio_t smio_t;
/* Opaque smio_cfg_t structure */
typedef struct _smio_cfg_t smio_cfg_t;

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
#include "dev_io_proto.h"
#include "dev_io_core.h"

/* SM_PR */
#include "sm_pr_err.h"
#include "sm_pr.h"
#include "sm_pr_spi.h"
#include "hw/wb_spi_regs.h"
#include "protocols/smpr_spi_regs.h"
#include "protocols/sm_pr_spi_defaults.h"
#include "sm_pr_i2c.h"
#include "hw/wb_i2c_regs.h"
#include "protocols/smpr_i2c_regs.h"
#include "protocols/sm_pr_i2c_defaults.h"
#include "sm_pr_bsmp.h"
#include "protocols/sm_pr_bsmp_defaults.h"
#include "sm_pr_scpi.h"

/* SM_CH */
#include "sm_ch_err.h"
#include "sm_ch_24aa64.h"
#include "chips/e24aa64_regs.h"
#include "sm_ch_ad9510.h"
#include "chips/ad9510_regs.h"
#include "chips/sm_ch_ad9510_defaults.h"
#include "sm_ch_pca9547.h"
#include "chips/pca9547_regs.h"
#include "sm_ch_si57x.h"
#include "chips/si57x_regs.h"
#include "chips/sm_ch_si57x_defaults.h"
#include "sm_ch_isla216p.h"
#include "chips/isla216p_regs.h"
#include "chips/sm_ch_isla216p_defaults.h"
#include "sm_ch_rffe.h"
#include "chips/sm_ch_rffe_defaults.h"
#include "sm_ch_rffe_scpi.h"
//#include "chips/sm_ch_rffe_scpi_defaults.h"
#include "sm_ch_ltc2174.h"
#include "chips/ltc2174_regs.h"
//#include "chips/sm_ch_ltc2174_defaults.h"

/* SM_IO */
#include "sm_io_err.h"
#include "sm_io_exports.h"
#include "sm_io_thsafe_codes.h"
#include "sm_io_bootstrap.h"
#include "sm_io_mod_dispatch.h"
#include "sm_io.h"
#include "sm_io_cfg.h"

/* MSG */
#include "msg_macros.h"
#include "str_msg_macros.h"
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
#include "halcs_client.h"
#include "rw_param.h"
#include "rw_param_codes.h"

#endif
