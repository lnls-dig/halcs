/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#include "bpm_server.h"
/* Private headers. Include all available module we can handle */
#include "fmc130m_4ch/sm_io_fmc130m_4ch_exp.h"
#include "acq/sm_io_acq_exp.h"
#include "dsp/sm_io_dsp_exp.h"
#include "swap/sm_io_swap_exp.h"
#include "rffe/sm_io_rffe_exp.h"

#if defined (__BOARD_AFCV3__)
#include "afc_diag/sm_io_afc_diag_exp.h"
#endif

/* Table of all known modules we can handle */
const smio_mod_dispatch_t smio_mod_dispatch [] = {
    {
        .id = FMC130M_4CH_SDB_DEVID,
        .name = FMC130M_4CH_SDB_NAME,
        .bootstrap_ops = &fmc130m_4ch_bootstrap_ops
    },
    {
        .id = ACQ_SDB_DEVID,
        .name = ACQ_SDB_NAME,
        .bootstrap_ops = &acq_bootstrap_ops
    },
    {
        .id = DSP_SDB_DEVID,
        .name = DSP_SDB_NAME,
        .bootstrap_ops = &dsp_bootstrap_ops
    },
    {
        .id = SWAP_SDB_DEVID,
        .name = SWAP_SDB_NAME,
        .bootstrap_ops = &swap_bootstrap_ops
    },
    {
        .id = RFFE_DEVID, /* No SDB as this is not an FPGA module */
        .name = RFFE_NAME,
        .bootstrap_ops = &rffe_bootstrap_ops
    },
#if defined (__BOARD_AFCV3__)
    {
        .id = AFC_DIAG_DEVID,
        .name = AFC_DIAG_NAME,
        .bootstrap_ops = &afc_diag_bootstrap_ops
    },
#endif
    {
        .id = SMIO_DISPATCH_END_ID,
        .name = "SMIO_DISPATCH_END",
        .bootstrap_ops = NULL
    }
};
