/*
 * Copyright (C) 2016 LNLS (www.lnls.br)
 * Author: Janito V. F. F. <janito.filho@lnls.br>
 *
 * Released according to the GNU GPL, version 3 or any later version.
 */

#ifdef __BOARD_AFCV3_1__
#   include "boards/afcv3_1/acq_chan.h"
#elif defined __BOARD_AFCV3__
#   include "boards/afcv3/acq_chan.h"
#elif defined __BOARD_ML605__
#   include "boards/ml605/acq_chan.h"
#else
#   error "Board must be specified"
#endif
