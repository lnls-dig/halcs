/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _SM_IO_CODES_H_
#define _SM_IO_CODES_H_

/* Include all module's codes */
#include "sm_io_fmc130m_4ch_codes.h"
#include "sm_io_acq_codes.h"
#include "sm_io_dsp_codes.h"
#include "sm_io_swap_codes.h"
#include "sm_io_rffe_codes.h"
#include "sm_io_afc_diag_codes.h"

/* Include all function descriptors */
#include "sm_io_fmc130m_4ch_exports.h"
#include "sm_io_acq_exports.h"
#include "sm_io_dsp_exports.h"
#include "sm_io_swap_exports.h"
#include "sm_io_rffe_exports.h"
#include "sm_io_afc_diag_exports.h"

/* Merge all function descriptors in a single structure */
extern const disp_op_t **smio_exp_ops [];

#endif
