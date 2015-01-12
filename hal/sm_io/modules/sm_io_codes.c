/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include "sm_io_codes.h"

const disp_op_t **smio_exp_ops [] = {
    acq_exp_ops,
    dsp_exp_ops,
    fmc130m_4ch_exp_ops,
    swap_exp_ops,
    rffe_exp_ops,
};

