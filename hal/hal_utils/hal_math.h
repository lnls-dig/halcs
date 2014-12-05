/*
 * Copyright (C) 2014 LNLS (www.lnls.br)
 * Author: Lucas Russo <lucas.russo@lnls.br>
 *
 * Released according to the GNU LGPL, version 3 or any later version.
 */

#ifndef _HAL_MATH_H_
#define _HAL_MATH_H_

#include <math.h>
#include <inttypes.h>

#define POW_2_28                268435456.0

#define FLOORF(value)           floorf (value)
#define CEILF(value)            ceilf (value)

#define FLOOR(value)            floor (value)
#define CEIL(value)             ceil (value)

uint64_t div_u64_rem (uint64_t dividend, uint32_t divisor,
 uint32_t *remainder);
uint64_t div_u64 (uint64_t dividend, uint32_t divisor);
uint64_t div64_u64 (uint64_t dividend, uint64_t divisor);
int64_t div64_s64 (int64_t dividend, int64_t divisor);

#endif
