#ifndef _ACQ_CHAN_H_
#define _ACQ_CHAN_H_

#if defined(__BOARD_ML605__)
#include "boards/ml605/priv_defs/acq_chan_ml605.h"
#elif defined(__BOARD_AFCV3__)
#include "boards/afcv3/priv_defs/acq_chan_afcv3.h"
#else
#error "Could not include acquisition channel definitions. Unsupported board!"
#endif

#endif
