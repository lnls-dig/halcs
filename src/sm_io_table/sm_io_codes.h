/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifndef _SM_IO_CODES_H_
#define _SM_IO_CODES_H_

#include <inttypes.h>

/* Forward references */

/* Forward smio_acq_data_block_t declaration structure */
typedef struct _smio_acq_data_block_t smio_acq_data_block_t;
/* Forward smio_afc_diag_revision_data_t declaration structure */
typedef struct _smio_afc_diag_revision_data_t smio_afc_diag_revision_data_t;
/* Forward smio_dsp_data_t declaration structure */
typedef struct _smio_dsp_data_t smio_dsp_data_t;
/* Forward smio_dsp_monit_data_t declaration structure */
typedef struct _smio_dsp_monit_data_t smio_dsp_monit_data_t;
/* Forward smio_rffe_data_block_t declaration structure */
typedef struct _smio_rffe_data_block_t smio_rffe_data_block_t;
/* Forward smio_rffe_version_t declaration structure */
typedef struct _smio_rffe_version_t smio_rffe_version_t;
/* Forward smio_init_board_type_t declaration structure */
typedef struct _smio_init_board_type_t smio_init_board_type_t;

/* Include all module's codes */
#include "sm_io_fmc130m_4ch_codes.h"
#include "sm_io_fmc250m_4ch_codes.h"
#include "sm_io_fmc100m_4ch_codes.h"
#include "sm_io_fmcpico1m_4ch_codes.h"
#include "sm_io_fmc_adc_common_codes.h"
#include "sm_io_fmc_active_clk_codes.h"
#include "sm_io_acq_codes.h"
#include "sm_io_dsp_codes.h"
#include "sm_io_swap_codes.h"
#include "sm_io_rffe_codes.h"
#include "sm_io_afc_diag_codes.h"
#include "sm_io_trigger_iface_codes.h"
#include "sm_io_trigger_mux_codes.h"
#include "sm_io_afc_timing_codes.h"
#include "sm_io_acq_pm_codes.h"
#include "sm_io_init_codes.h"
#include "sm_io_afc_mgmt_codes.h"
#include "sm_io_tim_rcv_codes.h"
#include "sm_io_orbit_intlk_codes.h"
#include "sm_io_fofb_ctrl_codes.h"
#include "sm_io_rtmlamp_ohwr_codes.h"

/* Include all function descriptors */
#include "sm_io_fmc130m_4ch_exports.h"
#include "sm_io_fmc250m_4ch_exports.h"
#include "sm_io_fmc100m_4ch_exports.h"
#include "sm_io_fmcpico1m_4ch_exports.h"
#include "sm_io_fmc_adc_common_exports.h"
#include "sm_io_fmc_active_clk_exports.h"
#include "sm_io_acq_exports.h"
#include "sm_io_dsp_exports.h"
#include "sm_io_swap_exports.h"
#include "sm_io_rffe_exports.h"
#include "sm_io_afc_diag_exports.h"
#include "sm_io_trigger_iface_exports.h"
#include "sm_io_trigger_mux_exports.h"
#include "sm_io_afc_timing_exports.h"
#include "sm_io_acq_pm_exports.h"
#include "sm_io_init_exports.h"
#include "sm_io_afc_mgmt_exports.h"
#include "sm_io_tim_rcv_exports.h"
#include "sm_io_orbit_intlk_exports.h"
#include "sm_io_fofb_ctrl_exports.h"
#include "sm_io_rtmlamp_ohwr_exports.h"

/* Merge all function descriptors in a single structure */
extern const disp_op_t **smio_exp_ops [];

#endif
