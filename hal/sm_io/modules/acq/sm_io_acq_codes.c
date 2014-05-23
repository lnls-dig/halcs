/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#include <sm_io_acq_codes.h>

const uint32_t acq_reply_ops [ACQ_REPLY_END] = {
    [0] = ACQ_REQ_OK,
    [1] = ACQ_REQ_ERR,
	[2] = ACQ_CHECK_OK,
	[3] = ACQ_CHECK_ERR,
	[4] = ACQ_BLOCK_OK,
    [5] = ACQ_BLOCK_ERR
};

